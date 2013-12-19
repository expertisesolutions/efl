#include "Eo.h"

#include "eo3_no_instant.h"

#include <assert.h>
#include <stdio.h>

typedef struct 
{
    unsigned int curr_inc_id
} Private_Data;

static unsigned int
_get_inc_id(Eo *obj EINA_UNUSED, Private_Data *self)
{
    unsigned int ret = self->curr_inc_id++;
    printf("eo3_no_instant.c: %s [%s]\n",
        __func__, eo_class_name_get(EO3_GET_CLASS(EO3_NO_INSTANT)));

    return ret;
}

static void
_class_contructor(Eo *obj EINA_UNUSED, Private_Data *self)
{
    self->curr_inc_id = 0;
}

EO3_DEFINE_NO_INSTANT(EO3_NO_INSTANT, ((EO3_NO_BASE_CLASS)), Private_Data)

