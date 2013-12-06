//  (C) Copyright Jim Douglas 2005. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  QNX specific config options:

#define EFL_EINA_BOOST_PLATFORM "QNX"

#define EFL_EINA_BOOST_HAS_UNISTD_H
#include <eina_boost/config/posix_features.hpp>

// QNX claims XOpen version 5 compatibility, but doesn't have an nl_types.h
// or log1p and expm1:
#undef  EFL_EINA_BOOST_HAS_NL_TYPES_H
#undef  EFL_EINA_BOOST_HAS_LOG1P
#undef  EFL_EINA_BOOST_HAS_EXPM1

#define EFL_EINA_BOOST_HAS_PTHREADS
#define EFL_EINA_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE

#define EFL_EINA_BOOST_HAS_GETTIMEOFDAY
#define EFL_EINA_BOOST_HAS_CLOCK_GETTIME
#define EFL_EINA_BOOST_HAS_NANOSLEEP





