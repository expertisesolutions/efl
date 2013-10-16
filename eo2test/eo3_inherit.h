#ifndef _EO3_INHERIT_H
#define _EO3_INHERIT_H

#include "Eo.h"

#define EO3_INHERIT_CLASS eo3_inherit_class                             \
    , EO3_CONSTRUCTOR_OVERRIDE(eo2_constructor, _constructor)           \
    , EO3_DESTRUCTOR(_destructor)                                       \
    , EO3_FUNCTION_OVERRIDE(simple_inc, _inc, void, int)                \
    , EO3_FUNCTION(eo3_inherit_get, _get, int)                          \
    , EO3_FUNCTION_OVERRIDE(simple_virtual, _virtual, int, int)         \

EO3_DECLARE_CLASS(EO3_INHERIT_CLASS)

#endif
