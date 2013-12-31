#include "Eo.h"
#include "eo3_simple.h"
#include "eo3_inherit.h"

#include <assert.h>

typedef struct
{
   int y;
} Private_Data;

static void
_inc(Eo *obj, Private_Data *data)
{
  puts("eo3_inherit::inc\n");
  eo2_do_super(obj, EO3_GET_CLASS(EO3_INHERIT_CLASS), simple_inc());
  data->y += 1;
}

static int
_get(Eo *obj EINA_UNUSED, Private_Data *data)
{
  puts("eo3_inherit::get\n");
  return data->y;
}

static void
_class_hello(const Eo_Class *klass, int a)
{
   printf("Hello %d - body %s - EAPI %s\n", a, eo_class_name_get(klass)
          , eo_class_name_get(EO3_GET_CLASS(EO3_SIMPLE_CLASS)));
   eo2_do_super(klass, EO3_GET_CLASS(EO3_INHERIT_CLASS), simple_class_foo(a * 2));
}

static int
_virtual(Eo *obj EINA_UNUSED, void *obj_data EINA_UNUSED, int in)
{
   return (in * 2);
}

static void
_constructor(Eo *obj, Private_Data *data)
{
  printf("eo3_inherit.c:_constructor\n");
  eo2_do_super(obj, EO3_GET_CLASS(EO3_INHERIT_CLASS), simple_constructor(66));
  data->y = 68;
}

static void
_destructor(Eo *obj, void *obj_data EINA_UNUSED)
{
  puts("eo3_inherit.c:_destructor\n");
  eo2_do_super(obj, EO3_GET_CLASS(EO3_INHERIT_CLASS), eo2_destructor());
}

EO3_DEFINE_CLASS(EO3_INHERIT_CLASS, ((EO3_SIMPLE_CLASS)), Private_Data)
