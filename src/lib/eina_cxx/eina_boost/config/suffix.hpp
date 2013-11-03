//  Boost config.hpp configuration header file  ------------------------------//
//	boostinspect:ndprecated_macros	-- tell the inspect tool to ignore this file

//  Copyright (c) 2001-2003 John Maddock
//  Copyright (c) 2001 Darin Adler
//  Copyright (c) 2001 Peter Dimov
//  Copyright (c) 2002 Bill Kempf
//  Copyright (c) 2002 Jens Maurer
//  Copyright (c) 2002-2003 David Abrahams
//  Copyright (c) 2003 Gennaro Prota
//  Copyright (c) 2003 Eric Friedman
//  Copyright (c) 2010 Eric Jourdanneau, Joel Falcou
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for most recent version.

//  Boost config.hpp policy and rationale documentation has been moved to
//  http://www.boost.org/libs/config/
//
//  This file is intended to be stable, and relatively unchanging.
//  It should contain boilerplate code only - no compiler specific
//  code unless it is unavoidable - no changes unless unavoidable.

#ifndef EFL_EINA_BOOST_CONFIG_SUFFIX_HPP
#define EFL_EINA_BOOST_CONFIG_SUFFIX_HPP

#if defined(__GNUC__) && (__GNUC__ >= 4)
//
// Some GCC-4.x versions issue warnings even when __extension__ is used,
// so use this as a workaround:
//
#pragma GCC system_header
#endif

//
// ensure that visibility macros are always defined, thus symplifying use
//
#ifndef EFL_EINA_BOOST_SYMBOL_EXPORT
# define EFL_EINA_BOOST_SYMBOL_EXPORT
#endif
#ifndef EFL_EINA_BOOST_SYMBOL_IMPORT
# define EFL_EINA_BOOST_SYMBOL_IMPORT
#endif
#ifndef EFL_EINA_BOOST_SYMBOL_VISIBLE
# define EFL_EINA_BOOST_SYMBOL_VISIBLE
#endif

//
// look for long long by looking for the appropriate macros in <limits.h>.
// Note that we use limits.h rather than climits for maximal portability,
// remember that since these just declare a bunch of macros, there should be
// no namespace issues from this.
//
#if !defined(EFL_EINA_BOOST_HAS_LONG_LONG) && !defined(EFL_EINA_BOOST_NO_LONG_LONG)                                              \
   && !defined(EFL_EINA_BOOST_MSVC) && !defined(__BORLANDC__)
# include <limits.h>
# if (defined(ULLONG_MAX) || defined(ULONG_LONG_MAX) || defined(ULONGLONG_MAX))
#   define EFL_EINA_BOOST_HAS_LONG_LONG
# else
#   define EFL_EINA_BOOST_NO_LONG_LONG
# endif
#endif

// GCC 3.x will clean up all of those nasty macro definitions that
// EFL_EINA_BOOST_NO_CTYPE_FUNCTIONS is intended to help work around, so undefine
// it under GCC 3.x.
#if defined(__GNUC__) && (__GNUC__ >= 3) && defined(EFL_EINA_BOOST_NO_CTYPE_FUNCTIONS)
#  undef EFL_EINA_BOOST_NO_CTYPE_FUNCTIONS
#endif

//
// Assume any extensions are in namespace std:: unless stated otherwise:
//
#  ifndef EFL_EINA_BOOST_STD_EXTENSION_NAMESPACE
#    define EFL_EINA_BOOST_STD_EXTENSION_NAMESPACE std
#  endif

//
// If cv-qualified specializations are not allowed, then neither are cv-void ones:
//
#  if defined(EFL_EINA_BOOST_NO_CV_SPECIALIZATIONS) \
      && !defined(EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS)
#     define EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
#  endif

//
// If there is no numeric_limits template, then it can't have any compile time
// constants either!
//
#  if defined(EFL_EINA_BOOST_NO_LIMITS) \
      && !defined(EFL_EINA_BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS)
#     define EFL_EINA_BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#     define EFL_EINA_BOOST_NO_MS_INT64_NUMERIC_LIMITS
#     define EFL_EINA_BOOST_NO_LONG_LONG_NUMERIC_LIMITS
#  endif

//
// if there is no long long then there is no specialisation
// for numeric_limits<long long> either:
//
#if !defined(EFL_EINA_BOOST_HAS_LONG_LONG) && !defined(EFL_EINA_BOOST_NO_LONG_LONG_NUMERIC_LIMITS)
#  define EFL_EINA_BOOST_NO_LONG_LONG_NUMERIC_LIMITS
#endif

//
// if there is no __int64 then there is no specialisation
// for numeric_limits<__int64> either:
//
#if !defined(EFL_EINA_BOOST_HAS_MS_INT64) && !defined(EFL_EINA_BOOST_NO_MS_INT64_NUMERIC_LIMITS)
#  define EFL_EINA_BOOST_NO_MS_INT64_NUMERIC_LIMITS
#endif

//
// if member templates are supported then so is the
// VC6 subset of member templates:
//
#  if !defined(EFL_EINA_BOOST_NO_MEMBER_TEMPLATES) \
       && !defined(EFL_EINA_BOOST_MSVC6_MEMBER_TEMPLATES)
#     define EFL_EINA_BOOST_MSVC6_MEMBER_TEMPLATES
#  endif

//
// Without partial specialization, can't test for partial specialisation bugs:
//
#  if defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(EFL_EINA_BOOST_BCB_PARTIAL_SPECIALIZATION_BUG)
#     define EFL_EINA_BOOST_BCB_PARTIAL_SPECIALIZATION_BUG
#  endif

//
// Without partial specialization, we can't have array-type partial specialisations:
//
#  if defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(EFL_EINA_BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
#     define EFL_EINA_BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS
#  endif

//
// Without partial specialization, std::iterator_traits can't work:
//
#  if defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(EFL_EINA_BOOST_NO_STD_ITERATOR_TRAITS)
#     define EFL_EINA_BOOST_NO_STD_ITERATOR_TRAITS
#  endif

//
// Without partial specialization, partial
// specialization with default args won't work either:
//
#  if defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(EFL_EINA_BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
#     define EFL_EINA_BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
#  endif

