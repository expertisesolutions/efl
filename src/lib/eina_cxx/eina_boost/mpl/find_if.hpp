
#ifndef EFL_EINA_BOOST_MPL_FIND_IF_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_FIND_IF_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: find_if.hpp 49274 2008-10-11 07:22:05Z agurtovoy $
// $Date: 2008-10-11 00:22:05 -0700 (Sat, 11 Oct 2008) $
// $Revision: 49274 $

#include <eina_boost/mpl/aux_/find_if_pred.hpp>
#include <eina_boost/mpl/arg.hpp>
#include <eina_boost/mpl/iter_fold_if.hpp>
#include <eina_boost/mpl/aux_/common_name_wknd.hpp>
#include <eina_boost/mpl/aux_/na_spec.hpp>
#include <eina_boost/mpl/aux_/lambda_support.hpp>

namespace efl_eina_boost { namespace mpl {

EFL_EINA_BOOST_MPL_AUX_COMMON_NAME_WKND(find_if)

template<
      typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(Predicate)
    >
struct find_if
{
    typedef typename iter_fold_if<
          Sequence
        , void
        , mpl::arg<1> // ignore
        , protect< aux::find_if_pred<Predicate> >
        >::type result_;

    typedef typename second<result_>::type type;

    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(2,find_if,(Sequence,Predicate))
};

EFL_EINA_BOOST_MPL_AUX_NA_SPEC(2,find_if)

}}

#endif // EFL_EINA_BOOST_MPL_FIND_IF_HPP_INCLUDED
