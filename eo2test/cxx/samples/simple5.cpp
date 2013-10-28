#include <iostream>

#include "eo_init.hpp"
#include "eo3_inherit_interface.hpp"

#include <assert.h>

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;

  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  Eo* eo = eo2_add(EO3_GET_CLASS(EO3_INHERIT_INTERFACE_CLASS), NULL);
  eo3_inherit_interface inherit(eo);

  inherit.isimple_inc();
  assert(inherit.isimple_get() == 69);
}

