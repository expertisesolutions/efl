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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_PARAMS_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_PARAMS_HPP
#
# include <eina_boost/preprocessor/arithmetic/dec.hpp>
# include <eina_boost/preprocessor/arithmetic/inc.hpp>
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/punctuation/comma_if.hpp>
# include <eina_boost/preprocessor/repetition/repeat.hpp>
#
# /* EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS(count, param) EFL_EINA_BOOST_PP_REPEAT(EFL_EINA_BOOST_PP_DEC(count), EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_M, param)
# else
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS(count, param) EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_I(count, param)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_I(count, param) EFL_EINA_BOOST_PP_REPEAT(EFL_EINA_BOOST_PP_DEC(count), EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_M, param)
# endif
#
# define EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_M(z, n, param) EFL_EINA_BOOST_PP_COMMA_IF(n) EFL_EINA_BOOST_PP_CAT(param, EFL_EINA_BOOST_PP_INC(n))
#
# /* EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_Z */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_Z(z, count, param) EFL_EINA_BOOST_PP_REPEAT_ ## z(EFL_EINA_BOOST_PP_DEC(count), EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_M, param)
# else
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_Z(z, count, param) EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_Z_I(z, count, param)
#    define EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_Z_I(z, count, param) EFL_EINA_BOOST_PP_REPEAT_ ## z(EFL_EINA_BOOST_PP_DEC(count), EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS_M, param)
# endif
#
# endif
