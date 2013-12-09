
#ifndef EFL_EINA_BOOST_MPL_HAS_XXX_HPP_INCLUDED
#define EFL_EINA_BOOST_MPL_HAS_XXX_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2006
// Copyright David Abrahams 2002-2003
// Copyright Daniel Walker 2007
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: has_xxx.hpp 64146 2010-07-19 00:46:31Z djwalker $
// $Date: 2010-07-18 17:46:31 -0700 (Sun, 18 Jul 2010) $
// $Revision: 64146 $

#include <eina_boost/mpl/bool.hpp>
#include <eina_boost/mpl/aux_/na_spec.hpp>
#include <eina_boost/mpl/aux_/type_wrapper.hpp>
#include <eina_boost/mpl/aux_/yes_no.hpp>
#include <eina_boost/mpl/aux_/config/gcc.hpp>
#include <eina_boost/mpl/aux_/config/has_xxx.hpp>
#include <eina_boost/mpl/aux_/config/msvc_typename.hpp>
#include <eina_boost/mpl/aux_/config/msvc.hpp>
#include <eina_boost/mpl/aux_/config/static_constant.hpp>
#include <eina_boost/mpl/aux_/config/workaround.hpp>

#include <eina_boost/preprocessor/array/elem.hpp>
#include <eina_boost/preprocessor/cat.hpp>
#include <eina_boost/preprocessor/control/if.hpp>
#include <eina_boost/preprocessor/repetition/enum_params.hpp>
#include <eina_boost/preprocessor/repetition/enum_trailing_params.hpp>

#if EFL_EINA_BOOST_WORKAROUND( __BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x590) )
# include <eina_boost/type_traits/is_class.hpp>
#endif

#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_HAS_XXX)

#   if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1300)

// agurt, 11/sep/02: MSVC-specific version (< 7.1), based on a USENET 
// newsgroup's posting by John Madsen (comp.lang.c++.moderated, 
// 1999-11-12 19:17:06 GMT); the code is _not_ standard-conforming, but 
// it works way more reliably than the SFINAE-based implementation

// Modified dwa 8/Oct/02 to handle reference types.

#   include <eina_boost/mpl/if.hpp>
#   include <eina_boost/mpl/bool.hpp>

namespace efl_eina_boost { namespace mpl { namespace aux {

struct has_xxx_tag;

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, == 1300)
template< typename U > struct msvc_incomplete_array
{
    typedef char (&type)[sizeof(U) + 1];
};
#endif

template< typename T >
struct msvc_is_incomplete
{
    // MSVC is capable of some kinds of SFINAE.  If U is an incomplete
    // type, it won't pick the second overload
    static char tester(...);

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, == 1300)
    template< typename U >
    static typename msvc_incomplete_array<U>::type tester(type_wrapper<U>);
#else
    template< typename U >
    static char (& tester(type_wrapper<U>) )[sizeof(U)+1];
#endif 
    
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = 
          sizeof(tester(type_wrapper<T>())) == 1
        );
};

template<>
struct msvc_is_incomplete<int>
{
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
};

}}}

#   define EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF_(trait, name, default_) \
template< typename T, typename name = ::efl_eina_boost::mpl::aux::has_xxx_tag > \
struct EFL_EINA_BOOST_PP_CAT(trait,_impl) : T \
{ \
    static efl_eina_boost::mpl::aux::no_tag \
    test(void(*)(::efl_eina_boost::mpl::aux::has_xxx_tag)); \
    \
    static efl_eina_boost::mpl::aux::yes_tag test(...); \
    \
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = \
          sizeof(test(static_cast<void(*)(name)>(0))) \
            != sizeof(efl_eina_boost::mpl::aux::no_tag) \
        ); \
    typedef efl_eina_boost::mpl::bool_<value> type; \
}; \
\
template< typename T, typename fallback_ = efl_eina_boost::mpl::bool_<default_> > \
struct trait \
    : efl_eina_boost::mpl::if_c< \
          efl_eina_boost::mpl::aux::msvc_is_incomplete<T>::value \
        , efl_eina_boost::mpl::bool_<false> \
        , EFL_EINA_BOOST_PP_CAT(trait,_impl)<T> \
        >::type \
{ \
}; \
\
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, void) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, bool) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, char) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, signed char) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, unsigned char) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, signed short) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, unsigned short) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, signed int) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, unsigned int) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, signed long) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, unsigned long) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, float) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, double) \
EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, long double) \
/**/

