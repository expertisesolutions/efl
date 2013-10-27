#include "Eo.h"
#include "eo3_simple.h"
#include "eo3_container.h"

#include <assert.h>

typedef struct
{
} Private_Data;

static void
_add(Eo *obj EINA_UNUSED, void *obj_data EINA_UNUSED, Eo *x)
{
  printf("eo3_container : _add : x = %p \t[EO3_CONTAINER_CLASS]\n", x);
}

static void
_add2(Eo *obj EINA_UNUSED, void *obj_data EINA_UNUSED, Eo *x)
{
  printf("eo3_container : _add2 : x = %p \t[EO3_SIMPLE_CLASS]\n", x);
}

static void
_constructor(Eo *obj, Private_Data *data)
{
  printf("eo3_inherit.c:_constructor\n");
  eo2_do_super(obj, EO3_GET_CLASS(EO3_CONTAINER_CLASS), eo2_constructor());
}

static void
_destructor(Eo *obj, void *obj_data EINA_UNUSED)
{
  puts("eo3_inherit.c:_destructor\n");
  eo2_do_super(obj, EO3_GET_CLASS(EO3_CONTAINER_CLASS), eo2_destructor());
}

EO3_DEFINE_CLASS(EO3_CONTAINER_CLASS, ((EO3_BASE_CLASS)), Private_Data)
