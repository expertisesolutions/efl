/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(EFL_EINA_BOOST_FUSION_SEQUENCE_EMPTY_IMPL_HPP_INCLUDED)
#define EFL_EINA_BOOST_FUSION_SEQUENCE_EMPTY_IMPL_HPP_INCLUDED

#include <eina_boost/type_traits/is_convertible.hpp>

namespace efl_eina_boost { namespace fusion
{
    struct cons_tag;

    struct nil_;

    template <typename Car, typename Cdr>
    struct cons;

    namespace extension
    {
        template <typename Tag>
        struct empty_impl;

        template <>
        struct empty_impl<cons_tag>
        {
            template <typename Sequence>
            struct apply
                : efl_eina_boost::is_convertible<Sequence, nil_>
            {};
        };
    }
}}

#endif
