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

EO3_DEFINE_CLASS(EO3_MIXIN_INHERIT_CLASS, ((EO3_SIMPLE_CLASS), (EO3_MIXIN_MIXIN)), NULL)
