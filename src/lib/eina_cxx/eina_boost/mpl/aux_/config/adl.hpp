
#ifndef EFL_EINA_BOOST_MPL_AUX_CONFIG_ADL_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_CONFIG_ADL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: adl.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/msvc.hpp>
#include <eina_boost/mpl/aux_/config/intel.hpp>
#include <eina_boost/mpl/aux_/config/gcc.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

// agurt, 25/apr/04: technically, the ADL workaround is only needed for GCC,
// but putting everything expect public, user-specializable metafunctions into
// a separate global namespace has a nice side effect of reducing the length 
// of template instantiation symbols, so we apply the workaround on all 
// platforms that can handle it

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE) \
    && (   EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, EFL_EINA_BOOST_TESTED_AT(1400)) \
        || EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610)) \
        || EFL_EINA_BOOST_WORKAROUND(__DMC__, EFL_EINA_BOOST_TESTED_AT(0x840)) \
        || EFL_EINA_BOOST_WORKAROUND(__MWERKS__, EFL_EINA_BOOST_TESTED_AT(0x3202)) \
        || EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_INTEL_CXX_VERSION, EFL_EINA_BOOST_TESTED_AT(810)) \
        )

#   define EFL_EINA_BOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE

#endif

#endif // EFL_EINA_BOOST_MPL_AUX_CONFIG_ADL_HPP_INCLUDED
