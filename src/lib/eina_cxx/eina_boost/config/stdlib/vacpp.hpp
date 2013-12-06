//  (C) Copyright John Maddock 2001 - 2002. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

#if __IBMCPP__ <= 501
#  define EFL_EINA_BOOST_NO_STD_ALLOCATOR
#endif

#define EFL_EINA_BOOST_HAS_MACRO_USE_FACET
#define EFL_EINA_BOOST_NO_STD_MESSAGES

// Apple doesn't seem to reliably defined a *unix* macro
#if !defined(CYGWIN) && (  defined(__unix__)  \
                        || defined(__unix)    \
                        || defined(unix)      \
                        || defined(__APPLE__) \
                        || defined(__APPLE)   \
                        || defined(APPLE))
#  include <unistd.h>
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

#define EFL_EINA_BOOST_STDLIB "Visual Age default standard library"



