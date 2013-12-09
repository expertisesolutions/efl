
#if !defined(EFL_EINA_BOOST_PP_IS_ITERATING)

///// header body

#ifndef EFL_EINA_BOOST_MPL_AUX_LAMBDA_NO_CTPS_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_LAMBDA_NO_CTPS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: lambda_no_ctps.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#if !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)
#   include <eina_boost/mpl/lambda_fwd.hpp>
#   include <eina_boost/mpl/bind_fwd.hpp>
#   include <eina_boost/mpl/protect.hpp>
#   include <eina_boost/mpl/is_placeholder.hpp>
#   include <eina_boost/mpl/if.hpp>
#   include <eina_boost/mpl/identity.hpp>
#   include <eina_boost/mpl/bool.hpp>
#   include <eina_boost/mpl/aux_/na_spec.hpp>
#   include <eina_boost/mpl/aux_/lambda_support.hpp>
#   include <eina_boost/mpl/aux_/template_arity.hpp>
#   include <eina_boost/mpl/aux_/value_wknd.hpp>
#endif

#include <eina_boost/mpl/aux_/config/use_preprocessed.hpp>

#if    !defined(EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)

#   define EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER lambda_no_ctps.hpp
#   include <eina_boost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <eina_boost/mpl/limits/arity.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/params.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/default_params.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/repeat.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/enum.hpp>
#   include <eina_boost/mpl/aux_/config/msvc.hpp>
#   include <eina_boost/mpl/aux_/config/workaround.hpp>

#   include <eina_boost/preprocessor/comma_if.hpp>
#   include <eina_boost/preprocessor/iterate.hpp>
#   include <eina_boost/preprocessor/inc.hpp>
#   include <eina_boost/preprocessor/cat.hpp>

namespace efl_eina_boost { namespace mpl {

#   define AUX778076_LAMBDA_PARAMS(i_, param) \
    EFL_EINA_BOOST_MPL_PP_PARAMS(i_, param) \
    /**/

namespace aux {

#define n_ EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY
template<
      EFL_EINA_BOOST_MPL_PP_DEFAULT_PARAMS(n_,bool C,false)
    >
struct lambda_or
    : true_
{
};

template<>
struct lambda_or< EFL_EINA_BOOST_MPL_PP_ENUM(n_,false) >
    : false_
{
};
#undef n_

template< typename Arity > struct lambda_impl
{
    template< typename T, typename Tag, typename Protect > struct result_
    {
        typedef T type;
        typedef is_placeholder<T> is_le;
    };
};

#define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 \
    (3,(1, EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY, <boost/mpl/aux_/lambda_no_ctps.hpp>))
#include EFL_EINA_BOOST_PP_ITERATE()

} // namespace aux

template<
      typename T
    , typename Tag
    , typename Protect
    >
struct lambda
{
    /// Metafunction forwarding confuses MSVC 6.x
    typedef typename aux::template_arity<T>::type arity_;
    typedef typename aux::lambda_impl<arity_>
        ::template result_< T,Tag,Protect > l_;

    typedef typename l_::type type;
    typedef typename l_::is_le is_le;
    
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(3, lambda, (T, Tag, Protect))
};

EFL_EINA_BOOST_MPL_AUX_NA_SPEC2(1, 3, lambda)

template<
      typename T
    >
struct is_lambda_expression
    : lambda<T>::is_le
{
};

#   undef AUX778076_LAMBDA_PARAMS

}}

#endif // EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // EFL_EINA_BOOST_MPL_AUX_LAMBDA_NO_CTPS_HPP_INCLUDED

///// iteration, depth == 1

#else

#define i_ EFL_EINA_BOOST_PP_FRAME_ITERATION(1)

#   define AUX778076_LAMBDA_TYPEDEF(unused, i_, F) \
    typedef lambda< \
          typename F::EFL_EINA_BOOST_PP_CAT(arg,EFL_EINA_BOOST_PP_INC(i_)) \
        , Tag \
        , false_ \
        > EFL_EINA_BOOST_PP_CAT(l,EFL_EINA_BOOST_PP_INC(i_)); \
    /**/

#   define AUX778076_IS_LE_TYPEDEF(unused, i_, unused2) \
    typedef typename EFL_EINA_BOOST_PP_CAT(l,EFL_EINA_BOOST_PP_INC(i_))::is_le \
        EFL_EINA_BOOST_PP_CAT(is_le,EFL_EINA_BOOST_PP_INC(i_)); \
    /**/

#   define AUX778076_IS_LAMBDA_EXPR(unused, i_, unused2) \
    EFL_EINA_BOOST_PP_COMMA_IF(i_) \
    EFL_EINA_BOOST_MPL_AUX_MSVC_VALUE_WKND(EFL_EINA_BOOST_PP_CAT(is_le,EFL_EINA_BOOST_PP_INC(i_)))::value \
    /**/

#   define AUX778076_LAMBDA_RESULT(unused, i_, unused2) \
    , typename EFL_EINA_BOOST_PP_CAT(l,EFL_EINA_BOOST_PP_INC(i_))::type \
    /**/

template<> struct lambda_impl< int_<i_> >
{
    template< typename F, typename Tag, typename Protect > struct result_
    {
        EFL_EINA_BOOST_MPL_PP_REPEAT(i_, AUX778076_LAMBDA_TYPEDEF, F)
        EFL_EINA_BOOST_MPL_PP_REPEAT(i_, AUX778076_IS_LE_TYPEDEF, unused)

        typedef aux::lambda_or<
              EFL_EINA_BOOST_MPL_PP_REPEAT(i_, AUX778076_IS_LAMBDA_EXPR, unused)
            > is_le;

        typedef EFL_EINA_BOOST_PP_CAT(bind,i_)<
              typename F::rebind
            EFL_EINA_BOOST_MPL_PP_REPEAT(i_, AUX778076_LAMBDA_RESULT, unused)
            > bind_;

        typedef typename if_<
              is_le
            , if_< Protect, mpl::protect<bind_>, bind_ >
            , identity<F>
            >::type type_;
    
        typedef typename type_::type type;
    };
};

#   undef AUX778076_LAMBDA_RESULT
#   undef AUX778076_IS_LAMBDA_EXPR
#   undef AUX778076_IS_LE_TYPEDEF
#   undef AUX778076_LAMBDA_TYPEDEF

#undef i_

#endif // EFL_EINA_BOOST_PP_IS_ITERATING
