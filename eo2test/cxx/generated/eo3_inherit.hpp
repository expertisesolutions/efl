
#ifndef GENCXX_EO3_INHERIT_HPP
#define GENCXX_EO3_INHERIT_HPP

/*
 * #define EO3_INHERIT_CLASS eo3_inherit_class                          \
 *     , EO3_CONSTRUCTOR_OVERRIDE(eo2_constructor, _constructor)        \
 *     , EO3_DESTRUCTOR(_destructor)                                    \
 *     , EO3_FUNCTION_OVERRIDE(simple_inc, _inc, void, int)             \
 *     , EO3_FUNCTION(eo3_inherit_get, _get, int)                       \
 *     , EO3_FUNCTION_OVERRIDE(simple_virtual, _virtual, int, int)      \
 *
 * EO3_DECLARE_CLASS(EO3_INHERIT_CLASS)
 */

extern "C" {
#include "eo3_inherit.h"
}

#include "eo3_simple.hpp"

struct eo3_inherit : eo3_simple
{
  explicit eo3_inherit(Eo* eo)
    : eo3_simple(eo) {}

  eo3_inherit()
    : eo3_simple(_c1())
  {
  }

  static Eo_Class const* _eo_class()
  {
    return EO3_GET_CLASS(EO3_INHERIT_CLASS);
  }

  int eo3_inherit_get()
  {
    int r = 0;
    eo2_do(_eo_ptr(), r = ::eo3_inherit_get());
    return r;
  }
private:
  static Eo* _c1()
  {
    return eo2_add(EO3_GET_CLASS(EO3_INHERIT_CLASS), NULL);
  }
};

#endif // GENCXX_EO3_INHERIT_HPP

