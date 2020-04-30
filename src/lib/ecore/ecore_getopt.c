#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef ENABLE_NLS
# include <libintl.h>
#else
# define gettext(x)          (x)
# define dgettext(domain, x) (x)
#endif

#define _(x)                 dgettext("ecore", x)

#include "Ecore.h"
#include "Ecore_Getopt.h"

static const char *prog = NULL;
static char **_argv = NULL;
static int _argc = 0;
static int cols = 80;
static int helpcol = 80 / 3;


static Eina_Bool _ecore_getopt_desc_is_sentinel(const Ecore_Getopt_Desc *desc);
static Ecore_Getopt_Desc_Arg_Requirement _ecore_getopt_desc_arg_requirement(const Ecore_Getopt_Desc *desc);
static void _ecore_getopt_help_desc_setup_metavar(const Ecore_Getopt_Desc *desc, char *metavar, int *metavarlen, int maxsize);


static void
_ecore_getopt_help_print_replace_program(FILE               *fp,
                                         const Ecore_Getopt *parser EINA_UNUSED,
                                         const char         *text)
{
   do
     {
        const char *d = strchr(text, '%');

        if (!d)
          {
             fputs(text, fp);
             break;
          }

        if (fwrite(text, 1, d - text, fp) != (size_t)(d - text))
          return;
        d++;
        if (strncmp(d, "prog", sizeof("prog") - 1) == 0)
          {
             fputs(prog ? prog : "???", fp);
             d += sizeof("prog") - 1;
          }
        else
          {
             if (d[0] == '%')
               d++;
             fputc('%', fp);
          }

        text = d;
     }
   while (text[0] != '\0');

   fputc('\n', fp);
}

static void
_ecore_getopt_version(FILE               *fp,
                      const Ecore_Getopt *parser)
{
   fputs(_("Version:"), fp);
   fputc(' ', fp);
   _ecore_getopt_help_print_replace_program(fp, parser, parser->version);
}

static void
_ecore_getopt_help_usage(FILE               *fp,
                         const Ecore_Getopt *parser)
{
   fputs(_("Usage:"), fp);
   fputc(' ', fp);

   if (!parser->usage)
     {
        const Ecore_Getopt_Desc *d;

        fprintf(fp, _("%s [options]"), prog);

        for (d = parser->descs; !_ecore_getopt_desc_is_sentinel(d); d++);

        if (d->metavar)
          {
             for (; d->metavar != NULL; d++)
               {
                  Ecore_Getopt_Desc_Arg_Requirement ar;
                  char metavar[32];
                  int metavarlen = 0;

                  ar = _ecore_getopt_desc_arg_requirement(d);
                  _ecore_getopt_help_desc_setup_metavar(d, metavar,
                                                        &metavarlen,
                                                        sizeof(metavar));

                  fputc(' ', fp);
                  if (ar != ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES)
                    fputc('[', fp);
                  fputs(metavar, fp);

                  if (ar != ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES)
                    fputc(']', fp);

                  if (d->action == ECORE_GETOPT_ACTION_APPEND)
                    {
                       fprintf(fp, " [%s] ...", metavar);
                       break;
                    }
               }
          }

        fputc('\n', fp);
        return;
     }

   _ecore_getopt_help_print_replace_program(fp, parser, gettext(parser->usage));
}

static int
_ecore_getopt_help_line(FILE       *fp,
                        const int   base,
                        const int   total,
                        int         used,
                        const char *text,
                        int         len)
{
   int linebreak = 0;
   do
     {
        /* process line considering spaces (new line and tabs are spaces!) */
         while ((used < total) && (len > 0))
           {
              const char *space = NULL;
              int i, todo;

              todo = total - used;
              if (todo > len)
                todo = len;

              for (i = 0; i < todo; i++)
                if (isspace((unsigned char)text[i]))
                  {
                     space = text + i;
                     break;
                  }

              if (space)
                {
                   i = fwrite(text, 1, i, fp);
                   i++;
                   text += i;
                   len -= i;
                   used += i;

                   /* if we start the line (linebreak is true) with a space
                    * other than newline or tab, ignore it.
                    */
                   if ((linebreak) && (i == 1) &&
                       (space[0] != '\n') && (space[0] != '\t'))
                     {
                        linebreak = 0;
                        continue;
                     }

                   if (space[0] == '\n')
                     break;
                   else if (space[0] == '\t')
                     {
                        int c;

                        used--;
                        c = ((used / 8) + 1) * 8;
                        if (c < total)
                          {
                             for (; used < c; used++)
                               fputc(' ', fp);
                          }
                        else
                          {
                             text--;
                             len++;
                             break;
                          }
                     }
                   else if (used < total)
                     fputc(space[0], fp);
                }
              else if ((!linebreak) && (todo < len))
                {
                   /* if not start of line and not last line, wrap line
                    * and try again. This avoids spliting words unless they
                    * are bigger than the available line width.
                    */
                   break;
                }
              else
                {
                   i = fwrite(text, 1, i, fp);
                   text += i;
                   len -= i;
                   used += i;
                }
              linebreak = 0;
           }
         if (len <= 0)
           break;
         linebreak = 1;
         fputc('\n', fp);
         for (used = 0; used < base; used++)
           fputc(' ', fp);
     }
   while (1);

   return used;
}

static void
_ecore_getopt_help_description(FILE               *fp,
                               const Ecore_Getopt *parser)
{
   const char *p, *prg, *ver;
   int used, prglen, verlen;

   p = gettext(parser->description);
   if (!p)
     return;

   fputc('\n', fp);

   prg = prog ? prog : "???";
   ver = parser->version ? parser->version : "???";

   prglen = strlen(prg);
   verlen = strlen(ver);

   used = 0;

   do
     {
        const char *d = strchr(p, '%');

        if (!d)
          {
             _ecore_getopt_help_line(fp, 0, cols, used, p, strlen(p));
             break;
          }

        used = _ecore_getopt_help_line(fp, 0, cols, used, p, d - p);
        d++;
        if (strncmp(d, "prog", sizeof("prog") - 1) == 0)
          {
             used = _ecore_getopt_help_line(fp, 0, cols, used, prg, prglen);
             d += sizeof("prog") - 1;
          }
        else if (strncmp(d, "version", sizeof("version") - 1) == 0)
          {
             used = _ecore_getopt_help_line(fp, 0, cols, used, ver, verlen);
             d += sizeof("version") - 1;
          }
        else
          {
             if (d[0] == '%')
               d++;
             used = _ecore_getopt_help_line(fp, 0, cols, used, "%", 1);
          }

        p = d;
     }
   while (p[0] != '\0');

   fputs("\n\n", fp);
}

static void
_ecore_getopt_copyright(FILE               *fp,
                        const Ecore_Getopt *parser)
{
   const char *txt = gettext(parser->copyright);
   fputs(_("Copyright:"), fp);
   fputs("\n   ", fp);
   _ecore_getopt_help_line
     (fp, 3, cols, 3, txt, strlen(txt));
   fputc('\n', fp);
}

static void
_ecore_getopt_license(FILE               *fp,
                      const Ecore_Getopt *parser)
{
   const char *txt = gettext(parser->license);
   fputs(_("License:"), fp);
   fputs("\n   ", fp);
   _ecore_getopt_help_line
     (fp, 3, cols, 3, txt, strlen(txt));
   fputc('\n', fp);
}

