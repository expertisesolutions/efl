
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef EFL_EINA_BOOST_TT_REMOVE_CV_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_REMOVE_CV_HPP_INCLUDED

#include <eina_boost/type_traits/broken_compiler_spec.hpp>
#include <eina_boost/type_traits/detail/cv_traits_impl.hpp>
#include <eina_boost/config.hpp>
#include <eina_boost/detail/workaround.hpp>

#include <cstddef>

#if EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC,<=1300)
#include <eina_boost/type_traits/msvc/remove_cv.hpp>
#endif

// should be the last #include
#include <eina_boost/type_traits/detail/type_trait_def.hpp>

namespace efl_eina_boost {

#ifndef EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace detail{

template <class T>
struct rvalue_ref_filter_rem_cv
{
   typedef typename efl_eina_boost::detail::cv_traits_imp<T*>::unqualified_type type;
};

#ifndef EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
//
// We can't filter out rvalue_references at the same level as
// references or we get ambiguities from msvc:
//
template <class T>
struct rvalue_ref_filter_rem_cv<T&&>
{
   typedef T&& type;
};
#endif

}


//  convert a type T to a non-cv-qualified type - remove_cv<T>
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_DEF1(remove_cv,T,typename efl_eina_boost::detail::rvalue_ref_filter_rem_cv<T>::type)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,remove_cv,T&,T&)
#if !defined(EFL_EINA_BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS)
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_cv,T const[N],T type[N])
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_cv,T volatile[N],T type[N])
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_2(typename T,std::size_t N,remove_cv,T const volatile[N],T type[N])
#endif

#elif !EFL_EINA_BOOST_WORKAROUND(EFL_EINA_BOOST_MSVC,<=1300)

namespace detail {
template <typename T>
struct remove_cv_impl
{
    typedef typename remove_volatile_impl< 
          typename remove_const_impl<T>::type
        >::type type;
};
}

EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_DEF1(remove_cv,T,typename efl_eina_boost::detail::remove_cv_impl<T>::type)

#endif // EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/type_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_REMOVE_CV_HPP_INCLUDED
