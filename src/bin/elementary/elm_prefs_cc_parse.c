#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <regex.h>

#include "elm_prefs_cc.h"
#include <Ecore.h>
#include <Ecore_File.h>

#ifdef _WIN32
# define EPP_EXT ".exe"
#else
# define EPP_EXT
#endif

static void        new_object(void);
static void        new_statement(void);
static char       *perform_math(char *input);
static int         isdelim(char c);
static char       *next_token(char *p, char *end, char **new_p, int *delim);
static const char *stack_id(void);
static void        parse(char *data, off_t size);

/* simple expression parsing protos */
static int         my_atoi(const char *s);
static char       *_alphai(char *s, int *val);
static char       *_betai(char *s, int *val);
static char       *_gammai(char *s, int *val);
static char       *_deltai(char *s, int *val);
static char       *_get_numi(char *s, int *val);
static int         _is_numi(char c);
static int         _is_op1i(char c);
static int         _is_op2i(char c);
static int         _calci(char op, int a, int b);

static double      my_atof(const char *s);
static char       *_alphaf(char *s, double *val);
static char       *_betaf(char *s, double *val);
static char       *_gammaf(char *s, double *val);
static char       *_deltaf(char *s, double *val);
static char       *_get_numf(char *s, double *val);
static int         _is_numf(char c);
static int         _is_op1f(char c);
static int         _is_op2f(char c);
static double      _calcf(char op, double a, double b);
static int         strstrip(const char *in, char *out, size_t size);

int line = 0;
Eina_List *stack = NULL;
Eina_List *params = NULL;

static char file_buf[4096];
static int verbatim = 0;
static int verbatim_line1 = 0;
static int verbatim_line2 = 0;
static char *verbatim_str = NULL;
static Eina_Strbuf *stack_buf = NULL;

static void
err_show_stack(void)
{
   const char *s;

   s = stack_id();
   if (s)
     ERR("PARSE STACK:\n%s", s);
   else
     ERR("NO PARSE STACK");
}

static void
err_show_params(void)
{
   Eina_List *l;
   char *p;

   ERR("PARAMS:");
   EINA_LIST_FOREACH(params, l, p)
     {
        ERR("  %s", p);
     }
}

static void
err_show(void)
{
   err_show_stack();
   err_show_params();
}

static char *
_parse_param_get(int n)
{
   if (n < (int) eina_list_count(params))
     return eina_list_nth(params, n);
   return NULL;
}

static Eina_Hash *_new_object_hash = NULL;
static Eina_Hash *_new_statement_hash = NULL;

static void
fill_object_statement_hashes(void)
{
   int i, n;

   if (_new_object_hash) return;

   _new_object_hash = eina_hash_string_superfast_new(NULL);
   _new_statement_hash = eina_hash_string_superfast_new(NULL);

   n = object_handler_num();
   for (i = 0; i < n; i++)
     {
        eina_hash_direct_add(_new_object_hash, object_handlers[i].type,
                             &(object_handlers[i]));
     }
   n = statement_handler_num();
   for (i = 0; i < n; i++)
     {
        eina_hash_direct_add(_new_statement_hash, statement_handlers[i].type,
                             &(statement_handlers[i]));
     }
}

static void
new_object(void)
{
   const char *id;
   New_Object_Handler *oh;
   New_Statement_Handler *sh;

   fill_object_statement_hashes();
   id = stack_id();
   oh = eina_hash_find(_new_object_hash, id);
   if (oh)
     {
        if (oh->func) oh->func();
     }
   else
     {
        sh = eina_hash_find(_new_statement_hash, id);
        if (!sh)
          {
             ERR("%s:%i unhandled keyword %s",
                 file_in, line - 1,
                 (char *)eina_list_data_get(eina_list_last(stack)));
             err_show();
             exit(-1);
          }
     }
}

static void
new_statement(void)
{
   const char *id;
   New_Statement_Handler *sh;

   fill_object_statement_hashes();
   id = stack_id();
   sh = eina_hash_find(_new_statement_hash, id);
   if (sh)
     {
        if (sh->func) sh->func();
     }
   else
     {
        ERR("%s:%i unhandled keyword %s",
            file_in, line - 1,
            (char *)eina_list_data_get(eina_list_last(stack)));
        err_show();
        exit(-1);
     }
}

static char *
perform_math(char *input)
{
   char buf[256];
   double res;

   /* FIXME
    * Always apply floating-point arithmetic.
    * Does this cause problems for integer parameters? (yes it will)
    *
    * What we should do is, loop over the string and figure out whether
    * there are floating point operands, too and then switch to
    * floating point math.
    */
   res = my_atof(input);
   snprintf(buf, sizeof (buf), "%lf", res);
   return strdup(buf);
}

static int
isdelim(char c)
{
   const char *delims = "{},;:";
   char *d;

   d = (char *)delims;
   while (*d)
     {
        if (c == *d) return 1;
        d++;
     }
   return 0;
}

static char *
next_token(char *p, char *end, char **new_p, int *delim)
{
   char *tok_start = NULL, *tok_end = NULL, *tok = NULL, *sa_start = NULL;
   int in_tok = 0;
   int in_quote = 0;
   int in_parens = 0;
   int in_comment_ss = 0;
   int in_comment_cpp = 0;
   int in_comment_sa = 0;
   int had_quote = 0;
   int is_escaped = 0;

   *delim = 0;
   if (p >= end) return NULL;
   while (p < end)
     {
        if (*p == '\n')
          {
             in_comment_ss = 0;
             in_comment_cpp = 0;
             line++;
          }
        if ((!in_comment_ss) && (!in_comment_sa))
          {
             if ((!in_quote) && (*p == '/') && (p < (end - 1)) && (*(p + 1) == '/'))
               in_comment_ss = 1;
             if ((!in_quote) && (*p == '#'))
               in_comment_cpp = 1;
             if ((!in_quote) && (*p == '/') && (p < (end - 1)) && (*(p + 1) == '*'))
               {
                  in_comment_sa = 1;
                  sa_start = p;
               }
          }
        if ((in_comment_cpp) && (*p == '#'))
          {
             char *pp, fl[4096];
             char *tmpstr = NULL;
             int l, nm;

             /* handle cpp comments */
             /* their line format is
              * #line <line no. of next line> <filename from next line on> [??]
              */

             pp = p;
             while ((pp < end) && (*pp != '\n'))
               {
                  pp++;
               }
             l = pp - p;
             tmpstr = alloca(l + 1);
             if (!tmpstr)
               {
                  ERR("%s:%i malloc %i bytes failed",
                      file_in, line - 1, l + 1);
                  exit(-1);
               }
             memcpy(tmpstr, p, l);
             tmpstr[l] = 0;
             if (l >= (int)sizeof(fl))
               {
                  ERR("Line too long: %i chars: %s", l, tmpstr);
                  exit(-1);
               }
             l = sscanf(tmpstr, "%*s %i \"%[^\"]\"", &nm, fl);
             if (l == 2)
               {
                  strcpy(file_buf, fl);
                  line = nm;
                  file_in = file_buf;
               }
          }
        else if ((!in_comment_ss) && (!in_comment_sa) && (!in_comment_cpp))
          {
             if (!in_tok)
               {
                  if (!in_quote)
                    {
                       if (!isspace(*p))
                         {
                            if (*p == '"')
                              {
                                 in_quote = 1;
                                 had_quote = 1;
                              }
                            else if (*p == '(')
                              in_parens++;

                            in_tok = 1;
                            tok_start = p;
                            if (isdelim(*p)) *delim = 1;
                         }
                    }
               }
             else
               {
                  if (in_quote)
                    {
                       if ((*p) == '\\')
                         is_escaped = !is_escaped;
                       else if (((*p) == '"') && (!is_escaped))
                         {
                            in_quote = 0;
                            had_quote = 1;
                         }
                       else if (is_escaped)
                         is_escaped = 0;
                    }
                  else if (in_parens)
                    {
                       if (((*p) == ')') && (!is_escaped))
                         in_parens--;
                    }
                  else
                    {
                       if (*p == '"')
                         {
                            in_quote = 1;
                            had_quote = 1;
                         }
                       else if (*p == '(')
                         in_parens++;

                       /* check for end-of-token */
                       if (
                         (isspace(*p)) ||
                         ((*delim) && (!isdelim(*p))) ||
                         (isdelim(*p))
                         ) /*the line below this is never  used because it skips to
                            * the 'done' label which is after the return call for
                            * in_tok being 0. is this intentional?
                            */
                         {
                            in_tok = 0;

                            tok_end = p - 1;
                            if (*p == '\n') line--;
                            goto done;
                         }
                    }
               }
          }
        if (in_comment_sa)
          {
             if ((*p == '/') && (*(p - 1) == '*') && ((p - sa_start) > 2))
               in_comment_sa = 0;
          }
        p++;
     }
   if (!in_tok) return NULL;
   tok_end = p - 1;

done:
   *new_p = p;

   tok = mem_alloc(tok_end - tok_start + 2);
   if (!tok) return NULL;
   strncpy(tok, tok_start, tok_end - tok_start + 1);
   tok[tok_end - tok_start + 1] = 0;

   if (had_quote)
     {
        is_escaped = 0;
        p = tok;

        while (*p)
          {
             if ((*p == '\"') && (!is_escaped))
               {
                  memmove(p, p + 1, strlen(p));
               }
             else if ((*p == '\\') && (*(p + 1) == 'n'))
               {
                  memmove(p, p + 1, strlen(p));
                  *p = '\n';
               }
             else if ((*p == '\\') && (*(p + 1) == 't'))
               {
                  memmove(p, p + 1, strlen(p));
                  *p = '\t';
               }
             else if (*p == '\\')
               {
                  memmove(p, p + 1, strlen(p));
                  if (*p == '\\') p++;
                  else is_escaped = 1;
               }
             else
               {
                  if (is_escaped) is_escaped = 0;
                  p++;
               }
          }
     }
   else if (*tok == '(')
     {
        char *tmp;
        tmp = tok;
        tok = perform_math(tok);
        free(tmp);
     }

   return tok;
}

