//  (C) Copyright John Maddock 2001 - 2002. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  SGI C++ compiler setup:

#define EFL_EINA_BOOST_COMPILER "SGI Irix compiler version " EFL_EINA_BOOST_STRINGIZE(_COMPILER_VERSION)

#include "eina_boost/config/compiler/common_edg.hpp"

//
// Threading support:
// Turn this on unconditionally here, it will get turned off again later
// if no threading API is detected.
//
#define EFL_EINA_BOOST_HAS_THREADS
#define EFL_EINA_BOOST_NO_TWO_PHASE_NAME_LOOKUP

#undef EFL_EINA_BOOST_NO_SWPRINTF
#undef EFL_EINA_BOOST_DEDUCED_TYPENAME

//
// version check:
// probably nothing to do here?


