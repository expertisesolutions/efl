
#define I BOOST_PP_ITERATION()

#if I
  template <BOOST_PP_ENUM_PARAMS(I, typename A)>
#endif
  inherit(BOOST_PP_ENUM_BINARY_PARAMS(I, A, a))
  {
    _eo_cls = detail::create_class
      <D, P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)
       , typename boost::fusion::result_of::make_vector<BOOST_PP_ENUM_PARAMS(I, A)>::type>();

    _eo_raw = eo2_add_custom
      (_eo_cls, NULL
       , detail::inherit_constructor<P BOOST_PP_ENUM_TRAILING_PARAMS(EFL_MAX_ARGS, E)
         , typename boost::fusion::result_of::make_vector<BOOST_PP_ENUM_PARAMS(I, A)>::type>
       (static_cast<void*>(this)
        , boost::fusion::make_vector(BOOST_PP_ENUM_PARAMS(I, a))));
  }