static void
stack_push(char *token)
{
   if (stack) eina_strbuf_append(stack_buf, ".");
   eina_strbuf_append(stack_buf, token);

   stack = eina_list_append(stack, token);
}

static void
stack_pop(void)
{
   char *top;
   int top_length;

   if (!stack)
     {
        ERR("parse error %s:%i. } marker without matching { marker",
            file_in, line - 1);

        err_show();
        exit(-1);
     }

   top = eina_list_data_get(eina_list_last(stack));
   top_length = strlen(top);

   stack = eina_list_remove_list(stack, eina_list_last(stack));

   if (eina_list_count(stack)) top_length++;  // remove '.' as well.

   eina_strbuf_remove(stack_buf,
                      eina_strbuf_length_get(stack_buf) - top_length,
                      eina_strbuf_length_get(stack_buf));

   free(top);
}

static const char *
stack_id(void)
{
   return eina_strbuf_string_get(stack_buf);
}

static void
parse(char *data, off_t size)
{
   char *p, *end, *token;
   int delim = 0;
   int do_params = 0;

   DBG("Parsing input file");

   p = data;
   end = data + size;
   line = 1;
   while ((token = next_token(p, end, &p, &delim)))
     {
        /* if we are in param mode, the only delimiter
         * we'll accept is the semicolon
         */
        if (do_params && delim && *token != ';')
          {
             ERR("parse error %s:%i. %c marker before ; marker",
                 file_in, line - 1, *token);
             err_show();
             exit(-1);
          }
        else if (delim)
          {
             if (*token == ',' || *token == ':') do_params = 1;
             else if (*token == '}')
               {
                  if (do_params)
                    {
                       ERR("Parse error %s:%i. } marker before ; marker",
                           file_in, line - 1);
                       err_show();
                       exit(-1);
                    }
                  else
                    stack_pop();
               }
             else if (*token == ';')
               {
                  if (do_params)
                    {
                       do_params = 0;
                       new_statement();
                       /* clear out params */
                       while (params)
                         {
                            free(eina_list_data_get(params));
                            params = eina_list_remove(params, eina_list_data_get(params));
                         }
                       /* remove top from stack */
                       stack_pop();
                    }
               }
             else if (*token == '{')
               {
                  if (do_params)
                    {
                       ERR("parse error %s:%i. { marker before ; marker",
                           file_in, line - 1);
                       err_show();
                       exit(-1);
                    }
               }
             free(token);
          }
        else
          {
             if (do_params)
               params = eina_list_append(params, token);
             else
               {
                  stack_push(token);
                  new_object();
                  if ((verbatim == 1) && (p < (end - 2)))
                    {
                       int escaped = 0;
                       int inquotes = 0;
                       int insquotes = 0;
                       int squigglie = 1;
                       int l1 = 0, l2 = 0;
                       char *verbatim_1;
                       char *verbatim_2;

                       l1 = line;
                       while ((p[0] != '{') && (p < end))
                         {
                            if (*p == '\n') line++;
                            p++;
                         }
                       p++;
                       verbatim_1 = p;
                       verbatim_2 = NULL;
                       for (; p < end; p++)
                         {
                            if (*p == '\n') line++;
                            if (escaped) escaped = 0;
                            if (!escaped)
                              {
                                 if (p[0] == '\\') escaped = 1;
                                 else if (p[0] == '\"')
                                   {
                                      if (!insquotes)
                                        {
                                           if (inquotes) inquotes = 0;
                                           else inquotes = 1;
                                        }
                                   }
                                 else if (p[0] == '\'')
                                   {
                                      if (!inquotes)
                                        {
                                           if (insquotes) insquotes = 0;
                                           else insquotes = 1;
                                        }
                                   }
                                 else if ((!inquotes) && (!insquotes))
                                   {
                                      if (p[0] == '{') squigglie++;
                                      else if (p[0] == '}')
                                        squigglie--;
                                      if (squigglie == 0)
                                        {
                                           verbatim_2 = p - 1;
                                           l2 = line;
                                           break;
                                        }
                                   }
                              }
                         }
                       if (verbatim_2 > verbatim_1)
                         {
                            int l;
                            char *v;

                            l = verbatim_2 - verbatim_1 + 1;
                            v = malloc(l + 1);
                            strncpy(v, verbatim_1, l);
                            v[l] = 0;
                            set_verbatim(v, l1, l2);
                         }
                       else
                         {
                            ERR("Parse error %s:%i. { marker does not have matching } marker",
                                file_in, line - 1);
                            err_show();
                            exit(-1);
                         }
                       new_object();
                       verbatim = 0;
                    }
               }
          }
     }

   DBG("Parsing done");
}

