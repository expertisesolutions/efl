#ifndef UNPOSIX_UNISTD_H
#define UNPOSIX_UNISTD_H

#include "unimplemented.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <corecrt.h>
#include <BaseTsd.h>

#include <windows.h>
#include <corecrt_io.h>
#include <process.h>

#include <sys/types.h>
#include <evil_unistd.h>

#define F_OK 0
#define W_OK 2
#define R_OK 4
#define X_OK 0

#include <getopt.h>

#include <evil_private.h>

UNIMPLEMENTED inline int getopt(int argc, char * const argv[],
          const char *optstring)
{
    #warning getopt is not implemented
    return 0;
}

#endif
