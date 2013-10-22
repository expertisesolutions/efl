
#include <iostream>

// #define EO3_SIMPLE_INTERFACE eo3_simple_interface                     \
//     , EO3_FUNCTION(isimple_inc, void)                                 \
//     , EO3_FUNCTION(isimple_get, int)                                  \
//     , EO3_FUNCTION(isimple_set, void, int)

// EO3_DECLARE_INTERFACE(EO3_SIMPLE_INTERFACE)

extern "C" {
#include "eo3_interface.h"
}
// <end>

struct Base
{
  explicit Base(Eo* eo) : _eo_raw(eo) {}

  Eo* _eo_ptr() const { return _eo_raw; }
protected:
  Eo* _eo_raw;
};

struct eo3_interface : Base
{
  eo3_interface(Eo* eo) : Base(eo) {}

  void isimple_inc()
  {
    eo2_do(_eo_ptr(), ::isimple_inc());
  }
};

namespace efl { namespace eo { namespace detail {

template <typename T>
struct extension_inheritance {};

template <>
struct extension_inheritance< ::eo3_interface>
{
  template <typename D>
  struct type
  {
    void isimple_inc()
    {
      eo2_do(static_cast<D*>(this)->_eo_ptr(), ::isimple_inc());
    }

    operator eo3_interface() const
    {
      return eo3_interface
	(eo_ref(static_cast<D const*>(this)->_eo_ptr()));
    }
    int isimple_get()
    {
      int r = 0;
      eo2_do(static_cast<D*>(this)->_eo_ptr(), r = ::isimple_get());
      std::cout << "simple_get " << r << std::endl;
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