void
set_verbatim(char *s, int l1, int l2)
{
   verbatim_line1 = l1;
   verbatim_line2 = l2;
   verbatim_str = s;
}

void
compile(void)
{
   int fd;
   off_t size;
   char *data;

   fd = open(file_in, O_RDONLY | O_BINARY, S_IRUSR | S_IWUSR);
   if (fd < 0)
     {
        ERR("Cannot open file \"%s\" for input. %s",
            file_in, strerror(errno));
        exit(-1);
     }
   DBG("Opening \"%s\" for input", file_in);

   size = lseek(fd, 0, SEEK_END);
   if (size <= 0)
     {
        ERR("lseek failed");
        close(fd);
        return;
     }
   lseek(fd, 0, SEEK_SET);
   data = malloc(size);
   if (data && (read(fd, data, size) == size))
     {
        stack_buf = eina_strbuf_new();
        parse(data, size);
        eina_strbuf_free(stack_buf);
        stack_buf = NULL;
     }
   else
     {
        ERR("Cannot read file \"%s\". %s", file_in, strerror(errno));
        exit(-1);
     }
   free(data);
   close(fd);
}

char *
parse_str(int n)
{
   char *str;
   char *s;

   str = eina_list_nth(params, n);
   if (!str)
     {
        ERR("%s:%i no parameter supplied as argument %i",
            file_in, line - 1, n + 1);
        err_show();
        exit(-1);
     }
   s = mem_strdup(str);
   return s;
}

