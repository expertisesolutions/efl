
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION

// Copyright Aleksey Gurtovoy 2000-2006
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: include_preprocessed.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/compiler.hpp>
#include <eina_boost/mpl/aux_/config/preprocessor.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>
#include <eina_boost/preprocessor/cat.hpp>
#include <eina_boost/preprocessor/stringize.hpp>

#if !defined(EFL_EINA_BOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING)
#   define AUX778076_PREPROCESSED_HEADER \
    EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR/EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER \
/**/
#else
#   define AUX778076_PREPROCESSED_HEADER \
    EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_MPL_CFG_COMPILER_DIR,/)##EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER \
/**/
#endif

#if EFL_EINA_BOOST_WORKAROUND(__IBMCPP__, EFL_EINA_BOOST_TESTED_AT(700))
#   define AUX778076_INCLUDE_STRING EFL_EINA_BOOST_PP_STRINGIZE(boost/mpl/aux_/preprocessed/AUX778076_PREPROCESSED_HEADER)
#   include AUX778076_INCLUDE_STRING
#   undef AUX778076_INCLUDE_STRING
#else
#   include EFL_EINA_BOOST_PP_STRINGIZE(eina_boost/mpl/aux_/preprocessed/AUX778076_PREPROCESSED_HEADER)
#endif

#   undef AUX778076_PREPROCESSED_HEADER

#undef EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER
