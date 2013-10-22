
#ifndef EO3_EO_PRIVATE_HPP
#define EO3_EO_PRIVATE_HPP

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>

namespace efl { namespace eo { namespace detail {

template <typename T, typename Seq>
struct args_class
{
  args_class(Seq seq)
    : seq(seq) {}

  template <int N>
  typename boost::remove_reference
  <
    typename boost::fusion::result_of::at_c<Seq, N>::type
  >::type get() const
  {
    return boost::fusion::at_c<N>(seq);
  }

  Seq seq;
};

}

template <typename T, typename A0>
detail::args_class<T, boost::fusion::vector<A0> > args(A0 a0)
{
  return detail::args_class<T, boost::fusion::vector<A0> >
    (boost::fusion::vector<A0>(a0));
}

template <typename T, typename A0, typename A1>
detail::args_class<T, boost::fusion::vector<A0, A1> > args(A0 a0, A1 a1)
{
  return detail::args_class<T, boost::fusion::vector<A0, A1> >
    (boost::fusion::vector<A0, A1>(a0, a1));
}

template <typename T, typename A0, typename A1, typename A2>
detail::args_class<T, boost::fusion::vector<A0, A1, A2> > args(A0 a0, A1 a1, A2 a2)
{
  return detail::args_class<T, boost::fusion::vector<A0, A1, A2> >
    (boost::fusion::vector<A0, A1, A2>(a0, a1, a2));
}

template <typename T, typename A0, typename A1, typename A2, typename A3>
detail::args_class<T, boost::fusion::vector<A0, A1, A2, A3> > args
(A0 a0, A1 a1, A2 a2, A3 a3)
{
  return detail::args_class<T, boost::fusion::vector<A0, A1, A2, A3> >
    (boost::fusion::vector<A0, A1, A2, A3>(a0, a1, a2, a3));
}

namespace detail {

template <typename T> struct tag {};
template <typename T> struct virtuals;

template <> struct virtuals<void>
{
  template <typename D>
  struct type {};
};

template <typename T> struct operation_description_class_size;

template <> struct operation_description_class_size<void>
{
  static const int value = 0;
};

template <typename P, typename E1, typename E2, typename E3, typename E4
	  , typename E5, typename E6, typename E7, typename E8>
struct do_eo_class_new
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, get_eo_class(tag<E1>()), get_eo_class(tag<E2>())
			, get_eo_class(tag<E3>()), get_eo_class(tag<E4>())
			, get_eo_class(tag<E5>()), get_eo_class(tag<E6>())
			, get_eo_class(tag<E7>()), get_eo_class(tag<E8>())
                        , (void*)NULL);
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
			, get_eo_class(tag<E3>()), get_eo_class(tag<E4>())
			, get_eo_class(tag<E5>()), get_eo_class(tag<E6>())
			, get_eo_class(tag<E7>())
			, (void*)NULL);
  }
};

template <typename P, typename E1, typename E2, typename E3, typename E4
	  , typename E5, typename E6>
struct do_eo_class_new<P, E1, E2, E3, E4, E5, E6, void, void>
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, get_eo_class(tag<E1>()), get_eo_class(tag<E2>())
			, get_eo_class(tag<E3>()), get_eo_class(tag<E4>())
			, get_eo_class(tag<E5>()), get_eo_class(tag<E6>())
			, (void*)NULL);
  }
};

template <typename P, typename E1, typename E2, typename E3, typename E4
	  , typename E5>
struct do_eo_class_new<P, E1, E2, E3, E4, E5, void, void, void>
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, get_eo_class(tag<E1>()), get_eo_class(tag<E2>())
			, get_eo_class(tag<E3>()), get_eo_class(tag<E4>())
			, get_eo_class(tag<E5>())
			, (void*)NULL);
  }
};

template <typename P, typename E1, typename E2, typename E3, typename E4>
struct do_eo_class_new<P, E1, E2, E3, E4, void, void, void, void>
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, get_eo_class(tag<E1>()), get_eo_class(tag<E2>())
			, get_eo_class(tag<E3>()), get_eo_class(tag<E4>())
			, (void*)NULL);
  }
};

template <typename P, typename E1, typename E2, typename E3>
struct do_eo_class_new<P, E1, E2, E3, void, void, void, void, void>
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, get_eo_class(tag<E1>()), get_eo_class(tag<E2>())
			, get_eo_class(tag<E3>())
			, (void*)NULL);
  }
};

template <typename P, typename E1, typename E2>
struct do_eo_class_new<P, E1, E2, void, void, void, void, void, void>
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, get_eo_class(tag<E1>()), get_eo_class(tag<E2>())
			, (void*)NULL);
  }
};

template <typename P, typename E1>
struct do_eo_class_new<P, E1, void, void, void, void, void, void, void>
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, get_eo_class(tag<E1>())
			, (void*)NULL);
  }
};

template <typename P>
struct do_eo_class_new<P, void, void, void, void, void, void, void, void>
{
  static Eo_Class const* do_(Eo_Class_Description& class_desc)
  {
    return eo_class_new(&class_desc, get_eo_class(tag<P>())
			, (void*)NULL);
  }
};

struct Inherit_Private_Data
{
  void* this_;
};

template <typename T>
void initialize_operation_description(tag<void>, Eo2_Op_Description*)
{
}

} } }

#endif
