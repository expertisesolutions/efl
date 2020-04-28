/* EINA - EFL data type library
 * Copyright (C) 2020 Lucas Cavalcante de Sousa
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "eina_types.h"
#include "eina_time.h"
# include <evil_private.h> /* windows.h localtime_r */

#include <stdint.h>
#include <time.h>

#if !defined(_WIN32) && defined(HAVE_SYS_TIME_H)
# include <sys/time.h>
#endif /* !_WIN32 && HAVE_SYS_TIME_H */


EAPI int
eina_gettimeofday(struct timeval * tp, struct timezone * tzp)
{
#ifdef _WIN32
   // This number is the number of 100 nanosecond intervals since 00:00:00
   // 01/01/1601 (UTC) (Windows way) until 00:00:00 01/01/1970 (POSIX way)
   static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

   SYSTEMTIME system_time;
   FILETIME file_time;
   uint64_t time;

   GetSystemTime (&system_time);
   SystemTimeToFileTime (&system_time, &file_time );
   time = (uint64_t) file_time.dwLowDateTime;
   time += ((uint64_t) file_time.dwHighDateTime) << 32;

   tp->tv_sec = (long) ((time - EPOCH) / 10000000L);
   tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
   return 0;
#else /* POSIX */
   return gettimeofday(tp, tzp);
#endif /* _WIN32 || POSIX */
}

EAPI struct tm *
eina_localtime_r(const time_t * time, struct tm * result)
{
#ifdef _WIN32
   return evil_localtime_r(time, result);
#else /* POSIX */
   return localtime_r(time, result);
#endif /* _WIN32 || POSIX */
}
