
#if !defined(EFL_EINA_BOOST_PP_IS_ITERATING)

///// header body

#ifndef EFL_EINA_BOOST_MPL_ARG_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_ARG_HPP_INCLUDED

// Copyright Peter Dimov 2001-2002
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: arg.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#if !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)
#   include <eina_boost/mpl/arg_fwd.hpp>
#   include <eina_boost/mpl/aux_/na.hpp>
#   include <eina_boost/mpl/aux_/na_assert.hpp>
#   include <eina_boost/mpl/aux_/arity_spec.hpp>
#   include <eina_boost/mpl/aux_/arg_typedef.hpp>
#endif

#include <eina_boost/mpl/aux_/config/static_constant.hpp>
#include <eina_boost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)

#   define EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER arg.hpp
#   include <eina_boost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <eina_boost/mpl/limits/arity.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/default_params.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/params.hpp>
#   include <eina_boost/mpl/aux_/config/lambda.hpp>
#   include <eina_boost/mpl/aux_/config/dtp.hpp>
#   include <eina_boost/mpl/aux_/nttp_decl.hpp>

#   include <eina_boost/preprocessor/iterate.hpp>
#   include <eina_boost/preprocessor/inc.hpp>
#   include <eina_boost/preprocessor/cat.hpp>

EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

// local macro, #undef-ined at the end of the header
#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#   define AUX778076_ARG_N_DEFAULT_PARAMS(param,value) \
    EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS( \
          EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        , value \
        ) \
    /**/
#else
#   define AUX778076_ARG_N_DEFAULT_PARAMS(param,value) \
    EFL_EINA_BOOST_MPL_PP_PARAMS( \
          EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY \
        , param \
        ) \
    /**/
#endif

#define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY, <boost/mpl/arg.hpp>))
#include EFL_EINA_BOOST_PP_ITERATE()


#   undef AUX778076_ARG_N_DEFAULT_PARAMS

EFL_EINA_BOOST_MPL_AUX_NONTYPE_ARITY_SPEC(1,int,arg)

EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

#endif // EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // EFL_EINA_BOOST_MPL_ARG_HPP_INCLUDED

///// iteration

#else
#define i_ EFL_EINA_BOOST_PP_FRAME_ITERATION(1)

#if i_ > 0

template<> struct arg<i_>
{
    EFL_EINA_BOOST_STATIC_CONSTANT(int, value = i_);
    typedef arg<EFL_EINA_BOOST_PP_INC(i_)> next;
    EFL_EINA_BOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    EFL_EINA_BOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          AUX778076_ARG_N_DEFAULT_PARAMS(typename U, na)
        >
    struct apply
    {
        typedef EFL_EINA_BOOST_PP_CAT(U,i_) type;
        EFL_EINA_BOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

#else

template<> struct arg<-1>
{
    EFL_EINA_BOOST_STATIC_CONSTANT(int, value = -1);
    EFL_EINA_BOOST_MPL_AUX_ARG_TYPEDEF(na, tag)
    EFL_EINA_BOOST_MPL_AUX_ARG_TYPEDEF(na, type)

    template<
          AUX778076_ARG_N_DEFAULT_PARAMS(typename U, na)
        >
    struct apply
    {
        typedef U1 type;
        EFL_EINA_BOOST_MPL_AUX_ASSERT_NOT_NA(type);
    };
};

#endif // i_ > 0

#undef i_
#endif // EFL_EINA_BOOST_PP_IS_ITERATING
