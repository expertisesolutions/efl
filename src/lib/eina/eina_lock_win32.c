/* EINA - EFL data type library
 * Copyright (C) 2020 Lucas Cavancate de Sousa
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


#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif

#include "unimplemented.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include "eina_config.h"
#include "Eina.h"

Eina_Bool fork_resetting;

EAPI void
_eina_lock_debug_abort(int err, const char *fn, const volatile void *ptr)
{
   if (fork_resetting) return;
   fprintf(stderr, "EINA ERROR: '%s' on %s %p\n", strerror(err), fn, ptr);
#ifdef EINA_HAVE_DEBUG_THREADS
   abort();
#endif
}

EAPI void
_eina_lock_debug_deadlock(const char *fn, const volatile void *ptr)
{
   fprintf(stderr, "EINA ERROR: DEADLOCK on %s %p\n", fn, ptr);
#ifdef EINA_HAVE_DEBUG_THREADS
   abort();
#endif
}

EAPI void
eina_lock_debug(const Eina_Lock *mutex)
{
#ifdef EINA_HAVE_DEBUG_THREADS
   fprintf(stderr, "EINA ERROR: DEADLOCK on lock %p, locked: %i, by %ti\n",
           mutex, (int)mutex->locked, (ptrdiff_t)mutex->lock_thread_id);
   backtrace_symbols_fd((void **)mutex->lock_bt, mutex->lock_bt_num, 1);
#else
   fprintf(stderr, "EINA ERROR: DEADLOCK on lock %p\n", mutex);
#endif
}

EAPI Eina_Bool
_eina_lock_new(Eina_Lock *mutex, Eina_Bool recursive)
{
   InitializeCriticalSection((mutex->mutex));
   DWORD ok = GetLastError();
   if (ok == ERROR_SUCCESS) return EINA_TRUE;
   return EINA_FALSE;

}

EAPI void
_eina_lock_free(Eina_Lock *mutex)
{
   DWORD ok;

   DeleteCriticalSection((mutex->mutex));
   ok = GetLastError();
   if (ok != ERROR_SUCCESS) EINA_LOCK_ABORT_DEBUG(ok, mutex_destroy, mutex);
}

UNIMPLEMENTED EAPI Eina_Bool
_eina_condition_new(Eina_Condition *cond, Eina_Lock *mutex)
{
   // TODO
   #warning eina_condition_new is not implemented
   return EINA_TRUE;
}

EAPI void
_eina_condition_free(Eina_Condition *cond)
{
   return;
}

EAPI Eina_Bool
_eina_rwlock_new(Eina_RWLock *mutex)
{
   DWORD ok;

   InitializeSRWLock((mutex->mutex));
   if (ok == ERROR_SUCCESS) return EINA_TRUE;
   else if ((ok == ERROR_NOT_ENOUGH_MEMORY) ||
            (ok == ERROR_ACCESS_DENIED) || (ok == ERROR_OUTOFMEMORY))
      return EINA_FALSE;
   else EINA_LOCK_ABORT_DEBUG(ok, rwlock_init, mutex);
   return EINA_FALSE;
}

EAPI void
_eina_rwlock_free(Eina_RWLock *mutex)
{
    return;
}

UNIMPLEMENTED EAPI Eina_Bool
_eina_barrier_new(Eina_Barrier *barrier, int needed)
{
#ifdef EINA_HAVE_WIN32_BARRIER
   InitializeSynchronizationBarrier(&(barrier->barrier), (LONG) needed, 0);
   DWORD ok = GetLastError();
   if (ok == ERROR_SUCCESS) return EINA_TRUE;
   else if ((ok == ERROR_NOT_ENOUGH_MEMORY) ||
            (ok == ERROR_ACCESS_DENIED) || (ok == ERROR_OUTOFMEMORY))
      return EINA_FALSE;
   else EINA_LOCK_ABORT_DEBUG(ok, barrier_init, barrier);
   return EINA_FALSE;
#else
   // TODO condition variable
   #warning eina_barrier_new needs condition variable that is not implemented
   return EINA_TRUE;
#endif
}

UNIMPLEMENTED EAPI void
_eina_barrier_free(Eina_Barrier *barrier)
{
#ifdef EINA_HAVE_WIN32_BARRIER
   DeleteSynchronizationBarrier(&(barrier->barrier));
   DWORD ok = GetLastError();
   if (ok != ERROR_SUCCESS) EINA_LOCK_ABORT_DEBUG(ok, barrier_destroy, barrier);
#else
   // TODO condition variable
   #warning eina_barrier_free needs condition variable that is not implemented
#endif
}

EAPI Eina_Bool
_eina_spinlock_new(Eina_Spinlock *spinlock)
{
#if defined(EINA_HAVE_WIN32_SPINLOCK)
   int ok = InitializeCriticalSectionAndSpinCount(spinlock, INFINITE);
   DWORD err = GetLastError();
   if (ok != 0) return EINA_TRUE;
   else if ((ok == ERROR_NOT_ENOUGH_MEMORY) ||
            (ok == ERROR_ACCESS_DENIED) || (ok == ERROR_OUTOFMEMORY))
      return EINA_FALSE;
   else EINA_LOCK_ABORT_DEBUG(ok, spin_init, spinlock);
   return EINA_FALSE;
#else
   return eina_lock_new(spinlock);
#endif
}

EAPI void
_eina_spinlock_free(Eina_Spinlock *spinlock)
{
#if defined(EINA_HAVE_WIN32_SPINLOCK)
   DeleteCriticalSection(spinlock);
   DWORD ok = GetLastError();
   if (ok != 0) EINA_LOCK_ABORT_DEBUG(ok, spin_destroy, spinlock);
#else
   eina_lock_free(spinlock);
#endif
}

EAPI Eina_Bool
_eina_semaphore_new(Eina_Semaphore *sem, int count_init)
{
   if (sem && (count_init >= 0))
     {
        sem = CreateSemaphoreA(NULL, count_init, count_init, NULL);
        DWORD ok = GetLastError();
        if (ok == ERROR_SUCCESS) return EINA_TRUE;
     }
   return EINA_FALSE;
}

EAPI Eina_Bool
_eina_semaphore_free(Eina_Semaphore *sem)
{
   if (sem)
     {
        CloseHandle(sem);
        DWORD ok = GetLastError();
        if (ok == ERROR_SUCCESS) return EINA_TRUE;
     }
   return EINA_FALSE;
}
