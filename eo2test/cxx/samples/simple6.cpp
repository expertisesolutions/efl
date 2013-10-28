#include <iostream>
#include <stdlib.h>

#include <boost/optional.hpp>

#include "eo_init.hpp"
#include "eo_wref.hpp"

#include "eo3_simple.hpp"

#include <assert.h>
#include <memory>

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;
  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);
 
  std::auto_ptr<eo3_simple> eo1(new eo3_simple(100));

  efl::eo::wref<eo3_simple> w1(*eo1);
  efl::eo::wref<eo3_simple> w2(w1);

  assert(eo1->ref_get() == 1);
  {
    eo3_simple obj2(*w1.lock());
    assert(eo1->ref_get() == 2);
  }
  assert(eo1->ref_get() == 1);

  eo1.reset();
  assert(!w1.lock());
  assert(!w2.lock());

  return 0;
}

