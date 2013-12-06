//  (C) Copyright John Maddock 2001 - 2003.
//  (C) Copyright David Abrahams 2002 - 2003.
//  (C) Copyright Aleksey Gurtovoy 2002.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Borland C++ compiler setup:

//
// versions check:
// we don't support Borland prior to version 5.4:
#if __BORLANDC__ < 0x540
#  error "Compiler not supported or configured - please reconfigure"
#endif

// last known compiler version:
#if (__BORLANDC__ > 0x613)
//#  if defined(EFL_EINA_BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
//#  else
//#     pragma message( "Unknown compiler version - please run the configure tests and report the results")
//#  endif
#elif (__BORLANDC__ == 0x600)
#  error "CBuilderX preview compiler is no longer supported"
#endif

//
// Support macros to help with standard library detection
#if (__BORLANDC__ < 0x560) || defined(_USE_OLD_RW_STL)
#  define EFL_EINA_BOOST_BCB_WITH_ROGUE_WAVE
#elif __BORLANDC__ < 0x570
#  define EFL_EINA_BOOST_BCB_WITH_STLPORT
#else
#  define EFL_EINA_BOOST_BCB_WITH_DINKUMWARE
#endif

//
// Version 5.0 and below:
#   if __BORLANDC__ <= 0x0550
// Borland C++Builder 4 and 5:
#     define EFL_EINA_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     if __BORLANDC__ == 0x0550
// Borland C++Builder 5, command-line compiler 5.5:
#       define EFL_EINA_BOOST_NO_OPERATORS_IN_NAMESPACE
#     endif
// Variadic macros do not exist for C++ Builder versions 5 and below
#define EFL_EINA_BOOST_NO_CXX11_VARIADIC_MACROS
#   endif

// Version 5.51 and below:
#if (__BORLANDC__ <= 0x551)
#  define EFL_EINA_BOOST_NO_CV_SPECIALIZATIONS
#  define EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
#  define EFL_EINA_BOOST_NO_DEDUCED_TYPENAME
// workaround for missing WCHAR_MAX/WCHAR_MIN:
#ifdef __cplusplus
#include <climits>
#include <cwchar>
#else
#include <limits.h>
#include <wchar.h>
#endif // __cplusplus
#ifndef WCHAR_MAX
#  define WCHAR_MAX 0xffff
#endif
#ifndef WCHAR_MIN
#  define WCHAR_MIN 0
#endif
#endif

// Borland C++ Builder 6 and below:
#if (__BORLANDC__ <= 0x564)

#  if defined(NDEBUG) && defined(__cplusplus)
      // fix broken <cstring> so that Boost.test works:
#     include <cstring>
#     undef strcmp
#  endif
   // fix broken errno declaration:
#  include <errno.h>
#  ifndef errno
#     define errno errno
#  endif

#endif

//
// new bug in 5.61:
#if (__BORLANDC__ >= 0x561) && (__BORLANDC__ <= 0x580)
   // this seems to be needed by the command line compiler, but not the IDE:
#  define EFL_EINA_BOOST_NO_MEMBER_FUNCTION_SPECIALIZATIONS
#endif

// Borland C++ Builder 2006 Update 2 and below:
#if (__BORLANDC__ <= 0x582)
#  define EFL_EINA_BOOST_NO_SFINAE
#  define EFL_EINA_BOOST_BCB_PARTIAL_SPECIALIZATION_BUG
#  define EFL_EINA_BOOST_NO_TEMPLATE_TEMPLATES

#  define EFL_EINA_BOOST_NO_PRIVATE_IN_AGGREGATE

#  ifdef _WIN32
#     define EFL_EINA_BOOST_NO_SWPRINTF
#  elif defined(linux) || defined(__linux__) || defined(__linux)
      // we should really be able to do without this
      // but the wcs* functions aren't imported into std::
#     define EFL_EINA_BOOST_NO_STDC_NAMESPACE
      // _CPPUNWIND doesn't get automatically set for some reason:
#     pragma defineonoption EFL_EINA_BOOST_CPPUNWIND -x
#  endif
#endif

#if (__BORLANDC__ <= 0x613)  // Beman has asked Alisdair for more info
   // we shouldn't really need this - but too many things choke
   // without it, this needs more investigation:
