
#include "eo_init.hpp"

#include <assert.h>

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;
  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  // TODO
}

