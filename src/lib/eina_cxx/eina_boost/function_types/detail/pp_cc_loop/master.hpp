
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusions

#ifdef __WAVE__
// this file has been generated from the master.hpp file in the same directory
#   pragma wave option(preserve: 0)
#endif


#if !EFL_EINA_BOOST_PP_IS_ITERATING

#   ifndef EFL_EINA_BOOST_FT_DETAIL_CC_LOOP_MASTER_HPP_INCLUDED
#   define EFL_EINA_BOOST_FT_DETAIL_CC_LOOP_MASTER_HPP_INCLUDED
#     include <eina_boost/function_types/config/cc_names.hpp>

#     include <eina_boost/preprocessor/cat.hpp>
#     include <eina_boost/preprocessor/seq/size.hpp>
#     include <eina_boost/preprocessor/seq/elem.hpp>
#     include <eina_boost/preprocessor/tuple/elem.hpp>
#     include <eina_boost/preprocessor/iteration/iterate.hpp>
#     include <eina_boost/preprocessor/facilities/expand.hpp>
#     include <eina_boost/preprocessor/arithmetic/inc.hpp>
#   endif

#   include <eina_boost/function_types/detail/encoding/def.hpp>
#   include <eina_boost/function_types/detail/encoding/aliases_def.hpp>

#   define  EFL_EINA_BOOST_PP_FILENAME_1 \
        <boost/function_types/detail/pp_cc_loop/master.hpp>
#   define  EFL_EINA_BOOST_PP_ITERATION_LIMITS \
        (0,EFL_EINA_BOOST_PP_SEQ_SIZE(EFL_EINA_BOOST_FT_CC_NAMES_SEQ)-1)
