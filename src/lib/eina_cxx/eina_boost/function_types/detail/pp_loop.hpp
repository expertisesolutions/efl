
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusions

#ifndef EFL_EINA_BOOST_FT_DETAIL_PP_LOOP_HPP_INCLUDED
#define EFL_EINA_BOOST_FT_DETAIL_PP_LOOP_HPP_INCLUDED
#   include <eina_boost/preprocessor/facilities/expand.hpp>
#   include <eina_boost/preprocessor/facilities/empty.hpp>
#   include <eina_boost/preprocessor/punctuation/paren.hpp>
#endif

#include <eina_boost/function_types/detail/encoding/def.hpp>
#include <eina_boost/function_types/detail/encoding/aliases_def.hpp>

#if defined(EFL_EINA_BOOST_FT_PREPROCESSING_MODE)
#   define EFL_EINA_BOOST_FT_loop <eina_boost/function_types/detail/pp_cc_loop/master.hpp>
#else
#   define EFL_EINA_BOOST_FT_loop \
        <eina_boost/function_types/detail/pp_cc_loop/preprocessed.hpp>
#endif

#if defined(EFL_EINA_BOOST_FT_al_path)

#   define EFL_EINA_BOOST_FT_cc_file \
        <eina_boost/function_types/detail/pp_variate_loop/preprocessed.hpp>
#   define EFL_EINA_BOOST_FT_variate_file \
        <eina_boost/function_types/detail/pp_arity_loop.hpp>

#   ifndef EFL_EINA_BOOST_FT_type_function
#   define EFL_EINA_BOOST_FT_type_function(cc,name) EFL_EINA_BOOST_FT_SYNTAX( \
      R EFL_EINA_BOOST_PP_EMPTY,EFL_EINA_BOOST_PP_EMPTY,cc,EFL_EINA_BOOST_PP_EMPTY,name,EFL_EINA_BOOST_PP_EMPTY)
#   endif
#   ifndef EFL_EINA_BOOST_FT_type_function_pointer
#   define EFL_EINA_BOOST_FT_type_function_pointer(cc,name) EFL_EINA_BOOST_FT_SYNTAX( \
      R EFL_EINA_BOOST_PP_EMPTY,EFL_EINA_BOOST_PP_LPAREN,cc,* EFL_EINA_BOOST_PP_EMPTY,name,EFL_EINA_BOOST_PP_RPAREN)
#   endif
#   ifndef EFL_EINA_BOOST_FT_type_function_reference
#   define EFL_EINA_BOOST_FT_type_function_reference(cc,name) EFL_EINA_BOOST_FT_SYNTAX( \
      R EFL_EINA_BOOST_PP_EMPTY,EFL_EINA_BOOST_PP_LPAREN,cc,& EFL_EINA_BOOST_PP_EMPTY,name,EFL_EINA_BOOST_PP_RPAREN)
#   endif
#   ifndef EFL_EINA_BOOST_FT_type_member_function_pointer
#   define EFL_EINA_BOOST_FT_type_member_function_pointer(cc,name) EFL_EINA_BOOST_FT_SYNTAX( \
      R EFL_EINA_BOOST_PP_EMPTY,EFL_EINA_BOOST_PP_LPAREN,cc,T0::* EFL_EINA_BOOST_PP_EMPTY,name,EFL_EINA_BOOST_PP_RPAREN)
#   endif

#   include EFL_EINA_BOOST_FT_loop

#   undef EFL_EINA_BOOST_FT_type_function
#   undef EFL_EINA_BOOST_FT_type_function_pointer
#   undef EFL_EINA_BOOST_FT_type_function_reference
#   undef EFL_EINA_BOOST_FT_type_member_function_pointer

#   undef EFL_EINA_BOOST_FT_variations
#   undef EFL_EINA_BOOST_FT_variate_file
#   undef EFL_EINA_BOOST_FT_cc_file
#   undef EFL_EINA_BOOST_FT_al_path

#elif defined(EFL_EINA_BOOST_FT_cc_file)

#   include EFL_EINA_BOOST_FT_loop
#   undef EFL_EINA_BOOST_FT_cc_file

#else

#   error "argument missing"

#endif

#undef EFL_EINA_BOOST_FT_loop

#include <eina_boost/function_types/detail/encoding/aliases_undef.hpp>
#include <eina_boost/function_types/detail/encoding/undef.hpp>


