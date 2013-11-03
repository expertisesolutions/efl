
#ifndef EFL_EINA_BOOST_MPL_AUX_VALUE_WKND_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_VALUE_WKND_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: value_wknd.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/static_cast.hpp>
#include <eina_boost/mpl/aux_/config/integral.hpp>
#include <eina_boost/mpl/aux_/config/eti.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

#if defined(EFL_EINA_BOOST_MPL_CFG_BCC_INTEGRAL_CONSTANTS) \
    || defined(EFL_EINA_BOOST_MPL_CFG_MSVC_60_ETI_BUG)

#   include <eina_boost/mpl/int.hpp>

namespace efl_eina_boost { namespace mpl { namespace aux {
template< typename C_ > struct value_wknd
    : C_
{
};

#if defined(EFL_EINA_BOOST_MPL_CFG_MSVC_60_ETI_BUG)
template<> struct value_wknd<int>
    : int_<1>
{
    using int_<1>::value;
};
#endif
}}}


#if !defined(EFL_EINA_BOOST_MPL_CFG_MSVC_60_ETI_BUG)
#   define EFL_EINA_BOOST_MPL_AUX_VALUE_WKND(C) \
    ::EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::aux::value_wknd< C > \
/**/
#    define EFL_EINA_BOOST_MPL_AUX_MSVC_VALUE_WKND(C) EFL_EINA_BOOST_MPL_AUX_VALUE_WKND(C)
#else
#   define EFL_EINA_BOOST_MPL_AUX_VALUE_WKND(C) C
#   define EFL_EINA_BOOST_MPL_AUX_MSVC_VALUE_WKND(C) \
    ::efl_eina_boost::mpl::aux::value_wknd< C > \
/**/
#endif

#else // EFL_EINA_BOOST_MPL_CFG_BCC_INTEGRAL_CONSTANTS

#   define EFL_EINA_BOOST_MPL_AUX_VALUE_WKND(C) C
#   define EFL_EINA_BOOST_MPL_AUX_MSVC_VALUE_WKND(C) C

#endif

#if EFL_EINA_BOOST_WORKAROUND(__EDG_VERSION__, <= 238)
#   define EFL_EINA_BOOST_MPL_AUX_NESTED_VALUE_WKND(T, C) \
    EFL_EINA_BOOST_MPL_AUX_STATIC_CAST(T, C::value) \
/**/
#else
#   define EFL_EINA_BOOST_MPL_AUX_NESTED_VALUE_WKND(T, C) \
    EFL_EINA_BOOST_MPL_AUX_VALUE_WKND(C)::value \
/**/
#endif


namespace efl_eina_boost { namespace mpl { namespace aux {

template< typename T > struct value_type_wknd
{
    typedef typename T::value_type type;
};

#if defined(EFL_EINA_BOOST_MPL_CFG_MSVC_ETI_BUG)
template<> struct value_type_wknd<int>
{
    typedef int type;
};
#endif

}}}

#endif // EFL_EINA_BOOST_MPL_AUX_VALUE_WKND_HPP_INCLUDED
