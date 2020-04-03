#ifndef UNPOSIX_FNMATCH_H
#define UNPOSIX_FNMATCH_H

#include "unimplemented.h"

#include "unposix_api.h"

#ifdef __cplusplus
extern "C" {
#endif

// The string does not match the specified pattern.
#define FNM_NOMATCH 1
// <slash> in string only matches <slash> in pattern.
#define FNM_PATHNAME (1 << 0)
// Disable backslash escaping.
#define FNM_NOESCAPE (1 << 1)
// Leading <period> in string must be exactly matched by <period> in pattern.
#define FNM_PERIOD (1 << 2)

UNIMPLEMENTED inline UNPOSIX_API int fnmatch(const char * a, const char * b, int c)
{
    #warning fnmatch is not implemented
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
