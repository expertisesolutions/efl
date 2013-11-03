
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef EFL_EINA_BOOST_FT_DETAIL_CLASSIFIER_HPP_INCLUDED
#define EFL_EINA_BOOST_FT_DETAIL_CLASSIFIER_HPP_INCLUDED

#include <eina_boost/type.hpp>
#include <eina_boost/config.hpp>
#include <eina_boost/type_traits/config.hpp>
#include <eina_boost/type_traits/is_reference.hpp>
#include <eina_boost/type_traits/add_reference.hpp>

#include <eina_boost/function_types/config/config.hpp>
#include <eina_boost/function_types/property_tags.hpp>

namespace efl_eina_boost { namespace function_types { namespace detail {

template<typename T> struct classifier;

template<std::size_t S> struct char_array { typedef char (&type)[S]; };

template<bits_t Flags, bits_t CCID, std::size_t Arity> struct encode_charr
{
  typedef typename char_array<
    ::efl_eina_boost::function_types::detail::encode_charr_impl<Flags,CCID,Arity>::value 
  >::type type;
};

char EFL_EINA_BOOST_TT_DECL classifier_impl(...);

#define EFL_EINA_BOOST_FT_variations EFL_EINA_BOOST_FT_function|EFL_EINA_BOOST_FT_pointer|\
                            EFL_EINA_BOOST_FT_member_pointer

#define EFL_EINA_BOOST_FT_type_function(cc,name) EFL_EINA_BOOST_FT_SYNTAX( \
    R EFL_EINA_BOOST_PP_EMPTY,EFL_EINA_BOOST_PP_LPAREN,cc,* EFL_EINA_BOOST_PP_EMPTY,name,EFL_EINA_BOOST_PP_RPAREN)

#define EFL_EINA_BOOST_FT_type_function_pointer(cc,name) EFL_EINA_BOOST_FT_SYNTAX( \
    R EFL_EINA_BOOST_PP_EMPTY,EFL_EINA_BOOST_PP_LPAREN,cc,** EFL_EINA_BOOST_PP_EMPTY,name,EFL_EINA_BOOST_PP_RPAREN)

#define EFL_EINA_BOOST_FT_type_member_function_pointer(cc,name) EFL_EINA_BOOST_FT_SYNTAX( \
    R EFL_EINA_BOOST_PP_EMPTY,EFL_EINA_BOOST_PP_LPAREN,cc,T0::** EFL_EINA_BOOST_PP_EMPTY,name,EFL_EINA_BOOST_PP_RPAREN)

#define EFL_EINA_BOOST_FT_al_path eina_boost/function_types/detail/classifier_impl
#include <eina_boost/function_types/detail/pp_loop.hpp>

template<typename T> struct classifier_bits
{
  static typename efl_eina_boost::add_reference<T>::type tester;

  EFL_EINA_BOOST_STATIC_CONSTANT(bits_t,value = (bits_t)sizeof(
    efl_eina_boost::function_types::detail::classifier_impl(& tester) 
  )-1);
};

template<typename T> struct classifier
{
  typedef detail::constant<
    ::efl_eina_boost::function_types::detail::decode_bits<
      ::efl_eina_boost::function_types::detail::classifier_bits<T>::value
    >::tag_bits > 
  bits;

  typedef detail::full_mask mask;
 
  typedef detail::constant<
    ::efl_eina_boost::function_types::detail::decode_bits<
      ::efl_eina_boost::function_types::detail::classifier_bits<T>::value
    >::arity > 
  function_arity;
};



} } } // namespace ::efl_eina_boost::function_types::detail

#endif