#   include EFL_EINA_BOOST_PP_ITERATE()
#   if !defined(EFL_EINA_BOOST_FT_config_valid) && EFL_EINA_BOOST_FT_CC_PREPROCESSING
#     define EFL_EINA_BOOST_FT_cc_id 1
#     define EFL_EINA_BOOST_FT_cc_name implicit_cc
#     define EFL_EINA_BOOST_FT_cc EFL_EINA_BOOST_PP_EMPTY
#     define EFL_EINA_BOOST_FT_cond callable_builtin
#     include EFL_EINA_BOOST_FT_cc_file
#     undef EFL_EINA_BOOST_FT_cond
#     undef EFL_EINA_BOOST_FT_cc_name
#     undef EFL_EINA_BOOST_FT_cc
#     undef EFL_EINA_BOOST_FT_cc_id
#   elif !defined(EFL_EINA_BOOST_FT_config_valid) // and generating preprocessed file
EFL_EINA_BOOST_PP_EXPAND(#) ifndef EFL_EINA_BOOST_FT_config_valid
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_cc_id 1
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_cc_name implicit_cc
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_cc EFL_EINA_BOOST_PP_EMPTY
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_cond callable_builtin
#define _()
EFL_EINA_BOOST_PP_EXPAND(#)   include EFL_EINA_BOOST_FT_cc_file
#undef _
EFL_EINA_BOOST_PP_EXPAND(#)   undef EFL_EINA_BOOST_FT_cond
EFL_EINA_BOOST_PP_EXPAND(#)   undef EFL_EINA_BOOST_FT_cc_name
EFL_EINA_BOOST_PP_EXPAND(#)   undef EFL_EINA_BOOST_FT_cc
EFL_EINA_BOOST_PP_EXPAND(#)   undef EFL_EINA_BOOST_FT_cc_id
EFL_EINA_BOOST_PP_EXPAND(#) else
EFL_EINA_BOOST_PP_EXPAND(#)   undef EFL_EINA_BOOST_FT_config_valid
EFL_EINA_BOOST_PP_EXPAND(#) endif

#   else
#     undef EFL_EINA_BOOST_FT_config_valid
#   endif

#   include <eina_boost/function_types/detail/encoding/aliases_undef.hpp>
#   include <eina_boost/function_types/detail/encoding/undef.hpp>

#elif EFL_EINA_BOOST_FT_CC_PREPROCESSING

#   define EFL_EINA_BOOST_FT_cc_id  EFL_EINA_BOOST_PP_INC(EFL_EINA_BOOST_PP_FRAME_ITERATION(1))
#   define EFL_EINA_BOOST_FT_cc_inf \
        EFL_EINA_BOOST_PP_SEQ_ELEM(EFL_EINA_BOOST_PP_FRAME_ITERATION(1),EFL_EINA_BOOST_FT_CC_NAMES_SEQ)

#   define EFL_EINA_BOOST_FT_cc_pp_name EFL_EINA_BOOST_PP_TUPLE_ELEM(3,0,EFL_EINA_BOOST_FT_cc_inf)
#   define EFL_EINA_BOOST_FT_cc_name    EFL_EINA_BOOST_PP_TUPLE_ELEM(3,1,EFL_EINA_BOOST_FT_cc_inf)
#   define EFL_EINA_BOOST_FT_cc         EFL_EINA_BOOST_PP_TUPLE_ELEM(3,2,EFL_EINA_BOOST_FT_cc_inf)

#   define EFL_EINA_BOOST_FT_cond EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_FT_CC_,EFL_EINA_BOOST_FT_cc_pp_name)

#   if EFL_EINA_BOOST_FT_cond
#     define EFL_EINA_BOOST_FT_config_valid 1
#     include EFL_EINA_BOOST_FT_cc_file
#   endif

#   undef EFL_EINA_BOOST_FT_cond

#   undef EFL_EINA_BOOST_FT_cc_pp_name
#   undef EFL_EINA_BOOST_FT_cc_name
#   undef EFL_EINA_BOOST_FT_cc

#   undef EFL_EINA_BOOST_FT_cc_id
#   undef EFL_EINA_BOOST_FT_cc_inf

#else // if generating preprocessed file
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_cc_id EFL_EINA_BOOST_PP_INC(EFL_EINA_BOOST_PP_ITERATION())

#   define EFL_EINA_BOOST_FT_cc_inf \
        EFL_EINA_BOOST_PP_SEQ_ELEM(EFL_EINA_BOOST_PP_ITERATION(),EFL_EINA_BOOST_FT_CC_NAMES_SEQ)

#   define EFL_EINA_BOOST_FT_cc_pp_name EFL_EINA_BOOST_PP_TUPLE_ELEM(3,0,EFL_EINA_BOOST_FT_cc_inf)

#   define EFL_EINA_BOOST_FT_CC_DEF(name,index) \
        name EFL_EINA_BOOST_PP_TUPLE_ELEM(3,index,EFL_EINA_BOOST_FT_cc_inf)
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_CC_DEF(EFL_EINA_BOOST_FT_cc_name,1)
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_CC_DEF(EFL_EINA_BOOST_FT_cc,2)
#   undef  EFL_EINA_BOOST_FT_CC_DEF

#   define EFL_EINA_BOOST_FT_cc_cond_v EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_FT_CC_,EFL_EINA_BOOST_FT_cc_pp_name)
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_cond EFL_EINA_BOOST_FT_cc_cond_v
#   undef  EFL_EINA_BOOST_FT_cc_cond_v

#   undef EFL_EINA_BOOST_FT_cc_pp_name
#   undef EFL_EINA_BOOST_FT_cc_inf

EFL_EINA_BOOST_PP_EXPAND(#) if EFL_EINA_BOOST_FT_cond
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_config_valid 1
#define _()
EFL_EINA_BOOST_PP_EXPAND(#)   include EFL_EINA_BOOST_FT_cc_file
#undef _
EFL_EINA_BOOST_PP_EXPAND(#) endif

EFL_EINA_BOOST_PP_EXPAND(#) undef EFL_EINA_BOOST_FT_cond

EFL_EINA_BOOST_PP_EXPAND(#) undef EFL_EINA_BOOST_FT_cc_name
EFL_EINA_BOOST_PP_EXPAND(#) undef EFL_EINA_BOOST_FT_cc

EFL_EINA_BOOST_PP_EXPAND(#) undef EFL_EINA_BOOST_FT_cc_id

#endif

