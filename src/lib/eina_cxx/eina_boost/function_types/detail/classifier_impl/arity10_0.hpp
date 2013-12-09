
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

template< typename R >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,0> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (EFL_EINA_BOOST_FT_nullary_param EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,1> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 , typename T1 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,2> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 , T1 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,3> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 , T1 , T2 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,4> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 , T1 , T2 , T3 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,5> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,6> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,7> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,8> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,9> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,10> ::type
classifier_impl(EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv);

