#ifdef LINUX

 #define _GNU_SOURCE                // needed for vsnprintf
 #define vasprintf _gnu_vasprintf   // revoking vasprintf
 #include <stdio.h>
 #undef vasprintf

int
vscprintf(const char *format, va_list ap)
{
   va_list ap_copy;
   va_copy(ap_copy, ap);
   int retval = vsnprintf(NULL, 0, format, ap_copy);
   va_end(ap_copy);
   return retval;
}

#else // WINDOWS

 #include "stdio.h"
 #include <stdio.h>
  #define vscprintf _vscprintf
  #define vsnprintf _vsnprintf
 #endif

#include <stdlib.h>

// REFS:
// https://linux.die.net/man/3/vasprintf
// https://stackoverflow.com/questions/40159892/using-asprintf-on-windows
// https://rextester.com/BTNY65545
// http://insanecoding.blogspot.com/2014/06/memory-management-in-c-and-auto.html

int
vasprintf(char **strp, const char *fmt, va_list ap)
{
   int ret = -1;

   int len = vscprintf(fmt, ap);
   if (len != -1)
     {
        char *str = (char *)malloc((len + 1) * sizeof(char));
        if (str != NULL) 
          {
             ret = vsnprintf(str, (unsigned long) len + 1, fmt, ap);
             if (ret == len) *strp = str;
             else            free(strp);
          }
     }

   return ret;
}
