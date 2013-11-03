//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_INTRINSICS_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_INTRINSICS_HPP_INCLUDED

#ifndef EFL_EINA_BOOST_TT_CONFIG_HPP_INCLUDED
#include <eina_boost/type_traits/config.hpp>
#endif

//
// Helper macros for builtin compiler support.
// If your compiler has builtin support for any of the following
// traits concepts, then redefine the appropriate macros to pick
// up on the compiler support:
//
// (these should largely ignore cv-qualifiers)
// EFL_EINA_BOOST_IS_UNION(T) should evaluate to true if T is a union type
// EFL_EINA_BOOST_IS_POD(T) should evaluate to true if T is a POD type
// EFL_EINA_BOOST_IS_EMPTY(T) should evaluate to true if T is an empty class type (and not a union)
// EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T) should evaluate to true if "T x;" has no effect
// EFL_EINA_BOOST_HAS_TRIVIAL_COPY(T) should evaluate to true if T(t) <==> memcpy
// EFL_EINA_BOOST_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T) should evaluate to true if T(efl_eina_boost::move(t)) <==> memcpy
// EFL_EINA_BOOST_HAS_TRIVIAL_ASSIGN(T) should evaluate to true if t = u <==> memcpy
// EFL_EINA_BOOST_HAS_TRIVIAL_MOVE_ASSIGN(T) should evaluate to true if t = efl_eina_boost::move(u) <==> memcpy
// EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T) should evaluate to true if ~T() has no effect
// EFL_EINA_BOOST_HAS_NOTHROW_CONSTRUCTOR(T) should evaluate to true if "T x;" can not throw
// EFL_EINA_BOOST_HAS_NOTHROW_COPY(T) should evaluate to true if T(t) can not throw
// EFL_EINA_BOOST_HAS_NOTHROW_ASSIGN(T) should evaluate to true if t = u can not throw
// EFL_EINA_BOOST_HAS_VIRTUAL_DESTRUCTOR(T) should evaluate to true T has a virtual destructor
//
// The following can also be defined: when detected our implementation is greatly simplified.
//
// EFL_EINA_BOOST_IS_ABSTRACT(T) true if T is an abstract type
// EFL_EINA_BOOST_IS_BASE_OF(T,U) true if T is a base class of U
// EFL_EINA_BOOST_IS_CLASS(T) true if T is a class type (and not a union)
// EFL_EINA_BOOST_IS_CONVERTIBLE(T,U) true if T is convertible to U
// EFL_EINA_BOOST_IS_ENUM(T) true is T is an enum
// EFL_EINA_BOOST_IS_POLYMORPHIC(T) true if T is a polymorphic type
// EFL_EINA_BOOST_ALIGNMENT_OF(T) should evaluate to the alignment requirements of type T.

#ifdef EFL_EINA_BOOST_HAS_SGI_TYPE_TRAITS
    // Hook into SGI's __type_traits class, this will pick up user supplied
    // specializations as well as SGI - compiler supplied specializations.
#   include <eina_boost/type_traits/is_same.hpp>
#   ifdef __NetBSD__
      // There are two different versions of type_traits.h on NetBSD on Spark
      // use an implicit include via algorithm instead, to make sure we get
      // the same version as the std lib:
#     include <algorithm>
#   else
#    include <type_traits.h>
#   endif
#   define EFL_EINA_BOOST_IS_POD(T) ::efl_eina_boost::is_same< typename ::__type_traits<T>::is_POD_type, ::__true_type>::value
#   define EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T) ::efl_eina_boost::is_same< typename ::__type_traits<T>::has_trivial_default_constructor, ::__true_type>::value
#   define EFL_EINA_BOOST_HAS_TRIVIAL_COPY(T) ::efl_eina_boost::is_same< typename ::__type_traits<T>::has_trivial_copy_constructor, ::__true_type>::value
#   define EFL_EINA_BOOST_HAS_TRIVIAL_ASSIGN(T) ::efl_eina_boost::is_same< typename ::__type_traits<T>::has_trivial_assignment_operator, ::__true_type>::value
#   define EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T) ::efl_eina_boost::is_same< typename ::__type_traits<T>::has_trivial_destructor, ::__true_type>::value

#   ifdef __sgi
#      define EFL_EINA_BOOST_HAS_TYPE_TRAITS_INTRINSICS
#   endif
#endif

