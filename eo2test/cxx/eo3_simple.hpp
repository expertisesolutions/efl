
#include "eo_private.hpp"
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

template <typename T>
void funcao_simple_inc(Eo* objid EINA_UNUSED, efl::eo::detail::Inherit_Private_Data* self)
{
  return static_cast<T*>(self->this_)->simple_inc();
}

template <typename T>
int funcao_simple_get(Eo* objid EINA_UNUSED, efl::eo::detail::Inherit_Private_Data* self)
{
  return static_cast<T*>(self->this_)->simple_get();
}

template <typename T>
void funcao_simple_set(Eo* objid EINA_UNUSED, efl::eo::detail::Inherit_Private_Data* self, int a0)
{
  return static_cast<T*>(self->this_)->simple_set(a0);
}

template <typename T>
void funcao_simple_virtual(Eo* objid EINA_UNUSED, efl::eo::detail::Inherit_Private_Data* self, int a0)
{
  return static_cast<T*>(self->this_)->simple_virtual(a0);
}

namespace efl { namespace eo { namespace detail {

template <>
struct virtuals<eo3_simple>
{
  template <typename T>
  struct type
  {
    virtual void simple_inc()
    {
      eo2_do_super(static_cast<T*>(this)->_eo_ptr()
		   , static_cast<T*>(this)->_eo_cls()
		   , ::simple_inc());
    }
    virtual int simple_get()
    {
      int r = 0;
      eo2_do_super(static_cast<T*>(this)->_eo_ptr()
		   , static_cast<T*>(this)->_eo_cls()
		   , r = ::simple_get());
      return r;
    }
    virtual void simple_set(int a0)
    {
      eo2_do_super(static_cast<T*>(this)->_eo_ptr()
		   , static_cast<T*>(this)->_eo_cls()
		   , ::simple_set(a0));
    }
    virtual void simple_virtual(int a0)
    {
      eo2_do_super(static_cast<T*>(this)->_eo_ptr()
		   , static_cast<T*>(this)->_eo_cls()
		   , ::simple_virtual(a0));
    }
  };
};

template <>
struct operation_description_class_size<eo3_simple>
{
  static const int value = 4;
};

template <typename T>
void initialize_operation_description(tag<eo3_simple>
				      , Eo2_Op_Description* ops)
{
  ops[0].func = reinterpret_cast<void*>(& ::funcao_simple_inc<T>);
  ops[0].api_func = reinterpret_cast<void*>(& ::simple_inc);
  ops[0].op = EO2_OP_OVERRIDE;
  ops[0].op_type = EO_OP_TYPE_REGULAR;
  ops[0].doc = NULL;
  
  ops[1].func = reinterpret_cast<void*>(& ::funcao_simple_get<T>);
  ops[1].api_func = reinterpret_cast<void*>(& ::simple_get);
  ops[1].op = EO2_OP_OVERRIDE;
  ops[1].op_type = EO_OP_TYPE_REGULAR;
  ops[1].doc = NULL;
  
  ops[2].func = reinterpret_cast<void*>(& ::funcao_simple_set<T>);
  ops[2].api_func = reinterpret_cast<void*>(& ::simple_set);
  ops[2].op = EO2_OP_OVERRIDE;
  ops[2].op_type = EO_OP_TYPE_REGULAR;
  ops[2].doc = NULL;

  ops[3].func = reinterpret_cast<void*>(& ::funcao_simple_virtual<T>);
  ops[3].api_func = reinterpret_cast<void*>(& ::simple_virtual);
  ops[3].op = EO2_OP_OVERRIDE;
  ops[3].op_type = EO_OP_TYPE_REGULAR;
  ops[3].doc = NULL;
}

/* args_class => fusion:: | eo3_simple em args_class<> eh tag,, */
void call_constructor(tag<eo3_simple>
		      , Eo* eo, Eo_Class const* cls
		      , args_class<eo3_simple, boost::fusion::vector<int> > const& args)
{
  std::cout << "call_constructor eo3_simple" << std::endl;
  eo2_do(eo/*, cls*/, ::simple_constructor(args.get<0>()));
}

inline Eo_Class const* get_eo_class(tag<eo3_simple>)
{
  return EO3_GET_CLASS(EO3_SIMPLE_CLASS);
}

} } }

// <end>
