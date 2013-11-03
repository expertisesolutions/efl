/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_CONS_07172005_0843)
#define FUSION_CONS_07172005_0843

#include <eina_boost/fusion/container/list/cons_fwd.hpp>
#include <eina_boost/fusion/support/detail/access.hpp>
#include <eina_boost/fusion/sequence/intrinsic/begin.hpp>
#include <eina_boost/fusion/sequence/intrinsic/end.hpp>
#include <eina_boost/fusion/iterator/next.hpp>
#include <eina_boost/fusion/iterator/deref.hpp>
#include <eina_boost/fusion/container/list/cons_iterator.hpp>
#include <eina_boost/fusion/container/list/detail/begin_impl.hpp>
#include <eina_boost/fusion/container/list/detail/end_impl.hpp>
#include <eina_boost/fusion/container/list/detail/at_impl.hpp>
#include <eina_boost/fusion/container/list/detail/value_at_impl.hpp>
#include <eina_boost/fusion/container/list/detail/empty_impl.hpp>
#include <eina_boost/type_traits/is_convertible.hpp>
#include <eina_boost/utility/enable_if.hpp>
#include <eina_boost/fusion/support/sequence_base.hpp>
#include <eina_boost/mpl/int.hpp>
#include <eina_boost/mpl/bool.hpp>
#include <eina_boost/mpl/or.hpp>

namespace efl_eina_boost { namespace fusion
{
    struct void_;
    struct cons_tag;
    struct forward_traversal_tag;
    struct fusion_sequence_tag;

    struct nil_ : sequence_base<nil_>
    {
        typedef mpl::int_<0> size;
        typedef cons_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;
        typedef forward_traversal_tag category;
        typedef void_ car_type;
        typedef void_ cdr_type;

        nil_() {}

        template <typename Iterator>
        nil_(Iterator const& /*iter*/, mpl::true_ /*this_is_an_iterator*/)
        {}

        template <typename Iterator>
        void assign_from_iter(Iterator const& /*iter*/)
        {
        }
    };

    template <typename Car, typename Cdr /*= nil_*/>
    struct cons : sequence_base<cons<Car, Cdr> >
    {
        typedef mpl::int_<Cdr::size::value+1> size;
        typedef cons_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;
        typedef forward_traversal_tag category;
        typedef Car car_type;
        typedef Cdr cdr_type;

        cons()
            : car(), cdr() {}

        explicit cons(typename detail::call_param<Car>::type in_car)
            : car(in_car), cdr() {}

        cons(
            typename detail::call_param<Car>::type in_car
          , typename detail::call_param<Cdr>::type in_cdr)
            : car(in_car), cdr(in_cdr) {}
        
        template <typename Car2, typename Cdr2>
        cons(cons<Car2, Cdr2> const& rhs)
            : car(rhs.car), cdr(rhs.cdr) {}

        cons(cons const& rhs)
            : car(rhs.car), cdr(rhs.cdr) {}

        template <typename Sequence>
        cons(
            Sequence const& seq
          , typename efl_eina_boost::disable_if<
                mpl::or_<
                    is_convertible<Sequence, cons> // use copy ctor instead
                  , is_convertible<Sequence, Car>  // use copy to car instead
                > 
            >::type* /*dummy*/ = 0
        )
            : car(*fusion::begin(seq))
            , cdr(fusion::next(fusion::begin(seq)), mpl::true_()) {}

        template <typename Iterator>
        cons(Iterator const& iter, mpl::true_ /*this_is_an_iterator*/)
            : car(*iter)
            , cdr(fusion::next(iter), mpl::true_()) {}

        template <typename Car2, typename Cdr2>
        cons& operator=(cons<Car2, Cdr2> const& rhs)
        {
            car = rhs.car;
            cdr = rhs.cdr;
            return *this;
        }

        cons& operator=(cons const& rhs)
        {
            car = rhs.car;
            cdr = rhs.cdr;
            return *this;
        }

        template <typename Sequence>
        typename efl_eina_boost::disable_if<is_convertible<Sequence, Car>, cons&>::type
        operator=(Sequence const& seq)
        {
            typedef typename result_of::begin<Sequence const>::type Iterator;
            Iterator iter = fusion::begin(seq);
            this->assign_from_iter(iter);
            return *this;
        }

        template <typename Iterator>
        void assign_from_iter(Iterator const& iter)
        {
            car = *iter;
            cdr.assign_from_iter(fusion::next(iter));
        }

        car_type car;
        cdr_type cdr;
    };
}}

#endif

