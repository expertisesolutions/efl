//  (C) Copyright John Maddock 2001 - 2003. 
//  (C) Copyright Jens Maurer 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  sun specific config options:

#define EFL_EINA_BOOST_PLATFORM "Sun Solaris"

#define EFL_EINA_BOOST_HAS_GETTIMEOFDAY

// boilerplate code:
#define EFL_EINA_BOOST_HAS_UNISTD_H
#include <eina_boost/config/posix_features.hpp>

//
// pthreads don't actually work with gcc unless _PTHREADS is defined:
//
#if defined(__GNUC__) && defined(_POSIX_THREADS) && !defined(_PTHREADS)
# undef EFL_EINA_BOOST_HAS_PTHREADS
#endif