#   define EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, T) \
template<> struct trait<T> \
{ \
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false); \
    typedef efl_eina_boost::mpl::bool_<false> type; \
}; \
/**/

#if !defined(EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T)
#   define EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, unused) \
    EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF_(trait, name, unused) \
    EFL_EINA_BOOST_MPL_AUX_HAS_XXX_TRAIT_SPEC(trait, wchar_t) \
/**/
#else
#   define EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, unused) \
    EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF_(trait, name, unused) \
/**/
#endif


// SFINAE-based implementations below are derived from a USENET newsgroup's 
// posting by Rani Sharoni (comp.lang.c++.moderated, 2002-03-17 07:45:09 PST)

#   elif EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, EFL_EINA_BOOST_TESTED_AT(1400)) \
      || EFL_EINA_BOOST_WORKAROUND(__IBMCPP__, <= 700)

// MSVC 7.1+ & VACPP

// agurt, 15/jun/05: replace overload-based SFINAE implementation with SFINAE
// applied to partial specialization to fix some apparently random failures 
// (thanks to Daniel Wallin for researching this!)

#   define EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, default_) \
template< typename T > \
struct EFL_EINA_BOOST_PP_CAT(trait, _msvc_sfinae_helper) \
{ \
    typedef void type; \
};\
\
template< typename T, typename U = void > \
struct EFL_EINA_BOOST_PP_CAT(trait,_impl_) \
{ \
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false); \
    typedef efl_eina_boost::mpl::bool_<value> type; \
}; \
\
template< typename T > \
struct EFL_EINA_BOOST_PP_CAT(trait,_impl_)< \
      T \
    , typename EFL_EINA_BOOST_PP_CAT(trait, _msvc_sfinae_helper)< typename T::name >::type \
    > \
{ \
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = true); \
    typedef efl_eina_boost::mpl::bool_<value> type; \
}; \
\
template< typename T, typename fallback_ = efl_eina_boost::mpl::bool_<default_> > \
struct trait \
    : EFL_EINA_BOOST_PP_CAT(trait,_impl_)<T> \
{ \
}; \
/**/

#   elif EFL_EINA_BOOST_WORKAROUND( __BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x590) )

#   define EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_BCB_DEF(trait, trait_tester, name, default_) \
template< typename T, bool IS_CLASS > \
struct trait_tester \
{ \
    EFL_EINA_BOOST_STATIC_CONSTANT( bool,  value = false ); \
}; \
template< typename T > \
struct trait_tester< T, true > \
{ \
    struct trait_tester_impl \
    { \
        template < class U > \
        static int  resolve( efl_eina_boost::mpl::aux::type_wrapper<U> const volatile * \
                           , efl_eina_boost::mpl::aux::type_wrapper<typename U::name >* = 0 ); \
        static char resolve( ... ); \
    }; \
    typedef efl_eina_boost::mpl::aux::type_wrapper<T> t_; \
    EFL_EINA_BOOST_STATIC_CONSTANT( bool, value = ( sizeof( trait_tester_impl::resolve( static_cast< t_ * >(0) ) ) == sizeof(int) ) ); \
}; \
template< typename T, typename fallback_ = efl_eina_boost::mpl::bool_<default_> > \
struct trait           \
{                      \
    EFL_EINA_BOOST_STATIC_CONSTANT( bool, value = (trait_tester< T, efl_eina_boost::is_class< T >::value >::value) );     \
    typedef efl_eina_boost::mpl::bool_< trait< T, fallback_ >::value > type; \
};

#   define EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, default_) \
    EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_BCB_DEF( trait \
                                         , EFL_EINA_BOOST_PP_CAT(trait,_tester)      \
                                         , name       \
                                         , default_ ) \
