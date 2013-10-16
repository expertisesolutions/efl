#ifndef EO3_INTERFACE_H
#define EO3_INTERFACE_H

#include "Eo.h"

#define EO3_SIMPLE_INTERFACE eo3_simple_interface                     \
    , EO3_FUNCTION(isimple_inc, void)                                 \
    , EO3_FUNCTION(isimple_get, int)                                  \
    , EO3_FUNCTION(isimple_set, void, int)

EO3_DECLARE_INTERFACE(EO3_SIMPLE_INTERFACE)

#endif
