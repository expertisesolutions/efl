
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef EFL_EINA_BOOST_FT_FUNCTION_ARITY_HPP_INCLUDED
#define EFL_EINA_BOOST_FT_FUNCTION_ARITY_HPP_INCLUDED

#include <eina_boost/blank.hpp>
#include <eina_boost/mpl/if.hpp>

#include <eina_boost/mpl/aux_/lambda_support.hpp>
#include <eina_boost/type_traits/detail/template_arity_spec.hpp>

#include <eina_boost/function_types/is_callable_builtin.hpp>
#include <eina_boost/function_types/components.hpp>

namespace efl_eina_boost 
{ 
  namespace function_types 
  {
    template<typename T> struct function_arity
      : mpl::if_
        < function_types::is_callable_builtin<T>
        , typename components<T>::function_arity, efl_eina_boost::blank
        >::type
    {
      EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(1,function_arity,(T)) 
    };
  }
  EFL_EINA_BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1,function_types::function_arity)
}

#endif

