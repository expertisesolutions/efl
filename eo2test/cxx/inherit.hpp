
#include "eo_private.hpp"

#include <boost/mpl/bool.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/find.hpp>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/at.hpp>

#include <cassert>

namespace efl { namespace eo {

template <typename D, typename P, typename E1 = void, typename E2 = void
	  , typename E3 = void
	  , typename E4 = void, typename E5 = void, typename E6 = void
	  , typename E7 = void, typename E8 = void /* ... */>
struct inherit;

namespace detail {

template <typename Args>
inline void call_constructor(tag<void>, Eo*, Eo_Class const*, Args) {}

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

template <typename T>
struct predicate
{
  template <typename Args>
  struct apply { typedef boost::mpl::false_ type; };
  template <typename V>
  struct apply<args_class<T, V> >
  {
    typedef boost::mpl::true_ type;
  };
};

template <typename T>
struct call_constructor_aux
{
  template <typename I, typename Args>
  static void do_aux(Args const&, Eo* eo, Eo_Class const* cls, boost::mpl::true_)
  {
    detail::call_constructor(tag<void>(), eo, cls
                             , args_class<T, boost::fusion::vector0<> >
                             (boost::fusion::vector0<>()));
  }

  template <typename I, typename Args>
  static void do_aux(Args const& args, Eo* eo, Eo_Class const* cls, boost::mpl::false_)
  {
    typedef typename
      boost::mpl::distance<typename boost::mpl::begin<Args>::type, I>::type
      index_type;
    detail::call_constructor
      (tag<T>(), eo, cls
       , boost::fusion::at<index_type>(args));
  }

  template <typename Args>
  static void do_(Args const& args, Eo* eo, Eo_Class const* cls)
  {
    typedef typename boost::mpl::find_if<Args, predicate<T> >::type
      iterator;
    do_aux<iterator>(args
                     , eo, cls
		     , typename boost::is_same
		     <
                       typename boost::mpl::end<Args>::type
		       , iterator
		     >::type());
  }
};

template <typename P, typename E1, typename E2, typename E3, typename E4
          , typename E5, typename E6, typename E7, typename E8, typename Args>
void inherit_constructor_impl(Eo* obj, Inherit_Private_Data* self, void* this_, Args args)
{
  std::cout << "inherit_constructor_impl" << std::endl;
  self->this_ = this_;
  Eo_Class const* cls
    = static_cast<inherit<P, E1, E2, E3, E4, E5, E6, E7, E8>*>(this_)->_eo_cls();
  detail::call_constructor_aux<P>::do_(args, obj, cls);
  detail::call_constructor_aux<E1>::do_(args, obj, cls);
  detail::call_constructor_aux<E2>::do_(args, obj, cls);
  detail::call_constructor_aux<E3>::do_(args, obj, cls);
  detail::call_constructor_aux<E4>::do_(args, obj, cls);
  detail::call_constructor_aux<E5>::do_(args, obj, cls);
  detail::call_constructor_aux<E6>::do_(args, obj, cls);
  detail::call_constructor_aux<E7>::do_(args, obj, cls);
  detail::call_constructor_aux<E8>::do_(args, obj, cls);
}

template <typename P, typename E1, typename E2, typename E3, typename E4
          , typename E5, typename E6, typename E7, typename E8, typename Args>
EAPI void inherit_constructor(void* this_, Args args)
{
  typedef void (*func_t)(Eo *, void *, void*, Args);
  Eo2_Op_Call_Data call;
  static Eo_Op op = EO_NOOP;
  if ( op == EO_NOOP )
    op = eo2_api_op_id_get((void*)&inherit_constructor
                           <P, E1, E2, E3, E4, E5, E6, E7, E8, Args>
                           , EO_OP_TYPE_REGULAR);
  if (!eo2_call_resolve(op, &call)) assert(eo2_call_resolve(op, &call));
  func_t func = (func_t) call.func;
  return func(call.obj, call.data, this_, args);
}

template <typename D, typename T>
struct conversion_operator
{
  operator T() const
  {
    eo_ref(static_cast<D const*>(this)->_eo_ptr());
    return T(static_cast<D const*>(this)->_eo_ptr());
  }
};

template <typename D>
struct conversion_operator<D, void> {};

template <typename D, typename P, typename E1, typename E2
	  , typename E3
	  , typename E4, typename E5, typename E6
	  , typename E7, typename E8 /* ... */
          , typename Args>
Eo_Class const* create_class(/*info*/)
{
  static const Eo_Class* my_class = NULL;
  static Eo2_Op_Description op_descs
    [ detail::operation_description_size
      <P, E1, E2, E3, E4, E5, E6, E7, E8>::value + 2 ];
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value].func =
    reinterpret_cast<void*>
    (
     static_cast<void(*)(Eo*, Inherit_Private_Data*, void*, Args)>
     (&detail::inherit_constructor_impl<P, E1, E2, E3, E4, E5, E6, E7, E8, Args>)
    );
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value].api_func =
    reinterpret_cast<void*>
    (
     static_cast<void(*)(void*, Args)>
     (&detail::inherit_constructor<P, E1, E2, E3, E4, E5, E6, E7, E8, Args>)
    );
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value].op = EO_NOOP;
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value].op_type = EO_OP_TYPE_REGULAR;
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value].doc = "";
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value+1].func = 0;
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value+1].api_func = 0;
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value+1].op = 0;
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value+1].op_type = EO_OP_TYPE_INVALID;
  op_descs[detail::operation_description_size
           <P, E1, E2, E3, E4, E5, E6, E7, E8>::value+1].doc = 0;
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
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<E3>()
     , &op_descs[detail::operation_description_size<P, E1, E2, E3>::value]);
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<E3>()
     , &op_descs[detail::operation_description_size<P, E1, E2, E3, E4>::value]);
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<E3>()
     , &op_descs[detail::operation_description_size<P, E1, E2, E3, E4, E5>::value]);
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<E3>()
     , &op_descs[detail::operation_description_size<P, E1, E2, E3, E4, E5, E6>::value]);
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<E3>()
     , &op_descs[detail::operation_description_size<P, E1, E2, E3, E4, E5, E6, E7>::value]);
  detail::initialize_operation_description
    <inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
    (detail::tag<E3>()
     , &op_descs[detail::operation_description_size<P, E1, E2, E3, E4, E5, E6, E7, E8>::value]);
  //locks
  if(!my_class)
  {
    static Eo_Class_Description class_desc = {
      EO2_VERSION,
      "C++ Inherited class",
      EO_CLASS_TYPE_REGULAR,
      EO2_CLASS_DESCRIPTION_OPS(op_descs),
      NULL,
      sizeof(detail::Inherit_Private_Data),
      NULL,
      NULL
    };
    my_class = detail::do_eo_class_new<P, E1, E2, E3, E4, E5, E6, E7, E8>::do_(class_desc);
  }
  return my_class;
}

}

