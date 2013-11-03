
#ifndef EFL_EINA_BOOST_MPL_AUX_PREPROCESSOR_SUB_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_PREPROCESSOR_SUB_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: sub.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/preprocessor.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES)

#   include <eina_boost/mpl/aux_/preprocessor/tuple.hpp>

#if defined(EFL_EINA_BOOST_MPL_CFG_BROKEN_PP_MACRO_EXPANSION)
#   include <eina_boost/preprocessor/cat.hpp>

#   define EFL_EINA_BOOST_MPL_PP_SUB(i,j) \
    EFL_EINA_BOOST_MPL_PP_SUB_DELAY(i,j) \
    /**/

#   define EFL_EINA_BOOST_MPL_PP_SUB_DELAY(i,j) \
    EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_MPL_PP_TUPLE_11_ELEM_##i,EFL_EINA_BOOST_MPL_PP_SUB_##j) \
    /**/
#else
#   define EFL_EINA_BOOST_MPL_PP_SUB(i,j) \
    EFL_EINA_BOOST_MPL_PP_SUB_DELAY(i,j) \
    /**/

#   define EFL_EINA_BOOST_MPL_PP_SUB_DELAY(i,j) \
    EFL_EINA_BOOST_MPL_PP_TUPLE_11_ELEM_##i EFL_EINA_BOOST_MPL_PP_SUB_##j \
    /**/
#endif

#   define EFL_EINA_BOOST_MPL_PP_SUB_0 (0,1,2,3,4,5,6,7,8,9,10)
#   define EFL_EINA_BOOST_MPL_PP_SUB_1 (0,0,1,2,3,4,5,6,7,8,9)
#   define EFL_EINA_BOOST_MPL_PP_SUB_2 (0,0,0,1,2,3,4,5,6,7,8)
#   define EFL_EINA_BOOST_MPL_PP_SUB_3 (0,0,0,0,1,2,3,4,5,6,7)
#   define EFL_EINA_BOOST_MPL_PP_SUB_4 (0,0,0,0,0,1,2,3,4,5,6)
#   define EFL_EINA_BOOST_MPL_PP_SUB_5 (0,0,0,0,0,0,1,2,3,4,5)
#   define EFL_EINA_BOOST_MPL_PP_SUB_6 (0,0,0,0,0,0,0,1,2,3,4)
#   define EFL_EINA_BOOST_MPL_PP_SUB_7 (0,0,0,0,0,0,0,0,1,2,3)
#   define EFL_EINA_BOOST_MPL_PP_SUB_8 (0,0,0,0,0,0,0,0,0,1,2)
#   define EFL_EINA_BOOST_MPL_PP_SUB_9 (0,0,0,0,0,0,0,0,0,0,1)
#   define EFL_EINA_BOOST_MPL_PP_SUB_10 (0,0,0,0,0,0,0,0,0,0,0)

#else

#   include <eina_boost/preprocessor/arithmetic/sub.hpp>

#   define EFL_EINA_BOOST_MPL_PP_SUB(i,j) \
    EFL_EINA_BOOST_PP_SUB(i,j) \
    /**/
    
#endif

#endif // EFL_EINA_BOOST_MPL_AUX_PREPROCESSOR_SUB_HPP_INCLUDED
