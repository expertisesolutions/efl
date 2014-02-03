
#ifndef EFL_EINA_BOOST_PP_IS_ITERATING

#ifndef EINA_CXX_CALLBACK_HELPER_HH
#define EINA_CXX_CALLBACK_HELPER_HH

#include <functional>
#include <eina_boost/function_types/parameter_types.hpp>
#include <eina_boost/function_types/is_function.hpp>
#include <eina_boost/function_types/function_pointer.hpp>
#include <eina_boost/function_types/result_type.hpp>
#include <eina_boost/mpl/find_if.hpp>
#include <eina_boost/mpl/pop_front.hpp>
#include <eina_boost/type_traits/remove_pointer.hpp>
#include <eina_boost/preprocessor/iterate.hpp>
#include <eina_boost/preprocessor/comma_if.hpp>
#include <memory>

namespace efl { namespace eina {

namespace _detail {

struct callback_helper_struct
{
  template <typename F>
  callback_helper_struct(F f)
    : f(f) {}

  std::function<void()> f;
};

template <typename Sig, typename F>
void* wrap_with_sig(F f)
{
  return new std::function<Sig>(std::bind(f));
}

template <typename T>
struct is_function
  : efl_eina_boost::function_types::is_function<typename efl_eina_boost::remove_pointer<T>::type>::type
{};

template <typename F>
struct caller
{
  typedef typename efl_eina_boost::function_types::parameter_types<F>::type parameter_types;
  typedef typename efl_eina_boost::mpl::find_if
  <parameter_types
   , is_function<efl_eina_boost::mpl::_> >::type function_iterator;
  EFL_EINA_BOOST_MPL_ASSERT((efl_eina_boost::mpl::not_
                             <efl_eina_boost::is_same<function_iterator
                             , typename efl_eina_boost::mpl::end<parameter_types>::type
                             > >));
  typedef typename efl_eina_boost::mpl::deref<function_iterator>::type type;
};

template <typename F>
struct wrapper
{
  typedef typename caller<F>::type function_type;

  struct type
  {
    template <typename T>
    void* operator()(T t) const
    {
      typedef typename efl_eina_boost::mpl::pop_front
        <typename efl_eina_boost::function_types::parameter_types<function_type>::type>::type
        sig_params_type;
      typedef typename efl_eina_boost::function_types::function_type
        <typename efl_eina_boost::mpl::push_front
         <sig_params_type
          , typename efl_eina_boost::function_types::result_type<function_type>::type>::type
         >::type sig_type;
      return new std::function<sig_type>(t);
    }
  };
};

template <int N, typename F, typename R>
struct call_holder;

#define EFL_EINA_BOOST_PP_ITERATION_PARAMS_1 (3, (1, 10, "eina_callback_helper.hh"))
#include EFL_EINA_BOOST_PP_ITERATE()

}

template <typename F>
typename _detail::wrapper<F>::type
wrap(F) { return typename _detail::wrapper<F>::type(); }


template <typename F>
typename _detail::caller<F>::type
call(F)
{
  typedef typename _detail::caller<F>::type type;
  typedef typename efl_eina_boost::function_types::parameter_types<type>::type parameter_types;
  return &_detail::call_holder<efl_eina_boost::mpl::size<parameter_types>::value
                               , parameter_types
                               , typename efl_eina_boost::function_types::result_type<type>::type>
    ::call;
}

} }

#endif

#else

template <typename F, typename R>
struct call_holder<EFL_EINA_BOOST_PP_ITERATION(), F, R>
{
#define EFL_EINA_EINA_CALLBACK_HELPER_AT_C(Z, N, DATA)                  \
  EFL_EINA_BOOST_PP_COMMA_IF(N) typename ::efl_eina_boost::mpl::at_c<F, N>::type EFL_EINA_BOOST_PP_CAT(a, N)
#define EFL_EINA_EINA_CALLBACK_HELPER_ARGS(Z, N, DATA)                  \
  EFL_EINA_BOOST_PP_COMMA_IF(N) EFL_EINA_BOOST_PP_CAT(a, EFL_EINA_BOOST_PP_INC(N))

  static R call(EFL_EINA_BOOST_PP_REPEAT(EFL_EINA_BOOST_PP_ITERATION(), EFL_EINA_EINA_CALLBACK_HELPER_AT_C, ~))
  {
    typedef typename efl_eina_boost::mpl::pop_front<F>::type sig_params_type;
    typedef typename efl_eina_boost::function_types::function_type
      <typename efl_eina_boost::mpl::push_front
       <sig_params_type, R>::type>::type sig_type;
    std::auto_ptr<std::function<sig_type> > f(static_cast<std::function<sig_type>*>(a0));
    return (*f)(EFL_EINA_BOOST_PP_REPEAT(EFL_EINA_BOOST_PP_DEC(EFL_EINA_BOOST_PP_ITERATION())
                                         , EFL_EINA_EINA_CALLBACK_HELPER_ARGS, ~));
  }
};

#endif
