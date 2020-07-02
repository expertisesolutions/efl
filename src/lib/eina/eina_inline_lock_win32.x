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

#ifndef EINA_INLINE_LOCK_WIN32_X_
#define EINA_INLINE_LOCK_WIN32_X_

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <synchapi.h>
#undef WIN32_LEAN_AND_MEAN

#include <errno.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#ifdef EINA_HAVE_DEBUG_THREADS
#include <assert.h>
#include <execinfo.h>
#define EINA_LOCK_DEBUG_BT_NUM 64
typedef void (*Eina_Lock_Bt_Func) ();

#include "eina_inlist.h"
#endif

#include "eina_hash.h"

EINA_API void _eina_lock_debug_abort(int err, const char *fn, const volatile void *ptr);
EINA_API void _eina_lock_debug_deadlock(const char *fn, const volatile void *ptr);

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
typedef SYNCHRONIZATION_BARRIER Eina_Barrier;
typedef DWORD Eina_TLS;
typedef HANDLE Eina_Semaphore;

typedef volatile LONG Eina_Spinlock;

EINA_API void eina_lock_debug(const Eina_Lock *mutex);

/** @privatesection  @{ */
typedef CRITICAL_SECTION _Eina_Mutex_t;
typedef CONDITION_VARIABLE _Eina_Condition_t;
typedef SRWLOCK _Eina_RWLock_t;

struct _Eina_Lock
{
   _Eina_Mutex_t     mutex; /**< The mutex that handles the locking */
#ifdef EINA_HAVE_DEBUG_THREADS
   EINA_INLIST; /**< Keeps track of the threads waiting for the lock */
   _Eina_Thread         lock_thread_id; /**< The ID of the thread that currently has the lock */
   Eina_Lock_Bt_Func lock_bt[EINA_LOCK_DEBUG_BT_NUM]; /**< The function that will produce a backtrace on the thread that has the lock */
   int               lock_bt_num; /**< Number of addresses in the backtrace */
   Eina_Bool         locked : 1;  /**< Indicates locked or not locked */
   Eina_Bool         recursive : 1;  /**< Indicates recursive lock */
#endif /* EINA_HAVE_DEBUG_THREADS */
};

struct _Eina_Condition
{
   Eina_Lock         *lock;      /**< The lock for this condition */
   _Eina_Condition_t condition; /**< The condition variable */
};

enum _Eina_RWLock_Mode {
   _Eina_RWLock_Mode_Shared,
   _Eina_RWLock_Mode_Exclusive,
   _Eina_RWLock_Mode_None
};

struct _Eina_RWLock
{
   _Eina_RWLock_t mutex; /**< The mutex that handles the locking */
   enum _Eina_RWLock_Mode mode; /**< The mode the SRWLock is (Shared, Exclusive or None)*/
};
/** @} privatesection */

EINA_API extern Eina_Bool _eina_threads_activated;

#ifdef EINA_HAVE_DEBUG_THREADS
EINA_API extern int _eina_threads_debug;
EINA_API extern _Eina_Thread _eina_main_loop;
EINA_API extern Eina_Lock _eina_tracking_lock;
EINA_API extern Eina_Inlist *_eina_tracking;
#endif

EINA_API Eina_Bool eina_lock_new(Eina_Lock *mutex);
EINA_API void eina_lock_free(Eina_Lock *mutex);
EINA_API Eina_Lock_Result eina_lock_take(Eina_Lock *mutex);
EINA_API Eina_Lock_Result eina_lock_take_try(Eina_Lock *mutex);
EINA_API Eina_Lock_Result eina_lock_release(Eina_Lock *mutex);
EINA_API Eina_Bool eina_condition_new(Eina_Condition *cond, Eina_Lock *mutex);
EINA_API void eina_condition_free(Eina_Condition *cond);
EINA_API Eina_Bool eina_condition_wait(Eina_Condition *cond);
EINA_API Eina_Bool eina_condition_broadcast(Eina_Condition *cond);

extern Eina_Hash *_eina_tls_map;
extern Eina_Lock _eina_tls_map_lock;

void _eina_free_tls_value(Eina_TLS *key, void *val);

static inline Eina_Bool
_eina_lock_new(Eina_Lock *mutex, Eina_Bool recursive)
{
   InitializeCriticalSection(&mutex->mutex);
   return EINA_TRUE;
}

