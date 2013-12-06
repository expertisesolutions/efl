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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_ARRAY_SIZE_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_ARRAY_SIZE_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
#
# /* EFL_EINA_BOOST_PP_ARRAY_SIZE */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_ARRAY_SIZE(array) EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 0, array)
# else
#    define EFL_EINA_BOOST_PP_ARRAY_SIZE(array) EFL_EINA_BOOST_PP_ARRAY_SIZE_I(array)
#    define EFL_EINA_BOOST_PP_ARRAY_SIZE_I(array) EFL_EINA_BOOST_PP_ARRAY_SIZE_II array
#    define EFL_EINA_BOOST_PP_ARRAY_SIZE_II(size, data) size
# endif
#
# endif
