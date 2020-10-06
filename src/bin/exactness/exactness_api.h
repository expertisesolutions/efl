#ifndef _EXACTNESS_API_H
#define _EXACTNESS_API_H

#ifdef EXACTNESS_API
#error EXACTNESS_API should not be already defined
#endif

#ifdef _WIN32
# ifndef EXACTNESS_STATIC
#  ifdef EXACTNESS_BUILD
#   define EXACTNESS_API __declspec(dllexport)
#  else
#   define EXACTNESS_API __declspec(dllimport)
#  endif
# else
#  define EXACTNESS_API
# endif
# define EXACTNESS_API_WEAK
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EXACTNESS_API __attribute__ ((visibility("default")))
#   define EXACTNESS_API_WEAK __attribute__ ((weak))
#  else
#   define EXACTNESS_API
#   define EXACTNESS_API_WEAK
#  endif
# else
#  define EXACTNESS_API
#  define EXACTNESS_API_WEAK
# endif
#endif

#endif
