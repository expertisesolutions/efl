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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_LOGICAL_NOT_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_LOGICAL_NOT_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/logical/bool.hpp>
# include <eina_boost/preprocessor/logical/compl.hpp>
#
# /* EFL_EINA_BOOST_PP_NOT */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_NOT(x) EFL_EINA_BOOST_PP_COMPL(EFL_EINA_BOOST_PP_BOOL(x))
# else
#    define EFL_EINA_BOOST_PP_NOT(x) EFL_EINA_BOOST_PP_NOT_I(x)
#    define EFL_EINA_BOOST_PP_NOT_I(x) EFL_EINA_BOOST_PP_COMPL(EFL_EINA_BOOST_PP_BOOL(x))
# endif
#
# endif