static inline void
_eina_lock_free(Eina_Lock *mutex)
{
#ifdef EINA_HAVE_DEBUG_THREADS
   if (mutex->locked)
     {
        eina_lock_take(_eina_tracking_lock);
        _eina_tracking = eina_inlist_remove(_eina_tracking,
                                            EINA_INLIST_GET(mutex));
        eina_lock_release(_eina_tracking_lock);
     }
#endif

   DeleteCriticalSection(&mutex->mutex);
}

static inline Eina_Lock_Result
_eina_lock_take_try(Eina_Lock *mutex)
{
#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated) return EINA_LOCK_SUCCEED;
#endif

   BOOL ok = TryEnterCriticalSection(&(mutex->mutex));
   Eina_Lock_Result ret = ok ? EINA_LOCK_SUCCEED : EINA_LOCK_FAIL;

#ifdef EINA_HAVE_DEBUG_THREADS
   if (ret == EINA_LOCK_SUCCEED)
     {
        /* recursive locks can't make use of any of this */
        if (mutex->recursive) return ret;
        mutex->locked = 1;
        mutex->lock_thread_id = GetCurrentThreadId();
        /* backtrace() can somehow generate EINVAL even though this is not documented anywhere? */
        int err = errno;
        mutex->lock_bt_num = backtrace((void **)(mutex->lock_bt), EINA_LOCK_DEBUG_BT_NUM);
        errno = err;

        eina_lock_take(_eina_tracking_lock);
        _eina_tracking = eina_inlist_append(_eina_tracking,
                                            EINA_INLIST_GET(mutex));
        eina_lock_release(_eina_tracking_lock);
     }
#endif

   return ret;
}

static inline Eina_Lock_Result
_eina_lock_take(Eina_Lock *mutex)
{
#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated) return EINA_LOCK_SUCCEED;
#endif

#ifdef EINA_HAVE_DEBUG_THREADS
   if (_eina_threads_debug >= 100)
     {
        struct timeval t0, t1;
        int dt;

        gettimeofday(&t0, NULL);
        EnterCriticalSection(&mutex->mutex);
        gettimeofday(&t1, NULL);

        dt = (t1.tv_sec - t0.tv_sec) * 1000000;
        if (t1.tv_usec > t0.tv_usec) dt += (t1.tv_usec - t0.tv_usec);
        else dt -= t0.tv_usec - t1.tv_usec;
        if (dt > _eina_threads_debug) abort();
     }
   else
     {
#endif
        EnterCriticalSection(&mutex->mutex);
#ifdef EINA_HAVE_DEBUG_THREADS
     }
#endif

#ifdef EINA_HAVE_DEBUG_THREADS
   /* recursive locks can't make use of any of this */
   if (mutex->recursive) return ret;
   mutex->locked = 1;
   mutex->lock_thread_id = GetCurrentThreadId();
   /* backtrace() can somehow generate EINVAL even though this is not documented anywhere? */
   int err = errno;
   mutex->lock_bt_num = backtrace((void **)(mutex->lock_bt), EINA_LOCK_DEBUG_BT_NUM);
   errno = err;

   eina_lock_take(_eina_tracking_lock);
   _eina_tracking = eina_inlist_append(_eina_tracking,
                                       EINA_INLIST_GET(mutex));
   eina_lock_release(_eina_tracking_lock);
#endif

   return EINA_LOCK_SUCCEED;
}

static inline Eina_Lock_Result
_eina_lock_release(Eina_Lock *mutex)
{
#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated) return EINA_LOCK_SUCCEED;
#endif

#ifdef EINA_HAVE_DEBUG_THREADS
/* recursive locks can't make use of any of this */
   if (!mutex->recursive)
     {
        mutex->locked = 0;
        mutex->lock_thread_id = 0;
        memset(mutex->lock_bt, 0, EINA_LOCK_DEBUG_BT_NUM * sizeof(Eina_Lock_Bt_Func));
        mutex->lock_bt_num = 0;
        eina_lock_take(_eina_tracking_lock);
        _eina_tracking = eina_inlist_remove(_eina_tracking,
                                            EINA_INLIST_GET(mutex));
        eina_lock_release(_eina_tracking_lock);
     }
#endif
   LeaveCriticalSection((&mutex->mutex));
   return EINA_LOCK_SUCCEED;
}

static inline Eina_Bool
_eina_condition_new(Eina_Condition *cond, Eina_Lock *mutex)
{
   InitializeConditionVariable(&cond->condition);
   cond->lock = mutex;
   return EINA_TRUE;
}

