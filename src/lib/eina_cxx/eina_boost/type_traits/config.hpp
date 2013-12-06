
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_CONFIG_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_CONFIG_HPP_INCLUDED

#ifndef EFL_EINA_BOOST_CONFIG_HPP
#include <eina_boost/config.hpp>
#endif

#include <eina_boost/detail/workaround.hpp>

//
// whenever we have a conversion function with ellipses
// it needs to be declared __cdecl to suppress compiler
// warnings from MS and Borland compilers (this *must*
// appear before we include is_same.hpp below):
#if defined(EFL_EINA_BOOST_MSVC) || (defined(__BORLANDC__) && !defined(EFL_EINA_BOOST_DISABLE_WIN32))
#   define EFL_EINA_BOOST_TT_DECL __cdecl
#else
#   define EFL_EINA_BOOST_TT_DECL /**/
#endif

# if (EFL_EINA_BOOST_WORKAROUND(__MWERKS__, < 0x3000)                         \
    || EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1301)                        \
    || !defined(__EDG_VERSION__) && EFL_EINA_BOOST_WORKAROUND(__GNUC__, < 3) \
    || EFL_EINA_BOOST_WORKAROUND(__IBMCPP__, < 600 )                         \
    || EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, < 0x5A0)                      \
    || defined(__ghs)                                               \
    || EFL_EINA_BOOST_WORKAROUND(__HP_aCC, < 60700)           \
    || EFL_EINA_BOOST_WORKAROUND(MPW_CPLUS, EFL_EINA_BOOST_TESTED_AT(0x890))          \
    || EFL_EINA_BOOST_WORKAROUND(__SUNPRO_CC, EFL_EINA_BOOST_TESTED_AT(0x580)))       \
    && defined(EFL_EINA_BOOST_NO_IS_ABSTRACT)

#   define EFL_EINA_BOOST_TT_NO_CONFORMING_IS_CLASS_IMPLEMENTATION 1

#endif

#ifndef EFL_EINA_BOOST_TT_NO_CONFORMING_IS_CLASS_IMPLEMENTATION
# define EFL_EINA_BOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION 1
#endif

//
// Define EFL_EINA_BOOST_TT_NO_ELLIPSIS_IN_FUNC_TESTING 
// when we can't test for function types with elipsis:
//
#if EFL_EINA_BOOST_WORKAROUND(__GNUC__, < 3)
#  define EFL_EINA_BOOST_TT_NO_ELLIPSIS_IN_FUNC_TESTING
#endif

//
// define EFL_EINA_BOOST_TT_TEST_MS_FUNC_SIGS
// when we want to test __stdcall etc function types with is_function etc
// (Note, does not work with Borland, even though it does support __stdcall etc):
//
#if defined(_MSC_EXTENSIONS) && !defined(__BORLANDC__)
#  define EFL_EINA_BOOST_TT_TEST_MS_FUNC_SIGS
#endif

//
// define EFL_EINA_BOOST_TT_NO_CV_FUNC_TEST
// if tests for cv-qualified member functions don't 
// work in is_member_function_pointer
//
#if EFL_EINA_BOOST_WORKAROUND(__MWERKS__, < 0x3000) || EFL_EINA_BOOST_WORKAROUND(__IBMCPP__, <= 600)
#  define EFL_EINA_BOOST_TT_NO_CV_FUNC_TEST
#endif

#endif // EFL_EINA_BOOST_TT_CONFIG_HPP_INCLUDED


