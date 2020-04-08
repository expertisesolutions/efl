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

#include "unimplemented.h"

#include <Windows.h>
#include <synchapi.h>

typedef LPDWORD Eina_TLS;

// for win32 spinlock use at least
//#define _WIN32_WINNT 0x0403

#if defined(EINA_HAVE_WIN32_SPINLOCK)
typedef PCRITICAL_SECTION Eina_Spinlock;
#endif

typedef HANDLE Eina_Semaphore;

/** @privatesection  @{ */
struct _Eina_Lock
{
#ifdef EINA_HAVE_DEBUG_THREADS
   EINA_INLIST; /**< Keeps track of the threads waiting for the lock */
#endif
   PCRITICAL_SECTION mutex; /**< The mutex that handles the locking */
#ifdef EINA_HAVE_DEBUG_THREADS
   LPDWORD           lock_thread_id; /**< The ID of the thread that currently has the lock */
   Eina_Lock_Bt_Func lock_bt[EINA_LOCK_DEBUG_BT_NUM]; /**< The function that will produce a backtrace on the thread that has the lock */
   int               lock_bt_num; /**< Number of addresses in the backtrace */
   Eina_Bool         locked : 1;  /**< Indicates locked or not locked */
   Eina_Bool         recursive : 1;  /**< Indicates recursive lock */
#endif
};

struct _Eina_Condition
{
   Eina_Lock            *lock;      /**< The lock for this condition */
   CONDITION_VARIABLE   condition; /**< The condition variable */
};

enum _Eina_RWLock_Mode {
   _Eina_RWLock_Mode_Shared,
   _Eina_RWLock_Mode_Exclusive,
   _Eina_RWLock_Mode_None
};

struct _Eina_RWLock
{
   PSRWLOCK mutex; /**< The mutex that handles the locking */
#ifdef EINA_HAVE_DEBUG_THREADS
   LPDWORD  lock_thread_wid; /**< The ID of the thread that currently has the lock */
#endif
   enum _Eina_RWLock_Mode mode; /**< The mode the SRWLock is (Shared, Exclusive or None)*/
};
/** @} privatesection */

#ifdef EINA_HAVE_DEBUG_THREADS
EAPI extern HANDLE _eina_main_loop;
EAPI extern HANDLE _eina_tracking_lock;
#endif

static inline void
eina_lock_free(Eina_Lock *mutex)
{
#ifdef EINA_HAVE_DEBUG_THREADS
   if (mutex->locked)
     {
        EnterCriticalSection(_eina_tracking_lock);
        _eina_tracking = eina_inlist_remove(_eina_tracking,
                                            EINA_INLIST_GET(mutex));
        LeavCriticalSection(_eina_tracking_lock);
     }
#endif
   _eina_lock_free(mutex);
}

static inline Eina_Lock_Result
eina_lock_take_try(Eina_Lock *mutex)
{
   Eina_Lock_Result ret = EINA_LOCK_FAIL;
   int ok;

#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated)
     {
        return EINA_LOCK_SUCCEED;
     }
#endif

   ok = TryEnterCriticalSection((mutex->mutex));
   DWORD err = GetLastError();
   if (ok != 0) ret = EINA_LOCK_SUCCEED;
   else if (err == ERROR_POSSIBLE_DEADLOCK)
     {
        eina_lock_debug(mutex);
        ret = EINA_LOCK_DEADLOCK;
     }
   else if (err != ERROR_TIMEOUT) EINA_LOCK_ABORT_DEBUG((int)err, trylock, mutex);
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

        EnterCriticalSection(_eina_tracking_lock);
        _eina_tracking = eina_inlist_append(_eina_tracking,
                                            EINA_INLIST_GET(mutex));
        LeaveCriticalSection(_eina_tracking_lock);
     }
#endif
   return ret;
}