static int
_parse_enum(char *str, va_list va)
{
   va_list va2;
   va_copy(va2, va); /* iterator for the error message */

   for (;; )
     {
        char *s;
        int v;

        s = va_arg(va, char *);

        /* End of the list, nothing matched. */
        if (!s)
          {
             ERR("%s:%i token %s not one of:", file_in, line - 1, str);
             s = va_arg(va2, char *);
             while (s)
               {
                  va_arg(va2, int);
                  fprintf(stderr, " %s", s);
                  s = va_arg(va2, char *);
                  if (!s) break;
               }
             fprintf(stderr, "\n");
             va_end(va2);
             va_end(va);
             err_show();
             exit(-1);
          }

        v = va_arg(va, int);
        if (!strcmp(s, str))
          {
             va_end(va2);
             va_end(va);
             return v;
          }
     }
   va_end(va2);
   va_end(va);
   return 0;
}

int
parse_enum(int n, ...)
{
   char *str;
   int result;
   va_list va;

   str = eina_list_nth(params, n);
   if (!str)
     {
        ERR("%s:%i no parameter supplied as argument %i",
            file_in, line - 1, n + 1);
        err_show();
        exit(-1);
     }

   va_start(va, n);
   result = _parse_enum(str, va);
   va_end(va);

   return result;
}

int
parse_int(int n)
{
   char *str;
   int i;

   str = eina_list_nth(params, n);
   if (!str)
     {
        ERR("%s:%i no parameter supplied as argument %i",
            file_in, line - 1, n + 1);
        err_show();
        exit(-1);
     }
   i = my_atoi(str);
   return i;
}

int
parse_int_range(int n, int f, int t)
{
   char *str;
   int i;

   str = eina_list_nth(params, n);
   if (!str)
     {
        ERR("%s:%i no parameter supplied as argument %i",
            file_in, line - 1, n + 1);
        err_show();
        exit(-1);
     }
   i = my_atoi(str);
   if ((i < f) || (i > t))
     {
        ERR("%s:%i integer %i out of range of %i to %i inclusive",
            file_in, line - 1, i, f, t);
        err_show();
        exit(-1);
     }
   return i;
}

int
parse_bool(int n)
{
   char *str, buf[4096];
   int i;

   str = eina_list_nth(params, n);
   if (!str)
     {
        ERR("%s:%i no parameter supplied as argument %i",
            file_in, line - 1, n + 1);
        err_show();
        exit(-1);
     }

   if (!strstrip(str, buf, sizeof (buf)))
     {
        ERR("%s:%i expression is too long",
            file_in, line - 1);
        return 0;
     }

   if (!strcasecmp(buf, "false") || !strcasecmp(buf, "off"))
     return 0;
   if (!strcasecmp(buf, "true") || !strcasecmp(buf, "on"))
     return 1;

   i = my_atoi(str);
   if ((i < 0) || (i > 1))
     {
        ERR("%s:%i integer %i out of range of 0 to 1 inclusive",
            file_in, line - 1, i);
        err_show();
        exit(-1);
     }
   return i;
}

double
parse_float(int n)
{
   char *str;
   double i;

   str = eina_list_nth(params, n);
   if (!str)
     {
        ERR("%s:%i no parameter supplied as argument %i",
            file_in, line - 1, n + 1);
        err_show();
        exit(-1);
     }
   i = my_atof(str);
   return i;
}

int
get_arg_count(void)
{
   return eina_list_count(params);
}

void
check_arg_count(int required_args)
{
   int num_args = eina_list_count(params);

   if (num_args != required_args)
     {
        ERR("%s:%i got %i arguments, but expected %i",
            file_in, line - 1, num_args, required_args);
        err_show();
        exit(-1);
     }
}

void
check_min_arg_count(int min_required_args)
{
   int num_args = eina_list_count(params);

   if (num_args < min_required_args)
     {
        ERR("%s:%i got %i arguments, but expected at least %i",
            file_in, line - 1, num_args, min_required_args);
        err_show();
        exit(-1);
     }
}

void
check_regex(const char *regex)
{
   int ret;
   char errbuf[1024];
   regex_t preg;

   ret = regcomp(&preg, regex, REG_EXTENDED | REG_NOSUB);
   if (ret)
     {
        regerror(ret, &preg, errbuf, 1024);
        ERR("%s:%i Invalid regular expression:\n"
            "%s", file_in, line, errbuf);
        err_show();
        exit(-1);
     }

   regfree(&preg);
}

/* simple expression parsing stuff */

/*
 * alpha ::= beta + beta || beta
 * beta  ::= gamma + gamma || gamma
 * gamma ::= num || delta
 * delta ::= '(' alpha ')'
 *
 */

