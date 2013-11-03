/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef EFL_EINA_BOOST_PP_IS_ITERATING
#if !defined(FUSION_VECTOR_N_CHOOSER_07072005_1248)
#define FUSION_VECTOR_N_CHOOSER_07072005_1248

#include <eina_boost/fusion/container/vector/limits.hpp>

//  include vector0..N where N is FUSION_MAX_VECTOR_SIZE
#include <eina_boost/fusion/container/vector/vector10.hpp>
#if (FUSION_MAX_VECTOR_SIZE > 10)
#include <eina_boost/fusion/container/vector/vector20.hpp>
#endif
#if (FUSION_MAX_VECTOR_SIZE > 20)
#include <eina_boost/fusion/container/vector/vector30.hpp>
#endif
#if (FUSION_MAX_VECTOR_SIZE > 30)
#include <eina_boost/fusion/container/vector/vector40.hpp>
#endif
#if (FUSION_MAX_VECTOR_SIZE > 40)
#include <eina_boost/fusion/container/vector/vector50.hpp>
#endif

#include <eina_boost/preprocessor/cat.hpp>
#include <eina_boost/preprocessor/arithmetic/dec.hpp>
#include <eina_boost/preprocessor/arithmetic/sub.hpp>
#include <eina_boost/preprocessor/facilities/intercept.hpp>
#include <eina_boost/preprocessor/repetition/enum_params.hpp>
#include <eina_boost/preprocessor/repetition/enum_trailing_params.hpp>

namespace efl_eina_boost { namespace fusion
{
    struct void_;
}}

#if !defined(EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <eina_boost/fusion/container/vector/detail/preprocessed/vector_chooser.hpp>
#else
#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/vector_chooser" FUSION_MAX_VECTOR_SIZE_STR ".hpp")
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

namespace efl_eina_boost { namespace fusion { namespace detail
{
    template <EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename T)>
    struct vector_n_chooser
    {
        typedef EFL_EINA_BOOST_PP_CAT(vector, FUSION_MAX_VECTOR_SIZE)<EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)> type;
    };

    template <>
    struct vector_n_chooser<EFL_EINA_BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, void_ EFL_EINA_BOOST_PP_INTERCEPT)>
    {
        typedef vector0<> type;
    };

#define EFL_EINA_BOOST_PP_FILENAME_1 \
    <boost/fusion/container/vector/detail/vector_n_chooser.hpp>
#define EFL_EINA_BOOST_PP_ITERATION_LIMITS (1, EFL_EINA_BOOST_PP_DEC(FUSION_MAX_VECTOR_SIZE))
#include EFL_EINA_BOOST_PP_ITERATE()

}}}

#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(EFL_EINA_BOOST_PP_IS_ITERATING)

#define N EFL_EINA_BOOST_PP_ITERATION()

    template <EFL_EINA_BOOST_PP_ENUM_PARAMS(N, typename T)>
    struct vector_n_chooser<
        EFL_EINA_BOOST_PP_ENUM_PARAMS(N, T)
        EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_SUB(FUSION_MAX_VECTOR_SIZE, N), void_ EFL_EINA_BOOST_PP_INTERCEPT)>
    {
        typedef EFL_EINA_BOOST_PP_CAT(vector, N)<EFL_EINA_BOOST_PP_ENUM_PARAMS(N, T)> type;
    };

#undef N
#endif // defined(EFL_EINA_BOOST_PP_IS_ITERATING)
