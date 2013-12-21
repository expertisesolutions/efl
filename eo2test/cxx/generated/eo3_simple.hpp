#ifndef GENCXX_EO3_SIMPLE_HPP
#define GENCXX_EO3_SIMPLE_HPP

#include "eo_private.hpp"
#include "eo_base.hpp"
#include "eo_callback.hpp"

#include <memory>

/*
 * #define EO3_SIMPLE_CLASS eo3_simple                            \
 *     , EO3_CONSTRUCTOR(simple_constructor, _constructor, int)   \
 *     , EO3_DESTRUCTOR(_destructor)                              \
 *     , EO3_FUNCTION(simple_inc, _inc, void)                     \
 *     , EO3_FUNCTION(simple_get, _get, int)                      \
 *     , EO3_FUNCTION(simple_set, _set, void, int)                \
 *     , EO3_FUNCTION(simple_virtual, _set, void, int)            \
 *     , EO3_EVENT(simple_set_event, int)
 *
 * EO3_DECLARE_CLASS(EO3_SIMPLE_CLASS) 
 */

extern "C" {
#include "eo3_simple.h"
}

struct eo3_simple
    : efl::eo::base
{
  explicit eo3_simple(Eo* eo)
    : efl::eo::base(eo)
  {}

  eo3_simple(int a0)
    : efl::eo::base(_c1(a0))
  {}

  eo3_simple(eo3_simple const& other)
    : efl::eo::base(eo_ref(other._eo_ptr()))
  {}

  ~eo3_simple() {}

  void simple_inc()
  {
    eo2_do(_eo_ptr(), ::simple_inc());
  }

  int simple_get()
  {
    int r = (int) 0;
    eo2_do(_eo_ptr(), r = ::simple_get());
    return r;
  }

  void simple_set(int a1)
  {
    eo2_do(_eo_ptr(), ::simple_set(a1));
  }

  void simple_virtual(int a2)
  {
    eo2_do(_eo_ptr(), ::simple_virtual(a2));
  }

  static Eo_Class const* _eo_class()
  {
    return(EO3_GET_CLASS(EO3_SIMPLE_CLASS));
  }

  template <typename F>
  ::efl::eo::callback_token simple_set_callback_add(F function)
  {
    std::auto_ptr<F> f(new F(function));
    ::efl::eo::callback_token t
      = { f.get()
      , reinterpret_cast<void*>(static_cast<Eina_Bool(*)(void*, int)>(&::efl::eo::detail::callback_function_object1<F, int>))
      , &::efl::eo::detail::free_callback<F>
      };
    eo2_do(_eo_ptr(), ::simple_set_callback_add
      (f.release(), &::efl::eo::detail::callback_function_object1<F, int>));
    return t;
  }

  void simple_set_callback_del(::efl::eo::callback_token t)
  {
    typedef Eina_Bool(*function_type)(void*, int);
    eo2_do(_eo_ptr(), ::simple_set_callback_del(t.data, (function_type)t.function));
    t.free_function(t.data);
  }

  void simple_set_callback_call(int a3)
  {
    eo2_do(_eo_ptr(), ::simple_set_callback_call(a3));
  }

private:
  static Eo* _c1(int a4)
  {
    return eo2_add_custom(EO3_GET_CLASS(EO3_SIMPLE_CLASS), NULL, ::simple_constructor(a4));
  }

};
template <typename T>
void simple_inc_wrapper(Eo* objid EINA_UNUSED, efl::eo::detail::Inherit_Private_Data* self)
{
  return static_cast<T*>(self->this_)->simple_inc();
}

template <typename T>
int simple_get_wrapper(Eo* objid EINA_UNUSED, efl::eo::detail::Inherit_Private_Data* self)
{
  return static_cast<T*>(self->this_)->simple_get();
}

template <typename T>
void simple_set_wrapper(Eo* objid EINA_UNUSED, efl::eo::detail::Inherit_Private_Data* self, int a5)
{
  return static_cast<T*>(self->this_)->simple_set(a5);
}

template <typename T>
void simple_virtual_wrapper(Eo* objid EINA_UNUSED, efl::eo::detail::Inherit_Private_Data* self, int a6)
{
  return static_cast<T*>(self->this_)->simple_virtual(a6);
}

namespace efl { namespace eo { namespace detail {

template<>
struct operations<eo3_simple>
{
  template <typename T>
  struct type
  {
    virtual void simple_inc()
    {
      eo2_do_super(static_cast<T*>(this)->_eo_ptr()
        , static_cast<T*>(this)->_eo_class()
        , ::simple_inc());
    }

    virtual int simple_get()
    {
      int r = (int) 0;
      eo2_do_super(static_cast<T*>(this)->_eo_ptr()
        , static_cast<T*>(this)->_eo_class()
        , r = ::simple_get());
      return r;
    }

    virtual void simple_set(int a7)
    {
      eo2_do_super(static_cast<T*>(this)->_eo_ptr()
        , static_cast<T*>(this)->_eo_class()
        , ::simple_set(a7));
    }

    virtual void simple_virtual(int a8)
    {
      eo2_do_super(static_cast<T*>(this)->_eo_ptr()
        , static_cast<T*>(this)->_eo_class()
        , ::simple_virtual(a8));
    }

  };
};

template<>
struct operation_description_class_size<eo3_simple>
{
  static const int value = 4;
};

template <typename T>
void initialize_operation_description(efl::eo::detail::tag<eo3_simple>
              , Eo2_Op_Description* ops)
{
  ops[0].func = reinterpret_cast<void*>(& ::simple_inc_wrapper<T>);
  ops[0].api_func = reinterpret_cast<void*>(& ::simple_inc);
  ops[0].op = EO2_OP_OVERRIDE;
  ops[0].op_type = EO_OP_TYPE_REGULAR;
  ops[0].doc = NULL;

  ops[1].func = reinterpret_cast<void*>(& ::simple_get_wrapper<T>);
  ops[1].api_func = reinterpret_cast<void*>(& ::simple_get);
  ops[1].op = EO2_OP_OVERRIDE;
  ops[1].op_type = EO_OP_TYPE_REGULAR;
  ops[1].doc = NULL;

  ops[2].func = reinterpret_cast<void*>(& ::simple_set_wrapper<T>);
  ops[2].api_func = reinterpret_cast<void*>(& ::simple_set);
  ops[2].op = EO2_OP_OVERRIDE;
  ops[2].op_type = EO_OP_TYPE_REGULAR;
  ops[2].doc = NULL;

  ops[3].func = reinterpret_cast<void*>(& ::simple_virtual_wrapper<T>);
  ops[3].api_func = reinterpret_cast<void*>(& ::simple_virtual);
  ops[3].op = EO2_OP_OVERRIDE;
  ops[3].op_type = EO_OP_TYPE_REGULAR;
  ops[3].doc = NULL;

}

void call_constructor(tag<eo3_simple>
          , Eo* eo, Eo_Class const* cls EINA_UNUSED,
          args_class<eo3_simple,boost::fusion::vector<int> > const& args)
{
  eo2_do(eo, ::simple_constructor(args.get<0>()));
}

inline Eo_Class const* get_eo_class(tag<eo3_simple>)
{
  return EO3_GET_CLASS(EO3_SIMPLE_CLASS);
}

} } }

// <end>

#endif // GENCXX_EO3_SIMPLE_HPP