#  define EFL_EINA_BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#  define EFL_EINA_BOOST_NO_IS_ABSTRACT
#  define EFL_EINA_BOOST_NO_FUNCTION_TYPE_SPECIALIZATIONS
#  define EFL_EINA_BOOST_NO_USING_TEMPLATE
#  define EFL_EINA_BOOST_SP_NO_SP_CONVERTIBLE

// Temporary workaround
#define EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif

// Borland C++ Builder 2008 and below:
#  define EFL_EINA_BOOST_NO_INTEGRAL_INT64_T
#  define EFL_EINA_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#  define EFL_EINA_BOOST_NO_DEPENDENT_NESTED_DERIVATIONS
#  define EFL_EINA_BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#  define EFL_EINA_BOOST_NO_TWO_PHASE_NAME_LOOKUP
#  define EFL_EINA_BOOST_NO_USING_DECLARATION_OVERLOADS_FROM_TYPENAME_BASE
#  define EFL_EINA_BOOST_NO_NESTED_FRIENDSHIP
#  define EFL_EINA_BOOST_NO_TYPENAME_WITH_CTOR
#if (__BORLANDC__ < 0x600)
#  define EFL_EINA_BOOST_ILLEGAL_CV_REFERENCES
#endif

//
//  Positive Feature detection
//
// Borland C++ Builder 2008 and below:
#if (__BORLANDC__ >= 0x599)
#  pragma defineonoption EFL_EINA_BOOST_CODEGEAR_0X_SUPPORT -Ax
#endif
//
// C++0x Macros:
//
#if !defined( EFL_EINA_BOOST_CODEGEAR_0X_SUPPORT ) || (__BORLANDC__ < 0x610)
#  define EFL_EINA_BOOST_NO_CXX11_CHAR16_T
#  define EFL_EINA_BOOST_NO_CXX11_CHAR32_T
#  define EFL_EINA_BOOST_NO_CXX11_DECLTYPE
#  define EFL_EINA_BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#  define EFL_EINA_BOOST_NO_CXX11_EXTERN_TEMPLATE
#  define EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
#  define EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS
#  define EFL_EINA_BOOST_NO_CXX11_STATIC_ASSERT
#else
#  define EFL_EINA_BOOST_HAS_ALIGNOF
#  define EFL_EINA_BOOST_HAS_CHAR16_T
#  define EFL_EINA_BOOST_HAS_CHAR32_T
#  define EFL_EINA_BOOST_HAS_DECLTYPE
#  define EFL_EINA_BOOST_HAS_EXPLICIT_CONVERSION_OPS
#  define EFL_EINA_BOOST_HAS_REF_QUALIFIER
#  define EFL_EINA_BOOST_HAS_RVALUE_REFS
#  define EFL_EINA_BOOST_HAS_STATIC_ASSERT
#endif

#define EFL_EINA_BOOST_NO_CXX11_AUTO_DECLARATIONS
#define EFL_EINA_BOOST_NO_CXX11_AUTO_MULTIDECLARATIONS
#define EFL_EINA_BOOST_NO_CXX11_CONSTEXPR
#define EFL_EINA_BOOST_NO_CXX11_DECLTYPE_N3276
#define EFL_EINA_BOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#define EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS
#define EFL_EINA_BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#define EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
#define EFL_EINA_BOOST_NO_CXX11_LAMBDAS
#define EFL_EINA_BOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#define EFL_EINA_BOOST_NO_CXX11_NULLPTR
#define EFL_EINA_BOOST_NO_CXX11_RANGE_BASED_FOR
#define EFL_EINA_BOOST_NO_CXX11_RAW_LITERALS
#define EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
#define EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS
#define EFL_EINA_BOOST_NO_SFINAE_EXPR
#define EFL_EINA_BOOST_NO_CXX11_TEMPLATE_ALIASES
#define EFL_EINA_BOOST_NO_CXX11_UNICODE_LITERALS    // UTF-8 still not supported
#define EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES
#define EFL_EINA_BOOST_NO_CXX11_NOEXCEPT
#define EFL_EINA_BOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#define EFL_EINA_BOOST_NO_CXX11_USER_DEFINED_LITERALS
#define EFL_EINA_BOOST_NO_CXX11_ALIGNAS
#define EFL_EINA_BOOST_NO_CXX11_TRAILING_RESULT_TYPES
#define EFL_EINA_BOOST_NO_CXX11_INLINE_NAMESPACES

