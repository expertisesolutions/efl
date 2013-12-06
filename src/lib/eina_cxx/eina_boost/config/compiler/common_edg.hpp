//  (C) Copyright John Maddock 2001 - 2002.
//  (C) Copyright Jens Maurer 2001.
//  (C) Copyright David Abrahams 2002.
//  (C) Copyright Aleksey Gurtovoy 2002.
//  (C) Copyright Markus Schoepflin 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//
// Options common to all edg based compilers.
//
// This is included from within the individual compiler mini-configs.

#ifndef  __EDG_VERSION__
#  error This file requires that __EDG_VERSION__ be defined.
#endif

#if (__EDG_VERSION__ <= 238)
#   define EFL_EINA_BOOST_NO_INTEGRAL_INT64_T
#   define EFL_EINA_BOOST_NO_SFINAE
#endif

#if (__EDG_VERSION__ <= 240)
#   define EFL_EINA_BOOST_NO_VOID_RETURNS
#endif

#if (__EDG_VERSION__ <= 241) && !defined(EFL_EINA_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
#   define EFL_EINA_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#endif

#if (__EDG_VERSION__ <= 244) && !defined(EFL_EINA_BOOST_NO_TEMPLATE_TEMPLATES)
#   define EFL_EINA_BOOST_NO_TEMPLATE_TEMPLATES
#endif

#if (__EDG_VERSION__ < 300) && !defined(EFL_EINA_BOOST_NO_IS_ABSTRACT)
#   define EFL_EINA_BOOST_NO_IS_ABSTRACT
#endif

#if (__EDG_VERSION__ <= 303) && !defined(EFL_EINA_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL)
#   define EFL_EINA_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#endif

// See also kai.hpp which checks a Kai-specific symbol for EH
# if !defined(__KCC) && !defined(__EXCEPTIONS) && !defined(EFL_EINA_BOOST_NO_EXCEPTIONS)
#     define EFL_EINA_BOOST_NO_EXCEPTIONS
# endif

# if !defined(__NO_LONG_LONG)
#     define EFL_EINA_BOOST_HAS_LONG_LONG
# else
#     define EFL_EINA_BOOST_NO_LONG_LONG
# endif

// Not sure what version was the first to support #pragma once, but
// different EDG-based compilers (e.g. Intel) supported it for ages.
// Add a proper version check if it causes problems.
#define EFL_EINA_BOOST_HAS_PRAGMA_ONCE

//
// C++0x features
//
//   See above for EFL_EINA_BOOST_NO_LONG_LONG
//
#if (__EDG_VERSION__ < 310)
#  define EFL_EINA_BOOST_NO_CXX11_EXTERN_TEMPLATE
#endif
#if (__EDG_VERSION__ <= 310)
// No support for initializer lists
#  define EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
#endif
#if (__EDG_VERSION__ < 400)
#  define EFL_EINA_BOOST_NO_CXX11_VARIADIC_MACROS
#endif

#define EFL_EINA_BOOST_NO_CXX11_AUTO_DECLARATIONS
#define EFL_EINA_BOOST_NO_CXX11_AUTO_MULTIDECLARATIONS
#define EFL_EINA_BOOST_NO_CXX11_CHAR16_T
#define EFL_EINA_BOOST_NO_CXX11_CHAR32_T
#define EFL_EINA_BOOST_NO_CXX11_CONSTEXPR
#define EFL_EINA_BOOST_NO_CXX11_DECLTYPE
#define EFL_EINA_BOOST_NO_CXX11_DECLTYPE_N3276
#define EFL_EINA_BOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#define EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS
#define EFL_EINA_BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#define EFL_EINA_BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#define EFL_EINA_BOOST_NO_CXX11_LAMBDAS
#define EFL_EINA_BOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#define EFL_EINA_BOOST_NO_CXX11_NOEXCEPT
#define EFL_EINA_BOOST_NO_CXX11_NULLPTR
#define EFL_EINA_BOOST_NO_CXX11_RANGE_BASED_FOR
#define EFL_EINA_BOOST_NO_CXX11_RAW_LITERALS
#define EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
#define EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS
#define EFL_EINA_BOOST_NO_SFINAE_EXPR
#define EFL_EINA_BOOST_NO_CXX11_STATIC_ASSERT
#define EFL_EINA_BOOST_NO_CXX11_TEMPLATE_ALIASES
#define EFL_EINA_BOOST_NO_CXX11_UNICODE_LITERALS
#define EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES
#define EFL_EINA_BOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#define EFL_EINA_BOOST_NO_CXX11_USER_DEFINED_LITERALS
#define EFL_EINA_BOOST_NO_CXX11_ALIGNAS
#define EFL_EINA_BOOST_NO_CXX11_TRAILING_RESULT_TYPES
#define EFL_EINA_BOOST_NO_CXX11_INLINE_NAMESPACES

#ifdef c_plusplus
// EDG has "long long" in non-strict mode
// However, some libraries have insufficient "long long" support
// #define EFL_EINA_BOOST_HAS_LONG_LONG
#endif
