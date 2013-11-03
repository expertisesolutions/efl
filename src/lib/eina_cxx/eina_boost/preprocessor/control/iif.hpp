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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_CONTROL_IIF_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_CONTROL_IIF_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_IIF(bit, t, f) EFL_EINA_BOOST_PP_IIF_I(bit, t, f)
# else
#    define EFL_EINA_BOOST_PP_IIF(bit, t, f) EFL_EINA_BOOST_PP_IIF_OO((bit, t, f))
#    define EFL_EINA_BOOST_PP_IIF_OO(par) EFL_EINA_BOOST_PP_IIF_I ## par
# endif
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC()
#    define EFL_EINA_BOOST_PP_IIF_I(bit, t, f) EFL_EINA_BOOST_PP_IIF_ ## bit(t, f)
# else
#    define EFL_EINA_BOOST_PP_IIF_I(bit, t, f) EFL_EINA_BOOST_PP_IIF_II(EFL_EINA_BOOST_PP_IIF_ ## bit(t, f))
#    define EFL_EINA_BOOST_PP_IIF_II(id) id
# endif
#
# define EFL_EINA_BOOST_PP_IIF_0(t, f) f
# define EFL_EINA_BOOST_PP_IIF_1(t, f) t
#
# endif
