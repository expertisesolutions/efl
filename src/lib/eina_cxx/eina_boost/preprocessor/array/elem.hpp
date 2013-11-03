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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_ARRAY_ELEM_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_ARRAY_ELEM_HPP
#
# include <eina_boost/preprocessor/array/data.hpp>
# include <eina_boost/preprocessor/array/size.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
#
# /* EFL_EINA_BOOST_PP_ARRAY_ELEM */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_ARRAY_ELEM(i, array) EFL_EINA_BOOST_PP_TUPLE_ELEM(EFL_EINA_BOOST_PP_ARRAY_SIZE(array), i, EFL_EINA_BOOST_PP_ARRAY_DATA(array))
# else
#    define EFL_EINA_BOOST_PP_ARRAY_ELEM(i, array) EFL_EINA_BOOST_PP_ARRAY_ELEM_I(i, array)
#    define EFL_EINA_BOOST_PP_ARRAY_ELEM_I(i, array) EFL_EINA_BOOST_PP_TUPLE_ELEM(EFL_EINA_BOOST_PP_ARRAY_SIZE(array), i, EFL_EINA_BOOST_PP_ARRAY_DATA(array))
# endif
#
# endif
