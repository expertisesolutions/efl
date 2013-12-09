
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION!

#if !defined(EFL_EINA_BOOST_PP_IS_ITERATING)

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: reverse_fold_impl_body.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#   include <eina_boost/mpl/limits/unrolling.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/repeat.hpp>
#   include <eina_boost/mpl/aux_/config/ctps.hpp>
#   include <eina_boost/mpl/aux_/nttp_decl.hpp>

#   include <eina_boost/preprocessor/arithmetic/sub.hpp>
#   include <eina_boost/preprocessor/iterate.hpp>
#   include <eina_boost/preprocessor/dec.hpp>
#   include <eina_boost/preprocessor/inc.hpp>
#   include <eina_boost/preprocessor/cat.hpp>

// local macros, #undef-ined at the end of the header

#   define AUX778076_ITER_FOLD_FORWARD_STEP(unused, n_, unused2) \
    typedef typename apply2< \
          ForwardOp \
        , EFL_EINA_BOOST_PP_CAT(fwd_state,n_) \
        , AUX778076_FOLD_IMPL_OP(EFL_EINA_BOOST_PP_CAT(iter,n_)) \
        >::type EFL_EINA_BOOST_PP_CAT(fwd_state,EFL_EINA_BOOST_PP_INC(n_)); \
    typedef typename mpl::next<EFL_EINA_BOOST_PP_CAT(iter,n_)>::type \
        EFL_EINA_BOOST_PP_CAT(iter,EFL_EINA_BOOST_PP_INC(n_)); \
    /**/

#   define AUX778076_ITER_FOLD_BACKWARD_STEP_FUNC(n_) \
    typedef typename apply2< \
          BackwardOp \
        , EFL_EINA_BOOST_PP_CAT(bkwd_state,n_) \
        , AUX778076_FOLD_IMPL_OP(EFL_EINA_BOOST_PP_CAT(iter,EFL_EINA_BOOST_PP_DEC(n_))) \
        >::type EFL_EINA_BOOST_PP_CAT(bkwd_state,EFL_EINA_BOOST_PP_DEC(n_)); \
    /**/

#   define AUX778076_ITER_FOLD_BACKWARD_STEP(unused, n_, j) \
    AUX778076_ITER_FOLD_BACKWARD_STEP_FUNC( \
          EFL_EINA_BOOST_PP_SUB_D(1,j,n_) \
        ) \
    /**/

#   define AUX778076_FIRST_BACKWARD_STATE_TYPEDEF(n_) \
    typedef typename nested_chunk::state EFL_EINA_BOOST_PP_CAT(bkwd_state,n_);
    /**/

#   define AUX778076_FOLD_IMPL_NAME \
    EFL_EINA_BOOST_PP_CAT(AUX778076_FOLD_IMPL_NAME_PREFIX,_impl) \
    /**/

#   define AUX778076_FOLD_CHUNK_NAME \
    EFL_EINA_BOOST_PP_CAT(AUX778076_FOLD_IMPL_NAME_PREFIX,_chunk) \
    /**/