static inline Eina_Lock_Result
eina_lock_take(Eina_Lock *mutex)
{
   Eina_Lock_Result ret = EINA_LOCK_FAIL;
   DWORD ok;

#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated)
     {
        return EINA_LOCK_SUCCEED;
     }
#endif

#ifdef EINA_HAVE_DEBUG_THREADS
   if (eina_lock_take_try(mutex) == EINA_LOCK_SUCCEED) return EINA_LOCK_SUCCEED;
#endif

#ifdef EINA_HAVE_DEBUG_THREADS
   if (_eina_threads_debug >= 100)
     {
        struct timeval t0, t1;
        int dt;

        gettimeofday(&t0, NULL);
        ok = EnterCriticalSection((mutex->mutex));
        gettimeofday(&t1, NULL);

        dt = (t1.tv_sec - t0.tv_sec) * 1000000;
        if (t1.tv_usec > t0.tv_usec)
           dt += (t1.tv_usec - t0.tv_usec);
        else
           dt -= t0.tv_usec - t1.tv_usec;

        if (dt > _eina_threads_debug) abort();
     }
   else
     {
#endif
        EnterCriticalSection((mutex->mutex));
        ok = GetLastError();
#ifdef EINA_HAVE_DEBUG_THREADS
     }
#endif

   if (ok == ERROR_SUCCESS) ret = EINA_LOCK_SUCCEED;
   else if (ok == ERROR_POSSIBLE_DEADLOCK)
     {
        eina_lock_debug(mutex);
        ret = EINA_LOCK_DEADLOCK;
#ifdef EINA_HAVE_DEBUG_THREADS
        if (_eina_threads_debug) abort();
#endif
     }
   else if (ok != ERROR_TIMEOUT) EINA_LOCK_ABORT_DEBUG((int)ok, lock, mutex);

#ifdef EINA_HAVE_DEBUG_THREADS
   /* recursive locks can't make use of any of this */
   if (mutex->recursive) return ret;
   mutex->locked = 1;
   mutex->lock_thread_id = GetCurrentThreadId();
   /* backtrace() can somehow generate EINVAL even though this is not documented anywhere? */
   int err = errno;
   mutex->lock_bt_num = backtrace((void **)(mutex->lock_bt), EINA_LOCK_DEBUG_BT_NUM);
   errno = err;

   EnterCriticalSection(_eina_tracking_lock);
   _eina_tracking = eina_inlist_append(_eina_tracking,
                                       EINA_INLIST_GET(mutex));
   LeaveCriticalSection(_eina_tracking_lock);
#endif

   return ret;
}

static inline Eina_Lock_Result
eina_lock_release(Eina_Lock *mutex)
{
   Eina_Lock_Result ret = EINA_LOCK_FAIL;
   DWORD ok;

#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated)
     {
        return EINA_LOCK_SUCCEED;
     }
#endif

#ifdef EINA_HAVE_DEBUG_THREADS
/* recursive locks can't make use of any of this */
   if (!mutex->recursive)
     {
        mutex->locked = 0;
        mutex->lock_thread_id = 0;
        memset(mutex->lock_bt, 0, EINA_LOCK_DEBUG_BT_NUM * sizeof(Eina_Lock_Bt_Func));
        mutex->lock_bt_num = 0;
        EnterCriticalSection(_eina_tracking_lock);
        _eina_tracking = eina_inlist_remove(_eina_tracking,
                                            EINA_INLIST_GET(mutex));
        LeaveCriticalSection(_eina_tracking_lock);
     }
#endif
   LeaveCriticalSection((mutex->mutex));
   ok = GetLastError();
   if (ok == ERROR_SUCCESS) ret = EINA_LOCK_SUCCEED;
   else if (ok != ERROR_ACCESS_DENIED) EINA_LOCK_ABORT_DEBUG((int)ok, unlock, mutex);
   return ret;
}

static inline Eina_Bool
eina_condition_wait(Eina_Condition *cond)
{
   Eina_Bool r = EINA_FALSE;
   int ok;

#ifdef EINA_HAVE_DEBUG_THREADS
   assert(_eina_threads_activated);
   assert(cond->lock != NULL);

   EnterCriticalSection(_eina_tracking_lock);
   _eina_tracking = eina_inlist_remove(_eina_tracking,
                                       EINA_INLIST_GET(cond->lock));
   LeaveCriticalSection(_eina_tracking_lock);
#endif

   ok = SleepConditionVariableCS(&(cond->condition), (cond->lock->mutex), INFINITE);
   DWORD err = GetLastError();
   if (ok != 0) r = EINA_TRUE;
   else if (err != ERROR_ACCESS_DENIED)
      EINA_LOCK_ABORT_DEBUG((int)ok, cond_wait, cond);

#ifdef EINA_HAVE_DEBUG_THREADS
   EnterCriticalSection(_eina_tracking_lock);
   _eina_tracking = eina_inlist_append(_eina_tracking,
                                       EINA_INLIST_GET(cond->lock));
   LeaveCriticalSection(_eina_tracking_lock);
#endif

   return r;
}

