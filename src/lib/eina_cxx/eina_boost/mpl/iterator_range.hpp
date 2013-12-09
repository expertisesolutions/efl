
#ifndef EFL_EINA_BOOST_MPL_ITERATOR_RANGE_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_ITERATOR_RANGE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: iterator_range.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/na_spec.hpp>
#include <eina_boost/mpl/aux_/lambda_support.hpp>

namespace efl_eina_boost { namespace mpl {

struct iterator_range_tag;

template<
      typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(First)
    , typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(Last)
    >
struct iterator_range
{
    typedef iterator_range_tag tag;
    typedef iterator_range type;
    typedef First begin;
    typedef Last end;

    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(2,iterator_range,(First,Last))
};

EFL_EINA_BOOST_MPL_AUX_NA_SPEC(2, iterator_range)

}}

#endif // EFL_EINA_BOOST_MPL_ITERATOR_RANGE_HPP_INCLUDED
