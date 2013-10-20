
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
  eo3_simple(eo3_simple const& other)
    : _eo_raw(eo_ref(other._eo_raw))
  {}
  eo3_simple& operator=(eo3_simple const& other)
  {
    _eo_raw = eo_ref(other._eo_raw);
    return *this;
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

// namespace efl { namespace eo { namespace detail {

// template <>
// struct virtuals<eo3_simple>
// {
//   template <typename T>
//   struct type
//   {
//     virtual void simple_inc()
//     {
//       eo2_do_super(static_cast<T*>(this)->_eo_raw
// 		   , static_cast<T*>(this)->_eo_class
// 		   , ::simple_inc());
//     }
//     virtual int simple_get()
//     {
//       int r = 0;
//       eo2_do_super(static_cast<T*>(this)->_eo_raw
// 		   , static_cast<T*>(this)->_eo_class
// 		   , r = ::simple_get());
//       return r;
//     }
//     virtual void simple_set(int a0)
//     {
//       eo2_do_super(static_cast<T*>(this)->_eo_raw
// 		   , static_cast<T*>(this)->_eo_class
// 		   , ::simple_set(a0));
//     }
//     virtual void simple_virtual(int) = 0;
//   };
// };

// template <>
// struct operation_description_class_size<eo3_simple>
// {
//   static const int value = 4;
// };

// template <typename T>
// void funcao_simple_inc(Eo* objid EINA_UNUSED, Inherit_Private_Data* self)
// {
//   static_cast<T*>(self->this_)->simple_inc();
// }

// template <typename T>
// void initialize_operation_description(tag<eo3_simple>
// 				      , Eo2_Op_Description* ops)
// {
//   ops[0] = {&funcao_simple_inc<T>, & ::simple_inc, EO2_OP_OVERRIDE, EO_OP_TYPE_REGULAR, NULL};
//   ops[1] = {&funcao_simple_get<T>, & ::simple_get, EO2_OP_OVERRIDE, EO_OP_TYPE_REGULAR, NULL};
//   ops[2] = {&funcao_simple_set<T>, & ::simple_set, EO2_OP_OVERRIDE, EO_OP_TYPE_REGULAR, NULL};
//   ops[3] = {&funcao_simple_virtual<T>, & ::simple_virtual, EO2_OP_OVERRIDE, EO_OP_TYPE_REGULAR, NULL};
// }

// /* args_class => fusion:: | eo3_simple em args_class<> eh tag,, */
// void call_constructor(tag<eo3_simple>
// 		      , Eo* eo, Eo_Class const* cls
// 		      , args_class<eo3_simple, int> const& args)
// {
//   eo_do_super(eo, cls, fusion::at_c<0>(args));
// }

// inline Eo_Class const* get_eo_class(tag<eo3_simple>)
// {
//   return EO3_GET_CLASS(EO3_SIMPLE_CLASS);
// }

// } } }

// <end>
