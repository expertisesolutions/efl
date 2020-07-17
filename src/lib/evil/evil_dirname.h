#ifndef __EVIL_DIRNAME_H__
#define __EVIL_DIRNAME_H__

#include <evil_api.h>

#ifdef _MSC_VER
# define dirname evil_dirname
#endif

EVIL_API char* dirname (char* path);

#endif