#if __BORLANDC__ >= 0x590
#  define EFL_EINA_BOOST_HAS_TR1_HASH

#  define EFL_EINA_BOOST_HAS_MACRO_USE_FACET
#endif

//
// Post 0x561 we have long long and stdint.h:
#if __BORLANDC__ >= 0x561
#  ifndef __NO_LONG_LONG
#     define EFL_EINA_BOOST_HAS_LONG_LONG
#  else
#     define EFL_EINA_BOOST_NO_LONG_LONG
#  endif
   // On non-Win32 platforms let the platform config figure this out:
#  ifdef _WIN32
#      define EFL_EINA_BOOST_HAS_STDINT_H
#  endif
#endif

// Borland C++Builder 6 defaults to using STLPort.  If _USE_OLD_RW_STL is
// defined, then we have 0x560 or greater with the Rogue Wave implementation
// which presumably has the std::DBL_MAX bug.
#if defined( EFL_EINA_BOOST_BCB_WITH_ROGUE_WAVE )
// <climits> is partly broken, some macros define symbols that are really in
// namespace std, so you end up having to use illegal constructs like
// std::DBL_MAX, as a fix we'll just include float.h and have done with:
#include <float.h>
#endif
//
// __int64:
//
#if (__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__)
#  define EFL_EINA_BOOST_HAS_MS_INT64
#endif
//
// check for exception handling support:
//
#if !defined(_CPPUNWIND) && !defined(EFL_EINA_BOOST_CPPUNWIND) && !defined(__EXCEPTIONS) && !defined(EFL_EINA_BOOST_NO_EXCEPTIONS)
#  define EFL_EINA_BOOST_NO_EXCEPTIONS
#endif
//
// all versions have a <dirent.h>:
//
#ifndef __STRICT_ANSI__
#  define EFL_EINA_BOOST_HAS_DIRENT_H
#endif
//
// all versions support __declspec:
//
#if defined(__STRICT_ANSI__)
// config/platform/win32.hpp will define EFL_EINA_BOOST_SYMBOL_EXPORT, etc., unless already defined
#  define EFL_EINA_BOOST_SYMBOL_EXPORT
#endif
//
// ABI fixing headers:
//
#if __BORLANDC__ != 0x600 // not implemented for version 6 compiler yet
#ifndef EFL_EINA_BOOST_ABI_PREFIX
#  define EFL_EINA_BOOST_ABI_PREFIX "eina_boost/config/abi/borland_prefix.hpp"
#endif
#ifndef EFL_EINA_BOOST_ABI_SUFFIX
#  define EFL_EINA_BOOST_ABI_SUFFIX "eina_boost/config/abi/borland_suffix.hpp"
#endif
#endif
//
// Disable Win32 support in ANSI mode:
//
#if __BORLANDC__ < 0x600
#  pragma defineonoption EFL_EINA_BOOST_DISABLE_WIN32 -A
#elif defined(__STRICT_ANSI__)
#  define EFL_EINA_BOOST_DISABLE_WIN32
#endif
//
// MSVC compatibility mode does some nasty things:
// TODO: look up if this doesn't apply to the whole 12xx range
//
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
#  define EFL_EINA_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#  define EFL_EINA_BOOST_NO_VOID_RETURNS
#endif

// Borland did not implement value-initialization completely, as I reported
// in 2007, Borland Report 51854, "Value-initialization: POD struct should be
// zero-initialized", http://qc.embarcadero.com/wc/qcmain.aspx?d=51854
// See also: http://www.boost.org/libs/utility/value_init.htm#compiler_issues
// (Niels Dekker, LKEB, April 2010)
#define EFL_EINA_BOOST_NO_COMPLETE_VALUE_INITIALIZATION

#define EFL_EINA_BOOST_COMPILER "Borland C++ version " EFL_EINA_BOOST_STRINGIZE(__BORLANDC__)