/**/

#   else // other SFINAE-capable compilers

#   define EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, default_) \
template< typename T, typename fallback_ = efl_eina_boost::mpl::bool_<default_> > \
struct trait \
{ \
    struct gcc_3_2_wknd \
    { \
        template< typename U > \
        static efl_eina_boost::mpl::aux::yes_tag test( \
              efl_eina_boost::mpl::aux::type_wrapper<U> const volatile* \
            , efl_eina_boost::mpl::aux::type_wrapper<EFL_EINA_BOOST_MSVC_TYPENAME U::name>* = 0 \
            ); \
    \
        static efl_eina_boost::mpl::aux::no_tag test(...); \
    }; \
    \
    typedef efl_eina_boost::mpl::aux::type_wrapper<T> t_; \
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = \
          sizeof(gcc_3_2_wknd::test(static_cast<t_*>(0))) \
            == sizeof(efl_eina_boost::mpl::aux::yes_tag) \
        ); \
    typedef efl_eina_boost::mpl::bool_<value> type; \
}; \
/**/

#   endif // EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1300)


#else // EFL_EINA_BOOST_MPL_CFG_NO_HAS_XXX

// placeholder implementation

#   define EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait, name, default_) \
template< typename T, typename fallback_ = efl_eina_boost::mpl::bool_<default_> > \
struct trait \
{ \
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = fallback_::value); \
    typedef fallback_ type; \
}; \
/**/

#endif

#define EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_DEF(name) \
    EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(EFL_EINA_BOOST_PP_CAT(has_,name), name, false) \
/**/


#if !defined(EFL_EINA_BOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE)

// Create a boolean Metafunction to detect a nested template
// member. This implementation is based on a USENET newsgroup's
// posting by Aleksey Gurtovoy (comp.lang.c++.moderated, 2002-03-19),
// Rani Sharoni's USENET posting cited above, the non-template has_xxx
// implementations above, and discussion on the Boost mailing list.

#   if !defined(EFL_EINA_BOOST_MPL_HAS_XXX_NO_WRAPPED_TYPES)
#     if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1400)
#       define EFL_EINA_BOOST_MPL_HAS_XXX_NO_WRAPPED_TYPES 1
#     endif
#   endif

#   if !defined(EFL_EINA_BOOST_MPL_HAS_XXX_NO_EXPLICIT_TEST_FUNCTION)
#     if (defined(EFL_EINA_BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS))
#       define EFL_EINA_BOOST_MPL_HAS_XXX_NO_EXPLICIT_TEST_FUNCTION 1
#     endif
#   endif

#   if !defined(EFL_EINA_BOOST_MPL_HAS_XXX_NEEDS_TEMPLATE_SFINAE)
#     if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1400)
#       define EFL_EINA_BOOST_MPL_HAS_XXX_NEEDS_TEMPLATE_SFINAE 1
#     endif
#   endif

// NOTE: Many internal implementation macros take a Boost.Preprocessor
// array argument called args which is of the following form.
//           ( 4, ( trait, name, max_arity, default_ ) )

#   define EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args) \
      EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ARRAY_ELEM(0, args) , _introspect) \
    /**/

#   define EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) \
      EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ARRAY_ELEM(0, args) , _substitute), n) \
    /**/

#   define EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args) \
      EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ARRAY_ELEM(0, args) , _test) \
    /**/

// Thanks to Guillaume Melquiond for pointing out the need for the
// "substitute" template as an argument to the overloaded test
// functions to get SFINAE to work for member templates with the
// correct name but different number of arguments.
#   define EFL_EINA_BOOST_MPL_HAS_MEMBER_MULTI_SUBSTITUTE(z, n, args) \
      template< \
          template< EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_INC(n), typename V) > class V \
       > \
      struct EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) { \
      }; \
    /**/

#   define EFL_EINA_BOOST_MPL_HAS_MEMBER_SUBSTITUTE(args, substitute_macro) \
      EFL_EINA_BOOST_PP_REPEAT( \
          EFL_EINA_BOOST_PP_ARRAY_ELEM(2, args) \
        , EFL_EINA_BOOST_MPL_HAS_MEMBER_MULTI_SUBSTITUTE \
        , args \
      ) \
    /**/

