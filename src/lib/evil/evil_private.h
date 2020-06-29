#ifndef __EVIL_PRIVATE_H__
#define __EVIL_PRIVATE_H__

#if _WIN32_WINNT < 0x0600
# error Windows XP not supported anymore
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
#  define EVIL_UNUSED __attribute__ ((__unused__))
# else
#  define EVIL_UNUSED
# endif
#endif

#ifndef EVIL_UNUSED
#define EVIL_UNUSED
#endif

#include <evil_windows.h>

#include <sys/stat.h> /* for mkdir in evil_macro_wrapper */

#include <evil_api.h>


#ifndef PATH_MAX
# define PATH_MAX MAX_PATH
#endif

#ifdef _WIN32
# define _USE_MATH_DEFINES
#endif

#include "evil_basename.h"
#include "evil_dlfcn.h"
#include "evil_fcntl.h"
#include "evil_langinfo.h"
#include "evil_locale.h"
#include "evil_main.h"
#include "evil_mman.h"
#include "evil_stat.h"
#include "evil_stdio.h"
#include "evil_stdlib.h"
#include "evil_string.h"
#include "evil_strings.h"
#include "evil_time.h"
#include "evil_unistd.h"
#include "evil_util.h"
#include "evil_vasprintf.h"

#define sigsetjmp(Env, Save) setjmp(Env)

#ifdef _MSC_VER

#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include "evil_macro_wrapper.h"

#ifdef __cplusplus
}
#endif


#endif /* __EVIL_PRIVATE_H__ */
