/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(EFL_EINA_BOOST_FUSION_ITERATOR_RANGE_SIZE_IMPL_HPP_INCLUDED)
#define EFL_EINA_BOOST_FUSION_ITERATOR_RANGE_SIZE_IMPL_HPP_INCLUDED

#include <eina_boost/fusion/iterator/distance.hpp>

namespace efl_eina_boost { namespace fusion
{
    struct iterator_range_tag;

    namespace extension
    {
        template <typename Tag>
        struct size_impl;

        template <>
        struct size_impl<iterator_range_tag>
        {
            template <typename Seq>
            struct apply
              : result_of::distance<
                    typename Seq::begin_type,
                    typename Seq::end_type
                >
            {};
        };
    }
}}

#endif

