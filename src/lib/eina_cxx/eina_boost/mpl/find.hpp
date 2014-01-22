
#ifndef EFL_EINA_BOOST_MPL_FIND_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_FIND_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2002
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: find.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <efl_eina_boost/mpl/find_if.hpp>
#include <efl_eina_boost/mpl/same_as.hpp>
#include <efl_eina_boost/mpl/aux_/na_spec.hpp>
#include <efl_eina_boost/mpl/aux_/lambda_support.hpp>

namespace efl_eina_boost { namespace mpl {

template<
      typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(T)
    >
struct find
    : find_if< Sequence,same_as<T> >
{
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(2,find,(Sequence,T))
};

EFL_EINA_BOOST_MPL_AUX_NA_SPEC(2, find)

}}

#endif // EFL_EINA_BOOST_MPL_FIND_HPP_INCLUDED
