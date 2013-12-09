
#ifndef EFL_EINA_BOOST_MPL_AUX_PREPROCESSOR_DEFAULT_PARAMS_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_PREPROCESSOR_DEFAULT_PARAMS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: default_params.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/preprocessor.hpp>

// EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(0,T,int): <nothing>
// EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(1,T,int): T1 = int
// EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(2,T,int): T1 = int, T2 = int
// EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(n,T,int): T1 = int, T2 = int, .., Tn = int

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_OWN_PP_PRIMITIVES)

#   include <eina_boost/preprocessor/cat.hpp>

#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(n,p,v) \
    EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_,n)(p,v) \
    /**/
    
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_0(p,v)
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_1(p,v) p##1=v
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_2(p,v) p##1=v,p##2=v
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_3(p,v) p##1=v,p##2=v,p##3=v
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_4(p,v) p##1=v,p##2=v,p##3=v,p##4=v
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_5(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_6(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v,p##6=v
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_7(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v,p##6=v,p##7=v
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_8(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v,p##6=v,p##7=v,p##8=v
#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS_9(p,v) p##1=v,p##2=v,p##3=v,p##4=v,p##5=v,p##6=v,p##7=v,p##8=v,p##9=v

#else

#   include <eina_boost/preprocessor/tuple/elem.hpp>
#   include <eina_boost/preprocessor/comma_if.hpp>
#   include <eina_boost/preprocessor/repeat.hpp>
#   include <eina_boost/preprocessor/inc.hpp>
#   include <eina_boost/preprocessor/cat.hpp>

#   define EFL_EINA_BOOST_MPL_PP_AUX_DEFAULT_PARAM_FUNC(unused, i, pv) \
    EFL_EINA_BOOST_PP_COMMA_IF(i) \
    EFL_EINA_BOOST_PP_CAT( EFL_EINA_BOOST_PP_TUPLE_ELEM(2,0,pv), EFL_EINA_BOOST_PP_INC(i) ) \
        = EFL_EINA_BOOST_PP_TUPLE_ELEM(2,1,pv) \
    /**/

#   define EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(n, param, value) \
    EFL_EINA_BOOST_PP_REPEAT( \
          n \
        , EFL_EINA_BOOST_MPL_PP_AUX_DEFAULT_PARAM_FUNC \
        , (param,value) \
        ) \
    /**/

#endif

#endif // EFL_EINA_BOOST_MPL_AUX_PREPROCESSOR_DEFAULT_PARAMS_HPP_INCLUDED
