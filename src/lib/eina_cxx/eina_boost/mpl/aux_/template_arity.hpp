
#if !defined(EFL_EINA_BOOST_PP_IS_ITERATING)

///// header body

#ifndef EFL_EINA_BOOST_MPL_AUX_TEMPLATE_ARITY_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_TEMPLATE_ARITY_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: template_arity.hpp 61584 2010-04-26 18:48:26Z agurtovoy $
// $Date: 2010-04-26 11:48:26 -0700 (Mon, 26 Apr 2010) $
// $Revision: 61584 $

#include <eina_boost/mpl/aux_/config/ttp.hpp>
#include <eina_boost/mpl/aux_/config/lambda.hpp>

#if !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)
#   include <eina_boost/mpl/aux_/template_arity_fwd.hpp>
#   include <eina_boost/mpl/int.hpp>
#   if !defined(EFL_EINA_BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
#   if defined(EFL_EINA_BOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING)
#       include <eina_boost/mpl/aux_/type_wrapper.hpp>
#   endif
#   else
#       include <eina_boost/mpl/aux_/has_rebind.hpp>
#   endif
#endif

#include <eina_boost/mpl/aux_/config/static_constant.hpp>
#include <eina_boost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)

#   define EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER template_arity.hpp
#   include <eina_boost/mpl/aux_/include_preprocessed.hpp>

#else

#   if !defined(EFL_EINA_BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
#   if defined(EFL_EINA_BOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING)

#   include <eina_boost/mpl/limits/arity.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/range.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/repeat.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/params.hpp>
#   include <eina_boost/mpl/aux_/nttp_decl.hpp>

#   include <eina_boost/preprocessor/seq/fold_left.hpp>
#   include <eina_boost/preprocessor/comma_if.hpp>
#   include <eina_boost/preprocessor/iterate.hpp>
#   include <eina_boost/preprocessor/inc.hpp>
#   include <eina_boost/preprocessor/cat.hpp>

#   define AUX778076_ARITY EFL_EINA_BOOST_PP_INC(EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY)

namespace efl_eina_boost { namespace mpl { namespace aux {

template< EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(int, N) > struct arity_tag
{
    typedef char (&type)[N + 1];
};

#   define AUX778076_MAX_ARITY_OP(unused, state, i_) \
    ( EFL_EINA_BOOST_PP_CAT(C,i_) > 0 ? EFL_EINA_BOOST_PP_CAT(C,i_) : state ) \
/**/

template<
      EFL_EINA_BOOST_MPL_PP_PARAMS(AUX778076_ARITY, EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(int, C))
    >
struct max_arity
{
    EFL_EINA_BOOST_STATIC_CONSTANT(int, value = 
          EFL_EINA_BOOST_PP_SEQ_FOLD_LEFT(
              AUX778076_MAX_ARITY_OP
            , -1
            , EFL_EINA_BOOST_MPL_PP_RANGE(1, AUX778076_ARITY)
            )
        );
};

#   undef AUX778076_MAX_ARITY_OP

arity_tag<0>::type arity_helper(...);

#   define EFL_EINA_BOOST_PP_ITERATION_LIMITS (1, AUX778076_ARITY)
#   define EFL_EINA_BOOST_PP_FILENAME_1 <eina_boost/mpl/aux_/template_arity.hpp>
#   include EFL_EINA_BOOST_PP_ITERATE()

template< typename F, EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(int, N) >
struct template_arity_impl
{
    EFL_EINA_BOOST_STATIC_CONSTANT(int, value = 
          sizeof(::efl_eina_boost::mpl::aux::arity_helper(type_wrapper<F>(),arity_tag<N>())) - 1
        );
};

#   define AUX778076_TEMPLATE_ARITY_IMPL_INVOCATION(unused, i_, F) \
    EFL_EINA_BOOST_PP_COMMA_IF(i_) template_arity_impl<F,EFL_EINA_BOOST_PP_INC(i_)>::value \
/**/

template< typename F >
struct template_arity
{
    EFL_EINA_BOOST_STATIC_CONSTANT(int, value = (
          max_arity< EFL_EINA_BOOST_MPL_PP_REPEAT(
              AUX778076_ARITY
            , AUX778076_TEMPLATE_ARITY_IMPL_INVOCATION
            , F
            ) >::value
        ));
        
    typedef mpl::int_<value> type;
};

#   undef AUX778076_TEMPLATE_ARITY_IMPL_INVOCATION

#   undef AUX778076_ARITY

}}}

#   endif // EFL_EINA_BOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING
#   else // EFL_EINA_BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT

#   include <eina_boost/mpl/aux_/config/eti.hpp>

namespace efl_eina_boost { namespace mpl { namespace aux {

template< bool >
struct template_arity_impl
{
    template< typename F > struct result_
        : mpl::int_<-1>
    {
    };
};

template<>
struct template_arity_impl<true>
{
    template< typename F > struct result_
        : F::arity
    {
    };
};

template< typename F >
struct template_arity
    : template_arity_impl< ::efl_eina_boost::mpl::aux::has_rebind<F>::value >
        ::template result_<F>
{
};

#if defined(EFL_EINA_BOOST_MPL_CFG_MSVC_ETI_BUG)
template<>
struct template_arity<int>
    : mpl::int_<-1>
{
};
#endif

}}}

#   endif // EFL_EINA_BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT

#endif // EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // EFL_EINA_BOOST_MPL_AUX_TEMPLATE_ARITY_HPP_INCLUDED

///// iteration

#else
#define i_ EFL_EINA_BOOST_PP_FRAME_ITERATION(1)

template<
      template< EFL_EINA_BOOST_MPL_PP_PARAMS(i_, typename P) > class F
    , EFL_EINA_BOOST_MPL_PP_PARAMS(i_, typename T)
    >
typename arity_tag<i_>::type
arity_helper(type_wrapper< F<EFL_EINA_BOOST_MPL_PP_PARAMS(i_, T)> >, arity_tag<i_>);

#undef i_
#endif // EFL_EINA_BOOST_PP_IS_ITERATING
