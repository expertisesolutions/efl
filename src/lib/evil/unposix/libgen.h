#ifndef UNPOSIX_LIBGEN_H
#define UNPOSIX_LIBGEN_H

#include "unimplemented.h"

#include "unposix_api.h"

UNIMPLEMENTED inline UNPOSIX_API char *basename(char * a)
{
    #warning basename is not implemented
    return NULL;
}

UNIMPLEMENTED inline UNPOSIX_API char *dirname(char * a)
{
    #warning dirname is not implemented
    return NULL;
}

#endif
