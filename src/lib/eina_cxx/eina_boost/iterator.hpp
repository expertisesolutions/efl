//  iterator.hpp workarounds for non-conforming standard libraries  ---------//

//  (C) Copyright Beman Dawes 2000. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/utility for documentation.

//  Revision History
//  12 Jan 01 added <cstddef> for std::ptrdiff_t (Jens Maurer)
//  28 Jun 00 Workarounds to deal with known MSVC bugs (David Abrahams)
//  26 Jun 00 Initial version (Jeremy Siek)

#ifndef EFL_EINA_BOOST_ITERATOR_HPP
#define EFL_EINA_BOOST_ITERATOR_HPP

#include <iterator>
#include <cstddef>           // std::ptrdiff_t
#include <eina_boost/config.hpp>

namespace efl_eina_boost
{
# if defined(EFL_EINA_BOOST_NO_STD_ITERATOR) && !defined(EFL_EINA_BOOST_MSVC_STD_ITERATOR)
  template <class Category, class T,
    class Distance = std::ptrdiff_t,
    class Pointer = T*, class Reference = T&>
  struct iterator
  {
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
    typedef Category  iterator_category;
  };
# else

  // declare iterator_base in namespace detail to work around MSVC bugs which
  // prevent derivation from an identically-named class in a different namespace.
  namespace detail {
   template <class Category, class T, class Distance, class Pointer, class Reference>
#  if !defined(EFL_EINA_BOOST_MSVC_STD_ITERATOR)
   struct iterator_base : std::iterator<Category, T, Distance, Pointer, Reference> {};
#  else
   struct iterator_base : std::iterator<Category, T, Distance>
   {
     typedef Reference reference;
     typedef Pointer pointer;
     typedef Distance difference_type;
   };
#  endif
  }

  template <class Category, class T, class Distance = std::ptrdiff_t,
            class Pointer = T*, class Reference = T&>
  struct iterator : efl_eina_boost::detail::iterator_base<Category, T, Distance, Pointer, Reference> {};
# endif
} // namespace efl_eina_boost

#endif // EFL_EINA_BOOST_ITERATOR_HPP
