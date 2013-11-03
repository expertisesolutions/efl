/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(EFL_EINA_BOOST_FUSION_BEGIN_IMPL_31122005_1209)
#define EFL_EINA_BOOST_FUSION_BEGIN_IMPL_31122005_1209

#include <eina_boost/fusion/adapted/mpl/mpl_iterator.hpp>
#include <eina_boost/mpl/begin.hpp>
#include <eina_boost/type_traits/remove_const.hpp>

namespace efl_eina_boost { namespace fusion {
    
    struct mpl_sequence_tag;

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<mpl_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef typename mpl::begin<
                    typename remove_const<Sequence>::type
                >::type iterator;
                typedef mpl_iterator<iterator> type;
                
                static type
                call(Sequence)
                {
                    return type();
                }
            };
        };
    }
}}

#endif
