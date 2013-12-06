//  (C) Copyright John Maddock 2001 - 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  cygwin specific config options:

#define EFL_EINA_BOOST_PLATFORM "Cygwin"
#define EFL_EINA_BOOST_HAS_DIRENT_H
#define EFL_EINA_BOOST_HAS_LOG1P
#define EFL_EINA_BOOST_HAS_EXPM1

//
// Threading API:
// See if we have POSIX threads, if we do use them, otherwise
// revert to native Win threads.
#define EFL_EINA_BOOST_HAS_UNISTD_H
#include <unistd.h>
#if defined(_POSIX_THREADS) && (_POSIX_THREADS+0 >= 0) && !defined(EFL_EINA_BOOST_HAS_WINTHREADS)
#  define EFL_EINA_BOOST_HAS_PTHREADS
#  define EFL_EINA_BOOST_HAS_SCHED_YIELD
#  define EFL_EINA_BOOST_HAS_GETTIMEOFDAY
#  define EFL_EINA_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#  define EFL_EINA_BOOST_HAS_SIGACTION
#else
#  if !defined(EFL_EINA_BOOST_HAS_WINTHREADS)
#     define EFL_EINA_BOOST_HAS_WINTHREADS
#  endif
#  define EFL_EINA_BOOST_HAS_FTIME
#endif

//
// find out if we have a stdint.h, there should be a better way to do this:
//
#include <sys/types.h>
#ifdef _STDINT_H
#define EFL_EINA_BOOST_HAS_STDINT_H
#endif

/// Cygwin has no fenv.h
#define EFL_EINA_BOOST_NO_FENV_H

// boilerplate code:
#include <eina_boost/config/posix_features.hpp>

//
// Cygwin lies about XSI conformance, there is no nl_types.h:
//
#ifdef EFL_EINA_BOOST_HAS_NL_TYPES_H
#  undef EFL_EINA_BOOST_HAS_NL_TYPES_H
#endif
 




