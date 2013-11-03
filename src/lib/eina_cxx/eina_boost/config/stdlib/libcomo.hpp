//  (C) Copyright John Maddock 2002 - 2003. 
//  (C) Copyright Jens Maurer 2002 - 2003. 
//  (C) Copyright Beman Dawes 2002 - 2003. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Comeau STL:

#if !defined(__LIBCOMO__)
#  include <eina_boost/config/no_tr1/utility.hpp>
#  if !defined(__LIBCOMO__)
#      error "This is not the Comeau STL!"
#  endif
#endif

//
// std::streambuf<wchar_t> is non-standard
// NOTE: versions of libcomo prior to beta28 have octal version numbering,
// e.g. version 25 is 21 (dec)
#if __LIBCOMO_VERSION__ <= 22
#  define EFL_EINA_BOOST_NO_STD_WSTREAMBUF
#endif

#if (__LIBCOMO_VERSION__ <= 31) && defined(_WIN32)
#define EFL_EINA_BOOST_NO_SWPRINTF
#endif

#if __LIBCOMO_VERSION__ >= 31
#  define EFL_EINA_BOOST_HAS_HASH
#  define EFL_EINA_BOOST_HAS_SLIST
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

//
// Intrinsic type_traits support.
// The SGI STL has it's own __type_traits class, which
// has intrinsic compiler support with SGI's compilers.
// Whatever map SGI style type traits to boost equivalents:
//
#define EFL_EINA_BOOST_HAS_SGI_TYPE_TRAITS

#define EFL_EINA_BOOST_STDLIB "Comeau standard library " EFL_EINA_BOOST_STRINGIZE(__LIBCOMO_VERSION__)


