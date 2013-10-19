
namespace efl { namespace eo {


namespace detail {
template <typename T> struct operation_description_class_size;

template <> struct operation_description_class_size<void>
{
  static const int value = 0;
};

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

template <typename T> struct tag {};

template <typename T> struct virtuals;

struct Inherit_Private_Data
{
  void* this_;
};

void inherit_constructor(Eo* obj, Inherit_Private_Data* self, void* this_)
{
  self->this_ = this_;
}

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
    my_class = eo_class_new(&class_desc, QUAISSAOMEUSPARENTS, (void*)NULL);
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
{
  typedef inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> inherit_base;
  template <typename A0>
  inherit(A0 a0)
  {
    _eo_class = create_class<D, P, E1, E2, E3, E4, E5, E6, E7, E8>();
    _eo_raw = eo_add_custom
      (c, NULL, detail::inherit_constructor(static_cast<void*>(this)));
  }
  template <typename A0, typename A1>
  inherit(A0 a0, A1 a1)
  {
  }
  // ...

  
};

} }
