#ifndef _EO3_SIMPLE_H
#define _EO3_SIMPLE_H

#include "Eo.h"

#define EO3_SIMPLE_CLASS eo3_simple                              \
    , constructor(simple_constructor, _constructor, int)         \
    , destructor(_destructor)                                    \
    , function(simple_inc, _inc, void)                           \
    , function(simple_get, _get, int)                            \
    , function(simple_set, _set, void, int)                      \
    , function(simple_virtual, _set, void, int)                  \
    , class_function(simple_class_foo, _class_hello, void, int)  \
    , event(simple_set, int)


EO3_DECLARE_CLASS(EO3_SIMPLE_CLASS)

#endif
