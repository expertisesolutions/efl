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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_SEQ_SEQ_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_SEQ_SEQ_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/seq/elem.hpp>
#
# /* EFL_EINA_BOOST_PP_SEQ_HEAD */
#
# define EFL_EINA_BOOST_PP_SEQ_HEAD(seq) EFL_EINA_BOOST_PP_SEQ_ELEM(0, seq)
#
# /* EFL_EINA_BOOST_PP_SEQ_TAIL */
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_SEQ_TAIL(seq) EFL_EINA_BOOST_PP_SEQ_TAIL_1((seq))
#    define EFL_EINA_BOOST_PP_SEQ_TAIL_1(par) EFL_EINA_BOOST_PP_SEQ_TAIL_2 ## par
#    define EFL_EINA_BOOST_PP_SEQ_TAIL_2(seq) EFL_EINA_BOOST_PP_SEQ_TAIL_I ## seq
# elif EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC()
#    define EFL_EINA_BOOST_PP_SEQ_TAIL(seq) EFL_EINA_BOOST_PP_SEQ_TAIL_ID(EFL_EINA_BOOST_PP_SEQ_TAIL_I seq)
#    define EFL_EINA_BOOST_PP_SEQ_TAIL_ID(id) id
# elif EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_SEQ_TAIL(seq) EFL_EINA_BOOST_PP_SEQ_TAIL_D(seq)
#    define EFL_EINA_BOOST_PP_SEQ_TAIL_D(seq) EFL_EINA_BOOST_PP_SEQ_TAIL_I seq
# else
#    define EFL_EINA_BOOST_PP_SEQ_TAIL(seq) EFL_EINA_BOOST_PP_SEQ_TAIL_I seq
# endif
#
# define EFL_EINA_BOOST_PP_SEQ_TAIL_I(x)
#
# /* EFL_EINA_BOOST_PP_SEQ_NIL */
#
# define EFL_EINA_BOOST_PP_SEQ_NIL(x) (x)
#
# endif
