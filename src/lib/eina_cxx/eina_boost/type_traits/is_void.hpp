
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_IS_VOID_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_VOID_HPP_INCLUDED

#include <eina_boost/config.hpp>

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

//* is a type T void - is_void<T>
#if defined( __CODEGEARC__ )
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_void,T,__is_void(T))
#else
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_void,T,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_void,void,true)

#ifndef EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_void,void const,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_void,void volatile,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_void,void const volatile,true)
#endif

#endif  // non-CodeGear implementation

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_VOID_HPP_INCLUDED
