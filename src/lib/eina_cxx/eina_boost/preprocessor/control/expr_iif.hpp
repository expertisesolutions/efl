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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_CONTROL_EXPR_IIF_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_CONTROL_EXPR_IIF_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
#
# /* EFL_EINA_BOOST_PP_EXPR_IIF */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_EXPR_IIF(bit, expr) EFL_EINA_BOOST_PP_EXPR_IIF_I(bit, expr)
# else
#    define EFL_EINA_BOOST_PP_EXPR_IIF(bit, expr) EFL_EINA_BOOST_PP_EXPR_IIF_OO((bit, expr))
#    define EFL_EINA_BOOST_PP_EXPR_IIF_OO(par) EFL_EINA_BOOST_PP_EXPR_IIF_I ## par
# endif
#
# define EFL_EINA_BOOST_PP_EXPR_IIF_I(bit, expr) EFL_EINA_BOOST_PP_EXPR_IIF_ ## bit(expr)
#
# define EFL_EINA_BOOST_PP_EXPR_IIF_0(expr)
# define EFL_EINA_BOOST_PP_EXPR_IIF_1(expr) expr
#
# endif
