#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include "eina_config.h"
#include "Eina.h"

Eina_Bool fork_resetting;

void
_eina_lock_debug_abort(int err, const char *fn, const volatile void *ptr)
{
   if (fork_resetting) return;
   fprintf(stderr, "EINA ERROR: '%s' on %s %p\n", strerror(err), fn, ptr);
#ifdef EINA_HAVE_DEBUG_THREADS
   abort();
#endif
}

void
_eina_lock_debug_deadlock(const char *fn, const volatile void *ptr)
{
   fprintf(stderr, "EINA ERROR: DEADLOCK on %s %p\n", fn, ptr);
#ifdef EINA_HAVE_DEBUG_THREADS
   abort();
#endif
}

void
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

Eina_Bool
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

Eina_Bool
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

void
eina_lock_free(Eina_Lock *mutex)
{
   _eina_lock_free(mutex);
}

Eina_Lock_Result
eina_lock_take_try(Eina_Lock *mutex)
{
   return _eina_lock_take_try(mutex);
}

Eina_Lock_Result
eina_lock_take(Eina_Lock *mutex)
{
   return _eina_lock_take(mutex);
}

Eina_Lock_Result
eina_lock_release(Eina_Lock *mutex)
{
   return _eina_lock_release(mutex);
}

Eina_Bool
eina_condition_new(Eina_Condition *cond, Eina_Lock *mutex)
{
   return _eina_condition_new(cond, mutex);
}

void
eina_condition_free(Eina_Condition *cond)
{
   _eina_condition_free(cond);
}

Eina_Bool
eina_condition_wait(Eina_Condition *cond)
{
   return _eina_condition_wait(cond);
}

Eina_Bool
eina_condition_timedwait(Eina_Condition *cond, double t)
{
   return _eina_condition_timedwait(cond, t);
}

Eina_Bool
eina_condition_broadcast(Eina_Condition *cond)
{
   return _eina_condition_broadcast(cond);
}

Eina_Bool
eina_condition_signal(Eina_Condition *cond)
{
   return _eina_condition_signal(cond);
}

Eina_Bool
eina_rwlock_new(Eina_RWLock *mutex)
{
   return _eina_rwlock_new(mutex);
}

void
eina_rwlock_free(Eina_RWLock *mutex)
{
   _eina_rwlock_free(mutex);
}

Eina_Lock_Result
eina_rwlock_take_read(Eina_RWLock *mutex)
{
   return _eina_rwlock_take_read(mutex);
}

Eina_Lock_Result
eina_rwlock_take_write(Eina_RWLock *mutex)
{
   return _eina_rwlock_take_write(mutex);
}

Eina_Lock_Result
eina_rwlock_release(Eina_RWLock *mutex)
{
   return _eina_rwlock_release(mutex);
}

Eina_Bool
eina_tls_cb_new(Eina_TLS *key, Eina_TLS_Delete_Cb delete_cb)
{
   return _eina_tls_cb_new(key, delete_cb);
}

Eina_Bool
eina_tls_new(Eina_TLS *key)
{
   return _eina_tls_cb_new(key, NULL);
}

void
eina_tls_free(Eina_TLS key)
{
   _eina_tls_free(key);
}

void *
eina_tls_get(Eina_TLS key)
{
   return _eina_tls_get(key);
}

Eina_Bool
eina_tls_set(Eina_TLS key, const void *data)
{
   return _eina_tls_set(key, data);
}

Eina_Bool
eina_barrier_new(Eina_Barrier *barrier, int needed)
{
   return _eina_barrier_new(barrier, needed);
}

void
eina_barrier_free(Eina_Barrier *barrier)
{
   _eina_barrier_free(barrier);
}

Eina_Bool
eina_barrier_wait(Eina_Barrier *barrier)
{
   return _eina_barrier_wait(barrier);
}

Eina_Bool
eina_spinlock_new(Eina_Spinlock *spinlock)
{
   return _eina_spinlock_new(spinlock);
}

void
eina_spinlock_free(Eina_Spinlock *spinlock)
{
   _eina_spinlock_free(spinlock);
}

Eina_Lock_Result
eina_spinlock_take_try(Eina_Spinlock *spinlock)
{
   return _eina_spinlock_take_try(spinlock);
}

Eina_Lock_Result
eina_spinlock_take(Eina_Spinlock *spinlock)
{
   return _eina_spinlock_take(spinlock);
}

Eina_Lock_Result
eina_spinlock_release(Eina_Spinlock *spinlock)
{
   return _eina_spinlock_release(spinlock);
}

Eina_Bool
eina_semaphore_new(Eina_Semaphore *sem, int count_init)
{
   return _eina_semaphore_new(sem, count_init);
}

Eina_Bool
eina_semaphore_free(Eina_Semaphore *sem)
{
   return _eina_semaphore_free(sem);
}

Eina_Bool
eina_semaphore_lock(Eina_Semaphore *sem)
{
   return _eina_semaphore_lock(sem);
}

Eina_Bool
eina_semaphore_release(Eina_Semaphore *sem, int count_release EINA_UNUSED)
{
   return _eina_semaphore_release(sem, count_release);
}