//
// Without member template support, we can't have template constructors
// in the standard library either:
//
#  if defined(EFL_EINA_BOOST_NO_MEMBER_TEMPLATES) \
      && !defined(EFL_EINA_BOOST_MSVC6_MEMBER_TEMPLATES) \
      && !defined(EFL_EINA_BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
#     define EFL_EINA_BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#  endif

//
// Without member template support, we can't have a conforming
// std::allocator template either:
//
#  if defined(EFL_EINA_BOOST_NO_MEMBER_TEMPLATES) \
      && !defined(EFL_EINA_BOOST_MSVC6_MEMBER_TEMPLATES) \
      && !defined(EFL_EINA_BOOST_NO_STD_ALLOCATOR)
#     define EFL_EINA_BOOST_NO_STD_ALLOCATOR
#  endif

//
// without ADL support then using declarations will break ADL as well:
//
#if defined(EFL_EINA_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP) && !defined(EFL_EINA_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL)
#  define EFL_EINA_BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
#endif

//
// Without typeid support we have no dynamic RTTI either:
//
#if defined(EFL_EINA_BOOST_NO_TYPEID) && !defined(EFL_EINA_BOOST_NO_RTTI)
#  define EFL_EINA_BOOST_NO_RTTI
#endif

//
// If we have a standard allocator, then we have a partial one as well:
//
#if !defined(EFL_EINA_BOOST_NO_STD_ALLOCATOR)
#  define EFL_EINA_BOOST_HAS_PARTIAL_STD_ALLOCATOR
#endif

//
// We can't have a working std::use_facet if there is no std::locale:
//
#  if defined(EFL_EINA_BOOST_NO_STD_LOCALE) && !defined(EFL_EINA_BOOST_NO_STD_USE_FACET)
#     define EFL_EINA_BOOST_NO_STD_USE_FACET
#  endif

//
// We can't have a std::messages facet if there is no std::locale:
//
#  if defined(EFL_EINA_BOOST_NO_STD_LOCALE) && !defined(EFL_EINA_BOOST_NO_STD_MESSAGES)
#     define EFL_EINA_BOOST_NO_STD_MESSAGES
#  endif

//
// We can't have a working std::wstreambuf if there is no std::locale:
//
#  if defined(EFL_EINA_BOOST_NO_STD_LOCALE) && !defined(EFL_EINA_BOOST_NO_STD_WSTREAMBUF)
#     define EFL_EINA_BOOST_NO_STD_WSTREAMBUF
#  endif

//
// We can't have a <cwctype> if there is no <cwchar>:
//
#  if defined(EFL_EINA_BOOST_NO_CWCHAR) && !defined(EFL_EINA_BOOST_NO_CWCTYPE)
#     define EFL_EINA_BOOST_NO_CWCTYPE
#  endif

//
// We can't have a swprintf if there is no <cwchar>:
//
#  if defined(EFL_EINA_BOOST_NO_CWCHAR) && !defined(EFL_EINA_BOOST_NO_SWPRINTF)
#     define EFL_EINA_BOOST_NO_SWPRINTF
#  endif

//
// If Win32 support is turned off, then we must turn off
// threading support also, unless there is some other
// thread API enabled:
//
#if defined(EFL_EINA_BOOST_DISABLE_WIN32) && defined(_WIN32) \
   && !defined(EFL_EINA_BOOST_DISABLE_THREADS) && !defined(EFL_EINA_BOOST_HAS_PTHREADS)
#  define EFL_EINA_BOOST_DISABLE_THREADS
#endif

//
// Turn on threading support if the compiler thinks that it's in
// multithreaded mode.  We put this here because there are only a
// limited number of macros that identify this (if there's any missing
// from here then add to the appropriate compiler section):
//
#if (defined(__MT__) || defined(_MT) || defined(_REENTRANT) \
    || defined(_PTHREADS) || defined(__APPLE__) || defined(__DragonFly__)) \
    && !defined(EFL_EINA_BOOST_HAS_THREADS)
#  define EFL_EINA_BOOST_HAS_THREADS
#endif

//
// Turn threading support off if EFL_EINA_BOOST_DISABLE_THREADS is defined:
//
#if defined(EFL_EINA_BOOST_DISABLE_THREADS) && defined(EFL_EINA_BOOST_HAS_THREADS)
#  undef EFL_EINA_BOOST_HAS_THREADS
#endif

//
// Turn threading support off if we don't recognise the threading API:
//
#if defined(EFL_EINA_BOOST_HAS_THREADS) && !defined(EFL_EINA_BOOST_HAS_PTHREADS)\
      && !defined(EFL_EINA_BOOST_HAS_WINTHREADS) && !defined(EFL_EINA_BOOST_HAS_BETHREADS)\
      && !defined(EFL_EINA_BOOST_HAS_MPTASKS)
#  undef EFL_EINA_BOOST_HAS_THREADS
#endif

//
// Turn threading detail macros off if we don't (want to) use threading
//
#ifndef EFL_EINA_BOOST_HAS_THREADS
#  undef EFL_EINA_BOOST_HAS_PTHREADS
#  undef EFL_EINA_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#  undef EFL_EINA_BOOST_HAS_PTHREAD_YIELD
#  undef EFL_EINA_BOOST_HAS_PTHREAD_DELAY_NP
#  undef EFL_EINA_BOOST_HAS_WINTHREADS
#  undef EFL_EINA_BOOST_HAS_BETHREADS
#  undef EFL_EINA_BOOST_HAS_MPTASKS
#endif

//
// If the compiler claims to be C99 conformant, then it had better
// have a <stdint.h>:
//
#  if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#     define EFL_EINA_BOOST_HAS_STDINT_H
#     ifndef EFL_EINA_BOOST_HAS_LOG1P
#        define EFL_EINA_BOOST_HAS_LOG1P
#     endif
#     ifndef EFL_EINA_BOOST_HAS_EXPM1
#        define EFL_EINA_BOOST_HAS_EXPM1
#     endif
#  endif

//
// Define EFL_EINA_BOOST_NO_SLIST and EFL_EINA_BOOST_NO_HASH if required.
// Note that this is for backwards compatibility only.
//
#  if !defined(EFL_EINA_BOOST_HAS_SLIST) && !defined(EFL_EINA_BOOST_NO_SLIST)
#     define EFL_EINA_BOOST_NO_SLIST
#  endif

