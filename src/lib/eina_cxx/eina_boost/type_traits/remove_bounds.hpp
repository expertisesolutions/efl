
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_REMOVE_BOUNDS_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_REMOVE_BOUNDS_HPP_INCLUDED

#include <eina_boost/config.hpp>
#include <cstddef>
#include <eina_boost/detail/workaround.hpp>

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC,<=1300)
#include <eina_boost/type_traits/msvc/remove_bounds.hpp>
#endif

// should be the last #include
#include <eina_boost/type_traits/detail/type_trait_def.hpp>

#if !EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC,<=1300)

namespace efl_eina_boost {

EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_DEF1(remove_bounds,T,T)

#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(EFL_EINA_BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_bounds,T[N],T type)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_bounds,T const[N],T const type)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_bounds,T volatile[N],T volatile type)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_bounds,T const volatile[N],T const volatile type)
#if !EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610)) && !defined(__IBMCPP__) &&  !EFL_EINA_BOOST_WORKAROUND(__DMC__, EFL_EINA_BOOST_TESTED_AT(0x840))
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,remove_bounds,T[],T)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,remove_bounds,T const[],T const)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,remove_bounds,T volatile[],T volatile)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,remove_bounds,T const volatile[],T const volatile)
#endif
#endif

} // namespace efl_eina_boost

#endif

#include <eina_boost/type_traits/detail/type_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_REMOVE_BOUNDS_HPP_INCLUDED
