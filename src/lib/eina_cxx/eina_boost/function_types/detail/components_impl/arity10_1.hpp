
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

template< typename R , typename T0, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) ( EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) ( EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,1> function_arity;
typedef mpl::vector2< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type > types;
};
template< typename R , typename T0 , typename T1, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,2> function_arity;
typedef mpl::vector3< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 > types;
};
template< typename R , typename T0 , typename T1 , typename T2, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,3> function_arity;
typedef mpl::vector4< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,4> function_arity;
typedef mpl::vector5< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,5> function_arity;
typedef mpl::vector6< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,6> function_arity;
typedef mpl::vector7< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,7> function_arity;
typedef mpl::vector8< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,8> function_arity;
typedef mpl::vector9< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,9> function_arity;
typedef mpl::vector10< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 > types;
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9, typename L>
struct components_impl<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L>
{
typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;
typedef function_types::components<EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc, EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv, L> type;
typedef components_mpl_sequence_tag tag;
typedef mpl::integral_c<std::size_t,10> function_arity;
typedef mpl::vector11< R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L> ::type , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 > types;
};

