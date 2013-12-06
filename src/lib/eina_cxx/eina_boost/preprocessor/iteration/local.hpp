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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_ITERATION_LOCAL_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_ITERATION_LOCAL_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/slot/slot.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
#
# /* EFL_EINA_BOOST_PP_LOCAL_ITERATE */
#
# define EFL_EINA_BOOST_PP_LOCAL_ITERATE() <eina_boost/preprocessor/iteration/detail/local.hpp>
#
# define EFL_EINA_BOOST_PP_LOCAL_C(n) (EFL_EINA_BOOST_PP_LOCAL_S) <= n && (EFL_EINA_BOOST_PP_LOCAL_F) >= n
# define EFL_EINA_BOOST_PP_LOCAL_R(n) (EFL_EINA_BOOST_PP_LOCAL_F) <= n && (EFL_EINA_BOOST_PP_LOCAL_S) >= n
#
# endif
