
#ifndef GENCXX_EO3_MIXIN_MIXIN_HPP
#define GENCXX_EO3_MIXIN_MIXIN_HPP

#include <iostream>

#include "eo_base.hpp"
#include "eo_private.hpp"

/*
 * #define EO3_MIXIN_MIXIN eo3_mixin_mixin                       \
 *   , function(mixin_ab_sum_get, _ab_sum_get, void, int*)
 *
 * EO3_DECLARE_MIXIN(EO3_MIXIN_MIXIN);
 *
 */

extern "C" {
#include "eo3_mixin_mixin.h"
}
// <end>

struct eo3_mixin_mixin : efl::eo::base
{
  explicit eo3_mixin_mixin(Eo* eo) : efl::eo::base(eo) {}

  static Eo_Class const* _eo_class()
  {
    return EO3_GET_CLASS(EO3_MIXIN_MIXIN);
  }

  void mixin_ab_sum_get(int *sum)
  {
    eo2_do(_eo_ptr(), ::mixin_ab_sum_get(sum));
  }
};

namespace efl { namespace eo { namespace detail {

template <>
struct extension_inheritance< ::eo3_mixin_mixin>
{
  template <typename D>
  struct type
  {
    operator eo3_mixin_mixin() const
    {
      return eo3_mixin_mixin
	(eo_ref(static_cast<D const*>(this)->_eo_ptr()));
    }

    void mixin_ab_sum_get(int *sum)
    {
      eo2_do(static_cast<D*>(this)->_eo_ptr(), ::mixin_ab_sum_get(sum));
    }
  };
};

} } }


// <end>

#endif // GENCXX_EO3_MIXIN_MIXIN_HPP
