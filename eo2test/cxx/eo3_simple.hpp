
#include <iostream>

// #define EO3_SIMPLE_CLASS eo3_simple                                  \
//     , EO3_CONSTRUCTOR(simple_constructor, _constructor, int)         \
//     , EO3_DESTRUCTOR(_destructor)                                    \
//     , EO3_FUNCTION(simple_inc, _inc, void)                           \
//     , EO3_FUNCTION(simple_get, _get, int)                            \
//     , EO3_FUNCTION(simple_set, _set, void, int)                      \
//     , EO3_FUNCTION(simple_virtual, _set, void, int)                  \
//     , EO3_EVENT(simple_set_event, int)

// EO3_DECLARE_CLASS(EO3_SIMPLE_CLASS)

extern "C" {
#include "eo3_simple.h"
}
// <end>

// (not yet) generated code <begin>
struct eo3_simple// : Base
{
  //typedef Base _base_class_0;
  explicit eo3_simple(Eo* eo)
    : _eo_raw(eo) {}

  eo3_simple(int a0)
    : _eo_raw(0)
  {
    _eo_raw = eo2_add_custom(EO3_GET_CLASS(EO3_SIMPLE_CLASS)
			     , NULL, ::simple_constructor(a0));
  }
  ~eo3_simple()
  {
    eo_unref(_eo_raw);
  }

  void simple_inc()
  {
    eo2_do(_eo_raw, ::simple_inc());
  }

  int simple_get()
  {
    int r = 0;
    eo2_do(_eo_raw, r = ::simple_get());
    std::cout << "simple_get " << r << std::endl;
    return r;
  }

  void simple_set(int a0)
  {
    eo2_do(_eo_raw, ::simple_set(a0));
  }

  void simple_virtual(int a0)
  {
    eo2_do(_eo_raw, ::simple_virtual(a0));
  }

  Eo* _eo_ptr() const { return _eo_raw; }
protected:
  Eo* _eo_raw;
};

namespace efl { namespace eo { namespace detail {

template <>
struct virtuals<eo3_simple>
{
  template <typename T>
  struct type
  {
    virtual void simple_inc()
    {
      eo2_do_super(static_cast<T*>(this)->_eo_raw
		   , static_cast<T*>(this)->_eo_class
		   , ::simple_inc());
    }
    virtual int simple_get()
    {
      int r = 0;
      eo2_do_super(static_cast<T*>(this)->_eo_raw
		   , static_cast<T*>(this)->_eo_class
		   , r = ::simple_get());
      return r;
    }
    virtual void simple_set(int a0)
    {
      eo2_do_super(static_cast<T*>(this)->_eo_raw
		   , static_cast<T*>(this)->_eo_class
		   , ::simple_set(a0));
    }
    virtual void simple_virtual(int) = 0;
  };
};

template <>
struct operation_description_class_size<eo3_simple>
{
  static const int value = 4;
};

template <typename T>
void funcao_simple_inc(Eo* objid EINA_UNUSED, Inherit_Private_Data* self)
{
  static_cast<T*>(self->this_)->simple_inc();
}

template <typename T>
void initialize_operation_description(tag<eo3_simple>
				      , Eo2_Op_Description* ops)
{
  ops[0] = {&funcao_simple_inc<T>, & ::simple_inc, EO2_OP_OVERRIDE, EO_OP_TYPE_REGULAR, NULL};
  ops[1] = {&funcao_simple_get<T>, & ::simple_get, EO2_OP_OVERRIDE, EO_OP_TYPE_REGULAR, NULL};
  ops[2] = {&funcao_simple_set<T>, & ::simple_set, EO2_OP_OVERRIDE, EO_OP_TYPE_REGULAR, NULL};
  ops[3] = {&funcao_simple_virtual<T>, & ::simple_virtual, EO2_OP_OVERRIDE, EO_OP_TYPE_REGULAR, NULL};
}


} } }

// typedef simple1_class_internal<eo::base_class> simple1_class;

