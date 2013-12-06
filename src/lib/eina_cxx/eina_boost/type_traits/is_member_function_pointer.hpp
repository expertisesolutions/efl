
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef EFL_EINA_BOOST_TT_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED

#include <eina_boost/type_traits/config.hpp>
#include <eina_boost/detail/workaround.hpp>

#if !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
   && !EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, < 0x600) && !defined(EFL_EINA_BOOST_TT_TEST_MS_FUNC_SIGS)
   //
   // Note: we use the "workaround" version for MSVC because it works for 
   // __stdcall etc function types, where as the partial specialisation
   // version does not do so.
   //
#   include <eina_boost/type_traits/detail/is_mem_fun_pointer_impl.hpp>
#   include <eina_boost/type_traits/remove_cv.hpp>
#else
#   include <eina_boost/type_traits/is_reference.hpp>
#   include <eina_boost/type_traits/is_array.hpp>
#   include <eina_boost/type_traits/detail/yes_no_type.hpp>
#   include <eina_boost/type_traits/detail/false_result.hpp>
#   include <eina_boost/type_traits/detail/ice_or.hpp>
#   include <eina_boost/type_traits/detail/is_mem_fun_pointer_tester.hpp>
#endif

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

#if defined( __CODEGEARC__ )
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_function_pointer,T,__is_member_function_pointer( T ))
#elif !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, < 0x600) && !defined(EFL_EINA_BOOST_TT_TEST_MS_FUNC_SIGS)

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(
      is_member_function_pointer
    , T
    , ::efl_eina_boost::type_traits::is_mem_fun_pointer_impl<typename remove_cv<T>::type>::value
    )

#else

namespace detail {

#ifndef __BORLANDC__

template <bool>
struct is_mem_fun_pointer_select
    : public ::efl_eina_boost::type_traits::false_result
{
};

template <>
struct is_mem_fun_pointer_select<false>
{
    template <typename T> struct result_
    {
#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC_FULL_VER, >= 140050000)
#pragma warning(push)
#pragma warning(disable:6334)
#endif
        static T* make_t;
        typedef result_<T> self_type;

        EFL_EINA_BOOST_STATIC_CONSTANT(
            bool, value = (
                1 == sizeof(::efl_eina_boost::type_traits::is_mem_fun_pointer_tester(self_type::make_t))
            ));
#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC_FULL_VER, >= 140050000)
#pragma warning(pop)
#endif
    };
};

template <typename T>
struct is_member_function_pointer_impl
    : public is_mem_fun_pointer_select<
          ::efl_eina_boost::type_traits::ice_or<
              ::efl_eina_boost::is_reference<T>::value
            , ::efl_eina_boost::is_array<T>::value
            >::value
        >::template result_<T>
{
};

#ifndef EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <typename T>
struct is_member_function_pointer_impl<T&> : public false_type{};
#endif

#else // Borland C++

template <typename T>
struct is_member_function_pointer_impl
{
   static T* m_t;
   EFL_EINA_BOOST_STATIC_CONSTANT(
              bool, value =
               (1 == sizeof(type_traits::is_mem_fun_pointer_tester(m_t))) );
};

template <typename T>
struct is_member_function_pointer_impl<T&>
{
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
};

#endif

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_function_pointer,void,false)
#ifndef EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_function_pointer,void const,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_function_pointer,void volatile,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_member_function_pointer,void const volatile,false)
#endif

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_function_pointer,T,::efl_eina_boost::detail::is_member_function_pointer_impl<T>::value)

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
