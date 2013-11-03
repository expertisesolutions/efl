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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_LOGICAL_COMPL_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_LOGICAL_COMPL_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
#
# /* EFL_EINA_BOOST_PP_COMPL */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_COMPL(x) EFL_EINA_BOOST_PP_COMPL_I(x)
# else
#    define EFL_EINA_BOOST_PP_COMPL(x) EFL_EINA_BOOST_PP_COMPL_OO((x))
#    define EFL_EINA_BOOST_PP_COMPL_OO(par) EFL_EINA_BOOST_PP_COMPL_I ## par
# endif
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC()
#    define EFL_EINA_BOOST_PP_COMPL_I(x) EFL_EINA_BOOST_PP_COMPL_ ## x
# else
#    define EFL_EINA_BOOST_PP_COMPL_I(x) EFL_EINA_BOOST_PP_COMPL_ID(EFL_EINA_BOOST_PP_COMPL_ ## x)
#    define EFL_EINA_BOOST_PP_COMPL_ID(id) id
# endif
#
# define EFL_EINA_BOOST_PP_COMPL_0 1
# define EFL_EINA_BOOST_PP_COMPL_1 0
#
# endif
