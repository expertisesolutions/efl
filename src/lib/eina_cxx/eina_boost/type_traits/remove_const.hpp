
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef EFL_EINA_BOOST_TT_REMOVE_CONST_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_REMOVE_CONST_HPP_INCLUDED

#include <eina_boost/type_traits/is_volatile.hpp>
#include <eina_boost/type_traits/broken_compiler_spec.hpp>
#include <eina_boost/type_traits/detail/cv_traits_impl.hpp>
#include <eina_boost/config.hpp>
#include <eina_boost/detail/workaround.hpp>

#include <cstddef>

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC,<=1300)
#include <eina_boost/type_traits/msvc/remove_const.hpp>
#endif

// should be the last #include
#include <eina_boost/type_traits/detail/type_trait_def.hpp>

namespace efl_eina_boost {

#ifndef EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail {

template <typename T, bool is_vol>
struct remove_const_helper
{
    typedef T type;
};

template <typename T>
struct remove_const_helper<T, true>
{
    typedef T volatile type;
};


template <typename T>
struct remove_const_impl
{
    typedef typename remove_const_helper<
          typename cv_traits_imp<T*>::unqualified_type
        , ::efl_eina_boost::is_volatile<T>::value
        >::type type;
};

#ifndef EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
//
// We can't filter out rvalue_references at the same level as
// references or we get ambiguities from msvc:
//
template <typename T>
struct remove_const_impl<T&&>
{
    typedef T&& type;
};
#endif

} // namespace detail

// * convert a type T to non-const type - remove_const<T>

EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_DEF1(remove_const,T,typename efl_eina_boost::detail::remove_const_impl<T>::type)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,remove_const,T&,T&)
#if !defined(EFL_EINA_BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_const,T const[N],T type[N])
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_const,T const volatile[N],T volatile type[N])
#endif

#elif !EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC,<=1300)

EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_DEF1(remove_const,T,typename efl_eina_boost::detail::remove_const_impl<T>::type)

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/type_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_REMOVE_CONST_HPP_INCLUDED
