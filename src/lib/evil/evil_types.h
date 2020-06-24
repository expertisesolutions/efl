#ifndef __EVIL_TYPES_H__
#define __EVIL_TYPES_H__

#include <evil_api.h>
#include <sys/types.h>

#ifdef _MSC_VER
// when runing testscheck defines pid_t as int, but olher e-libs don't have it
# ifndef pid_t
typedef DWORD pid_t;
# endif
#endif

#endif
