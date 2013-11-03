/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_VECTOR10_05042005_0257)
#define FUSION_VECTOR10_05042005_0257

#include <eina_boost/fusion/container/vector/vector10_fwd.hpp>
#include <eina_boost/fusion/support/sequence_base.hpp>
#include <eina_boost/fusion/support/detail/access.hpp>
#include <eina_boost/fusion/iterator/next.hpp>
#include <eina_boost/fusion/iterator/deref.hpp>
#include <eina_boost/fusion/sequence/intrinsic/begin.hpp>
#include <eina_boost/fusion/container/vector/detail/at_impl.hpp>
#include <eina_boost/fusion/container/vector/detail/value_at_impl.hpp>
#include <eina_boost/fusion/container/vector/detail/begin_impl.hpp>
#include <eina_boost/fusion/container/vector/detail/end_impl.hpp>

#include <eina_boost/mpl/void.hpp>
#include <eina_boost/mpl/int.hpp>
#include <eina_boost/mpl/bool.hpp>
#include <eina_boost/mpl/at.hpp>
#include <eina_boost/mpl/vector/vector10.hpp>
#include <eina_boost/type_traits/is_convertible.hpp>
#include <eina_boost/utility/enable_if.hpp>

#include <eina_boost/preprocessor/dec.hpp>
#include <eina_boost/preprocessor/iteration/iterate.hpp>
#include <eina_boost/preprocessor/repetition/enum.hpp>
#include <eina_boost/preprocessor/repetition/enum_shifted.hpp>
#include <eina_boost/preprocessor/repetition/enum_params.hpp>
#include <eina_boost/preprocessor/repetition/enum_binary_params.hpp>
#include <eina_boost/preprocessor/repetition/repeat_from_to.hpp>

namespace efl_eina_boost { namespace fusion
{
    struct vector_tag;
    struct fusion_sequence_tag;
    struct random_access_traversal_tag;

    template <typename Dummy>
    struct vector0 : sequence_base<vector0<Dummy> >
    {
        typedef mpl::vector0<> types;
        typedef vector_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;
        typedef random_access_traversal_tag category;
        typedef mpl::int_<0> size;

        vector0() {}

        template<typename Sequence>
        vector0(Sequence const& /*seq*/)
        {}
    };
}}

#if !defined(EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <eina_boost/fusion/container/vector/detail/preprocessed/vector10.hpp>
#else
#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "detail/preprocessed/vector10.hpp")
#endif

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is an auto-generated file. Do not edit!
==============================================================================*/

#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

namespace efl_eina_boost { namespace fusion
{

// expand vector1 to vector10
#define EFL_EINA_BOOST_PP_FILENAME_1 <eina_boost/fusion/container/vector/detail/vector_n.hpp>
#define EFL_EINA_BOOST_PP_ITERATION_LIMITS (1, 10)
#include EFL_EINA_BOOST_PP_ITERATE()

}}

#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif
