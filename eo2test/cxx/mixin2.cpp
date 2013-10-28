#include <iostream>
#include <stdio.h>

#include "eo_init.hpp"

extern "C" {
#include "eo3_mixin_mixin.h"
#include "eo3_mixin_inherit.h"
#include "eo3_simple.h"
}

#include "eo3_mixin_mixin.hpp"
#include "eo3_mixin_inherit.hpp"
#include <assert.h>

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;
  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  Eo *obj = eo2_add(EO3_GET_CLASS(EO3_MIXIN_INHERIT_CLASS), NULL);
  eo2_do(obj, simple_set(11));

  eo3_mixin_mixin m1(obj);
  int sum = 0;
  m1.mixin_ab_sum_get(&sum);
  assert(sum == 22);

  eo3_mixin_inherit m2;
  m2.simple_set(110);
  m2.mixin_ab_sum_get(&sum);
  assert(sum == 220);

  m2.mixin_inherit_print();
  return 0;
}