#  if !defined(EFL_EINA_BOOST_HAS_HASH) && !defined(EFL_EINA_BOOST_NO_HASH)
#     define EFL_EINA_BOOST_NO_HASH
#  endif

//
// Set EFL_EINA_BOOST_SLIST_HEADER if not set already:
//
#if defined(EFL_EINA_BOOST_HAS_SLIST) && !defined(EFL_EINA_BOOST_SLIST_HEADER)
#  define EFL_EINA_BOOST_SLIST_HEADER <slist>
#endif

//
// Set EFL_EINA_BOOST_HASH_SET_HEADER if not set already:
//
#if defined(EFL_EINA_BOOST_HAS_HASH) && !defined(EFL_EINA_BOOST_HASH_SET_HEADER)
#  define EFL_EINA_BOOST_HASH_SET_HEADER <hash_set>
#endif

//
// Set EFL_EINA_BOOST_HASH_MAP_HEADER if not set already:
//
#if defined(EFL_EINA_BOOST_HAS_HASH) && !defined(EFL_EINA_BOOST_HASH_MAP_HEADER)
#  define EFL_EINA_BOOST_HASH_MAP_HEADER <hash_map>
#endif

//  EFL_EINA_BOOST_HAS_ABI_HEADERS
//  This macro gets set if we have headers that fix the ABI,
//  and prevent ODR violations when linking to external libraries:
#if defined(EFL_EINA_BOOST_ABI_PREFIX) && defined(EFL_EINA_BOOST_ABI_SUFFIX) && !defined(EFL_EINA_BOOST_HAS_ABI_HEADERS)
#  define EFL_EINA_BOOST_HAS_ABI_HEADERS
#endif

#if defined(EFL_EINA_BOOST_HAS_ABI_HEADERS) && defined(EFL_EINA_BOOST_DISABLE_ABI_HEADERS)
#  undef EFL_EINA_BOOST_HAS_ABI_HEADERS
#endif

//  EFL_EINA_BOOST_NO_STDC_NAMESPACE workaround  --------------------------------------//
//  Because std::size_t usage is so common, even in boost headers which do not
//  otherwise use the C library, the <cstddef> workaround is included here so
//  that ugly workaround code need not appear in many other boost headers.
//  NOTE WELL: This is a workaround for non-conforming compilers; <cstddef>
//  must still be #included in the usual places so that <cstddef> inclusion
//  works as expected with standard conforming compilers.  The resulting
//  double inclusion of <cstddef> is harmless.

# if defined(EFL_EINA_BOOST_NO_STDC_NAMESPACE) && defined(__cplusplus)
#   include <cstddef>
    namespace std { using ::ptrdiff_t; using ::size_t; }
# endif

//  Workaround for the unfortunate min/max macros defined by some platform headers

#define EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION

#ifndef EFL_EINA_BOOST_USING_STD_MIN
#  define EFL_EINA_BOOST_USING_STD_MIN() using std::min
#endif

#ifndef EFL_EINA_BOOST_USING_STD_MAX
#  define EFL_EINA_BOOST_USING_STD_MAX() using std::max
#endif

//  EFL_EINA_BOOST_NO_STD_MIN_MAX workaround  -----------------------------------------//

#  if defined(EFL_EINA_BOOST_NO_STD_MIN_MAX) && defined(__cplusplus)

namespace std {
  template <class _Tp>
  inline const _Tp& min EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (const _Tp& __a, const _Tp& __b) {
    return __b < __a ? __b : __a;
  }
  template <class _Tp>
  inline const _Tp& max EFL_EINA_BOOST_PREVENT_MACRO_SUBSTITUTION (const _Tp& __a, const _Tp& __b) {
    return  __a < __b ? __b : __a;
  }
}

#  endif

// EFL_EINA_BOOST_STATIC_CONSTANT workaround --------------------------------------- //
// On compilers which don't allow in-class initialization of static integral
// constant members, we must use enums as a workaround if we want the constants
// to be available at compile-time. This macro gives us a convenient way to
// declare such constants.

#  ifdef EFL_EINA_BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#       define EFL_EINA_BOOST_STATIC_CONSTANT(type, assignment) enum { assignment }
#  else
#     define EFL_EINA_BOOST_STATIC_CONSTANT(type, assignment) static const type assignment
#  endif

// EFL_EINA_BOOST_USE_FACET / HAS_FACET workaround ----------------------------------//
// When the standard library does not have a conforming std::use_facet there
// are various workarounds available, but they differ from library to library.
// The same problem occurs with has_facet.
// These macros provide a consistent way to access a locale's facets.
// Usage:
//    replace
//       std::use_facet<Type>(loc);
//    with
//       EFL_EINA_BOOST_USE_FACET(Type, loc);
//    Note do not add a std:: prefix to the front of EFL_EINA_BOOST_USE_FACET!
//  Use for EFL_EINA_BOOST_HAS_FACET is analogous.

#if defined(EFL_EINA_BOOST_NO_STD_USE_FACET)
#  ifdef EFL_EINA_BOOST_HAS_TWO_ARG_USE_FACET
#     define EFL_EINA_BOOST_USE_FACET(Type, loc) std::use_facet(loc, static_cast<Type*>(0))
#     define EFL_EINA_BOOST_HAS_FACET(Type, loc) std::has_facet(loc, static_cast<Type*>(0))
#  elif defined(EFL_EINA_BOOST_HAS_MACRO_USE_FACET)
#     define EFL_EINA_BOOST_USE_FACET(Type, loc) std::_USE(loc, Type)
#     define EFL_EINA_BOOST_HAS_FACET(Type, loc) std::_HAS(loc, Type)
#  elif defined(EFL_EINA_BOOST_HAS_STLP_USE_FACET)
#     define EFL_EINA_BOOST_USE_FACET(Type, loc) (*std::_Use_facet<Type >(loc))
#     define EFL_EINA_BOOST_HAS_FACET(Type, loc) std::has_facet< Type >(loc)
#  endif
#else
#  define EFL_EINA_BOOST_USE_FACET(Type, loc) std::use_facet< Type >(loc)
#  define EFL_EINA_BOOST_HAS_FACET(Type, loc) std::has_facet< Type >(loc)
#endif

