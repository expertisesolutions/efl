/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EFL_EINA_BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_VALUE_OF_DATA_IMPL_HPP
#define EFL_EINA_BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_VALUE_OF_DATA_IMPL_HPP

#include <eina_boost/fusion/iterator/value_of_data.hpp>

namespace efl_eina_boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_of_data_impl;

    template <>
    struct value_of_data_impl<joint_view_iterator_tag>
    {
        template <typename It>
        struct apply
          : result_of::value_of_data<typename It::first_type>
        {};
    };
}}}

#endif
