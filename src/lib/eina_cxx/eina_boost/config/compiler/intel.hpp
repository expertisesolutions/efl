//  (C) Copyright John Maddock 2001-8.
//  (C) Copyright Peter Dimov 2001.
//  (C) Copyright Jens Maurer 2001.
//  (C) Copyright David Abrahams 2002 - 2003.
//  (C) Copyright Aleksey Gurtovoy 2002 - 2003.
//  (C) Copyright Guillaume Melquiond 2002 - 2003.
//  (C) Copyright Beman Dawes 2003.
//  (C) Copyright Martin Wille 2003.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Intel compiler setup:

#include "eina_boost/config/compiler/common_edg.hpp"

#if defined(__INTEL_COMPILER)
#  define EFL_EINA_BOOST_INTEL_CXX_VERSION __INTEL_COMPILER
#elif defined(__ICL)
#  define EFL_EINA_BOOST_INTEL_CXX_VERSION __ICL
#elif defined(__ICC)
#  define EFL_EINA_BOOST_INTEL_CXX_VERSION __ICC
#elif defined(__ECC)
#  define EFL_EINA_BOOST_INTEL_CXX_VERSION __ECC
#endif

// Flags determined by comparing output of 'icpc -dM -E' with and without '-std=c++0x'
#if (!(defined(_WIN32) || defined(_WIN64)) && defined(__STDC_HOSTED__) && (__STDC_HOSTED__ && (EFL_EINA_BOOST_INTEL_CXX_VERSION <= 1200))) || defined(__GXX_EXPERIMENTAL_CPP0X__) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#  define EFL_EINA_BOOST_INTEL_STDCXX0X
#endif
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#  define EFL_EINA_BOOST_INTEL_STDCXX0X
#endif

#ifdef EFL_EINA_BOOST_INTEL_STDCXX0X
#define EFL_EINA_BOOST_COMPILER "Intel C++ C++0x mode version " EFL_EINA_BOOST_STRINGIZE(EFL_EINA_BOOST_INTEL_CXX_VERSION)
#else
#define EFL_EINA_BOOST_COMPILER "Intel C++ version " EFL_EINA_BOOST_STRINGIZE(EFL_EINA_BOOST_INTEL_CXX_VERSION)
#endif
#define EFL_EINA_BOOST_INTEL EFL_EINA_BOOST_INTEL_CXX_VERSION

#if defined(_WIN32) || defined(_WIN64)
#  define EFL_EINA_BOOST_INTEL_WIN EFL_EINA_BOOST_INTEL
#else
#  define EFL_EINA_BOOST_INTEL_LINUX EFL_EINA_BOOST_INTEL
#endif

#if (EFL_EINA_BOOST_INTEL_CXX_VERSION <= 600)

#  if defined(_MSC_VER) && (_MSC_VER <= 1300) // added check for <= VC 7 (Peter Dimov)

// Boost libraries assume strong standard conformance unless otherwise
// indicated by a config macro. As configured by Intel, the EDG front-end
// requires certain compiler options be set to achieve that strong conformance.
// Particularly /Qoption,c,--arg_dep_lookup (reported by Kirk Klobe & Thomas Witt)
// and /Zc:wchar_t,forScope. See boost-root/tools/build/intel-win32-tools.jam for
// details as they apply to particular versions of the compiler. When the
// compiler does not predefine a macro indicating if an option has been set,
// this config file simply assumes the option has been set.
// Thus EFL_EINA_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP will not be defined, even if
// the compiler option is not enabled.

#     define EFL_EINA_BOOST_NO_SWPRINTF
#  endif

// Void returns, 64 bit integrals don't work when emulating VC 6 (Peter Dimov)

#  if defined(_MSC_VER) && (_MSC_VER <= 1200)
#     define EFL_EINA_BOOST_NO_VOID_RETURNS
#     define EFL_EINA_BOOST_NO_INTEGRAL_INT64_T
#  endif

#endif

#if (EFL_EINA_BOOST_INTEL_CXX_VERSION <= 710) && defined(_WIN32)
#  define EFL_EINA_BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS
#endif

// See http://aspn.activestate.com/ASPN/Mail/Message/boost/1614864
#if EFL_EINA_BOOST_INTEL_CXX_VERSION < 600
#  define EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T
#else
// We should test the macro _WCHAR_T_DEFINED to check if the compiler
// supports wchar_t natively. *BUT* there is a problem here: the standard
// headers define this macro if they typedef wchar_t. Anyway, we're lucky
// because they define it without a value, while Intel C++ defines it
// to 1. So we can check its value to see if the macro was defined natively
// or not.
// Under UNIX, the situation is exactly the same, but the macro _WCHAR_T
// is used instead.
#  if ((_WCHAR_T_DEFINED + 0) == 0) && ((_WCHAR_T + 0) == 0)
#    define EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T
#  endif
#endif

#if defined(__GNUC__) && !defined(EFL_EINA_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL)
//
// Figure out when Intel is emulating this gcc bug
// (All Intel versions prior to 9.0.26, and versions
// later than that if they are set up to emulate gcc 3.2
// or earlier):
//
#  if ((__GNUC__ == 3) && (__GNUC_MINOR__ <= 2)) || (EFL_EINA_BOOST_INTEL < 900) || (__INTEL_COMPILER_BUILD_DATE < 20050912)
#     define EFL_EINA_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#  endif
#endif
#if (defined(__GNUC__) && (__GNUC__ < 4)) || (defined(_WIN32) && (EFL_EINA_BOOST_INTEL_CXX_VERSION <= 1200)) || (EFL_EINA_BOOST_INTEL_CXX_VERSION <= 1200)
// GCC or VC emulation:
#define EFL_EINA_BOOST_NO_TWO_PHASE_NAME_LOOKUP
#endif
//
// Verify that we have actually got EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T
// set correctly, if we don't do this now, we will get errors later
// in type_traits code among other things, getting this correct
// for the Intel compiler is actually remarkably fragile and tricky:
//
#ifdef __cplusplus
#if defined(EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T)
#include <cwchar>
template< typename T > struct assert_no_intrinsic_wchar_t;
template<> struct assert_no_intrinsic_wchar_t<wchar_t> { typedef void type; };
// if you see an error here then you need to unset EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T
// where it is defined above:
typedef assert_no_intrinsic_wchar_t<unsigned short>::type assert_no_intrinsic_wchar_t_;
#else
template< typename T > struct assert_intrinsic_wchar_t;
template<> struct assert_intrinsic_wchar_t<wchar_t> {};
// if you see an error here then define EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T on the command line:
template<> struct assert_intrinsic_wchar_t<unsigned short> {};
#endif
#endif

#if defined(_MSC_VER) && (_MSC_VER+0 >= 1000)
#  if _MSC_VER >= 1200
#     define EFL_EINA_BOOST_HAS_MS_INT64
#  endif
#  define EFL_EINA_BOOST_NO_SWPRINTF
#  define EFL_EINA_BOOST_NO_TWO_PHASE_NAME_LOOKUP
#elif defined(_WIN32)
#  define EFL_EINA_BOOST_DISABLE_WIN32
#endif

// I checked version 6.0 build 020312Z, it implements the NRVO.
// Correct this as you find out which version of the compiler
// implemented the NRVO first.  (Daniel Frey)
#if (EFL_EINA_BOOST_INTEL_CXX_VERSION >= 600)
#  define EFL_EINA_BOOST_HAS_NRVO
#endif

// Branch prediction hints
// I'm not sure 8.0 was the first version to support these builtins,
// update the condition if the version is not accurate. (Andrey Semashev)
#if defined(__GNUC__) && EFL_EINA_BOOST_INTEL_CXX_VERSION >= 800
#define EFL_EINA_BOOST_LIKELY(x) __builtin_expect(x, 1)
#define EFL_EINA_BOOST_UNLIKELY(x) __builtin_expect(x, 0)
#endif

//
// versions check:
// we don't support Intel prior to version 6.0:
#if EFL_EINA_BOOST_INTEL_CXX_VERSION < 600
#  error "Compiler not supported or configured - please reconfigure"
#endif