// EFL_EINA_BOOST_NESTED_TEMPLATE workaround ------------------------------------------//
// Member templates are supported by some compilers even though they can't use
// the A::template member<U> syntax, as a workaround replace:
//
// typedef typename A::template rebind<U> binder;
//
// with:
//
// typedef typename A::EFL_EINA_BOOST_NESTED_TEMPLATE rebind<U> binder;

#ifndef EFL_EINA_BOOST_NO_MEMBER_TEMPLATE_KEYWORD
#  define EFL_EINA_BOOST_NESTED_TEMPLATE template
#else
#  define EFL_EINA_BOOST_NESTED_TEMPLATE
#endif

// EFL_EINA_BOOST_UNREACHABLE_RETURN(x) workaround -------------------------------------//
// Normally evaluates to nothing, unless EFL_EINA_BOOST_NO_UNREACHABLE_RETURN_DETECTION
// is defined, in which case it evaluates to return x; Use when you have a return
// statement that can never be reached.

#ifdef EFL_EINA_BOOST_NO_UNREACHABLE_RETURN_DETECTION
#  define EFL_EINA_BOOST_UNREACHABLE_RETURN(x) return x;
#else
#  define EFL_EINA_BOOST_UNREACHABLE_RETURN(x)
#endif

// EFL_EINA_BOOST_DEDUCED_TYPENAME workaround ------------------------------------------//
//
// Some compilers don't support the use of `typename' for dependent
// types in deduced contexts, e.g.
//
//     template <class T> void f(T, typename T::type);
//                                  ^^^^^^^^
// Replace these declarations with:
//
//     template <class T> void f(T, EFL_EINA_BOOST_DEDUCED_TYPENAME T::type);

#ifndef EFL_EINA_BOOST_NO_DEDUCED_TYPENAME
#  define EFL_EINA_BOOST_DEDUCED_TYPENAME typename
#else
#  define EFL_EINA_BOOST_DEDUCED_TYPENAME
#endif

#ifndef EFL_EINA_BOOST_NO_TYPENAME_WITH_CTOR
#  define EFL_EINA_BOOST_CTOR_TYPENAME typename
#else
#  define EFL_EINA_BOOST_CTOR_TYPENAME
#endif

// long long workaround ------------------------------------------//
// On gcc (and maybe other compilers?) long long is alway supported
// but it's use may generate either warnings (with -ansi), or errors
// (with -pedantic -ansi) unless it's use is prefixed by __extension__
//
#if defined(EFL_EINA_BOOST_HAS_LONG_LONG) && defined(__cplusplus)
namespace efl_eina_boost{
#  ifdef __GNUC__
   __extension__ typedef long long long_long_type;
   __extension__ typedef unsigned long long ulong_long_type;
#  else
   typedef long long long_long_type;
   typedef unsigned long long ulong_long_type;
#  endif
}
#endif
// same again for __int128:
#if defined(EFL_EINA_BOOST_HAS_INT128) && defined(__cplusplus)
namespace efl_eina_boost{
#  ifdef __GNUC__
   __extension__ typedef __int128 int128_type;
   __extension__ typedef unsigned __int128 uint128_type;
#  else
   typedef __int128 int128_type;
   typedef unsigned __int128 uint128_type;
#  endif
}
#endif

// EFL_EINA_BOOST_[APPEND_]EXPLICIT_TEMPLATE_[NON_]TYPE macros --------------------------//

// These macros are obsolete. Port away and remove.

#  define EFL_EINA_BOOST_EXPLICIT_TEMPLATE_TYPE(t)
#  define EFL_EINA_BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(t)
#  define EFL_EINA_BOOST_EXPLICIT_TEMPLATE_NON_TYPE(t, v)
#  define EFL_EINA_BOOST_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)

#  define EFL_EINA_BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(t)
#  define EFL_EINA_BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(t)
#  define EFL_EINA_BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(t, v)
#  define EFL_EINA_BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE_SPEC(t, v)

// When EFL_EINA_BOOST_NO_STD_TYPEINFO is defined, we can just import
// the global definition into std namespace:
#if defined(EFL_EINA_BOOST_NO_STD_TYPEINFO) && defined(__cplusplus)
#include <typeinfo>
namespace std{ using ::type_info; }
#endif

// ---------------------------------------------------------------------------//

//
// Helper macro EFL_EINA_BOOST_STRINGIZE:
// Converts the parameter X to a string after macro replacement
// on X has been performed.
//
#define EFL_EINA_BOOST_STRINGIZE(X) EFL_EINA_BOOST_DO_STRINGIZE(X)
#define EFL_EINA_BOOST_DO_STRINGIZE(X) #X

//
// Helper macro EFL_EINA_BOOST_JOIN:
// The following piece of macro magic joins the two
// arguments together, even when one of the arguments is
// itself a macro (see 16.3.1 in C++ standard).  The key
// is that macro expansion of macro arguments does not
// occur in EFL_EINA_BOOST_DO_JOIN2 but does in EFL_EINA_BOOST_DO_JOIN.
//
#define EFL_EINA_BOOST_JOIN( X, Y ) EFL_EINA_BOOST_DO_JOIN( X, Y )
#define EFL_EINA_BOOST_DO_JOIN( X, Y ) EFL_EINA_BOOST_DO_JOIN2(X,Y)
#define EFL_EINA_BOOST_DO_JOIN2( X, Y ) X##Y

//
// Set some default values for compiler/library/platform names.
// These are for debugging config setup only:
//
#  ifndef EFL_EINA_BOOST_COMPILER
#     define EFL_EINA_BOOST_COMPILER "Unknown ISO C++ Compiler"
#  endif
#  ifndef EFL_EINA_BOOST_STDLIB
#     define EFL_EINA_BOOST_STDLIB "Unknown ISO standard library"
#  endif
#  ifndef EFL_EINA_BOOST_PLATFORM
#     if defined(unix) || defined(__unix) || defined(_XOPEN_SOURCE) \
         || defined(_POSIX_SOURCE)
