#include <iostream>

#include "eo3_simple.hpp"

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

// void bar(eo3_interface s)
// {
  
// }

void f(eo3_simple s)
{
  s.simple_inc();
  assert(s.simple_get() == 1);
}

// void foo(WeakRef<eo3_simple> s)
// {
//   f(s.lock());
// }

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;

  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  inherit2 inherit;
  ::f(inherit);
}
