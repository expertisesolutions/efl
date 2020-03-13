#ifndef UNPOSIX_STDIO_H
#define UNPOSIX_STDIO_H

#include "unimplemented.h"

#include_next <stdio.h>
#include <unistd.h>

#ifndef popen
#define popen _popen
#endif

#ifndef pclose
#define pclose _pclose
#endif

UNIMPLEMENTED inline int vasprintf(char **str, const char *fmt, va_list ap)
{
    #warning vasprintf is not implemented
    return 0;
}

#endif