UNIMPLEMENTED static inline Eina_Bool
eina_condition_timedwait(Eina_Condition *cond, double t)
{
   //** TODO **//
   #warning eina_condition_timedwait is not implemented
   return EINA_TRUE;
}

static inline Eina_Bool
eina_condition_broadcast(Eina_Condition *cond)
{
   DWORD ok;

#ifdef EINA_HAVE_DEBUG_THREADS
   assert(cond->lock != NULL);
#endif

   WakeAllConditionVariable(&(cond->condition));
   ok = GetLastError();
   if (ok == ERROR_SUCCESS) return EINA_TRUE;

   EINA_LOCK_ABORT_DEBUG((int)ok, cond_broadcast, cond);
   return EINA_FALSE;
}

static inline Eina_Bool
eina_condition_signal(Eina_Condition *cond)
{
   DWORD ok;

#ifdef EINA_HAVE_DEBUG_THREADS
   assert(cond->lock != NULL);
#endif
   WakeConditionVariable(&(cond->condition));
   ok = GetLastError();
   if (ok == ERROR_SUCCESS) return EINA_TRUE;

   EINA_LOCK_ABORT_DEBUG((int)ok, cond_signal, cond);
   return EINA_FALSE;
}

static inline Eina_Lock_Result
eina_rwlock_take_read(Eina_RWLock *mutex)
{
   DWORD ok;

#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated)
     {
        return EINA_LOCK_SUCCEED;
     }
#endif

   AcquireSRWLockShared((mutex->mutex));
   ok = GetLastError();
   if (ok == ERROR_SUCCESS) {
      mutex->mode = _Eina_RWLock_Mode_Shared;
      return EINA_LOCK_SUCCEED;
   } else if (ok == ERROR_NOT_ENOUGH_MEMORY || ok == ERROR_OUTOFMEMORY) {
      return EINA_LOCK_FAIL;
   } else if (ok == ERROR_POSSIBLE_DEADLOCK) {
      EINA_LOCK_DEADLOCK_DEBUG(rwlock_rdlock, mutex);
   } else EINA_LOCK_ABORT_DEBUG((int)ok, rwlock_rdlock, mutex);
   return EINA_LOCK_FAIL;
}

static inline Eina_Lock_Result
eina_rwlock_take_write(Eina_RWLock *mutex)
{
   DWORD ok;

#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated)
     {
        return EINA_LOCK_SUCCEED;
     }
#endif

   AcquireSRWLockExclusive((mutex->mutex));
   ok = GetLastError();
   if (ok == ERROR_SUCCESS) {
      mutex->mode = _Eina_RWLock_Mode_Exclusive;
      return EINA_LOCK_SUCCEED;
   } else if (ok == ERROR_NOT_ENOUGH_MEMORY || ok == ERROR_OUTOFMEMORY) {
      return EINA_LOCK_FAIL;
   } else if (ok == ERROR_POSSIBLE_DEADLOCK) {
      EINA_LOCK_DEADLOCK_DEBUG(rwlock_rdlock, mutex);
   } else EINA_LOCK_ABORT_DEBUG((int)ok, rwlock_rdlock, mutex);
   return EINA_LOCK_FAIL;
}

static inline Eina_Lock_Result
eina_rwlock_release(Eina_RWLock *mutex)
{
   DWORD ok;

#ifdef EINA_HAVE_ON_OFF_THREADS
   if (!_eina_threads_activated)
     {
        return EINA_LOCK_SUCCEED;
     }
#endif

   if (mutex->mode == _Eina_RWLock_Mode_Exclusive) {
      ReleaseSRWLockExclusive((mutex->mutex));
      ok = GetLastError();
   } else if (mutex->mode == _Eina_RWLock_Mode_Shared) {
      ReleaseSRWLockShared((mutex->mutex));
      ok = GetLastError();
   }
   if (ok  == ERROR_SUCCESS) return EINA_LOCK_SUCCEED;
   else if (ok == ERROR_ACCESS_DENIED) return EINA_LOCK_FAIL;
   EINA_LOCK_ABORT_DEBUG((int)ok, rwlock_unlock, mutex);

   return EINA_LOCK_FAIL;
}



UNIMPLEMENTED static inline Eina_Bool
eina_tls_cb_new(Eina_TLS *key, Eina_TLS_Delete_Cb delete_cb)
{
   //** TODO **//
   #warning eina_tls_cb_new is not implemented
   return EINA_TRUE;
}

