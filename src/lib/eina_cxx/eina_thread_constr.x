
#define N EFL_EINA_BOOST_PP_ITERATION()

template <typename F EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
thread(F const& f EFL_EINA_BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, A, a))
  : _joinable(true)
{
  namespace fusion = ::efl_eina_boost::fusion;
  typedef typename fusion::result_of::make_vector
    <F const*, Eina_Condition, Eina_Lock, bool
     EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(N, A)>
    ::type arguments_type;

  Eina_Condition c;
  Eina_Lock l;
  arguments_type arguments (&f, c, l, false
                            EFL_EINA_BOOST_PP_ENUM_TRAILING_PARAMS(N, a));
  init(arguments);
}

