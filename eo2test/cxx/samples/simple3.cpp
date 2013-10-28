#include <iostream>

#include "eo3_inherit.hpp"

#include "eo_init.hpp"
#include "inherit.hpp"

#include <assert.h>

struct inherit2 : efl::eo::inherit<inherit2, eo3_simple>
{
  int i;
  inherit2() : inherit_base
	       (
		efl::eo::args<eo3_simple>(66)
	       )
	     , i(0)
  {}
   
  void simple_inc()
  {
    std::cout << "inherit2::simple_inc" << std::endl;
    ++i;
  }
  int simple_get()
  {
    return i;
  }
};

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;

  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  inherit2 inherit;
  // codigo em C
  eo2_do(inherit._eo_ptr(), ::simple_inc());
  int i = 0;
  eo2_do(inherit._eo_ptr(), i = ::simple_get());
  assert(i == 1);
}

