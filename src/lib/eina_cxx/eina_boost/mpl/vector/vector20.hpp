
#ifndef EFL_EINA_BOOST_MPL_VECTOR_VECTOR20_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_VECTOR_VECTOR20_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: vector20.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#if !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)
#   include <eina_boost/mpl/vector/vector10.hpp>
#endif

#include <eina_boost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)

#   define EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER vector20.hpp
#   include <eina_boost/mpl/vector/aux_/include_preprocessed.hpp>

#else

#   include <eina_boost/mpl/aux_/config/typeof.hpp>
#   include <eina_boost/mpl/aux_/config/ctps.hpp>
#   include <eina_boost/preprocessor/iterate.hpp>

namespace efl_eina_boost { namespace mpl {

#   define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 \
    (3,(11, 20, <boost/mpl/vector/aux_/numbered.hpp>))
#   include EFL_EINA_BOOST_PP_ITERATE()

}}

#endif // EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS

#endif // EFL_EINA_BOOST_MPL_VECTOR_VECTOR20_HPP_INCLUDED
