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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_DEBUG_ERROR_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_DEBUG_ERROR_HPP
#
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
#
# /* EFL_EINA_BOOST_PP_ERROR */
#
# if EFL_EINA_BOOST_PP_CONFIG_ERRORS
#    define EFL_EINA_BOOST_PP_ERROR(code) EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ERROR_, code)
# endif
#
# define EFL_EINA_BOOST_PP_ERROR_0x0000 EFL_EINA_BOOST_PP_ERROR(0x0000, EFL_EINA_BOOST_PP_INDEX_OUT_OF_BOUNDS)
# define EFL_EINA_BOOST_PP_ERROR_0x0001 EFL_EINA_BOOST_PP_ERROR(0x0001, EFL_EINA_BOOST_PP_WHILE_OVERFLOW)
# define EFL_EINA_BOOST_PP_ERROR_0x0002 EFL_EINA_BOOST_PP_ERROR(0x0002, EFL_EINA_BOOST_PP_FOR_OVERFLOW)
# define EFL_EINA_BOOST_PP_ERROR_0x0003 EFL_EINA_BOOST_PP_ERROR(0x0003, EFL_EINA_BOOST_PP_REPEAT_OVERFLOW)
# define EFL_EINA_BOOST_PP_ERROR_0x0004 EFL_EINA_BOOST_PP_ERROR(0x0004, EFL_EINA_BOOST_PP_LIST_FOLD_OVERFLOW)
# define EFL_EINA_BOOST_PP_ERROR_0x0005 EFL_EINA_BOOST_PP_ERROR(0x0005, EFL_EINA_BOOST_PP_SEQ_FOLD_OVERFLOW)
# define EFL_EINA_BOOST_PP_ERROR_0x0006 EFL_EINA_BOOST_PP_ERROR(0x0006, EFL_EINA_BOOST_PP_ARITHMETIC_OVERFLOW)
# define EFL_EINA_BOOST_PP_ERROR_0x0007 EFL_EINA_BOOST_PP_ERROR(0x0007, EFL_EINA_BOOST_PP_DIVISION_BY_ZERO)
#
# endif
