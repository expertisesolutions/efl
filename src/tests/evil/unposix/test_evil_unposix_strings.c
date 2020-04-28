#include "strings.h"
#include <stdio.h>

int
equals_strings()
{   
   return strcasecmp("fOo","FoO");
}

int
str1_lessthan_str2()
{
   return strcasecmp("fOo","fOoO");
}

int
str1_greaterthan_str2()
{
   return strcasecmp("fOoO","fOo");
}

int
str1_n_equals_str2()
{
   return strncasecmp("fOoO","fOo",3);
}

int
main()
{
    
   printf("str1 equal str2 (should be 0): %d\n", equals_strings());
   printf("str1 less than str2 (should be < 0): %d\n", str1_lessthan_str2());    
   printf("str1 greater than str2 (should be > 0): %d\n", str1_greaterthan_str2());
   printf("str1 first 3 chars equals str2 (should be 0): %d\n", str1_n_equals_str2());

   return 0;
}

