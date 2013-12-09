
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef EFL_EINA_BOOST_FT_IS_CALLABLE_BUILTIN_HPP_INCLUDED
#define EFL_EINA_BOOST_FT_IS_CALLABLE_BUILTIN_HPP_INCLUDED

#include <eina_boost/mpl/aux_/lambda_support.hpp>
#include <eina_boost/type_traits/detail/template_arity_spec.hpp>

#include <eina_boost/function_types/components.hpp>

namespace efl_eina_boost 
{ 
  namespace function_types 
  {
    template< typename T, typename Tag = null_tag > 
    struct is_callable_builtin
      : function_types::represents
        < function_types::components<T>
        , function_types::tag<Tag, detail::callable_builtin_tag> 
        >
    { 
      EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(2,is_callable_builtin,(T,Tag))
    };
  }
  EFL_EINA_BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(2,function_types::is_callable_builtin)
}

#endif

