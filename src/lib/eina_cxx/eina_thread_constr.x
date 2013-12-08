
#define N BOOST_PP_ITERATION()

template <typename F BOOST_PP_TRAILING_ENUM_PARAMS(N, typename A)>
thread(F const& f BOOST_PP_TRAILING_BINARY_ENUM_PARAMS(N, A, a))
  : _joinable(true)
{
  namespace fusion = boost::fusion;
  typedef typename fusion::result_of::make_vector
    <F*, Eina_Condition, Eina_Lock, bool BOOST_PP_TRAILING_ENUM_PARAMS(N, A)>
    ::type arguments_type;

  arguments_type arguments (&f, {}, {}, false
                            BOOST_PP_TRAILING_ENUM_PARAMS(N, a));
  init(arguments);
}