static Ecore_Getopt_Desc_Arg_Requirement
_ecore_getopt_desc_arg_requirement(const Ecore_Getopt_Desc *desc)
{
   switch (desc->action)
     {
      case ECORE_GETOPT_ACTION_STORE:
        return desc->action_param.store.arg_req;

      case ECORE_GETOPT_ACTION_STORE_CONST:
        return ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO;

      case ECORE_GETOPT_ACTION_STORE_TRUE:
        return ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO;

      case ECORE_GETOPT_ACTION_STORE_FALSE:
        return ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO;

      case ECORE_GETOPT_ACTION_CHOICE:
        return ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES;

      case ECORE_GETOPT_ACTION_APPEND:
        return ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES;

      case ECORE_GETOPT_ACTION_COUNT:
        return ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO;

      case ECORE_GETOPT_ACTION_CALLBACK:
        return desc->action_param.callback.arg_req;

      case ECORE_GETOPT_ACTION_HELP:
        return ECORE_GETOPT_DESC_ARG_REQUIREMENT_OPTIONAL;

      case ECORE_GETOPT_ACTION_VERSION:
        return ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO;

      default:
        return ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO;
     }
}

static void
_ecore_getopt_help_desc_setup_metavar(const Ecore_Getopt_Desc *desc,
                                      char                    *metavar,
                                      int                     *metavarlen,
                                      int                      maxsize)
{
   if (desc->metavar)
     {
        const char *txt = gettext(desc->metavar);
        *metavarlen = strlen(txt);
        if (*metavarlen > maxsize - 1)
          *metavarlen = maxsize - 1;

        memcpy(metavar, txt, *metavarlen);
        metavar[*metavarlen] = '\0';
     }
   else if (desc->longname)
     {
        int i;

        *metavarlen = strlen(desc->longname);
        if (*metavarlen > maxsize - 1)
          *metavarlen = maxsize - 1;

        for (i = 0; i < *metavarlen; i++)
          metavar[i] = toupper((int) desc->longname[i]);
        metavar[i] = '\0';
     }
}

static int
_ecore_getopt_help_desc_show_arg(FILE                             *fp,
                                 Ecore_Getopt_Desc_Arg_Requirement requirement,
                                 const char                       *metavar,
                                 int                               metavarlen,
                                 Eina_Bool                         show_attr)
{
   int used;

   if (requirement == ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO)
     return 0;

   used = 0;

   if (requirement == ECORE_GETOPT_DESC_ARG_REQUIREMENT_OPTIONAL)
     {
        fputc('[', fp);
        used++;
     }

   if (requirement != ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO)
     {
        if (show_attr)
          {
             fputc('=', fp);
             used++;
          }
        fputs(metavar, fp);
        used += metavarlen;
     }

   if (requirement == ECORE_GETOPT_DESC_ARG_REQUIREMENT_OPTIONAL)
     {
        fputc(']', fp);
        used++;
     }

   return used;
}

static int
_ecore_getopt_help_desc_store(FILE                    *fp,
                              const int                base,
                              const int                total,
                              int                      used,
                              const Ecore_Getopt_Desc *desc)
{
   const Ecore_Getopt_Desc_Store *store = &desc->action_param.store;
   char buf[64];
   const char *str;
   size_t len;

   fputc('\n', fp);
   for (used = 0; used < base; used++)
     fputc(' ', fp);

   switch (store->type)
     {
      case ECORE_GETOPT_TYPE_STR:
        str = "STR";
        len = sizeof("STR") - 1;
        break;

      case ECORE_GETOPT_TYPE_BOOL:
        str = "BOOL";
        len = sizeof("BOOL") - 1;
        break;

      case ECORE_GETOPT_TYPE_SHORT:
        str = "SHORT";
        len = sizeof("SHORT") - 1;
        break;

      case ECORE_GETOPT_TYPE_INT:
        str = "INT";
        len = sizeof("INT") - 1;
        break;

      case ECORE_GETOPT_TYPE_LONG:
        str = "LONG";
        len = sizeof("LONG") - 1;
        break;

      case ECORE_GETOPT_TYPE_USHORT:
        str = "USHORT";
        len = sizeof("USHORT") - 1;
        break;

      case ECORE_GETOPT_TYPE_UINT:
        str = "UINT";
        len = sizeof("UINT") - 1;
        break;

      case ECORE_GETOPT_TYPE_ULONG:
        str = "ULONG";
        len = sizeof("ULONG") - 1;
        break;

      case ECORE_GETOPT_TYPE_DOUBLE:
        str = "DOUBLE";
        len = sizeof("DOUBLE") - 1;
        break;

      default:
        str = "???";
        len = sizeof("???") - 1;
     }

   used = _ecore_getopt_help_line
       (fp, base, total, used, _("Type: "), strlen(_("Type: ")));
   used = _ecore_getopt_help_line(fp, base, total, used, str, len);

   if (store->arg_req == ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES)
     goto end;

   used = _ecore_getopt_help_line
       (fp, base, total, used, ". ", sizeof(". ") - 1);

   switch (store->type)
     {
      case ECORE_GETOPT_TYPE_STR:
        str = store->def.strv;
        len = str ? strlen(str) : 0;
        break;

      case ECORE_GETOPT_TYPE_BOOL:
        str = store->def.boolv ? "true" : "false";
        len = strlen(str);
        break;

      case ECORE_GETOPT_TYPE_SHORT:
        str = buf;
        len = snprintf(buf, sizeof(buf), "%hd", store->def.shortv);
        if (len > sizeof(buf) - 1)
          len = sizeof(buf) - 1;
        break;

      case ECORE_GETOPT_TYPE_INT:
        str = buf;
        len = snprintf(buf, sizeof(buf), "%d", store->def.intv);
        if (len > sizeof(buf) - 1)
          len = sizeof(buf) - 1;
        break;

      case ECORE_GETOPT_TYPE_LONG:
        str = buf;
        len = snprintf(buf, sizeof(buf), "%ld", store->def.longv);
        if (len > sizeof(buf) - 1)
          len = sizeof(buf) - 1;
        break;

      case ECORE_GETOPT_TYPE_USHORT:
        str = buf;
        len = snprintf(buf, sizeof(buf), "%hu", store->def.ushortv);
        if (len > sizeof(buf) - 1)
          len = sizeof(buf) - 1;
        break;

      case ECORE_GETOPT_TYPE_UINT:
        str = buf;
        len = snprintf(buf, sizeof(buf), "%u", store->def.uintv);
        if (len > sizeof(buf) - 1)
          len = sizeof(buf) - 1;
        break;

      case ECORE_GETOPT_TYPE_ULONG:
        str = buf;
        len = snprintf(buf, sizeof(buf), "%lu", store->def.ulongv);
        if (len > sizeof(buf) - 1)
          len = sizeof(buf) - 1;
        break;

      case ECORE_GETOPT_TYPE_DOUBLE:
        str = buf;
        len = snprintf(buf, sizeof(buf), "%f", store->def.doublev);
        if (len > sizeof(buf) - 1)
          len = sizeof(buf) - 1;
        break;

      default:
        str = "???";
        len = sizeof("???") - 1;
     }

   used = _ecore_getopt_help_line
       (fp, base, total, used, _("Default: "), strlen(_("Default: ")));
   used = _ecore_getopt_help_line(fp, base, total, used, str, len);

end:
   return _ecore_getopt_help_line(fp, base, total, used, ".", 1);
}

