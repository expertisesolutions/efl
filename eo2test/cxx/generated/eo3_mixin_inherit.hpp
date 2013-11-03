
#ifndef GENCXX_EO3_MIXIN_INHERIT_HPP
#define GENCXX_EO3_MIXIN_INHERIT_HPP

#include <iostream>

#include "eo_base.hpp"
#include "eo_private.hpp"

/*
 * #define EO3_MIXIN_INHERIT_CLASS eo3_mixin_inherit     \
 *   , function(mixin_inherit_print, _print, void)
 *
 * EO3_DECLARE_CLASS(EO3_MIXIN_INHERIT_CLASS)
 */

extern "C" {
#include "eo3_mixin_inherit.h"
}

#include "eo3_mixin_mixin.hpp"
#include "eo3_simple.hpp"

// <end>

struct eo3_mixin_inherit 
    : eo3_simple
    , efl::eo::detail::extension_inheritance<eo3_mixin_mixin>::type<eo3_mixin_inherit>
{
  explicit eo3_mixin_inherit(Eo* eo) : eo3_simple(eo) {}

  eo3_mixin_inherit() : eo3_simple(_c()) {}

  eo3_mixin_inherit(eo3_mixin_inherit const& other)
    : eo3_simple(eo_ref(other._eo_ptr())) {}

  static Eo_Class const* _eo_class()
  {
    return EO3_GET_CLASS(EO3_MIXIN_INHERIT_CLASS);
  }

  void mixin_inherit_print()
  {
    eo2_do(_eo_ptr(), ::mixin_inherit_print());
  }

private:
  static Eo* _c()
  {
    return eo2_add(EO3_GET_CLASS(EO3_MIXIN_INHERIT_CLASS), NULL);
  }
};

namespace efl { namespace eo { namespace detail {

template <>
struct extension_inheritance< ::eo3_mixin_inherit>
{
  template <typename D>
  struct type
  {
    operator eo3_mixin_inherit() const
    {
      return eo3_mixin_inherit
	(eo_ref(static_cast<D const*>(this)->_eo_ptr()));
    }

    void mixin_inherit_print()
    {
      eo2_do(static_cast<D*>(this)->_eo_ptr(), ::mixin_inherit_print());
    }
  };
};

} } }


// <end>

#endif // GENCXX_EO3_MIXIN_INHERIT_HPP
