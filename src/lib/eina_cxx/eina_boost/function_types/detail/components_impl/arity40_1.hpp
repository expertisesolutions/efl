
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

// input:  EFL_EINA_BOOST_FT_syntax     type macro to use
// input:  EFL_EINA_BOOST_FT_cc         empty or cc specifier 
// input:  EFL_EINA_BOOST_FT_ell        empty or "..."
// input:  EFL_EINA_BOOST_FT_cv         empty or cv qualifiers
// input:  EFL_EINA_BOOST_FT_flags      single decimal integer encoding the flags
// output: EFL_EINA_BOOST_FT_n          number of component types (arity+1)
// output: EFL_EINA_BOOST_FT_arity      current arity
// output: EFL_EINA_BOOST_FT_type       macro that expands to the type
// output: EFL_EINA_BOOST_FT_tplargs(p) template arguments with given prefix
// output: EFL_EINA_BOOST_FT_params(p)  parameters with given prefix

# include <eina_boost/function_types/detail/components_impl/arity30_1.hpp>
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,31> function_arity;
typedef mpl::vector32< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30 , typename T31, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,32> function_arity;
typedef mpl::vector33< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30 , typename T31 , typename T32, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,33> function_arity;
typedef mpl::vector34< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30 , typename T31 , typename T32 , typename T33, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,34> function_arity;
typedef mpl::vector35< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30 , typename T31 , typename T32 , typename T33 , typename T34, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,35> function_arity;
typedef mpl::vector36< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30 , typename T31 , typename T32 , typename T33 , typename T34 , typename T35, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,36> function_arity;
typedef mpl::vector37< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30 , typename T31 , typename T32 , typename T33 , typename T34 , typename T35 , typename T36, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,37> function_arity;
typedef mpl::vector38< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30 , typename T31 , typename T32 , typename T33 , typename T34 , typename T35 , typename T36 , typename T37, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 , T37 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 , T37 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,38> function_arity;
typedef mpl::vector39< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 , T37 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30 , typename T31 , typename T32 , typename T33 , typename T34 , typename T35 , typename T36 , typename T37 , typename T38, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 , T37 , T38 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 , T37 , T38 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,39> function_arity;
typedef mpl::vector40< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 , T37 , T38 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 , typename T10 , typename T11 , typename T12 , typename T13 , typename T14 , typename T15 , typename T16 , typename T17 , typename T18 , typename T19 , typename T20 , typename T21 , typename T22 , typename T23 , typename T24 , typename T25 , typename T26 , typename T27 , typename T28 , typename T29 , typename T30 , typename T31 , typename T32 , typename T33 , typename T34 , typename T35 , typename T36 , typename T37 , typename T38 , typename T39, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 , T37 , T38 , T39 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 , T37 , T38 , T39 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,40> function_arity;
typedef mpl::vector41< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19 , T20 , T21 , T22 , T23 , T24 , T25 , T26 , T27 , T28 , T29 , T30 , T31 , T32 , T33 , T34 , T35 , T36 , T37 , T38 , T39 > types;
};

