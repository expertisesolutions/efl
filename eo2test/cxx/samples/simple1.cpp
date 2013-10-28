#include <iostream>

#include "eo_init.hpp"
#include "eo3_simple.hpp"

#include <assert.h>

Eina_Bool callback(void*, int x)
{
  std::cout << "callback  "<< x << std::endl;
  return EINA_TRUE;
}

Eina_Bool callback1(void*, int x)
{
  std::cout << "callback1 " << x << std::endl;
  return EINA_TRUE;
}

int main(int argc, char** argv)
{
  efl::eo::eo_init eo_init;

  if (argc > 1)
    eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

  eo3_simple simple(66);
  simple.simple_inc();
  assert(simple.simple_get() == 67);
  assert(sizeof(simple) == sizeof(Eo*));

  eo2_do(simple._eo_ptr(), ::simple_set_callback_add(0, callback));
  eo2_do(simple._eo_ptr(), ::simple_set_callback_add(0, callback1));
  simple.simple_set(68);

  eo2_do(simple._eo_ptr(), ::simple_class_foo(10));
}

