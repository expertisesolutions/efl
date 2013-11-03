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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_SEQ_FIRST_N_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_SEQ_FIRST_N_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/control/if.hpp>
# include <eina_boost/preprocessor/seq/detail/split.hpp>
# include <eina_boost/preprocessor/tuple/eat.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
#
# /* EFL_EINA_BOOST_PP_SEQ_FIRST_N */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_SEQ_FIRST_N(n, seq) EFL_EINA_BOOST_PP_IF(n, EFL_EINA_BOOST_PP_TUPLE_ELEM, EFL_EINA_BOOST_PP_TUPLE_EAT_3)(2, 0, EFL_EINA_BOOST_PP_SEQ_SPLIT(n, seq (nil)))
# else
#    define EFL_EINA_BOOST_PP_SEQ_FIRST_N(n, seq) EFL_EINA_BOOST_PP_SEQ_FIRST_N_I(n, seq)
#    define EFL_EINA_BOOST_PP_SEQ_FIRST_N_I(n, seq) EFL_EINA_BOOST_PP_IF(n, EFL_EINA_BOOST_PP_TUPLE_ELEM, EFL_EINA_BOOST_PP_TUPLE_EAT_3)(2, 0, EFL_EINA_BOOST_PP_SEQ_SPLIT(n, seq (nil)))
# endif
#
# endif
