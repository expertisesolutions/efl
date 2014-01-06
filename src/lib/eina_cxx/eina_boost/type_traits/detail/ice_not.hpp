//  (C) Copyright John Maddock and Steve Cleary 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_DETAIL_ICE_NOT_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_DETAIL_ICE_NOT_HPP_INCLUDED

#include <eina_boost/config.hpp>

namespace efl_eina_boost {
namespace type_traits {

template <bool b>
struct ice_not
{
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = true);
};

template <>
struct ice_not<true>
{
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
};

} // namespace type_traits
} // namespace efl_eina_boost

#endif // EFL_EINA_BOOST_TT_DETAIL_ICE_NOT_HPP_INCLUDED
