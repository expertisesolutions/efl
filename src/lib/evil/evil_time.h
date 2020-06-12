#ifndef __EVIL_TIME_H__
#define __EVIL_TIME_H__

#include <evil_api.h>

#include <stdint.h>
#include <time.h>

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h> // strcut timeval
#undef WIN32_LEAN_AND_MEAN

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


static int
gettimeofday(struct timeval * tp, struct timezone * tzp)
{
   // This number is the number of 100 nanosecond intervals since 00:00:00
   // 01/01/1601 (UTC) (Windows way) until 00:00:00 01/01/1970 (POSIX way)
   static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

   SYSTEMTIME system_time;
   FILETIME file_time;
   uint64_t time;

   GetSystemTime(&system_time);
   SystemTimeToFileTime( &system_time, &file_time );
   time =  ((uint64_t)file_time.dwLowDateTime )      ;
   time += ((uint64_t)file_time.dwHighDateTime) << 32;

   tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
   tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
   return 0;
}


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


/**
 * @}
 */


#endif /* __EVIL_TIME_H__ */
