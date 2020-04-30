#ifndef UNPOSIX_SYS_TIME_H
#define UNPOSIX_SYS_TIME_H

#include "unposix_api.h"

#ifdef WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
#endif

#include <winsock2.h>
#include <minwinbase.h>
#include <sysinfoapi.h>
#include <timezoneapi.h>

#include <stdint.h>

#include <../ucrt/time.h>

#include <sys/types.h>

typedef unsigned short u_short;

UNPOSIX_API struct timezone {
  int tz_minuteswest;     /* minutes west of Greenwich */
  int tz_dsttime;         /* type of DST correction */
};

UNPOSIX_API int gettimeofday(struct timeval * tp, struct timezone * tzp);

UNPOSIX_API struct tm *localtime_r(const time_t * time, struct tm * result);

#endif
