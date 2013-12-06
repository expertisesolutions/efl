//  (C) Copyright John Maddock 2001. 
//  (C) Copyright Darin Adler 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Metrowerks standard library:

#ifndef __MSL_CPP__
#  include <eina_boost/config/no_tr1/utility.hpp>
#  ifndef __MSL_CPP__
#     error This is not the MSL standard library!
#  endif
#endif

#if __MSL_CPP__ >= 0x6000  // Pro 6
#  define EFL_EINA_BOOST_HAS_HASH
#  define EFL_EINA_BOOST_STD_EXTENSION_NAMESPACE Metrowerks
#endif
#define EFL_EINA_BOOST_HAS_SLIST

#if __MSL_CPP__ < 0x6209
#  define EFL_EINA_BOOST_NO_STD_MESSAGES
#endif

// check C lib version for <stdint.h>
#include <cstddef>

#if defined(__MSL__) && (__MSL__ >= 0x5000)
#  define EFL_EINA_BOOST_HAS_STDINT_H
#  if !defined(__PALMOS_TRAPS__)
#    define EFL_EINA_BOOST_HAS_UNISTD_H
#  endif
   // boilerplate code:
#  include <eina_boost/config/posix_features.hpp>
#endif

#if defined(_MWMT) || _MSL_THREADSAFE
#  define EFL_EINA_BOOST_HAS_THREADS
#endif

#ifdef _MSL_NO_EXPLICIT_FUNC_TEMPLATE_ARG
#  define EFL_EINA_BOOST_NO_STD_USE_FACET
#  define EFL_EINA_BOOST_HAS_TWO_ARG_USE_FACET
#endif

//  C++0x headers not yet implemented
//
#  define EFL_EINA_BOOST_NO_CXX11_HDR_ARRAY
#  define EFL_EINA_BOOST_NO_CXX11_HDR_CHRONO
#  define EFL_EINA_BOOST_NO_CXX11_HDR_CODECVT
#  define EFL_EINA_BOOST_NO_CXX11_HDR_CONDITION_VARIABLE
#  define EFL_EINA_BOOST_NO_CXX11_HDR_FORWARD_LIST
#  define EFL_EINA_BOOST_NO_CXX11_HDR_FUTURE
#  define EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
#  define EFL_EINA_BOOST_NO_CXX11_HDR_MUTEX
#  define EFL_EINA_BOOST_NO_CXX11_HDR_RANDOM
#  define EFL_EINA_BOOST_NO_CXX11_HDR_RATIO
#  define EFL_EINA_BOOST_NO_CXX11_HDR_REGEX
#  define EFL_EINA_BOOST_NO_CXX11_HDR_SYSTEM_ERROR
#  define EFL_EINA_BOOST_NO_CXX11_HDR_THREAD
#  define EFL_EINA_BOOST_NO_CXX11_HDR_TUPLE
#  define EFL_EINA_BOOST_NO_CXX11_HDR_TYPE_TRAITS
#  define EFL_EINA_BOOST_NO_CXX11_HDR_TYPEINDEX
#  define EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_MAP
#  define EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_SET
#  define EFL_EINA_BOOST_NO_CXX11_NUMERIC_LIMITS
#  define EFL_EINA_BOOST_NO_CXX11_ALLOCATOR
#  define EFL_EINA_BOOST_NO_CXX11_ATOMIC_SMART_PTR
#  define EFL_EINA_BOOST_NO_CXX11_SMART_PTR
#  define EFL_EINA_BOOST_NO_CXX11_HDR_FUNCTIONAL

#define EFL_EINA_BOOST_STDLIB "Metrowerks Standard Library version " EFL_EINA_BOOST_STRINGIZE(__MSL_CPP__)









