#ifndef __EVIL_SYS_TIME_H__
#define __EVIL_SYS_TIME_H__

#include <stdint.h>
#include <time.h>

#include <winsock2.h> // struct timeval

/**
 * @file evil_sys_time.h
 * @brief The file that provides functions ported from Unix in syy/time.h.
 * @defgroup Evil_Sys_Time_Group Sys_Time.h functions
 * @ingroup Evil
 *
 * This header provides functions ported from Unix in time.h.
 *
 * @{
 */


#ifndef HAVE_GETTIMEOFDAY
# define HAVE_GETTIMEOFDAY
# define gettimeofday evil_gettimeofday

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

#endif

EVIL_API int evil_gettimeofday(struct timeval * tp, struct timezone * tzp);

/**
 * @}
 */


#endif /* __EVIL_TIME_H__ */
