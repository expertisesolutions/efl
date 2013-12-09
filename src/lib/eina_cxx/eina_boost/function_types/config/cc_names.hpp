
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef EFL_EINA_BOOST_FT_CONFIG_CC_NAMES_HPP_INCLUDED
#define EFL_EINA_BOOST_FT_CONFIG_CC_NAMES_HPP_INCLUDED

#define EFL_EINA_BOOST_FT_BUILTIN_CC_NAMES \
  (( IMPLICIT           , implicit_cc , EFL_EINA_BOOST_PP_EMPTY                ))\
  (( CDECL              , cdecl_cc    , EFL_EINA_BOOST_PP_IDENTITY(__cdecl   ) ))\
  (( STDCALL            , stdcall_cc  , EFL_EINA_BOOST_PP_IDENTITY(__stdcall ) ))\
  (( PASCAL             , pascal_cc   , EFL_EINA_BOOST_PP_IDENTITY(pascal    ) ))\
  (( FASTCALL           , fastcall_cc , EFL_EINA_BOOST_PP_IDENTITY(__fastcall) ))\
  (( CLRCALL            , clrcall_cc  , EFL_EINA_BOOST_PP_IDENTITY(__clrcall ) ))\
  (( THISCALL           , thiscall_cc , EFL_EINA_BOOST_PP_IDENTITY(__thiscall) ))\
  (( IMPLICIT_THISCALL  , thiscall_cc , EFL_EINA_BOOST_PP_EMPTY                )) 

// append user-defined cc names to builtin ones
#ifdef EFL_EINA_BOOST_FT_CC_NAMES 
#   define EFL_EINA_BOOST_FT_CC_NAMES_SEQ EFL_EINA_BOOST_FT_BUILTIN_CC_NAMES EFL_EINA_BOOST_FT_CC_NAMES
#   define EFL_EINA_BOOST_FT_CC_PREPROCESSING 1
#else
#   define EFL_EINA_BOOST_FT_CC_NAMES_SEQ EFL_EINA_BOOST_FT_BUILTIN_CC_NAMES
#endif

#endif

