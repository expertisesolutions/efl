#ifndef UNPOSIX_STRINGS_H
#define UNPOSIX_STRINGS_H

#include_next<string.h>

#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define strncasecmp_l _strnicmp_l
#define strcasecmp_l _strncmp_l

inline int ffs(int i);
#endif
