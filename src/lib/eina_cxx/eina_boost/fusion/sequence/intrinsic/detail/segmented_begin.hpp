/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(EFL_EINA_BOOST_FUSION_SEGMENTED_BEGIN_HPP_INCLUDED)
#define EFL_EINA_BOOST_FUSION_SEGMENTED_BEGIN_HPP_INCLUDED

#include <eina_boost/fusion/sequence/intrinsic/detail/segmented_begin_impl.hpp>
#include <eina_boost/fusion/iterator/segmented_iterator.hpp>
#include <eina_boost/fusion/view/iterator_range.hpp>
#include <eina_boost/fusion/sequence/intrinsic/begin.hpp>
#include <eina_boost/fusion/sequence/intrinsic/end.hpp>
#include <eina_boost/fusion/sequence/intrinsic/empty.hpp>
#include <eina_boost/fusion/container/list/cons.hpp>

namespace efl_eina_boost { namespace fusion { namespace detail
{
    //auto segmented_begin( seq )
    //{
    //    return make_segmented_iterator( segmented_begin_impl( seq, nil_ ) );
    //}

    template <typename Sequence, typename Nil_ = fusion::nil_>
    struct segmented_begin
    {
        typedef
            segmented_iterator<
                typename segmented_begin_impl<Sequence, Nil_>::type
            >
        type;

        static type call(Sequence& seq)
        {
            return type(
                segmented_begin_impl<Sequence, Nil_>::call(seq, Nil_()));
        }
    };

}}}

#endif
