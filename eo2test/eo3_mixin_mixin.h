#ifndef EO3_MIXIN_MIXIN_H
#define EO3_MIXIN_MIXIN_H

#include "Eo.h"

#define EO3_MIXIN_MIXIN eo3_mixin_mixin                        \
    , function(mixin_ab_sum_get, _ab_sum_get, void, int*)

EO3_DECLARE_CLASS(EO3_MIXIN_MIXIN); // XXX: probably EO3_DECLARE_MIXIN?

#endif // EO3_MIXIN_MIXIN_H
