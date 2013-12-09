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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_ARITHMETIC_SUB_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_ARITHMETIC_SUB_HPP
#
# include <eina_boost/preprocessor/arithmetic/dec.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/control/while.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
#
# /* EFL_EINA_BOOST_PP_SUB */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_SUB(x, y) EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, EFL_EINA_BOOST_PP_WHILE(EFL_EINA_BOOST_PP_SUB_P, EFL_EINA_BOOST_PP_SUB_O, (x, y)))
# else
#    define EFL_EINA_BOOST_PP_SUB(x, y) EFL_EINA_BOOST_PP_SUB_I(x, y)
#    define EFL_EINA_BOOST_PP_SUB_I(x, y) EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, EFL_EINA_BOOST_PP_WHILE(EFL_EINA_BOOST_PP_SUB_P, EFL_EINA_BOOST_PP_SUB_O, (x, y)))
# endif
#
# define EFL_EINA_BOOST_PP_SUB_P(d, xy) EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 1, xy)
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_SUB_O(d, xy) EFL_EINA_BOOST_PP_SUB_O_I xy
# else
#    define EFL_EINA_BOOST_PP_SUB_O(d, xy) EFL_EINA_BOOST_PP_SUB_O_I(EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, xy), EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 1, xy))
# endif
#
# define EFL_EINA_BOOST_PP_SUB_O_I(x, y) (EFL_EINA_BOOST_PP_DEC(x), EFL_EINA_BOOST_PP_DEC(y))
#
# /* EFL_EINA_BOOST_PP_SUB_D */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_SUB_D(d, x, y) EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, EFL_EINA_BOOST_PP_WHILE_ ## d(EFL_EINA_BOOST_PP_SUB_P, EFL_EINA_BOOST_PP_SUB_O, (x, y)))
# else
#    define EFL_EINA_BOOST_PP_SUB_D(d, x, y) EFL_EINA_BOOST_PP_SUB_D_I(d, x, y)
#    define EFL_EINA_BOOST_PP_SUB_D_I(d, x, y) EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, EFL_EINA_BOOST_PP_WHILE_ ## d(EFL_EINA_BOOST_PP_SUB_P, EFL_EINA_BOOST_PP_SUB_O, (x, y)))
# endif
#
# endif