#if defined(__MSL_CPP__) && (__MSL_CPP__ >= 0x8000)
    // Metrowerks compiler is acquiring intrinsic type traits support
    // post version 8.  We hook into the published interface to pick up
    // user defined specializations as well as compiler intrinsics as 
    // and when they become available:
#   include <msl_utility>
#   define EFL_EINA_BOOST_IS_UNION(T) EFL_EINA_BOOST_STD_EXTENSION_NAMESPACE::is_union<T>::value
#   define EFL_EINA_BOOST_IS_POD(T) EFL_EINA_BOOST_STD_EXTENSION_NAMESPACE::is_POD<T>::value
#   define EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T) EFL_EINA_BOOST_STD_EXTENSION_NAMESPACE::has_trivial_default_ctor<T>::value
#   define EFL_EINA_BOOST_HAS_TRIVIAL_COPY(T) EFL_EINA_BOOST_STD_EXTENSION_NAMESPACE::has_trivial_copy_ctor<T>::value
#   define EFL_EINA_BOOST_HAS_TRIVIAL_ASSIGN(T) EFL_EINA_BOOST_STD_EXTENSION_NAMESPACE::has_trivial_assignment<T>::value
#   define EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T) EFL_EINA_BOOST_STD_EXTENSION_NAMESPACE::has_trivial_dtor<T>::value
#   define EFL_EINA_BOOST_HAS_TYPE_TRAITS_INTRINSICS
#endif

#if (defined(EFL_EINA_BOOST_MSVC) && defined(EFL_EINA_BOOST_MSVC_FULL_VER) && (EFL_EINA_BOOST_MSVC_FULL_VER >=140050215))\
         || (defined(EFL_EINA_BOOST_INTEL) && defined(_MSC_VER) && (_MSC_VER >= 1500))
#   include <eina_boost/type_traits/is_same.hpp>
#   include <eina_boost/type_traits/is_function.hpp>

#   define EFL_EINA_BOOST_IS_UNION(T) __is_union(T)
#   define EFL_EINA_BOOST_IS_POD(T) (__is_pod(T) && __has_trivial_constructor(T))
#   define EFL_EINA_BOOST_IS_EMPTY(T) __is_empty(T)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_COPY(T) (__has_trivial_copy(T)|| ( ::efl_eina_boost::is_pod<T>::value && !::efl_eina_boost::is_volatile<T>::value))
#   define EFL_EINA_BOOST_HAS_TRIVIAL_ASSIGN(T) (__has_trivial_assign(T) || ( ::efl_eina_boost::is_pod<T>::value && ! ::efl_eina_boost::is_const<T>::value && !::efl_eina_boost::is_volatile<T>::value))
#   define EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T) (__has_trivial_destructor(T) || ::efl_eina_boost::is_pod<T>::value)
#   define EFL_EINA_BOOST_HAS_NOTHROW_CONSTRUCTOR(T) (__has_nothrow_constructor(T) || ::efl_eina_boost::has_trivial_constructor<T>::value)
#   define EFL_EINA_BOOST_HAS_NOTHROW_COPY(T) (__has_nothrow_copy(T) || ::efl_eina_boost::has_trivial_copy<T>::value)
#   define EFL_EINA_BOOST_HAS_NOTHROW_ASSIGN(T) (__has_nothrow_assign(T) || ::efl_eina_boost::has_trivial_assign<T>::value)
#   define EFL_EINA_BOOST_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)

#   define EFL_EINA_BOOST_IS_ABSTRACT(T) __is_abstract(T)
#   define EFL_EINA_BOOST_IS_BASE_OF(T,U) (__is_base_of(T,U) && !is_same<T,U>::value)
#   define EFL_EINA_BOOST_IS_CLASS(T) __is_class(T)
#   define EFL_EINA_BOOST_IS_CONVERTIBLE(T,U) ((__is_convertible_to(T,U) || (is_same<T,U>::value && !is_function<U>::value)) && !__is_abstract(U))
#   define EFL_EINA_BOOST_IS_ENUM(T) __is_enum(T)
//  This one doesn't quite always do the right thing:
//  #   define EFL_EINA_BOOST_IS_POLYMORPHIC(T) __is_polymorphic(T)
//  This one fails if the default alignment has been changed with /Zp:
//  #   define EFL_EINA_BOOST_ALIGNMENT_OF(T) __alignof(T)

