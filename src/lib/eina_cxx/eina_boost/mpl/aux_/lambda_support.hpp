
#ifndef EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: lambda_support.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include <eina_boost/mpl/aux_/config/lambda.hpp>

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)

#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) /**/
#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(i,name,params) /**/

#else

#   include <eina_boost/mpl/int_fwd.hpp>
#   include <eina_boost/mpl/aux_/yes_no.hpp>
#   include <eina_boost/mpl/aux_/na_fwd.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/params.hpp>
#   include <eina_boost/mpl/aux_/preprocessor/enum.hpp>
#   include <eina_boost/mpl/aux_/config/msvc.hpp>
#   include <eina_boost/mpl/aux_/config/workaround.hpp>

#   include <eina_boost/preprocessor/tuple/to_list.hpp>
#   include <eina_boost/preprocessor/list/for_each_i.hpp>
#   include <eina_boost/preprocessor/inc.hpp>
#   include <eina_boost/preprocessor/cat.hpp>

#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC(R,typedef_,i,param) \
    typedef_ param EFL_EINA_BOOST_PP_CAT(arg,EFL_EINA_BOOST_PP_INC(i)); \
    /**/

// agurt, 07/mar/03: restore an old revision for the sake of SGI MIPSpro C++
#if EFL_EINA_BOOST_WORKAROUND(__EDG_VERSION__, <= 238) 

#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    typedef EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::int_<i> arity; \
    EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_R( \
          1 \
        , EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
        , typedef \
        , EFL_EINA_BOOST_PP_TUPLE_TO_LIST(i,params) \
        ) \
    struct rebind \
    { \
        template< EFL_EINA_BOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
            : name< EFL_EINA_BOOST_MPL_PP_PARAMS(i,U) > \
        { \
        }; \
    }; \
    /**/

#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    /**/

#elif EFL_EINA_BOOST_WORKAROUND(__EDG_VERSION__, <= 244) && !defined(EFL_EINA_BOOST_INTEL_CXX_VERSION)
// agurt, 18/jan/03: old EDG-based compilers actually enforce 11.4 para 9
// (in strict mode), so we have to provide an alternative to the 
// MSVC-optimized implementation

#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
    typedef EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::int_<i> arity; \
    EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_R( \
          1 \
        , EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
        , typedef \
        , EFL_EINA_BOOST_PP_TUPLE_TO_LIST(i,params) \
        ) \
    struct rebind; \
/**/

#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
template< EFL_EINA_BOOST_MPL_PP_PARAMS(i,typename T) > \
struct name<EFL_EINA_BOOST_MPL_PP_PARAMS(i,T)>::rebind \
{ \
    template< EFL_EINA_BOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
        : name< EFL_EINA_BOOST_MPL_PP_PARAMS(i,U) > \
    { \
    }; \
/**/

#else // __EDG_VERSION__

namespace efl_eina_boost { namespace mpl { namespace aux {
template< typename T > struct has_rebind_tag;
}}}

#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
    typedef EFL_EINA_BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::int_<i> arity; \
    EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_R( \
          1 \
        , EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
        , typedef \
        , EFL_EINA_BOOST_PP_TUPLE_TO_LIST(i,params) \
        ) \
    friend class EFL_EINA_BOOST_PP_CAT(name,_rebind); \
    typedef EFL_EINA_BOOST_PP_CAT(name,_rebind) rebind; \
/**/

#if EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x610))
#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
template< EFL_EINA_BOOST_MPL_PP_PARAMS(i,typename T) > \
::efl_eina_boost::mpl::aux::yes_tag operator|( \
      ::efl_eina_boost::mpl::aux::has_rebind_tag<int> \
    , name<EFL_EINA_BOOST_MPL_PP_PARAMS(i,T)>* \
    ); \
::efl_eina_boost::mpl::aux::no_tag operator|( \
      ::efl_eina_boost::mpl::aux::has_rebind_tag<int> \
    , name< EFL_EINA_BOOST_MPL_PP_ENUM(i,::efl_eina_boost::mpl::na) >* \
    ); \
/**/
#elif !EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, < 1300)
#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
template< EFL_EINA_BOOST_MPL_PP_PARAMS(i,typename T) > \
::efl_eina_boost::mpl::aux::yes_tag operator|( \
      ::efl_eina_boost::mpl::aux::has_rebind_tag<int> \
    , ::efl_eina_boost::mpl::aux::has_rebind_tag< name<EFL_EINA_BOOST_MPL_PP_PARAMS(i,T)> >* \
    ); \
/**/
#else
#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) /**/
#endif

#   if !defined(__BORLANDC__)
#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
class EFL_EINA_BOOST_PP_CAT(name,_rebind) \
{ \
 public: \
    template< EFL_EINA_BOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
        : name< EFL_EINA_BOOST_MPL_PP_PARAMS(i,U) > \
    { \
    }; \
/**/
#   else
#   define EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
class EFL_EINA_BOOST_PP_CAT(name,_rebind) \
{ \
 public: \
    template< EFL_EINA_BOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
    { \
        typedef typename name< EFL_EINA_BOOST_MPL_PP_PARAMS(i,U) >::type type; \
    }; \
/**/
#   endif // __BORLANDC__

#endif // __EDG_VERSION__

#endif // EFL_EINA_BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT

#endif // EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_HPP_INCLUDED