// Intel on MacOS requires
#if defined(__APPLE__) && defined(__INTEL_COMPILER)
#  define EFL_EINA_BOOST_NO_TWO_PHASE_NAME_LOOKUP
#endif

// Intel on Altix Itanium
#if defined(__itanium__) && defined(__INTEL_COMPILER)
#  define EFL_EINA_BOOST_NO_TWO_PHASE_NAME_LOOKUP
#endif

//
// An attempt to value-initialize a pointer-to-member may trigger an
// internal error on Intel <= 11.1 (last checked version), as was
// reported by John Maddock, Intel support issue 589832, May 2010.
// Moreover, according to test results from Huang-Vista-x86_32_intel,
// intel-vc9-win-11.1 may leave a non-POD array uninitialized, in some
// cases when it should be value-initialized.
// (Niels Dekker, LKEB, May 2010)
// Apparently Intel 12.1 (compiler version number 9999 !!) has the same issue (compiler regression).
#if defined(__INTEL_COMPILER)
#  if (__INTEL_COMPILER <= 1110) || (__INTEL_COMPILER == 9999) || (defined(_WIN32) && (__INTEL_COMPILER < 1500))
#    define EFL_EINA_BOOST_NO_COMPLETE_VALUE_INITIALIZATION
#  endif
#endif

//
// Dynamic shared object (DSO) and dynamic-link library (DLL) support
//
#if defined(__GNUC__) && (__GNUC__ >= 4)
#  define EFL_EINA_BOOST_SYMBOL_EXPORT __attribute__((visibility("default")))
#  define EFL_EINA_BOOST_SYMBOL_IMPORT
#  define EFL_EINA_BOOST_SYMBOL_VISIBLE __attribute__((visibility("default")))
#endif
//
// C++0x features
//     - ICC added static_assert in 11.0 (first version with C++0x support)
//
#if defined(EFL_EINA_BOOST_INTEL_STDCXX0X)
#  undef  EFL_EINA_BOOST_NO_CXX11_STATIC_ASSERT
//
// These pass our test cases, but aren't officially supported according to:
// http://software.intel.com/en-us/articles/c0x-features-supported-by-intel-c-compiler/
//
//#  undef  EFL_EINA_BOOST_NO_CXX11_LAMBDAS
//#  undef  EFL_EINA_BOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
//#  undef  EFL_EINA_BOOST_NO_CXX11_DECLTYPE
//#  undef  EFL_EINA_BOOST_NO_CXX11_AUTO_DECLARATIONS
//#  undef  EFL_EINA_BOOST_NO_CXX11_AUTO_MULTIDECLARATIONS
#endif

#if defined(EFL_EINA_BOOST_INTEL_STDCXX0X) && (EFL_EINA_BOOST_INTEL_CXX_VERSION >= 1200)
//#  undef  EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES // Enabling this breaks Filesystem and Exception libraries
//#  undef  EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS  // doesn't really work!!
#  undef  EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS
#  undef  EFL_EINA_BOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#  undef  EFL_EINA_BOOST_NO_CXX11_LAMBDAS
#  undef  EFL_EINA_BOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#  undef  EFL_EINA_BOOST_NO_CXX11_DECLTYPE
#  undef  EFL_EINA_BOOST_NO_CXX11_AUTO_DECLARATIONS
#  undef  EFL_EINA_BOOST_NO_CXX11_AUTO_MULTIDECLARATIONS
#  undef  EFL_EINA_BOOST_NO_CXX11_TRAILING_RESULT_TYPES
#endif

// icl Version 12.1.0.233 Build 20110811 and possibly some other builds
// had an incorrect __INTEL_COMPILER value of 9999. Intel say this has been fixed.
#if defined(EFL_EINA_BOOST_INTEL_STDCXX0X) && (EFL_EINA_BOOST_INTEL_CXX_VERSION > 1200)
#  undef  EFL_EINA_BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#  undef  EFL_EINA_BOOST_NO_CXX11_NULLPTR
#  undef  EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
#  undef  EFL_EINA_BOOST_NO_SFINAE_EXPR
#  undef  EFL_EINA_BOOST_NO_CXX11_TEMPLATE_ALIASES
#  undef  EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES

