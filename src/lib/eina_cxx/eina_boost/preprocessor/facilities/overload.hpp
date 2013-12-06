# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef EFL_EINA_BOOST_PREPROCESSOR_FACILITIES_OVERLOAD_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_FACILITIES_OVERLOAD_HPP
#
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/variadic/size.hpp>
#
# /* EFL_EINA_BOOST_PP_OVERLOAD */
#
# if EFL_EINA_BOOST_PP_VARIADICS
#    define EFL_EINA_BOOST_PP_OVERLOAD(prefix, ...) EFL_EINA_BOOST_PP_CAT(prefix, EFL_EINA_BOOST_PP_VARIADIC_SIZE(__VA_ARGS__))
# endif
#
# endif