#        define EFL_EINA_BOOST_PLATFORM "Generic Unix"
#     else
#        define EFL_EINA_BOOST_PLATFORM "Unknown"
#     endif
#  endif

//
// Set some default values GPU support
//
#  ifndef EFL_EINA_BOOST_GPU_ENABLED
#  define EFL_EINA_BOOST_GPU_ENABLED
#  endif

// EFL_EINA_BOOST_FORCEINLINE ---------------------------------------------//
// Macro to use in place of 'inline' to force a function to be inline
#if !defined(EFL_EINA_BOOST_FORCEINLINE)
#  if defined(_MSC_VER)
#    define EFL_EINA_BOOST_FORCEINLINE __forceinline
#  elif defined(__GNUC__) && __GNUC__ > 3
     // Clang also defines __GNUC__ (as 4)
#    define EFL_EINA_BOOST_FORCEINLINE inline __attribute__ ((__always_inline__))
#  else
#    define EFL_EINA_BOOST_FORCEINLINE inline
#  endif
#endif

// EFL_EINA_BOOST_NOINLINE ---------------------------------------------//
// Macro to use in place of 'inline' to prevent a function to be inlined
#if !defined(EFL_EINA_BOOST_NOINLINE)
#  if defined(_MSC_VER)
#    define EFL_EINA_BOOST_NOINLINE __declspec(noinline)
#  elif defined(__GNUC__) && __GNUC__ > 3
     // Clang also defines __GNUC__ (as 4)
#    define EFL_EINA_BOOST_NOINLINE __attribute__ ((__noinline__))
#  else
#    define EFL_EINA_BOOST_NOINLINE
#  endif
#endif

// Branch prediction hints
// These macros are intended to wrap conditional expressions that yield true or false
//
//  if (EFL_EINA_BOOST_LIKELY(var == 10))
//  {
//     // the most probable code here
//  }
//
#if !defined(EFL_EINA_BOOST_LIKELY)
#  define EFL_EINA_BOOST_LIKELY(x) x
#endif
#if !defined(EFL_EINA_BOOST_UNLIKELY)
#  define EFL_EINA_BOOST_UNLIKELY(x) x
#endif

// Type and data alignment specification
//
#if !defined(EFL_EINA_BOOST_NO_CXX11_ALIGNAS)
#  define EFL_EINA_BOOST_ALIGNMENT(x) alignas(x)
#elif defined(_MSC_VER)
#  define EFL_EINA_BOOST_ALIGNMENT(x) __declspec(align(x))
#elif defined(__GNUC__)
#  define EFL_EINA_BOOST_ALIGNMENT(x) __attribute__ ((__aligned__(x)))
#else
#  define EFL_EINA_BOOST_NO_ALIGNMENT
#  define EFL_EINA_BOOST_ALIGNMENT(x)
#endif

// Defaulted and deleted function declaration helpers
// These macros are intended to be inside a class definition.
// EFL_EINA_BOOST_DEFAULTED_FUNCTION accepts the function declaration and its
// body, which will be used if the compiler doesn't support defaulted functions.
// EFL_EINA_BOOST_DELETED_FUNCTION only accepts the function declaration. It
// will expand to a private function declaration, if the compiler doesn't support
// deleted functions. Because of this it is recommended to use EFL_EINA_BOOST_DELETED_FUNCTION
// in the end of the class definition.
//
//  class my_class
//  {
//  public:
//      // Default-constructible
//      EFL_EINA_BOOST_DEFAULTED_FUNCTION(my_class(), {})
//      // Copying prohibited
//      EFL_EINA_BOOST_DELETED_FUNCTION(my_class(my_class const&))
//      EFL_EINA_BOOST_DELETED_FUNCTION(my_class& operator= (my_class const&))
//  };
//
#if !(defined(EFL_EINA_BOOST_NO_CXX11_DEFAULTED_FUNCTIONS) || defined(EFL_EINA_BOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS))
#   define EFL_EINA_BOOST_DEFAULTED_FUNCTION(fun, body) fun = default;
#else
#   define EFL_EINA_BOOST_DEFAULTED_FUNCTION(fun, body) fun body
#endif

#if !defined(EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS)
#   define EFL_EINA_BOOST_DELETED_FUNCTION(fun) fun = delete;
#else
#   define EFL_EINA_BOOST_DELETED_FUNCTION(fun) private: fun;
#endif

//
// Set EFL_EINA_BOOST_NO_DECLTYPE_N3276 when EFL_EINA_BOOST_NO_DECLTYPE is defined
//
#if defined(EFL_EINA_BOOST_NO_CXX11_DECLTYPE) && !defined(EFL_EINA_BOOST_NO_CXX11_DECLTYPE_N3276)
#define	EFL_EINA_BOOST_NO_CXX11_DECLTYPE_N3276 EFL_EINA_BOOST_NO_CXX11_DECLTYPE
#endif

//  -------------------- Deprecated macros for 1.50 ---------------------------
//  These will go away in a future release

//  Use EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_SET or EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_MAP
//           instead of EFL_EINA_BOOST_NO_STD_UNORDERED
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_MAP) || defined (EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_SET)
# ifndef EFL_EINA_BOOST_NO_CXX11_STD_UNORDERED
#  define EFL_EINA_BOOST_NO_CXX11_STD_UNORDERED
# endif
#endif

//  Use EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST instead of EFL_EINA_BOOST_NO_INITIALIZER_LISTS
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST) && !defined(EFL_EINA_BOOST_NO_INITIALIZER_LISTS)
#  define EFL_EINA_BOOST_NO_INITIALIZER_LISTS
#endif

