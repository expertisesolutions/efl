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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_LIST_REVERSE_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_LIST_REVERSE_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/list/fold_left.hpp>
#
# /* EFL_EINA_BOOST_PP_LIST_REVERSE */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_LIST_REVERSE(list) EFL_EINA_BOOST_PP_LIST_FOLD_LEFT(EFL_EINA_BOOST_PP_LIST_REVERSE_O, EFL_EINA_BOOST_PP_NIL, list)
# else
#    define EFL_EINA_BOOST_PP_LIST_REVERSE(list) EFL_EINA_BOOST_PP_LIST_REVERSE_I(list)
#    define EFL_EINA_BOOST_PP_LIST_REVERSE_I(list) EFL_EINA_BOOST_PP_LIST_FOLD_LEFT(EFL_EINA_BOOST_PP_LIST_REVERSE_O, EFL_EINA_BOOST_PP_NIL, list)
# endif
#
# define EFL_EINA_BOOST_PP_LIST_REVERSE_O(d, s, x) (x, s)
#
# /* EFL_EINA_BOOST_PP_LIST_REVERSE_D */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_LIST_REVERSE_D(d, list) EFL_EINA_BOOST_PP_LIST_FOLD_LEFT_ ## d(EFL_EINA_BOOST_PP_LIST_REVERSE_O, EFL_EINA_BOOST_PP_NIL, list)
# else
#    define EFL_EINA_BOOST_PP_LIST_REVERSE_D(d, list) EFL_EINA_BOOST_PP_LIST_REVERSE_D_I(d, list)
#    define EFL_EINA_BOOST_PP_LIST_REVERSE_D_I(d, list) EFL_EINA_BOOST_PP_LIST_FOLD_LEFT_ ## d(EFL_EINA_BOOST_PP_LIST_REVERSE_O, EFL_EINA_BOOST_PP_NIL, list)
# endif
#
# endif
