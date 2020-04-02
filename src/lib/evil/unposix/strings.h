#ifndef UNPOSIX_STRINGS_H
#define UNPOSIX_STRINGS_H

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 
#endif
#include <Windows.h>
#include_next<string.h>

#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define strncasecmp_l _strnicmp_l
#define strcasecmp_l _strncmp_l
inline int ffs(int i)
#endif
