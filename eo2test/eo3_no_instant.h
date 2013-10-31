#ifndef _EO3_NO_INSTANT_H
#define _EO3_NO_INSTANT_H

#include "Eo.h"

#define EO3_NO_INSTANT eo3_no_instant                         \
    , function(get_inc_id, _get_inc_id, unsigned int)         \
    , class_constructor(_class_contructor)                    \


EO3_DECLARE_CLASS(EO3_NO_INSTANT)

#endif
