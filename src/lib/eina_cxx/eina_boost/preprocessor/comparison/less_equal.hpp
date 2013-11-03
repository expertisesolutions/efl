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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_COMPARISON_LESS_EQUAL_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_COMPARISON_LESS_EQUAL_HPP
#
# include <eina_boost/preprocessor/arithmetic/sub.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/logical/not.hpp>
#
# /* EFL_EINA_BOOST_PP_LESS_EQUAL */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_LESS_EQUAL(x, y) EFL_EINA_BOOST_PP_NOT(EFL_EINA_BOOST_PP_SUB(x, y))
# else
#    define EFL_EINA_BOOST_PP_LESS_EQUAL(x, y) EFL_EINA_BOOST_PP_LESS_EQUAL_I(x, y)
#    define EFL_EINA_BOOST_PP_LESS_EQUAL_I(x, y) EFL_EINA_BOOST_PP_NOT(EFL_EINA_BOOST_PP_SUB(x, y))
# endif
#
# /* EFL_EINA_BOOST_PP_LESS_EQUAL_D */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_LESS_EQUAL_D(d, x, y) EFL_EINA_BOOST_PP_NOT(EFL_EINA_BOOST_PP_SUB_D(d, x, y))
# else
#    define EFL_EINA_BOOST_PP_LESS_EQUAL_D(d, x, y) EFL_EINA_BOOST_PP_LESS_EQUAL_D_I(d, x, y)
#    define EFL_EINA_BOOST_PP_LESS_EQUAL_D_I(d, x, y) EFL_EINA_BOOST_PP_NOT(EFL_EINA_BOOST_PP_SUB_D(d, x, y))
# endif
#
# endif
