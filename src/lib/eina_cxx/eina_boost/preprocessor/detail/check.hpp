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
# ifndef EFL_EINA_BOOST_PREPROCESSOR_DETAIL_CHECK_HPP
# define EFL_EINA_BOOST_PREPROCESSOR_DETAIL_CHECK_HPP
#
# include <eina_boost/preprocessor/cat.hpp>
# include <eina_boost/preprocessor/config/config.hpp>
#
# /* EFL_EINA_BOOST_PP_CHECK */
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MWCC()
#    define EFL_EINA_BOOST_PP_CHECK(x, type) EFL_EINA_BOOST_PP_CHECK_D(x, type)
# else
#    define EFL_EINA_BOOST_PP_CHECK(x, type) EFL_EINA_BOOST_PP_CHECK_OO((x, type))
#    define EFL_EINA_BOOST_PP_CHECK_OO(par) EFL_EINA_BOOST_PP_CHECK_D ## par
# endif
#
# if ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC() && ~EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_DMC()
#    define EFL_EINA_BOOST_PP_CHECK_D(x, type) EFL_EINA_BOOST_PP_CHECK_1(EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_CHECK_RESULT_, type x))
#    define EFL_EINA_BOOST_PP_CHECK_1(chk) EFL_EINA_BOOST_PP_CHECK_2(chk)
#    define EFL_EINA_BOOST_PP_CHECK_2(res, _) res
# elif EFL_EINA_BOOST_PP_CONFIG_FLAGS() & EFL_EINA_BOOST_PP_CONFIG_MSVC()
#    define EFL_EINA_BOOST_PP_CHECK_D(x, type) EFL_EINA_BOOST_PP_CHECK_1(type x)
#    define EFL_EINA_BOOST_PP_CHECK_1(chk) EFL_EINA_BOOST_PP_CHECK_2(chk)
#    define EFL_EINA_BOOST_PP_CHECK_2(chk) EFL_EINA_BOOST_PP_CHECK_3((EFL_EINA_BOOST_PP_CHECK_RESULT_ ## chk))
#    define EFL_EINA_BOOST_PP_CHECK_3(im) EFL_EINA_BOOST_PP_CHECK_5(EFL_EINA_BOOST_PP_CHECK_4 im)
#    define EFL_EINA_BOOST_PP_CHECK_4(res, _) res
#    define EFL_EINA_BOOST_PP_CHECK_5(res) res
# else /* DMC */
#    define EFL_EINA_BOOST_PP_CHECK_D(x, type) EFL_EINA_BOOST_PP_CHECK_OO((type x))
#    define EFL_EINA_BOOST_PP_CHECK_OO(par) EFL_EINA_BOOST_PP_CHECK_0 ## par
#    define EFL_EINA_BOOST_PP_CHECK_0(chk) EFL_EINA_BOOST_PP_CHECK_1(EFL_EINA_BOOST_PP_CAT(EFL_EINA_BOOST_PP_CHECK_RESULT_, chk))
#    define EFL_EINA_BOOST_PP_CHECK_1(chk) EFL_EINA_BOOST_PP_CHECK_2(chk)
#    define EFL_EINA_BOOST_PP_CHECK_2(res, _) res
# endif
#
# define EFL_EINA_BOOST_PP_CHECK_RESULT_1 1, EFL_EINA_BOOST_PP_NIL
#
# endif
