#ifndef UNPOSIX_FCNTL_H
#define UNPOSIX_FCNTL_H

#include "unimplemented.h"

#include_next <fcntl.h>

#include "sys/types.h"
#include <corecrt_io.h>

#include "unposix_api.h"

UNIMPLEMENTED inline UNPOSIX_API int fcntl(int a, int b, ...)
{
    #warning fcntl is not implemented
    return 0;
}

UNIMPLEMENTED inline UNPOSIX_API int open(const char * a, int b, ...)
{
    #warning open is not implemented
    return 0;
}

UNIMPLEMENTED inline UNPOSIX_API int openat(int a, const char * b, int c, ...)
{
    #warning openat is not implemented
    return 0;
}

UNIMPLEMENTED inline UNPOSIX_API int posix_fadvise(int a, off_t b, off_t c, int d)
{
    #warning posix_fadvise is not implemented
    return 0;
}

UNIMPLEMENTED inline UNPOSIX_API int posix_fallocate(int a, off_t b, off_t c)
{
    #warning posix_fallocate is not implemented
    return 0;
}


#endif
