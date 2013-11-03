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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_ITERATION_ITERATE_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_ITERATION_ITERATE_HPP
#
# include <eina_boost/preprocessor/arithmetic/dec.hpp>
# include <eina_boost/preprocessor/arithmetic/inc.hpp>
# include <eina_boost/preprocessor/array/elem.hpp>
# include <eina_boost/preprocessor/array/size.hpp>
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/slot/slot.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
#
# /* EFL_EINA_BOOST_PP_ITERATION_DEPTH */
#
# define EFL_EINA_BOOST_PP_ITERATION_DEPTH() 0
#
# /* EFL_EINA_BOOST_PP_ITERATION */
#
# define EFL_EINA_BOOST_PP_ITERATION() EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ITERATION_, EFL_EINA_BOOST_PP_ITERATION_DEPTH())
#
# /* EFL_EINA_BOOST_PP_ITERATION_START && EFL_EINA_BOOST_PP_ITERATION_FINISH */
#
# define EFL_EINA_BOOST_PP_ITERATION_START() EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ITERATION_START_, EFL_EINA_BOOST_PP_ITERATION_DEPTH())
# define EFL_EINA_BOOST_PP_ITERATION_FINISH() EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ITERATION_FINISH_, EFL_EINA_BOOST_PP_ITERATION_DEPTH())
#
# /* EFL_EINA_BOOST_PP_ITERATION_FLAGS */
#
# define EFL_EINA_BOOST_PP_ITERATION_FLAGS() (EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ITERATION_FLAGS_, EFL_EINA_BOOST_PP_ITERATION_DEPTH())())
#
# /* EFL_EINA_BOOST_PP_FRAME_ITERATION */
#
# define EFL_EINA_BOOST_PP_FRAME_ITERATION(i) EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ITERATION_, i)
#
# /* EFL_EINA_BOOST_PP_FRAME_START && EFL_EINA_BOOST_PP_FRAME_FINISH */
#
# define EFL_EINA_BOOST_PP_FRAME_START(i) EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ITERATION_START_, i)
# define EFL_EINA_BOOST_PP_FRAME_FINISH(i) EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ITERATION_FINISH_, i)
#
# /* EFL_EINA_BOOST_PP_FRAME_FLAGS */
#
# define EFL_EINA_BOOST_PP_FRAME_FLAGS(i) (EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ITERATION_FLAGS_, i)())
#
# /* EFL_EINA_BOOST_PP_RELATIVE_ITERATION */
#
# define EFL_EINA_BOOST_PP_RELATIVE_ITERATION(i) EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_RELATIVE_, i)(EFL_EINA_BOOST_PP_ITERATION_)
#
# define EFL_EINA_BOOST_PP_RELATIVE_0(m) EFL_EINA_BOOST_PP_CAT(m, EFL_EINA_BOOST_PP_ITERATION_DEPTH())
# define EFL_EINA_BOOST_PP_RELATIVE_1(m) EFL_EINA_BOOST_PP_CAT(m, EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_ITERATION_DEPTH()))
# define EFL_EINA_BOOST_PP_RELATIVE_2(m) EFL_EINA_BOOST_PP_CAT(m, EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_ITERATION_DEPTH())))
# define EFL_EINA_BOOST_PP_RELATIVE_3(m) EFL_EINA_BOOST_PP_CAT(m, EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_ITERATION_DEPTH()))))
# define EFL_EINA_BOOST_PP_RELATIVE_4(m) EFL_EINA_BOOST_PP_CAT(m, EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_ITERATION_DEPTH())))))
#
# /* EFL_EINA_BOOST_PP_RELATIVE_START && EFL_EINA_BOOST_PP_RELATIVE_FINISH */
#
# define EFL_EINA_BOOST_PP_RELATIVE_START(i) EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_RELATIVE_, i)(EFL_EINA_BOOST_PP_ITERATION_START_)
# define EFL_EINA_BOOST_PP_RELATIVE_FINISH(i) EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_RELATIVE_, i)(EFL_EINA_BOOST_PP_ITERATION_FINISH_)
#
# /* EFL_EINA_BOOST_PP_RELATIVE_FLAGS */
#
# define EFL_EINA_BOOST_PP_RELATIVE_FLAGS(i) (EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_RELATIVE_, i)(EFL_EINA_BOOST_PP_ITERATION_FLAGS_)())
#
# /* EFL_EINA_BOOST_PP_ITERATE */
#
# define EFL_EINA_BOOST_PP_ITERATE() EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ITERATE_, EFL_EINA_BOOST_PP_INC(EFL_EINA_BOOST_PP_ITERATION_DEPTH()))
#
# define EFL_EINA_BOOST_PP_ITERATE_1 <eina_boost/preprocessor/iteration/detail/iter/forward1.hpp>
# define EFL_EINA_BOOST_PP_ITERATE_2 <eina_boost/preprocessor/iteration/detail/iter/forward2.hpp>
# define EFL_EINA_BOOST_PP_ITERATE_3 <eina_boost/preprocessor/iteration/detail/iter/forward3.hpp>
# define EFL_EINA_BOOST_PP_ITERATE_4 <eina_boost/preprocessor/iteration/detail/iter/forward4.hpp>
# define EFL_EINA_BOOST_PP_ITERATE_5 <eina_boost/preprocessor/iteration/detail/iter/forward5.hpp>
#
# endif
