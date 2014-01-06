
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


#ifndef EFL_EINA_BOOST_TT_IS_VOLATILE_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_VOLATILE_HPP_INCLUDED

#include <eina_boost/config.hpp>
#include <eina_boost/detail/workaround.hpp>

#ifndef EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#   include <eina_boost/type_traits/detail/cv_traits_impl.hpp>
#   if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, < 1400)
#       include <eina_boost/type_traits/remove_bounds.hpp>
#   endif
#else
#   include <eina_boost/type_traits/is_reference.hpp>
#   include <eina_boost/type_traits/is_array.hpp>
#   include <eina_boost/type_traits/detail/yes_no_type.hpp>
#   include <eina_boost/type_traits/detail/false_result.hpp>
#endif

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

namespace detail{
template <class T>
struct is_volatile_rval_filter
{
#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, < 1400)
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = ::efl_eina_boost::detail::cv_traits_imp<typename efl_eina_boost::remove_bounds<T>::type*>::is_volatile);
#else
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = ::efl_eina_boost::detail::cv_traits_imp<T*>::is_volatile);
#endif
};
#ifndef EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
//
// We can't filter out rvalue_references at the same level as
// references or we get ambiguities from msvc:
//
template <class T>
struct is_volatile_rval_filter<T&&>
{
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
};
#endif
}

#if defined( __CODEGEARC__ )
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_volatile,T,__is_volatile(T))
#elif !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

//* is a type T declared volatile - is_volatile<T>
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_volatile,T,::efl_eina_boost::detail::is_volatile_rval_filter<T>::value)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_volatile,T&,false)

#if  defined(EFL_EINA_BOOST_ILLEGAL_CV_REFERENCES)
// these are illegal specialisations; cv-qualifies applied to
// references have no effect according to [8.3.2p1],
// C++ Builder requires them though as it treats cv-qualified
// references as distinct types...
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_volatile,T& const,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_volatile,T& volatile,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_volatile,T& const volatile,false)
#endif

#else

namespace detail {

using ::efl_eina_boost::type_traits::yes_type;
using ::efl_eina_boost::type_traits::no_type;

yes_type is_volatile_tester(void const volatile*);
no_type is_volatile_tester(void const*);

template <bool is_ref, bool array>
struct is_volatile_helper
    : public ::efl_eina_boost::type_traits::false_result
{
};

template <>
struct is_volatile_helper<false,false>
{
    template <typename T> struct result_
    {
        static T* t;
        EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = (
            sizeof(efl_eina_boost::detail::yes_type) == sizeof(efl_eina_boost::detail::is_volatile_tester(t))
            ));
    };
};

template <>
struct is_volatile_helper<false,true>
{
    template <typename T> struct result_
    {
        static T t;
        EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = (
            sizeof(efl_eina_boost::detail::yes_type) == sizeof(efl_eina_boost::detail::is_volatile_tester(&t))
            ));
    };
};

template <typename T>
struct is_volatile_impl
    : public is_volatile_helper<
          is_reference<T>::value
        , is_array<T>::value
        >::template result_<T>
{
};

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_volatile,void,false)
#ifndef EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_volatile,void const,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_volatile,void volatile,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_volatile,void const volatile,true)
#endif

} // namespace detail

//* is a type T declared volatile - is_volatile<T>
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_volatile,T,::efl_eina_boost::detail::is_volatile_impl<T>::value)

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_VOLATILE_HPP_INCLUDED