/* int set of function */

static int
my_atoi(const char *s)
{
   int res = 0;
   char buf[4096];

   if (!s) return 0;
   if (!strstrip(s, buf, sizeof(buf)))
     {
        ERR("%s:%i expression is too long",
            file_in, line - 1);
        return 0;
     }
   _alphai(buf, &res);
   return res;
}

static char *
_deltai(char *s, int *val)
{
   if (!val) return NULL;
   if ('(' != s[0])
     {
        ERR("%s:%i unexpected character at %s",
            file_in, line - 1, s);
        return s;
     }
   else
     {
        s++;
        s = _alphai(s, val);
        s++;
        return s;
     }
   return s;
}

static char *
_funci(char *s, int *val)
{
   if (!strncmp(s, "floor(", 6))
     {
        s += 5;
        s = _deltai(s, val);
     }
   else if (!strncmp(s, "ceil(", 5))
     {
        s += 4;
        s = _deltai(s, val);
     }
   else
     {
        ERR("%s:%i unexpected character at %s",
            file_in, line - 1, s);
     }
   return s;
}

static char *
_gammai(char *s, int *val)
{
   if (!val) return NULL;
   if (_is_numi(s[0]))
     {
        s = _get_numi(s, val);
        return s;
     }
   else if ('(' == s[0])
     {
        s = _deltai(s, val);
        return s;
     }
   else
     {
        s = _funci(s, val);
//        ERR("%s:%i unexpected character at %s",
//                progname, file_in, line - 1, s);
     }
   return s;
}

static char *
_betai(char *s, int *val)
{
   int a1, a2;
   char op;

   if (!val) return NULL;
   s = _gammai(s, &a1);
   while (_is_op1i(s[0]))
     {
        op = s[0];
        s++;
        s = _gammai(s, &a2);
        a1 = _calci(op, a1, a2);
     }
   (*val) = a1;
   return s;
}

static char *
_alphai(char *s, int *val)
{
   int a1, a2;
   char op;

   if (!val) return NULL;
   s = _betai(s, &a1);
   while (_is_op2i(s[0]))
     {
        op = s[0];
        s++;
        s = _betai(s, &a2);
        a1 = _calci(op, a1, a2);
     }
   (*val) = a1;
   return s;
}

static char *
_get_numi(char *s, int *val)
{
   char buf[4096];
   int pos = 0;

   if (!val) return s;
   while ((('0' <= s[pos]) && ('9' >= s[pos])) ||
          ((0 == pos) && ('-' == s[pos])))
     {
        buf[pos] = s[pos];
        pos++;
     }
   buf[pos] = '\0';
   (*val) = atoi(buf);
   return s + pos;
}

static int
_is_numi(char c)
{
   if (((c >= '0') && (c <= '9')) || ('-' == c) || ('+' == c))
     return 1;
   else
     return 0;
}

static int
_is_op1i(char c)
{
   switch (c)
     {
      case '*':;

      case '%':;

      case '/': return 1;

      default: break;
     }
   return 0;
}

static int
_is_op2i(char c)
{
   switch (c)
     {
      case '+':;

      case '-': return 1;

      default: break;
     }
   return 0;
}

static int
_calci(char op, int a, int b)
{
   switch (op)
     {
      case '+':
        a += b;
        return a;

      case '-':
        a -= b;
        return a;

      case '/':
        if (0 != b) a /= b;
        else
          ERR("%s:%i divide by zero", file_in, line - 1);
        return a;

      case '*':
        a *= b;
        return a;

      case '%':
        if (0 != b) a = a % b;
        else
          ERR("%s:%i modula by zero", file_in, line - 1);
        return a;

      default:
        ERR("%s:%i unexpected character '%c'", file_in, line - 1, op);
     }
   return a;
}

/* float set of functoins */

static double
my_atof(const char *s)
{
   double res = 0;
   char buf[4096];

   if (!s) return 0;

   if (!strstrip(s, buf, sizeof (buf)))
     {
        ERR("%s:%i expression is too long", file_in, line - 1);
        return 0;
     }
   _alphaf(buf, &res);
   return res;
}

