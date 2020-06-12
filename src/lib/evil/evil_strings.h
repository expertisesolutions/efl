#ifndef EVIL_STRINGS_H__
#define EVIL_STRINGS_H__

#include <evil_api.h>
#include <evil_windows.h>

#ifndef _WIN32
# include <string.h>
#endif

#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define strncasecmp_l _strnicmp_l
#define strcasecmp_l _strncmp_l

EVIL_API int ffs(int i);

#endif
