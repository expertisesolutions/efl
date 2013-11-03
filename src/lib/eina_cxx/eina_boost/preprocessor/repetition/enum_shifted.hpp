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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_HPP
#
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/arithmetic/dec.hpp>
# include <eina_boost/preprocessor/arithmetic/inc.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/debug/error.hpp>
# include <eina_boost/preprocessor/detail/auto_rec.hpp>
# include <eina_boost/preprocessor/punctuation/comma_if.hpp>
# include <eina_boost/preprocessor/repetition/repeat.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
# include <eina_boost/preprocessor/tuple/rem.hpp>
#
# /* EFL_EINA_BOOST_PP_ENUM_SHIFTED */
#
# if 0
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED(count, macro, data)
# endif
#
# define EFL_EINA_BOOST_PP_ENUM_SHIFTED EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ENUM_SHIFTED_, EFL_EINA_BOOST_PP_AUTO_REC(EFL_EINA_BOOST_PP_REPEAT_P, 4))
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_1(c, m, d) EFL_EINA_BOOST_PP_REPEAT_1(EFL_EINA_BOOST_PP_DEC(c), EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_1, (m, d))
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_2(c, m, d) EFL_EINA_BOOST_PP_REPEAT_2(EFL_EINA_BOOST_PP_DEC(c), EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_2, (m, d))
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_3(c, m, d) EFL_EINA_BOOST_PP_REPEAT_3(EFL_EINA_BOOST_PP_DEC(c), EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_3, (m, d))
# else
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_1(c, m, d) EFL_EINA_BOOST_PP_ENUM_SHIFTED_1_I(c, m, d)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_2(c, m, d) EFL_EINA_BOOST_PP_ENUM_SHIFTED_1_2(c, m, d)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_3(c, m, d) EFL_EINA_BOOST_PP_ENUM_SHIFTED_1_3(c, m, d)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_1_I(c, m, d) EFL_EINA_BOOST_PP_REPEAT_1(EFL_EINA_BOOST_PP_DEC(c), EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_1, (m, d))
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_2_I(c, m, d) EFL_EINA_BOOST_PP_REPEAT_2(EFL_EINA_BOOST_PP_DEC(c), EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_2, (m, d))
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_3_I(c, m, d) EFL_EINA_BOOST_PP_REPEAT_3(EFL_EINA_BOOST_PP_DEC(c), EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_3, (m, d))
# endif
#
# define EFL_EINA_BOOST_PP_ENUM_SHIFTED_4(c, m, d) EFL_EINA_BOOST_PP_ERROR(0x0003)
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_STRICT()
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_1(z, n, md) EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_1_IM(z, n, EFL_EINA_BOOST_PP_TUPLE_REM_2 md)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_2(z, n, md) EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_2_IM(z, n, EFL_EINA_BOOST_PP_TUPLE_REM_2 md)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_3(z, n, md) EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_3_IM(z, n, EFL_EINA_BOOST_PP_TUPLE_REM_2 md)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_1_IM(z, n, im) EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_1_I(z, n, im)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_2_IM(z, n, im) EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_2_I(z, n, im)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_3_IM(z, n, im) EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_3_I(z, n, im)
# else
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_1(z, n, md) EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_1_I(z, n, EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, md), EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 1, md))
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_2(z, n, md) EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_2_I(z, n, EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, md), EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 1, md))
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_3(z, n, md) EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_3_I(z, n, EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, md), EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 1, md))
# endif
#
# define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_1_I(z, n, m, d) EFL_EINA_BOOST_PP_COMMA_IF(n) m(z, EFL_EINA_BOOST_PP_INC(n), d)
# define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_2_I(z, n, m, d) EFL_EINA_BOOST_PP_COMMA_IF(n) m(z, EFL_EINA_BOOST_PP_INC(n), d)
# define EFL_EINA_BOOST_PP_ENUM_SHIFTED_M_3_I(z, n, m, d) EFL_EINA_BOOST_PP_COMMA_IF(n) m(z, EFL_EINA_BOOST_PP_INC(n), d)
#
# endif
