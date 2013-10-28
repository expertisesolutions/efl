#include <iostream>

#include "eo_init.hpp"

extern "C" {
#include "eo3_container.h"
#include "eo3_simple.h"
}

#include <assert.h>

#include "eo3_container.hpp"

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;
  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  Eo* simple = eo2_add(EO3_GET_CLASS(EO3_SIMPLE_CLASS), NULL);
  Eo* c = eo2_add(EO3_GET_CLASS(EO3_CONTAINER_CLASS), NULL);
  eo2_do(c, eo3_container_add(c); eo3_container_add2(simple));

  eo3_container container1;
  container1.eo3_container_add(eo3_container(c));
  container1.eo3_container_add2(eo3_simple(simple));
}

