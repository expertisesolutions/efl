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

#include "eina_thread_posix.h"

#ifdef HAVE_PTHREAD_NP_H
#include <pthread_np.h>
#endif

#ifdef __linux__
#include <sys/types.h>
#endif

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


static inline void *
_eina_thread_join(Eina_Thread t)
{
   void *ret = NULL;
   int err = pthread_join((pthread_t)t, &ret);

   if (err == 0) return ret;
   return NULL;
}

static inline Eina_Bool
_eina_thread_name_set(Eina_Thread thread, char *buf)
{
#ifndef __linux__
   pthread_set_name_np((pthread_t)t, buf);
   return EINA_TRUE;
#else
   return pthread_setname_np((pthread_t)thread, buf);
#endif
}

static inline Eina_Bool
_eina_thread_create(Eina_Thread *t, int affinity, void *(*func)(void *data), void *data)
{
   int err;
   pthread_attr_t attr;

   sigset_t oldset, newset;

   pthread_attr_init(&attr);
   if (affinity >= 0)
     {
   #ifdef EINA_HAVE_PTHREAD_AFFINITY
        cpu_set_t cpu;
        CPU_ZERO(&cpu);
        CPU_SET(affinity, &cpu);
        pthread_attr_setaffinity_np(&attr, sizeof(cpu), &cpu);
   #endif
     }

   /* setup initial locks */

   sigemptyset(&newset);
   sigaddset(&newset, SIGPIPE);
   sigaddset(&newset, SIGALRM);
   sigaddset(&newset, SIGCHLD);
   sigaddset(&newset, SIGUSR1);
   sigaddset(&newset, SIGUSR2);
   sigaddset(&newset, SIGHUP);
   sigaddset(&newset, SIGQUIT);
   sigaddset(&newset, SIGINT);
   sigaddset(&newset, SIGTERM);
#ifdef SIGPWR
   sigaddset(&newset, SIGPWR);
#endif
   pthread_sigmask(SIG_BLOCK, &newset, &oldset);

   err = pthread_create((pthread_t *)t, &attr, func, data);

   pthread_sigmask(SIG_SETMASK, &oldset, NULL);

   pthread_attr_destroy(&attr);

   if (err == 0) return EINA_TRUE;

   return EINA_FALSE;
}

static inline Eina_Bool
_eina_thread_equal(Eina_Thread t1, Eina_Thread t2)
{
   return !!pthread_equal((pthread_t)t1, (pthread_t)t2);
}

static inline Eina_Bool
_eina_thread_cancel(Eina_Thread thread)
{
   return pthread_cancel((pthread_t)thread);
}

static inline Eina_Thread
_eina_thread_self(void)
{
   return (Eina_Thread)pthread_self();
}

static inline Eina_ThreadId
_eina_thread_self_id(void)
{
#if defined HAVE_PTHREAD_GETTHREADID_NP
    return pthread_getthreadid_np();
#elif defined __linux__
    return gettid();
#else
    return (Eina_ThreadId) pthread_self();
#endif
}

static inline Eina_ThreadId
_eina_thread_id(Eina_Thread t)
{
    return (Eina_ThreadId) t;
}

static inline void
_eina_thread_setcanceltype(EINA_UNUSED int type, int *oldtype)
{
   pthread_setcanceltype(EINA_THREAD_CANCEL_DEFERRED, oldtype);
}

static inline int
_eina_thread_setcancelstate(int type, int *oldtype)
{
   return pthread_setcancelstate(type, oldtype);
}

static inline Eina_Bool
_eina_thread_cancellable_set(Eina_Bool cancellable, Eina_Bool *was_cancellable)
{
   int state = cancellable ? EINA_THREAD_CANCEL_ENABLE : EINA_THREAD_CANCEL_DISABLE;
   int old = 0;
   int r;

   /* enforce deferred in case users changed to asynchronous themselves */

   _eina_thread_setcanceltype(EINA_THREAD_CANCEL_DEFERRED, &old);

   r = _eina_thread_setcancelstate(state, &old);
   if (was_cancellable && r == 0)
     *was_cancellable = (old == EINA_THREAD_CANCEL_ENABLE);
   return r == 0;
}

static inline void
_eina_thread_cancel_checkpoint(){
   pthread_testcancel();
}
