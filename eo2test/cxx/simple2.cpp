
#include "eo_init.hpp"
#include "eo3_inherit.hpp"

#include <assert.h>

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;

  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  eo3_inherit inherit;
  inherit.simple_inc();
  assert(inherit.simple_get() == 67);
  assert(inherit.eo3_inherit_get() == 69);
}

