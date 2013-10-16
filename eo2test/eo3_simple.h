#ifndef _EO3_SIMPLE_H
#define _EO3_SIMPLE_H

#include "Eo.h"

#define EO3_SIMPLE_CLASS eo3_simple                                  \
    , EO3_CONSTRUCTOR(simple_constructor, _constructor, int)         \
    , EO3_DESTRUCTOR(_destructor)                                    \
    , EO3_FUNCTION(simple_inc, _inc, void)                           \
    , EO3_FUNCTION(simple_get, _get, int)                            \
    , EO3_FUNCTION(simple_set, _set, void, int)                      \
    , EO3_FUNCTION(simple_virtual, _set, void, int)                  \
    , EO3_EVENT(simple_set_event, int)

EO3_DECLARE_CLASS(EO3_SIMPLE_CLASS)

#endif
