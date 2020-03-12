#ifndef PTHREAD_H
#define PTHREAD_H

#include "unimplemented.h"

UNIMPLEMENTED typedef unsigned long long pthread_t;

UNIMPLEMENTED_STRUCT_T(pthread_attr)

UNIMPLEMENTED struct sched_param {
    int sched_priority;
};

UNIMPLEMENTED typedef int pthread_key_t;

#define SCHED_RR 5
#define SCHED_FIFO 6

#define PTHREAD_BARRIER_SERIAL_THREAD 1
#define PTHREAD_CANCEL_ASYNCHRONOUS 2
#define PTHREAD_CANCEL_ENABLE 3
#define PTHREAD_CANCEL_DEFERRED 4
#define PTHREAD_CANCEL_DISABLE 5
#define PTHREAD_CANCELED 6
#define PTHREAD_CREATE_DETACHED 7
#define PTHREAD_CREATE_JOINABLE 8
#define PTHREAD_EXPLICIT_SCHED 9
#define PTHREAD_INHERIT_SCHED 10
#define PTHREAD_ONCE_INIT 17
#define PTHREAD_PRIO_INHERIT 18
#define PTHREAD_PRIO_NONE 19
#define PTHREAD_PRIO_PROTECT 20
#define PTHREAD_PROCESS_SHARED 21
#define PTHREAD_PROCESS_PRIVATE 22
#define PTHREAD_SCOPE_PROCESS 23
#define PTHREAD_SCOPE_SYSTEM 24

UNIMPLEMENTED inline int pthread_getschedparam(void* a, ...)
{
    #warning pthread_getschedparam is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_self()
{
    #warning pthread_self is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_setschedparam(void* c, void* d, void* e)
{
    #warning pthread_ is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_cancel(void* a, ...)
{
    #warning pthread_cancel is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_setcancelstate(void* a, ...)
{
    #warning pthread_setcancelstate is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_testcancel()
{
    #warning pthread_testcancel is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_cleanup_pop(void* a, ...)
{
    #warning pthread_cleanup_pop is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_cleanup_push(void* a, ...)
{
    #warning pthread_cleanup_push is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_attr_init(void* a, ...)
{
    #warning pthread_attr_init is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_join(void* a, ...)
{
    #warning pthread_join is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_create(void* a, ...)
{
    #warning pthread_create is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_equal(void* a, ...)
{
    #warning pthread_equal is not implemented.
    return 0;
}

UNIMPLEMENTED inline int pthread_setcanceltype(void* a, ...)
{
    #warning pthread_setcanceltype is not implemented.
    return 0;
}

#endif
