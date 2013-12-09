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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_SEQ_SUBSEQ_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_SEQ_SUBSEQ_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/seq/first_n.hpp>
# include <eina_boost/preprocessor/seq/rest_n.hpp>
#
# /* EFL_EINA_BOOST_PP_SEQ_SUBSEQ */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_SEQ_SUBSEQ(seq, i, len) EFL_EINA_BOOST_PP_SEQ_FIRST_N(len, EFL_EINA_BOOST_PP_SEQ_REST_N(i, seq))
# else
#    define EFL_EINA_BOOST_PP_SEQ_SUBSEQ(seq, i, len) EFL_EINA_BOOST_PP_SEQ_SUBSEQ_I(seq, i, len)
#    define EFL_EINA_BOOST_PP_SEQ_SUBSEQ_I(seq, i, len) EFL_EINA_BOOST_PP_SEQ_FIRST_N(len, EFL_EINA_BOOST_PP_SEQ_REST_N(i, seq))
# endif
#
# endif
