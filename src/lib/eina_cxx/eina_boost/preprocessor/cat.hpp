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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_CAT_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_CAT_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
#
# /* EFL_EINA_BOOST_PP_CAT */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_CAT(a, b) EFL_EINA_BOOST_PP_CAT_I(a, b)
# else
#    define EFL_EINA_BOOST_PP_CAT(a, b) EFL_EINA_BOOST_PP_CAT_OO((a, b))
#    define EFL_EINA_BOOST_PP_CAT_OO(par) EFL_EINA_BOOST_PP_CAT_I ## par
# endif
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC()
#    define EFL_EINA_BOOST_PP_CAT_I(a, b) a ## b
# else
#    define EFL_EINA_BOOST_PP_CAT_I(a, b) EFL_EINA_BOOST_PP_CAT_II(~, a ## b)
#    define EFL_EINA_BOOST_PP_CAT_II(p, res) res
# endif
#
# endif
