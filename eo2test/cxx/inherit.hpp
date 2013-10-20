
namespace efl { namespace eo {


namespace detail {
template <typename T> struct operation_description_class_size;

template <> struct operation_description_class_size<void>
{
  static const int value = 0;
};

inline void call_constructor(tag<void>) {}

template <typename T0, typename T1 = void, typename T2 = void
	  , typename T3 = void, typename T4 = void
	  , typename T5 = void, typename T6 = void
	  , typename T7 = void, typename T8 = void>
struct operation_description_size
{
  static const int value = operation_description_class_size<T0>::value
    + operation_description_class_size<T1>::value
    + operation_description_class_size<T2>::value
    + operation_description_class_size<T3>::value
    + operation_description_class_size<T4>::value
    + operation_description_class_size<T5>::value
    + operation_description_class_size<T6>::value
    + operation_description_class_size<T7>::value
    + operation_description_class_size<T8>::value
    ;
};

template <typename P, typename E1, typename E2, typename E3, typename E4
	  , typename E5, typename E6, typename E7, typename E8>
struct do_eo_class_new
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, get_eo_class(tag<E1>()), get_eo_class(tag<E2>())
			, ..., (void*)NULL);
  }
};

template <typename P, typename E1, typename E2, typename E3, typename E4
	  , typename E5, typename E6, typename E7>
struct do_eo_class_new<P, E1, E2, E3, E4, E5, E6, E7, void>
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, get_eo_class(tag<E1>()), get_eo_class(tag<E2>())
			, ..., (void*)NULL);
  }
};

template <typename T> struct tag {};

template <typename T> struct virtuals;

struct Inherit_Private_Data
{
  void* this_;
};

template <typename T>
struct predicate
{
  template <typename Args>
  struct apply { typedef mpl::false_ type; };
  template <typename A0, typename A1, typename A2, ...>
  struct apply<args_class<T, A0, A1, A2, ...>
  {
    typedef mpl::true_ type;
  };
};

template <typename T>
struct call_constructor_aux
{
  template <typename I, typename Args>
  static void do_aux(Args const& args, Eo* eo, Eo_Class* cls, mpl::true_)
  {
    detail::call_constructor(tag<T>(), eo, cls);
  }

  template <typename I, typename Args>
  static void do_aux(Args const& args, Eo* eo, Eo_Class* cls, mpl::false_)
  {
    detail::call_constructor
      (tag<T>(), eo, cls
       , fusion::at<mpl::distance<mpl::begin<Args>, I> >(args));
  }

  template <typename Args>
  static void do_(Args const& args, Eo* eo, Eo_Class* cls)
  {
    typedef typename mpl::find_if<Args, predicate<T> >::type
      iterator;
    do_aux<iterator>(args.get<0>(), eo, cls
		     , typename boost::is_same
		     <
		       typename mpl::end<Args>::type
		       , iterator
		     >::type());
  }
};

template <typename P, E1, E2, E3, E4, E5, E6, E7, E8, typename Args>
void inherit_constructor(Eo* obj, Inherit_Private_Data* self, void* this_, Args args)
{
  Eo_Class const* cls = static_cast<inherit<P, E1, E2, E3, E4, E5, E6, E7, E8>*>(this_)->_eo_class;
  detail::call_constructor_aux<P>::do_(args, eo, cls);
  detail::call_constructor_aux<E1>::do_(args, eo, cls);
  // detail::call_constructor(tag<E2>());
  // detail::call_constructor(tag<E3>());
  // detail::call_constructor(tag<E4>());
  // detail::call_constructor(tag<E5>());
  // detail::call_constructor(tag<E6>());
  // detail::call_constructor(tag<E7>());
  // detail::call_constructor(tag<E8>());
  self->this_ = this_;
}

template <typename D, typename T>
struct conversion_operator
{
  operator T() const
  {
    eo_ref(static_cast<D const*>(this)->_eo_raw);
    return T(static_cast<D const*>(this)->_eo_raw);
  }
};

template <typename D>
struct conversion_operator<void> {};

}

template <typename D, typename P, typename E1 = void, typename E2 = void
	  , typename E3 = void
	  , typename E4 = void, typename E5 = void, typename E6 = void
	  , typename E7 = void, typename E8 = void /* ... */>
Eo_Class const* create_class(/*info*/)
{
  static const Eo_Class* my_class = NULL;
  static Eo2_Op_Description op_descs
    [ detail::operation_description_size
      <P, E1, E2, E3, E4, E5, E6, E7, E8>::size ];
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<P>(), op_descs);
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<E1>()
     , &op_descs[detail::operation_description_size<P>::value]);
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<E2>()
     , &op_descs[detail::operation_description_size<P, E1>::value]);
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<E3>()
     , &op_descs[detail::operation_description_size<P, E1, E2>::value]);
  //locks
  if(!my_class)
  {
    Eo_Class_Description class_desc = {
      EO2_VERSION,
      "C++ Inherited class",
      EO_CLASS_TYPE_REGULAR,
      EO2_CLASS_DESCRIPTION_OPS(op_descs),
      NULL,
      sizeof(Inherit_Private_Data),
      NULL,
      NULL
    };
    my_class = do_eo_class_new<P, E1, E2, E3, E4, E5, E6, E7, E8>::do_(class_desc);
  }
  return my_class;
}

template <typename D, typename P, typename E1 = void, typename E2 = void
	  , typename E3 = void
	  , typename E4 = void, typename E5 = void, typename E6 = void
	  , typename E7 = void, typename E8 = void /* ... */>
struct inherit
  : virtuals<P>::type<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
  , virtuals<E1>::type<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
  , virtuals<E2>::type<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, P>
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E1>
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E2>
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E3>
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E4>
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E5>
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E6>
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E7>
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E8>
{
  typedef inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> inherit_base;
  template <typename A0>
  inherit(A0 a0)
  {
    _eo_class = create_class<D, P, E1, E2, E3, E4, E5, E6, E7, E8>();
    _eo_raw = eo_add_custom
      (c, NULL, detail::inherit_constructor(static_cast<void*>(this)
					    , args_all(a0)));
  }
  template <typename A0, typename A1>
  inherit(A0 a0, A1 a1)
  {
  }
  // ...
  ~inhrerit()
  {
    eo_unref(_eo_raw);
  }
};

} }