static inline void
_eina_condition_free(Eina_Condition *cond)
{
}

static inline Eina_Bool
_eina_condition_wait_ex(Eina_Condition *cond, DWORD timeout)
{
   Eina_Bool r = EINA_FALSE;

#ifdef EINA_HAVE_DEBUG_THREADS
   assert(_eina_threads_activated);
   assert(cond->lock != NULL);

   eina_lock_take(_eina_tracking_lock);
   _eina_tracking = eina_inlist_remove(_eina_tracking,
                                       EINA_INLIST_GET(cond->lock));
   eina_lock_release(_eina_tracking_lock);
#endif

   if (SleepConditionVariableCS(&cond->condition
                              , &cond->lock->mutex
                              , timeout))
     {
        r = EINA_TRUE;
     }

   if (EINA_FALSE == r)
     {
        DWORD err = GetLastError();
        if (ERROR_ACCESS_DENIED == err)
          {
             EINA_LOCK_ABORT_DEBUG((int) err, cond_wait, cond);
          }
        else if (ERROR_TIMEOUT == err)
          {
             eina_error_set(ETIMEDOUT);
          }
     }

#ifdef EINA_HAVE_DEBUG_THREADS
   eina_lock_take(_eina_tracking_lock);
   _eina_tracking = eina_inlist_append(_eina_tracking
                                      , EINA_INLIST_GET(cond->lock));
   eina_lock_release(_eina_tracking_lock);
#endif

   return r;
}

static inline Eina_Bool
_eina_condition_wait(Eina_Condition *cond)
{
   return _eina_condition_wait_ex(cond, INFINITE);
}

static inline Eina_Bool
_eina_condition_timedwait(Eina_Condition *cond, double t)
{
   DWORD timeout = INFINITE;

   if (t >= 0.0)
     {
        uint64_t sec = (uint64_t) t;
        uint64_t millisecs = (uint64_t) ((t - sec) * 1000.0);
        uint64_t temp = sec * 1000ULL + millisecs;
        if (temp < (uint64_t) ((DWORD) -1))
           timeout = (DWORD) temp;
     }
   return _eina_condition_wait_ex(cond, timeout);
}

static inline Eina_Bool
_eina_condition_broadcast(Eina_Condition *cond)
{
#ifdef EINA_HAVE_DEBUG_THREADS
   assert(cond->lock != NULL);
#endif

   WakeAllConditionVariable(&cond->condition);
   return EINA_TRUE;
}

static inline Eina_Bool
_eina_condition_signal(Eina_Condition *cond)
{
#ifdef EINA_HAVE_DEBUG_THREADS
   assert(cond->lock != NULL);
#endif

   WakeConditionVariable(&cond->condition);
   return EINA_TRUE;
}

static inline Eina_Bool
_eina_rwlock_new(Eina_RWLock *mutex)
{
   InitializeSRWLock(&mutex->mutex);
   return EINA_TRUE;
}

static inline void
_eina_rwlock_free(Eina_RWLock *mutex)
{
    return;
}

static inline Eina_Lock_Result
_eina_rwlock_take_read(Eina_RWLock *mutex)
{
#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated) return EINA_LOCK_SUCCEED;
#endif

   AcquireSRWLockShared(&mutex->mutex);
   mutex->mode = _Eina_RWLock_Mode_Shared;
   return EINA_LOCK_SUCCEED;
}
static inline Eina_Lock_Result
_eina_rwlock_take_write(Eina_RWLock *mutex)
{
#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated) return EINA_LOCK_SUCCEED;
#endif

   AcquireSRWLockExclusive(&mutex->mutex);
   mutex->mode = _Eina_RWLock_Mode_Exclusive;
   return EINA_LOCK_SUCCEED;
}

static inline Eina_Lock_Result
_eina_rwlock_release(Eina_RWLock *mutex)
{
#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated) return EINA_LOCK_SUCCEED;
#endif

   if (mutex->mode == _Eina_RWLock_Mode_Exclusive)
     {
        mutex->mode = _Eina_RWLock_Mode_None;
        ReleaseSRWLockExclusive(&mutex->mutex);
     }
   else if (mutex->mode == _Eina_RWLock_Mode_Shared)
     {
        mutex->mode = _Eina_RWLock_Mode_None;
        ReleaseSRWLockShared(&mutex->mutex);
     }
   else return EINA_LOCK_FAIL;

   return EINA_LOCK_SUCCEED;
}

