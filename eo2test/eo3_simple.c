#include "Eo.h"
#include "eo3_simple.h"
#include <assert.h>

#include <stdio.h>

typedef struct
{
   int x;
} Private_Data;

static void _inc(Eo *objid EINA_UNUSED, Private_Data* self)
{
  printf("eo3_simple::inc %d->%d\n", self->x, self->x+1);
  self->x += 1;
}
static int _get(Eo *objid EINA_UNUSED, Private_Data* self)
{
  printf("eo3_simple::get %d\n", self->x);
  return self->x;
}
static void _set(Eo *objid, Private_Data* self, int x)
{
  eo2_do(objid, simple_set_callback_call(x));
  printf("eo3_simple::set %d\n", x);
  self->x = x;
}
static void _class_hello(const Eo_Class *klass, int a)
{
  (void)klass;
  (void)a;
  printf("Hello %d - body %s - EAPI %s\n", a, eo_class_name_get(klass)
    , eo_class_name_get(EO3_GET_CLASS(EO3_SIMPLE_CLASS))); 
}
static void _constructor(Eo *obj, Private_Data* self, int x)
{
  printf("eo3_simple _constructor %d\n", x);
  eo2_do_super(obj, EO3_GET_CLASS(EO3_SIMPLE_CLASS), eo2_constructor());
  self->x = x;
  /* printf("called super\n"); */
}
static void _destructor(Eo *obj, Private_Data* self EINA_UNUSED)
{
  puts("eo3_simple.c:_destructor\n");
  eo2_do_super(obj, EO3_GET_CLASS(EO3_SIMPLE_CLASS), eo2_destructor());
}

EO3_DEFINE_CLASS(EO3_SIMPLE_CLASS, ((EO3_BASE_CLASS)), Private_Data)

