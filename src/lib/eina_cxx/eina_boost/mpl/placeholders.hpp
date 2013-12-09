
#if !defined(EFL_EINA_BOOST_PP_IS_ITERATING)

///// header body

#ifndef EFL_EINA_BOOST_MPL_PLACEHOLDERS_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_PLACEHOLDERS_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
// Copyright Peter Dimov 2001-2003
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: placeholders.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $


#if !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)
#   include <eina_boost/mpl/arg.hpp>
#   include <eina_boost/mpl/aux_/adl_barrier.hpp>

#   if !defined(EFL_EINA_BOOST_MPL_CFG_NO_ADL_BARRIER_NAMESPACE)
#       define EFL_EINA_BOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(type) \
        using ::EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::type; \
        /**/
#   else
#       define EFL_EINA_BOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(type) /**/
#   endif

#endif

#include <eina_boost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
 && !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)

#   define EFL_EINA_BOOST_MPL_PREPROCESSED_HEADER placeholders.hpp
#   include <eina_boost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <eina_boost/mpl/aux_/nttp_decl.hpp>
#   include <eina_boost/mpl/limits/arity.hpp>
#   include <eina_boost/preprocessor/iterate.hpp>
#   include <eina_boost/preprocessor/cat.hpp>

// watch out for GNU gettext users, who #define _(x)
#if !defined(_) || defined(EFL_EINA_BOOST_MPL_CFG_NO_UNNAMED_PLACEHOLDER_SUPPORT)
EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN
typedef arg<-1> _;
EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

namespace efl_eina_boost { namespace mpl { 

EFL_EINA_BOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(_)

namespace placeholders {
using EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::_;
}

}}
#endif

/// agurt, 17/mar/02: one more placeholder for the last 'apply#' 
/// specialization
#define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 \
    (3,(1, EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY + 1, <boost/mpl/placeholders.hpp>))
#include EFL_EINA_BOOST_PP_ITERATE()

#endif // EFL_EINA_BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // EFL_EINA_BOOST_MPL_PLACEHOLDERS_HPP_INCLUDED

///// iteration

#else
#define i_ EFL_EINA_BOOST_PP_FRAME_ITERATION(1)

EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

typedef arg<i_> EFL_EINA_BOOST_PP_CAT(_,i_);

EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE

namespace efl_eina_boost { namespace mpl { 

EFL_EINA_BOOST_MPL_AUX_ARG_ADL_BARRIER_DECL(EFL_EINA_BOOST_PP_CAT(_,i_))

namespace placeholders {
using EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::EFL_EINA_BOOST_PP_CAT(_,i_);
}

}}

#undef i_
#endif // EFL_EINA_BOOST_PP_IS_ITERATING
