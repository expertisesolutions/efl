// Boost result_of library

//  Copyright Douglas Gregor 2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/utility
#ifndef EFL_EINA_BOOST_RESULT_OF_HPP
#define EFL_EINA_BOOST_RESULT_OF_HPP

#include <eina_boost/config.hpp>
#include <eina_boost/preprocessor/cat.hpp>
#include <eina_boost/preprocessor/iteration/iterate.hpp>
#include <eina_boost/preprocessor/repetition/enum_params.hpp>
#include <eina_boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <eina_boost/preprocessor/repetition/enum_binary_params.hpp>
#include <eina_boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <eina_boost/preprocessor/facilities/intercept.hpp>
#include <eina_boost/detail/workaround.hpp>
#include <eina_boost/mpl/has_xxx.hpp>
#include <eina_boost/mpl/if.hpp>
#include <eina_boost/mpl/eval_if.hpp>
#include <eina_boost/mpl/bool.hpp>
#include <eina_boost/mpl/identity.hpp>
#include <eina_boost/mpl/or.hpp>
#include <eina_boost/type_traits/is_class.hpp>
#include <eina_boost/type_traits/is_pointer.hpp>
#include <eina_boost/type_traits/is_member_function_pointer.hpp>
#include <eina_boost/type_traits/remove_cv.hpp>
#include <eina_boost/type_traits/remove_reference.hpp>
#include <eina_boost/utility/declval.hpp>
#include <eina_boost/utility/enable_if.hpp>

#ifndef EFL_EINA_BOOST_RESULT_OF_NUM_ARGS
#  define EFL_EINA_BOOST_RESULT_OF_NUM_ARGS 16
#endif

// Use the decltype-based version of result_of by default if the compiler
// supports N3276 <http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2011/n3276.pdf>.
// The user can force the choice by defining EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE,
// EFL_EINA_BOOST_RESULT_OF_USE_TR1, or EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK but not more than one!
#if (defined(EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE) && defined(EFL_EINA_BOOST_RESULT_OF_USE_TR1)) || \
    (defined(EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE) && defined(EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK)) || \
    (defined(EFL_EINA_BOOST_RESULT_OF_USE_TR1) && defined(EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK))
#  error More than one of EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE, EFL_EINA_BOOST_RESULT_OF_USE_TR1 and \
  EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK cannot be defined at the same time.
#endif

#if defined(EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK) && defined(EFL_EINA_BOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE)
#  error Cannot fallback to decltype if EFL_EINA_BOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE is not defined.
#endif

#ifndef EFL_EINA_BOOST_RESULT_OF_USE_TR1
#  ifndef EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE
#    ifndef EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK
#      ifndef EFL_EINA_BOOST_NO_CXX11_DECLTYPE_N3276 // this implies !defined(EFL_EINA_BOOST_NO_CXX11_DECLTYPE)
#        define EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE
#      else
#        define EFL_EINA_BOOST_RESULT_OF_USE_TR1
#      endif
#    endif
#  endif
#endif

namespace efl_eina_boost {

template<typename F> struct result_of;
template<typename F> struct tr1_result_of; // a TR1-style implementation of result_of

#if !defined(EFL_EINA_BOOST_NO_SFINAE) && !defined(EFL_EINA_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
namespace detail {

EFL_EINA_BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)

EFL_EINA_BOOST_MPL_HAS_XXX_TEMPLATE_DEF(result)

template<typename F, typename FArgs, bool HasResultType> struct tr1_result_of_impl;

template<typename F> struct cpp0x_result_of;

#ifdef EFL_EINA_BOOST_NO_SFINAE_EXPR

// There doesn't seem to be any other way to turn this off such that the presence of
// the user-defined operator,() below doesn't cause spurious warning all over the place,
// so unconditionally turn it off.
#if EFL_EINA_BOOST_MSVC
#  pragma warning(disable: 4913) // user defined binary operator ',' exists but no overload could convert all operands, default built-in binary operator ',' used
#endif

struct result_of_private_type {};

struct result_of_weird_type {
  friend result_of_private_type operator,(result_of_private_type, result_of_weird_type);
};

typedef char result_of_yes_type;      // sizeof(result_of_yes_type) == 1
typedef char (&result_of_no_type)[2]; // sizeof(result_of_no_type)  == 2

template<typename T>
result_of_no_type result_of_is_private_type(T const &);
result_of_yes_type result_of_is_private_type(result_of_private_type);

template<typename C>
struct result_of_callable_class : C {
    result_of_callable_class();
    typedef result_of_private_type const &(*pfn_t)(...);
    operator pfn_t() const volatile;
};

template<typename C>
struct result_of_wrap_callable_class {
  typedef result_of_callable_class<C> type;
};

template<typename C>
struct result_of_wrap_callable_class<C const> {
  typedef result_of_callable_class<C> const type;
};

template<typename C>
struct result_of_wrap_callable_class<C volatile> {
  typedef result_of_callable_class<C> volatile type;
};

template<typename C>
struct result_of_wrap_callable_class<C const volatile> {
  typedef result_of_callable_class<C> const volatile type;
};

template<typename C>
struct result_of_wrap_callable_class<C &> {
  typedef typename result_of_wrap_callable_class<C>::type &type;
};

template<typename F, bool TestCallability = true> struct cpp0x_result_of_impl;

#else // EFL_EINA_BOOST_NO_SFINAE_EXPR

template<typename T>
struct result_of_always_void
{
  typedef void type;
};

template<typename F, typename Enable = void> struct cpp0x_result_of_impl {};

#endif // EFL_EINA_BOOST_NO_SFINAE_EXPR

template<typename F>
struct result_of_void_impl
{
  typedef void type;
};

template<typename R>
struct result_of_void_impl<R (*)(void)>
{
  typedef R type;
};

template<typename R>
struct result_of_void_impl<R (&)(void)>
{
  typedef R type;
};

// Determine the return type of a function pointer or pointer to member.
template<typename F, typename FArgs>
struct result_of_pointer
  : tr1_result_of_impl<typename remove_cv<F>::type, FArgs, false> { };

template<typename F, typename FArgs>
struct tr1_result_of_impl<F, FArgs, true>
{
  typedef typename F::result_type type;
};

template<typename FArgs>
struct is_function_with_no_args : mpl::false_ {};

template<typename F>
struct is_function_with_no_args<F(void)> : mpl::true_ {};

template<typename F, typename FArgs>
struct result_of_nested_result : F::template result<FArgs>
{};

template<typename F, typename FArgs>
struct tr1_result_of_impl<F, FArgs, false>
  : mpl::if_<is_function_with_no_args<FArgs>,
             result_of_void_impl<F>,
             result_of_nested_result<F, FArgs> >::type
{};

} // end namespace detail

#define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 (3,(0,EFL_EINA_BOOST_RESULT_OF_NUM_ARGS,<eina_boost/utility/detail/result_of_iterate.hpp>))
#include EFL_EINA_BOOST_PP_ITERATE()

#else
#  define EFL_EINA_BOOST_NO_RESULT_OF 1
#endif

}

#endif // EFL_EINA_BOOST_RESULT_OF_HPP