static char *
_deltaf(char *s, double *val)
{
   if (!val) return NULL;
   if ('(' != s[0])
     {
        ERR("%s:%i unexpected character at %s", file_in, line - 1, s);
        return s;
     }
   else
     {
        s++;
        s = _alphaf(s, val);
        s++;
     }
   return s;
}

static char *
_funcf(char *s, double *val)
{
   if (!strncmp(s, "floor(", 6))
     {
        s += 5;
        s = _deltaf(s, val);
        *val = floor(*val);
     }
   else if (!strncmp(s, "ceil(", 5))
     {
        s += 4;
        s = _deltaf(s, val);
        *val = ceil(*val);
     }
   else
     {
        ERR("%s:%i unexpected character at %s", file_in, line - 1, s);
     }
   return s;
}

static char *
_gammaf(char *s, double *val)
{
   if (!val) return NULL;

   if (_is_numf(s[0]))
     {
        s = _get_numf(s, val);
        return s;
     }
   else if ('(' == s[0])
     {
        s = _deltaf(s, val);
        return s;
     }
   else
     {
        s = _funcf(s, val);
//        ERR("%s:%i unexpected character at %s",
//                progname, file_in, line - 1, s);
     }
   return s;
}

static char *
_betaf(char *s, double *val)
{
   double a1 = 0, a2 = 0;
   char op;

   if (!val) return NULL;
   s = _gammaf(s, &a1);
   while (_is_op1f(s[0]))
     {
        op = s[0];
        s++;
        s = _gammaf(s, &a2);
        a1 = _calcf(op, a1, a2);
     }
   (*val) = a1;
   return s;
}

static char *
_alphaf(char *s, double *val)
{
   double a1 = 0, a2 = 0;
   char op;

   if (!val) return NULL;
   s = _betaf(s, &a1);
   while (_is_op2f(s[0]))
     {
        op = s[0];
        s++;
        s = _betaf(s, &a2);
        a1 = _calcf(op, a1, a2);
     }
   (*val) = a1;
   return s;
}

static char *
_get_numf(char *s, double *val)
{
   char buf[4096];
   int pos = 0;

   if (!val) return s;

   while ((('0' <= s[pos]) && ('9' >= s[pos])) ||
          ('.' == s[pos]) ||
          ((0 == pos) && ('-' == s[pos])))
     {
        buf[pos] = s[pos];
        pos++;
     }
   buf[pos] = '\0';
   (*val) = atof(buf);
   return s + pos;
}

int
params_min_check(int n)
{
   char *str;

   str = _parse_param_get(n);
   if (str) return 1;
   return 0;
}

static int
_is_numf(char c)
{
   if (((c >= '0') && (c <= '9'))
       || ('-' == c)
       || ('.' == c)
       || ('+' == c))
     return 1;
   return 0;
}

static int
_is_op1f(char c)
{
   switch (c)
     {
      case '*':;

      case '%':;

      case '/': return 1;

      default: break;
     }
   return 0;
}

static int
_is_op2f(char c)
{
   switch (c)
     {
      case '+':;

      case '-': return 1;

      default: break;
     }
   return 0;
}

static double
_calcf(char op, double a, double b)
{
   switch (op)
     {
      case '+':
        a += b;
        return a;

      case '-':
        a -= b;
        return a;

      case '/':
        if (EINA_DBL_NONZERO(b)) a /= b;
        else
          ERR("%s:%i divide by zero", file_in, line - 1);
        return a;

      case '*':
        a *= b;
        return a;

      case '%':
        if (EINA_DBL_NONZERO(b)) a = (double)((int)a % (int)b);
        else
          ERR("%s:%i modula by zero", file_in, line - 1);
        return a;

      default:
        ERR("%s:%i unexpected character '%c'", file_in, line - 1, op);
     }
   return a;
}

static int
strstrip(const char *in, char *out, size_t size)
{
   if ((size - 1) < strlen(in))
     {
        ERR("%s:%i expression is too long", file_in, line - 1);
        return 0;
     }
   /* remove spaces and tabs */
   while (*in)
     {
        if ((0x20 != *in) && (0x09 != *in))
          {
             *out = *in;
             out++;
          }
        in++;
     }
   *out = '\0';
   return 1;
}