namespace efl_eina_boost { namespace mpl { namespace aux {

/// forward declaration
template<
      EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(long, N)
    , typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct AUX778076_FOLD_IMPL_NAME;

#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
    && !defined(EFL_EINA_BOOST_MPL_CFG_NO_NONTYPE_TEMPLATE_PARTIAL_SPEC)

#   define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, EFL_EINA_BOOST_MPL_LIMIT_UNROLLING, <boost/mpl/aux_/reverse_fold_impl_body.hpp>))
#   include EFL_EINA_BOOST_PP_ITERATE()

// implementation for N that exceeds EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
template<
      EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(long, N)
    , typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct AUX778076_FOLD_IMPL_NAME
{
    typedef First iter0;
    typedef State fwd_state0;

    EFL_EINA_BOOST_MPL_PP_REPEAT(
          EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
        , AUX778076_ITER_FOLD_FORWARD_STEP
        , unused
        )

    typedef AUX778076_FOLD_IMPL_NAME<
          ( (N - EFL_EINA_BOOST_MPL_LIMIT_UNROLLING) < 0 ? 0 : N - EFL_EINA_BOOST_MPL_LIMIT_UNROLLING )
        , EFL_EINA_BOOST_PP_CAT(iter,EFL_EINA_BOOST_MPL_LIMIT_UNROLLING)
        , Last
        , EFL_EINA_BOOST_PP_CAT(fwd_state,EFL_EINA_BOOST_MPL_LIMIT_UNROLLING)
        , BackwardOp
        , ForwardOp
        > nested_chunk;
        
    AUX778076_FIRST_BACKWARD_STATE_TYPEDEF(EFL_EINA_BOOST_MPL_LIMIT_UNROLLING)

    EFL_EINA_BOOST_MPL_PP_REPEAT(
          EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
        , AUX778076_ITER_FOLD_BACKWARD_STEP
        , EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
        )

    typedef bkwd_state0 state;
    typedef typename nested_chunk::iterator iterator;
};

// fallback implementation for sequences of unknown size
template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct AUX778076_FOLD_IMPL_NAME<-1,First,Last,State,BackwardOp,ForwardOp>
{
    typedef AUX778076_FOLD_IMPL_NAME<
          -1
        , typename mpl::next<First>::type
        , Last
        , typename apply2<ForwardOp,State,AUX778076_FOLD_IMPL_OP(First)>::type
        , BackwardOp
        , ForwardOp
        > nested_step;

    typedef typename apply2<
          BackwardOp
        , typename nested_step::state
        , AUX778076_FOLD_IMPL_OP(First)
        >::type state;

    typedef typename nested_step::iterator iterator;
};

template<
      typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct AUX778076_FOLD_IMPL_NAME<-1,Last,Last,State,BackwardOp,ForwardOp>
{
    typedef State state;
    typedef Last iterator;
};

#else // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template< EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(long, N) >
struct AUX778076_FOLD_CHUNK_NAME;

#   define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, EFL_EINA_BOOST_MPL_LIMIT_UNROLLING, <boost/mpl/aux_/reverse_fold_impl_body.hpp>))
#   include EFL_EINA_BOOST_PP_ITERATE()

// implementation for N that exceeds EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
template< EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(long, N) > 
struct AUX778076_FOLD_CHUNK_NAME
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        > 
    struct result_
    {
        typedef First iter0;
        typedef State fwd_state0;

        EFL_EINA_BOOST_MPL_PP_REPEAT(
              EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
            , AUX778076_ITER_FOLD_FORWARD_STEP
            , unused
            )

        typedef AUX778076_FOLD_IMPL_NAME<
              ( (N - EFL_EINA_BOOST_MPL_LIMIT_UNROLLING) < 0 ? 0 : N - EFL_EINA_BOOST_MPL_LIMIT_UNROLLING )
            , EFL_EINA_BOOST_PP_CAT(iter,EFL_EINA_BOOST_MPL_LIMIT_UNROLLING)
            , Last
            , EFL_EINA_BOOST_PP_CAT(fwd_state,EFL_EINA_BOOST_MPL_LIMIT_UNROLLING)
            , BackwardOp
            , ForwardOp
            > nested_chunk;
            
        AUX778076_FIRST_BACKWARD_STATE_TYPEDEF(EFL_EINA_BOOST_MPL_LIMIT_UNROLLING)

        EFL_EINA_BOOST_MPL_PP_REPEAT(
              EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
            , AUX778076_ITER_FOLD_BACKWARD_STEP
            , EFL_EINA_BOOST_MPL_LIMIT_UNROLLING
            )

        typedef bkwd_state0 state;
        typedef typename nested_chunk::iterator iterator;
    };
};

// fallback implementation for sequences of unknown size
template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct EFL_EINA_BOOST_PP_CAT(AUX778076_FOLD_IMPL_NAME_PREFIX,_step);

template<
      typename Last
    , typename State
    >
struct EFL_EINA_BOOST_PP_CAT(AUX778076_FOLD_IMPL_NAME_PREFIX,_null_step)
{
    typedef Last iterator;
    typedef State state;
};

template<> 
struct AUX778076_FOLD_CHUNK_NAME<-1>
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        > 
    struct result_
    {
        typedef typename if_<
              typename is_same<First,Last>::type
            , EFL_EINA_BOOST_PP_CAT(AUX778076_FOLD_IMPL_NAME_PREFIX,_null_step)<Last,State>
            , EFL_EINA_BOOST_PP_CAT(AUX778076_FOLD_IMPL_NAME_PREFIX,_step)<First,Last,State,BackwardOp,ForwardOp>
            >::type res_;

