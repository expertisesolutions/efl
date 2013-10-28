#ifndef _EO3_CONTAINER_H
#define _EO3_CONTAINER_H

#include "Eo.h"
#include "eo3_simple.h"

#define EO3_CONTAINER_CLASS eo3_container_class                              \
    , constructor_override(eo2_constructor, _constructor)                    \
    , destructor(_destructor)                                                \
    , function(eo3_container_add, _add, void, EO_TYPE(EO3_CONTAINER_CLASS) ) \
    , function(eo3_container_add2, _add2, void, EO_TYPE(EO3_SIMPLE_CLASS) )

EO3_DECLARE_CLASS(EO3_CONTAINER_CLASS)

#endif
