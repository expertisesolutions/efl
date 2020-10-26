#ifndef EVIL_API_H
#define EVIL_API_H

#ifdef EAPI
#undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# if __GNUC__ >= 4
#  define EAPI __attribute__ ((visibility("default")))
# else
#  define EAPI
# endif
#endif

#endif
