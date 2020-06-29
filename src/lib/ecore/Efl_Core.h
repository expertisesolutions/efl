#ifndef _EFL_CORE_H
#define _EFL_CORE_H

#include <Efl_Config.h>

#include <Eina.h>
#include <Eo.h>
#include <Efl.h>

#include <ecore_api.h>

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <winsock2.h>
# undef WIN32_LEAN_AND_MEAN
#elif defined (__FreeBSD__) || defined (__OpenBSD__)
# include <sys/select.h>
# include <signal.h>
#elif defined (__ANDROID__)
# include <sys/select.h>
#elif defined (__sun)
# include <sys/siginfo.h>
#else
# include <sys/time.h>
# include <signal.h>
#endif

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

ECORE_API extern double _efl_startup_time;

#include "Ecore_Common.h"
#include "Ecore_Eo.h"

#ifdef __cplusplus
}
#endif

#endif

// We are including efl_general.h again, just in case Efl_Core.h was already included before this
// and the __EFL_*_IS_REQUIRED changed since then.
#include "efl_general.h"