static inline Eina_Bool
_eina_tls_cb_new(Eina_TLS *key, Eina_TLS_Delete_Cb delete_cb)
{
   const DWORD k = TlsAlloc();
   if (TLS_OUT_OF_INDEXES != k)
     {
        if (delete_cb)
          {
             eina_lock_take(&_eina_tls_map_lock);
             eina_hash_add(_eina_tls_map, &k, delete_cb);
             eina_lock_release(&_eina_tls_map_lock);
          }
        *key = k;
        return EINA_TRUE;
     }
   else
     {
        return EINA_FALSE;
     }
}

static inline void
_eina_tls_free(Eina_TLS key)
{
   eina_lock_take(&_eina_tls_map_lock);
   eina_hash_del_by_key(_eina_tls_map, &key);
   eina_lock_release(&_eina_tls_map_lock);
   TlsFree(key);
}

static inline void *
_eina_tls_get(Eina_TLS key)
{
   return TlsGetValue(key);
}

static inline Eina_Bool
_eina_tls_set(Eina_TLS key, const void *data)
{
   const Eina_Bool ret = TlsSetValue(key, (void *) data) ? EINA_TRUE : EINA_FALSE;
   return ret;
}


static inline Eina_Bool
_eina_barrier_new(Eina_Barrier *barrier, int needed)
{
   return InitializeSynchronizationBarrier(barrier, (LONG) needed, 10)
       ? EINA_TRUE : EINA_FALSE;
}

static inline  void
_eina_barrier_free(Eina_Barrier *barrier)
{
   if (!DeleteSynchronizationBarrier(barrier))
     {
        EINA_LOCK_ABORT_DEBUG(GetLastError(), barrier_destroy, barrier);
     }
}

static inline Eina_Bool
_eina_barrier_wait(Eina_Barrier *barrier)
{
   EnterSynchronizationBarrier(barrier, 0);
   return EINA_TRUE;
}

static inline Eina_Bool
_eina_spinlock_new(Eina_Spinlock *spinlock)
{
    *spinlock = 0L;
    return EINA_TRUE;
}

static inline void
_eina_spinlock_free(Eina_Spinlock *spinlock)
{
}

static inline Eina_Lock_Result
_eina_spinlock_take_try(Eina_Spinlock *spinlock)
{
    return 0 == InterlockedCompareExchange(spinlock, 1, 0)
        ? EINA_LOCK_SUCCEED : EINA_LOCK_FAIL;
}

static inline Eina_Lock_Result
_eina_spinlock_take(Eina_Spinlock *spinlock)
{
    for (;;)
      {
          /*
           * Implementations of compare-exchange operations may actually
           * perform a memory write even though the compare is falsy,
           * yielding memory contention.
           * We perform a little optimization by checking if the value is
           * zero first before the compare-exchange operation.
           */
          if (0L == *spinlock
              && EINA_LOCK_SUCCEED == _eina_spinlock_take_try(spinlock))
            {
               return EINA_LOCK_SUCCEED;
            }

          /*
           * _mm_pause will emit a 'PAUSE' intruction, which tells the processor
           * we are in the middle of a spinlock loop
           */
          _mm_pause();
      }
}

static inline Eina_Lock_Result
_eina_spinlock_release(Eina_Spinlock *spinlock)
{
    return 1 == InterlockedCompareExchange(spinlock, 0, 1)
        ? EINA_LOCK_SUCCEED : EINA_LOCK_FAIL;
}

static inline Eina_Bool
_eina_semaphore_new(Eina_Semaphore *sem, int count_init)
{
   if (count_init < 0) return EINA_FALSE;
   *sem = CreateSemaphoreW(NULL, count_init, count_init, NULL);
   return *sem ? EINA_TRUE : EINA_FALSE;
}

static inline Eina_Bool
_eina_semaphore_free(Eina_Semaphore *sem)
{
    return CloseHandle(*sem) ? EINA_TRUE : EINA_FALSE;
}

static inline Eina_Bool
_eina_semaphore_lock(Eina_Semaphore *sem)
{
   return WAIT_OBJECT_0 == WaitForSingleObject(*sem, INFINITE)
       ? EINA_TRUE : EINA_FALSE;
}

static inline Eina_Bool
_eina_semaphore_release(Eina_Semaphore *sem, int count_release EINA_UNUSED)
{
     return ReleaseSemaphore(*sem, 1, NULL) ? EINA_TRUE : EINA_FALSE;
}

#endif
