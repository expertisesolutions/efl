
#ifndef EFL_EINA_BOOST_MPL_AUX_HAS_REBIND_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_HAS_REBIND_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: has_rebind.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/msvc.hpp>
#include <eina_boost/mpl/aux_/config/intel.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

#if EFL_EINA_BOOST_WORKAROUND(__EDG_VERSION__, <= 244) && !defined(EFL_EINA_BOOST_INTEL_CXX_VERSION)
#   include <eina_boost/mpl/has_xxx.hpp>
#elif EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, < 1300)
#   include <eina_boost/mpl/has_xxx.hpp>
#   include <eina_boost/mpl/if.hpp>
#   include <eina_boost/mpl/bool.hpp>
#   include <eina_boost/mpl/aux_/msvc_is_class.hpp>
#elif EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610))
#   include <eina_boost/mpl/if.hpp>
#   include <eina_boost/mpl/bool.hpp>
#   include <eina_boost/mpl/aux_/yes_no.hpp>
#   include <eina_boost/mpl/aux_/config/static_constant.hpp>
#   include <eina_boost/type_traits/is_class.hpp>
#else
#   include <eina_boost/mpl/aux_/type_wrapper.hpp>
#   include <eina_boost/mpl/aux_/yes_no.hpp>
#   include <eina_boost/mpl/aux_/config/static_constant.hpp>
#endif

namespace efl_eina_boost { namespace mpl { namespace aux {

#if EFL_EINA_BOOST_WORKAROUND(__EDG_VERSION__, <= 244) && !defined(EFL_EINA_BOOST_INTEL_CXX_VERSION)

EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_rebind, rebind, false)

#elif EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, < 1300)

EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_rebind_impl, rebind, false)

template< typename T >
struct has_rebind
    : if_< 
          msvc_is_class<T>
        , has_rebind_impl<T>
        , bool_<false>
        >::type
{
};

#else // the rest

template< typename T > struct has_rebind_tag {};
no_tag operator|(has_rebind_tag<int>, void const volatile*);

#   if !EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610))
template< typename T >
struct has_rebind
{
    static has_rebind_tag<T>* get();
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = 
          sizeof(has_rebind_tag<int>() | get()) == sizeof(yes_tag)
        );
};
#   else // __BORLANDC__
template< typename T >
struct has_rebind_impl
{
    static T* get();
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = 
          sizeof(has_rebind_tag<int>() | get()) == sizeof(yes_tag)
        );
};

template< typename T >
struct has_rebind
    : if_< 
          is_class<T>
        , has_rebind_impl<T>
        , bool_<false>
        >::type
{
};
#   endif // __BORLANDC__

#endif

}}}

#endif // EFL_EINA_BOOST_MPL_AUX_HAS_REBIND_HPP_INCLUDED
