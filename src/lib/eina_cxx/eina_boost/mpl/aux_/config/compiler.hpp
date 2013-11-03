
#ifndef EFL_EINA_BOOST_MPL_AUX_CONFIG_COMPILER_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_CONFIG_COMPILER_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2008
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: compiler.hpp 53189 2009-05-22 20:07:55Z hkaiser $
// $Date: 2009-05-22 13:07:55 -0700 (Fri, 22 May 2009) $
// $Revision: 53189 $

#if !defined(EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR)

#   include <eina_boost/mpl/aux_/config/dtp.hpp>
#   include <eina_boost/mpl/aux_/config/ttp.hpp>
#   include <eina_boost/mpl/aux_/config/ctps.hpp>
#   include <eina_boost/mpl/aux_/config/msvc.hpp>
#   include <eina_boost/mpl/aux_/config/gcc.hpp>
#   include <eina_boost/mpl/aux_/config/workaround.hpp>

#   if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, < 1300)
#       define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR msvc60

#   elif EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, == 1300)
#       define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR msvc70

#   elif EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MPL_CFG_GCC, EFL_EINA_BOOST_TESTED_AT(0x0304))
#       define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR gcc

#   elif EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610))
#       if !defined(EFL_EINA_BOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#           define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR bcc551
#       elif EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, >= 0x590)
#           define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR bcc
#       else
#           define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR bcc_pre590
#       endif

#   elif EFL_EINA_BOOST_WORKAROUND(__DMC__, EFL_EINA_BOOST_TESTED_AT(0x840))
#       define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR dmc

#   elif defined(__MWERKS__)
#       if defined(EFL_EINA_BOOST_MPL_CFG_BROKEN_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
#           define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR mwcw
#       else
#           define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR plain
#       endif

#   elif defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#       define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR no_ctps

#   elif defined(EFL_EINA_BOOST_MPL_CFG_NO_TEMPLATE_TEMPLATE_PARAMETERS)
#       define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR no_ttp

#   else
#       define EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR plain
#   endif

#endif // EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR

#endif // EFL_EINA_BOOST_MPL_AUX_CONFIG_COMPILER_HPP_INCLUDED
