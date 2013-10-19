
#include "eo_init.hpp"
#include "eo3_simple.hpp"

#include <assert.h>

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;

  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  eo3_simple simple(66);
  simple.simple_inc();
  assert(simple.simple_get() == 67);
}

