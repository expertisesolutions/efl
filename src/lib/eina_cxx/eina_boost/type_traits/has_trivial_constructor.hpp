
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_HAS_TRIVIAL_CONSTRUCTOR_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_HAS_TRIVIAL_CONSTRUCTOR_HPP_INCLUDED

#include <eina_boost/type_traits/config.hpp>
#include <eina_boost/type_traits/intrinsics.hpp>
#include <eina_boost/type_traits/is_pod.hpp>
#include <eina_boost/type_traits/detail/ice_or.hpp>

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

namespace detail {

template <typename T>
struct has_trivial_ctor_impl
{
#ifdef EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value =
      (::efl_eina_boost::type_traits::ice_or<
         ::efl_eina_boost::is_pod<T>::value,
         EFL_EINA_BOOST_HAS_TRIVIAL_CONSTRUCTOR(T)
      >::value));
#else
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value =
      (::efl_eina_boost::type_traits::ice_or<
         ::efl_eina_boost::is_pod<T>::value,
         false
      >::value));
#endif
};

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(has_trivial_constructor,T,::efl_eina_boost::detail::has_trivial_ctor_impl<T>::value)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(has_trivial_default_constructor,T,::efl_eina_boost::detail::has_trivial_ctor_impl<T>::value)

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_HAS_TRIVIAL_CONSTRUCTOR_HPP_INCLUDED
