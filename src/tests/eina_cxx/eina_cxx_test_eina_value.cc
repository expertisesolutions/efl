
#include "Eina.hh"

#include <cassert>

#include <check.h>

START_TEST(eina_cxx_eina_value_constructors)
{
  efl::eina::eina_init init;

  efl::eina::eina_value v;

  char c = 'c';
  efl::eina::eina_value vchar(c);

  short s = 5;
  efl::eina::eina_value vshort(s);

  efl::eina::eina_value vint(5);

  efl::eina::eina_value vlong(5l);

  unsigned char uc = 'b';
  efl::eina::eina_value vuchar(uc);

  unsigned short us = 5;
  efl::eina::eina_value vushort(us);

  efl::eina::eina_value vuint(5u);

  efl::eina::eina_value vulong(5ul);

  efl::eina::eina_value vfloat(5.0f);

  efl::eina::eina_value vdouble(5.0);
}
END_TEST

START_TEST(eina_cxx_eina_value_get)
{
  efl::eina::eina_init init;

  char c = 'c';
  efl::eina::eina_value vchar(c);
  assert(efl::eina::get<char>(vchar) == 'c');

  short s = 5;
  efl::eina::eina_value vshort(s);
  assert(efl::eina::get<short>(vshort) == 5);

  efl::eina::eina_value vint(6);
  assert(efl::eina::get<int>(vint) == 6);

  efl::eina::eina_value vlong(7l);
  assert(efl::eina::get<long>(vlong) == 7l);

  unsigned char uc = 'b';
  efl::eina::eina_value vuchar(uc);
  assert(efl::eina::get<unsigned char>(vuchar) == 'b');

  unsigned short us = 8;
  efl::eina::eina_value vushort(us);
  assert(efl::eina::get<unsigned short>(vushort) == 8);

  efl::eina::eina_value vuint(9u);
  assert(efl::eina::get<unsigned int>(vuint) == 9u);

  efl::eina::eina_value vulong(10ul);
  assert(efl::eina::get<unsigned long>(vulong) == 10ul);

  efl::eina::eina_value vfloat(11.0f);
  assert(efl::eina::get<float>(vfloat) == 11.0f);

  efl::eina::eina_value vdouble(12.0);
  assert(efl::eina::get<double>(vdouble) == 12.0f);
}
END_TEST

START_TEST(eina_cxx_eina_value_wrong_get)
{
  efl::eina::eina_init init;

  char c = 'c';
  efl::eina::eina_value vchar(c);
  try
  {
    efl::eina::get<int>(vchar);
    std::abort();
  }
  catch(efl::eina::system_error const&)
  {
  }
}
END_TEST

START_TEST(eina_cxx_eina_value_comparison_operators)
{
  efl::eina::eina_init init;

  efl::eina::eina_value v;

  char c = 5;
  efl::eina::eina_value vchar(c);

  short s = 5;
  efl::eina::eina_value vshort(s);

  efl::eina::eina_value vint(5);

  efl::eina::eina_value vlong(5l);

  unsigned char uc = 5;
  efl::eina::eina_value vuchar(uc);

  unsigned short us = 5;
  efl::eina::eina_value vushort(us);

  efl::eina::eina_value vuint(5u);

  efl::eina::eina_value vulong(5ul);

  efl::eina::eina_value vfloat(5.0f);

  efl::eina::eina_value vdouble(5.0);

  assert(vchar == vchar);
  assert(vshort == vshort);
  assert(vint == vint);
  assert(vlong == vlong);
  assert(vuchar == vuchar);
  assert(vushort == vushort);
  assert(vuint == vuint);
  assert(vulong == vulong);
  assert(vfloat == vfloat);
  assert(vdouble == vdouble);

  assert(vchar != vshort);
  assert(vshort != vint);
  assert(vint != vlong);
  assert(vlong != vuchar);
  assert(vuchar != vushort);
  assert(vushort != vuint);
  assert(vuint != vulong);
  assert(vulong != vfloat);
  assert(vfloat != vdouble);
  assert(vdouble != vchar);

  assert(vchar != vuchar);
  assert(vshort != vushort);
  assert(vint != vuint);
  assert(vlong != vulong);
  assert(vfloat != vdouble);
  assert(vdouble != vfloat);
}
END_TEST

START_TEST(eina_cxx_eina_value_copying)
{
  char c = 5;

  efl::eina::eina_value vchar(c);
  efl::eina::eina_value vchar2(vchar);
  assert(vchar == vchar2);
  assert(efl::eina::get<char>(vchar) == 5);
  assert(efl::eina::get<char>(vchar2) == 5);

  efl::eina::eina_value vint(10);
  vchar = vint;
  assert(vchar != vchar2);
  assert(vint == vchar);
  assert(efl::eina::get<int>(vchar) == 10);
  assert(efl::eina::get<int>(vint) == 10);
}
END_TEST

void
eina_test_eina_value(TCase* tc)
{
  tcase_add_test(tc, eina_cxx_eina_value_constructors);
  tcase_add_test(tc, eina_cxx_eina_value_get);
  tcase_add_test(tc, eina_cxx_eina_value_wrong_get);
  tcase_add_test(tc, eina_cxx_eina_value_comparison_operators);
  tcase_add_test(tc, eina_cxx_eina_value_copying);
}
