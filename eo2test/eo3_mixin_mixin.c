#include "Eo.h"

#include "eo3_mixin_mixin.h"
#include "eo3_simple.h"

#include <assert.h>
#include <stdio.h>

typedef struct 
{
} Private_Data;

static void
_ab_sum_get(Eo *obj, Private_Data *self EINA_UNUSED, int *sum)
{
   int a, b;
   assert(eo_isa(obj, EO3_GET_CLASS(EO3_SIMPLE_CLASS))); // XXX
   eo2_do(obj, a = simple_get(), b = simple_get());
   if (sum)
      *sum = a + b;
   printf("eo3_mixin_mixin.c: %s [%s] = %d\n"
     , __func__, eo_class_name_get(EO3_GET_CLASS(EO3_MIXIN_MIXIN)), *sum);
}

/* static void */
/* _constructor(Eo *obj, Private_Data* self EINA_UNUSED) // XXX not invoked! */
/* { */
/*   printf("eo3_mixin_mixin.c:_constructor\n"); */
/*   eo2_do_super(obj, EO3_GET_CLASS(EO3_MIXIN_MIXIN), eo2_constructor()); */
/* } */

/* static void */
/* _destructor(Eo *obj, Private_Data* self EINA_UNUSED) // XXX not invoked! */
/* { */
/*   printf("eo3_mixin_mixin.c:_destructor\n"); */
/*   eo2_do_super(obj, EO3_GET_CLASS(EO3_MIXIN_MIXIN), eo2_destructor()); */
/* } */

EO3_DEFINE_MIXIN(EO3_MIXIN_MIXIN, ((EO3_NO_BASE_CLASS)), Private_Data)

