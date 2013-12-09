/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_BEGIN_10022005_1620)
#define FUSION_BEGIN_10022005_1620

#include <eina_boost/mpl/begin_end.hpp>
#include <eina_boost/fusion/sequence/intrinsic/begin.hpp>
#include <eina_boost/fusion/adapted/mpl/detail/begin_impl.hpp>
#include <eina_boost/fusion/iterator/mpl/fusion_iterator.hpp>

namespace efl_eina_boost { namespace mpl
{
    template <typename Tag>
    struct begin_impl;

    template <>
    struct begin_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence>
        struct apply
        {
            typedef fusion_iterator<typename fusion::result_of::begin<Sequence>::type> type;
        };
    };
}}

#endif
