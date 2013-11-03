
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

#if   EFL_EINA_BOOST_FT_ARITY_LOOP_PREFIX

#   ifndef EFL_EINA_BOOST_FT_DETAIL_SYNTHESIZE_IMPL_MASTER_HPP_INCLUDED
#   define EFL_EINA_BOOST_FT_DETAIL_SYNTHESIZE_IMPL_MASTER_HPP_INCLUDED
#     include <eina_boost/preprocessor/cat.hpp>
#     include <eina_boost/preprocessor/arithmetic/dec.hpp>
#     include <eina_boost/preprocessor/iteration/local.hpp>
#     include <eina_boost/preprocessor/facilities/empty.hpp>
#     include <eina_boost/preprocessor/facilities/identity.hpp>
#   endif

#   define EFL_EINA_BOOST_FT_type_name type

#   ifdef EFL_EINA_BOOST_FT_flags
#     define EFL_EINA_BOOST_FT_make_type(flags,cc,arity) EFL_EINA_BOOST_FT_make_type_impl(flags,cc,arity)
#     define EFL_EINA_BOOST_FT_make_type_impl(flags,cc,arity) make_type_ ## flags ## _ ## cc ## _ ## arity
#   else
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_make_type(flags,cc,arity) EFL_EINA_BOOST_FT_make_type_impl(flags,cc,arity)
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_make_type_impl(flags,cc,arity) make_type_ ## flags ## _ ## cc ## _ ## arity
#   endif

#   define EFL_EINA_BOOST_FT_iter(i) EFL_EINA_BOOST_PP_CAT(iter_,i)

#elif EFL_EINA_BOOST_FT_ARITY_LOOP_IS_ITERATING

template< EFL_EINA_BOOST_FT_tplargs(EFL_EINA_BOOST_PP_IDENTITY(typename)) >
struct EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,EFL_EINA_BOOST_FT_arity)
{
  typedef EFL_EINA_BOOST_FT_type ;
};

template<> 
struct synthesize_impl_o< EFL_EINA_BOOST_FT_flags, EFL_EINA_BOOST_FT_cc_id, EFL_EINA_BOOST_FT_n > 
{ 
  template<typename S> struct synthesize_impl_i
  {
  private:
    typedef typename mpl::begin<S>::type EFL_EINA_BOOST_FT_iter(0);
#   if EFL_EINA_BOOST_FT_n > 1
#     define EFL_EINA_BOOST_PP_LOCAL_MACRO(i) typedef typename mpl::next< \
          EFL_EINA_BOOST_FT_iter(EFL_EINA_BOOST_PP_DEC(i)) >::type EFL_EINA_BOOST_FT_iter(i);
#     define EFL_EINA_BOOST_PP_LOCAL_LIMITS (1,EFL_EINA_BOOST_FT_n-1)
#     include EFL_EINA_BOOST_PP_LOCAL_ITERATE()
#   endif
  public:
    typedef typename detail::EFL_EINA_BOOST_FT_make_type(EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,EFL_EINA_BOOST_FT_arity) 
    < typename mpl::deref< EFL_EINA_BOOST_FT_iter(0) >::type 
#   if EFL_EINA_BOOST_FT_mfp
    , typename detail::cv_traits< 
          typename mpl::deref< EFL_EINA_BOOST_FT_iter(1) >::type >::type
#   endif
#   if EFL_EINA_BOOST_FT_n > (EFL_EINA_BOOST_FT_mfp+1)
#     define EFL_EINA_BOOST_PP_LOCAL_LIMITS (EFL_EINA_BOOST_FT_mfp+1,EFL_EINA_BOOST_FT_n-1)
#     define EFL_EINA_BOOST_PP_LOCAL_MACRO(i) \
        , typename mpl::deref< EFL_EINA_BOOST_FT_iter(i) >::type
#     include EFL_EINA_BOOST_PP_LOCAL_ITERATE()
#   endif
    >::type type;
  };
};

#elif EFL_EINA_BOOST_FT_ARITY_LOOP_SUFFIX

#   ifdef EFL_EINA_BOOST_FT_flags
#     undef EFL_EINA_BOOST_FT_make_type
#     undef EFL_EINA_BOOST_FT_make_type_impl
#   else
EFL_EINA_BOOST_PP_EXPAND(#) undef EFL_EINA_BOOST_FT_make_type
EFL_EINA_BOOST_PP_EXPAND(#) undef EFL_EINA_BOOST_FT_make_type_impl
#   endif
#   undef EFL_EINA_BOOST_FT_iter
#   undef EFL_EINA_BOOST_FT_type_name

#else
#   error "attempt to use arity loop master file without loop"
#endif

