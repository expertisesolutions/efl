# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  *
#  * See http://www.boost.org for most recent version.
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# ifndef EFL_EINA_BOOST_PREPROCESSOR_LIST_ADT_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_LIST_ADT_HPP
#
# include <eina_boost/preprocessor/config/config.hpp>
# include <eina_boost/preprocessor/detail/is_binary.hpp>
# include <eina_boost/preprocessor/logical/compl.hpp>
# include <eina_boost/preprocessor/tuple/eat.hpp>
#
# /* EFL_EINA_BOOST_PP_LIST_CONS */
#
# define EFL_EINA_BOOST_PP_LIST_CONS(head, tail) (head, tail)
#
# /* EFL_EINA_BOOST_PP_LIST_NIL */
#
# define EFL_EINA_BOOST_PP_LIST_NIL EFL_EINA_BOOST_PP_NIL
#
# /* EFL_EINA_BOOST_PP_LIST_FIRST */
#
# define EFL_EINA_BOOST_PP_LIST_FIRST(list) EFL_EINA_BOOST_PP_LIST_FIRST_D(list)
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_LIST_FIRST_D(list) EFL_EINA_BOOST_PP_LIST_FIRST_I list
# else
#    define EFL_EINA_BOOST_PP_LIST_FIRST_D(list) EFL_EINA_BOOST_PP_LIST_FIRST_I ## list
# endif
#
# define EFL_EINA_BOOST_PP_LIST_FIRST_I(head, tail) head
#
# /* EFL_EINA_BOOST_PP_LIST_REST */
#
# define EFL_EINA_BOOST_PP_LIST_REST(list) EFL_EINA_BOOST_PP_LIST_REST_D(list)
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_LIST_REST_D(list) EFL_EINA_BOOST_PP_LIST_REST_I list
# else
#    define EFL_EINA_BOOST_PP_LIST_REST_D(list) EFL_EINA_BOOST_PP_LIST_REST_I ## list
# endif
#
# define EFL_EINA_BOOST_PP_LIST_REST_I(head, tail) tail
#
# /* EFL_EINA_BOOST_PP_LIST_IS_CONS */
#
# if EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_BCC()
#    define EFL_EINA_BOOST_PP_LIST_IS_CONS(list) EFL_EINA_BOOST_PP_LIST_IS_CONS_D(list)
#    define EFL_EINA_BOOST_PP_LIST_IS_CONS_D(list) EFL_EINA_BOOST_PP_LIST_IS_CONS_ ## list
#    define EFL_EINA_BOOST_PP_LIST_IS_CONS_(head, tail) 1
#    define EFL_EINA_BOOST_PP_LIST_IS_CONS_EFL_EINA_BOOST_PP_NIL 0
# else
#    define EFL_EINA_BOOST_PP_LIST_IS_CONS(list) EFL_EINA_BOOST_PP_IS_BINARY(list)
# endif
#
# /* EFL_EINA_BOOST_PP_LIST_IS_NIL */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_BCC()
#    define EFL_EINA_BOOST_PP_LIST_IS_NIL(list) EFL_EINA_BOOST_PP_COMPL(EFL_EINA_BOOST_PP_IS_BINARY(list))
# else
#    define EFL_EINA_BOOST_PP_LIST_IS_NIL(list) EFL_EINA_BOOST_PP_COMPL(EFL_EINA_BOOST_PP_LIST_IS_CONS(list))
# endif
#
# endif
