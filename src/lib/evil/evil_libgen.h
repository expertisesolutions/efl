#ifndef __EVIL_LIBGEN_H__
#define __EVIL_LIBGEN_H__

#include <evil_private.h>

#ifndef HAVE_DIRNAME
# define HAVE_DIRNAME
# define dirname evil_dirname
#endif

EVIL_API char* dirname (char* path);

#endif

