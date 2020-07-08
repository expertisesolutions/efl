
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <evil_api.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

inline int
vasprintf(char **strp, const char *fmt, va_list ap)
{
   int ret = -1;

   int len = _vscprintf(fmt, ap);
   if (len != -1)
     {
        char *str = (char *)malloc((len + 1) * sizeof(char));
        if (str != NULL) 
          {
             ret = vsnprintf(str, (unsigned long) len + 1, fmt, ap);
             if (ret == len) *strp = str;
             else free(strp);
          }
     }

   return ret;
}

inline int
asprintf(char **strp, const char *fmt, ...)
{
   va_list args;
   va_start(args, fmt);
   int i = vasprintf(strp, fmt, args);
   va_end(args);
   return i;
}
