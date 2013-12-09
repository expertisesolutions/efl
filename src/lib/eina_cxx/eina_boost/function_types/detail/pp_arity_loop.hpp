
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

#ifndef EFL_EINA_BOOST_FT_PREPROCESSING_MODE
// input:  EFL_EINA_BOOST_FT_mfp        0 or 1 <=> member function pointer?
// input:  EFL_EINA_BOOST_FT_type_name  EFL_EINA_BOOST_FT_type --> "R (* ..._type_name)()" (pass2)
#endif
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

#ifdef __WAVE__
#   pragma wave option(preserve: 0)
#endif

#ifndef EFL_EINA_BOOST_FT_ARITY_LOOP_IS_ITERATING

#   define EFL_EINA_BOOST_FT_AL_PREPROCESSED \
        EFL_EINA_BOOST_FT_AL_FILE(EFL_EINA_BOOST_FT_al_path,EFL_EINA_BOOST_FT_FROM_ARITY,EFL_EINA_BOOST_FT_mfp)

#   define EFL_EINA_BOOST_FT_AL_FILE(base_path,max_arity,mfp) \
        EFL_EINA_BOOST_FT_AL_FILE_I(base_path,max_arity,mfp)
#   define EFL_EINA_BOOST_FT_AL_FILE_I(base_path,max_arity,mfp) \
        <base_path/arity ## max_arity ## _ ## mfp.hpp>

#   if !defined(EFL_EINA_BOOST_FT_PREPROCESSING_MODE)

#     if EFL_EINA_BOOST_FT_MAX_ARITY < 10
#       define EFL_EINA_BOOST_FT_FROM_ARITY 0 
#     elif EFL_EINA_BOOST_FT_MAX_ARITY < 20
#       define EFL_EINA_BOOST_FT_FROM_ARITY 10
#     elif EFL_EINA_BOOST_FT_MAX_ARITY < 30
#       define EFL_EINA_BOOST_FT_FROM_ARITY 20
#     elif EFL_EINA_BOOST_FT_MAX_ARITY < 40
#       define EFL_EINA_BOOST_FT_FROM_ARITY 30
#     endif

#     if EFL_EINA_BOOST_FT_FROM_ARITY
#       include EFL_EINA_BOOST_FT_AL_PREPROCESSED
#     endif

#   elif !defined(EFL_EINA_BOOST_FT_FROM_ARITY) // single pass preprocessing
#     define EFL_EINA_BOOST_FT_FROM_ARITY 0

