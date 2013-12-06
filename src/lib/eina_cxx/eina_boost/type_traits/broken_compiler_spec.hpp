
//  Copyright 2001-2003 Aleksey Gurtovoy.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC_HPP_INCLUDED

#include <eina_boost/mpl/aux_/lambda_support.hpp>
#include <eina_boost/config.hpp>

// these are needed regardless of EFL_EINA_BOOST_TT_NO_BROKEN_COMPILER_SPEC 
#if defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
namespace efl_eina_boost { namespace detail {
template< typename T > struct remove_const_impl     { typedef T type; };
template< typename T > struct remove_volatile_impl  { typedef T type; };
template< typename T > struct remove_pointer_impl   { typedef T type; };
template< typename T > struct remove_reference_impl { typedef T type; };
typedef int invoke_EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC_outside_all_namespaces;
}}
#endif

// agurt, 27/jun/03: disable the workaround if user defined 
// EFL_EINA_BOOST_TT_NO_BROKEN_COMPILER_SPEC
#if    !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
    || defined(EFL_EINA_BOOST_TT_NO_BROKEN_COMPILER_SPEC)

#   define EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(T) /**/

#else

// same as EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_IMPL_SPEC1 macro, except that it
// never gets #undef-ined
#   define EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(trait,spec,result) \
template<> struct trait##_impl<spec> \
{ \
    typedef result type; \
}; \
/**/

#   define EFL_EINA_BOOST_TT_AUX_REMOVE_CONST_VOLATILE_RANK1_SPEC(T)                         \
    EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(remove_const,T const,T)                    \
    EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(remove_const,T const volatile,T volatile)  \
    EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(remove_volatile,T volatile,T)              \
    EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(remove_volatile,T const volatile,T const)  \
    /**/

#   define EFL_EINA_BOOST_TT_AUX_REMOVE_PTR_REF_RANK_1_SPEC(T)                               \
    EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(remove_pointer,T*,T)                       \
    EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(remove_pointer,T*const,T)                  \
    EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(remove_pointer,T*volatile,T)               \
    EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(remove_pointer,T*const volatile,T)         \
    EFL_EINA_BOOST_TT_AUX_BROKEN_TYPE_TRAIT_SPEC1(remove_reference,T&,T)                     \
    /**/

#   define EFL_EINA_BOOST_TT_AUX_REMOVE_PTR_REF_RANK_2_SPEC(T)                               \
    EFL_EINA_BOOST_TT_AUX_REMOVE_PTR_REF_RANK_1_SPEC(T)                                      \
    EFL_EINA_BOOST_TT_AUX_REMOVE_PTR_REF_RANK_1_SPEC(T const)                                \
    EFL_EINA_BOOST_TT_AUX_REMOVE_PTR_REF_RANK_1_SPEC(T volatile)                             \
    EFL_EINA_BOOST_TT_AUX_REMOVE_PTR_REF_RANK_1_SPEC(T const volatile)                       \
    /**/

#   define EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_1_SPEC(T)                                   \
    EFL_EINA_BOOST_TT_AUX_REMOVE_PTR_REF_RANK_2_SPEC(T)                                      \
    EFL_EINA_BOOST_TT_AUX_REMOVE_CONST_VOLATILE_RANK1_SPEC(T)                                \
    /**/

#   define EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_2_SPEC(T)                                   \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_1_SPEC(T*)                                         \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_1_SPEC(T const*)                                   \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_1_SPEC(T volatile*)                                \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_1_SPEC(T const volatile*)                          \
    /**/

#   define EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(T)                                         \
    namespace efl_eina_boost { namespace detail {                                            \
    typedef invoke_EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC_outside_all_namespaces             \
      please_invoke_EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC_outside_all_namespaces;           \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_1_SPEC(T)                                          \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_2_SPEC(T)                                          \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_2_SPEC(T*)                                         \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_2_SPEC(T const*)                                   \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_2_SPEC(T volatile*)                                \
    EFL_EINA_BOOST_TT_AUX_REMOVE_ALL_RANK_2_SPEC(T const volatile*)                          \
    }}                                                                              \
    /**/

#   include <eina_boost/type_traits/detail/type_trait_undef.hpp>

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(bool)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(char)
#ifndef EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(wchar_t)
#endif
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(signed char)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(unsigned char)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(signed short)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(unsigned short)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(signed int)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(unsigned int)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(signed long)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(unsigned long)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(float)
EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(double)
//EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(long double)

// for backward compatibility
#define EFL_EINA_BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION(T) \
    EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC(T) \
/**/

#endif // EFL_EINA_BOOST_TT_BROKEN_COMPILER_SPEC_HPP_INCLUDED
