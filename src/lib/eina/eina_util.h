/* EINA - EFL data type library
 * Copyright (C) 2015 Vincent Torri
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

#ifndef EINA_UTIL_H_
#define EINA_UTIL_H_

#ifdef _WIN32
# include <string.h>
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <Windows.h>
# undef WIN32_LEAN_AND_MEAN
#else
# include <pwd.h>
#endif
#include <unistd.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "eina_config.h"
#include "eina_private.h"
#include "eina_tmpstr.h"

/**
 * @addtogroup Eina_Tools_Group Tools
 *
 * @{
 */

/**
 * @brief Suspends the execution of calling thread until time specified by seconds.
 * @param[in] Time in seconds for which execution is to be suspendend.
 * 
 * @since 1.15
 */
EAPI void eina_sleep(unsigned int seconds);

/**
 * @brief Suspends the execution of calling thread until time specified by microseconds.
 * @param[in] Time in microseconds for which execution is to be suspendend.
 * 
 * @since 1.15
 */
EAPI void eina_usleep(unsigned int seconds);

/**
 * @brief Changes the ownership of the file specified by pathname.
 * @param[in] The directory path to change owner.
 * @param[in] The new user to set.
 * @param[in] The new group to set.
 * 
 * @since 1.15
 */
EAPI int eina_chown(const char *pathname, uid_t owner, gid_t group);

/**
 * @brief Places the contents of the symbolic link pathname in the buffer buf, which has size bufsiz.
 * @param[in] The symbolic link pathname.
 * @param[out] The destination buffer to store pathname.
 * @param[in] The size of the destination buf.
 * @return Return the numbers of bytes placed in buf. If returned value equals bufsize, then truncation may have ocurred.
 * 
 * @since 1.15
 */
EAPI int eina_readlink(const char *pathname, char *buf, size_t bufsize);

/**
 * @brief Creates a new session if the calling process is not a process group leader.
 * @return On success, the (new) session ID of the calling process is returned. On error, 
 * (pid_t) -1 is returned, and errno is set to indicate the error.
 * 
 * @since 1.15
 */
EAPI int eina_setsid(void);

/**
 * @brief Returns the content of the environment referred by HOME on this system.
 * @return A temporary string to the content referred by HOME on this system.
 *
 * @note The result of this call is highly system dependent and you better use
 * it instead of the naive getenv("HOME").
 *
 * @since 1.15
 */
EAPI const char *eina_environment_home_get(void);

/**
 * @brief Returns the content of the environment referred as TMPDIR on this system.
 * @return A temporary string to the content referred by TMPDIR on this system.
 *
 * @note The result of this call is highly system dependent and you better use
 * it instead of the naive getenv("TMPDIR").
 *
 * @since 1.15
 */
EAPI const char *eina_environment_tmp_get(void);

/**
 * @brief Warningless comparison of doubles using ==
 * @param[in] a First member to compare
 * @param[in] b Second member to compare
 *
 * @return @c true if two doubles match
 * @since 1.19
 */
static inline Eina_Bool eina_dbl_exact(double a, double b);

/**
 * @brief Warningless comparison of floats using ==
 * @param[in] a First member to compare
 * @param[in] b Second member to compare
 *
 * @return @c true if two floats match
 * @since 1.19
 */
static inline Eina_Bool eina_flt_exact(float a, float b);

/**
 * @brief Safe comparison of float
 * @param[in] a First member to compare
 * @param[in] b Second member to compare
 * @since 1.19
 *
 * @return @c true if two floats match
 */
#define EINA_FLT_EQ(a, b) (!!(fabsf((float)(a) - (float)(b)) <= FLT_EPSILON))

/**
 * @brief Determines if a float is not zero
 * @param[in] a The float
 *
 * @return @c true if float is not zero
 * @since 1.19
 */
#define EINA_FLT_NONZERO(a) (!!(fpclassify((float)(a)) != FP_ZERO))

/**
 * @brief Safe comparison of double
 * @param[in] a First member to compare
 * @param[in] b Second member to compare
 * @since 1.19
 *
 * @return @c true if two double match
 */
#define EINA_DBL_EQ(a, b) (!!(fabs((double)(a) - (double)(b)) <= DBL_EPSILON))

/**
 * @brief Determines if a double is not zero
 * @param[in] a The double
 *
 * @return @c true if double is not zero
 * @since 1.19
 */
#define EINA_DBL_NONZERO(a) (!!(fpclassify((double)(a)) != FP_ZERO))

/**
 * @}
 */

#include "eina_inline_util.x"

#endif
