
#ifndef EFL_EINA_BOOST_MPL_EVAL_IF_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_EVAL_IF_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: eval_if.hpp 61921 2010-05-11 21:33:24Z neilgroves $
// $Date: 2010-05-11 14:33:24 -0700 (Tue, 11 May 2010) $
// $Revision: 61921 $

#include <eina_boost/mpl/if.hpp>
#include <eina_boost/mpl/aux_/na_spec.hpp>
#include <eina_boost/mpl/aux_/lambda_support.hpp>
#include <eina_boost/mpl/aux_/config/msvc.hpp>
#include <eina_boost/mpl/aux_/config/gcc.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

namespace efl_eina_boost { namespace mpl {

template<
      typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(C)
    , typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(F1)
    , typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(F2)
    >
struct eval_if
#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1300) \
     || ( EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MPL_CFG_GCC, >= 0x0300) \
        && EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MPL_CFG_GCC, EFL_EINA_BOOST_TESTED_AT(0x0304)) \
        )
{
    typedef typename if_<C,F1,F2>::type f_;
    typedef typename f_::type type;
#else
    : if_<C,F1,F2>::type
{
#endif
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(3,eval_if,(C,F1,F2))
};

// (almost) copy & paste in order to save one more 
// recursively nested template instantiation to user
template<
      bool C
    , typename F1
    , typename F2
    >
struct eval_if_c
#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1300) \
     || ( EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MPL_CFG_GCC, >= 0x0300) \
        && EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MPL_CFG_GCC, EFL_EINA_BOOST_TESTED_AT(0x0304)) \
        )
{
    typedef typename if_c<C,F1,F2>::type f_;
    typedef typename f_::type type;
#else
    : if_c<C,F1,F2>::type
{
#endif
};

EFL_EINA_BOOST_MPL_AUX_NA_SPEC(3, eval_if)

}}

#endif // EFL_EINA_BOOST_MPL_EVAL_IF_HPP_INCLUDED
