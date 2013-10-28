#ifndef _EO3_COMPOSITE_H
#define _EO3_COMPOSITE_H

#include "Eo.h"

#define EO3_COMPOSITE_CLASS eo3_composite_class			     \
  , constructor_override(eo2_constructor, _constructor, void, int)   \
  , function_override(simple_get, _get, int)

EO3_DECLARE_CLASS(EO3_COMPOSITE_CLASS)

#endif
