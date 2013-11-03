
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_IS_POD_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_POD_HPP_INCLUDED

#include <eina_boost/type_traits/config.hpp>
#include <eina_boost/type_traits/is_void.hpp>
#include <eina_boost/type_traits/is_scalar.hpp>
#include <eina_boost/type_traits/detail/ice_or.hpp>
#include <eina_boost/type_traits/intrinsics.hpp>

#include <cstddef>

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

#ifndef EFL_EINA_BOOST_IS_POD
#define EFL_EINA_BOOST_INTERNAL_IS_POD(T) false
#else
#define EFL_EINA_BOOST_INTERNAL_IS_POD(T) EFL_EINA_BOOST_IS_POD(T)
#endif

namespace efl_eina_boost {

// forward declaration, needed by 'is_pod_array_helper' template below
template< typename T > struct is_POD;

namespace detail {

#ifndef EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template <typename T> struct is_pod_impl
{ 
    EFL_EINA_BOOST_STATIC_CONSTANT(
        bool, value =
        (::efl_eina_boost::type_traits::ice_or<
            ::efl_eina_boost::is_scalar<T>::value,
            ::efl_eina_boost::is_void<T>::value,
            EFL_EINA_BOOST_INTERNAL_IS_POD(T)
         >::value));
};

#if !defined(EFL_EINA_BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
template <typename T, std::size_t sz>
struct is_pod_impl<T[sz]>
    : public is_pod_impl<T>
{
};
#endif

#else

template <bool is_array = false>
struct is_pod_helper
{
    template <typename T> struct result_
    {
        EFL_EINA_BOOST_STATIC_CONSTANT(
            bool, value =
            (::efl_eina_boost::type_traits::ice_or<
                ::efl_eina_boost::is_scalar<T>::value,
                ::efl_eina_boost::is_void<T>::value,
                EFL_EINA_BOOST_INTERNAL_IS_POD(T)
            >::value));
    };
};

template <bool b>
struct bool_to_yes_no_type
{
    typedef ::efl_eina_boost::type_traits::no_type type;
};

template <>
struct bool_to_yes_no_type<true>
{
    typedef ::efl_eina_boost::type_traits::yes_type type;
};

template <typename ArrayType>
struct is_pod_array_helper
{
    enum { is_pod = ::efl_eina_boost::is_POD<ArrayType>::value }; // MSVC workaround
    typedef typename bool_to_yes_no_type<is_pod>::type type;
    type instance() const;
};

template <typename T>
is_pod_array_helper<T> is_POD_array(T*);

template <>
struct is_pod_helper<true>
{
    template <typename T> struct result_
    {
        static T& help();
        EFL_EINA_BOOST_STATIC_CONSTANT(bool, value =
            sizeof(is_POD_array(help()).instance()) == sizeof(::efl_eina_boost::type_traits::yes_type)
            );
    };
};


template <typename T> struct is_pod_impl
{ 
   EFL_EINA_BOOST_STATIC_CONSTANT(
       bool, value = (
           ::efl_eina_boost::detail::is_pod_helper<
              ::efl_eina_boost::is_array<T>::value
           >::template result_<T>::value
           )
       );
};

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

// the following help compilers without partial specialization support:
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pod,void,true)

#ifndef EFL_EINA_BOOST_NO_CV_VOID_SPECIALIZATIONS
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pod,void const,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pod,void volatile,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_pod,void const volatile,true)
#endif

} // namespace detail

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_pod,T,::efl_eina_boost::detail::is_pod_impl<T>::value)
// is_POD is the old depricated name for this trait, do not use this as it may
// be removed in future without warning!!
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_POD,T,::efl_eina_boost::is_pod<T>::value)

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#undef EFL_EINA_BOOST_INTERNAL_IS_POD

#endif // EFL_EINA_BOOST_TT_IS_POD_HPP_INCLUDED
