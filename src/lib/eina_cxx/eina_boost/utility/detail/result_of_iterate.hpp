// Boost result_of library

//  Copyright Douglas Gregor 2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  Copyright Daniel Walker, Eric Niebler, Michel Morin 2008-2012.
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or
//  copy at http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/utility
#if !defined(EFL_EINA_BOOST_PP_IS_ITERATING)
# error Boost result_of - do not include this file!
#endif

// CWPro8 requires an argument in a function type specialization
#if EFL_EINA_BOOST_WORKAROUND(__MWERKS__, EFL_EINA_BOOST_TESTED_AT(0x3002)) && EFL_EINA_BOOST_PP_ITERATION() == 0
# define EFL_EINA_BOOST_RESULT_OF_ARGS void
#else
# define EFL_EINA_BOOST_RESULT_OF_ARGS EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T)
#endif

#if !EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x551))
template<typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct tr1_result_of<F(EFL_EINA_BOOST_RESULT_OF_ARGS)>
    : mpl::if_<
          mpl::or_< is_pointer<F>, is_member_function_pointer<F> >
        , efl_eina_boost::detail::tr1_result_of_impl<
            typename remove_cv<F>::type,
            typename remove_cv<F>::type(EFL_EINA_BOOST_RESULT_OF_ARGS),
            (efl_eina_boost::detail::has_result_type<F>::value)>
        , efl_eina_boost::detail::tr1_result_of_impl<
            F,
            F(EFL_EINA_BOOST_RESULT_OF_ARGS),
            (efl_eina_boost::detail::has_result_type<F>::value)> >::type { };
#endif

#ifdef EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE
template<typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct result_of<F(EFL_EINA_BOOST_RESULT_OF_ARGS)>
    : detail::cpp0x_result_of<F(EFL_EINA_BOOST_RESULT_OF_ARGS)> { };
#endif // EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE

#ifdef EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK
template<typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct result_of<F(EFL_EINA_BOOST_RESULT_OF_ARGS)>
    : mpl::if_<mpl::or_<detail::has_result_type<F>, detail::has_result<F> >,
               tr1_result_of<F(EFL_EINA_BOOST_RESULT_OF_ARGS)>,
               detail::cpp0x_result_of<F(EFL_EINA_BOOST_RESULT_OF_ARGS)> >::type { };
#endif // EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK

#if defined(EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE) || defined(EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK)

namespace detail {

template<typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct cpp0x_result_of<F(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T))>
    : mpl::if_<
          is_member_function_pointer<F>
        , detail::tr1_result_of_impl<
            typename remove_cv<F>::type,
            typename remove_cv<F>::type(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T)), false
          >
        , detail::cpp0x_result_of_impl<
              F(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T))
          >
      >::type
{};

#ifdef EFL_EINA_BOOST_NO_SFINAE_EXPR

template<typename F>
struct EFL_EINA_BOOST_PP_CAT(result_of_callable_fun_2_, EFL_EINA_BOOST_PP_ITERATION());

template<typename R EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(), typename T)>
struct EFL_EINA_BOOST_PP_CAT(result_of_callable_fun_2_, EFL_EINA_BOOST_PP_ITERATION())<R(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(), T))> {
    R operator()(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(), T)) const;
    typedef result_of_private_type const &(*pfn_t)(...);
    operator pfn_t() const volatile;
};

template<typename F>
struct EFL_EINA_BOOST_PP_CAT(result_of_callable_fun_, EFL_EINA_BOOST_PP_ITERATION());

template<typename F>
struct EFL_EINA_BOOST_PP_CAT(result_of_callable_fun_, EFL_EINA_BOOST_PP_ITERATION())<F *>
  : EFL_EINA_BOOST_PP_CAT(result_of_callable_fun_2_, EFL_EINA_BOOST_PP_ITERATION())<F>
{};

template<typename F>
struct EFL_EINA_BOOST_PP_CAT(result_of_callable_fun_, EFL_EINA_BOOST_PP_ITERATION())<F &>
  : EFL_EINA_BOOST_PP_CAT(result_of_callable_fun_2_, EFL_EINA_BOOST_PP_ITERATION())<F>
{};

template<typename F>
struct EFL_EINA_BOOST_PP_CAT(result_of_select_call_wrapper_type_, EFL_EINA_BOOST_PP_ITERATION())
  : mpl::eval_if<
        is_class<typename remove_reference<F>::type>,
        result_of_wrap_callable_class<F>,
        mpl::identity<EFL_EINA_BOOST_PP_CAT(result_of_callable_fun_, EFL_EINA_BOOST_PP_ITERATION())<typename remove_cv<F>::type> >
    >
{};

