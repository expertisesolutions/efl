/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_END_10022005_1619)
#define FUSION_END_10022005_1619

#include <eina_boost/mpl/begin_end.hpp>
#include <eina_boost/fusion/sequence/intrinsic/end.hpp>
#include <eina_boost/fusion/adapted/mpl/detail/end_impl.hpp>
#include <eina_boost/fusion/iterator/mpl/fusion_iterator.hpp>

namespace efl_eina_boost { namespace mpl
{
    template <typename Tag>
    struct end_impl;

    template <>
    struct end_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence>
        struct apply
        {
            typedef fusion_iterator<typename fusion::result_of::end<Sequence>::type> type;
        };
    };
}}

#endif
