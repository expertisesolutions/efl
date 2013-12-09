/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(EFL_EINA_BOOST_FUSION_ITERATOR_RANGE_SEGMENTS_HPP_INCLUDED)
#define EFL_EINA_BOOST_FUSION_ITERATOR_RANGE_SEGMENTS_HPP_INCLUDED

#include <eina_boost/mpl/assert.hpp>
#include <eina_boost/fusion/sequence/intrinsic/segments.hpp>
#include <eina_boost/fusion/support/is_segmented.hpp>
#include <eina_boost/fusion/view/iterator_range/detail/segmented_iterator_range.hpp>

namespace efl_eina_boost { namespace fusion
{
    struct iterator_range_tag;

    namespace extension
    {
        template <typename Tag>
        struct segments_impl;

        template <>
        struct segments_impl<iterator_range_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef
                    detail::make_segmented_range<
                        typename Sequence::begin_type
                      , typename Sequence::end_type
                    >
                impl;

                EFL_EINA_BOOST_MPL_ASSERT((traits::is_segmented<typename impl::type>));

                typedef
                    typename result_of::segments<typename impl::type>::type
                type;

                static type call(Sequence & seq)
                {
                    return fusion::segments(impl::call(seq.first, seq.last));
                }
            };
        };
    }
}}

#endif
