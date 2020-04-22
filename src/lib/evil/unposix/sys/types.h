#ifndef UNPOSIX_SYS_TYPES_H
#define UNPOSIX_SYS_TYPES_H

#ifndef _CRT_DECLARE_NONSTDC_NAMES
#define _CRT_DECLARE_NONSTDC_NAMES
#endif

#include_next <sys/types.h>

#ifdef _CRT_DECLARE_NONSTDC_NAMES
#undef _CRT_DECLARE_NONSTDC_NAMES
#endif

#ifdef WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif

# include <BaseTsd.h>
# include <crtdefs.h>

typedef SSIZE_T ssize_t;

# undef WIN32_LEAN_AND_MEAN
#endif

#include <stdint.h>
#include <time.h>
#include <wchar.h>

typedef int clockid_t;
typedef int gid_t;
typedef int mode_t;
typedef unsigned long pid_t;
typedef long suseconds_t;
typedef int uid_t;

#endif
