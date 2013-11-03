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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_COMPARISON_LESS_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_COMPARISON_LESS_HPP
#
# include <eina_boost/preprocessor/comparison/less_equal.hpp>
# include <eina_boost/preprocessor/comparison/not_equal.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/control/iif.hpp>
# include <eina_boost/preprocessor/logical/bitand.hpp>
# include <eina_boost/preprocessor/tuple/eat.hpp>
#
# /* EFL_EINA_BOOST_PP_LESS */
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & (EFL_EINA_BOOST_PP_CONFIG_MWCC() | EFL_EINA_BOOST_PP_CONFIG_DMC())
#    define EFL_EINA_BOOST_PP_LESS(x, y) EFL_EINA_BOOST_PP_BITAND(EFL_EINA_BOOST_PP_NOT_EQUAL(x, y), EFL_EINA_BOOST_PP_LESS_EQUAL(x, y))
# elif ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_LESS(x, y) EFL_EINA_BOOST_PP_IIF(EFL_EINA_BOOST_PP_NOT_EQUAL(x, y), EFL_EINA_BOOST_PP_LESS_EQUAL, 0 EFL_EINA_BOOST_PP_TUPLE_EAT_2)(x, y)
# else
#    define EFL_EINA_BOOST_PP_LESS(x, y) EFL_EINA_BOOST_PP_LESS_I(x, y)
#    define EFL_EINA_BOOST_PP_LESS_I(x, y) EFL_EINA_BOOST_PP_IIF(EFL_EINA_BOOST_PP_NOT_EQUAL(x, y), EFL_EINA_BOOST_PP_LESS_EQUAL, 0 EFL_EINA_BOOST_PP_TUPLE_EAT_2)(x, y)
# endif
#
# /* EFL_EINA_BOOST_PP_LESS_D */
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & (EFL_EINA_BOOST_PP_CONFIG_MWCC() | EFL_EINA_BOOST_PP_CONFIG_DMC())
#    define EFL_EINA_BOOST_PP_LESS_D(d, x, y) EFL_EINA_BOOST_PP_BITAND(EFL_EINA_BOOST_PP_NOT_EQUAL(x, y), EFL_EINA_BOOST_PP_LESS_EQUAL_D(d, x, y))
# elif ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_LESS_D(d, x, y) EFL_EINA_BOOST_PP_IIF(EFL_EINA_BOOST_PP_NOT_EQUAL(x, y), EFL_EINA_BOOST_PP_LESS_EQUAL_D, 0 EFL_EINA_BOOST_PP_TUPLE_EAT_3)(d, x, y)
# else
#    define EFL_EINA_BOOST_PP_LESS_D(d, x, y) EFL_EINA_BOOST_PP_LESS_D_I(d, x, y)
#    define EFL_EINA_BOOST_PP_LESS_D_I(d, x, y) EFL_EINA_BOOST_PP_IIF(EFL_EINA_BOOST_PP_NOT_EQUAL(x, y), EFL_EINA_BOOST_PP_LESS_EQUAL_D, 0 EFL_EINA_BOOST_PP_TUPLE_EAT_3)(d, x, y)
# endif
#
# endif
