
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


// Some fixes for is_array are based on a newsgroup posting by Jonathan Lundquist.


#ifndef EFL_EINA_BOOST_TT_IS_ARRAY_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_ARRAY_HPP_INCLUDED

#include <eina_boost/type_traits/config.hpp>

#ifdef EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#   include <eina_boost/type_traits/detail/yes_no_type.hpp>
#   include <eina_boost/type_traits/detail/wrap.hpp>
#endif

#include <cstddef>

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

#if defined( __CODEGEARC__ )
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_array,T,__is_array(T))
#elif !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_array,T,false)
#if !defined(EFL_EINA_BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,is_array,T[N],true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,is_array,T const[N],true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,is_array,T volatile[N],true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,is_array,T const volatile[N],true)
#if !EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, < 0x600) && !defined(__IBMCPP__) &&  !EFL_EINA_BOOST_WORKAROUND(__DMC__, EFL_EINA_BOOST_TESTED_AT(0x840))
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_array,T[],true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_array,T const[],true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_array,T volatile[],true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_array,T const volatile[],true)
#endif
#endif

#else // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail {

using ::efl_eina_boost::type_traits::yes_type;
using ::efl_eina_boost::type_traits::no_type;
using ::efl_eina_boost::type_traits::wrap;

template< typename T > T(* is_array_tester1(wrap<T>) )(wrap<T>);
char EFL_EINA_BOOST_TT_DECL is_array_tester1(...);

template< typename T> no_type is_array_tester2(T(*)(wrap<T>));
yes_type EFL_EINA_BOOST_TT_DECL is_array_tester2(...);

template< typename T >
struct is_array_impl
{ 
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = 
        sizeof(::efl_eina_boost::detail::is_array_tester2(
            ::efl_eina_boost::detail::is_array_tester1(
                ::efl_eina_boost::type_traits::wrap<T>()
                )
        )) == 1
    );
};

#ifndef EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_array,void,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_array,void const,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_array,void volatile,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_array,void const volatile,false)
#endif

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_array,T,::efl_eina_boost::detail::is_array_impl<T>::value)

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_ARRAY_HPP_INCLUDED
