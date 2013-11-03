
#ifndef EFL_EINA_BOOST_MPL_AUX_PREPROCESSOR_PARTIAL_SPEC_PARAMS_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_PREPROCESSOR_PARTIAL_SPEC_PARAMS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: partial_spec_params.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/limits/arity.hpp>
#include <eina_boost/mpl/aux_/preprocessor/params.hpp>
#include <eina_boost/mpl/aux_/preprocessor/enum.hpp>
#include <eina_boost/mpl/aux_/preprocessor/sub.hpp>
#include <eina_boost/preprocessor/comma_if.hpp>

#define EFL_EINA_BOOST_MPL_PP_PARTIAL_SPEC_PARAMS(n, param, def) \
EFL_EINA_BOOST_MPL_PP_PARAMS(n, param) \
EFL_EINA_BOOST_PP_COMMA_IF(EFL_EINA_BOOST_MPL_PP_SUB(EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY,n)) \
EFL_EINA_BOOST_MPL_PP_ENUM( \
      EFL_EINA_BOOST_MPL_PP_SUB(EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY,n) \
    , def \
    ) \
/**/

#endif // EFL_EINA_BOOST_MPL_AUX_PREPROCESSOR_PARTIAL_SPEC_PARAMS_HPP_INCLUDED
