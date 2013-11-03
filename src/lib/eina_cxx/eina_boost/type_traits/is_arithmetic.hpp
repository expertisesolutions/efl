
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_IS_ARITHMETIC_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_ARITHMETIC_HPP_INCLUDED

#if !defined( __CODEGEARC__ )
#include <eina_boost/type_traits/is_integral.hpp>
#include <eina_boost/type_traits/is_float.hpp>
#include <eina_boost/type_traits/detail/ice_or.hpp>
#include <eina_boost/config.hpp>
#endif

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

#if !defined(__CODEGEARC__)
namespace detail {

template< typename T >
struct is_arithmetic_impl
{ 
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = 
        (::efl_eina_boost::type_traits::ice_or< 
            ::efl_eina_boost::is_integral<T>::value,
            ::efl_eina_boost::is_float<T>::value
        >::value)); 
};

} // namespace detail
#endif

//* is a type T an arithmetic type described in the standard (3.9.1p8)
#if defined(__CODEGEARC__)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_arithmetic,T,__is_arithmetic(T))
#else
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_arithmetic,T,::efl_eina_boost::detail::is_arithmetic_impl<T>::value)
#endif

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_ARITHMETIC_HPP_INCLUDED
