
#include "Eina.hh"

#include <cassert>
#include <algorithm>

#include <check.h>

START_TEST(eina_cxx_iterator)
{
  efl::eina::eina_init eina_init;

  efl::eina::ptr_list<int> list;
  assert(list.size() == 0);
  assert(list.empty());

  list.push_back(new int(5));
  
  efl::eina::iterator<int> iterator
    (eina_list_iterator_new(list.native_handle())), last;

  // assert(std::distance(iterator, last) == 1);
  // iterator = efl::eina::iterator<bigger_than_pointer>
  //   (eina_list_iterator_new(list.native_handle()));
  // bigger_than_pointer r = *iterator;
  // assert(r.a == 5 && r.b == 10);
}
END_TEST

void
eina_test_iterator(TCase *tc)
{
}
