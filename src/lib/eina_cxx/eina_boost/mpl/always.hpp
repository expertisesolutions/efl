
#ifndef EFL_EINA_BOOST_MPL_ALWAYS_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_ALWAYS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: always.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/preprocessor/def_params_tail.hpp>
#include <eina_boost/mpl/aux_/na.hpp>
#include <eina_boost/mpl/aux_/arity_spec.hpp>

namespace efl_eina_boost { namespace mpl {

template< typename Value > struct always
{
    template<
          typename T
        EFL_EINA_BOOST_MPL_PP_NESTED_DEF_PARAMS_TAIL(1, typename T, na)
        >
    struct apply
    {
        typedef Value type;
    };
};

EFL_EINA_BOOST_MPL_AUX_ARITY_SPEC(1, always)

}}

#endif // EFL_EINA_BOOST_MPL_ALWAYS_HPP_INCLUDED
