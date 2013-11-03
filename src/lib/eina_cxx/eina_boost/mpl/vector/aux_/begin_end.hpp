
#ifndef EFL_EINA_BOOST_MPL_VECTOR_AUX_BEGIN_END_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_VECTOR_AUX_BEGIN_END_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: begin_end.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/typeof.hpp>

#if defined(EFL_EINA_BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)

#   include <eina_boost/mpl/begin_end_fwd.hpp>
#   include <eina_boost/mpl/vector/aux_/iterator.hpp>
#   include <eina_boost/mpl/vector/aux_/tag.hpp>

namespace efl_eina_boost { namespace mpl {

template<>
struct begin_impl< aux::vector_tag >
{
    template< typename Vector > struct apply
    {
        typedef v_iter<Vector,0> type;
    };
};

template<>
struct end_impl< aux::vector_tag >
{
    template< typename Vector > struct apply
    {
        typedef v_iter<Vector,Vector::size::value> type;
    };
};

}}

#endif // EFL_EINA_BOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES

#endif // EFL_EINA_BOOST_MPL_VECTOR_AUX_BEGIN_END_HPP_INCLUDED
