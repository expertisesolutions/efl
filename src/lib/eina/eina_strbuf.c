#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "eina_private.h"
#include "eina_str.h"
#include "eina_strbuf_common.h"
#include "eina_unicode.h"

#ifdef _WIN32
# include <evil_private.h> /* vasprintf */
#endif /* _WIN32 */

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#ifdef _STRBUF_DATA_TYPE
# undef _STRBUF_DATA_TYPE
#endif

#ifdef _STRBUF_CSIZE
# undef _STRBUF_CSIZE
#endif

#ifdef _STRBUF_STRUCT_NAME
# undef _STRBUF_STRUCT_NAME
#endif

#ifdef _STRBUF_STRLEN_FUNC
# undef _STRBUF_STRLEN_FUNC
#endif

#ifdef _STRBUF_STRESCAPE_FUNC
# undef _STRBUF_STRESCAPE_FUNC
#endif

#ifdef _STRBUF_MAGIC
# undef _STRBUF_MAGIC
#endif

#ifdef _STRBUF_MAGIC_STR
# undef _STRBUF_MAGIC_STR
#endif

#ifdef _FUNC_EXPAND
# undef _FUNC_EXPAND
#endif


#define _STRBUF_DATA_TYPE         char
#define _STRBUF_CSIZE             sizeof(_STRBUF_DATA_TYPE)
#define _STRBUF_STRUCT_NAME       Eina_Strbuf
#define _STRBUF_STRLEN_FUNC(x)    strlen(x)
#define _STRBUF_STRESCAPE_FUNC(x) eina_str_escape(x)
#define _STRBUF_MAGIC             EINA_MAGIC_STRBUF
#define _STRBUF_MAGIC_STR         __STRBUF_MAGIC_STR
static const char __STRBUF_MAGIC_STR[] = "Eina Strbuf";

#define _FUNC_EXPAND(y) eina_strbuf_ ## y

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


Eina_Bool
eina_strbuf_append_printf(Eina_Strbuf *buf, const char *fmt, ...)
{
   va_list args;
   char *str;
   size_t len;
   Eina_Bool ret;

   va_start(args, fmt);
   len = vasprintf(&str, fmt, args);
   va_end(args);

   if (len == 0 || !str)
      return EINA_FALSE;

   ret = eina_strbuf_append_length(buf, str, len);
   free(str);
   return ret;
}

Eina_Bool
eina_strbuf_append_vprintf(Eina_Strbuf *buf, const char *fmt, va_list args)
{
   char *str;
   size_t len;
   Eina_Bool ret;

   len = vasprintf(&str, fmt, args);

   if (len == 0 || !str)
      return EINA_FALSE;

   ret = eina_strbuf_append_length(buf, str, len);
   free(str);
   return ret;
}

Eina_Bool
eina_strbuf_insert_printf(Eina_Strbuf *buf, const char *fmt, size_t pos, ...)
{
   va_list args;
   char *str;
   size_t len;
   Eina_Bool ret;

   va_start(args, pos);
   len = vasprintf(&str, fmt, args);
   va_end(args);

   if (len == 0 || !str)
      return EINA_FALSE;

   ret = eina_strbuf_insert(buf, str, pos);
   free(str);
   return ret;
}

Eina_Bool
eina_strbuf_insert_vprintf(Eina_Strbuf *buf,
                           const char *fmt,
                           size_t pos,
                           va_list args)
{
   char *str;
   size_t len;
   Eina_Bool ret;

   len = vasprintf(&str, fmt, args);

   if (len == 0 || !str)
      return EINA_FALSE;

   ret = eina_strbuf_insert(buf, str, pos);
   free(str);
   return ret;
}

void
eina_strbuf_trim(Eina_Strbuf *buf)
{
   unsigned char *c = buf->buf;

   while (buf->len > 0 && isspace(c[buf->len - 1]))
     buf->len--;
   while (buf->len > 0 && isspace(*c))
     {
        c++;
        buf->len--;
     }
   memmove(buf->buf, c, buf->len);
   ((unsigned char *)buf->buf)[buf->len] = '\0';
}

void
eina_strbuf_ltrim(Eina_Strbuf *buf)
{
   unsigned char *c = buf->buf;

   while (buf->len > 0 && isspace(*c))
     {
        c++;
        buf->len--;
     }
   memmove(buf->buf, c, buf->len);
   ((unsigned char *)buf->buf)[buf->len] = '\0';
}

void
eina_strbuf_rtrim(Eina_Strbuf *buf)
{
   while (buf->len > 0 && isspace(((unsigned char*)(buf->buf))[buf->len - 1]))
     buf->len--;
   ((unsigned char *)buf->buf)[buf->len] = '\0';
}

void
eina_strbuf_tolower(Eina_Strbuf *buf)
{
   if (!buf || !(buf->buf)) return;

   eina_str_tolower((char **)&(buf->buf));
}

Eina_Strbuf *
eina_strbuf_substr_get(Eina_Strbuf *buf, size_t pos, size_t len)
{
   char *str;

   if ((!buf) || ((pos + len) > buf->len))
      return NULL;

   str = calloc(1, len + 1);

   strncpy(str,((char *)(buf->buf)) + pos, len);

   return eina_strbuf_manage_new(str);
}

Eina_Bool
eina_strbuf_append_strftime(Eina_Strbuf *buf, const char *format, const struct tm *tm)
{
   char *outputbuf;

   outputbuf = eina_strftime(format, tm);
   if (!outputbuf) return EINA_FALSE;

   eina_strbuf_append(buf, outputbuf);
   free(outputbuf);

   return EINA_TRUE;
}

Eina_Bool
eina_strbuf_insert_strftime(Eina_Strbuf *buf, const char *format, const struct tm *tm, size_t pos)
{
   char *outputbuf;

   outputbuf = eina_strftime(format, tm);
   if (!outputbuf) return EINA_FALSE;

   eina_strbuf_insert_length(buf, outputbuf, strlen(outputbuf), pos);
   free(outputbuf);

   return EINA_TRUE;
}

/* Unicode */

#include "eina_strbuf_template_c.x"
