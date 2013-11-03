//  Boost version.hpp configuration header file  ------------------------------//

//  (C) Copyright John maddock 1999. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for documentation

#ifndef EFL_EINA_BOOST_VERSION_HPP
#define EFL_EINA_BOOST_VERSION_HPP

//
//  Caution, this is the only boost header that is guarenteed
//  to change with every boost release, including this header
//  will cause a recompile every time a new boost version is
//  released.
//
//  EFL_EINA_BOOST_VERSION % 100 is the patch level
//  EFL_EINA_BOOST_VERSION / 100 % 1000 is the minor version
//  EFL_EINA_BOOST_VERSION / 100000 is the major version

#define EFL_EINA_BOOST_VERSION 105500

//
//  EFL_EINA_BOOST_LIB_VERSION must be defined to be the same as EFL_EINA_BOOST_VERSION
//  but as a *string* in the form "x_y[_z]" where x is the major version
//  number, y is the minor version number, and z is the patch level if not 0.
//  This is used by <config/auto_link.hpp> to select which library version to link to.

#define EFL_EINA_BOOST_LIB_VERSION "1_55"

#endif
