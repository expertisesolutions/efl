#ifndef EO3_INHERIT_INTERFACE_H
#define EO3_INHERIT_INTERFACE_H

#include "Eo.h"

#define EO3_INHERIT_INTERFACE_CLASS eo3_inherit_interface_class         \
    , EO3_CONSTRUCTOR_OVERRIDE(eo2_constructor, _constructor)           \
    , EO3_DESTRUCTOR(_destructor)                                       \
    , EO3_FUNCTION_OVERRIDE(isimple_inc, _inc, void)                    \
    , EO3_FUNCTION_OVERRIDE(isimple_get, _get, int)                     \
    , EO3_FUNCTION_OVERRIDE(isimple_set, _set, void, int)

EO3_DECLARE_CLASS(EO3_INHERIT_INTERFACE_CLASS)

#endif
