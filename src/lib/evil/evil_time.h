#ifndef __EVIL_TIME_H__
#define __EVIL_TIME_H__

#include <evil_api.h>

#include <time.h>
#include <evil_windows.h>
#include <winsock2.h>

/**
 * @file evil_time.h
 * @brief The file that provides functions ported from Unix in time.h.
 * @defgroup Evil_Time_Group Time.h functions
 * @ingroup Evil
 *
 * This header provides functions ported from Unix in time.h.
 *
 * @{
 */

EVIL_API struct tm *evil_localtime_r(const time_t * time, struct tm * result);

#ifdef _MSC_VER
#define localtime_r evil_localtime_r
#endif

/**
 * @brief Convert a string representation of time to a time tm structure .
 *
 * @param buf The string to convert.
 * @param fmt The representation of time.
 * @param tm The time tm structure.
 * @return The first character not processed in this function call.
 *
 * This function converts the string @p s to a time tm structure and
 * fill the buffer @p tm. The format of the time is specified by
 * @p format. On success, this function returns the first character
 * not processed in this function call, @c NULL otherwise.
 *
 * Conformity: Non applicable.
 *
 * Supported OS: Windows XP.
 */
EVIL_API char *strptime(const char *buf, const char *fmt, struct tm *tm);

#ifdef _MSC_VER
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

EVIL_API int gettimeofday(struct timeval * tp, struct timezone * tzp);
#endif


/**
 * @}
 */


#endif /* __EVIL_TIME_H__ */