// http://software.intel.com/en-us/articles/c0x-features-supported-by-intel-c-compiler/
// continues to list scoped enum support as "Partial"
//#  undef  EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS
#endif
#if defined(EFL_EINA_BOOST_INTEL_STDCXX0X) && (EFL_EINA_BOOST_INTEL_CXX_VERSION >= 1310) && !defined(_MSC_VER)
#  undef EFL_EINA_BOOST_NO_CXX11_INLINE_NAMESPACES
#  undef EFL_EINA_BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
// This one generates internal compiler errors in multiprecision, disabled for now:
//#  undef EFL_EINA_BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS  
// This one generates errors when used with conditional exception specifications, for example in multiprecision:
//#  undef EFL_EINA_BOOST_NO_CXX11_NOEXCEPT
#  undef EFL_EINA_BOOST_NO_CXX11_RANGE_BASED_FOR
#  undef EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS
#  undef EFL_EINA_BOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#endif
#if (EFL_EINA_BOOST_INTEL_CXX_VERSION >= 1310)
#  undef  EFL_EINA_BOOST_NO_SFINAE_EXPR
#endif
#if defined(EFL_EINA_BOOST_INTEL_STDCXX0X) && (EFL_EINA_BOOST_INTEL_CXX_VERSION >= 1400) && !defined(_MSC_VER)
#  undef EFL_EINA_BOOST_NO_CXX11_UNICODE_LITERALS 
#  undef EFL_EINA_BOOST_NO_CXX11_RAW_LITERALS 
// This one generates errors when used with conditional exception specifications, for example in multiprecision:
//#  undef EFL_EINA_BOOST_NO_CXX11_NOEXCEPT 
// This breaks multiprecision:
//#  undef EFL_EINA_BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS 
#  undef EFL_EINA_BOOST_NO_CXX11_HDR_THREAD 
#  undef EFL_EINA_BOOST_NO_CXX11_CHAR32_T 
#  undef EFL_EINA_BOOST_NO_CXX11_CHAR16_T
#endif

#if defined(EFL_EINA_BOOST_INTEL_STDCXX0X) && (EFL_EINA_BOOST_INTEL_CXX_VERSION <= 1310)
#  define EFL_EINA_BOOST_NO_CXX11_HDR_FUTURE
#  define EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
#endif

#if defined(EFL_EINA_BOOST_INTEL_STDCXX0X) && (EFL_EINA_BOOST_INTEL_CXX_VERSION == 1400)
// A regression in Intel's compiler means that <tuple> seems to be broken in this release as well as <future> :
#  define EFL_EINA_BOOST_NO_CXX11_HDR_FUTURE
#  define EFL_EINA_BOOST_NO_CXX11_HDR_TUPLE
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1700)
//
// Although the Intel compiler is capable of supporting these, it appears not to in MSVC compatibility mode:
//
#  define  EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
#  define  EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES
#  define  EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS
#  define  EFL_EINA_BOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#  define  EFL_EINA_BOOST_NO_CXX11_TEMPLATE_ALIASES
#  if(EFL_EINA_BOOST_INTEL_CXX_VERSION < 1310)
#     define  EFL_EINA_BOOST_NO_CXX11_TRAILING_RESULT_TYPES
#  endif
#endif

#if (EFL_EINA_BOOST_INTEL_CXX_VERSION < 1200)
//
// fenv.h appears not to work with Intel prior to 12.0:
//
#  define EFL_EINA_BOOST_NO_FENV_H
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#  define EFL_EINA_BOOST_HAS_STDINT_H
#endif

#if defined(__LP64__) && defined(__GNUC__) && (EFL_EINA_BOOST_INTEL_CXX_VERSION >= 1310)
#  define EFL_EINA_BOOST_HAS_INT128
#endif

//
// last known and checked version:
#if (EFL_EINA_BOOST_INTEL_CXX_VERSION > 1310)
#  if defined(EFL_EINA_BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  elif defined(_MSC_VER)
//
//      We don't emit this warning any more, since we have so few
//      defect macros set anyway (just the one).
//
//#     pragma message("Unknown compiler version - please run the configure tests and report the results")
#  endif
#endif

