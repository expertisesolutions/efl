
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

#if   EFL_EINA_BOOST_FT_ARITY_LOOP_PREFIX

#   ifndef EFL_EINA_BOOST_FT_DETAIL_COMPONENTS_IMPL_MASTER_HPP_INCLUDED
#   define EFL_EINA_BOOST_FT_DETAIL_COMPONENTS_IMPL_MASTER_HPP_INCLUDED
#     include <eina_boost/preprocessor/cat.hpp>
#     include <eina_boost/preprocessor/facilities/empty.hpp>
#     include <eina_boost/preprocessor/facilities/identity.hpp>
#     include <eina_boost/preprocessor/arithmetic/dec.hpp>
#     include <eina_boost/preprocessor/punctuation/comma_if.hpp>
#   endif

#   define EFL_EINA_BOOST_FT_type_name

#   if !EFL_EINA_BOOST_FT_mfp

#     define EFL_EINA_BOOST_FT_types \
          R EFL_EINA_BOOST_PP_COMMA_IF(EFL_EINA_BOOST_FT_arity) EFL_EINA_BOOST_FT_params(EFL_EINA_BOOST_PP_EMPTY)
#   else

#     define EFL_EINA_BOOST_FT_types \
          R, typename class_transform<T0 EFL_EINA_BOOST_FT_cv, L>::type \
          EFL_EINA_BOOST_PP_COMMA_IF(EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_FT_arity)) \
          EFL_EINA_BOOST_FT_params(EFL_EINA_BOOST_PP_EMPTY)

#   endif

#elif EFL_EINA_BOOST_FT_ARITY_LOOP_IS_ITERATING

template< EFL_EINA_BOOST_FT_tplargs(EFL_EINA_BOOST_PP_IDENTITY(typename)), typename L>
struct components_impl<EFL_EINA_BOOST_FT_type, L>
{
  typedef encode_bits<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id> bits;
  typedef constant<EFL_EINA_BOOST_FT_full_mask> mask;

  typedef function_types::components<EFL_EINA_BOOST_FT_type, L> type;
  typedef components_mpl_sequence_tag tag;

  typedef mpl::integral_c<std::size_t,EFL_EINA_BOOST_FT_arity> function_arity;

  typedef EFL_EINA_BOOST_PP_CAT(mpl::vector,EFL_EINA_BOOST_FT_n)< EFL_EINA_BOOST_FT_types > types;
};

#elif EFL_EINA_BOOST_FT_ARITY_LOOP_SUFFIX

#   undef EFL_EINA_BOOST_FT_types
#   undef EFL_EINA_BOOST_FT_type_name

#else
#   error "attempt to use arity loop master file without loop"
#endif

