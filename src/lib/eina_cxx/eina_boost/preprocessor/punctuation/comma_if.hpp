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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_PUNCTUATION_COMMA_IF_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_PUNCTUATION_COMMA_IF_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/control/if.hpp>
# include <eina_boost/preprocessor/facilities/empty.hpp>
# include <eina_boost/preprocessor/punctuation/comma.hpp>
#
# /* EFL_EINA_BOOST_PP_COMMA_IF */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_COMMA_IF(cond) EFL_EINA_BOOST_PP_IF(cond, EFL_EINA_BOOST_PP_COMMA, EFL_EINA_BOOST_PP_EMPTY)()
# else
#    define EFL_EINA_BOOST_PP_COMMA_IF(cond) EFL_EINA_BOOST_PP_COMMA_IF_I(cond)
#    define EFL_EINA_BOOST_PP_COMMA_IF_I(cond) EFL_EINA_BOOST_PP_IF(cond, EFL_EINA_BOOST_PP_COMMA, EFL_EINA_BOOST_PP_EMPTY)()
# endif
#
# endif
