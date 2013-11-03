//  Copyright 2010 John Maddock

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef EFL_EINA_BOOST_TYPE_TRAITS_EXT_ADD_LVALUE_REFERENCE__HPP
#define EFL_EINA_BOOST_TYPE_TRAITS_EXT_ADD_LVALUE_REFERENCE__HPP

#include <eina_boost/type_traits/add_reference.hpp>

// should be the last #include
#include <eina_boost/type_traits/detail/type_trait_def.hpp>

namespace efl_eina_boost{

EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_DEF1(add_lvalue_reference,T,typename efl_eina_boost::add_reference<T>::type)

#ifndef EFL_EINA_BOOST_NO_CXX11_RVALUE_REFERENCES
EFL_EINA_BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,add_lvalue_reference,T&&,T&)
#endif

}

#include <eina_boost/type_traits/detail/type_trait_undef.hpp>

#endif  // EFL_EINA_BOOST_TYPE_TRAITS_EXT_ADD_LVALUE_REFERENCE__HPP
