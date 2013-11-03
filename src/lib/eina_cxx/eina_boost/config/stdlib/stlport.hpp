//  (C) Copyright John Maddock 2001 - 2002. 
//  (C) Copyright Darin Adler 2001. 
//  (C) Copyright Jens Maurer 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  STLPort standard library config:

#if !defined(__SGI_STL_PORT) && !defined(_STLPORT_VERSION)
#  include <cstddef>
#  if !defined(__SGI_STL_PORT) && !defined(_STLPORT_VERSION)
#      error "This is not STLPort!"
#  endif
#endif

// Apple doesn't seem to reliably defined a *unix* macro
#if !defined(CYGWIN) && (  defined(__unix__)  \
                        || defined(__unix)    \
                        || defined(unix)      \
                        || defined(__APPLE__) \
                        || defined(__APPLE)   \
                        || defined(APPLE))
#  include <unistd.h>
#endif

//
// __STL_STATIC_CONST_INIT_BUG implies EFL_EINA_BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
// for versions prior to 4.1(beta)
//
#if (defined(__STL_STATIC_CONST_INIT_BUG) || defined(_STLP_STATIC_CONST_INIT_BUG)) && (__SGI_STL_PORT <= 0x400)
#  define EFL_EINA_BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#endif

//
// If STLport thinks that there is no partial specialisation, then there is no
// std::iterator traits:
//
#if !(defined(_STLP_CLASS_PARTIAL_SPECIALIZATION) || defined(__STL_CLASS_PARTIAL_SPECIALIZATION))
#  define EFL_EINA_BOOST_NO_STD_ITERATOR_TRAITS
#endif

//
// No new style iostreams on GCC without STLport's iostreams enabled:
//
#if (defined(__GNUC__) && (__GNUC__ < 3)) && !(defined(__SGI_STL_OWN_IOSTREAMS) || defined(_STLP_OWN_IOSTREAMS))
#  define EFL_EINA_BOOST_NO_STRINGSTREAM
#endif

//
// No new iostreams implies no std::locale, and no std::stringstream:
//
#if defined(__STL_NO_IOSTREAMS) || defined(__STL_NO_NEW_IOSTREAMS) || defined(_STLP_NO_IOSTREAMS) || defined(_STLP_NO_NEW_IOSTREAMS)
#  define EFL_EINA_BOOST_NO_STD_LOCALE
#  define EFL_EINA_BOOST_NO_STRINGSTREAM
#endif

//
// If the streams are not native, and we have a "using ::x" compiler bug
// then the io stream facets are not available in namespace std::
//
#ifdef _STLPORT_VERSION
#  if !(_STLPORT_VERSION >= 0x500) && !defined(_STLP_OWN_IOSTREAMS) && defined(_STLP_USE_NAMESPACES) && defined(EFL_EINA_BOOST_NO_USING_TEMPLATE) && !defined(__BORLANDC__)
#     define EFL_EINA_BOOST_NO_STD_LOCALE
#  endif
#else
#  if !defined(__SGI_STL_OWN_IOSTREAMS) && defined(__STL_USE_NAMESPACES) && defined(EFL_EINA_BOOST_NO_USING_TEMPLATE) && !defined(__BORLANDC__)
#     define EFL_EINA_BOOST_NO_STD_LOCALE
#  endif
#endif

#if defined(_STLPORT_VERSION) && (_STLPORT_VERSION >= 0x520)
#  define EFL_EINA_BOOST_HAS_TR1_UNORDERED_SET
#  define EFL_EINA_BOOST_HAS_TR1_UNORDERED_MAP
#endif
//
// Without member template support enabled, their are no template
// iterate constructors, and no std::allocator:
//
#if !(defined(__STL_MEMBER_TEMPLATES) || defined(_STLP_MEMBER_TEMPLATES))
#  define EFL_EINA_BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#  define EFL_EINA_BOOST_NO_STD_ALLOCATOR
#endif
//
// however we always have at least a partial allocator:
//
#define EFL_EINA_BOOST_HAS_PARTIAL_STD_ALLOCATOR

#if !defined(_STLP_MEMBER_TEMPLATE_CLASSES) || defined(_STLP_DONT_SUPPORT_REBIND_MEMBER_TEMPLATE)
#  define EFL_EINA_BOOST_NO_STD_ALLOCATOR
#endif

#if defined(_STLP_NO_MEMBER_TEMPLATE_KEYWORD) && defined(EFL_EINA_BOOST_MSVC) && (EFL_EINA_BOOST_MSVC <= 1300)
#  define EFL_EINA_BOOST_NO_STD_ALLOCATOR
#endif

