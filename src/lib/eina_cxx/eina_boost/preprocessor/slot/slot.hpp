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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_SLOT_SLOT_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_SLOT_SLOT_HPP
#
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/slot/detail/def.hpp>
#
# /* EFL_EINA_BOOST_PP_ASSIGN_SLOT */
#
# define EFL_EINA_BOOST_PP_ASSIGN_SLOT(i) EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_ASSIGN_SLOT_, i)
#
# define EFL_EINA_BOOST_PP_ASSIGN_SLOT_1 <eina_boost/preprocessor/slot/detail/slot1.hpp>
# define EFL_EINA_BOOST_PP_ASSIGN_SLOT_2 <eina_boost/preprocessor/slot/detail/slot2.hpp>
# define EFL_EINA_BOOST_PP_ASSIGN_SLOT_3 <eina_boost/preprocessor/slot/detail/slot3.hpp>
# define EFL_EINA_BOOST_PP_ASSIGN_SLOT_4 <eina_boost/preprocessor/slot/detail/slot4.hpp>
# define EFL_EINA_BOOST_PP_ASSIGN_SLOT_5 <eina_boost/preprocessor/slot/detail/slot5.hpp>
#
# /* EFL_EINA_BOOST_PP_SLOT */
#
# define EFL_EINA_BOOST_PP_SLOT(i) EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_SLOT_, i)()
#
# endif
