
#include "Eina.hh"

#include <cassert>
#include <algorithm>
#include <functional>

#include <check.h>

START_TEST(eina_cxx_inlist_push_back)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  list.push_back(5);
  list.push_back(10);
  list.push_back(15);

  int result[] = {5, 10, 15};

  assert(list.size() == 3);
  assert(std::equal(list.begin(), list.end(), result));
}
END_TEST

START_TEST(eina_cxx_inlist_pop_back)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.pop_back();

  int result[] = {5, 10};

  assert(list.size() == 2);
  assert(std::equal(list.begin(), list.end(), result));
}
END_TEST

START_TEST(eina_cxx_inlist_push_front)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  list.push_front(5);
  list.push_front(10);
  list.push_front(15);

  int result[] = {15, 10, 5};

  assert(list.size() == 3);
  assert(std::equal(list.begin(), list.end(), result));
}
END_TEST

START_TEST(eina_cxx_inlist_pop_front)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  list.push_front(5);
  list.push_front(10);
  list.push_front(15);
  list.pop_front();

  int result[] = {10, 5};

  assert(list.size() == 2);
  assert(std::equal(list.begin(), list.end(), result));
}
END_TEST

START_TEST(eina_cxx_inlist_insert)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list;

  efl::eina::inlist<int>::iterator it;

  it = list.insert(list.end(), 5); // first element
  assert(it != list.end());
  ++it;
  assert(it == list.end());

  it = list.insert(list.end(), 10);  // equivalent to push_back
  assert(it != list.end());
  ++it;
  assert(it == list.end());

  it = list.insert(list.begin(), 15); // equivalent to push_front
  assert(it == list.begin());

  it = list.end();
  --it;
  list.insert(it, 20); // insert before the last element

  int result[] = {15, 5, 20, 10};

  assert(list.size() == 4);
  assert(std::equal(list.begin(), list.end(), result));

  efl::eina::inlist<int> list2;
  it = list2.insert(list2.end(), list.begin(), list.end());
  assert(it == list2.begin());
  assert(list == list2);

  efl::eina::inlist<int> list3;
  list3.push_back(1);
  it = list3.insert(list3.end(), list.begin(), list.end());
  assert(list3.size() == 5);
  assert(list3.front() == 1);
  it = list3.begin();
  ++it;
  assert(std::equal(it, list3.end(), list.begin()));

  efl::eina::inlist<int> list4;
  list4.push_back(1);
  it = list4.insert(list4.begin(), list.begin(), list.end());
  assert(list4.size() == 5);
  assert(list4.back() == 1);
  assert(std::equal(list.begin(), list.end(), list4.begin()));
}
END_TEST

START_TEST(eina_cxx_inlist_constructors)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list1;
  assert(list1.empty());

  efl::eina::inlist<int> list2(10, 5);
  assert(list2.size() == 10);
  assert(std::find_if(list2.begin(), list2.end()
                      , std::not1(std::bind1st(std::equal_to<int>(), 5))) == list2.end());

  efl::eina::inlist<int> list3(list2);
  assert(list2 == list3);

  efl::eina::inlist<int> list4(list2.begin(), list2.end());
  assert(list2 == list4);
}
END_TEST

START_TEST(eina_cxx_inlist_erase)
{
  efl::eina::eina_init eina_init;

  efl::eina::inlist<int> list1;
  list1.push_back(5);
  list1.push_back(10);
  list1.push_back(15);
  list1.push_back(20);
  list1.push_back(25);
  list1.push_back(30);
  
  efl::eina::inlist<int>::iterator it = list1.begin(), it2;

  it = list1.erase(it);
  assert(it == list1.begin());
  assert(list1.size() == 5);
  assert(list1.front() == 10);

  it = list1.begin();
  it2 = list1.begin();
  ++it;
  ++it2; ++it2;
  assert(*it2 == 20);
  it = list1.erase(it);
  assert(it == it2);
  assert(list1.size() == 4);
  assert(*it2 == 20);
  
  it = list1.end();
  --it;
  it = list1.erase(it);
  assert(it == list1.end());
  assert(list1.size() == 3);
  assert(list1.back() == 25);

  it = list1.begin();
  ++it;
  it2 = list1.end();
  --it2;
  it = list1.erase(it, it2);
  it2 = list1.end();
  --it2;
  assert(it == it2);
  assert(list1.size() == 2);
  assert(list1.front() == 10);
  assert(list1.back() == 25);
}
END_TEST

void
eina_test_inlist(TCase *tc)
{
  tcase_add_test(tc, eina_cxx_inlist_push_back);
  tcase_add_test(tc, eina_cxx_inlist_push_front);
  tcase_add_test(tc, eina_cxx_inlist_pop_back);
  tcase_add_test(tc, eina_cxx_inlist_pop_front);
  tcase_add_test(tc, eina_cxx_inlist_insert);
  tcase_add_test(tc, eina_cxx_inlist_erase);
  tcase_add_test(tc, eina_cxx_inlist_constructors);
}
