
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, 
//      Howard Hinnant and John Maddock 2000. 
//  (C) Copyright Mat Marcus, Jesse Jones and Adobe Systems Inc 2001

//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

//    Fixed is_pointer, is_reference, is_const, is_volatile, is_same, 
//    is_member_pointer based on the Simulated Partial Specialization work 
//    of Mat Marcus and Jesse Jones. See  http://opensource.adobe.com or 
//    http://groups.yahoo.com/group/boost/message/5441 
//    Some workarounds in here use ideas suggested from "Generic<Programming>: 
//    Mappings between Types and Values" 
//    by Andrei Alexandrescu (see http://www.cuj.com/experts/1810/alexandr.html).


#ifndef EFL_EINA_BOOST_TT_IS_POINTER_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_POINTER_HPP_INCLUDED

#include <eina_boost/type_traits/is_member_pointer.hpp>
#include <eina_boost/type_traits/detail/ice_and.hpp>
#include <eina_boost/type_traits/detail/ice_not.hpp>
#include <eina_boost/type_traits/config.hpp>
#if !EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC,<=1300)
#include <eina_boost/type_traits/remove_cv.hpp>
#endif

#ifdef EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#   include <eina_boost/type_traits/is_reference.hpp>
#   include <eina_boost/type_traits/is_array.hpp>
#   include <eina_boost/type_traits/detail/is_function_ptr_tester.hpp>
#   include <eina_boost/type_traits/detail/false_result.hpp>
#   include <eina_boost/type_traits/detail/ice_or.hpp>
#endif

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

#if defined( __CODEGEARC__ )
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_pointer,T,__is_pointer(T))
#elif !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace detail {

template< typename T > struct is_pointer_helper
{
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
};

#   define TT_AUX_BOOL_TRAIT_HELPER_PARTIAL_SPEC(helper,sp,result) \
template< typename T > struct helper<sp> \
{ \
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = result); \
}; \
/**/

TT_AUX_BOOL_TRAIT_HELPER_PARTIAL_SPEC(is_pointer_helper,T*,true)

#   undef TT_AUX_BOOL_TRAIT_HELPER_PARTIAL_SPEC

template< typename T >
struct is_pointer_impl
{
#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC,<=1300)
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value =
        (::efl_eina_boost::type_traits::ice_and<
              ::efl_eina_boost::detail::is_pointer_helper<T>::value
            , ::efl_eina_boost::type_traits::ice_not<
                ::efl_eina_boost::is_member_pointer<T>::value
                >::value
            >::value)
        );
#else
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value =
        (::efl_eina_boost::type_traits::ice_and<
        ::efl_eina_boost::detail::is_pointer_helper<typename remove_cv<T>::type>::value
            , ::efl_eina_boost::type_traits::ice_not<
                ::efl_eina_boost::is_member_pointer<T>::value
                >::value
            >::value)
        );
#endif
};

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_pointer,T,::efl_eina_boost::detail::is_pointer_impl<T>::value)

#if defined(__BORLANDC__) && !defined(__COMO__) && (__BORLANDC__ < 0x600)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_pointer,T&,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_pointer,T& const,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_pointer,T& volatile,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_pointer,T& const volatile,false)
#endif

#else // no partial template specialization

namespace detail {

struct pointer_helper
{
    pointer_helper(const volatile void*);
};

yes_type EFL_EINA_BOOST_TT_DECL is_pointer_tester(pointer_helper);
no_type EFL_EINA_BOOST_TT_DECL is_pointer_tester(...);

template <bool>
struct is_pointer_select
    : public ::efl_eina_boost::type_traits::false_result
{
};

template <>
struct is_pointer_select<false>
{
    template <typename T> struct result_
    {
        static T& make_t();
        EFL_EINA_BOOST_STATIC_CONSTANT(bool, value =
                (::efl_eina_boost::type_traits::ice_or<
                    (1 == sizeof(is_pointer_tester(make_t()))),
                    (1 == sizeof(type_traits::is_function_ptr_tester(make_t())))
                >::value));
    };
};

template <typename T>
struct is_pointer_impl
    : public is_pointer_select<
          ::efl_eina_boost::type_traits::ice_or<
              ::efl_eina_boost::is_reference<T>::value
            , ::efl_eina_boost::is_array<T>::value
            >::value
        >::template result_<T>
{
};

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pointer,void,false)
#ifndef EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pointer,void const,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pointer,void volatile,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pointer,void const volatile,false)
#endif

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_pointer,T,::efl_eina_boost::detail::is_pointer_impl<T>::value)

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_POINTER_HPP_INCLUDED
