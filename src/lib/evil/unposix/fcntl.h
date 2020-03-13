#ifndef UNPOSIX_FCNTL_H
#define UNPOSIX_FCNTL_H

#include "unimplemented.h"

#include <sys/types.h>
#include <corecrt_io.h>
#include_next <fcntl.h>

UNIMPLEMENTED inline int fcntl(int a, int b, ...)
{
    #warning fcntl is not implemented
    return 0;
}

UNIMPLEMENTED inline int open(const char * a, int b, ...)
{
    #warning open is not implemented
    return 0;
}

UNIMPLEMENTED inline int openat(int a, const char * b, int c, ...)
{
    #warning openat is not implemented
    return 0;
}

UNIMPLEMENTED inline int posix_fadvise(int a, off_t b, off_t c, int d)
{
    #warning posix_fadvise is not implemented
    return 0;
}

UNIMPLEMENTED inline int posix_fallocate(int a, off_t b, off_t c)
{
    #warning posix_fallocate is not implemented
    return 0;
}


#endif
