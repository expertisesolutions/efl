/* EINA - EFL data type library
 * Copyright (C) 2020 Carlos Signor
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

#ifndef EINA_THREAD_POSIX_H
#define EINA_THREAD_POSIX_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "eina_thread.h"
#include <pthread.h>
#include "eina_safety_checks.h"
#include <signal.h>

#define EINA_THREAD_CANCEL_ENABLE PTHREAD_CANCEL_ENABLE
#define EINA_THREAD_CANCEL_DISABLE PTHREAD_CANCEL_DISABLE
#define EINA_THREAD_CANCEL_DEFERRED PTHREAD_CANCEL_DEFERRED
#define EINA_THREAD_CANCEL_ASYNCHRONOUS PTHREAD_CANCEL_ASYNCHRONOUS
#define EINA_THREAD_CANCELED   PTHREAD_CANCELED

/**
 * @def EINA_THREAD_CLEANUP_PUSH(cleanup, data)
 *
 * @brief Pushes a cleanup function to be executed when the thread is
 * canceled.
 *
 * This macro will schedule a function cleanup(data) to be executed if
 * the thread is canceled with eina_thread_cancel() and the thread
 * was previously marked as cancellable with
 * eina_thread_cancellable_set().
 *
 * It @b must be paired with EINA_THREAD_CLEANUP_POP() in the same
 * code block as they will expand to do {} while ()!
 *
 * The cleanup function may also be executed if
 * EINA_THREAD_CLEANUP_POP(EINA_TRUE) is used.
 *
 * @note If the block within EINA_THREAD_CLEANUP_PUSH() and
 *       EINA_THREAD_CLEANUP_POP() returns, the cleanup callback will
 *       @b not be executed! To avoid problems prefer to use
 *       eina_thread_cancellable_run()!
 *
 * @param[in] cleanup The function to execute on cancellation.
 * @param[in] data The context to give to cleanup function.
 *
 * @see eina_thread_cancellable_run()
 *
 * @since 1.19
 */
#define EINA_THREAD_CLEANUP_PUSH(cleanup, data) \
  pthread_cleanup_push(cleanup, data)

/**
 * @def EINA_THREAD_CLEANUP_POP(exec_cleanup)
 *
 * @brief Pops a cleanup function to be executed when the thread is
 * canceled.
 *
 * This macro will remove a previously pushed cleanup function, thus
 * if the thread is canceled with eina_thread_cancel() and the thread
 * was previously marked as cancellable with
 * eina_thread_cancellable_set(), that cleanup won't be executed
 * anymore.
 *
 * It @b must be paired with EINA_THREAD_CLEANUP_PUSH() in the same
 * code block as they will expand to do {} while ()!
 *
 * @note If the block within EINA_THREAD_CLEANUP_PUSH() and
 *       EINA_THREAD_CLEANUP_POP() returns, the cleanup callback will
 *       @b not be executed even if exec_cleanup is EINA_TRUE! To
 *       avoid problems prefer to use eina_thread_cancellable_run()!
 *
 * @param[in] exec_cleanup if EINA_TRUE, the function registered with
 *        EINA_THREAD_CLEANUP_PUSH() will be executed.
 *
 * @see eina_thread_cancellable_run()
 *
 * @since 1.19
 */
#define EINA_THREAD_CLEANUP_POP(exec_cleanup) \
  pthread_cleanup_pop(exec_cleanup)




#if defined(EINA_HAVE_PTHREAD_AFFINITY) || defined(EINA_HAVE_PTHREAD_SETNAME)
# ifndef __linux__
#  include <pthread_np.h>
#  define cpu_set_t cpuset_t
# endif
#endif
typedef uintptr_t Eina_Thread;
/**
 * @typedef Eina_Thread
 * Type for a generic thread.
 */

#if defined HAVE_PTHREAD_GETTHREADID_NP
typedef int Eina_ThreadId;
#elif defined __linux__
typedef pid_t Eina_ThreadId;
#else
typedef size_t Eina_ThreadId;
#endif

#endif
