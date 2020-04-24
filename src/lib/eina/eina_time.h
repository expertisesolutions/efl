#ifndef EINA_TIME_H
#define EINA_TIME_H

#include "eina_types.h"

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif /* WIN32_LEAN_AND_MEAN */

# include <windows.h>
# include <winsock2.h>
# include <minwinbase.h>
# include <sysinfoapi.h>
# include <timezoneapi.h>

# undef WIN32_LEAN_AND_MEAN
#else /* POSIX */
# include <sys/time.h>
#endif /* WIN32 || POSIX */

#include <stdint.h>
#include <time.h>
#include <sys/types.h>

#ifdef _WIN32
typedef unsigned short u_short;

struct timezone {
  int tz_minuteswest;     /* minutes west of Greenwich */
  int tz_dsttime;         /* type of DST correction */
};
#endif /* _WIN32 */

EAPI int eina_gettimeofday(struct timeval * tp, struct timezone * tzp);

EAPI struct tm *eina_localtime_r(const time_t * time
                                       , struct tm * result);
#endif /* EINA_TIME_H */
