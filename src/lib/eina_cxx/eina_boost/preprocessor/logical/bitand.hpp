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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_LOGICAL_BITAND_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_LOGICAL_BITAND_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
#
# /* EFL_EINA_BOOST_PP_BITAND */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_BITAND(x, y) EFL_EINA_BOOST_PP_BITAND_I(x, y)
# else
#    define EFL_EINA_BOOST_PP_BITAND(x, y) EFL_EINA_BOOST_PP_BITAND_OO((x, y))
#    define EFL_EINA_BOOST_PP_BITAND_OO(par) EFL_EINA_BOOST_PP_BITAND_I ## par
# endif
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC()
#    define EFL_EINA_BOOST_PP_BITAND_I(x, y) EFL_EINA_BOOST_PP_BITAND_ ## x ## y
# else
#    define EFL_EINA_BOOST_PP_BITAND_I(x, y) EFL_EINA_BOOST_PP_BITAND_ID(EFL_EINA_BOOST_PP_BITAND_ ## x ## y)
#    define EFL_EINA_BOOST_PP_BITAND_ID(res) res
# endif
#
# define EFL_EINA_BOOST_PP_BITAND_00 0
# define EFL_EINA_BOOST_PP_BITAND_01 0
# define EFL_EINA_BOOST_PP_BITAND_10 0
# define EFL_EINA_BOOST_PP_BITAND_11 1
#
# endif
