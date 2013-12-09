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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_STRINGIZE_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_STRINGIZE_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
#
# /* EFL_EINA_BOOST_PP_STRINGIZE */
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC()
#    define EFL_EINA_BOOST_PP_STRINGIZE(text) EFL_EINA_BOOST_PP_STRINGIZE_A((text))
#    define EFL_EINA_BOOST_PP_STRINGIZE_A(arg) EFL_EINA_BOOST_PP_STRINGIZE_I arg
# elif EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_STRINGIZE(text) EFL_EINA_BOOST_PP_STRINGIZE_OO((text))
#    define EFL_EINA_BOOST_PP_STRINGIZE_OO(par) EFL_EINA_BOOST_PP_STRINGIZE_I ## par
# else
#    define EFL_EINA_BOOST_PP_STRINGIZE(text) EFL_EINA_BOOST_PP_STRINGIZE_I(text)
# endif
#
# define EFL_EINA_BOOST_PP_STRINGIZE_I(text) #text
#
# endif
