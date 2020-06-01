#ifndef EINA_PREAMBLE_H
#define EINA_PREAMBLE_H

#ifdef EAPI
# undef EAPI
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
# define EAPI_WEAK
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#   define EAPI_WEAK __attribute__ ((weak))
#  else
#   define EAPI
#   define EAPI_WEAK
#  endif
# else
/**
 * @def EAPI
 * @brief Used to export functions (by changing visibility).
 */
#  define EAPI
/**
 * @def EAPI_WEAK
 * @brief Weak symbol, primarily useful in defining library functions which
 * can be overridden in user code.
 * Note: Not supported on all platforms.
 */
#  define EAPI_WEAK
# endif
#endif

#endif
