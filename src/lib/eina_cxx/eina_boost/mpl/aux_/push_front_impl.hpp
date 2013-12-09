
#ifndef EFL_EINA_BOOST_MPL_AUX_PUSH_FRONT_IMPL_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_PUSH_FRONT_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2008
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: push_front_impl.hpp 55679 2009-08-20 07:50:16Z agurtovoy $
// $Date: 2009-08-20 00:50:16 -0700 (Thu, 20 Aug 2009) $
// $Revision: 55679 $

#include <eina_boost/mpl/push_front_fwd.hpp>
#include <eina_boost/mpl/assert.hpp>
#include <eina_boost/mpl/aux_/has_type.hpp>
#include <eina_boost/mpl/aux_/traits_lambda_spec.hpp>
#include <eina_boost/mpl/aux_/config/forwarding.hpp>
#include <eina_boost/mpl/aux_/config/static_constant.hpp>

#include <eina_boost/type_traits/is_same.hpp>

namespace efl_eina_boost { namespace mpl {

struct has_push_front_arg {};

// agurt 05/feb/04: no default implementation; the stub definition is needed 
// to enable the default 'has_push_front' implementation below

template< typename Tag >
struct push_front_impl
{
    template< typename Sequence, typename T > struct apply
    {
        // should be instantiated only in the context of 'has_push_front_impl';
        // if you've got an assert here, you are requesting a 'push_front' 
        // specialization that doesn't exist.
        EFL_EINA_BOOST_MPL_ASSERT_MSG(
              ( efl_eina_boost::is_same< T, has_push_front_arg >::value )
            , REQUESTED_PUSH_FRONT_SPECIALIZATION_FOR_SEQUENCE_DOES_NOT_EXIST
            , ( Sequence )
            );
    };
};

template< typename Tag >
struct has_push_front_impl
{
    template< typename Seq > struct apply
#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_NESTED_FORWARDING)
        : aux::has_type< push_front< Seq, has_push_front_arg > >
    {
#else
    {
        typedef aux::has_type< push_front< Seq, has_push_front_arg > > type;
        EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = 
              (aux::has_type< push_front< Seq, has_push_front_arg > >::value)
            );
#endif
    };
};

EFL_EINA_BOOST_MPL_ALGORITM_TRAITS_LAMBDA_SPEC(2, push_front_impl)
EFL_EINA_BOOST_MPL_ALGORITM_TRAITS_LAMBDA_SPEC(1, has_push_front_impl)

}}

#endif // EFL_EINA_BOOST_MPL_AUX_PUSH_FRONT_IMPL_HPP_INCLUDED
