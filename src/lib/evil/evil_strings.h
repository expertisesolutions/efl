#ifndef EVIL_STRINGS_H__
#define EVIL_STRINGS_H__

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <Windows.h>
# undef WIN32_LEAN_AND_MEAN
#else
# include<string.h>
#endif

#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define strncasecmp_l _strnicmp_l
#define strcasecmp_l _strncmp_l

int ffs(int i);

#endif