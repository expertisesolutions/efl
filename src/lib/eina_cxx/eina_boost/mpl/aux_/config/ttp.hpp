
#ifndef EFL_EINA_BOOST_MPL_AUX_CONFIG_TTP_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_CONFIG_TTP_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: ttp.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/msvc.hpp>
#include <eina_boost/mpl/aux_/config/gcc.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_TEMPLATE_TEMPLATE_PARAMETERS) \
    && ( defined(EFL_EINA_BOOST_NO_TEMPLATE_TEMPLATES) \
      || EFL_EINA_BOOST_WORKAROUND( __BORLANDC__, EFL_EINA_BOOST_TESTED_AT( 0x590) ) \
       )

#   define EFL_EINA_BOOST_MPL_CFG_NO_TEMPLATE_TEMPLATE_PARAMETERS

#endif


#if    !defined(EFL_EINA_BOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING) \
    && !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE) \
    && (   EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MPL_CFG_GCC, EFL_EINA_BOOST_TESTED_AT(0x0302)) \
        || EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610)) \
        )

#   define EFL_EINA_BOOST_MPL_CFG_EXTENDED_TEMPLATE_PARAMETERS_MATCHING

#endif

#endif // EFL_EINA_BOOST_MPL_AUX_CONFIG_TTP_HPP_INCLUDED