        typedef typename res_::state state;
        typedef typename res_::iterator iterator;
    };

#if defined(EFL_EINA_BOOST_MPL_CFG_MSVC_60_ETI_BUG)
    /// ETI workaround
    template<> struct result_<int,int,int,int,int>
    {
        typedef int state;
        typedef int iterator;
    };
#endif
};

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct EFL_EINA_BOOST_PP_CAT(AUX778076_FOLD_IMPL_NAME_PREFIX,_step)
{
    typedef AUX778076_FOLD_CHUNK_NAME<-1>::template result_<
          typename mpl::next<First>::type
        , Last
        , typename apply2<ForwardOp,State,AUX778076_FOLD_IMPL_OP(First)>::type
        , BackwardOp
        , ForwardOp
        > nested_step;

    typedef typename apply2<
          BackwardOp
        , typename nested_step::state
        , AUX778076_FOLD_IMPL_OP(First)
        >::type state;

    typedef typename nested_step::iterator iterator;
};

template<
      EFL_EINA_BOOST_MPL_AUX_NTTP_DECL(long, N)
    , typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    > 
struct AUX778076_FOLD_IMPL_NAME
    : AUX778076_FOLD_CHUNK_NAME<N>
        ::template result_<First,Last,State,BackwardOp,ForwardOp>
{
};

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

}}}

#   undef AUX778076_FIRST_BACKWARD_STATE_TYPEDEF
#   undef AUX778076_ITER_FOLD_BACKWARD_STEP
#   undef AUX778076_ITER_FOLD_BACKWARD_STEP_FUNC
#   undef AUX778076_ITER_FOLD_FORWARD_STEP

#undef AUX778076_FOLD_IMPL_OP
#undef AUX778076_FOLD_IMPL_NAME_PREFIX

///// iteration

#else

#   define n_ EFL_EINA_BOOST_PP_FRAME_ITERATION(1)

#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
    && !defined(EFL_EINA_BOOST_MPL_CFG_NO_NONTYPE_TEMPLATE_PARTIAL_SPEC)

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct AUX778076_FOLD_IMPL_NAME<n_,First,Last,State,BackwardOp,ForwardOp>
{
    typedef First iter0;
    typedef State fwd_state0;

    EFL_EINA_BOOST_MPL_PP_REPEAT(
          n_
        , AUX778076_ITER_FOLD_FORWARD_STEP
        , unused
        )

    typedef EFL_EINA_BOOST_PP_CAT(fwd_state,n_) EFL_EINA_BOOST_PP_CAT(bkwd_state,n_);

    EFL_EINA_BOOST_MPL_PP_REPEAT(
          n_
        , AUX778076_ITER_FOLD_BACKWARD_STEP
        , n_
        )

    typedef bkwd_state0 state;
    typedef EFL_EINA_BOOST_PP_CAT(iter,n_) iterator;
};

#else

template<> struct AUX778076_FOLD_CHUNK_NAME<n_>
{
    template<
          typename First
        , typename Last
        , typename State
        , typename BackwardOp
        , typename ForwardOp
        >
    struct result_
    {
        typedef First iter0;
        typedef State fwd_state0;

        EFL_EINA_BOOST_MPL_PP_REPEAT(
              n_
            , AUX778076_ITER_FOLD_FORWARD_STEP
            , unused
            )

        typedef EFL_EINA_BOOST_PP_CAT(fwd_state,n_) EFL_EINA_BOOST_PP_CAT(bkwd_state,n_);

        EFL_EINA_BOOST_MPL_PP_REPEAT(
              n_
            , AUX778076_ITER_FOLD_BACKWARD_STEP
            , n_
            )

        typedef bkwd_state0 state;
        typedef EFL_EINA_BOOST_PP_CAT(iter,n_) iterator;
    };

#if defined(EFL_EINA_BOOST_MPL_CFG_MSVC_60_ETI_BUG)
    /// ETI workaround
    template<> struct result_<int,int,int,int,int>
    {
        typedef int state;
        typedef int iterator;
    };
#endif
};

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#   undef n_

#endif // EFL_EINA_BOOST_PP_IS_ITERATING
