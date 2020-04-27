#include "eina_stdio.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


int vasprintf_wrap(char **ret, const char *fmt, int nargs, ...);
int simple_test(void);

int vasprintf_wrap(char **ret, const char *fmt, int nargs, ...)
{
   va_list ap;
   va_start(ap, nargs);

   int len = vasprintf(ret, fmt, ap);

   va_end(ap);
   return len;
}

int
simple_test(void)
{
   char *str;
   int len;

   const char *expected_str = 
     "\tString: Hello World!\n"
     "\tInteger: 46234\n"
     "\tDouble: 9.629400E-03\n";

   const char *fmt = 
     "\tString: %s\n"
     "\tInteger: %d\n"
     "\tDouble: %E\n";

   len = vasprintf_wrap(&str, fmt, 3, "Hello World!", 46234, 0.0096294);

   printf("## Expecting:\n```%s```\n", expected_str);
   printf("## Returning:\n```%s```\n", str);

   printf("\tResult:"); 
   int result = strcmp(str, expected_str);
   if (!result) printf("........ok!\n");
   else printf("........nok!\n");

   free(str);
   return result;
}

int
main(void)
{
#ifdef GNU
   printf("Test using vasprintf from GNU:\n");
#else
   printf("Test using custom vasprintf:\n");
#endif

   simple_test();

   return 0;
}
