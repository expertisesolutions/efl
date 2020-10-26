#include "evil_strings.h"

EVIL_API int
ffs(int i)
{
   for (int j = 0; j < sizeof(i)*8; j++)
     if (i & (1 << j)) return j;

   return 0;
}
