// #define EO3_INHERIT_INTERFACE_CLASS eo3_inherit_interface_class         \
//     , EO3_CONSTRUCTOR_OVERRIDE(eo2_constructor, _constructor)           \
//     , EO3_DESTRUCTOR(_destructor)                                       \
//     , EO3_FUNCTION_OVERRIDE(isimple_inc, _inc, void)                    \
//     , EO3_FUNCTION_OVERRIDE(isimple_get, _get, int)                     \
//     , EO3_FUNCTION_OVERRIDE(isimple_set, _set, void, int)

// EO3_DECLARE_CLASS(EO3_INHERIT_INTERFACE_CLASS)

extern "C" {
#include "eo3_inherit_interface.h"
}
#include "eo3_interface.hpp"

struct eo3_inherit_interface 
  : Base, efl::eo::detail::extension_inheritance<eo3_interface>::type<eo3_inherit_interface>
{
  explicit eo3_inherit_interface(Eo* eo)
    : Base(eo)
  {}
};

