
#ifndef EFL_EINA_BOOST_MPL_AUX_CONFIG_NTTP_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_CONFIG_NTTP_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: nttp.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/msvc.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

// MSVC 6.5 ICE-s on the code as simple as this (see "aux_/nttp_decl.hpp"
// for a workaround):
//
//    namespace std {
//    template< typename Char > struct string;
//    }
//
//    void foo(std::string<char>);
//
//    namespace efl_eina_boost { namespace mpl {
//    template< int > struct arg;
//    }}

#if    !defined(EFL_EINA_BOOST_MPL_CFG_NTTP_BUG) \
    && !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE) \
    && EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, < 1300)

#   define EFL_EINA_BOOST_MPL_CFG_NTTP_BUG

#endif

#endif // EFL_EINA_BOOST_MPL_AUX_CONFIG_NTTP_HPP_INCLUDED
