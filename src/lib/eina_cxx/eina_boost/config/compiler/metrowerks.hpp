//  (C) Copyright John Maddock 2001.
//  (C) Copyright Darin Adler 2001.
//  (C) Copyright Peter Dimov 2001.
//  (C) Copyright David Abrahams 2001 - 2002.
//  (C) Copyright Beman Dawes 2001 - 2003.
//  (C) Copyright Stefan Slapeta 2004.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Metrowerks C++ compiler setup:

// locale support is disabled when linking with the dynamic runtime
#   ifdef _MSL_NO_LOCALE
#     define EFL_EINA_BOOST_NO_STD_LOCALE
#   endif

#   if __MWERKS__ <= 0x2301  // 5.3
#     define EFL_EINA_BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#     define EFL_EINA_BOOST_NO_POINTER_TO_MEMBER_CONST
#     define EFL_EINA_BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#     define EFL_EINA_BOOST_NO_MEMBER_TEMPLATE_KEYWORD
#   endif

#   if __MWERKS__ <= 0x2401  // 6.2
//#     define EFL_EINA_BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#   endif

#   if(__MWERKS__ <= 0x2407)  // 7.x
#     define EFL_EINA_BOOST_NO_MEMBER_FUNCTION_SPECIALIZATIONS
#     define EFL_EINA_BOOST_NO_UNREACHABLE_RETURN_DETECTION
#   endif

#   if(__MWERKS__ <= 0x3003)  // 8.x
#     define EFL_EINA_BOOST_NO_SFINAE
#    endif

// the "|| !defined(EFL_EINA_BOOST_STRICT_CONFIG)" part should apply to the last
// tested version *only*:
#   if(__MWERKS__ <= 0x3207) || !defined(EFL_EINA_BOOST_STRICT_CONFIG) // 9.6
#     define EFL_EINA_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     define EFL_EINA_BOOST_NO_IS_ABSTRACT
#    endif

#if !__option(wchar_type)
#   define EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T
#endif

#if !__option(exceptions) && !defined(EFL_EINA_BOOST_NO_EXCEPTIONS)
#   define EFL_EINA_BOOST_NO_EXCEPTIONS
#endif

#if (__INTEL__ && _WIN32) || (__POWERPC__ && macintosh)
#   if __MWERKS__ == 0x3000
#     define EFL_EINA_BOOST_COMPILER_VERSION 8.0
#   elif __MWERKS__ == 0x3001
#     define EFL_EINA_BOOST_COMPILER_VERSION 8.1
#   elif __MWERKS__ == 0x3002
#     define EFL_EINA_BOOST_COMPILER_VERSION 8.2
#   elif __MWERKS__ == 0x3003
#     define EFL_EINA_BOOST_COMPILER_VERSION 8.3
#   elif __MWERKS__ == 0x3200
#     define EFL_EINA_BOOST_COMPILER_VERSION 9.0
#   elif __MWERKS__ == 0x3201
#     define EFL_EINA_BOOST_COMPILER_VERSION 9.1
#   elif __MWERKS__ == 0x3202
#     define EFL_EINA_BOOST_COMPILER_VERSION 9.2
#   elif __MWERKS__ == 0x3204
#     define EFL_EINA_BOOST_COMPILER_VERSION 9.3
#   elif __MWERKS__ == 0x3205
#     define EFL_EINA_BOOST_COMPILER_VERSION 9.4
#   elif __MWERKS__ == 0x3206
#     define EFL_EINA_BOOST_COMPILER_VERSION 9.5
#   elif __MWERKS__ == 0x3207
#     define EFL_EINA_BOOST_COMPILER_VERSION 9.6
#   else
#     define EFL_EINA_BOOST_COMPILER_VERSION __MWERKS__
#   endif
#else
#  define EFL_EINA_BOOST_COMPILER_VERSION __MWERKS__
#endif

//
// C++0x features
//
//   See boost\config\suffix.hpp for EFL_EINA_BOOST_NO_LONG_LONG
//
#if __MWERKS__ > 0x3206 && __option(rvalue_refs)
#  define EFL_EINA_BOOST_HAS_RVALUE_REFS
#else
#  define EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
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
#define EFL_EINA_BOOST_NO_CXX11_EXTERN_TEMPLATE
#define EFL_EINA_BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#define EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
#define EFL_EINA_BOOST_NO_CXX11_LAMBDAS
#define EFL_EINA_BOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#define EFL_EINA_BOOST_NO_CXX11_NOEXCEPT
#define EFL_EINA_BOOST_NO_CXX11_NULLPTR
#define EFL_EINA_BOOST_NO_CXX11_RANGE_BASED_FOR
#define EFL_EINA_BOOST_NO_CXX11_RAW_LITERALS
#define EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS
#define EFL_EINA_BOOST_NO_SFINAE_EXPR
#define EFL_EINA_BOOST_NO_CXX11_STATIC_ASSERT
#define EFL_EINA_BOOST_NO_CXX11_TEMPLATE_ALIASES
#define EFL_EINA_BOOST_NO_CXX11_UNICODE_LITERALS
#define EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES
#define EFL_EINA_BOOST_NO_CXX11_VARIADIC_MACROS
#define EFL_EINA_BOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#define EFL_EINA_BOOST_NO_CXX11_USER_DEFINED_LITERALS
#define EFL_EINA_BOOST_NO_CXX11_ALIGNAS
#define EFL_EINA_BOOST_NO_CXX11_TRAILING_RESULT_TYPES
#define EFL_EINA_BOOST_NO_CXX11_INLINE_NAMESPACES

#define EFL_EINA_BOOST_COMPILER "Metrowerks CodeWarrior C++ version " EFL_EINA_BOOST_STRINGIZE(EFL_EINA_BOOST_COMPILER_VERSION)

//
// versions check:
// we don't support Metrowerks prior to version 5.3:
#if __MWERKS__ < 0x2301
#  error "Compiler not supported or configured - please reconfigure"
#endif
//
// last known and checked version:
#if (__MWERKS__ > 0x3205)
#  if defined(EFL_EINA_BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif







