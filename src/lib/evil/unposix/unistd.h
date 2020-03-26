#ifndef UNPOSIX_UNISTD_H
#define UNPOSIX_UNISTD_H

#include "unimplemented.h"

#include <corecrt.h>
#include <BaseTsd.h>
#include <sys/types.h>
#include <evil_unistd.h>

#include <windows.h>
#include <corecrt_io.h>
#include <process.h>

#define F_OK 0
#define W_OK 2
#define R_OK 4
#define X_OK 0

UNIMPLEMENTED extern char *optarg;
UNIMPLEMENTED extern int optind, opterr, optopt;

typedef SSIZE_T ssize_t;
typedef uint32_t mode_t;
typedef _off_t off_t;

UNIMPLEMENTED inline int getopt(int a, char * const b[], const char * c)
{
    #warning getopt is not implemented
    return 0;
}

#endif
