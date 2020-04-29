#ifndef EINA_STDLIB_H
#define EINA_STDLIB_H

#include <stdlib.h>

#ifdef _WIN32

int eina_mkstemp(char* template);
int eina_mkostemp(char* template, int flags);
int eina_mkostemps(char* template, int suffixlen, int flags);

#else

#define eina_mktemp mkstemp
#define eina_mkostemp mkostemp
#define eina_mkostemps mkostemps

#endif /* _WIN32 */

#endif
