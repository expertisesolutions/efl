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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_SEQ_REST_N_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_SEQ_REST_N_HPP
#
# include <eina_boost/preprocessor/arithmetic/inc.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/facilities/empty.hpp>
# include <eina_boost/preprocessor/seq/detail/split.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
#
# /* EFL_EINA_BOOST_PP_SEQ_REST_N */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_SEQ_REST_N(n, seq) EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 1, EFL_EINA_BOOST_PP_SEQ_SPLIT(EFL_EINA_BOOST_PP_INC(n), (nil) seq EFL_EINA_BOOST_PP_EMPTY))()
# else
#    define EFL_EINA_BOOST_PP_SEQ_REST_N(n, seq) EFL_EINA_BOOST_PP_SEQ_REST_N_I(n, seq)
#    define EFL_EINA_BOOST_PP_SEQ_REST_N_I(n, seq) EFL_EINA_BOOST_PP_TUPLE_ELEM(2, 1, EFL_EINA_BOOST_PP_SEQ_SPLIT(EFL_EINA_BOOST_PP_INC(n), (nil) seq EFL_EINA_BOOST_PP_EMPTY))()
# endif
#
# endif
