
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

#if EFL_EINA_BOOST_FT_ARITY_LOOP_PREFIX

#   ifndef EFL_EINA_BOOST_FT_DETAIL_CLASSIFIER_IMPL_MASTER_HPP_INCLUDED
#   define EFL_EINA_BOOST_FT_DETAIL_CLASSIFIER_IMPL_MASTER_HPP_INCLUDED
#     include <eina_boost/preprocessor/facilities/identity.hpp>
#   endif

#   define EFL_EINA_BOOST_FT_type_name

#elif EFL_EINA_BOOST_FT_ARITY_LOOP_IS_ITERATING

template< EFL_EINA_BOOST_FT_tplargs(EFL_EINA_BOOST_PP_IDENTITY(typename)) >
typename encode_charr<EFL_EINA_BOOST_FT_flags,EFL_EINA_BOOST_FT_cc_id,EFL_EINA_BOOST_FT_arity>::type
classifier_impl(EFL_EINA_BOOST_FT_type);

#elif EFL_EINA_BOOST_FT_ARITY_LOOP_SUFFIX

#   undef EFL_EINA_BOOST_FT_type_name

#else
#   error "attempt to use arity loop master file without loop"
#endif

