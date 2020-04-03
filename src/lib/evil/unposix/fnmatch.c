#include "fnmatch.h"

int fnmatch(const char *pattern, const char *string, int flags)
{
    
    if (flags & FNM_PATHNAME)
        ; // do something

    if (flags & FNM_NOESCAPE)
        ; // do something

    if (flags & FNM_PERIOD)
        ; // do something

    PathMatchSpecExA(string, pattern, 0);
    
    return 0;
}