#   if defined(_MSC_VER) && (_MSC_VER >= 1700)
#       define EFL_EINA_BOOST_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T) ((__has_trivial_move_constructor(T) || ::efl_eina_boost::is_pod<T>::value) && !::efl_eina_boost::is_volatile<T>::value)
#       define EFL_EINA_BOOST_HAS_TRIVIAL_MOVE_ASSIGN(T) ((__has_trivial_move_assign(T) || ::efl_eina_boost::is_pod<T>::value) && ! ::efl_eina_boost::is_const<T>::value && !::efl_eina_boost::is_volatile<T>::value)
#   endif

#   define EFL_EINA_BOOST_HAS_TYPE_TRAITS_INTRINSICS
#endif

#if defined(__DMC__) && (__DMC__ >= 0x848)
// For Digital Mars C++, www.digitalmars.com
#   define EFL_EINA_BOOST_IS_UNION(T) (__typeinfo(T) & 0x400)
#   define EFL_EINA_BOOST_IS_POD(T) (__typeinfo(T) & 0x800)
#   define EFL_EINA_BOOST_IS_EMPTY(T) (__typeinfo(T) & 0x1000)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T) (__typeinfo(T) & 0x10)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_COPY(T) (__typeinfo(T) & 0x20)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_ASSIGN(T) (__typeinfo(T) & 0x40)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T) (__typeinfo(T) & 0x8)
#   define EFL_EINA_BOOST_HAS_NOTHROW_CONSTRUCTOR(T) (__typeinfo(T) & 0x80)
#   define EFL_EINA_BOOST_HAS_NOTHROW_COPY(T) (__typeinfo(T) & 0x100)
#   define EFL_EINA_BOOST_HAS_NOTHROW_ASSIGN(T) (__typeinfo(T) & 0x200)
#   define EFL_EINA_BOOST_HAS_VIRTUAL_DESTRUCTOR(T) (__typeinfo(T) & 0x4)
#   define EFL_EINA_BOOST_HAS_TYPE_TRAITS_INTRINSICS
#endif

#if defined(EFL_EINA_BOOST_CLANG) && defined(__has_feature)
#   include <cstddef>
#   include <eina_boost/type_traits/is_same.hpp>
#   include <eina_boost/type_traits/is_reference.hpp>
#   include <eina_boost/type_traits/is_volatile.hpp>

#   if __has_feature(is_union)
#     define EFL_EINA_BOOST_IS_UNION(T) __is_union(T)
#   endif
#   if (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20080306 && __GLIBCXX__ != 20080519)) && __has_feature(is_pod)
#     define EFL_EINA_BOOST_IS_POD(T) __is_pod(T)
#   endif
#   if (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20080306 && __GLIBCXX__ != 20080519)) && __has_feature(is_empty)
#     define EFL_EINA_BOOST_IS_EMPTY(T) __is_empty(T)
#   endif
#   if __has_feature(has_trivial_constructor)
#     define EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#   endif
#   if __has_feature(has_trivial_copy)
#     define EFL_EINA_BOOST_HAS_TRIVIAL_COPY(T) (__has_trivial_copy(T) && !is_reference<T>::value && !is_volatile<T>::value)
#   endif
#   if __has_feature(has_trivial_assign)
#     define EFL_EINA_BOOST_HAS_TRIVIAL_ASSIGN(T) (__has_trivial_assign(T) && !is_volatile<T>::value)
#   endif
#   if __has_feature(has_trivial_destructor)
#     define EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#   endif
#   if __has_feature(has_nothrow_constructor)
#     define EFL_EINA_BOOST_HAS_NOTHROW_CONSTRUCTOR(T) __has_nothrow_constructor(T)
#   endif
#   if __has_feature(has_nothrow_copy)
#     define EFL_EINA_BOOST_HAS_NOTHROW_COPY(T) (__has_nothrow_copy(T) && !is_volatile<T>::value && !is_reference<T>::value)
#   endif
#   if __has_feature(has_nothrow_assign)
#     define EFL_EINA_BOOST_HAS_NOTHROW_ASSIGN(T) (__has_nothrow_assign(T) && !is_volatile<T>::value)
#   endif
#   if __has_feature(has_virtual_destructor)
#     define EFL_EINA_BOOST_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)
#   endif
#   if __has_feature(is_abstract)
#     define EFL_EINA_BOOST_IS_ABSTRACT(T) __is_abstract(T)
#   endif
#   if __has_feature(is_base_of)
#     define EFL_EINA_BOOST_IS_BASE_OF(T,U) (__is_base_of(T,U) && !is_same<T,U>::value)
#   endif
#   if __has_feature(is_class)
#     define EFL_EINA_BOOST_IS_CLASS(T) __is_class(T)
#   endif
#   if __has_feature(is_convertible_to)
#     include <eina_boost/type_traits/is_abstract.hpp>
#     define EFL_EINA_BOOST_IS_CONVERTIBLE(T,U) (__is_convertible_to(T,U) && !::efl_eina_boost::is_abstract<U>::value)
#   endif
#   if __has_feature(is_enum)
#     define EFL_EINA_BOOST_IS_ENUM(T) __is_enum(T)
#   endif
#   if __has_feature(is_polymorphic)
#     define EFL_EINA_BOOST_IS_POLYMORPHIC(T) __is_polymorphic(T)
#   endif
#   if __has_feature(has_trivial_move_constructor)
#     define EFL_EINA_BOOST_HAS_TRIVIAL_MOVE_CONSTRUCTOR(T) __has_trivial_move_constructor(T)
#   endif
#   if __has_feature(has_trivial_move_assign)
#     define EFL_EINA_BOOST_HAS_TRIVIAL_MOVE_ASSIGN(T) __has_trivial_move_assign(T)
#   endif
#   define EFL_EINA_BOOST_ALIGNMENT_OF(T) __alignof(T)

