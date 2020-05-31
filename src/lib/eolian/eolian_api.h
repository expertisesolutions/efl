#ifndef EOLIAN_API_H
#define EOLIAN_API_H

#ifdef _MSC_VER
# if defined(EOLIAN_BUILD)
#  define EOLIAN_API __declspec(dllexport)
# else
#  define EOLIAN_API __declspec(dllimport)
# endif
#else
# if __GNUC__ >= 4
#  define EOLIAN_API __attribute__ ((visibility("default")))
# else
#  define EOLIAN_API
# endif
#endif

#endif

