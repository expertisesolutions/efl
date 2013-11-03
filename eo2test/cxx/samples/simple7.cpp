#include <iostream>
#include <stdlib.h>

#include <boost/optional.hpp>

#include "eo_init.hpp"
#include "eo_base.hpp"

#include "eo3_simple.hpp"

#include <assert.h>
#include <memory>

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;
  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  eo3_simple eo1(111);
  eo3_simple eo2(222);

  eo1.parent_set(eo2);

  eo3_simple eo3 = efl::eo::downcast<eo3_simple>(*eo1.parent_get());

  return 0;
}


