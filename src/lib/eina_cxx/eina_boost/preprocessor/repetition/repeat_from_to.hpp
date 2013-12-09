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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_REPETITION_REPEAT_FROM_TO_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_REPETITION_REPEAT_FROM_TO_HPP
#
# include <eina_boost/preprocessor/arithmetic/add.hpp>
# include <eina_boost/preprocessor/arithmetic/sub.hpp>
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/control/while.hpp>
# include <eina_boost/preprocessor/debug/error.hpp>
# include <eina_boost/preprocessor/detail/auto_rec.hpp>
# include <eina_boost/preprocessor/repetition/repeat.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
# include <eina_boost/preprocessor/tuple/rem.hpp>
#
# /* EFL_EINA_BOOST_PP_REPEAT_FROM_TO */
#
# if 0
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO(first, last, macro, data)
# endif
#
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_REPEAT_FROM_TO_, EFL_EINA_BOOST_PP_AUTO_REC(EFL_EINA_BOOST_PP_REPEAT_P, 4))
#
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_1(f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_1(EFL_EINA_BOOST_PP_AUTO_REC(EFL_EINA_BOOST_PP_WHILE_P, 256), f, l, m, dt)
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_2(f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_2(EFL_EINA_BOOST_PP_AUTO_REC(EFL_EINA_BOOST_PP_WHILE_P, 256), f, l, m, dt)
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_3(f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_3(EFL_EINA_BOOST_PP_AUTO_REC(EFL_EINA_BOOST_PP_WHILE_P, 256), f, l, m, dt)
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_4(f, l, m, dt) EFL_EINA_BOOST_PP_ERROR(0x0003)
#
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_1ST EFL_EINA_BOOST_PP_REPEAT_FROM_TO_1
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_2ND EFL_EINA_BOOST_PP_REPEAT_FROM_TO_2
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_3RD EFL_EINA_BOOST_PP_REPEAT_FROM_TO_3
#
# /* EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D */
#
# if 0
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D(d, first, last, macro, data)
# endif
#
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_, EFL_EINA_BOOST_PP_AUTO_REC(EFL_EINA_BOOST_PP_REPEAT_P, 4))
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_1(d, f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_1(EFL_EINA_BOOST_PP_SUB_D(d, l, f), EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1, (d, f, m, dt))
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_2(d, f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_2(EFL_EINA_BOOST_PP_SUB_D(d, l, f), EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2, (d, f, m, dt))
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_3(d, f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_3(EFL_EINA_BOOST_PP_SUB_D(d, l, f), EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3, (d, f, m, dt))
# else
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_1(d, f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_1_I(d, f, l, m, dt)
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_2(d, f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_2_I(d, f, l, m, dt)
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_3(d, f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_3_I(d, f, l, m, dt)
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_1_I(d, f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_1(EFL_EINA_BOOST_PP_SUB_D(d, l, f), EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1, (d, f, m, dt))
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_2_I(d, f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_2(EFL_EINA_BOOST_PP_SUB_D(d, l, f), EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2, (d, f, m, dt))
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_D_3_I(d, f, l, m, dt) EFL_EINA_BOOST_PP_REPEAT_3(EFL_EINA_BOOST_PP_SUB_D(d, l, f), EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3, (d, f, m, dt))
# endif
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_STRICT()
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1(z, n, dfmd) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1_IM(z, n, EFL_EINA_BOOST_PP_TUPLE_REM_4 dfmd)
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2(z, n, dfmd) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2_IM(z, n, EFL_EINA_BOOST_PP_TUPLE_REM_4 dfmd)
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3(z, n, dfmd) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3_IM(z, n, EFL_EINA_BOOST_PP_TUPLE_REM_4 dfmd)
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1_IM(z, n, im) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1_I(z, n, im)
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2_IM(z, n, im) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2_I(z, n, im)
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3_IM(z, n, im) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3_I(z, n, im)
# else
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1(z, n, dfmd) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1_I(z, n, EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 0, dfmd), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 1, dfmd), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 2, dfmd), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 3, dfmd))
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2(z, n, dfmd) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2_I(z, n, EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 0, dfmd), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 1, dfmd), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 2, dfmd), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 3, dfmd))
#    define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3(z, n, dfmd) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3_I(z, n, EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 0, dfmd), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 1, dfmd), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 2, dfmd), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 3, dfmd))
# endif
#
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1_I(z, n, d, f, m, dt) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1_II(z, EFL_EINA_BOOST_PP_ADD_D(d, n, f), m, dt)
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2_I(z, n, d, f, m, dt) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2_II(z, EFL_EINA_BOOST_PP_ADD_D(d, n, f), m, dt)
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3_I(z, n, d, f, m, dt) EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3_II(z, EFL_EINA_BOOST_PP_ADD_D(d, n, f), m, dt)
#
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_1_II(z, n, m, dt) m(z, n, dt)
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_2_II(z, n, m, dt) m(z, n, dt)
# define EFL_EINA_BOOST_PP_REPEAT_FROM_TO_M_3_II(z, n, m, dt) m(z, n, dt)
#
# endif
