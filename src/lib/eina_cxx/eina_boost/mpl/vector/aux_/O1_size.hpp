
#ifndef EFL_EINA_BOOST_MPL_VECTOR_AUX_O1_SIZE_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_VECTOR_AUX_O1_SIZE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: O1_size.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/O1_size_fwd.hpp>
#include <eina_boost/mpl/minus.hpp>
#include <eina_boost/mpl/long.hpp>
#include <eina_boost/mpl/vector/aux_/tag.hpp>
#include <eina_boost/mpl/aux_/config/typeof.hpp>
#include <eina_boost/mpl/aux_/config/ctps.hpp>

namespace efl_eina_boost { namespace mpl {

#if defined(EFL_EINA_BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)

template<>
struct O1_size_impl< aux::vector_tag >
{
    template< typename Vector > struct apply
        : Vector::size
    {
    };
};

#else

#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< long N >
struct O1_size_impl< aux::vector_tag<N> >
{
    template< typename Vector > struct apply
        : mpl::long_<N>
    {
    };
};

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // EFL_EINA_BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES

}}

#endif // EFL_EINA_BOOST_MPL_VECTOR_AUX_O1_SIZE_HPP_INCLUDED
