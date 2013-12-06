
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef EFL_EINA_BOOST_TT_IS_UNION_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_UNION_HPP_INCLUDED

#include <eina_boost/type_traits/remove_cv.hpp>
#include <eina_boost/type_traits/config.hpp>
#include <eina_boost/type_traits/intrinsics.hpp>

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

namespace detail {
#ifndef __GNUC__
template <typename T> struct is_union_impl
{
   typedef typename remove_cv<T>::type cvt;
#ifdef EFL_EINA_BOOST_IS_UNION
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = EFL_EINA_BOOST_IS_UNION(cvt));
#else
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
#endif
};
#else
//
// using remove_cv here generates a whole load of needless
// warnings with gcc, since it doesn't do any good with gcc
// in any case (at least at present), just remove it:
//
template <typename T> struct is_union_impl
{
#ifdef EFL_EINA_BOOST_IS_UNION
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = EFL_EINA_BOOST_IS_UNION(T));
#else
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
#endif
};
#endif
} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_union,T,::efl_eina_boost::detail::is_union_impl<T>::value)

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_UNION_HPP_INCLUDED
