# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef EFL_EINA_BOOST_PREPROCESSOR_FACILITIES_EXPAND_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_FACILITIES_EXPAND_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC() && ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_DMC()
#    define EFL_EINA_BOOST_PP_EXPAND(x) EFL_EINA_BOOST_PP_EXPAND_I(x)
# else
#    define EFL_EINA_BOOST_PP_EXPAND(x) EFL_EINA_BOOST_PP_EXPAND_OO((x))
#    define EFL_EINA_BOOST_PP_EXPAND_OO(par) EFL_EINA_BOOST_PP_EXPAND_I ## par
# endif
#
# define EFL_EINA_BOOST_PP_EXPAND_I(x) x
#
# endif