#   define EFL_EINA_BOOST_HAS_TYPE_TRAITS_INTRINSICS
#endif

#if defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3) && !defined(__GCCXML__))) && !defined(EFL_EINA_BOOST_CLANG)
#   include <eina_boost/type_traits/is_same.hpp>
#   include <eina_boost/type_traits/is_reference.hpp>
#   include <eina_boost/type_traits/is_volatile.hpp>

#ifdef EFL_EINA_BOOST_INTEL
#  define EFL_EINA_BOOST_INTEL_TT_OPTS || is_pod<T>::value
#else
#  define EFL_EINA_BOOST_INTEL_TT_OPTS
#endif

#   define EFL_EINA_BOOST_IS_UNION(T) __is_union(T)
#   define EFL_EINA_BOOST_IS_POD(T) __is_pod(T)
#   define EFL_EINA_BOOST_IS_EMPTY(T) __is_empty(T)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T) ((__has_trivial_constructor(T) EFL_EINA_BOOST_INTEL_TT_OPTS) && ! ::efl_eina_boost::is_volatile<T>::value)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_COPY(T) ((__has_trivial_copy(T) EFL_EINA_BOOST_INTEL_TT_OPTS) && !is_reference<T>::value && ! ::efl_eina_boost::is_volatile<T>::value)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_ASSIGN(T) ((__has_trivial_assign(T) EFL_EINA_BOOST_INTEL_TT_OPTS) && ! ::efl_eina_boost::is_volatile<T>::value && ! ::efl_eina_boost::is_const<T>::value)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T) (__has_trivial_destructor(T) EFL_EINA_BOOST_INTEL_TT_OPTS)
#   define EFL_EINA_BOOST_HAS_NOTHROW_CONSTRUCTOR(T) (__has_nothrow_constructor(T) EFL_EINA_BOOST_INTEL_TT_OPTS)
#   define EFL_EINA_BOOST_HAS_NOTHROW_COPY(T) ((__has_nothrow_copy(T) EFL_EINA_BOOST_INTEL_TT_OPTS) && !is_volatile<T>::value && !is_reference<T>::value)
#   define EFL_EINA_BOOST_HAS_NOTHROW_ASSIGN(T) ((__has_nothrow_assign(T) EFL_EINA_BOOST_INTEL_TT_OPTS) && !is_volatile<T>::value && !is_const<T>::value)
#   define EFL_EINA_BOOST_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)

#   define EFL_EINA_BOOST_IS_ABSTRACT(T) __is_abstract(T)
#   define EFL_EINA_BOOST_IS_BASE_OF(T,U) (__is_base_of(T,U) && !is_same<T,U>::value)
#   define EFL_EINA_BOOST_IS_CLASS(T) __is_class(T)
#   define EFL_EINA_BOOST_IS_ENUM(T) __is_enum(T)
#   define EFL_EINA_BOOST_IS_POLYMORPHIC(T) __is_polymorphic(T)
#   if (!defined(unix) && !defined(__unix__)) || defined(__LP64__)
      // GCC sometimes lies about alignment requirements
      // of type double on 32-bit unix platforms, use the
      // old implementation instead in that case:
#     define EFL_EINA_BOOST_ALIGNMENT_OF(T) __alignof__(T)
#   endif

#   define EFL_EINA_BOOST_HAS_TYPE_TRAITS_INTRINSICS
#endif

