
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_IS_SCALAR_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_SCALAR_HPP_INCLUDED

#include <eina_boost/type_traits/is_arithmetic.hpp>
#include <eina_boost/type_traits/is_enum.hpp>
#include <eina_boost/type_traits/is_pointer.hpp>
#include <eina_boost/type_traits/is_member_pointer.hpp>
#include <eina_boost/type_traits/detail/ice_or.hpp>
#include <eina_boost/config.hpp>

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

namespace detail {

template <typename T>
struct is_scalar_impl
{ 
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value =
      (::efl_eina_boost::type_traits::ice_or<
         ::efl_eina_boost::is_arithmetic<T>::value,
         ::efl_eina_boost::is_enum<T>::value,
         ::efl_eina_boost::is_pointer<T>::value,
         ::efl_eina_boost::is_member_pointer<T>::value
      >::value));
};

// these specializations are only really needed for compilers 
// without partial specialization support:
template <> struct is_scalar_impl<void>{ EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false ); };
#ifndef EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
template <> struct is_scalar_impl<void const>{ EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false ); };
template <> struct is_scalar_impl<void volatile>{ EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false ); };
template <> struct is_scalar_impl<void const volatile>{ EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false ); };
#endif

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_scalar,T,::efl_eina_boost::detail::is_scalar_impl<T>::value)

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_SCALAR_HPP_INCLUDED
