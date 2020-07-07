#ifndef _EFL_ECORE_EVAS_WAYLAND_API_H
#define _EFL_ECORE_EVAS_WAYLAND_API_H

#ifdef EMODAPI_API
#error EMODAPI_API should not be already defined
#endif


#ifdef _WIN32
# ifndef EFL_MODULE_STATIC
#  define EMODAPI __declspec(dllexport)
# else
#  define EMODAPI
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EMODAPI __attribute__ ((visibility("default")))
#  endif
# endif
#endif /* ! _WIN32 */

#ifndef EMODAPI
# define EMODAPI
#endif

#endif
