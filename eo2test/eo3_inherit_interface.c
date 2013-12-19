#include "Eo.h"
#include "eo3_interface.h"
#include "eo3_inherit_interface.h"

#include <assert.h>

typedef struct
{
   int y;
} Private_Data;

static void
_inc(Eo *obj, Private_Data *data)
{
  /* puts("eo3_inherit_interface::inc\n"); */
  data->y++;
}

static int
_get(Eo *obj EINA_UNUSED, Private_Data *data)
{
  /* puts("eo3_inherit_interface::get\n"); */
  return data->y;
}

static void
_set(Eo *obj EINA_UNUSED, Private_Data *data, int x)
{
  /* puts("eo3_inherit_interface::get\n"); */
  data->y = x;
  eo2_do(obj, isimple_set_callback_call(x));
}

static void
_constructor(Eo *obj, Private_Data *data)
{
   eo2_do_super(obj, EO3_GET_CLASS(EO3_INHERIT_INTERFACE_CLASS), eo2_constructor());

   data->y = 68;
}

static void
_destructor(Eo *obj, void *obj_data EINA_UNUSED)
{
  eo2_do_super(obj, EO3_GET_CLASS(EO3_INHERIT_INTERFACE_CLASS), eo2_destructor());
}

EO3_DEFINE_CLASS(EO3_INHERIT_INTERFACE_CLASS, ((EO3_BASE_CLASS),(EO3_SIMPLE_INTERFACE)), Private_Data)
