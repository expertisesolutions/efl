
#ifndef EFL_EINA_BOOST_MPL_PROTECT_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_PROTECT_HPP_INCLUDED

// Copyright Peter Dimov 2001
// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: protect.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/arity.hpp>
#include <eina_boost/mpl/aux_/config/dtp.hpp>
#include <eina_boost/mpl/aux_/nttp_decl.hpp>
#include <eina_boost/mpl/aux_/na_spec.hpp>

namespace efl_eina_boost { namespace mpl {

template<
      typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(T)
    , int not_le_ = 0
    >
struct protect : T
{
#if EFL_EINA_BOOST_WORKAROUND(__EDG_VERSION__, == 238)
    typedef mpl::protect type;
#else
    typedef protect type;
#endif
};

#if defined(EFL_EINA_BOOST_MPL_CFG_BROKEN_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
namespace aux { 
template< EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(int, N), typename T >
struct arity< protect<T>, N > 
    : arity<T,N>
{ 
};
} // namespace aux
#endif

EFL_EINA_BOOST_MPL_AUX_NA_SPEC_MAIN(1, protect)
#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)
EFL_EINA_BOOST_MPL_AUX_NA_SPEC_TEMPLATE_ARITY(1, 1, protect)
#endif

}}

#endif // EFL_EINA_BOOST_MPL_PROTECT_HPP_INCLUDED