template<typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(), typename T)>
struct EFL_EINA_BOOST_PP_CAT(result_of_is_callable_, EFL_EINA_BOOST_PP_ITERATION()) {
    typedef typename EFL_EINA_BOOST_PP_CAT(result_of_select_call_wrapper_type_, EFL_EINA_BOOST_PP_ITERATION())<F>::type wrapper_t;
    static const bool value = (
        sizeof(result_of_no_type) == sizeof(detail::result_of_is_private_type(
            (efl_eina_boost::declval<wrapper_t>()(EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS(EFL_EINA_BOOST_PP_ITERATION(), efl_eina_boost::declval<T, >() EFL_EINA_BOOST_PP_INTERCEPT)), result_of_weird_type())
        ))
    );
    typedef mpl::bool_<value> type;
};

template<typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct cpp0x_result_of_impl<F(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T)), true>
    : lazy_enable_if<
          EFL_EINA_BOOST_PP_CAT(result_of_is_callable_, EFL_EINA_BOOST_PP_ITERATION())<F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(), T)>
        , cpp0x_result_of_impl<F(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T)), false>
      >
{};

template<typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct cpp0x_result_of_impl<F(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T)), false>
{
  typedef decltype(
    efl_eina_boost::declval<F>()(
      EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS(EFL_EINA_BOOST_PP_ITERATION(), efl_eina_boost::declval<T, >() EFL_EINA_BOOST_PP_INTERCEPT)
    )
  ) type;
};

#else // EFL_EINA_BOOST_NO_SFINAE_EXPR

template<typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct cpp0x_result_of_impl<F(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T)),
                            typename result_of_always_void<decltype(
                                efl_eina_boost::declval<F>()(
                                    EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS(EFL_EINA_BOOST_PP_ITERATION(), efl_eina_boost::declval<T, >() EFL_EINA_BOOST_PP_INTERCEPT)
                                )
                            )>::type> {
  typedef decltype(
    efl_eina_boost::declval<F>()(
      EFL_EINA_BOOST_PP_ENUM_BINARY_PARAMS(EFL_EINA_BOOST_PP_ITERATION(), efl_eina_boost::declval<T, >() EFL_EINA_BOOST_PP_INTERCEPT)
    )
  ) type;
};

#endif // EFL_EINA_BOOST_NO_SFINAE_EXPR

} // namespace detail

#else // defined(EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE) || defined(EFL_EINA_BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK)

#if !EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x551))
template<typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct result_of<F(EFL_EINA_BOOST_RESULT_OF_ARGS)>
    : tr1_result_of<F(EFL_EINA_BOOST_RESULT_OF_ARGS)> { };
#endif

#endif // defined(EFL_EINA_BOOST_RESULT_OF_USE_DECLTYPE)

#undef EFL_EINA_BOOST_RESULT_OF_ARGS

#if EFL_EINA_BOOST_PP_ITERATION() >= 1

namespace detail {

template<typename R,  typename FArgs EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct tr1_result_of_impl<R (*)(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T)), FArgs, false>
{
  typedef R type;
};

template<typename R,  typename FArgs EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct tr1_result_of_impl<R (&)(EFL_EINA_BOOST_PP_ENUM_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T)), FArgs, false>
{
  typedef R type;
};

#if !EFL_EINA_BOOST_WORKAROUND(__BORLANDC__, EFL_EINA_BOOST_TESTED_AT(0x551))
template<typename R, typename FArgs EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct tr1_result_of_impl<R (T0::*)
                     (EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T)),
                 FArgs, false>
{
  typedef R type;
};

template<typename R, typename FArgs EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct tr1_result_of_impl<R (T0::*)
                     (EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T))
                     const,
                 FArgs, false>
{
  typedef R type;
};

template<typename R, typename FArgs EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct tr1_result_of_impl<R (T0::*)
                     (EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T))
                     volatile,
                 FArgs, false>
{
  typedef R type;
};

template<typename R, typename FArgs EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),typename T)>
struct tr1_result_of_impl<R (T0::*)
                     (EFL_EINA_BOOST_PP_ENUM_SHIFTED_PARAMS(EFL_EINA_BOOST_PP_ITERATION(),T))
                     const volatile,
                 FArgs, false>
{
  typedef R type;
};
#endif

}
#endif