//  Use EFL_EINA_BOOST_NO_CXX11_HDR_ARRAY instead of EFL_EINA_BOOST_NO_0X_HDR_ARRAY
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_ARRAY) && !defined(EFL_EINA_BOOST_NO_0X_HDR_ARRAY)
#  define EFL_EINA_BOOST_NO_0X_HDR_ARRAY
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_CHRONO instead of EFL_EINA_BOOST_NO_0X_HDR_CHRONO
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_CHRONO) && !defined(EFL_EINA_BOOST_NO_0X_HDR_CHRONO)
#  define EFL_EINA_BOOST_NO_0X_HDR_CHRONO
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_CODECVT instead of EFL_EINA_BOOST_NO_0X_HDR_CODECVT
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_CODECVT) && !defined(EFL_EINA_BOOST_NO_0X_HDR_CODECVT)
#  define EFL_EINA_BOOST_NO_0X_HDR_CODECVT
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_CONDITION_VARIABLE instead of EFL_EINA_BOOST_NO_0X_HDR_CONDITION_VARIABLE
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_CONDITION_VARIABLE) && !defined(EFL_EINA_BOOST_NO_0X_HDR_CONDITION_VARIABLE)
#  define EFL_EINA_BOOST_NO_0X_HDR_CONDITION_VARIABLE
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_FORWARD_LIST instead of EFL_EINA_BOOST_NO_0X_HDR_FORWARD_LIST
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_FORWARD_LIST) && !defined(EFL_EINA_BOOST_NO_0X_HDR_FORWARD_LIST)
#  define EFL_EINA_BOOST_NO_0X_HDR_FORWARD_LIST
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_FUTURE instead of EFL_EINA_BOOST_NO_0X_HDR_FUTURE
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_FUTURE) && !defined(EFL_EINA_BOOST_NO_0X_HDR_FUTURE)
#  define EFL_EINA_BOOST_NO_0X_HDR_FUTURE
#endif

//  Use EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
//  instead of EFL_EINA_BOOST_NO_0X_HDR_INITIALIZER_LIST or EFL_EINA_BOOST_NO_INITIALIZER_LISTS
#ifdef EFL_EINA_BOOST_NO_CXX11_HDR_INITIALIZER_LIST
# ifndef EFL_EINA_BOOST_NO_0X_HDR_INITIALIZER_LIST
#  define EFL_EINA_BOOST_NO_0X_HDR_INITIALIZER_LIST
# endif
# ifndef EFL_EINA_BOOST_NO_INITIALIZER_LISTS
#  define EFL_EINA_BOOST_NO_INITIALIZER_LISTS
# endif
#endif

//  Use EFL_EINA_BOOST_NO_CXX11_HDR_MUTEX instead of EFL_EINA_BOOST_NO_0X_HDR_MUTEX
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_MUTEX) && !defined(EFL_EINA_BOOST_NO_0X_HDR_MUTEX)
#  define EFL_EINA_BOOST_NO_0X_HDR_MUTEX
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_RANDOM instead of EFL_EINA_BOOST_NO_0X_HDR_RANDOM
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_RANDOM) && !defined(EFL_EINA_BOOST_NO_0X_HDR_RANDOM)
#  define EFL_EINA_BOOST_NO_0X_HDR_RANDOM
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_RATIO instead of EFL_EINA_BOOST_NO_0X_HDR_RATIO
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_RATIO) && !defined(EFL_EINA_BOOST_NO_0X_HDR_RATIO)
#  define EFL_EINA_BOOST_NO_0X_HDR_RATIO
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_REGEX instead of EFL_EINA_BOOST_NO_0X_HDR_REGEX
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_REGEX) && !defined(EFL_EINA_BOOST_NO_0X_HDR_REGEX)
#  define EFL_EINA_BOOST_NO_0X_HDR_REGEX
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_SYSTEM_ERROR instead of EFL_EINA_BOOST_NO_0X_HDR_SYSTEM_ERROR
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_SYSTEM_ERROR) && !defined(EFL_EINA_BOOST_NO_0X_HDR_SYSTEM_ERROR)
#  define EFL_EINA_BOOST_NO_0X_HDR_SYSTEM_ERROR
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_THREAD instead of EFL_EINA_BOOST_NO_0X_HDR_THREAD
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_THREAD) && !defined(EFL_EINA_BOOST_NO_0X_HDR_THREAD)
#  define EFL_EINA_BOOST_NO_0X_HDR_THREAD
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_TUPLE instead of EFL_EINA_BOOST_NO_0X_HDR_TUPLE
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_TUPLE) && !defined(EFL_EINA_BOOST_NO_0X_HDR_TUPLE)
#  define EFL_EINA_BOOST_NO_0X_HDR_TUPLE
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_TYPE_TRAITS instead of EFL_EINA_BOOST_NO_0X_HDR_TYPE_TRAITS
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_TYPE_TRAITS) && !defined(EFL_EINA_BOOST_NO_0X_HDR_TYPE_TRAITS)
#  define EFL_EINA_BOOST_NO_0X_HDR_TYPE_TRAITS
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_TYPEINDEX instead of EFL_EINA_BOOST_NO_0X_HDR_TYPEINDEX
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_TYPEINDEX) && !defined(EFL_EINA_BOOST_NO_0X_HDR_TYPEINDEX)
#  define EFL_EINA_BOOST_NO_0X_HDR_TYPEINDEX
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_MAP instead of EFL_EINA_BOOST_NO_0X_HDR_UNORDERED_MAP
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_MAP) && !defined(EFL_EINA_BOOST_NO_0X_HDR_UNORDERED_MAP)
#  define EFL_EINA_BOOST_NO_0X_HDR_UNORDERED_MAP
#endif
//  Use EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_SET instead of EFL_EINA_BOOST_NO_0X_HDR_UNORDERED_SET
#if defined(EFL_EINA_BOOST_NO_CXX11_HDR_UNORDERED_SET) && !defined(EFL_EINA_BOOST_NO_0X_HDR_UNORDERED_SET)
#  define EFL_EINA_BOOST_NO_0X_HDR_UNORDERED_SET
#endif

//  ------------------ End of deprecated macros for 1.50 ---------------------------

//  -------------------- Deprecated macros for 1.51 ---------------------------
//  These will go away in a future release

