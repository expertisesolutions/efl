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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_DETAIL_IS_BINARY_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_DETAIL_IS_BINARY_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/detail/check.hpp>
#
# /* EFL_EINA_BOOST_PP_IS_BINARY */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_IS_BINARY(x) EFL_EINA_BOOST_PP_CHECK(x, EFL_EINA_BOOST_PP_IS_BINARY_CHECK)
# else
#    define EFL_EINA_BOOST_PP_IS_BINARY(x) EFL_EINA_BOOST_PP_IS_BINARY_I(x)
#    define EFL_EINA_BOOST_PP_IS_BINARY_I(x) EFL_EINA_BOOST_PP_CHECK(x, EFL_EINA_BOOST_PP_IS_BINARY_CHECK)
# endif
#
# define EFL_EINA_BOOST_PP_IS_BINARY_CHECK(a, b) 1
# define EFL_EINA_BOOST_PP_CHECK_RESULT_EFL_EINA_BOOST_PP_IS_BINARY_CHECK 0, EFL_EINA_BOOST_PP_NIL
#
# endif
