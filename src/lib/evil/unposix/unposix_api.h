#ifndef UNPOSIX_API_H
#define UNPOSIX_API_H

#ifdef UNPOSIX_API
# undef UNPOSIX_API
#endif

#ifndef _MSC_VER
# ifdef UNPOSIX_SOURCE
#  define UNPOSIX_API __declspec(dllexport)
# else
#  define UNPOSIX_API __declspec(dllimport)
# endif
#else
// TODO: Add more compiler exports
# define UNPOSIX_API
#endif

#endif