template <typename D, typename P, typename E1, typename E2
	  , typename E3
	  , typename E4, typename E5, typename E6
	  , typename E7, typename E8 /* ... */>
struct inherit
  : detail::virtuals<P>::template type<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
  // , detail::virtuals<E1>::template type<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
  // , detail::virtuals<E2>::template type<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> >
  , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, P>
  // , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E1>
  // , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E2>
  // , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E3>
  // , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E4>
  // , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E5>
  // , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E6>
  // , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E7>
  // , detail::conversion_operator<inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8>, E8>
{
  typedef inherit<D, P, E1, E2, E3, E4, E5, E6, E7, E8> inherit_base;
  template <typename A0>
  inherit(A0 a0)
  {
    _eo_class = detail::create_class
      <D, P, E1, E2, E3, E4, E5, E6, E7, E8
       , typename boost::fusion::result_of::make_vector<A0>::type>();
    _eo_raw = eo2_add_custom
      (_eo_class, NULL
       , detail::inherit_constructor<P, E1, E2, E3, E4, E5, E6, E7, E8
       , typename boost::fusion::result_of::make_vector<A0>::type>
       (static_cast<void*>(this)
        , boost::fusion::make_vector(a0)));
  }
  template <typename A0, typename A1>
  inherit(A0 a0, A1 a1)
  {
  }
  // ...
  ~inherit()
  {
    eo_unref(_eo_raw);
  }
  Eo* _eo_ptr() const { return _eo_raw; }
  Eo_Class const* _eo_cls() const { return _eo_class; }
private:
  Eo_Class const* _eo_class;
  Eo* _eo_raw;
};

} }
