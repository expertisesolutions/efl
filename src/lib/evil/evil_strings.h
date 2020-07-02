#ifndef EVIL_STRINGS_H__
#define EVIL_STRINGS_H__

#include <evil_private.h>

#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define strncasecmp_l _strnicmp_l
#define strcasecmp_l _strncmp_l

EVIL_API int ffs(int i);

#endif