static int
_ecore_getopt_help_desc_choices(FILE                    *fp,
                                const int                base,
                                const int                total,
                                int                      used,
                                const Ecore_Getopt_Desc *desc)
{
   const char *const *itr;
   const char sep[] = ", ";
   const int seplen = sizeof(sep) - 1;

   if (used > 0)
     {
        fputc('\n', fp);
        used = 0;
     }
   for (; used < base; used++)
     fputc(' ', fp);

   used = _ecore_getopt_help_line
       (fp, base, total, used, _("Choices: "), strlen(_("Choices: ")));

   for (itr = desc->action_param.choices; *itr; itr++)
     {
        used = _ecore_getopt_help_line
            (fp, base, total, used, *itr, strlen(*itr));
        if (itr[1])
          used = _ecore_getopt_help_line(fp, base, total, used, sep, seplen);
     }

   return _ecore_getopt_help_line(fp, base, total, used, ".", 1);
}

static int
_ecore_getopt_help_desc_categories(FILE                    *fp,
                                   const int                base,
                                   const int                total,
                                   int                      used,
                                   const Ecore_Getopt_Desc *desc)
{
   const char sep[] = ", ";
   const int seplen = sizeof(sep) - 1;
   Eina_Bool cat_before = EINA_FALSE;

   if (used > 0)
     {
        fputc('\n', fp);
        used = 0;
     }
   for (; used < base; used++)
     fputc(' ', fp);

   /* do not print available categories if none available */
   for (; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     if (desc->action == ECORE_GETOPT_ACTION_CATEGORY && desc->longname)
       goto hascat;

   return _ecore_getopt_help_line(fp, base, total, used,
                                  _("No categories available."),
                                  strlen(_("No categories available.")));

hascat:
   used = _ecore_getopt_help_line
       (fp, base, total, used, _("Categories: "), strlen(_("Categories: ")));

   for (; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     {
         if (desc->action != ECORE_GETOPT_ACTION_CATEGORY || !desc->longname)
           continue;
         if (cat_before)
           used = _ecore_getopt_help_line(fp, base, total, used, sep, seplen);
         used = _ecore_getopt_help_line
             (fp, base, total, used, desc->longname, strlen(desc->longname));
         cat_before = EINA_TRUE;
     }

   return _ecore_getopt_help_line(fp, base, total, used, ".", 1);
}

static void
_ecore_getopt_help_desc(FILE                    *fp,
                        const Ecore_Getopt_Desc *desc,
                        const Ecore_Getopt      *parser)
{
   Ecore_Getopt_Desc_Arg_Requirement arg_req;
   char metavar[32] = "ARG";
   int metavarlen = 3;
   int used;

   if (desc->action == ECORE_GETOPT_ACTION_CATEGORY)
     {
        fprintf(fp, "\n%s\n", desc->help);
        return;
     }

   arg_req = _ecore_getopt_desc_arg_requirement(desc);
   if (arg_req != ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO)
     _ecore_getopt_help_desc_setup_metavar
       (desc, metavar, &metavarlen, sizeof(metavar));

   fputs("  ", fp);
   used = 2;

   if (desc->shortname)
     {
        fputc('-', fp);
        fputc(desc->shortname, fp);
        used += 2;
        used += _ecore_getopt_help_desc_show_arg
          (fp, arg_req, metavar, metavarlen, EINA_TRUE);
     }

   if (desc->shortname && desc->longname)
     {
        fputs(", ", fp);
        used += 2;
     }

   if (desc->longname)
     {
        int namelen = strlen(desc->longname);

        fputs("--", fp);
        fputs(desc->longname, fp);
        used += 2 + namelen;
        used += _ecore_getopt_help_desc_show_arg
          (fp, arg_req, metavar, metavarlen, EINA_TRUE);
     }
   else if ((!desc->shortname) && (desc->metavar))
     {
        used += _ecore_getopt_help_desc_show_arg
          (fp, arg_req, metavar, metavarlen, EINA_FALSE);
     }

   if (!desc->help)
     goto end;

   if (used + 3 >= helpcol)
     {
        fputc('\n', fp);
        used = 0;
     }

   for (; used < helpcol; used++)
     fputc(' ', fp);

   used = _ecore_getopt_help_line
       (fp, helpcol, cols, used, desc->help, strlen(desc->help));

   switch (desc->action)
     {
      case ECORE_GETOPT_ACTION_STORE:
        _ecore_getopt_help_desc_store(fp, helpcol, cols, used, desc);
        break;

      case ECORE_GETOPT_ACTION_CHOICE:
        _ecore_getopt_help_desc_choices(fp, helpcol, cols, used, desc);
        break;

      case ECORE_GETOPT_ACTION_HELP:
        _ecore_getopt_help_desc_categories(fp, helpcol, cols, used,
                                           parser->descs);
        break;

      default:
        break;
     }

end:
   fputc('\n', fp);
}

static Eina_Bool
_ecore_getopt_desc_is_sentinel(const Ecore_Getopt_Desc *desc)
{
   return (desc->shortname == '\0') && (!desc->longname)
     && (desc->action != ECORE_GETOPT_ACTION_CATEGORY);
}

static void
_ecore_getopt_help_options(FILE               *fp,
                           const Ecore_Getopt *parser)
{
   const Ecore_Getopt_Desc *desc;

   fputs(_("Options:\n"), fp);

   for (desc = parser->descs; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     _ecore_getopt_help_desc(fp, desc, parser);

   fputc('\n', fp);

   if (!desc->metavar) return;

   fputs(_("Positional arguments:\n"), fp);
   for (; desc->metavar != NULL; desc++)
     _ecore_getopt_help_desc(fp, desc, parser);

   fputc('\n', fp);
}

static Eina_Bool
_ecore_getopt_help_prepare(const Ecore_Getopt *parser)
{
   const char *var;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(EINA_FALSE);
   if (!parser) return EINA_FALSE;

   if (_argc < 1)
     {
        ecore_app_args_get(&_argc, &_argv);
        if ((_argc > 0) && (_argv[0]))
          prog = _argv[0];
        else
          prog = parser->prog;
     }

   var = getenv("COLUMNS");
   if (var)
     {
        cols = atoi(var);
        if (cols < 20)
          cols = 20;

        helpcol = cols / 3;
     }

   return EINA_TRUE;
}

EAPI void
ecore_getopt_help(FILE               *fp,
                  const Ecore_Getopt *parser)
{
   if (!_ecore_getopt_help_prepare(parser))
     return;

   _ecore_getopt_help_usage(fp, parser);
   _ecore_getopt_help_description(fp, parser);
   _ecore_getopt_help_options(fp, parser);
}

EAPI Eina_Bool
ecore_getopt_help_category(FILE               *fp,
                           const Ecore_Getopt *parser,
                           const char         *category)
{
   const Ecore_Getopt_Desc *desc;
   Eina_Bool found = EINA_FALSE;

   if (!category || !_ecore_getopt_help_prepare(parser)) return EINA_FALSE;
   for (desc = parser->descs; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     {
        if (desc->action != ECORE_GETOPT_ACTION_CATEGORY) continue;
        if (!desc->longname || strcmp(desc->longname, category)) continue;
        found = EINA_TRUE;
        break;
     }
   if (!found)
     {
        fprintf(stderr, _("ERROR: unknown category '%s'.\n"), category);
        return EINA_FALSE;
     }

   _ecore_getopt_help_usage(fp, parser);
   _ecore_getopt_help_description(fp, parser);

   fprintf(fp, "%s\n", (desc++)->help);
   for (; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     {
        if (desc->action == ECORE_GETOPT_ACTION_CATEGORY) break;
        _ecore_getopt_help_desc(fp, desc, parser);
     }

   return EINA_TRUE;
}

static const Ecore_Getopt_Desc *
_ecore_getopt_parse_find_long(const Ecore_Getopt *parser,
                              const char         *name)
{
   const Ecore_Getopt_Desc *desc = parser->descs;
   const char *p = strchr(name, '=');
   int len = 0;

   if (p)
     len = p - name;

   for (; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     {
        if (!desc->longname || desc->action == ECORE_GETOPT_ACTION_CATEGORY)
          continue;

        if (p)
          {
             if ((strncmp(name, desc->longname, len) == 0) &&
                 (desc->longname[len] == '\0'))
               return desc;
          }
        else
          {
             if (strcmp(name, desc->longname) == 0)
               return desc;
          }
     }

   return NULL;
}

static const Ecore_Getopt_Desc *
_ecore_getopt_parse_find_short(const Ecore_Getopt *parser,
                               char                name)
{
   const Ecore_Getopt_Desc *desc = parser->descs;
   for (; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     if (name == desc->shortname)
       return desc;
   return NULL;
}

static int
_ecore_getopt_parse_find_nonargs_base(const Ecore_Getopt *parser,
                                      int                 argc,
                                      char              **argv)
{
   char **nonargs;
   int src, dst, used, base, abreak;

   nonargs = alloca(sizeof(char *) * argc);
   src = 1;
   dst = 1;
   used = 0;
   base = 0;
   abreak = 0;
   while (src < argc)
     {
        const Ecore_Getopt_Desc *desc;
        Ecore_Getopt_Desc_Arg_Requirement arg_req;
        char *arg = argv[src];

        if (abreak)
          {
             base = 1;
             break;
          }

        if (arg[0] != '-')
          goto found_nonarg;

        if (arg[1] == '-')
          {
             if (arg[2] == '\0') /* explicit end of options, "--" */
               {
                  base = 1;
                  break;
               }
             desc = _ecore_getopt_parse_find_long(parser, arg + 2);
          }
        else
          desc = _ecore_getopt_parse_find_short(parser, arg[1]);

        if (!desc)
          {
             if (arg[1] == '-')
               fprintf(stderr, _("ERROR: unknown option --%s.\n"), arg + 2);
             else
               fprintf(stderr, _("ERROR: unknown option -%c.\n"), arg[1]);
             if (parser->strict)
               {
                  memmove(argv + dst, nonargs, used * sizeof(char *));
                  return -1;
               }
             else
               goto found_nonarg;
          }



        if (desc->action == ECORE_GETOPT_ACTION_BREAK)
          abreak = 1;

        if (src != dst)
          argv[dst] = argv[src];
        src++;
        dst++;

        arg_req = _ecore_getopt_desc_arg_requirement(desc);
        if (arg_req == ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO)
          continue;

        if (strchr(arg, '='))
          continue;

        if ((src >= argc) || (argv[src][0] == '-'))
          continue;

        if (src != dst)
          argv[dst] = argv[src];
        src++;
        dst++;
        continue;

found_nonarg:
        nonargs[used] = arg;
        used++;
        src++;
     }

   if (!base) /* '--' not found */
     base = dst;
   else
     {
        base = dst;
        if (src != dst)
          argv[dst] = argv[src];
        dst++;
     }

   memmove(argv + dst, nonargs, used * sizeof(char *));
   return base;
}

static void
_ecore_getopt_desc_print_error(const Ecore_Getopt_Desc *desc,
                               const char              *fmt,
                               ...)
{
   va_list ap;

   fputs(_("ERROR: "), stderr);

   if (desc->shortname)
     {
        fputc('-', stderr);
        fputc(desc->shortname, stderr);
     }

   if (desc->shortname && desc->longname)
     fputs(", ", stderr);

   if (desc->longname)
     {
        fputs("--", stderr);
        fputs(desc->longname, stderr);
     }
   else if ((!desc->shortname) && (desc->metavar))
     {
        char metavar[32];
        int metavarlen = 0;
        _ecore_getopt_help_desc_setup_metavar(desc, metavar, &metavarlen,
                                              sizeof(metavar));
        fputs(metavar, stderr);
     }

   fputs(": ", stderr);

   va_start(ap, fmt);
   vfprintf(stderr, fmt, ap);
   va_end(ap);
}

static Eina_Bool
_ecore_getopt_parse_bool(const char *str,
                         Eina_Bool  *v)
{
   if (!str)
     return EINA_FALSE;

   if ((strcmp(str, "0") == 0) ||
       (strcasecmp(str, "f") == 0) ||
       (strcasecmp(str, "false") == 0) ||
       (strcasecmp(str, "no") == 0) ||
       (strcasecmp(str, "off") == 0)
       )
     {
        *v = EINA_FALSE;
        return EINA_TRUE;
     }
   else if ((strcmp(str, "1") == 0) ||
            (strcasecmp(str, "t") == 0) ||
            (strcasecmp(str, "true") == 0) ||
            (strcasecmp(str, "yes") == 0) ||
            (strcasecmp(str, "on") == 0)
            )
     {
        *v = EINA_TRUE;
        return EINA_TRUE;
     }

   return EINA_FALSE;
}

static Eina_Bool
_ecore_getopt_parse_long(const char *str,
                         long int   *v)
{
   char *endptr = NULL;
   if (!str) return EINA_FALSE;
   *v = strtol(str, &endptr, 0);
   return endptr > str;
}

static Eina_Bool
_ecore_getopt_parse_double(const char *str,
                           double     *v)
{
   char *endptr = NULL;
   if (!str) return EINA_FALSE;
   *v = strtod(str, &endptr);
   return endptr > str;
}

static Eina_Bool
_ecore_getopt_parse_store(const Ecore_Getopt      *parser EINA_UNUSED,
                          const Ecore_Getopt_Desc *desc,
                          Ecore_Getopt_Value      *value,
                          const char              *arg_val)
{
   const Ecore_Getopt_Desc_Store *store = &desc->action_param.store;
   long int v;
   double d;
   Eina_Bool b;

   if (!value->ptrp)
     {
        _ecore_getopt_desc_print_error(desc, _("value has no pointer set.\n"));
        return EINA_FALSE;
     }

   switch (store->arg_req)
     {
      case ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO:
        goto use_optional;

      case ECORE_GETOPT_DESC_ARG_REQUIREMENT_OPTIONAL:
        if (!arg_val)
          goto use_optional;

      case ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES:
        break;
     }

   switch (store->type)
     {
      case ECORE_GETOPT_TYPE_STR:
        *value->strp = (char *)arg_val;
        return EINA_TRUE;

      case ECORE_GETOPT_TYPE_BOOL:
        if (_ecore_getopt_parse_bool(arg_val, &b))
          {
             *value->boolp = b;
             return EINA_TRUE;
          }
        else
          {
             _ecore_getopt_desc_print_error
               (desc, _("unknown boolean value %s.\n"), arg_val);
             return EINA_FALSE;
          }

      case ECORE_GETOPT_TYPE_SHORT:
        if (!_ecore_getopt_parse_long(arg_val, &v))
          goto error;
        *value->shortp = v;
        return EINA_TRUE;

      case ECORE_GETOPT_TYPE_INT:
        if (!_ecore_getopt_parse_long(arg_val, &v))
          goto error;
        *value->intp = v;
        return EINA_TRUE;

      case ECORE_GETOPT_TYPE_LONG:
        if (!_ecore_getopt_parse_long(arg_val, &v))
          goto error;
        *value->longp = v;
        return EINA_TRUE;

      case ECORE_GETOPT_TYPE_USHORT:
        if (!_ecore_getopt_parse_long(arg_val, &v))
          goto error;
        *value->ushortp = v;
        return EINA_TRUE;

      case ECORE_GETOPT_TYPE_UINT:
        if (!_ecore_getopt_parse_long(arg_val, &v))
          goto error;
        *value->uintp = v;
        return EINA_TRUE;

      case ECORE_GETOPT_TYPE_ULONG:
        if (!_ecore_getopt_parse_long(arg_val, &v))
          goto error;
        *value->ulongp = v;
        return EINA_TRUE;

      case ECORE_GETOPT_TYPE_DOUBLE:
        if (!_ecore_getopt_parse_double(arg_val, &d))
          goto error;
        *value->doublep = d;
        break;
     }

   return EINA_TRUE;

error:
   _ecore_getopt_desc_print_error
     (desc, _("invalid number format %s\n"), arg_val);
   return EINA_FALSE;

use_optional:
   switch (store->type)
     {
      case ECORE_GETOPT_TYPE_STR:
        *value->strp = (char *)store->def.strv;
        break;

      case ECORE_GETOPT_TYPE_BOOL:
        *value->boolp = store->def.boolv;
        break;

      case ECORE_GETOPT_TYPE_SHORT:
        *value->shortp = store->def.shortv;
        break;

      case ECORE_GETOPT_TYPE_INT:
        *value->intp = store->def.intv;
        break;

      case ECORE_GETOPT_TYPE_LONG:
        *value->longp = store->def.longv;
        break;

      case ECORE_GETOPT_TYPE_USHORT:
        *value->ushortp = store->def.ushortv;
        break;

      case ECORE_GETOPT_TYPE_UINT:
        *value->uintp = store->def.uintv;
        break;

      case ECORE_GETOPT_TYPE_ULONG:
        *value->ulongp = store->def.ulongv;
        break;

      case ECORE_GETOPT_TYPE_DOUBLE:
        *value->doublep = store->def.doublev;
        break;
     }

   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_store_const(const Ecore_Getopt      *parser EINA_UNUSED,
                                const Ecore_Getopt_Desc *desc,
                                Ecore_Getopt_Value      *val,
                                const char              *arg_val EINA_UNUSED)
{
   if (!val->ptrp)
     {
        _ecore_getopt_desc_print_error(desc, _("value has no pointer set.\n"));
        return EINA_FALSE;
     }

   *val->ptrp = (void *)desc->action_param.store_const;
   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_store_true(const Ecore_Getopt      *parser EINA_UNUSED,
                               const Ecore_Getopt_Desc *desc,
                               Ecore_Getopt_Value      *val,
                               const char              *arg_val EINA_UNUSED)
{
   if (!val->boolp)
     {
        _ecore_getopt_desc_print_error(desc, _("value has no pointer set.\n"));
        return EINA_FALSE;
     }
   *val->boolp = EINA_TRUE;
   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_store_false(const Ecore_Getopt      *parser EINA_UNUSED,
                                const Ecore_Getopt_Desc *desc,
                                Ecore_Getopt_Value      *val,
                                const char              *arg_val EINA_UNUSED)
{
   if (!val->boolp)
     {
        _ecore_getopt_desc_print_error(desc, _("value has no pointer set.\n"));
        return EINA_FALSE;
     }
   *val->boolp = EINA_FALSE;
   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_choice(const Ecore_Getopt      *parser EINA_UNUSED,
                           const Ecore_Getopt_Desc *desc,
                           Ecore_Getopt_Value      *val,
                           const char              *arg_val)
{
   const char *const *pchoice;

   if (!arg_val)
     return EINA_FALSE;

   if (!val->strp)
     {
        _ecore_getopt_desc_print_error(desc, _("value has no pointer set.\n"));
        return EINA_FALSE;
     }

   pchoice = desc->action_param.choices;
   for (; *pchoice; pchoice++)
     if (strcmp(*pchoice, arg_val) == 0)
       {
          *val->strp = (char *)*pchoice;
          return EINA_TRUE;
       }

   _ecore_getopt_desc_print_error
     (desc, _("invalid choice \"%s\". Valid values are: "), arg_val);

   pchoice = desc->action_param.choices;
   for (; *pchoice; pchoice++)
     {
        fputs(*pchoice, stderr);
        if (pchoice[1])
          fputs(", ", stderr);
     }

   fputs(".\n", stderr);
   return EINA_FALSE;
}

static Eina_Bool
_ecore_getopt_parse_append(const Ecore_Getopt      *parser EINA_UNUSED,
                           const Ecore_Getopt_Desc *desc,
                           Ecore_Getopt_Value      *val,
                           const char              *arg_val)
{
   void *data;
   long int v;
   double d;
   Eina_Bool b;

   if (!arg_val)
     {
        _ecore_getopt_desc_print_error
          (desc, _("missing parameter to append.\n"));
        return EINA_FALSE;
     }

   if (!val->listp)
     {
        _ecore_getopt_desc_print_error(desc, _("value has no pointer set.\n"));
        return EINA_FALSE;
     }

   switch (desc->action_param.append_type)
     {
      case ECORE_GETOPT_TYPE_STR:
        data = strdup(arg_val);
        break;

      case ECORE_GETOPT_TYPE_BOOL:
      {
         if (_ecore_getopt_parse_bool(arg_val, &b))
           {
              data = malloc(sizeof(Eina_Bool));
              if (data)
                *(Eina_Bool *)data = b;
           }
         else
           {
              _ecore_getopt_desc_print_error(desc, _("unknown boolean value %s.\n"), arg_val);
              return EINA_FALSE;
           }
      }
      break;

      case ECORE_GETOPT_TYPE_SHORT:
      {
         if (!_ecore_getopt_parse_long(arg_val, &v))
           goto error;
         data = malloc(sizeof(short));
         if (data)
           *(short *)data = (short)v;
      }
      break;

      case ECORE_GETOPT_TYPE_INT:
      {
         if (!_ecore_getopt_parse_long(arg_val, &v))
           goto error;
         data = malloc(sizeof(int));
         if (data)
           *(int *)data = (int)v;
      }
      break;

      case ECORE_GETOPT_TYPE_LONG:
      {
         if (!_ecore_getopt_parse_long(arg_val, &v))
           goto error;
         data = malloc(sizeof(long));
         if (data)
           *(long *)data = v;
      }
      break;

      case ECORE_GETOPT_TYPE_USHORT:
      {
         if (!_ecore_getopt_parse_long(arg_val, &v))
           goto error;
         data = malloc(sizeof(unsigned short));
         if (data)
           *(unsigned short *)data = (unsigned short)v;
      }
      break;

      case ECORE_GETOPT_TYPE_UINT:
      {
         if (!_ecore_getopt_parse_long(arg_val, &v))
           goto error;
         data = malloc(sizeof(unsigned int));
         if (data)
           *(unsigned int *)data = (unsigned int)v;
      }
      break;

      case ECORE_GETOPT_TYPE_ULONG:
      {
         if (!_ecore_getopt_parse_long(arg_val, &v))
           goto error;
         data = malloc(sizeof(unsigned long));
         if (data)
           *(unsigned long *)data = v;
      }
      break;

      case ECORE_GETOPT_TYPE_DOUBLE:
      {
         if (!_ecore_getopt_parse_double(arg_val, &d))
           goto error;
         data = malloc(sizeof(double));
         if (data)
           *(double *)data = d;
      }
      break;

      default:
      {
         _ecore_getopt_desc_print_error(desc, _("could not parse value.\n"));
         return EINA_FALSE;
      }
     }

   *val->listp = eina_list_append(*val->listp, data);
   return EINA_TRUE;

error:
   _ecore_getopt_desc_print_error
     (desc, _("invalid number format %s\n"), arg_val);
   return EINA_FALSE;
}

static Eina_Bool
_ecore_getopt_parse_count(const Ecore_Getopt      *parser EINA_UNUSED,
                          const Ecore_Getopt_Desc *desc,
                          Ecore_Getopt_Value      *val,
                          const char              *arg_val EINA_UNUSED)
{
   if (!val->intp)
     {
        _ecore_getopt_desc_print_error(desc, _("value has no pointer set.\n"));
        return EINA_FALSE;
     }

   (*val->intp)++;
   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_callback(const Ecore_Getopt      *parser,
                             const Ecore_Getopt_Desc *desc,
                             Ecore_Getopt_Value      *val,
                             const char              *arg_val)
{
   const Ecore_Getopt_Desc_Callback *cb = &desc->action_param.callback;

   switch (cb->arg_req)
     {
      case ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO:
        arg_val = cb->def;
        break;

      case ECORE_GETOPT_DESC_ARG_REQUIREMENT_OPTIONAL:
        if (!arg_val)
          arg_val = cb->def;
        break;

      case ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES:
        break;
     }

   if (cb->arg_req != ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO)
     {
        if ((!arg_val) || (arg_val[0] == '\0'))
          {
             _ecore_getopt_desc_print_error(desc, _("missing parameter.\n"));
             return EINA_FALSE;
          }

        if (!val->ptrp)
          {
             _ecore_getopt_desc_print_error(desc, _("value has no pointer set.\n"));
             return EINA_FALSE;
          }
     }

   if (!cb->func)
     {
        _ecore_getopt_desc_print_error(desc, _("missing callback function!\n"));
        return EINA_FALSE;
     }

   return cb->func(parser, desc, arg_val, (void *)cb->data, val);
}

static Eina_Bool
_ecore_getopt_parse_help(const Ecore_Getopt      *parser,
                         const Ecore_Getopt_Desc *desc EINA_UNUSED,
                         Ecore_Getopt_Value      *val,
                         const char              *arg_val)
{
   if (val->boolp)
     (*val->boolp) = EINA_TRUE;
   if (arg_val)
     return ecore_getopt_help_category(stdout, parser, arg_val);
   ecore_getopt_help(stdout, parser);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_version(const Ecore_Getopt      *parser,
                            const Ecore_Getopt_Desc *desc,
                            Ecore_Getopt_Value      *val,
                            const char              *arg_val EINA_UNUSED)
{
   if (val->boolp)
     (*val->boolp) = EINA_TRUE;
   if (!parser->version)
     {
        _ecore_getopt_desc_print_error(desc, _("no version was defined.\n"));
        return EINA_FALSE;
     }
   _ecore_getopt_version(stdout, parser);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_copyright(const Ecore_Getopt      *parser,
                              const Ecore_Getopt_Desc *desc,
                              Ecore_Getopt_Value      *val,
                              const char              *arg_val EINA_UNUSED)
{
   if (val->boolp)
     (*val->boolp) = EINA_TRUE;
   if (!parser->copyright)
     {
        _ecore_getopt_desc_print_error(desc, _("no copyright was defined.\n"));
        return EINA_FALSE;
     }
   _ecore_getopt_copyright(stdout, parser);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_license(const Ecore_Getopt      *parser,
                            const Ecore_Getopt_Desc *desc,
                            Ecore_Getopt_Value      *val,
                            const char              *arg_val EINA_UNUSED)
{
   if (val->boolp)
     (*val->boolp) = EINA_TRUE;
   if (!parser->license)
     {
        _ecore_getopt_desc_print_error(desc, _("no license was defined.\n"));
        return EINA_FALSE;
     }
   _ecore_getopt_license(stdout, parser);
   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_break(const Ecore_Getopt      *parser EINA_UNUSED,
                          const Ecore_Getopt_Desc *desc EINA_UNUSED,
                          Ecore_Getopt_Value      *val,
                          const char              *arg_val EINA_UNUSED)
{
   if (val->boolp)
     (*val->boolp) = EINA_TRUE;

   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_desc_handle(const Ecore_Getopt      *parser,
                          const Ecore_Getopt_Desc *desc,
                          Ecore_Getopt_Value      *value,
                          const char              *arg_val)
{
   switch (desc->action)
     {
      case ECORE_GETOPT_ACTION_STORE:
        return _ecore_getopt_parse_store(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_STORE_CONST:
        return _ecore_getopt_parse_store_const(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_STORE_TRUE:
        return _ecore_getopt_parse_store_true(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_STORE_FALSE:
        return _ecore_getopt_parse_store_false(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_CHOICE:
        return _ecore_getopt_parse_choice(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_APPEND:
        return _ecore_getopt_parse_append(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_COUNT:
        return _ecore_getopt_parse_count(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_CALLBACK:
        return _ecore_getopt_parse_callback(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_HELP:
        return _ecore_getopt_parse_help(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_VERSION:
        return _ecore_getopt_parse_version(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_COPYRIGHT:
        return _ecore_getopt_parse_copyright(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_LICENSE:
        return _ecore_getopt_parse_license(parser, desc, value, arg_val);

      case ECORE_GETOPT_ACTION_BREAK:
        return _ecore_getopt_parse_break(parser, desc, value, arg_val);

      default:
        return EINA_FALSE;
     }
}

static Eina_Bool
_ecore_getopt_parse_arg_long(const Ecore_Getopt *parser,
                             Ecore_Getopt_Value *values,
                             int                 argc EINA_UNUSED,
                             char              **argv,
                             int                *idx,
                             int                *nonargs,
                             const char         *arg)
{
   const Ecore_Getopt_Desc *desc;
   Ecore_Getopt_Desc_Arg_Requirement arg_req;
   const char *arg_val;
   int desc_idx;
   Ecore_Getopt_Value *value;
   Eina_Bool ret;

   desc = _ecore_getopt_parse_find_long(parser, arg);
   if (!desc)
     {
        fprintf(stderr, _("ERROR: unknown option --%s, ignored.\n"), arg);
        if (parser->strict)
          return EINA_FALSE;

        (*idx)++;
        return EINA_TRUE;
     }

   (*idx)++;

   arg_req = _ecore_getopt_desc_arg_requirement(desc);
   if (arg_req != ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO)
     {
        arg_val = strchr(arg, '=');
        if (arg_val)
          arg_val++;
        else
          {
             if ((*idx < *nonargs) && (argv[*idx][0] != '-'))
               {
                  arg_val = argv[*idx];
                  (*idx)++;
               }
             else
               arg_val = NULL;
          }

        /* empty strings are still valid values */
        if (!(((desc->action == ECORE_GETOPT_ACTION_STORE) &&
               (desc->action_param.store.type == ECORE_GETOPT_TYPE_STR)) ||
              ((desc->action == ECORE_GETOPT_ACTION_APPEND) &&
               (desc->action_param.append_type == ECORE_GETOPT_TYPE_STR))))
          {
             if (arg_val && arg_val[0] == '\0')
               arg_val = NULL;
          }

        if ((!arg_val) && (arg_req == ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES))
          {
             fprintf
               (stderr, _("ERROR: option --%s requires an argument!\n"), arg);
             if (parser->strict)
               return EINA_FALSE;
             return EINA_TRUE;
          }
     }
   else
     arg_val = NULL;

   desc_idx = desc - parser->descs;
   value = values + desc_idx;
   ret = _ecore_getopt_desc_handle(parser, desc, value, arg_val);
   if ((!ret) && parser->strict)
     return EINA_FALSE;

   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_arg_short(const Ecore_Getopt *parser,
                              Ecore_Getopt_Value *values,
                              int                 argc EINA_UNUSED,
                              char              **argv,
                              int                *idx,
                              int                *nonargs,
                              const char         *arg)
{
   int run = 1;
   while (run && (arg[0] != '\0'))
     {
        int opt = arg[0];
        const Ecore_Getopt_Desc *desc;
        Ecore_Getopt_Desc_Arg_Requirement arg_req;
        const char *arg_val;
        int desc_idx;
        Ecore_Getopt_Value *value;
        Eina_Bool ret;

        desc = _ecore_getopt_parse_find_short(parser, arg[0]);
        if (!desc)
          {
             fprintf
               (stderr, _("ERROR: unknown option -%c, ignored.\n"), arg[0]);
             if (parser->strict)
               return EINA_FALSE;

             arg++;
             continue;
          }

        arg++;

        arg_req = _ecore_getopt_desc_arg_requirement(desc);
        if (arg_req != ECORE_GETOPT_DESC_ARG_REQUIREMENT_NO)
          {
             (*idx)++;
             run = 0;

             if (arg[0] == '=')
               arg_val = arg + 1;
             else if (arg[0] != '\0')
               arg_val = arg;
             else
               {
                  if ((*idx < *nonargs) && (argv[*idx][0] != '-'))
                    {
                       arg_val = argv[*idx];
                       (*idx)++;
                    }
                  else
                    arg_val = NULL;
               }

             /* empty strings are still valid values */
             if (!(((desc->action == ECORE_GETOPT_ACTION_STORE) &&
                    (desc->action_param.store.type == ECORE_GETOPT_TYPE_STR)) ||
                   ((desc->action == ECORE_GETOPT_ACTION_APPEND) &&
                    (desc->action_param.append_type == ECORE_GETOPT_TYPE_STR))))
               {
                  if (arg_val && arg_val[0] == '\0')
                    arg_val = NULL;
               }

             if ((!arg_val) &&
                 (arg_req == ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES))
               {
                  fprintf
                    (stderr, _("ERROR: option -%c requires an argument!\n"),
                    opt);
                  if (parser->strict)
                    return EINA_FALSE;
                  return EINA_TRUE;
               }
          }
        else
          arg_val = NULL;

        desc_idx = desc - parser->descs;
        value = values + desc_idx;
        ret = _ecore_getopt_desc_handle(parser, desc, value, arg_val);
        if ((!ret) && parser->strict)
          return EINA_FALSE;
     }

   if (run)
     (*idx)++;

   return EINA_TRUE;
}

static Eina_Bool
_ecore_getopt_parse_arg(const Ecore_Getopt *parser,
                        Ecore_Getopt_Value *values,
                        int                 argc,
                        char              **argv,
                        int                *idx,
                        int                *nonargs)
{
   char *arg = argv[*idx];

   if (arg[0] != '-')
     {
        char **dst, **src, **src_end;

        dst = argv + *idx;
        src = dst + 1;
        src_end = src + *nonargs - *idx - 1;

        for (; src < src_end; src++, dst++)
          *dst = *src;

        *dst = arg;
        (*nonargs)--;
        return EINA_TRUE;
     }

   if (arg[1] == '-')
     return _ecore_getopt_parse_arg_long(parser, values, argc, argv, idx, nonargs, arg + 2);
   else
     return _ecore_getopt_parse_arg_short(parser, values, argc, argv, idx, nonargs, arg + 1);
}

static Eina_Bool
_ecore_getopt_parse_pos(const Ecore_Getopt *parser,
                        const Ecore_Getopt_Desc **p_desc,
                        Ecore_Getopt_Value *values,
                        int                 argc,
                        char              **argv,
                        int                *idx,
                        int                *nonargs)
{
   const Ecore_Getopt_Desc *desc = *p_desc;
   Ecore_Getopt_Desc_Arg_Requirement arg_req;
   char metavar[32];
   int metavarlen = 0;
   const char *arg_val;
   int desc_idx;
   Ecore_Getopt_Value *value;
   Eina_Bool ret;

   _ecore_getopt_help_desc_setup_metavar
     (desc, metavar, &metavarlen, sizeof(metavar));

   desc_idx = desc - parser->descs;
   value = values + desc_idx;

   arg_req = _ecore_getopt_desc_arg_requirement(desc);
   if (*idx >= argc)
     {
        (*p_desc)++;
        if (arg_req == ECORE_GETOPT_DESC_ARG_REQUIREMENT_YES)
          {
             /* TODO: should we consider callback here as well? */
             if ((desc->action == ECORE_GETOPT_ACTION_APPEND) &&
                 (value->listp) && (*value->listp))
               {
                  printf("append desc: %s (%d), value: %p\n",
                         desc->metavar, desc_idx, value->listp);
                  return EINA_TRUE;
               }

             fprintf(stderr,
                     _("ERROR: missing required positional argument %s.\n"),
                     metavar);
             return EINA_FALSE;
          }
        return EINA_TRUE;
     }

   arg_val = argv[*idx];

   switch (desc->action)
     {
      case ECORE_GETOPT_ACTION_STORE:
         ret = _ecore_getopt_parse_store(parser, desc, value, arg_val);
         (*p_desc)++;
         break;

      case ECORE_GETOPT_ACTION_CHOICE:
         ret = _ecore_getopt_parse_choice(parser, desc, value, arg_val);
         (*p_desc)++;
         break;

      case ECORE_GETOPT_ACTION_APPEND:
         ret = _ecore_getopt_parse_append(parser, desc, value, arg_val);
         /* no changes to p_desc, we keep appending until the end */
         break;

      case ECORE_GETOPT_ACTION_CALLBACK:
         ret = _ecore_getopt_parse_callback(parser, desc, value, arg_val);
         (*p_desc)++;
         break;

      default:
         fprintf(stderr, _("ERROR: unsupported action type %d "
                           "for positional argument %s\n"),
                 desc->action, metavar);
         (*p_desc)++;
         ret = EINA_FALSE;
         break;
     }

   if (ret)
     {
        (*idx)++;
        (*nonargs)++;
     }

   if ((!ret) && parser->strict)
     return EINA_FALSE;

   return EINA_TRUE;
}

static const Ecore_Getopt_Desc *
_ecore_getopt_parse_find_short_other(const Ecore_Getopt      *parser,
                                     const Ecore_Getopt_Desc *orig)
{
   const Ecore_Getopt_Desc *desc = parser->descs;
   const char c = orig->shortname;

   for (; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     {
        if (desc == orig)
          return NULL;

        if (c == desc->shortname)
          return desc;
     }

   return NULL;
}

static const Ecore_Getopt_Desc *
_ecore_getopt_parse_find_long_other(const Ecore_Getopt      *parser,
                                    const Ecore_Getopt_Desc *orig)
{
   const Ecore_Getopt_Desc *desc = parser->descs;
   const char *name = orig->longname;

   for (; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     {
        if (desc == orig)
          return NULL;
        if (desc->action == ECORE_GETOPT_ACTION_CATEGORY)
          continue;

        if (desc->longname && (strcmp(name, desc->longname) == 0))
          return desc;
     }

   return NULL;
}

EAPI Eina_Bool
ecore_getopt_parser_has_duplicates(const Ecore_Getopt *parser)
{
   const Ecore_Getopt_Desc *desc = parser->descs;
   for (; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     {
        if (desc->shortname)
          {
             const Ecore_Getopt_Desc *other;
             other = _ecore_getopt_parse_find_short_other(parser, desc);
             if (other)
               {
                  _ecore_getopt_desc_print_error(desc, "short name -%c already exists.", desc->shortname);

                  if (other->longname)
                    fprintf(stderr, " Other is --%s.\n", other->longname);
                  else
                    fputc('\n', stderr);
                  return EINA_TRUE;
               }
          }

        if (desc->longname)
          {
             const Ecore_Getopt_Desc *other;
             other = _ecore_getopt_parse_find_long_other(parser, desc);
             if (other)
               {
                  _ecore_getopt_desc_print_error(desc, "long name --%s already exists.", desc->longname);

                  if (other->shortname)
                    fprintf(stderr, " Other is -%c.\n", other->shortname);
                  else
                    fputc('\n', stderr);
                  return EINA_TRUE;
               }
          }
     }
   return EINA_FALSE;
}

static const Ecore_Getopt_Desc *
_ecore_getopt_find_help(const Ecore_Getopt *parser)
{
   const Ecore_Getopt_Desc *desc = parser->descs;
   for (; !_ecore_getopt_desc_is_sentinel(desc); desc++)
     if (desc->action == ECORE_GETOPT_ACTION_HELP)
       return desc;
   return NULL;
}

EAPI int
ecore_getopt_parse(const Ecore_Getopt *parser,
                   Ecore_Getopt_Value *values,
                   int                 argc,
                   char              **argv)
{
   int i, nonargs;

   if (!parser)
     {
        fputs(_("ERROR: no parser provided.\n"), stderr);
        return -1;
     }
   if (!values)
     {
        fputs(_("ERROR: no values provided.\n"), stderr);
        return -1;
     }

   if ((argc < 1) || (!argv))
     ecore_app_args_get(&argc, &argv);

   if (argc < 1)
     {
        fputs(_("ERROR: no arguments provided.\n"), stderr);
        return -1;
     }

   if (argv[0])
     prog = argv[0];
   else
     prog = parser->prog;

   nonargs = _ecore_getopt_parse_find_nonargs_base(parser, argc, argv);
   if (nonargs < 0)
     goto error;

   if (nonargs > argc)
     nonargs = argc;

   i = 1;
   while (i < nonargs)
     if (!_ecore_getopt_parse_arg(parser, values, argc, argv, &i, &nonargs))
       goto error;

   return nonargs;

error:
   {
      const Ecore_Getopt_Desc *help;
      fputs(_("ERROR: invalid options found."), stderr);

      help = _ecore_getopt_find_help(parser);
      if (!help)
        fputc('\n', stderr);
      else if (help->longname)
        fprintf(stderr, _(" See --%s.\n"), help->longname);
      else
        fprintf(stderr, _(" See -%c.\n"), help->shortname);
   }

   return -1;
}

EAPI int
ecore_getopt_parse_positional(const Ecore_Getopt *parser,
                              Ecore_Getopt_Value *values,
                              int argc,
                              char **argv,
                              int start)
{
   const Ecore_Getopt_Desc *desc;
   int nonargs;

   if (!parser)
     {
        fputs(_("ERROR: no parser provided.\n"), stderr);
        return -1;
     }
   if (!values)
     {
        fputs(_("ERROR: no values provided.\n"), stderr);
        return -1;
     }

   if ((argc < 1) || (!argv))
     ecore_app_args_get(&argc, &argv);

   if (argc < 1)
     {
        fputs(_("ERROR: no arguments provided.\n"), stderr);
        return -1;
     }

   if (argv[0])
     prog = argv[0];
   else
     prog = parser->prog;

   if (start > argc)
     start = argc;
   else if (start < 1)
     start = _ecore_getopt_parse_find_nonargs_base(parser, argc, argv);
   if (start < 0) goto error;

   nonargs = start;
   for (desc = parser->descs; !_ecore_getopt_desc_is_sentinel(desc); desc++);
   while (desc->metavar)
     if (!_ecore_getopt_parse_pos
         (parser, &desc, values, argc, argv, &start, &nonargs))
       goto error;

   return nonargs;

error:
   {
      const Ecore_Getopt_Desc *help;
      fputs(_("ERROR: invalid positional arguments found."), stderr);

      help = _ecore_getopt_find_help(parser);
      if (!help)
        fputc('\n', stderr);
      else if (help->longname)
        fprintf(stderr, _(" See --%s.\n"), help->longname);
      else
        fprintf(stderr, _(" See -%c.\n"), help->shortname);
   }

   return -1;
}

EAPI Eina_List *
ecore_getopt_list_free(Eina_List *list)
{
   void *data;

   EINA_LIST_FREE(list, data)
     free(data);
   return NULL;
}

EAPI Eina_Bool
ecore_getopt_callback_geometry_parse(const Ecore_Getopt      *parser EINA_UNUSED,
                                     const Ecore_Getopt_Desc *desc EINA_UNUSED,
                                     const char              *str,
                                     void                    *data EINA_UNUSED,
                                     Ecore_Getopt_Value      *storage)
{
   Eina_Rectangle *v = (Eina_Rectangle *)storage->ptrp;

   if (sscanf(str, "%d:%d:%d:%d", &v->x, &v->y, &v->w, &v->h) != 4)
     {
        fprintf(stderr, _("ERROR: incorrect geometry value '%s'\n"), str);
        return EINA_FALSE;
     }

   return EINA_TRUE;
}

EAPI Eina_Bool
ecore_getopt_callback_size_parse(const Ecore_Getopt      *parser EINA_UNUSED,
                                 const Ecore_Getopt_Desc *desc EINA_UNUSED,
                                 const char              *str,
                                 void                    *data EINA_UNUSED,
                                 Ecore_Getopt_Value      *storage)
{
   Eina_Rectangle *v = (Eina_Rectangle *)storage->ptrp;

   if (sscanf(str, "%dx%d", &v->w, &v->h) != 2)
     {
        fprintf(stderr, _("ERROR: incorrect size value '%s'\n"), str);
        return EINA_FALSE;
     }
   v->x = 0;
   v->y = 0;

   return EINA_TRUE;
}
