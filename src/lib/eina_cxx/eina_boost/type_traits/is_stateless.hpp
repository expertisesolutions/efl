
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_IS_STATELESS_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_STATELESS_HPP_INCLUDED

#include <eina_boost/type_traits/has_trivial_constructor.hpp>
#include <eina_boost/type_traits/has_trivial_copy.hpp>
#include <eina_boost/type_traits/has_trivial_destructor.hpp>
#include <eina_boost/type_traits/is_class.hpp>
#include <eina_boost/type_traits/is_empty.hpp>
#include <eina_boost/type_traits/detail/ice_and.hpp>
#include <eina_boost/config.hpp>

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

namespace detail {

template <typename T>
struct is_stateless_impl
{
  EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = 
    (::efl_eina_boost::type_traits::ice_and<
       ::efl_eina_boost::has_trivial_constructor<T>::value,
       ::efl_eina_boost::has_trivial_copy<T>::value,
       ::efl_eina_boost::has_trivial_destructor<T>::value,
       ::efl_eina_boost::is_class<T>::value,
       ::efl_eina_boost::is_empty<T>::value
     >::value));
};

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_stateless,T,::efl_eina_boost::detail::is_stateless_impl<T>::value)

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_STATELESS_HPP_INCLUDED
