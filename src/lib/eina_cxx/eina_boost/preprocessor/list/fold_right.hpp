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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_LIST_FOLD_RIGHT_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_LIST_FOLD_RIGHT_HPP
#
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/control/while.hpp>
# include <eina_boost/preprocessor/debug/error.hpp>
# include <eina_boost/preprocessor/detail/auto_rec.hpp>
#
# if 0
#    define EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT(op, state, list)
# endif
#
# define EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT_, EFL_EINA_BOOST_PP_AUTO_REC(EFL_EINA_BOOST_PP_WHILE_P, 256))
#
# define EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT_257(o, s, l) EFL_EINA_BOOST_PP_ERROR(0x0004)
#
# define EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT_D(d, o, s, l) EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT_ ## d(o, s, l)
# define EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT_2ND EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT
# define EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT_2ND_D EFL_EINA_BOOST_PP_LIST_FOLD_RIGHT_D
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    include <eina_boost/preprocessor/list/detail/edg/fold_right.hpp>
# else
#    include <eina_boost/preprocessor/list/detail/fold_right.hpp>
# endif
#
# endif
