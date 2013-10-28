#include <iostream>
#include <stdio.h>

#include "eo_init.hpp"

extern "C" {
#include "eo3_mixin_mixin.h"
#include "eo3_mixin_inherit.h"
#include "eo3_simple.h"
}

#include <assert.h>

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;
  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  Eo *obj = eo2_add(EO3_GET_CLASS(EO3_MIXIN_INHERIT_CLASS), NULL);
  eo2_do(obj, simple_set(11));

  int sum = 0;
  eo2_do(obj, mixin_ab_sum_get(&sum));
  assert(sum == 22);

  return 0;
}

