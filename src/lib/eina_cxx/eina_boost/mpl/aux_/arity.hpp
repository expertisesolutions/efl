
#ifndef EFL_EINA_BOOST_MPL_AUX_ARITY_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_ARITY_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: arity.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/dtp.hpp>

#if defined(EFL_EINA_BOOST_MPL_CFG_BROKEN_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)

#   include <eina_boost/mpl/aux_/nttp_decl.hpp>
#   include <eina_boost/mpl/aux_/config/static_constant.hpp>

namespace efl_eina_boost { namespace mpl { namespace aux {

// agurt, 15/mar/02: it's possible to implement the template so that it will 
// "just work" and do not require any specialization, but not on the compilers
// that require the arity workaround in the first place
template< typename F, EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(int, N) >
struct arity
{
    EFL_EINA_BOOST_STATIC_CONSTANT(int, value = N);
};

}}}

#endif // EFL_EINA_BOOST_MPL_CFG_BROKEN_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES

#endif // EFL_EINA_BOOST_MPL_AUX_ARITY_HPP_INCLUDED
