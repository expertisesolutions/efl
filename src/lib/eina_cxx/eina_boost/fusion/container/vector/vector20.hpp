/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_VECTOR20_05052005_0205)
#define FUSION_VECTOR20_05052005_0205

#include <eina_boost/fusion/container/vector/vector20_fwd.hpp>
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
#include <eina_boost/mpl/at.hpp>
#include <eina_boost/mpl/bool.hpp>
#include <eina_boost/mpl/vector/vector20.hpp>
#include <eina_boost/type_traits/is_convertible.hpp>
#include <eina_boost/utility/enable_if.hpp>

#include <eina_boost/preprocessor/dec.hpp>
#include <eina_boost/preprocessor/iteration/iterate.hpp>
#include <eina_boost/preprocessor/repetition/enum.hpp>
#include <eina_boost/preprocessor/repetition/enum_shifted.hpp>
#include <eina_boost/preprocessor/repetition/enum_params.hpp>
#include <eina_boost/preprocessor/repetition/enum_binary_params.hpp>
#include <eina_boost/preprocessor/repetition/repeat_from_to.hpp>

#if !defined(EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <eina_boost/fusion/container/vector/detail/preprocessed/vector20.hpp>
#else
#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "detail/preprocessed/vector20.hpp")
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
    struct vector_tag;
    struct fusion_sequence_tag;
    struct random_access_traversal_tag;

// expand vector11 to vector20
#define EFL_EINA_BOOST_PP_FILENAME_1 <eina_boost/fusion/container/vector/detail/vector_n.hpp>
#define EFL_EINA_BOOST_PP_ITERATION_LIMITS (11, 20)
#include EFL_EINA_BOOST_PP_ITERATE()

}}

#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif

