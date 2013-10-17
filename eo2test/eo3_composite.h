#ifndef _EO3_COMPOSITE_H
#define _EO3_COMPOSITE_H

#include "Eo.h"

#define EO3_COMPOSITE_CLASS eo3_composite_class				\
    , EO3_CONSTRUCTOR_OVERRIDE(eo2_constructor, _constructor, void, int)   \
    , EO3_FUNCTION_OVERRIDE(simple_get, _get, int)         		\

EO3_DECLARE_CLASS(EO3_COMPOSITE_CLASS)

#endif
