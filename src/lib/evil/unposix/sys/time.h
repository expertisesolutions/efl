#ifndef UNPOSIX_SYS_TIME_H
#define UNPOSIX_SYS_TIME_H

#include "unposix_api.h"

#include <define_lean_and_mean.h>
#include <require_lean_and_mean.h>

#include <winsock2.h>
#include <define_lean_and_mean.h>
#include <minwinbase.h>
#include <define_lean_and_mean.h>
#include <sysinfoapi.h>
#include <define_lean_and_mean.h>
#include <timezoneapi.h>

#include "define_lean_and_mean.h"
#include <stdint.h>

#include "define_lean_and_mean.h"
#include <../ucrt/time.h>

#include "define_lean_and_mean.h"
#include <sys/types.h>

typedef unsigned short u_short;

UNPOSIX_API struct timezone {
  int tz_minuteswest;     /* minutes west of Greenwich */
  int tz_dsttime;         /* type of DST correction */
};

UNPOSIX_API int gettimeofday(struct timeval * tp, struct timezone * tzp);

UNPOSIX_API struct tm *localtime_r(const time_t * time, struct tm * result);

#endif
