
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
  for(efl::eina::accessor_iterator<int> first (list.accessor())
        , last (list.accessor(), list.size()); first != last; ++first, ++pos)
    {
      assert(pos != 0u || *first == 5);
      assert(pos != 1u || *first == 10);
      assert(pos != 2u || *first == 15);
      assert(pos != 3u || *first == 20);
    }
}
END_TEST

START_TEST(eina_cxx_accessor_relops)
{
  efl::eina::ptr_list<int> list;
  list.push_back(new int(5));
  list.push_back(new int(10));
  list.push_back(new int(15));
  list.push_back(new int(20));

  efl::eina::accessor_iterator<int> first (list.accessor())
    , second(list.accessor(), 1u)
    , third(list.accessor(), 2u)
    , fourth(list.accessor(), 3u)
    ;
  assert(!(first < first)); assert(first < second);
  assert(first < third); assert(first < fourth);
  assert(!(second < first)); assert(!(second < second));
  assert(second < third); assert(second < fourth);
  assert(!(third < first)); assert(!(third < second));
  assert(!(third < third)); assert(third < fourth);
  assert(!(fourth < first)); assert(!(fourth < second));
  assert(!(fourth < third)); assert(!(fourth < fourth));

  assert(first <= first); assert(first <= second);
  assert(first <= third); assert(first <= fourth);
  assert(!(second <= first)); assert(second <= second);
  assert(second <= third); assert(second <= fourth);
  assert(!(third <= first)); assert(!(third <= second));
  assert(third <= third); assert(third <= fourth);
  assert(!(fourth <= first)); assert(!(fourth <= second));
  assert(!(fourth <= third)); assert(fourth <= fourth);

  assert(!(first > first)); assert(!(first > second));
  assert(!(first > third)); assert(!(first > fourth));
  assert(second > first); assert(!(second > second));
  assert(!(second > third)); assert(!(second > fourth));
  assert(third > first); assert(third > second);
  assert(!(third > third)); assert(!(third > fourth));
  assert(fourth > first); assert(fourth > second);
  assert(fourth > third); assert(!(fourth > fourth));

  assert(first >= first); assert(!(first >= second));
  assert(!(first >= third)); assert(!(first >= fourth));
  assert(second >= first); assert(second >= second);
  assert(!(second >= third)); assert(!(second >= fourth));
  assert(third >= first); assert(third >= second);
  assert(third >= third); assert(!(third >= fourth));
  assert(fourth >= first); assert(fourth >= second);
  assert(fourth >= third); assert(fourth >= fourth);
}
END_TEST

void
eina_test_accessor(TCase* tc)
{
  tcase_add_test(tc, eina_cxx_accessor_indexing);
  tcase_add_test(tc, eina_cxx_accessor_iterator);
  tcase_add_test(tc, eina_cxx_accessor_relops);
}