//  Use     EFL_EINA_BOOST_NO_CXX11_AUTO_DECLARATIONS instead of   EFL_EINA_BOOST_NO_AUTO_DECLARATIONS
#if defined(EFL_EINA_BOOST_NO_CXX11_AUTO_DECLARATIONS) && !defined(EFL_EINA_BOOST_NO_AUTO_DECLARATIONS)
#  define EFL_EINA_BOOST_NO_AUTO_DECLARATIONS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_AUTO_MULTIDECLARATIONS instead of   EFL_EINA_BOOST_NO_AUTO_MULTIDECLARATIONS
#if defined(EFL_EINA_BOOST_NO_CXX11_AUTO_MULTIDECLARATIONS) && !defined(EFL_EINA_BOOST_NO_AUTO_MULTIDECLARATIONS)
#  define EFL_EINA_BOOST_NO_AUTO_MULTIDECLARATIONS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_CHAR16_T instead of   EFL_EINA_BOOST_NO_CHAR16_T
#if defined(EFL_EINA_BOOST_NO_CXX11_CHAR16_T) && !defined(EFL_EINA_BOOST_NO_CHAR16_T)
#  define EFL_EINA_BOOST_NO_CHAR16_T
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_CHAR32_T instead of   EFL_EINA_BOOST_NO_CHAR32_T
#if defined(EFL_EINA_BOOST_NO_CXX11_CHAR32_T) && !defined(EFL_EINA_BOOST_NO_CHAR32_T)
#  define EFL_EINA_BOOST_NO_CHAR32_T
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_TEMPLATE_ALIASES instead of   EFL_EINA_BOOST_NO_TEMPLATE_ALIASES
#if defined(EFL_EINA_BOOST_NO_CXX11_TEMPLATE_ALIASES) && !defined(EFL_EINA_BOOST_NO_TEMPLATE_ALIASES)
#  define EFL_EINA_BOOST_NO_TEMPLATE_ALIASES
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_CONSTEXPR instead of   EFL_EINA_BOOST_NO_CONSTEXPR
#if defined(EFL_EINA_BOOST_NO_CXX11_CONSTEXPR) && !defined(EFL_EINA_BOOST_NO_CONSTEXPR)
#  define EFL_EINA_BOOST_NO_CONSTEXPR
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_DECLTYPE_N3276 instead of   EFL_EINA_BOOST_NO_DECLTYPE_N3276
#if defined(EFL_EINA_BOOST_NO_CXX11_DECLTYPE_N3276) && !defined(EFL_EINA_BOOST_NO_DECLTYPE_N3276)
#  define EFL_EINA_BOOST_NO_DECLTYPE_N3276
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_DECLTYPE instead of   EFL_EINA_BOOST_NO_DECLTYPE
#if defined(EFL_EINA_BOOST_NO_CXX11_DECLTYPE) && !defined(EFL_EINA_BOOST_NO_DECLTYPE)
#  define EFL_EINA_BOOST_NO_DECLTYPE
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_DEFAULTED_FUNCTIONS instead of   EFL_EINA_BOOST_NO_DEFAULTED_FUNCTIONS
#if defined(EFL_EINA_BOOST_NO_CXX11_DEFAULTED_FUNCTIONS) && !defined(EFL_EINA_BOOST_NO_DEFAULTED_FUNCTIONS)
#  define EFL_EINA_BOOST_NO_DEFAULTED_FUNCTIONS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS instead of   EFL_EINA_BOOST_NO_DELETED_FUNCTIONS
#if defined(EFL_EINA_BOOST_NO_CXX11_DELETED_FUNCTIONS) && !defined(EFL_EINA_BOOST_NO_DELETED_FUNCTIONS)
#  define EFL_EINA_BOOST_NO_DELETED_FUNCTIONS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS instead of   EFL_EINA_BOOST_NO_EXPLICIT_CONVERSION_OPERATORS
#if defined(EFL_EINA_BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS) && !defined(EFL_EINA_BOOST_NO_EXPLICIT_CONVERSION_OPERATORS)
#  define EFL_EINA_BOOST_NO_EXPLICIT_CONVERSION_OPERATORS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_EXTERN_TEMPLATE instead of   EFL_EINA_BOOST_NO_EXTERN_TEMPLATE
#if defined(EFL_EINA_BOOST_NO_CXX11_EXTERN_TEMPLATE) && !defined(EFL_EINA_BOOST_NO_EXTERN_TEMPLATE)
#  define EFL_EINA_BOOST_NO_EXTERN_TEMPLATE
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS instead of   EFL_EINA_BOOST_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
#if defined(EFL_EINA_BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS) && !defined(EFL_EINA_BOOST_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS)
#  define EFL_EINA_BOOST_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_LAMBDAS instead of   EFL_EINA_BOOST_NO_LAMBDAS
#if defined(EFL_EINA_BOOST_NO_CXX11_LAMBDAS) && !defined(EFL_EINA_BOOST_NO_LAMBDAS)
#  define EFL_EINA_BOOST_NO_LAMBDAS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS instead of   EFL_EINA_BOOST_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS
#if defined(EFL_EINA_BOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS) && !defined(EFL_EINA_BOOST_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS)
#  define EFL_EINA_BOOST_NO_LOCAL_CLASS_TEMPLATE_PARAMETERS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_NOEXCEPT instead of   EFL_EINA_BOOST_NO_NOEXCEPT
#if defined(EFL_EINA_BOOST_NO_CXX11_NOEXCEPT) && !defined(EFL_EINA_BOOST_NO_NOEXCEPT)
#  define EFL_EINA_BOOST_NO_NOEXCEPT
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_NULLPTR instead of   EFL_EINA_BOOST_NO_NULLPTR
#if defined(EFL_EINA_BOOST_NO_CXX11_NULLPTR) && !defined(EFL_EINA_BOOST_NO_NULLPTR)
#  define EFL_EINA_BOOST_NO_NULLPTR
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_RAW_LITERALS instead of   EFL_EINA_BOOST_NO_RAW_LITERALS
#if defined(EFL_EINA_BOOST_NO_CXX11_RAW_LITERALS) && !defined(EFL_EINA_BOOST_NO_RAW_LITERALS)
#  define EFL_EINA_BOOST_NO_RAW_LITERALS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES instead of   EFL_EINA_BOOST_NO_RVALUE_REFERENCES
#if defined(EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(EFL_EINA_BOOST_NO_RVALUE_REFERENCES)
#  define EFL_EINA_BOOST_NO_RVALUE_REFERENCES
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS instead of   EFL_EINA_BOOST_NO_SCOPED_ENUMS
#if defined(EFL_EINA_BOOST_NO_CXX11_SCOPED_ENUMS) && !defined(EFL_EINA_BOOST_NO_SCOPED_ENUMS)
#  define EFL_EINA_BOOST_NO_SCOPED_ENUMS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_STATIC_ASSERT instead of   EFL_EINA_BOOST_NO_STATIC_ASSERT
#if defined(EFL_EINA_BOOST_NO_CXX11_STATIC_ASSERT) && !defined(EFL_EINA_BOOST_NO_STATIC_ASSERT)
#  define EFL_EINA_BOOST_NO_STATIC_ASSERT
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_STD_UNORDERED instead of   EFL_EINA_BOOST_NO_STD_UNORDERED
#if defined(EFL_EINA_BOOST_NO_CXX11_STD_UNORDERED) && !defined(EFL_EINA_BOOST_NO_STD_UNORDERED)
#  define EFL_EINA_BOOST_NO_STD_UNORDERED
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_UNICODE_LITERALS instead of   EFL_EINA_BOOST_NO_UNICODE_LITERALS
#if defined(EFL_EINA_BOOST_NO_CXX11_UNICODE_LITERALS) && !defined(EFL_EINA_BOOST_NO_UNICODE_LITERALS)
#  define EFL_EINA_BOOST_NO_UNICODE_LITERALS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX instead of   EFL_EINA_BOOST_NO_UNIFIED_INITIALIZATION_SYNTAX
#if defined(EFL_EINA_BOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX) && !defined(EFL_EINA_BOOST_NO_UNIFIED_INITIALIZATION_SYNTAX)
#  define EFL_EINA_BOOST_NO_UNIFIED_INITIALIZATION_SYNTAX
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES instead of   EFL_EINA_BOOST_NO_VARIADIC_TEMPLATES
#if defined(EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(EFL_EINA_BOOST_NO_VARIADIC_TEMPLATES)
#  define EFL_EINA_BOOST_NO_VARIADIC_TEMPLATES
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_VARIADIC_MACROS instead of   EFL_EINA_BOOST_NO_VARIADIC_MACROS
#if defined(EFL_EINA_BOOST_NO_CXX11_VARIADIC_MACROS) && !defined(EFL_EINA_BOOST_NO_VARIADIC_MACROS)
#  define EFL_EINA_BOOST_NO_VARIADIC_MACROS
#endif
//  Use     EFL_EINA_BOOST_NO_CXX11_NUMERIC_LIMITS instead of   EFL_EINA_BOOST_NO_NUMERIC_LIMITS_LOWEST
#if defined(EFL_EINA_BOOST_NO_CXX11_NUMERIC_LIMITS) && !defined(EFL_EINA_BOOST_NO_NUMERIC_LIMITS_LOWEST)
#  define EFL_EINA_BOOST_NO_NUMERIC_LIMITS_LOWEST
#endif
//  ------------------ End of deprecated macros for 1.51 ---------------------------



