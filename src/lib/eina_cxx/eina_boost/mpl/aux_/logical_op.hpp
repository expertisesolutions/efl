
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: logical_op.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION!

#if !defined(EFL_EINA_BOOST_MPL_PREPROCESSING_MODE)
#   include <eina_boost/mpl/bool.hpp>
#   include <eina_boost/mpl/aux_/nested_type_wknd.hpp>
#   include <eina_boost/mpl/aux_/na_spec.hpp>
#   include <eina_boost/mpl/aux_/lambda_support.hpp>
#endif

#include <eina_boost/mpl/limits/arity.hpp>
#include <eina_boost/mpl/aux_/preprocessor/params.hpp>
#include <eina_boost/mpl/aux_/preprocessor/ext_params.hpp>
#include <eina_boost/mpl/aux_/preprocessor/def_params_tail.hpp>
#include <eina_boost/mpl/aux_/preprocessor/enum.hpp>
#include <eina_boost/mpl/aux_/preprocessor/sub.hpp>
#include <eina_boost/mpl/aux_/config/ctps.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

#include <eina_boost/preprocessor/dec.hpp>
#include <eina_boost/preprocessor/inc.hpp>
#include <eina_boost/preprocessor/cat.hpp>

namespace efl_eina_boost { namespace mpl {

#   define AUX778076_PARAMS(param, sub) \
    EFL_EINA_BOOST_MPL_PP_PARAMS( \
          EFL_EINA_BOOST_MPL_PP_SUB(EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY, sub) \
        , param \
        ) \
    /**/

#   define AUX778076_SHIFTED_PARAMS(param, sub) \
    EFL_EINA_BOOST_MPL_PP_EXT_PARAMS( \
          2, EFL_EINA_BOOST_MPL_PP_SUB(EFL_EINA_BOOST_PP_INC(EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY), sub) \
        , param \
        ) \
    /**/

#   define AUX778076_SPEC_PARAMS(param) \
    EFL_EINA_BOOST_MPL_PP_ENUM( \
          EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY) \
        , param \
        ) \
    /**/

namespace aux {

#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< bool C_, AUX778076_PARAMS(typename T, 1) >
struct EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)
    : EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE1,_)
{
};

template< AUX778076_PARAMS(typename T, 1) >
struct EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)< AUX778076_OP_VALUE2,AUX778076_PARAMS(T, 1) >
    : EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)<
          EFL_EINA_BOOST_MPL_AUX_NESTED_TYPE_WKND(T1)::value
        , AUX778076_SHIFTED_PARAMS(T, 1)
        , EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE2,_)
        >
{
};

template<>
struct EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)<
          AUX778076_OP_VALUE2
        , AUX778076_SPEC_PARAMS(EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE2,_))
        >
    : EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE2,_)
{
};

#else

template< bool C_ > struct EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)
{
    template< AUX778076_PARAMS(typename T, 1) > struct result_
        : EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE1,_)
    {
    };
};

template<> struct EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)<AUX778076_OP_VALUE2>
{
    template< AUX778076_PARAMS(typename T, 1) > struct result_
        : EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)< 
              EFL_EINA_BOOST_MPL_AUX_NESTED_TYPE_WKND(T1)::value
            >::template result_< AUX778076_SHIFTED_PARAMS(T,1),EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE2,_) >
    {
    };

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, == 1300)
    template<> struct result_<AUX778076_SPEC_PARAMS(EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE2,_))>
        : EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE2,_)
    {
    };
};
#else
};

template<>
struct EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)<AUX778076_OP_VALUE2>
    ::result_< AUX778076_SPEC_PARAMS(EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE2,_)) >
        : EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE2,_)
{
};
#endif // EFL_EINA_BOOST_MSVC == 1300

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace aux

template<
      typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(T1)
    , typename EFL_EINA_BOOST_MPL_AUX_NA_PARAM(T2)
    EFL_EINA_BOOST_MPL_PP_DEF_PARAMS_TAIL(2, typename T, EFL_EINA_BOOST_PP_CAT(AUX778076_OP_VALUE2,_))
    >
struct AUX778076_OP_NAME
#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    : aux::EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)<
          EFL_EINA_BOOST_MPL_AUX_NESTED_TYPE_WKND(T1)::value
        , AUX778076_SHIFTED_PARAMS(T,0)
        >
#else
    : aux::EFL_EINA_BOOST_PP_CAT(AUX778076_OP_NAME,impl)< 
          EFL_EINA_BOOST_MPL_AUX_NESTED_TYPE_WKND(T1)::value
        >::template result_< AUX778076_SHIFTED_PARAMS(T,0) >
#endif
{
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(
          EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY
        , AUX778076_OP_NAME
        , (AUX778076_PARAMS(T, 0))
        )
};

EFL_EINA_BOOST_MPL_AUX_NA_SPEC2(
      2
    , EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY
    , AUX778076_OP_NAME
    )

}}

#undef AUX778076_SPEC_PARAMS
#undef AUX778076_SHIFTED_PARAMS
#undef AUX778076_PARAMS
#undef AUX778076_OP_NAME
#undef AUX778076_OP_VALUE1
#undef AUX778076_OP_VALUE2