//
// If STLport thinks there is no wchar_t at all, then we have to disable
// the support for the relevant specilazations of std:: templates.
//
#if !defined(_STLP_HAS_WCHAR_T) && !defined(_STLP_WCHAR_T_IS_USHORT)
#  ifndef  EFL_EINA_BOOST_NO_STD_WSTRING
#     define EFL_EINA_BOOST_NO_STD_WSTRING
#  endif
#  ifndef  EFL_EINA_BOOST_NO_STD_WSTREAMBUF
#     define EFL_EINA_BOOST_NO_STD_WSTREAMBUF
#  endif
#endif

//
// We always have SGI style hash_set, hash_map, and slist:
//
#ifndef _STLP_NO_EXTENSIONS
#define EFL_EINA_BOOST_HAS_HASH
#define EFL_EINA_BOOST_HAS_SLIST
#endif

//
// STLport does a good job of importing names into namespace std::,
// but doesn't always get them all, define EFL_EINA_BOOST_NO_STDC_NAMESPACE, since our
// workaround does not conflict with STLports:
//
//
// Harold Howe says:
// Borland switched to STLport in BCB6. Defining EFL_EINA_BOOST_NO_STDC_NAMESPACE with
// BCB6 does cause problems. If we detect C++ Builder, then don't define 
// EFL_EINA_BOOST_NO_STDC_NAMESPACE
//
#if !defined(__BORLANDC__) && !defined(__DMC__)
//
// If STLport is using it's own namespace, and the real names are in
// the global namespace, then we duplicate STLport's using declarations
// (by defining EFL_EINA_BOOST_NO_STDC_NAMESPACE), we do this because STLport doesn't
// necessarily import all the names we need into namespace std::
// 
#  if (defined(__STL_IMPORT_VENDOR_CSTD) \
         || defined(__STL_USE_OWN_NAMESPACE) \
         || defined(_STLP_IMPORT_VENDOR_CSTD) \
         || defined(_STLP_USE_OWN_NAMESPACE)) \
      && (defined(__STL_VENDOR_GLOBAL_CSTD) || defined (_STLP_VENDOR_GLOBAL_CSTD))
#     define EFL_EINA_BOOST_NO_STDC_NAMESPACE
#     define EFL_EINA_BOOST_NO_EXCEPTION_STD_NAMESPACE
#  endif
#elif defined(__BORLANDC__) && __BORLANDC__ < 0x560
// STLport doesn't import std::abs correctly:
#include <stdlib.h>
namespace std { using ::abs; }
// and strcmp/strcpy don't get imported either ('cos they are macros)
#include <string.h>
#ifdef strcpy
#  undef strcpy
#endif
#ifdef strcmp
#  undef strcmp
#endif
#ifdef _STLP_VENDOR_CSTD
namespace std{ using _STLP_VENDOR_CSTD::strcmp; using _STLP_VENDOR_CSTD::strcpy; }
#endif
#endif

//
// std::use_facet may be non-standard, uses a class instead:
//
#if defined(__STL_NO_EXPLICIT_FUNCTION_TMPL_ARGS) || defined(_STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS)
#  define EFL_EINA_BOOST_NO_STD_USE_FACET
#  define EFL_EINA_BOOST_HAS_STLP_USE_FACET
#endif

//
// If STLport thinks there are no wide functions, <cwchar> etc. is not working; but
// only if EFL_EINA_BOOST_NO_STDC_NAMESPACE is not defined (if it is then we do the import 
// into std:: ourselves).
//
#if defined(_STLP_NO_NATIVE_WIDE_FUNCTIONS) && !defined(EFL_EINA_BOOST_NO_STDC_NAMESPACE)
#  define EFL_EINA_BOOST_NO_CWCHAR
#  define EFL_EINA_BOOST_NO_CWCTYPE
#endif

//
// If STLport for some reason was configured so that it thinks that wchar_t
// is not an intrinsic type, then we have to disable the support for it as
// well (we would be missing required specializations otherwise).
//
#if !defined( _STLP_HAS_WCHAR_T) || defined(_STLP_WCHAR_T_IS_USHORT)
#  undef  EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T
#  define EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T
#endif

//
// Borland ships a version of STLport with C++ Builder 6 that lacks
// hashtables and the like:
//
#if defined(__BORLANDC__) && (__BORLANDC__ == 0x560)
#  undef EFL_EINA_BOOST_HAS_HASH
#endif

//
// gcc-2.95.3/STLPort does not like the using declarations we use to get ADL with std::min/max
//
#if defined(__GNUC__) && (__GNUC__ < 3)
#  include <algorithm> // for std::min and std::max
#  define EFL_EINA_BOOST_USING_STD_MIN() ((void)0)
#  define EFL_EINA_BOOST_USING_STD_MAX() ((void)0)
namespace efl_eina_boost { using std::min; using std::max; }
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

#define EFL_EINA_BOOST_STDLIB "STLPort standard library version " EFL_EINA_BOOST_STRINGIZE(__SGI_STL_PORT)








