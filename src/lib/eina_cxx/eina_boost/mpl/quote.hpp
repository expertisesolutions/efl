
#if !defined(EFL_EINA_BOOST_PP_IS_ITERATING)

///// header body

#ifndef EFL_EINA_BOOST_MPL_QUOTE_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_QUOTE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2008
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: quote.hpp 49272 2008-10-11 06:50:46Z agurtovoy $
// $Date: 2008-10-10 23:50:46 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49272 $

#if !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)
#   include <eina_boost/mpl/void.hpp>
#   include <eina_boost/mpl/aux_/has_type.hpp>
#endif

#include <eina_boost/mpl/aux_/config/bcc.hpp>
#include <eina_boost/mpl/aux_/config/ttp.hpp>

#if defined(EFL_EINA_BOOST_MPL_CFG_NO_TEMPLATE_TEMPLATE_PARAMETERS) \
    && !defined(EFL_EINA_BOOST_MPL_CFG_BCC590_WORKAROUNDS)
#   define EFL_EINA_BOOST_MPL_CFG_NO_QUOTE_TEMPLATE
#endif

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_IMPLICIT_METAFUNCTIONS) \
    && defined(EFL_EINA_BOOST_MPL_CFG_NO_HAS_XXX)
#   define EFL_EINA_BOOST_MPL_CFG_NO_IMPLICIT_METAFUNCTIONS
#endif

#include <eina_boost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
 && !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)

#   define EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER quote.hpp
#   include <eina_boost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <eina_boost/mpl/limits/arity.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/params.hpp>
#   include <eina_boost/mpl/aux_/config/ctps.hpp>
#   include <eina_boost/mpl/aux_/config/workaround.hpp>

#   include <eina_boost/preprocessor/iterate.hpp>
#   include <eina_boost/preprocessor/cat.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_QUOTE_TEMPLATE)

namespace efl_eina_boost { namespace mpl {

#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< typename T, bool has_type_ >
struct quote_impl
// GCC has a problem with metafunction forwarding when T is a
// specialization of a template called 'type'.
# if EFL_EINA_BOOST_WORKAROUND(__GNUC__, EFL_EINA_BOOST_TESTED_AT(4)) \
    && EFL_EINA_BOOST_WORKAROUND(__GNUC_MINOR__, EFL_EINA_BOOST_TESTED_AT(0)) \
    && EFL_EINA_BOOST_WORKAROUND(__GNUC_PATCHLEVEL__, EFL_EINA_BOOST_TESTED_AT(2))
{
    typedef typename T::type type;
};
# else 
    : T
{
};
# endif 

template< typename T >
struct quote_impl<T,false>
{
    typedef T type;
};

#else // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template< bool > struct quote_impl
{
    template< typename T > struct result_
        : T
    {
    };
};

template<> struct quote_impl<false>
{
    template< typename T > struct result_
    {
        typedef T type;
    };
};

#endif 

#define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 \
    (3,(1, EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY, <boost/mpl/quote.hpp>))
#include EFL_EINA_BOOST_PP_ITERATE()

}}

#endif // EFL_EINA_BOOST_MPL_CFG_NO_QUOTE_TEMPLATE

#endif // EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // EFL_EINA_BOOST_MPL_QUOTE_HPP_INCLUDED

///// iteration

#else
#define i_ EFL_EINA_BOOST_PP_FRAME_ITERATION(1)

template<
      template< EFL_EINA_BOOST_MPL_PP_PARAMS(i_, typename P) > class F
    , typename Tag = void_
    >
struct EFL_EINA_BOOST_PP_CAT(quote,i_)
{
    template< EFL_EINA_BOOST_MPL_PP_PARAMS(i_, typename U) > struct apply
#if defined(EFL_EINA_BOOST_MPL_CFG_BCC590_WORKAROUNDS)
    {
        typedef typename quote_impl<
              F< EFL_EINA_BOOST_MPL_PP_PARAMS(i_, U) >
            , aux::has_type< F< EFL_EINA_BOOST_MPL_PP_PARAMS(i_, U) > >::value
            >::type type;
    };
#elif !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
        : quote_impl<
              F< EFL_EINA_BOOST_MPL_PP_PARAMS(i_, U) >
            , aux::has_type< F< EFL_EINA_BOOST_MPL_PP_PARAMS(i_, U) > >::value
            >
    {
    };
#else
        : quote_impl< aux::has_type< F< EFL_EINA_BOOST_MPL_PP_PARAMS(i_, U) > >::value >
            ::template result_< F< EFL_EINA_BOOST_MPL_PP_PARAMS(i_, U) > >
    {
    };
#endif
};

#undef i_
#endif // EFL_EINA_BOOST_PP_IS_ITERATING
