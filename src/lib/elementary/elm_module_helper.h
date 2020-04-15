/* A small helper header defining ELEMENTARY_API for elementary modules, it should be
 * included last in the modules C files.
 */

#ifndef ELM_MODULE_HELPER_H
#define ELM_MODULE_HELPER_H

#ifdef ELEMENTARY_API
# undef ELEMENTARY_API
#endif
#ifdef EWAPI
# undef EWAPI
#endif

#ifdef _WIN32
# ifdef EFL_BUILD
#  ifdef DLL_EXPORT
#   define ELEMENTARY_API __declspec(dllexport)
#  else
#   define ELEMENTARY_API
#  endif
# else
#  define ELEMENTARY_API __declspec(dllimport)
# endif
# define ELEMENTARY_API_WEAK
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define ELEMENTARY_API __attribute__ ((visibility("default")))
#   define ELEMENTARY_API_WEAK __attribute__ ((weak))
#  else
#   define ELEMENTARY_API
#   define ELEMENTARY_API_WEAK
#  endif
# else
#  define ELEMENTARY_API
#  define ELEMENTARY_API_WEAK
# endif
#endif

#define EWAPI ELEMENTARY_API ELEMENTARY_API_WEAK

#endif