static inline Eina_Bool
eina_tls_new(Eina_TLS *key)
{
   return eina_tls_cb_new(key, NULL);
}

UNIMPLEMENTED static inline void
eina_tls_free(Eina_TLS key)
{
   //** TODO **//
   #warning eina_tls_free is not implemented
   return;
}

UNIMPLEMENTED static inline void *
eina_tls_get(Eina_TLS key)
{
   //** TODO **//
   #warning eina_tls_get is not implemented
   return (void*) 0;
}
UNIMPLEMENTED
static inline Eina_Bool
eina_tls_set(Eina_TLS key, const void *data)
{
   //** TODO **//
   #warning eina_tls_set is not implemented
   return EINA_TRUE;
}


#ifdef EINA_HAVE_WIN32_BARRIER
typedef struct _Eina_Barrier Eina_Barrier;

struct _Eina_Barrier
{
   LPSYNCHRONIZATION_BARRIER barrier;
};

static inline Eina_Bool
eina_barrier_wait(Eina_Barrier *barrier)
{
   EnterSyncronizarionBarrier(&(barrier->barrier),
                              SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY);
   DWORD ok = GetLastError();
   if (ok == ERROR_SUCCESS) return EINA_TRUE;
   else EINA_LOCK_ABORT_DEBUG((int)ok, barrier_wait, barrier);
   return EINA_TRUE;
}

#endif

static inline Eina_Lock_Result
eina_spinlock_take_try(Eina_Spinlock *spinlock)
{
#if defined(EINA_HAVE_WIN32_SPINLOCK)
   int t = TryEnterCriticalSection(spinlock);
   if (t != 0) return EINA_LOCK_SUCCEED;
   else if (t == 0) return EINA_LOCK_FAIL;
   else {
      DWORD err = GetLastError();
      EINA_LOCK_ABORT_DEBUG((int)err, spin_trylock, spinlock);
   }
   return EINA_LOCK_FAIL;
#else
   return eina_lock_take_try(spinlock);
#endif
}

static inline Eina_Lock_Result
eina_spinlock_take(Eina_Spinlock *spinlock)
{
#if defined(EINA_HAVE_WIN32_SPINLOCK)
   int t;

#ifdef EINA_HAVE_DEBUG_THREADS
   if (eina_spinlock_take_try(spinlock) == EINA_LOCK_SUCCEED) return EINA_LOCK_SUCCEED;
#endif

   for (;;)
     {
        t = EnterCriticalSection(spinlock);
        if (t != 0) break;
        else {
           DWORD ok = GetLastError();
           EINA_LOCK_ABORT_DEBUG((int)ok, spin_lock, spinlock);
        }
     }

   return EINA_LOCK_SUCCEED;
#else
   return eina_lock_take(spinlock);
#endif
}

static inline Eina_Lock_Result
eina_spinlock_release(Eina_Spinlock *spinlock)
{
#if defined(EINA_HAVE_WIN32_SPINLOCK)
   LeaveCriticalSection(spinlock);
   DWORD ok = GetLastError();
   if (ok == ERROR_SUCCESS) return EINA_LOCK_SUCCEED;
   else if (ok == ERROR_ACCESS_DENIED) return EINA_LOCK_FAIL;
   else EINA_LOCK_ABORT_DEBUG((int)ok, spin_unlock, spinlock);
   return EINA_LOCK_FAIL;
#else
   return eina_lock_release(spinlock);
#endif
}

static inline Eina_Bool
eina_semaphore_lock(Eina_Semaphore *sem)
{
   if (sem)
     {
        DWORD ok;
        for (;;)
          {
             WaitForSingleObject(sem, INFINITE);
             ok = GetLastError();
             if (ok == ERROR_SUCCESS)
                return EINA_TRUE;
             else if (ok == WAIT_OBJECT_0 || ok == WAIT_TIMEOUT)
                continue;
             else if (errno == ERROR_POSSIBLE_DEADLOCK)
                EINA_LOCK_DEADLOCK_DEBUG(sem_wait, sem);

             return EINA_FALSE;

          }
     }
   return EINA_FALSE;
}


static inline Eina_Bool
eina_semaphore_release(Eina_Semaphore *sem, int count_release EINA_UNUSED)
{
   if (sem)
     return (ReleaseSemaphore(sem, 1, NULL) != 0) ? EINA_TRUE : EINA_FALSE;
   return EINA_FALSE;
}

#endif