#   if !EFL_EINA_BOOST_MPL_HAS_XXX_NO_EXPLICIT_TEST_FUNCTION
#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_REJECT(args, member_macro) \
        template< typename V > \
        static efl_eina_boost::mpl::aux::no_tag \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)(...); \
      /**/
#   else
#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_REJECT(args, member_macro) \
        static efl_eina_boost::mpl::aux::no_tag \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)(...); \
      /**/
#   endif

#   if !EFL_EINA_BOOST_MPL_HAS_XXX_NO_WRAPPED_TYPES
#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_MULTI_ACCEPT(z, n, args) \
        template< typename V > \
        static efl_eina_boost::mpl::aux::yes_tag \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)( \
            efl_eina_boost::mpl::aux::type_wrapper< V > const volatile* \
          , EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) < \
                V::template EFL_EINA_BOOST_PP_ARRAY_ELEM(1, args) \
            >* = 0 \
        ); \
      /**/
#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_ACCEPT(args, member_macro) \
        EFL_EINA_BOOST_PP_REPEAT( \
            EFL_EINA_BOOST_PP_ARRAY_ELEM(2, args) \
          , EFL_EINA_BOOST_MPL_HAS_MEMBER_MULTI_ACCEPT \
          , args \
        ) \
      /**/
#   else
#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_ACCEPT(args, member_macro) \
        template< typename V > \
        static efl_eina_boost::mpl::aux::yes_tag \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)( \
            V const volatile* \
          , member_macro(args, V, T)* = 0 \
        ); \
      /**/
#   endif

#   if !EFL_EINA_BOOST_MPL_HAS_XXX_NO_EXPLICIT_TEST_FUNCTION
#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_TEST(args) \
          sizeof(EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)< U >(0)) \
              == sizeof(efl_eina_boost::mpl::aux::yes_tag) \
      /**/
#   else
#     if !EFL_EINA_BOOST_MPL_HAS_XXX_NO_WRAPPED_TYPES
#       define EFL_EINA_BOOST_MPL_HAS_MEMBER_TEST(args) \
          sizeof( \
              EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)( \
                  static_cast< efl_eina_boost::mpl::aux::type_wrapper< U >* >(0) \
              ) \
          ) == sizeof(efl_eina_boost::mpl::aux::yes_tag) \
        /**/
#     else
#       define EFL_EINA_BOOST_MPL_HAS_MEMBER_TEST(args) \
          sizeof( \
              EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)( \
                  static_cast< U* >(0) \
              ) \
          ) == sizeof(efl_eina_boost::mpl::aux::yes_tag) \
        /**/
#     endif
#   endif

#   define EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECT( \
               args, substitute_macro, member_macro \
           ) \
      template< typename U > \
      struct EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args) { \
          EFL_EINA_BOOST_MPL_HAS_MEMBER_SUBSTITUTE(args, substitute_macro) \
          EFL_EINA_BOOST_MPL_HAS_MEMBER_REJECT(args, member_macro) \
          EFL_EINA_BOOST_MPL_HAS_MEMBER_ACCEPT(args, member_macro) \
          EFL_EINA_BOOST_STATIC_CONSTANT( \
              bool, value = EFL_EINA_BOOST_MPL_HAS_MEMBER_TEST(args) \
          ); \
          typedef efl_eina_boost::mpl::bool_< value > type; \
      }; \
    /**/

#   define EFL_EINA_BOOST_MPL_HAS_MEMBER_IMPLEMENTATION( \
               args, introspect_macro, substitute_macro, member_macro \
           ) \
      template< \
          typename T \
        , typename fallback_ \
              = efl_eina_boost::mpl::bool_< EFL_EINA_BOOST_PP_ARRAY_ELEM(3, args) > \
      > \
      class EFL_EINA_BOOST_PP_ARRAY_ELEM(0, args) { \
          introspect_macro(args, substitute_macro, member_macro) \
      public: \
          static const bool value \
              = EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args)< T >::value; \
          typedef typename EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args)< \
              T \
          >::type type; \
      }; \
    /**/

