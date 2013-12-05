#ifndef EO3_INTERFACE_H
#define EO3_INTERFACE_H

#include "Eo.h"

#define EO3_SIMPLE_INTERFACE eo3_simple_interface                 \
    , function(isimple_inc, void)                                 \
    , function(isimple_get, int)                                  \
    , function(isimple_set, void, int)                            \
    , event(isimple_set, int)

EO3_DECLARE_INTERFACE(EO3_SIMPLE_INTERFACE)

#endif
