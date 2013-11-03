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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_CONTROL_EXPR_IF_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_CONTROL_EXPR_IF_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/control/expr_iif.hpp>
# include <eina_boost/preprocessor/logical/bool.hpp>
#
# /* EFL_EINA_BOOST_PP_EXPR_IF */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_EXPR_IF(cond, expr) EFL_EINA_BOOST_PP_EXPR_IIF(EFL_EINA_BOOST_PP_BOOL(cond), expr)
# else
#    define EFL_EINA_BOOST_PP_EXPR_IF(cond, expr) EFL_EINA_BOOST_PP_EXPR_IF_I(cond, expr)
#    define EFL_EINA_BOOST_PP_EXPR_IF_I(cond, expr) EFL_EINA_BOOST_PP_EXPR_IIF(EFL_EINA_BOOST_PP_BOOL(cond), expr)
# endif
#
# endif
