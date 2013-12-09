
#ifndef EFL_EINA_BOOST_MPL_AUX_INSERTER_ALGORITHM_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_INSERTER_ALGORITHM_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: inserter_algorithm.hpp 55648 2009-08-18 05:16:53Z agurtovoy $
// $Date: 2009-08-17 22:16:53 -0700 (Mon, 17 Aug 2009) $
// $Revision: 55648 $

#include <eina_boost/mpl/back_inserter.hpp>
#include <eina_boost/mpl/front_inserter.hpp>
#include <eina_boost/mpl/push_back.hpp>
#include <eina_boost/mpl/push_front.hpp>
#include <eina_boost/mpl/back_inserter.hpp>
#include <eina_boost/mpl/front_inserter.hpp>
#include <eina_boost/mpl/clear.hpp>
#include <eina_boost/mpl/eval_if.hpp>
#include <eina_boost/mpl/if.hpp>
#include <eina_boost/mpl/aux_/na.hpp>
#include <eina_boost/mpl/aux_/common_name_wknd.hpp>
#include <eina_boost/mpl/aux_/na_spec.hpp>
#include <eina_boost/mpl/aux_/preprocessor/params.hpp>
#include <eina_boost/mpl/aux_/preprocessor/default_params.hpp>
#include <eina_boost/mpl/aux_/config/ctps.hpp>

#include <eina_boost/preprocessor/arithmetic/dec.hpp>

#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#   define EFL_EINA_BOOST_MPL_AUX_INSERTER_ALGORITHM_DEF(arity, name) \
EFL_EINA_BOOST_MPL_AUX_COMMON_NAME_WKND(name) \
template< \
      EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(arity, typename P, na) \
    > \
struct name \
    : aux::name##_impl<EFL_EINA_BOOST_MPL_PP_PARAMS(arity, P)> \
{ \
}; \
\
template< \
      EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), typename P) \
    > \
struct name< EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P),na > \
    : if_< has_push_back< typename clear<P1>::type> \
        , aux::name##_impl< \
              EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P) \
            , back_inserter< typename clear<P1>::type > \
            > \
        , aux::reverse_##name##_impl< \
              EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P) \
            , front_inserter< typename clear<P1>::type > \
            > \
        >::type \
{ \
}; \
\
template< \
      EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(arity, typename P, na) \
    > \
struct reverse_##name \
    : aux::reverse_##name##_impl<EFL_EINA_BOOST_MPL_PP_PARAMS(arity, P)> \
{ \
}; \
\
template< \
      EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), typename P) \
    > \
struct reverse_##name< EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P),na > \
    : if_< has_push_back<P1> \
        , aux::reverse_##name##_impl< \
              EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P) \
            , back_inserter< typename clear<P1>::type > \
            > \
        , aux::name##_impl< \
              EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P) \
            , front_inserter< typename clear<P1>::type > \
            > \
        >::type \
{ \
}; \
EFL_EINA_BOOST_MPL_AUX_NA_SPEC(arity, name) \
EFL_EINA_BOOST_MPL_AUX_NA_SPEC(arity, reverse_##name) \
/**/

#else

#   define EFL_EINA_BOOST_MPL_AUX_INSERTER_ALGORITHM_DEF(arity, name) \
EFL_EINA_BOOST_MPL_AUX_COMMON_NAME_WKND(name) \
template< \
      EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), typename P) \
    > \
struct def_##name##_impl \
    : if_< has_push_back<P1> \
        , aux::name##_impl< \
              EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P) \
            , back_inserter< typename clear<P1>::type > \
            > \
        , aux::reverse_##name##_impl< \
              EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P) \
            , front_inserter< typename clear<P1>::type > \
            > \
        >::type \
{ \
}; \
\
template< \
      EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(arity, typename P, na) \
    > \
struct name \
{ \
    typedef typename eval_if< \
          is_na<EFL_EINA_BOOST_PP_CAT(P, arity)> \
        , def_##name##_impl<EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P)> \
        , aux::name##_impl<EFL_EINA_BOOST_MPL_PP_PARAMS(arity, P)> \
        >::type type; \
}; \
\
template< \
      EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), typename P) \
    > \
struct def_reverse_##name##_impl \
    : if_< has_push_back<P1> \
        , aux::reverse_##name##_impl< \
              EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P) \
            , back_inserter< typename clear<P1>::type > \
            > \
        , aux::name##_impl< \
              EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P) \
            , front_inserter< typename clear<P1>::type > \
            > \
        >::type \
{ \
}; \
template< \
      EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(arity, typename P, na) \
    > \
struct reverse_##name \
{ \
    typedef typename eval_if< \
          is_na<EFL_EINA_BOOST_PP_CAT(P, arity)> \
        , def_reverse_##name##_impl<EFL_EINA_BOOST_MPL_PP_PARAMS(EFL_EINA_BOOST_PP_DEC(arity), P)> \
        , aux::reverse_##name##_impl<EFL_EINA_BOOST_MPL_PP_PARAMS(arity, P)> \
        >::type type; \
}; \
EFL_EINA_BOOST_MPL_AUX_NA_SPEC(arity, name) \
EFL_EINA_BOOST_MPL_AUX_NA_SPEC(arity, reverse_##name) \
/**/

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // EFL_EINA_BOOST_MPL_AUX_INSERTER_ALGORITHM_HPP_INCLUDED