#if defined(__ghs__) && (__GHS_VERSION_NUMBER >= 600)
#   include <eina_boost/type_traits/is_same.hpp>
#   include <eina_boost/type_traits/is_reference.hpp>
#   include <eina_boost/type_traits/is_volatile.hpp>

#   define EFL_EINA_BOOST_IS_UNION(T) __is_union(T)
#   define EFL_EINA_BOOST_IS_POD(T) __is_pod(T)
#   define EFL_EINA_BOOST_IS_EMPTY(T) __is_empty(T)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_COPY(T) (__has_trivial_copy(T) && !is_reference<T>::value && !is_volatile<T>::value)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_ASSIGN(T) (__has_trivial_assign(T) && !is_volatile<T>::value)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#   define EFL_EINA_BOOST_HAS_NOTHROW_CONSTRUCTOR(T) __has_nothrow_constructor(T)
#   define EFL_EINA_BOOST_HAS_NOTHROW_COPY(T) (__has_nothrow_copy(T) && !is_volatile<T>::value && !is_reference<T>::value)
#   define EFL_EINA_BOOST_HAS_NOTHROW_ASSIGN(T) (__has_nothrow_assign(T) && !is_volatile<T>::value)
#   define EFL_EINA_BOOST_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)

#   define EFL_EINA_BOOST_IS_ABSTRACT(T) __is_abstract(T)
#   define EFL_EINA_BOOST_IS_BASE_OF(T,U) (__is_base_of(T,U) && !is_same<T,U>::value)
#   define EFL_EINA_BOOST_IS_CLASS(T) __is_class(T)
#   define EFL_EINA_BOOST_IS_ENUM(T) __is_enum(T)
#   define EFL_EINA_BOOST_IS_POLYMORPHIC(T) __is_polymorphic(T)
#   define EFL_EINA_BOOST_ALIGNMENT_OF(T) __alignof__(T)

#   define EFL_EINA_BOOST_HAS_TYPE_TRAITS_INTRINSICS
#endif

# if defined(__CODEGEARC__)
#   include <eina_boost/type_traits/is_same.hpp>
#   include <eina_boost/type_traits/is_reference.hpp>
#   include <eina_boost/type_traits/is_volatile.hpp>
#   include <eina_boost/type_traits/is_void.hpp>

#   define EFL_EINA_BOOST_IS_UNION(T) __is_union(T)
#   define EFL_EINA_BOOST_IS_POD(T) __is_pod(T)
#   define EFL_EINA_BOOST_IS_EMPTY(T) __is_empty(T)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T) (__has_trivial_default_constructor(T))
#   define EFL_EINA_BOOST_HAS_TRIVIAL_COPY(T) (__has_trivial_copy_constructor(T) && !is_volatile<T>::value && !is_reference<T>::value)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_ASSIGN(T) (__has_trivial_assign(T) && !is_volatile<T>::value)
#   define EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T) (__has_trivial_destructor(T))
#   define EFL_EINA_BOOST_HAS_NOTHROW_CONSTRUCTOR(T) (__has_nothrow_default_constructor(T))
#   define EFL_EINA_BOOST_HAS_NOTHROW_COPY(T) (__has_nothrow_copy_constructor(T) && !is_volatile<T>::value && !is_reference<T>::value)
#   define EFL_EINA_BOOST_HAS_NOTHROW_ASSIGN(T) (__has_nothrow_assign(T) && !is_volatile<T>::value)
#   define EFL_EINA_BOOST_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)

#   define EFL_EINA_BOOST_IS_ABSTRACT(T) __is_abstract(T)
#   define EFL_EINA_BOOST_IS_BASE_OF(T,U) (__is_base_of(T,U) && !is_void<T>::value && !is_void<U>::value)
#   define EFL_EINA_BOOST_IS_CLASS(T) __is_class(T)
#   define EFL_EINA_BOOST_IS_CONVERTIBLE(T,U) (__is_convertible(T,U) || is_void<U>::value)
#   define EFL_EINA_BOOST_IS_ENUM(T) __is_enum(T)
#   define EFL_EINA_BOOST_IS_POLYMORPHIC(T) __is_polymorphic(T)
#   define EFL_EINA_BOOST_ALIGNMENT_OF(T) alignof(T)

#   define EFL_EINA_BOOST_HAS_TYPE_TRAITS_INTRINSICS
#endif

#endif // EFL_EINA_BOOST_TT_INTRINSICS_HPP_INCLUDED







