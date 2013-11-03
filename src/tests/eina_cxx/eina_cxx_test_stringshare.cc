
#include "Eina.hh"

#include <cassert>
#include <algorithm>
#include <functional>

#include <check.h>

START_TEST(eina_cxx_stringshare_constructors)
{
  efl::eina::eina_init eina_init;

  efl::eina::stringshare string1;
  assert(string1.empty());

  efl::eina::stringshare string2("string");
  assert(string2.size() == 6);
  assert(string2 == "string");

  efl::eina::stringshare string3(string2);
  assert(string2 == string3);

  efl::eina::stringshare string4(string3.begin(), string3.end());
  assert(string2 == string3);
}
END_TEST

void
eina_test_stringshare(TCase *tc)
{
  tcase_add_test(tc, eina_cxx_stringshare_constructors);
}
