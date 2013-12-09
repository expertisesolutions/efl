
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef EFL_EINA_BOOST_FT_PARAMETER_TYPES_HPP_INCLUDED
#define EFL_EINA_BOOST_FT_PARAMETER_TYPES_HPP_INCLUDED

#include <eina_boost/blank.hpp>
#include <eina_boost/mpl/if.hpp>

#include <eina_boost/mpl/aux_/lambda_support.hpp>
#include <eina_boost/type_traits/detail/template_arity_spec.hpp>

#include <eina_boost/mpl/pop_front.hpp>

#include <eina_boost/function_types/is_callable_builtin.hpp>
#include <eina_boost/function_types/components.hpp>

namespace efl_eina_boost 
{ 
  namespace function_types 
  {
    using mpl::placeholders::_;
 
    template< typename T, typename ClassTypeTransform = add_reference<_> >
    struct parameter_types;

    namespace detail
    {
      template<typename T, typename ClassTypeTransform> 
      struct parameter_types_impl
        : mpl::pop_front
          < typename function_types::components<T,ClassTypeTransform>::types 
          >::type
      { };
    }

    template<typename T, typename ClassTypeTransform> struct parameter_types
      : mpl::if_
        < function_types::is_callable_builtin<T>
        , detail::parameter_types_impl<T,ClassTypeTransform>, efl_eina_boost::blank
        >::type
    {
      EFL_EINA_BOOST_MPL_AUX_LAMBDA_SUPPORT(2,parameter_types,(T,ClassTypeTransform)) 
    };
  }
  EFL_EINA_BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(2,function_types::parameter_types)
}

#endif

