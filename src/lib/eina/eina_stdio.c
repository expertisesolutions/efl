#include "eina_stdio.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>


int
eina_vasprintf(char **strp, const char *fmt, va_list ap)
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
