
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef EFL_EINA_BOOST_TT_IS_ENUM_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_ENUM_HPP_INCLUDED

#include <eina_boost/type_traits/intrinsics.hpp>
#ifndef EFL_EINA_BOOST_IS_ENUM
#include <eina_boost/type_traits/add_reference.hpp>
#include <eina_boost/type_traits/is_arithmetic.hpp>
#include <eina_boost/type_traits/is_reference.hpp>
#include <eina_boost/type_traits/is_convertible.hpp>
#include <eina_boost/type_traits/is_array.hpp>
#ifdef __GNUC__
#include <eina_boost/type_traits/is_function.hpp>
#endif
#include <eina_boost/type_traits/config.hpp>
#if defined(EFL_EINA_BOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION) 
#  include <eina_boost/type_traits/is_class.hpp>
#  include <eina_boost/type_traits/is_union.hpp>
#endif
#endif

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

#ifndef EFL_EINA_BOOST_IS_ENUM
#if !(defined(__BORLANDC__) && (__BORLANDC__ <= 0x551))

namespace detail {

#if defined(EFL_EINA_BOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION) 

template <typename T>
struct is_class_or_union
{
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, value =
      (::efl_eina_boost::type_traits::ice_or<
           ::efl_eina_boost::is_class<T>::value
         , ::efl_eina_boost::is_union<T>::value
      >::value));
};

#else

template <typename T>
struct is_class_or_union
{
# if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, < 1300) || EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x581))// we simply can't detect it this way.
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
# else
    template <class U> static ::efl_eina_boost::type_traits::yes_type is_class_or_union_tester(void(U::*)(void));

#  if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC, == 1300)                 \
    || EFL_EINA_BOOST_WORKAROUND(__MWERKS__, <= 0x3000) // no SFINAE
    static ::efl_eina_boost::type_traits::no_type is_class_or_union_tester(...);
    EFL_EINA_BOOST_STATIC_CONSTANT(
        bool, value = sizeof(is_class_or_union_tester(0)) == sizeof(::efl_eina_boost::type_traits::yes_type));
#  else
    template <class U>
    static ::efl_eina_boost::type_traits::no_type is_class_or_union_tester(...);
    EFL_EINA_BOOST_STATIC_CONSTANT(
        bool, value = sizeof(is_class_or_union_tester<T>(0)) == sizeof(::efl_eina_boost::type_traits::yes_type));
#  endif
# endif
};
#endif

struct int_convertible
{
    int_convertible(int);
};

// Don't evaluate convertibility to int_convertible unless the type
// is non-arithmetic. This suppresses warnings with GCC.
template <bool is_typename_arithmetic_or_reference = true>
struct is_enum_helper
{
    template <typename T> struct type
    {
        EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = false);
    };
};

template <>
struct is_enum_helper<false>
{
    template <typename T> struct type
       : public ::efl_eina_boost::is_convertible<typename efl_eina_boost::add_reference<T>::type,::efl_eina_boost::detail::int_convertible>
    {
    };
};

template <typename T> struct is_enum_impl
{
   //typedef ::efl_eina_boost::add_reference<T> ar_t;
   //typedef typename ar_t::type r_type;

#if defined(__GNUC__)

#ifdef EFL_EINA_BOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION
    
   // We MUST check for is_class_or_union on conforming compilers in
   // order to correctly deduce that noncopyable types are not enums
   // (dwa 2002/04/15)...
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, selector =
      (::efl_eina_boost::type_traits::ice_or<
           ::efl_eina_boost::is_arithmetic<T>::value
         , ::efl_eina_boost::is_reference<T>::value
         , ::efl_eina_boost::is_function<T>::value
         , is_class_or_union<T>::value
         , is_array<T>::value
      >::value));
#else
   // ...however, not checking is_class_or_union on non-conforming
   // compilers prevents a dependency recursion.
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, selector =
      (::efl_eina_boost::type_traits::ice_or<
           ::efl_eina_boost::is_arithmetic<T>::value
         , ::efl_eina_boost::is_reference<T>::value
         , ::efl_eina_boost::is_function<T>::value
         , is_array<T>::value
      >::value));
#endif // EFL_EINA_BOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION

#else // !defined(__GNUC__):
    
   EFL_EINA_BOOST_STATIC_CONSTANT(bool, selector =
      (::efl_eina_boost::type_traits::ice_or<
           ::efl_eina_boost::is_arithmetic<T>::value
         , ::efl_eina_boost::is_reference<T>::value
         , is_class_or_union<T>::value
         , is_array<T>::value
      >::value));
    
#endif

#if EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, < 0x600)
    typedef ::efl_eina_boost::detail::is_enum_helper<
          ::efl_eina_boost::detail::is_enum_impl<T>::selector
        > se_t;
#else
    typedef ::efl_eina_boost::detail::is_enum_helper<selector> se_t;
#endif

    typedef typename se_t::template type<T> helper;
    EFL_EINA_BOOST_STATIC_CONSTANT(bool, value = helper::value);
};

// these help on compilers with no partial specialization support:
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_enum,void,false)
#ifndef EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_enum,void const,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_enum,void volatile,false)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_enum,void const volatile,false)
#endif

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_enum,T,::efl_eina_boost::detail::is_enum_impl<T>::value)

#else // __BORLANDC__
//
// buggy is_convertible prevents working
// implementation of is_enum:
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_enum,T,false)

#endif

#else // EFL_EINA_BOOST_IS_ENUM

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_enum,T,EFL_EINA_BOOST_IS_ENUM(T))

#endif

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_ENUM_HPP_INCLUDED
