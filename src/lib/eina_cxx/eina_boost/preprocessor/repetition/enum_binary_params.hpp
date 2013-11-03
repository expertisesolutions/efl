# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef EFL_EINA_BOOST_PREPROCESSOR_REPETITION_ENUM_BINARY_PARAMS_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_REPETITION_ENUM_BINARY_PARAMS_HPP
#
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/punctuation/comma_if.hpp>
# include <eina_boost/preprocessor/repetition/repeat.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
# include <eina_boost/preprocessor/tuple/rem.hpp>
#
# /* EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS(count, p1, p2) EFL_EINA_BOOST_PP_REPEAT(count, EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M, (p1, p2))
# else
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS(count, p1, p2) EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_I(count, p1, p2)
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_I(count, p1, p2) EFL_EINA_BOOST_PP_REPEAT(count, EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M, (p1, p2))
# endif
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_STRICT()
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M(z, n, pp) EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M_IM(z, n, EFL_EINA_BOOST_PP_TUPLE_REM_2 pp)
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M_IM(z, n, im) EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M_I(z, n, im)
# else
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M(z, n, pp) EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M_I(z, n, EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, pp), EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 1, pp))
# endif
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC()
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M_I(z, n, p1, p2) EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M_II(z, n, p1, p2)
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M_II(z, n, p1, p2) EFL_EINA_BOOST_PP_COMMA_IF(n) p1 ## n p2 ## n
# else
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M_I(z, n, p1, p2) EFL_EINA_BOOST_PP_COMMA_IF(n) EFL_EINA_BOOST_PP_CAT(p1, n) EFL_EINA_BOOST_PP_CAT(p2, n)
# endif
#
# /* EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_Z */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_Z(z, count, p1, p2) EFL_EINA_BOOST_PP_REPEAT_ ## z(count, EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M, (p1, p2))
# else
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_Z(z, count, p1, p2) EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_Z_I(z, count, p1, p2)
#    define EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_Z_I(z, count, p1, p2) EFL_EINA_BOOST_PP_REPEAT_ ## z(count, EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS_M, (p1, p2))
# endif
#
# endif
