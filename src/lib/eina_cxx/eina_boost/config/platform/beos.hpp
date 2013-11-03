//  (C) Copyright John Maddock 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  BeOS specific config options:

#define EFL_EINA_BOOST_PLATFORM "BeOS"

#define EFL_EINA_BOOST_NO_CWCHAR
#define EFL_EINA_BOOST_NO_CWCTYPE
#define EFL_EINA_BOOST_HAS_UNISTD_H

#define EFL_EINA_BOOST_HAS_BETHREADS

#ifndef EFL_EINA_BOOST_DISABLE_THREADS
#  define EFL_EINA_BOOST_HAS_THREADS
#endif

// boilerplate code:
#include <eina_boost/config/posix_features.hpp>
 


