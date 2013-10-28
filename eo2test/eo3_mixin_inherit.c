#include "Eo.h"

#include "eo3_simple.h"
#include "eo3_mixin_mixin.h"
#include "eo3_mixin_inherit.h"

#include <assert.h>
#include <stdio.h>

static void
_print(Eo *objid EINA_UNUSED, void* self)
{
  puts("eo3_mixin_inherit.c _print\n");
}

static void
_constructor(Eo *obj, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
  printf("eo3_mixin_inherit.c:_constructor\n");
  eo2_do_super(obj, EO3_GET_CLASS(EO3_MIXIN_INHERIT_CLASS), eo2_constructor());
}

static void
_destructor(Eo *obj, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
  printf("eo3_mixin_inherit.c:_destructor\n");
  eo2_do_super(obj, EO3_GET_CLASS(EO3_MIXIN_INHERIT_CLASS), eo2_destructor());
}

EO3_DEFINE_CLASS(EO3_MIXIN_INHERIT_CLASS, ((EO3_SIMPLE_CLASS), (EO3_MIXIN_MIXIN)), NULL)
