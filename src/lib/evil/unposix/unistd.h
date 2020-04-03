#ifndef UNPOSIX_UNISTD_H
#define UNPOSIX_UNISTD_H

#include "unimplemented.h"

#include "unposix_api.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <corecrt.h>
#include <BaseTsd.h>

#include <corecrt_io.h>
#include <process.h>

#include <sys/types.h>

#define F_OK 0
#define W_OK 2
#define R_OK 4
#define X_OK 0

#include <getopt.h>

#include <evil_private.h>

#endif
