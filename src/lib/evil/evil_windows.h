#ifndef __EVIL_WINDOWS_H__
#define __EVIL_WINDOWS_H__

#ifdef _INC_WINDOWS_
#error Already included windows.h
#endif

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <evil_api.h>

#endif