// struct simple1_class_virtual_members : eo::impl_base_class
// {
//   typedef eo::base_class _base_0;
//   explicit simple1_class_virtual_members(Eo* eo) : _base_0(eo) {}

//   virtual void simple1_inc() = 0;
//   virtual int simple1_get() = 0;
//   virtual void simple1_set(int) = 0;
// protected:
//   void simple1_value_set_hook_call(int x) const
//   {
//     eo_do(_eo_raw(), ::simple1_value_set_hook_call(x));
//   }
// };
// // <end>

// //// implementation detail (traits and meta-functions)
// template <typename Derived, typename B0, typename B1, typename B2 ...>
// struct inherit<Derived, B0, B1, B2, ...> : virtual_members<B0>::type, virtual_members<B1>::type, virtual_members<B2>::type, ...
// {
//   typedef inherit<Derived, B0, B1, B2, ...> _inherit_type;
//   typedef typename virtual_members<B0>::type _base0;
//   typedef typename virtual_members<B0>::type _base1;
//   typedef typename virtual_members<B0>::type _base2;
//   template <typename A0, typename A1, typename A2, ...>
//   inherit(A0 a0, A1 a1, A2 a2)
//     : _base0(instantiate_new_class<Derived>(a0, EO_GET_CLASS(B0), a1, EO_GET_CLASS(B1), a2, EO_GET_CLASS(B2), ...))
//     , _base1(_base0::_eo_raw())
//     , _base2(_base0::_eo_raw())
//     ...
//   {
//   }
// };

// template <>
// struct virtual_members_base<simple1_class> { typedef simple1_class_virtual_members type; }

// template <>
// struct non_virtual_members_base<simple1_class> { typedef eo::base_class type; }

// template <typename T>
// struct virtual_members;

// template <template <typename> C, typename T>
// struct virtual_members<C<T> >
// {
//   typedef C<typename virtual_members_base<C<T> >::type> type;
// };

// template <typename T>
// struct non_virtual_members;

// template <template <typename> C, typename T>
// struct non_virtual_members<C<T> >
// {
//   typedef C<typename non_virtual_members_base<C<T> >::type> type;
// };

// template <typename T, typename U, typename Enable = void>
// struct is_base_and_derived;

// template <typename T>
// struct is_user_implementation
//   : std::is_convertible_to<U, ::eo::impl_base_class>
// {
// };

// template <typename T, typename U>
// struct is_base_and_derived<T, U, typename std::enable_if<is_user_implementation<U> >::type>
//   : std::is_base_and_derived<T, typename non_virtual_members<U>::type>
// {

// };

// template <typename T, typename U>
// struct is_base_and_derived<T, U, typename std::disable_if<is_user_implementation<U> >::type>
//   : std::is_base_and_derived<T, U>
// {
  
// };

// template <typename T>
// struct ptr
// {
//   template <typename U>
//   ptr(U* p, typename std::enable_if<is_base_and_derived<T, U> >::type* = 0)
//     : object(p->_eo_raw())
//   {
    
//   }

//   T* operator->() const { return &object; }
//   // ...
// private:
//   T object;
// };

// // usage example <begin>
// struct derived_from_simple1_class : inherit<derived_from_simple1_class, simple1_class>
// {
//   derived_from_simple1_class(int x)
//     : inherit_type
//       (
//        args<simple1_class>(0) // args for simple1_class constructor
//       )
//   {
//   }

//   void simple1_inc()
//   {
//     std::cout << "inc was called on my derived instance" << std::endl;
//   }

//   int simple1_get()
//   {
//     std::cout << "get was called on my derived instance" << std::endl;
//     return 0;
//   }

//   void simple1_set(int x)
//   {
//     std::cout << "set was called on my derived instance" << std::endl;
//     simple1_value_set_hook_call(x);
//   }
// };

// void foo(ptr<simple1_class> s)
// {
//   s->simple1_inc();
// }

// void C_foo(ptr<simple1_class> s)
// {
//   eo_do(s->_eo_raw(), simple_inc());
// }

// <end>
