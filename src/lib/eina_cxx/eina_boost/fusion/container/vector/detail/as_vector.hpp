/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef EFL_EINA_BOOST_PP_IS_ITERATING
#if !defined(FUSION_AS_VECTOR_09222005_0950)
#define FUSION_AS_VECTOR_09222005_0950

#include <eina_boost/preprocessor/iterate.hpp>
#include <eina_boost/preprocessor/repetition/enum_params.hpp>
#include <eina_boost/preprocessor/repetition/enum_binary_params.hpp>
#include <eina_boost/preprocessor/repetition/repeat.hpp>
#include <eina_boost/preprocessor/cat.hpp>
#include <eina_boost/preprocessor/inc.hpp>
#include <eina_boost/preprocessor/dec.hpp>
#include <eina_boost/fusion/container/vector/vector.hpp>
#include <eina_boost/fusion/iterator/value_of.hpp>
#include <eina_boost/fusion/iterator/deref.hpp>
#include <eina_boost/fusion/iterator/next.hpp>

namespace efl_eina_boost { namespace fusion { namespace detail
{
    template <int size>
    struct as_vector;

    template <>
    struct as_vector<0>
    {
        template <typename Iterator>
        struct apply
        {
            typedef vector0<> type;
        };

        template <typename Iterator>
        static typename apply<Iterator>::type
        call(Iterator)
        {
            return vector0<>();
        }
    };
}}}

#if !defined(EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <eina_boost/fusion/container/vector/detail/preprocessed/as_vector.hpp>
#else
#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/as_vector" FUSION_MAX_VECTOR_SIZE_STR ".hpp")
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

#define EFL_EINA_BOOST_FUSION_NEXT_ITERATOR(z, n, data)                                  \
    typedef typename fusion::result_of::next<EFL_EINA_BOOST_PP_CAT(I, n)>::type          \
        EFL_EINA_BOOST_PP_CAT(I, EFL_EINA_BOOST_PP_INC(n));

#define EFL_EINA_BOOST_FUSION_NEXT_CALL_ITERATOR(z, n, data)                             \
    typename gen::EFL_EINA_BOOST_PP_CAT(I, EFL_EINA_BOOST_PP_INC(n))                              \
        EFL_EINA_BOOST_PP_CAT(i, EFL_EINA_BOOST_PP_INC(n)) = fusion::next(EFL_EINA_BOOST_PP_CAT(i, n));

#define EFL_EINA_BOOST_FUSION_VALUE_OF_ITERATOR(z, n, data)                              \
    typedef typename fusion::result_of::value_of<EFL_EINA_BOOST_PP_CAT(I, n)>::type      \
        EFL_EINA_BOOST_PP_CAT(T, n);

#define EFL_EINA_BOOST_PP_FILENAME_1 <eina_boost/fusion/container/vector/detail/as_vector.hpp>
#define EFL_EINA_BOOST_PP_ITERATION_LIMITS (1, FUSION_MAX_VECTOR_SIZE)
#include EFL_EINA_BOOST_PP_ITERATE()

#undef EFL_EINA_BOOST_FUSION_NEXT_ITERATOR
#undef EFL_EINA_BOOST_FUSION_NEXT_CALL_ITERATOR
#undef EFL_EINA_BOOST_FUSION_VALUE_OF_ITERATOR

}}}

#if defined(__WAVE__) && defined(EFL_EINA_BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // EFL_EINA_BOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif
#else // defined(EFL_EINA_BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

#define N EFL_EINA_BOOST_PP_ITERATION()

    template <>
    struct as_vector<N>
    {
        template <typename I0>
        struct apply
        {
            EFL_EINA_BOOST_PP_REPEAT(N, EFL_EINA_BOOST_FUSION_NEXT_ITERATOR, _)
            EFL_EINA_BOOST_PP_REPEAT(N, EFL_EINA_BOOST_FUSION_VALUE_OF_ITERATOR, _)
            typedef EFL_EINA_BOOST_PP_CAT(vector, N)<EFL_EINA_BOOST_PP_ENUM_PARAMS(N, T)> type;
        };

        template <typename Iterator>
        static typename apply<Iterator>::type
        call(Iterator const& i0)
        {
            typedef apply<Iterator> gen;
            typedef typename gen::type result;
            EFL_EINA_BOOST_PP_REPEAT(EFL_EINA_BOOST_PP_DEC(N), EFL_EINA_BOOST_FUSION_NEXT_CALL_ITERATOR, _)
            return result(EFL_EINA_BOOST_PP_ENUM_PARAMS(N, *i));
        }
    };

#undef N
#endif // defined(EFL_EINA_BOOST_PP_IS_ITERATING)