// EFL_EINA_BOOST_MPL_HAS_MEMBER_WITH_FUNCTION_SFINAE expands to the full
// implementation of the function-based metafunction. Compile with -E
// to see the preprocessor output for this macro.
#   define EFL_EINA_BOOST_MPL_HAS_MEMBER_WITH_FUNCTION_SFINAE( \
               args, substitute_macro, member_macro \
           ) \
      EFL_EINA_BOOST_MPL_HAS_MEMBER_IMPLEMENTATION( \
          args \
        , EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECT \
        , substitute_macro \
        , member_macro \
      ) \
    /**/

#   if EFL_EINA_BOOST_MPL_HAS_XXX_NEEDS_TEMPLATE_SFINAE

#     if !defined(EFL_EINA_BOOST_MPL_HAS_XXX_NEEDS_NAMESPACE_LEVEL_SUBSTITUTE)
#       if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1400)
#         define EFL_EINA_BOOST_MPL_HAS_XXX_NEEDS_NAMESPACE_LEVEL_SUBSTITUTE 1
#       endif
#     endif

#     if !EFL_EINA_BOOST_MPL_HAS_XXX_NEEDS_NAMESPACE_LEVEL_SUBSTITUTE
#       define EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                   args, n \
               ) \
          EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) \
        /**/
#     else
#       define EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                   args, n \
               ) \
          EFL_EINA_BOOST_PP_CAT( \
              boost_mpl_has_xxx_ \
            , EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME(args, n) \
          ) \
        /**/
#     endif

#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_TAG_NAME( \
                 args \
             ) \
        EFL_EINA_BOOST_PP_CAT( \
            EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                args, 0 \
            ) \
          , _tag \
        ) \
      /**/

#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_MULTI_SUBSTITUTE_WITH_TEMPLATE_SFINAE( \
                 z, n, args \
             ) \
        template< \
             template< EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_INC(n), typename U) > class U \
        > \
        struct EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                args, n \
               ) { \
            typedef \
                EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_TAG_NAME(args) \
                type; \
        }; \
      /**/

#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_SUBSTITUTE_WITH_TEMPLATE_SFINAE( \
                 args, substitute_macro \
             ) \
        typedef void \
            EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_TAG_NAME(args); \
        EFL_EINA_BOOST_PP_REPEAT( \
            EFL_EINA_BOOST_PP_ARRAY_ELEM(2, args) \
          , EFL_EINA_BOOST_MPL_HAS_MEMBER_MULTI_SUBSTITUTE_WITH_TEMPLATE_SFINAE \
          , args \
        ) \
      /**/

#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_REJECT_WITH_TEMPLATE_SFINAE( \
                 args, member_macro \
             ) \
        template< \
            typename U \
          , typename V \
                = EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_TAG_NAME(args) \
        > \
        struct EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args) { \
            EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false); \
            typedef efl_eina_boost::mpl::bool_< value > type; \
        }; \
      /**/

#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_MULTI_ACCEPT_WITH_TEMPLATE_SFINAE( \
                 z, n, args \
             ) \
        template< typename U > \
        struct EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)< \
            U \
          , typename \
                EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_SUBSTITUTE_NAME_WITH_TEMPLATE_SFINAE( \
                    args, n \
                )< \
                    EFL_EINA_BOOST_MSVC_TYPENAME U::EFL_EINA_BOOST_PP_ARRAY_ELEM(1, args)< > \
                >::type \
        > { \
            EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = true); \
            typedef efl_eina_boost::mpl::bool_< value > type; \
        }; \
      /**/

#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_ACCEPT_WITH_TEMPLATE_SFINAE( \
                 args, member_macro \
             ) \
        EFL_EINA_BOOST_PP_REPEAT( \
            EFL_EINA_BOOST_PP_ARRAY_ELEM(2, args) \
          , EFL_EINA_BOOST_MPL_HAS_MEMBER_MULTI_ACCEPT_WITH_TEMPLATE_SFINAE \
          , args \
        ) \
      /**/

#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECT_WITH_TEMPLATE_SFINAE( \
                 args, substitute_macro, member_macro \
             ) \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_REJECT_WITH_TEMPLATE_SFINAE(args, member_macro) \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_ACCEPT_WITH_TEMPLATE_SFINAE(args, member_macro) \
        template< typename U > \
        struct EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_NAME(args) \
            : EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECTION_TEST_NAME(args)< U > { \
        }; \
      /**/
 
// EFL_EINA_BOOST_MPL_HAS_MEMBER_WITH_TEMPLATE_SFINAE expands to the full
// implementation of the template-based metafunction. Compile with -E
// to see the preprocessor output for this macro.
//
// Note that if EFL_EINA_BOOST_MPL_HAS_XXX_NEEDS_NAMESPACE_LEVEL_SUBSTITUTE is
// defined EFL_EINA_BOOST_MPL_HAS_MEMBER_SUBSTITUTE_WITH_TEMPLATE_SFINAE needs
// to be expanded at namespace level before
// EFL_EINA_BOOST_MPL_HAS_MEMBER_WITH_TEMPLATE_SFINAE can be used.
#     define EFL_EINA_BOOST_MPL_HAS_MEMBER_WITH_TEMPLATE_SFINAE( \
                 args, substitute_macro, member_macro \
             ) \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_SUBSTITUTE_WITH_TEMPLATE_SFINAE( \
            args, substitute_macro \
        ) \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_IMPLEMENTATION( \
            args \
          , EFL_EINA_BOOST_MPL_HAS_MEMBER_INTROSPECT_WITH_TEMPLATE_SFINAE \
          , substitute_macro \
          , member_macro \
        ) \
      /**/

#   endif // EFL_EINA_BOOST_MPL_HAS_XXX_NEEDS_TEMPLATE_SFINAE

// Note: In the current implementation the parameter and access macros
// are no longer expanded.
#   if !EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, <= 1400)
#     define EFL_EINA_BOOST_MPL_HAS_XXX_TEMPLATE_NAMED_DEF(trait, name, default_) \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_WITH_FUNCTION_SFINAE( \
            ( 4, ( trait, name, EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY, default_ ) ) \
          , EFL_EINA_BOOST_MPL_HAS_MEMBER_TEMPLATE_SUBSTITUTE_PARAMETER \
          , EFL_EINA_BOOST_MPL_HAS_MEMBER_TEMPLATE_ACCESS \
        ) \
      /**/
#   else
#     define EFL_EINA_BOOST_MPL_HAS_XXX_TEMPLATE_NAMED_DEF(trait, name, default_) \
        EFL_EINA_BOOST_MPL_HAS_MEMBER_WITH_TEMPLATE_SFINAE( \
            ( 4, ( trait, name, EFL_EINA_BOOST_MPL_LIMIT_METAFUNCTION_ARITY, default_ ) ) \
          , EFL_EINA_BOOST_MPL_HAS_MEMBER_TEMPLATE_SUBSTITUTE_PARAMETER \
          , EFL_EINA_BOOST_MPL_HAS_MEMBER_TEMPLATE_ACCESS \
        ) \
      /**/
#   endif

#else // EFL_EINA_BOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE

// placeholder implementation

#   define EFL_EINA_BOOST_MPL_HAS_XXX_TEMPLATE_NAMED_DEF(trait, name, default_) \
      template< typename T \
              , typename fallback_ = efl_eina_boost::mpl::bool_< default_ > > \
      struct trait { \
          EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = fallback_::value); \
          typedef fallback_ type; \
      }; \
    /**/

#endif // EFL_EINA_BOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE

#   define EFL_EINA_BOOST_MPL_HAS_XXX_TEMPLATE_DEF(name) \
      EFL_EINA_BOOST_MPL_HAS_XXX_TEMPLATE_NAMED_DEF( \
          EFL_EINA_BOOST_PP_CAT(has_, name), name, false \
      ) \
    /**/

#endif // EFL_EINA_BOOST_MPL_HAS_XXX_HPP_INCLUDED
