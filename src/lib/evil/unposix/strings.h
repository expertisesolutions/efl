#ifndef UNPOSIX_STRINGS_H
#define UNPOSIX_STRINGS_H

#include <sys/types.h>
#include "locale.h"


#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 
#endif
#include <Windows.h>

inline int ffs(int i);
inline int strcasecmp(const char* s1, const char* s2);
inline int strncasecmp(const char* s1, const char* s2, size_t size);
inline int strcasecmp_l(const char* s1, const char* s2, locale_t locale);
inline int strncasecmp_l(const char* s1, const char* s2, size_t size, locale_t locale);
#endif
