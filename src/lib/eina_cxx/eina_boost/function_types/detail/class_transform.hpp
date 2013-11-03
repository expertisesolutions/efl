
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef EFL_EINA_BOOST_FT_DETAIL_CLASS_TRANSFORM_HPP_INCLUDED
#define EFL_EINA_BOOST_FT_DETAIL_CLASS_TRANSFORM_HPP_INCLUDED

#include <eina_boost/mpl/apply.hpp>
#include <eina_boost/mpl/always.hpp>
#include <eina_boost/mpl/identity.hpp>
#include <eina_boost/mpl/placeholders.hpp>

#include <eina_boost/type_traits/remove_cv.hpp>
#include <eina_boost/type_traits/add_pointer.hpp>
#include <eina_boost/type_traits/add_reference.hpp>

namespace efl_eina_boost { namespace function_types { namespace detail {

using mpl::placeholders::_;

// Transformation metafunction for the class type of member function pointers.
template<typename T, typename L>
struct class_transform
{ typedef typename mpl::apply1<L,T>::type type; };


#ifndef EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
// We can short-circuit the mechanism implemented in the primary template for 
// the most common lambda expression and save both the "un-lambdaing" and the
// type traits invocation (we know that T can only be a class type).

template<typename T> struct class_transform< T, mpl::identity<_> >
{ typedef T type; };

template<typename T> struct class_transform< T, add_reference<_> > 
{ typedef T & type; };

template<typename T> struct class_transform< T, add_pointer<_> >
{ typedef T * type; };

template<typename T> struct class_transform< T, remove_cv<_> >
{ typedef typename efl_eina_boost::remove_cv<T>::type type; };

template<typename T> struct class_transform< T, add_reference< remove_cv<_> > >
{ typedef typename efl_eina_boost::remove_cv<T>::type & type; };

template<typename T> struct class_transform< T, add_pointer< remove_cv<_> > >
{ typedef typename efl_eina_boost::remove_cv<T>::type * type; };

template<typename T, typename U> struct class_transform< T, mpl::always<U> >
{ typedef U type; };
#endif


} } } // namespace ::efl_eina_boost::function_types::detail

#endif