//
// Helper macros EFL_EINA_BOOST_NOEXCEPT, EFL_EINA_BOOST_NOEXCEPT_IF, EFL_EINA_BOOST_NOEXCEPT_EXPR
// These aid the transition to C++11 while still supporting C++03 compilers
//
#ifdef EFL_EINA_BOOST_NO_CXX11_NOEXCEPT
#  define EFL_EINA_BOOST_NOEXCEPT
#  define EFL_EINA_BOOST_NOEXCEPT_OR_NOTHROW throw()
#  define EFL_EINA_BOOST_NOEXCEPT_IF(Predicate)
#  define EFL_EINA_BOOST_NOEXCEPT_EXPR(Expression) false
#else
#  define EFL_EINA_BOOST_NOEXCEPT noexcept
#  define EFL_EINA_BOOST_NOEXCEPT_OR_NOTHROW noexcept
#  define EFL_EINA_BOOST_NOEXCEPT_IF(Predicate) noexcept((Predicate))
#  define EFL_EINA_BOOST_NOEXCEPT_EXPR(Expression) noexcept((Expression))
#endif
//
// Helper macro EFL_EINA_BOOST_FALLTHROUGH
// Fallback definition of EFL_EINA_BOOST_FALLTHROUGH macro used to mark intended
// fall-through between case labels in a switch statement. We use a definition
// that requires a semicolon after it to avoid at least one type of misuse even
// on unsupported compilers.
//
#ifndef EFL_EINA_BOOST_FALLTHROUGH
#  define EFL_EINA_BOOST_FALLTHROUGH ((void)0)
#endif

//
// constexpr workarounds
//
#if defined(EFL_EINA_BOOST_NO_CXX11_CONSTEXPR)
#define EFL_EINA_BOOST_CONSTEXPR
#define EFL_EINA_BOOST_CONSTEXPR_OR_CONST const
#else
#define EFL_EINA_BOOST_CONSTEXPR constexpr
#define EFL_EINA_BOOST_CONSTEXPR_OR_CONST constexpr
#endif

#define EFL_EINA_BOOST_STATIC_CONSTEXPR  static EFL_EINA_BOOST_CONSTEXPR_OR_CONST

//
// Set EFL_EINA_BOOST_HAS_STATIC_ASSERT when EFL_EINA_BOOST_NO_CXX11_STATIC_ASSERT is not defined
//
#if !defined(EFL_EINA_BOOST_NO_CXX11_STATIC_ASSERT) && !defined(EFL_EINA_BOOST_HAS_STATIC_ASSERT)
#  define EFL_EINA_BOOST_HAS_STATIC_ASSERT
#endif

//
// Set EFL_EINA_BOOST_HAS_RVALUE_REFS when EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES is not defined
//
#if !defined(EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(EFL_EINA_BOOST_HAS_RVALUE_REFS)
#define EFL_EINA_BOOST_HAS_RVALUE_REFS
#endif

//
// Set EFL_EINA_BOOST_HAS_VARIADIC_TMPL when EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES is not defined
//
#if !defined(EFL_EINA_BOOST_NO_CXX11_VARIADIC_TEMPLATES) && !defined(EFL_EINA_BOOST_HAS_VARIADIC_TMPL)
#define EFL_EINA_BOOST_HAS_VARIADIC_TMPL
#endif


#endif