#   elif EFL_EINA_BOOST_FT_FROM_ARITY > 0       // arity20 includes arity10
EFL_EINA_BOOST_PP_EXPAND(#) include EFL_EINA_BOOST_FT_AL_PREPROCESSED
#   endif

#   undef EFL_EINA_BOOST_FT_AL_PREPROCESSED

#   undef EFL_EINA_BOOST_FT_AL_FILE
#   undef EFL_EINA_BOOST_FT_AL_FILE_I

#   if EFL_EINA_BOOST_FT_MAX_ARITY > EFL_EINA_BOOST_FT_FROM_ARITY

#     ifndef EFL_EINA_BOOST_FT_DETAIL_ARITY_LOOP_HPP_INCLUDED
#     define EFL_EINA_BOOST_FT_DETAIL_ARITY_LOOP_HPP_INCLUDED
#         include <eina_boost/preprocessor/cat.hpp>
#         include <eina_boost/preprocessor/tuple/eat.hpp>
#         include <eina_boost/preprocessor/control/if.hpp>
#         include <eina_boost/preprocessor/arithmetic/inc.hpp>
#         include <eina_boost/preprocessor/facilities/empty.hpp>
#         include <eina_boost/preprocessor/facilities/expand.hpp>
#         include <eina_boost/preprocessor/iteration/iterate.hpp>
#         include <eina_boost/preprocessor/repetition/enum_params.hpp>
#         include <eina_boost/preprocessor/repetition/enum_shifted_params.hpp>
#         include <eina_boost/preprocessor/repetition/enum_trailing_params.hpp>
#     endif

#     define EFL_EINA_BOOST_FT_AL_INCLUDE_FILE <EFL_EINA_BOOST_FT_al_path/master.hpp>

#     define EFL_EINA_BOOST_FT_ARITY_LOOP_PREFIX 1
#     include EFL_EINA_BOOST_FT_AL_INCLUDE_FILE
#     undef  EFL_EINA_BOOST_FT_ARITY_LOOP_PREFIX

#     if !EFL_EINA_BOOST_PP_IS_ITERATING
#       define EFL_EINA_BOOST_PP_FILENAME_1 EFL_EINA_BOOST_FT_AL_INCLUDE_FILE
#     elif EFL_EINA_BOOST_PP_ITERATION_DEPTH() == 1
#       define EFL_EINA_BOOST_PP_FILENAME_2 EFL_EINA_BOOST_FT_AL_INCLUDE_FILE
#     else
#       error "loops nested too deeply"
#     endif

#     define EFL_EINA_BOOST_FT_arity EFL_EINA_BOOST_PP_ITERATION()
#     define EFL_EINA_BOOST_FT_n     EFL_EINA_BOOST_PP_INC(EFL_EINA_BOOST_FT_arity)

#     define EFL_EINA_BOOST_FT_type \
          EFL_EINA_BOOST_FT_syntax(EFL_EINA_BOOST_FT_cc,EFL_EINA_BOOST_FT_type_name EFL_EINA_BOOST_PP_EMPTY)\
               (EFL_EINA_BOOST_FT_params(EFL_EINA_BOOST_PP_EMPTY) EFL_EINA_BOOST_FT_ell) EFL_EINA_BOOST_FT_cv

#     define EFL_EINA_BOOST_FT_tplargs(prefx) \
          prefx() R EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_FT_arity,prefx() T)

#     if !EFL_EINA_BOOST_FT_mfp

#       define EFL_EINA_BOOST_FT_params(prefx) \
            EFL_EINA_BOOST_PP_IF(EFL_EINA_BOOST_FT_arity,EFL_EINA_BOOST_PP_ENUM_PARAMS, \
                EFL_EINA_BOOST_FT_nullary_param EFL_EINA_BOOST_PP_TUPLE_EAT(2))( \
                    EFL_EINA_BOOST_FT_arity,prefx() T) 
#     else

#       define EFL_EINA_BOOST_FT_params(prefx) \
            EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS(EFL_EINA_BOOST_FT_arity,prefx() T)

#     endif

#     if !EFL_EINA_BOOST_FT_FROM_ARITY 
#       define EFL_EINA_BOOST_PP_ITERATION_LIMITS (EFL_EINA_BOOST_FT_mfp, EFL_EINA_BOOST_FT_MAX_ARITY)
#     else
#       define EFL_EINA_BOOST_PP_ITERATION_LIMITS \
            (EFL_EINA_BOOST_FT_FROM_ARITY+1, EFL_EINA_BOOST_FT_MAX_ARITY)
#     endif

#     define EFL_EINA_BOOST_FT_ARITY_LOOP_IS_ITERATING 1
#     include EFL_EINA_BOOST_PP_ITERATE()
#     undef  EFL_EINA_BOOST_FT_ARITY_LOOP_IS_ITERATING

#     undef EFL_EINA_BOOST_FT_arity
#     undef EFL_EINA_BOOST_FT_params
#     undef EFL_EINA_BOOST_FT_tplargs
#     undef EFL_EINA_BOOST_FT_type

#     define EFL_EINA_BOOST_FT_ARITY_LOOP_SUFFIX 1
#     include EFL_EINA_BOOST_FT_AL_INCLUDE_FILE
#     undef  EFL_EINA_BOOST_FT_ARITY_LOOP_SUFFIX

#     undef EFL_EINA_BOOST_FT_AL_INCLUDE_FILE
#   endif

#   undef EFL_EINA_BOOST_FT_FROM_ARITY

#else
#   error "attempt to nest arity loops"
#endif

