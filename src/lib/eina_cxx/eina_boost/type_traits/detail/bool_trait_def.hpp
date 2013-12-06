
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// $Source$
// $Date: 2011-10-09 15:28:33 -0700 (Sun, 09 Oct 2011) $
// $Revision: 74865 $

#include <eina_boost/type_traits/detail/template_arity_spec.hpp>
#include <eina_boost/type_traits/integral_constant.hpp>
#include <eina_boost/mpl/bool.hpp>
#include <eina_boost/mpl/aux_/lambda_support.hpp>
#include <eina_boost/config.hpp>

//
// Unfortunately some libraries have started using this header without
// cleaning up afterwards: so we'd better undef the macros just in case 
// they've been defined already....
//
#ifdef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL
#undef EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF2
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC2
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC2
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_2
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_PARTIAL_SPEC2_1
#undef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1
#endif

#if defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x570)
#   define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
    typedef ::efl_eina_boost::integral_constant<bool,C> type; \
    enum { value = type::value }; \
    /**/
#   define EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C)

#elif defined(EFL_EINA_BOOST_MSVC) && EFL_EINA_BOOST_MSVC < 1300

#   define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
    typedef ::efl_eina_boost::integral_constant<bool,C> base_; \
    using base_::value; \
    /**/

#endif

#ifndef EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL
#   define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) /**/
#endif

#ifndef EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE
#   define EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) : public ::efl_eina_boost::integral_constant<bool,C>
#endif 


#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(trait,T,C) \
template< typename T > struct trait \
    EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(1,trait,(T)) \
}; \
\
EFL_EINA_BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1,trait) \
/**/


#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF2(trait,T1,T2,C) \
template< typename T1, typename T2 > struct trait \
    EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(2,trait,(T1,T2)) \
}; \
\
EFL_EINA_BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(2,trait) \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF3(trait,T1,T2,T3,C) \
template< typename T1, typename T2, typename T3 > struct trait \
    EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(3,trait,(T1,T2,T3)) \
}; \
\
EFL_EINA_BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(3,trait) \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(trait,sp,C) \
template<> struct trait< sp > \
    EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(1,trait,(sp)) \
}; \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC2(trait,sp1,sp2,C) \
template<> struct trait< sp1,sp2 > \
    EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(2,trait,(sp1,sp2)) \
}; \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(trait,sp,C) \
template<> struct trait##_impl< sp > \
{ \
public:\
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = (C)); \
}; \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC2(trait,sp1,sp2,C) \
template<> struct trait##_impl< sp1,sp2 > \
{ \
public:\
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = (C)); \
}; \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(param,trait,sp,C) \
template< param > struct trait< sp > \
    EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
}; \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(param1,param2,trait,sp,C) \
template< param1, param2 > struct trait< sp > \
    EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
}; \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_1(param,trait,sp1,sp2,C) \
template< param > struct trait< sp1,sp2 > \
    EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
    EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(2,trait,(sp1,sp2)) \
}; \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC2_2(param1,param2,trait,sp1,sp2,C) \
template< param1, param2 > struct trait< sp1,sp2 > \
    EFL_EINA_BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
}; \
/**/

#define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_PARTIAL_SPEC2_1(param,trait,sp1,sp2,C) \
template< param > struct trait##_impl< sp1,sp2 > \
{ \
public:\
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = (C)); \
}; \
/**/

#ifndef EFL_EINA_BOOST_NO_CV_SPECIALIZATIONS
#   define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(trait,sp,value) \
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(trait,sp,value) \
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(trait,sp const,value) \
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(trait,sp volatile,value) \
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(trait,sp const volatile,value) \
    /**/
#else
#   define EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(trait,sp,value) \
    EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_SPEC1(trait,sp,value) \
    /**/
#endif
