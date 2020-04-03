#ifndef EVIL_API_H
#define EVIL_API_H

#ifdef EVIL_API
# undef EVIL_API
#endif

#ifdef EVIL_BUILD
# define EVIL_API __declspec(dllexport)
#else
# define EVIL_API __declspec(dllimport)
#endif

#endif
