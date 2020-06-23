#ifndef EVIL_API_H
#define EVIL_API_H

#ifdef EVIL_API
#undef EVIL_API
#endif

#ifdef _WIN32
# ifdef EFL_BUILD
#  ifdef DLL_EXPORT
#   define EVIL_API __declspec(dllexport)
#  else
#   define EVIL_API
#  endif
# else
#  define EVIL_API __declspec(dllimport)
# endif
#else
# if __GNUC__ >= 4
#  define EVIL_API __attribute__ ((visibility("default")))
# else
#  define EVIL_API
# endif
#endif

#endif
