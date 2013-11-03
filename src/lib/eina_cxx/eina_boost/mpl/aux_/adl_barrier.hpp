
#ifndef EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: adl_barrier.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/adl.hpp>
#include <eina_boost/mpl/aux_/config/gcc.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE)

#   define EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE efl_eina_boost_mpl_
#   define EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN namespace efl_eina_boost_mpl_ {
#   define EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE }
#   define EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_DECL(type) \
    namespace efl_eina_boost { namespace mpl { \
    using ::EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::type; \
    } } \
/**/

#if !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)
EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN namespace aux {} EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
namespace efl_eina_boost { namespace mpl { using namespace EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE; 
namespace aux { using namespace EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::aux; }
}}
#endif

#else // EFL_EINA_BOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE

#   define EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE efl_eina_boost::mpl
#   define EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN namespace efl_eina_boost { namespace mpl {
#   define EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE }}
#   define EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_DECL(type) /**/

#endif

#endif // EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_HPP_INCLUDED
