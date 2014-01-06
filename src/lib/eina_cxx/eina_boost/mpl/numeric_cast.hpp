
#ifndef EFL_EINA_BOOST_MPL_NUMERIC_CAST_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_NUMERIC_CAST_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: numeric_cast.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/msvc.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

// agurt 21/sep/04: portability macro for the sake of MSVC 6.x-7.0;
// resolves conflicts with 'efl_eina_boost::numeric_cast' function template.
// use it in your own code _only_ if you care about compatibility with
// these outdated compilers!
#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1300) || EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x570) )
#   define EFL_EINA_BOOST_MPL_AUX_NUMERIC_CAST numeric_cast_
#else
#   define EFL_EINA_BOOST_MPL_AUX_NUMERIC_CAST numeric_cast
#endif

namespace efl_eina_boost { namespace mpl {

// no default implementation; the definition is needed to make MSVC happy

template< typename SourceTag, typename TargetTag > struct EFL_EINA_BOOST_MPL_AUX_NUMERIC_CAST
{
    template< typename N > struct apply;
};

}}

#endif // EFL_EINA_BOOST_MPL_NUMERIC_CAST_HPP_INCLUDED
