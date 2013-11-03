
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TYPE_TRAITS_IS_FLOAT_HPP_INCLUDED
#define EFL_EINA_BOOST_TYPE_TRAITS_IS_FLOAT_HPP_INCLUDED

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

//* is a type T a floating-point type described in the standard (3.9.1p8)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_float,T,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_float,float,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_float,double,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_float,long double,true)

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TYPE_TRAITS_IS_FLOAT_HPP_INCLUDED
