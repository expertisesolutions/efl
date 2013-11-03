/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(EFL_EINA_BOOST_FUSION_SEGMENTED_END_IMPL_HPP_INCLUDED)
#define EFL_EINA_BOOST_FUSION_SEGMENTED_END_IMPL_HPP_INCLUDED

#include <eina_boost/mpl/assert.hpp>
#include <eina_boost/type_traits/add_const.hpp>
#include <eina_boost/type_traits/remove_reference.hpp>
#include <eina_boost/fusion/sequence/intrinsic_fwd.hpp>
#include <eina_boost/fusion/container/list/cons_fwd.hpp>
#include <eina_boost/fusion/support/is_segmented.hpp>

namespace efl_eina_boost { namespace fusion
{
    template <typename First, typename Last>
    struct iterator_range;
}}

namespace efl_eina_boost { namespace fusion { namespace detail
{
    //auto segmented_end_impl( seq, stack )
    //{
    //    assert(is_segmented(seq));
    //    auto it = end(segments(seq));
    //    return cons(iterator_range(it, it), stack);
    //}

    template <typename Sequence, typename Stack>
    struct segmented_end_impl
    {
        EFL_EINA_BOOST_MPL_ASSERT((traits::is_segmented<Sequence>));

        typedef
            typename result_of::end<
                typename remove_reference<
                    typename add_const<
                        typename result_of::segments<Sequence>::type
                    >::type
                >::type
            >::type
        end_type;

        typedef iterator_range<end_type, end_type>  pair_type;
        typedef cons<pair_type, Stack>              type;

        static type call(Sequence & seq, Stack stack)
        {
            end_type end = fusion::end(fusion::segments(seq));
            return type(pair_type(end, end), stack);
        }
    };

}}}

#endif
