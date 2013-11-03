
#ifndef GENCXX_EO3_INTERFACE_HPP
#define GENCXX_EO3_INTERFACE_HPP

#include <iostream>

#include "eo_base.hpp"
#include "eo_private.hpp"

/*
 * #define EO3_SIMPLE_INTERFACE eo3_simple_interface                   \
 *    , EO3_FUNCTION(isimple_inc, void)                                \
 *    , EO3_FUNCTION(isimple_get, int)                                 \
 *    , EO3_FUNCTION(isimple_set, void, int)
 *
 * EO3_DECLARE_INTERFACE(EO3_SIMPLE_INTERFACE)
 */

extern "C" {
#include "eo3_interface.h"
}
// <end>

struct eo3_interface : efl::eo::base
{
  explicit eo3_interface(Eo* eo) : efl::eo::base(eo) {}

  static Eo_Class const* _eo_class()
  {
    return EO3_GET_CLASS(EO3_SIMPLE_INTERFACE);
  }

  void isimple_inc()
  {
    eo2_do(_eo_ptr(), ::isimple_inc());
  }

  int isimple_get()
  {
    int r = 0;
    eo2_do(_eo_ptr(), r = ::isimple_get());
    return r;
  }

  void isimple_set(int a0)
  {
    eo2_do(_eo_ptr(), ::isimple_set(a0));
  }
};

namespace efl { namespace eo { namespace detail {

template <>
struct extension_inheritance< ::eo3_interface>
{
  template <typename D>
  struct type
  {
    operator eo3_interface() const
    {
      return eo3_interface
	(eo_ref(static_cast<D const*>(this)->_eo_ptr()));
    }

    void isimple_inc()
    {
      eo2_do(static_cast<D*>(this)->_eo_ptr(), ::isimple_inc());
    }

    int isimple_get()
    {
      int r = 0;
      eo2_do(static_cast<D*>(this)->_eo_ptr(), r = ::isimple_get());
      std::cout << "eo3_interface::simple_get " << r << std::endl;
      return r;
    }

    void isimple_set(int a0)
    {
      eo2_do(static_cast<D*>(this)->_eo_ptr(), ::isimple_set(a0));
    }
  };
};

} } }


// <end>

#endif // GENCXX_EO3_INTERFACE_HPP
