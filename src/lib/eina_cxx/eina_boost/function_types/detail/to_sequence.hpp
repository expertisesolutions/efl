
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef EFL_EINA_BOOST_FT_DETAIL_TO_SEQUENCE_HPP_INCLUDED
#define EFL_EINA_BOOST_FT_DETAIL_TO_SEQUENCE_HPP_INCLUDED

#include <eina_boost/mpl/eval_if.hpp>
#include <eina_boost/mpl/identity.hpp>
#include <eina_boost/mpl/is_sequence.hpp>
#include <eina_boost/mpl/placeholders.hpp>
#include <eina_boost/type_traits/add_reference.hpp>

#include <eina_boost/function_types/is_callable_builtin.hpp>

namespace efl_eina_boost { namespace function_types { namespace detail {

// wrap first arguments in components, if callable builtin type
template<typename T>
struct to_sequence
{
  typedef typename
   mpl::eval_if
   < is_callable_builtin<T>
   , to_sequence< components<T> >
   , mpl::identity< T >
   >::type
  type;
};

#ifndef EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
// reduce template instantiations, if possible
template<typename T, typename U>
struct to_sequence< components<T,U> > 
{
  typedef typename components<T,U>::types type;
};
#endif

} } } // namespace ::efl_eina_boost::function_types::detail

#endif

