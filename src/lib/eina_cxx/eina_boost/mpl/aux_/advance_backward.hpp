
#if !defined(EFL_EINA_BOOST_PP_IS_ITERATING)

///// header body

#ifndef EFL_EINA_BOOST_MPL_AUX778076_ADVANCE_BACKWARD_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX778076_ADVANCE_BACKWARD_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: advance_backward.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#if !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)
#   include <eina_boost/mpl/prior.hpp>
#   include <eina_boost/mpl/apply_wrap.hpp>
#endif

#include <eina_boost/mpl/aux_/config/use_preprocessed.hpp>

#if    !defined(EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)

#   define EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER advance_backward.hpp
#   include <eina_boost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <eina_boost/mpl/limits/unrolling.hpp>
#   include <eina_boost/mpl/aux_/nttp_decl.hpp>
#   include <eina_boost/mpl/aux_/config/eti.hpp>

#   include <eina_boost/preprocessor/iterate.hpp>
#   include <eina_boost/preprocessor/cat.hpp>
#   include <eina_boost/preprocessor/inc.hpp>

namespace efl_eina_boost { namespace mpl { namespace aux {

// forward declaration
template< EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(long, N) > struct advance_backward;

#   define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, EFL_EINA_BOOST_MPL_LIMIT_UNROLLING, <boost/mpl/aux_/advance_backward.hpp>))
#   include EFL_EINA_BOOST_PP_ITERATE()

// implementation for N that exceeds EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
template< EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(long, N) >
struct advance_backward
{
    template< typename Iterator > struct apply
    {
        typedef typename apply_wrap1<
              advance_backward<EFL_EINA_BOOST_MPL_LIMIT_UNROLLING>
            , Iterator
            >::type chunk_result_;

        typedef typename apply_wrap1<
              advance_backward<(
                (N - EFL_EINA_BOOST_MPL_LIMIT_UNROLLING) < 0
                    ? 0
                    : N - EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
                    )>
            , chunk_result_
            >::type type;
    };
};

}}}

#endif // EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // EFL_EINA_BOOST_MPL_AUX778076_ADVANCE_BACKWARD_HPP_INCLUDED

///// iteration, depth == 1

// For gcc 4.4 compatability, we must include the
// EFL_EINA_BOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // EFL_EINA_BOOST_PP_IS_ITERATING
#if EFL_EINA_BOOST_PP_ITERATION_DEPTH() == 1
#define i_ EFL_EINA_BOOST_PP_FRAME_ITERATION(1)

template<>
struct advance_backward< EFL_EINA_BOOST_PP_FRAME_ITERATION(1) >
{
    template< typename Iterator > struct apply
    {
        typedef Iterator iter0;

#if i_ > 0
#   define EFL_EINA_BOOST_PP_ITERATION_PARAMS_2 \
    (3,(1, EFL_EINA_BOOST_PP_FRAME_ITERATION(1), <boost/mpl/aux_/advance_backward.hpp>))
#   include EFL_EINA_BOOST_PP_ITERATE()
#endif

        typedef EFL_EINA_BOOST_PP_CAT(iter,EFL_EINA_BOOST_PP_FRAME_ITERATION(1)) type;
    };

#if defined(EFL_EINA_BOOST_MPL_CFG_MSVC_60_ETI_BUG)
    /// ETI workaround
    template<> struct apply<int>
    {
        typedef int type;
    };
#endif
};

#undef i_

///// iteration, depth == 2

#elif EFL_EINA_BOOST_PP_ITERATION_DEPTH() == 2

#   define AUX778076_ITER_0 EFL_EINA_BOOST_PP_CAT(iter,EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_FRAME_ITERATION(2)))
#   define AUX778076_ITER_1 EFL_EINA_BOOST_PP_CAT(iter,EFL_EINA_BOOST_PP_FRAME_ITERATION(2))

        typedef typename prior<AUX778076_ITER_0>::type AUX778076_ITER_1;
        
#   undef AUX778076_ITER_1
#   undef AUX778076_ITER_0

#endif // EFL_EINA_BOOST_PP_ITERATION_DEPTH()
#endif // EFL_EINA_BOOST_PP_IS_ITERATING
