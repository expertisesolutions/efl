#ifndef UNPOSIX_STDLIB_H
#define UNPOSIX_STDLIB_H

#include "unimplemented.h"

#include_next <stdlib.h>

#include <unposix_api.h>

UNIMPLEMENTED inline UNPOSIX_API int mkstemp(char* template)
{
    #warning mkstemp is not implemented
    return 0;
}
UNIMPLEMENTED inline UNPOSIX_API int mkostemp(char* template, int flags)
{
    #warning mkostemp is not implemented
    return 0;
}

UNIMPLEMENTED inline UNPOSIX_API int mkostemps(char* template, int suffixlen, int flags)
{
    #warning mkostemps is not implemented
    return 0;
}

#endif
