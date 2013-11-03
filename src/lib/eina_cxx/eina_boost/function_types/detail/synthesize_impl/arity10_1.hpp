
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

# define EFL_EINA_BOOST_FT_make_type(flags,cc,arity) EFL_EINA_BOOST_FT_make_type_impl(flags,cc,arity)
# define EFL_EINA_BOOST_FT_make_type_impl(flags,cc,arity) make_type_ ## flags ## _ ## cc ## _ ## arity
template< typename R , typename T0 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,1)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) ( EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 2 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,1) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
> ::type type;
};
};
template< typename R , typename T0 , typename T1 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,2)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) (T1 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 3 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
typedef typename mpl::next< iter_1 > ::type iter_2;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,2) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
, typename mpl::deref< iter_2 > ::type
> ::type type;
};
};
template< typename R , typename T0 , typename T1 , typename T2 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,3)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 4 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
typedef typename mpl::next< iter_1 > ::type iter_2;
typedef typename mpl::next< iter_2 > ::type iter_3;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,3) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
, typename mpl::deref< iter_2 > ::type
, typename mpl::deref< iter_3 > ::type
> ::type type;
};
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,4)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 5 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
typedef typename mpl::next< iter_1 > ::type iter_2;
typedef typename mpl::next< iter_2 > ::type iter_3;
typedef typename mpl::next< iter_3 > ::type iter_4;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,4) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
, typename mpl::deref< iter_2 > ::type
, typename mpl::deref< iter_3 > ::type
, typename mpl::deref< iter_4 > ::type
> ::type type;
};
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,5)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 6 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
typedef typename mpl::next< iter_1 > ::type iter_2;
typedef typename mpl::next< iter_2 > ::type iter_3;
typedef typename mpl::next< iter_3 > ::type iter_4;
typedef typename mpl::next< iter_4 > ::type iter_5;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,5) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
, typename mpl::deref< iter_2 > ::type
, typename mpl::deref< iter_3 > ::type
, typename mpl::deref< iter_4 > ::type
, typename mpl::deref< iter_5 > ::type
> ::type type;
};
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,6)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 7 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
typedef typename mpl::next< iter_1 > ::type iter_2;
typedef typename mpl::next< iter_2 > ::type iter_3;
typedef typename mpl::next< iter_3 > ::type iter_4;
typedef typename mpl::next< iter_4 > ::type iter_5;
typedef typename mpl::next< iter_5 > ::type iter_6;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,6) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
, typename mpl::deref< iter_2 > ::type
, typename mpl::deref< iter_3 > ::type
, typename mpl::deref< iter_4 > ::type
, typename mpl::deref< iter_5 > ::type
, typename mpl::deref< iter_6 > ::type
> ::type type;
};
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,7)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 8 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
typedef typename mpl::next< iter_1 > ::type iter_2;
typedef typename mpl::next< iter_2 > ::type iter_3;
typedef typename mpl::next< iter_3 > ::type iter_4;
typedef typename mpl::next< iter_4 > ::type iter_5;
typedef typename mpl::next< iter_5 > ::type iter_6;
typedef typename mpl::next< iter_6 > ::type iter_7;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,7) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
, typename mpl::deref< iter_2 > ::type
, typename mpl::deref< iter_3 > ::type
, typename mpl::deref< iter_4 > ::type
, typename mpl::deref< iter_5 > ::type
, typename mpl::deref< iter_6 > ::type
, typename mpl::deref< iter_7 > ::type
> ::type type;
};
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,8)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 9 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
typedef typename mpl::next< iter_1 > ::type iter_2;
typedef typename mpl::next< iter_2 > ::type iter_3;
typedef typename mpl::next< iter_3 > ::type iter_4;
typedef typename mpl::next< iter_4 > ::type iter_5;
typedef typename mpl::next< iter_5 > ::type iter_6;
typedef typename mpl::next< iter_6 > ::type iter_7;
typedef typename mpl::next< iter_7 > ::type iter_8;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,8) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
, typename mpl::deref< iter_2 > ::type
, typename mpl::deref< iter_3 > ::type
, typename mpl::deref< iter_4 > ::type
, typename mpl::deref< iter_5 > ::type
, typename mpl::deref< iter_6 > ::type
, typename mpl::deref< iter_7 > ::type
, typename mpl::deref< iter_8 > ::type
> ::type type;
};
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,9)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 10 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
typedef typename mpl::next< iter_1 > ::type iter_2;
typedef typename mpl::next< iter_2 > ::type iter_3;
typedef typename mpl::next< iter_3 > ::type iter_4;
typedef typename mpl::next< iter_4 > ::type iter_5;
typedef typename mpl::next< iter_5 > ::type iter_6;
typedef typename mpl::next< iter_6 > ::type iter_7;
typedef typename mpl::next< iter_7 > ::type iter_8;
typedef typename mpl::next< iter_8 > ::type iter_9;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,9) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
, typename mpl::deref< iter_2 > ::type
, typename mpl::deref< iter_3 > ::type
, typename mpl::deref< iter_4 > ::type
, typename mpl::deref< iter_5 > ::type
, typename mpl::deref< iter_6 > ::type
, typename mpl::deref< iter_7 > ::type
, typename mpl::deref< iter_8 > ::type
, typename mpl::deref< iter_9 > ::type
> ::type type;
};
};
template< typename R , typename T0 , typename T1 , typename T2 , typename T3 , typename T4 , typename T5 , typename T6 , typename T7 , typename T8 , typename T9 >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,10)
{
typedef EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,type EFL_EINA_BOOST_PP_EMPTY) (T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv ;
};
template< > 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, 11 > 
{ 
template<typename S> struct synthesize_impl_i
{
private:
typedef typename mpl::begin<S> ::type iter_0;
typedef typename mpl::next< iter_0 > ::type iter_1;
typedef typename mpl::next< iter_1 > ::type iter_2;
typedef typename mpl::next< iter_2 > ::type iter_3;
typedef typename mpl::next< iter_3 > ::type iter_4;
typedef typename mpl::next< iter_4 > ::type iter_5;
typedef typename mpl::next< iter_5 > ::type iter_6;
typedef typename mpl::next< iter_6 > ::type iter_7;
typedef typename mpl::next< iter_7 > ::type iter_8;
typedef typename mpl::next< iter_8 > ::type iter_9;
typedef typename mpl::next< iter_9 > ::type iter_10;
public:
typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,10) 
< typename mpl::deref< iter_0 > ::type 
, typename detail::cv_traits< 
typename mpl::deref< iter_1 > ::type > ::type
, typename mpl::deref< iter_2 > ::type
, typename mpl::deref< iter_3 > ::type
, typename mpl::deref< iter_4 > ::type
, typename mpl::deref< iter_5 > ::type
, typename mpl::deref< iter_6 > ::type
, typename mpl::deref< iter_7 > ::type
, typename mpl::deref< iter_8 > ::type
, typename mpl::deref< iter_9 > ::type
, typename mpl::deref< iter_10 > ::type
> ::type type;
};
};
# undef EFL_EINA_BOOST_FT_make_type
# undef EFL_EINA_BOOST_FT_make_type_impl

