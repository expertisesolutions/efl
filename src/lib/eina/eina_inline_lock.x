/* EINA - EFL data type library
 * Copyright (C) 2011 Vincent Torri
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

#ifndef EINA_INLINE_LOCK_X_
#define EINA_INLINE_LOCK_X_

#ifdef _XOPEN_SOURCE
# define EINA_XOPEN_SOURCE _XOPEN_SOURCE
# undef _XOPEN_SOURCE
#endif
#define _XOPEN_SOURCE 600

#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>

#include <sys/types.h>

#ifdef EINA_HAVE_DEBUG_THREADS
#include <assert.h>
#include <execinfo.h>
#define EINA_LOCK_DEBUG_BT_NUM 64
typedef void (*Eina_Lock_Bt_Func) ();

#include "eina_inlist.h"
#endif

EAPI void _eina_lock_debug_abort(int err, const char *fn, const volatile void *ptr);
EAPI void _eina_lock_debug_deadlock(const char *fn, const volatile void *ptr);

#define EINA_LOCK_ABORT_DEBUG(err, fn, ptr) \
   _eina_lock_debug_abort(err, #fn, ptr)
#define EINA_LOCK_DEADLOCK_DEBUG(fn, ptr) \
   _eina_lock_debug_deadlock(#fn, ptr)

/* For cond_timedwait */
#include <time.h>

#include <eina_error.h>

typedef struct _Eina_Lock Eina_Lock;
typedef struct _Eina_RWLock Eina_RWLock;
typedef struct _Eina_Condition Eina_Condition;

#if !defined(EINA_HAVE_POSIX_SPINLOCK) && !defined(EINA_HAVE_WIN32_SPINLOCK) && \
!defined(EINA_HAVE_OSX_SPINLOCK)
typedef Eina_Lock Eina_Spinlock;
#endif

EAPI void eina_lock_debug(const Eina_Lock *mutex);

EAPI extern Eina_Bool _eina_threads_activated;

#ifdef EINA_HAVE_DEBUG_THREADS
EAPI extern int _eina_threads_debug;
EAPI extern Eina_Inlist *_eina_tracking;
#endif

#ifdef _WIN32
#include <eina_inline_lock_win32.x>
#else
#include <eina_inline_lock_posix.x>
#endif


EAPI Eina_Bool _eina_lock_new(Eina_Lock *mutex, Eina_Bool recursive);
EAPI void      _eina_lock_free(Eina_Lock *mutex);
EAPI Eina_Bool _eina_condition_new(Eina_Condition *cond, Eina_Lock *mutex);
EAPI void      _eina_condition_free(Eina_Condition *cond);
EAPI Eina_Bool _eina_rwlock_new(Eina_RWLock *mutex);
EAPI void      _eina_rwlock_free(Eina_RWLock *mutex);
EAPI Eina_Bool _eina_spinlock_new(Eina_Spinlock *spinlock);
EAPI void      _eina_spinlock_free(Eina_Spinlock *spinlock);
EAPI Eina_Bool _eina_semaphore_new(Eina_Semaphore *sem, int count_init);
EAPI Eina_Bool _eina_semaphore_free(Eina_Semaphore *sem);

static inline Eina_Bool
eina_lock_new(Eina_Lock *mutex)
{
   Eina_Bool ret = _eina_lock_new(mutex, EINA_FALSE);
#ifdef EINA_HAVE_DEBUG_THREADS
   mutex->recursive = EINA_FALSE;
   mutex->lock_thread_id = 0;
   mutex->lock_bt_num = 0;
   mutex->locked = 0;
#endif
   return ret;
}

static inline Eina_Bool
eina_lock_recursive_new(Eina_Lock *mutex)
{
   Eina_Bool ret = _eina_lock_new(mutex, EINA_TRUE);
#ifdef EINA_HAVE_DEBUG_THREADS
   mutex->recursive = EINA_TRUE;
   mutex->lock_thread_id = 0;
   mutex->lock_bt_num = 0;
   mutex->locked = 0;
#endif
   return ret;
}

static inline Eina_Bool
eina_condition_new(Eina_Condition *cond, Eina_Lock *mutex)
{
   return _eina_condition_new(cond, mutex);
}

static inline void
eina_condition_free(Eina_Condition *cond)
{
   _eina_condition_free(cond);
}

static inline Eina_Bool
eina_rwlock_new(Eina_RWLock *mutex)
{
   return _eina_rwlock_new(mutex);
}

static inline void
eina_rwlock_free(Eina_RWLock *mutex)
{
   _eina_rwlock_free(mutex);
}

#if !defined(EINA_HAVE_PTHREAD_BARRIER) && !defined(EINA_HAVE_WIN32_BARRIER)
#include "eina_inline_lock_barrier.x"
#endif

EAPI Eina_Bool _eina_barrier_new(Eina_Barrier *barrier, int needed);
EAPI void      _eina_barrier_free(Eina_Barrier *barrier);

static inline Eina_Bool
eina_barrier_new(Eina_Barrier *barrier, int needed)
{
   return _eina_barrier_new(barrier, needed);
}

static inline void
eina_barrier_free(Eina_Barrier *barrier)
{
   _eina_barrier_free(barrier);
}


static inline Eina_Bool
eina_spinlock_new(Eina_Spinlock *spinlock)
{
   return _eina_spinlock_new(spinlock);
}

static inline void
eina_spinlock_free(Eina_Spinlock *spinlock)
{
   _eina_spinlock_free(spinlock);
}

static inline Eina_Bool
eina_semaphore_new(Eina_Semaphore *sem, int count_init)
{
   return _eina_semaphore_new(sem, count_init);
}

static inline Eina_Bool
eina_semaphore_free(Eina_Semaphore *sem)
{
   return _eina_semaphore_free(sem);
}

#undef _XOPEN_SOURCE
// This is necessary to let third party still define this macro
#ifdef EINA_XOPEN_SOURCE
# define _XOPEN_SOURCE EINA_XOPEN_SOURCE
#endif

#endif
