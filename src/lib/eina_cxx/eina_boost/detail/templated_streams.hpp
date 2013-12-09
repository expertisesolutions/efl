//-----------------------------------------------------------------------------
// boost detail/templated_streams.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef EFL_EINA_BOOST_DETAIL_TEMPLATED_STREAMS_HPP
#define EFL_EINA_BOOST_DETAIL_TEMPLATED_STREAMS_HPP

#include "eina_boost/config.hpp"

///////////////////////////////////////////////////////////////////////////////
// (detail) EFL_EINA_BOOST_TEMPLATED_STREAM_* macros
//
// Provides workaround platforms without stream class templates.
//

#if !defined(EFL_EINA_BOOST_NO_STD_LOCALE)

#define EFL_EINA_BOOST_TEMPLATED_STREAM_TEMPLATE(E,T) \
    template < typename E , typename T >

#define EFL_EINA_BOOST_TEMPLATED_STREAM_TEMPLATE_ALLOC(E,T,A) \
    template < typename E , typename T , typename A >

#define EFL_EINA_BOOST_TEMPLATED_STREAM_ARGS(E,T) \
    typename E , typename T 

#define EFL_EINA_BOOST_TEMPLATED_STREAM_ARGS_ALLOC(E,T,A) \
    typename E , typename T , typename A 

#define EFL_EINA_BOOST_TEMPLATED_STREAM_COMMA        ,

#define EFL_EINA_BOOST_TEMPLATED_STREAM_ELEM(E)      E
#define EFL_EINA_BOOST_TEMPLATED_STREAM_TRAITS(T)    T
#define EFL_EINA_BOOST_TEMPLATED_STREAM_ALLOC(A)     A

#define EFL_EINA_BOOST_TEMPLATED_STREAM(X,E,T) \
    EFL_EINA_BOOST_JOIN(std::basic_,X)< E , T >

#define EFL_EINA_BOOST_TEMPLATED_STREAM_WITH_ALLOC(X,E,T,A) \
    EFL_EINA_BOOST_JOIN(std::basic_,X)< E , T , A >

#else // defined(EFL_EINA_BOOST_NO_STD_LOCALE)

#define EFL_EINA_BOOST_TEMPLATED_STREAM_TEMPLATE(E,T) /**/

#define EFL_EINA_BOOST_TEMPLATED_STREAM_TEMPLATE_ALLOC(E,T,A) /**/

#define EFL_EINA_BOOST_TEMPLATED_STREAM_ARGS(E,T) /**/

#define EFL_EINA_BOOST_TEMPLATED_STREAM_ARGS_ALLOC(E,T,A) /**/

#define EFL_EINA_BOOST_TEMPLATED_STREAM_COMMA        /**/

#define EFL_EINA_BOOST_TEMPLATED_STREAM_ELEM(E)      char
#define EFL_EINA_BOOST_TEMPLATED_STREAM_TRAITS(T)    std::char_traits<char>
#define EFL_EINA_BOOST_TEMPLATED_STREAM_ALLOC(A)     std::allocator<char>

#define EFL_EINA_BOOST_TEMPLATED_STREAM(X,E,T) \
    std::X

#define EFL_EINA_BOOST_TEMPLATED_STREAM_WITH_ALLOC(X,E,T,A) \
    std::X

#endif // EFL_EINA_BOOST_NO_STD_LOCALE

#endif // EFL_EINA_BOOST_DETAIL_TEMPLATED_STREAMS_HPP
