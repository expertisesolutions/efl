
#ifndef EFL_EINA_BOOST_MPL_FRONT_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_FRONT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: front.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/front_fwd.hpp>
#include <eina_boost/mpl/aux_/front_impl.hpp>
#include <eina_boost/mpl/sequence_tag.hpp>
#include <eina_boost/mpl/aux_/na_spec.hpp>
#include <eina_boost/mpl/aux_/lambda_support.hpp>

namespace efl_eina_boost { namespace mpl {

template<
      typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(Sequence)
    >
struct front
    : front_impl< typename sequence_tag<Sequence>::type >
        ::template apply< Sequence >
{
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(1,front,(Sequence))
};

EFL_EINA_BOOST_MPL_AUX_NA_SPEC(1, front)

}}

#endif // EFL_EINA_BOOST_MPL_FRONT_HPP_INCLUDED
