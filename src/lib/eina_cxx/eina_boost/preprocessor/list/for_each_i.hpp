# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef EFL_EINA_BOOST_PREPROCESSOR_LIST_LIST_FOR_EACH_I_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_LIST_LIST_FOR_EACH_I_HPP
#
# include <eina_boost/preprocessor/arithmetic/inc.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/list/adt.hpp>
# include <eina_boost/preprocessor/repetition/for.hpp>
# include <eina_boost/preprocessor/tuple/elem.hpp>
# include <eina_boost/preprocessor/tuple/rem.hpp>
#
# /* EFL_EINA_BOOST_PP_LIST_FOR_EACH_I */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG() && ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC()
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I(macro, data, list) EFL_EINA_BOOST_PP_FOR((macro, data, list, 0), EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_P, EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_O, EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M)
# else
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I(macro, data, list) EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_I(macro, data, list)
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_I(macro, data, list) EFL_EINA_BOOST_PP_FOR((macro, data, list, 0), EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_P, EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_O, EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M)
# endif
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_STRICT()
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_P(r, x) EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_P_D x
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_P_D(m, d, l, i) EFL_EINA_BOOST_PP_LIST_IS_CONS(l)
# else
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_P(r, x) EFL_EINA_BOOST_PP_LIST_IS_CONS(EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 2, x))
# endif
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_O(r, x) EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_O_D x
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_O_D(m, d, l, i) (m, d, EFL_EINA_BOOST_PP_LIST_REST(l), EFL_EINA_BOOST_PP_INC(i))
# else
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_O(r, x) (EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 0, x), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 1, x), EFL_EINA_BOOST_PP_LIST_REST(EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 2, x)), EFL_EINA_BOOST_PP_INC(EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 3, x)))
# endif
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M(r, x) EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M_D(r, EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 0, x), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 1, x), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 2, x), EFL_EINA_BOOST_PP_TUPLE_ELEM(4, 3, x))
# else
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M(r, x) EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M_I(r, EFL_EINA_BOOST_PP_TUPLE_REM_4 x)
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M_I(r, x_e) EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M_D(r, x_e)
# endif
#
# define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M_D(r, m, d, l, i) m(r, d, i, EFL_EINA_BOOST_PP_LIST_FIRST(l))
#
# /* EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_R */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_EDG()
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_R(r, macro, data, list) EFL_EINA_BOOST_PP_FOR_ ## r((macro, data, list, 0), EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_P, EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_O, EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M)
# else
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_R(r, macro, data, list) EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_R_I(r, macro, data, list)
#    define EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_R_I(r, macro, data, list) EFL_EINA_BOOST_PP_FOR_ ## r((macro, data, list, 0), EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_P, EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_O, EFL_EINA_BOOST_PP_LIST_FOR_EACH_I_M)
# endif
#
# endif
