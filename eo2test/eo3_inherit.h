#ifndef _EO3_INHERIT_H
#define _EO3_INHERIT_H

#include "Eo.h"

#define EO3_INHERIT_CLASS eo3_inherit_class                             \
    , constructor_override(eo2_constructor, _constructor)               \
    , destructor(_destructor)                                           \
    , function_override(simple_inc, _inc)				\
    , function(eo3_inherit_get, _get, int)                              \
    , function_override(simple_virtual, _virtual)

EO3_DECLARE_CLASS(EO3_INHERIT_CLASS)

#endif
