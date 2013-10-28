#ifndef EO3_INHERIT_INTERFACE_H
#define EO3_INHERIT_INTERFACE_H

#include "Eo.h"

#define EO3_INHERIT_INTERFACE_CLASS eo3_inherit_interface_class         \
    , constructor_override(eo2_constructor, _constructor)               \
    , destructor(_destructor)                                           \
    , function_override(isimple_inc, _inc)				\
    , function_override(isimple_get, _get)				\
    , function_override(isimple_set, _set)

EO3_DECLARE_CLASS(EO3_INHERIT_INTERFACE_CLASS)

#endif
