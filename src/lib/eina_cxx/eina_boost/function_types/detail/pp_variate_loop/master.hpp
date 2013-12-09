
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifdef __WAVE__
// this file has been generated from the master.hpp file in the same directory
#   pragma wave option(preserve: 0)
#endif

#if !defined(EFL_EINA_BOOST_FT_PREPROCESSING_MODE)
#   error "this file is only for two-pass preprocessing"
#endif

#if !defined(EFL_EINA_BOOST_PP_VALUE)
#   include <eina_boost/preprocessor/slot/slot.hpp>
#   include <eina_boost/preprocessor/facilities/empty.hpp>
#   include <eina_boost/preprocessor/facilities/expand.hpp>
#   include <eina_boost/function_types/detail/encoding/def.hpp>

EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_mfp 0
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_syntax EFL_EINA_BOOST_FT_type_function

#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_non_variadic
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_variadic
#   include __FILE__

EFL_EINA_BOOST_PP_EXPAND(#) if !EFL_EINA_BOOST_FT_NO_CV_FUNC_SUPPORT
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_non_variadic|EFL_EINA_BOOST_FT_const
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_variadic|EFL_EINA_BOOST_FT_const
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_non_variadic|EFL_EINA_BOOST_FT_volatile
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_variadic|EFL_EINA_BOOST_FT_volatile
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_non_variadic|EFL_EINA_BOOST_FT_const|EFL_EINA_BOOST_FT_volatile
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_variadic|EFL_EINA_BOOST_FT_const|EFL_EINA_BOOST_FT_volatile
#   include __FILE__
EFL_EINA_BOOST_PP_EXPAND(#) endif


EFL_EINA_BOOST_PP_EXPAND(#) undef  EFL_EINA_BOOST_FT_syntax
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_syntax EFL_EINA_BOOST_FT_type_function_pointer

#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_pointer|EFL_EINA_BOOST_FT_non_variadic
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_pointer|EFL_EINA_BOOST_FT_variadic
#   include __FILE__

EFL_EINA_BOOST_PP_EXPAND(#) undef  EFL_EINA_BOOST_FT_syntax
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_syntax EFL_EINA_BOOST_FT_type_function_reference

#   define EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_reference|EFL_EINA_BOOST_FT_non_variadic
#   include __FILE__
#   define EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_reference|EFL_EINA_BOOST_FT_variadic
#   include __FILE__

EFL_EINA_BOOST_PP_EXPAND(#) undef  EFL_EINA_BOOST_FT_syntax
EFL_EINA_BOOST_PP_EXPAND(#) undef  EFL_EINA_BOOST_FT_mfp

EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_mfp 1
EFL_EINA_BOOST_PP_EXPAND(#) define EFL_EINA_BOOST_FT_syntax EFL_EINA_BOOST_FT_type_member_function_pointer

#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_member_function_pointer|EFL_EINA_BOOST_FT_non_variadic
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_member_function_pointer|EFL_EINA_BOOST_FT_variadic
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_member_function_pointer|EFL_EINA_BOOST_FT_non_variadic|EFL_EINA_BOOST_FT_const
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_member_function_pointer|EFL_EINA_BOOST_FT_variadic|EFL_EINA_BOOST_FT_const
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_member_function_pointer|EFL_EINA_BOOST_FT_non_variadic|EFL_EINA_BOOST_FT_volatile
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_member_function_pointer|EFL_EINA_BOOST_FT_variadic|EFL_EINA_BOOST_FT_volatile
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_member_function_pointer|EFL_EINA_BOOST_FT_non_variadic|EFL_EINA_BOOST_FT_const|EFL_EINA_BOOST_FT_volatile
#   include __FILE__
#   define  EFL_EINA_BOOST_PP_VALUE \
      EFL_EINA_BOOST_FT_member_function_pointer|EFL_EINA_BOOST_FT_variadic|EFL_EINA_BOOST_FT_const|EFL_EINA_BOOST_FT_volatile
#   include __FILE__

EFL_EINA_BOOST_PP_EXPAND(#) undef  EFL_EINA_BOOST_FT_syntax
EFL_EINA_BOOST_PP_EXPAND(#) undef  EFL_EINA_BOOST_FT_mfp

#   include <eina_boost/function_types/detail/encoding/undef.hpp>
#else 

#   include EFL_EINA_BOOST_PP_ASSIGN_SLOT(1)

#   define  EFL_EINA_BOOST_PP_VALUE EFL_EINA_BOOST_PP_SLOT(1) & EFL_EINA_BOOST_FT_kind_mask
#   include EFL_EINA_BOOST_PP_ASSIGN_SLOT(2)

EFL_EINA_BOOST_PP_EXPAND(#) if !!(EFL_EINA_BOOST_PP_SLOT(2) & (EFL_EINA_BOOST_FT_variations))
EFL_EINA_BOOST_PP_EXPAND(#) if (EFL_EINA_BOOST_PP_SLOT(1) & (EFL_EINA_BOOST_FT_cond)) == (EFL_EINA_BOOST_FT_cond)

#   if ( EFL_EINA_BOOST_PP_SLOT(1) & (EFL_EINA_BOOST_FT_variadic) )
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_ell ...
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_nullary_param
#   else
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_ell
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_nullary_param EFL_EINA_BOOST_FT_NULLARY_PARAM
#   endif

#   if !( EFL_EINA_BOOST_PP_SLOT(1) & (EFL_EINA_BOOST_FT_volatile) )
#     if !( EFL_EINA_BOOST_PP_SLOT(1) & (EFL_EINA_BOOST_FT_const) )
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_cv 
#     else
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_cv const
#     endif
#   else
#     if !( EFL_EINA_BOOST_PP_SLOT(1) & (EFL_EINA_BOOST_FT_const) )
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_cv volatile
#     else
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_cv const volatile
#     endif
#   endif
EFL_EINA_BOOST_PP_EXPAND(#)   define EFL_EINA_BOOST_FT_flags EFL_EINA_BOOST_PP_SLOT(1)
EFL_EINA_BOOST_PP_EXPAND(#)   include EFL_EINA_BOOST_FT_variate_file

EFL_EINA_BOOST_PP_EXPAND(#)   undef EFL_EINA_BOOST_FT_cv
EFL_EINA_BOOST_PP_EXPAND(#)   undef EFL_EINA_BOOST_FT_ell
EFL_EINA_BOOST_PP_EXPAND(#)   undef EFL_EINA_BOOST_FT_nullary_param
EFL_EINA_BOOST_PP_EXPAND(#)   undef EFL_EINA_BOOST_FT_flags
EFL_EINA_BOOST_PP_EXPAND(#) endif
EFL_EINA_BOOST_PP_EXPAND(#) endif
#endif

