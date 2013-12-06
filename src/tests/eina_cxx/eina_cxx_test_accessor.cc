
#include "Eina.hh"

#include <cassert>
#include <algorithm>

#include <check.h>

START_TEST(eina_cxx_accessor_indexing)
{
  efl::eina::ptr_list<int> list;
  list.push_back(new int(5));
  list.push_back(new int(10));
  list.push_back(new int(15));
  list.push_back(new int(20));

  efl::eina::accessor<int> accessor(list.accessor());

  assert(accessor[0] == 5);
  assert(accessor[1] == 10);
  assert(accessor[2] == 15);
  assert(accessor[3] == 20);
}
END_TEST

START_TEST(eina_cxx_accessor_iterator)
{
  efl::eina::ptr_list<int> list;
  list.push_back(new int(5));
  list.push_back(new int(10));
  list.push_back(new int(15));
  list.push_back(new int(20));

  std::size_t pos = 0u;
  efl::eina::accessor<int> accessor = list.accessor();
  for(efl::eina::accessor_iterator<int> first (accessor)
        , last (accessor, list.size()); first != last; ++first, ++pos)
    {
      assert(pos != 0u || *first == 5);
      assert(pos != 1u || *first == 10);
      assert(pos != 2u || *first == 15);
      assert(pos != 3u || *first == 20);
    }
}
END_TEST

void
eina_test_accessor(TCase* tc)
{
  tcase_add_test(tc, eina_cxx_accessor_indexing);
  tcase_add_test(tc, eina_cxx_accessor_iterator);
}
