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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_ARRAY_DATA_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_ARRAY_DATA_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
#
# /* EFL_EINA_BOOST_PP_ARRAY_DATA */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_ARRAY_DATA(array) EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 1, array)
# else
#    define EFL_EINA_BOOST_PP_ARRAY_DATA(array) EFL_EINA_BOOST_PP_ARRAY_DATA_I(array)
#    define EFL_EINA_BOOST_PP_ARRAY_DATA_I(array) EFL_EINA_BOOST_PP_ARRAY_DATA_II array
#    define EFL_EINA_BOOST_PP_ARRAY_DATA_II(size, data) data
# endif
#
# endif
