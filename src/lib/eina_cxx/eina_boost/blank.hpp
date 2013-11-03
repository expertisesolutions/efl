//-----------------------------------------------------------------------------
// boost blank.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef EFL_EINA_BOOST_BLANK_HPP
#define EFL_EINA_BOOST_BLANK_HPP

#include "eina_boost/blank_fwd.hpp"

#if !defined(EFL_EINA_BOOST_NO_IOSTREAM)
#include <iosfwd> // for std::basic_ostream forward declare
#include "eina_boost/detail/templated_streams.hpp"
#endif // EFL_EINA_BOOST_NO_IOSTREAM

#include "eina_boost/mpl/bool.hpp"
#include "eina_boost/type_traits/is_empty.hpp"
#include "eina_boost/type_traits/is_pod.hpp"
#include "eina_boost/type_traits/is_stateless.hpp"

namespace efl_eina_boost {

struct blank
{
};

// type traits specializations
//

template <>
struct is_pod< blank >
    : mpl::true_
{
};

template <>
struct is_empty< blank >
    : mpl::true_
{
};

template <>
struct is_stateless< blank >
    : mpl::true_
{
};

// relational operators
//

inline bool operator==(const blank&, const blank&)
{
    return true;
}

inline bool operator<=(const blank&, const blank&)
{
    return true;
}

inline bool operator>=(const blank&, const blank&)
{
    return true;
}

inline bool operator!=(const blank&, const blank&)
{
    return false;
}

inline bool operator<(const blank&, const blank&)
{
    return false;
}

inline bool operator>(const blank&, const blank&)
{
    return false;
}

// streaming support
//
#if !defined(EFL_EINA_BOOST_NO_IOSTREAM)

EFL_EINA_BOOST_TEMPLATED_STREAM_TEMPLATE(E,T)
inline EFL_EINA_BOOST_TEMPLATED_STREAM(ostream, E,T)& operator<<(
      EFL_EINA_BOOST_TEMPLATED_STREAM(ostream, E,T)& out
    , const blank&
    )
{
    // (output nothing)
    return out;
}

#endif // EFL_EINA_BOOST_NO_IOSTREAM

} // namespace efl_eina_boost

#endif // EFL_EINA_BOOST_BLANK_HPP
