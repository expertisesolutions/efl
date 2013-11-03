
//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef EFL_EINA_BOOST_TT_IS_INTEGRAL_HPP_INCLUDED
#define EFL_EINA_BOOST_TT_IS_INTEGRAL_HPP_INCLUDED

#include <eina_boost/config.hpp>

// should be the last #include
#include <eina_boost/type_traits/detail/bool_trait_def.hpp>

namespace efl_eina_boost {

//* is a type T an [cv-qualified-] integral type described in the standard (3.9.1p3)
// as an extension we include long long, as this is likely to be added to the
// standard at a later date
#if defined( __CODEGEARC__ )
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_integral,T,__is_integral(T))
#else
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_integral,T,false)

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,unsigned char,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,unsigned short,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,unsigned int,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,unsigned long,true)

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,signed char,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,signed short,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,signed int,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,signed long,true)

EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,bool,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,char,true)

#ifndef EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T
// If the following line fails to compile and you're using the Intel
// compiler, see http://lists.boost.org/MailArchives/boost-users/msg06567.php,
// and define EFL_EINA_BOOST_NO_INTRINSIC_WCHAR_T on the command line.
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,wchar_t,true)
#endif

// Same set of integral types as in boost/type_traits/integral_promotion.hpp.
// Please, keep in sync. -- Alexander Nasonov
#if (defined(EFL_EINA_BOOST_MSVC) && (EFL_EINA_BOOST_MSVC < 1300)) \
    || (defined(EFL_EINA_BOOST_INTEL_CXX_VERSION) && defined(_MSC_VER) && (EFL_EINA_BOOST_INTEL_CXX_VERSION <= 600)) \
    || (defined(__BORLANDC__) && (__BORLANDC__ == 0x600) && (_MSC_VER < 1300))
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,unsigned __int8,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,__int8,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,unsigned __int16,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,__int16,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,unsigned __int32,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,__int32,true)
#ifdef __BORLANDC__
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,unsigned __int64,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,__int64,true)
#endif
#endif

# if defined(EFL_EINA_BOOST_HAS_LONG_LONG)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral, ::efl_eina_boost::ulong_long_type,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral, ::efl_eina_boost::long_long_type,true)
#elif defined(EFL_EINA_BOOST_HAS_MS_INT64)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,unsigned __int64,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,__int64,true)
#endif
        
#ifdef EFL_EINA_BOOST_HAS_INT128
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,efl_eina_boost::int128_type,true)
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,efl_eina_boost::uint128_type,true)
#endif
#ifndef EFL_EINA_BOOST_NO_CXX11_CHAR16_T
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,char16_t,true)
#endif
#ifndef EFL_EINA_BOOST_NO_CXX11_CHAR32_T
EFL_EINA_BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_integral,char32_t,true)
#endif

#endif  // non-CodeGear implementation

} // namespace efl_eina_boost

#include <eina_boost/type_traits/detail/bool_trait_undef.hpp>

#endif // EFL_EINA_BOOST_TT_IS_INTEGRAL_HPP_INCLUDED
