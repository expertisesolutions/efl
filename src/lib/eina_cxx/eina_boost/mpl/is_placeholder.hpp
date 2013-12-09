
#ifndef EFL_EINA_BOOST_MPL_IS_PLACEHOLDER_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_IS_PLACEHOLDER_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: is_placeholder.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/arg_fwd.hpp>
#include <eina_boost/mpl/bool.hpp>
#include <eina_boost/mpl/aux_/yes_no.hpp>
#include <eina_boost/mpl/aux_/type_wrapper.hpp>
#include <eina_boost/mpl/aux_/nttp_decl.hpp>
#include <eina_boost/mpl/aux_/config/ctps.hpp>
#include <eina_boost/mpl/aux_/config/static_constant.hpp>

namespace efl_eina_boost { namespace mpl {

#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< typename T >
struct is_placeholder
    : bool_<false>
{
};

template< EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(int, N) >
struct is_placeholder< arg<N> >
    : bool_<true>
{
};

#else

namespace aux {

aux::no_tag is_placeholder_helper(...);

template< EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(int, N) >
aux::yes_tag is_placeholder_helper(aux::type_wrapper< arg<N> >*);

} // namespace aux

template< typename T >
struct is_placeholder
{
    static aux::type_wrapper<T>* get();
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = 
          sizeof(aux::is_placeholder_helper(get())) == sizeof(aux::yes_tag)
        );
    
    typedef bool_<value> type;
};

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

}}

#endif // EFL_EINA_BOOST_MPL_IS_PLACEHOLDER_HPP_INCLUDED
