
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_HAS_TRIVIAL_DESTRUCTOR_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_HAS_TRIVIAL_DESTRUCTOR_HPP_INCLUDED

#include <eina_boost/type_traits/config.hpp>
#include <eina_boost/type_traits/intrinsics.hpp>
#include <eina_boost/type_traits/is_pod.hpp>
#include <eina_boost/type_traits/detail/ice_or.hpp>

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

namespace detail {

template <typename T>
struct has_trivial_dtor_impl
{
#ifdef EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = EFL_EINA_BOOST_HAS_TRIVIAL_DESTRUCTOR(T));
#else
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = ::efl_eina_boost::is_pod<T>::value);
#endif
};

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(has_trivial_destructor,T,::efl_eina_boost::detail::has_trivial_dtor_impl<T>::value)

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_trivial_destructor,void,false)
#ifndef EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_trivial_destructor,void const,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_trivial_destructor,void const volatile,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(has_trivial_destructor,void volatile,false)
#endif

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_HAS_TRIVIAL_DESTRUCTOR_HPP_INCLUDED
