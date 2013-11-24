
#include "Eina.hh"

#include <cassert>
#include <algorithm>

#include <check.h>

START_TEST(eina_cxx_ptrarray_push_back)
{
  efl::eina::eina_init eina_init;

  efl::eina::ptr_array<int> array;

  array.push_back(new int(5));
  array.push_back(new int(10));
  array.push_back(new int(15));

  int result[] = {5, 10, 15};

  assert(array.size() == 3);
  assert(std::equal(array.begin(), array.end(), result));
}
END_TEST

START_TEST(eina_cxx_ptrarray_pop_back)
{
  efl::eina::eina_init eina_init;

  efl::eina::ptr_array<int> array;

  array.push_back(new int(5));
  array.push_back(new int(10));
  array.push_back(new int(15));
  array.pop_back();

  int result[] = {5, 10};

  assert(array.size() == 2);
  assert(std::equal(array.begin(), array.end(), result));
}
END_TEST

START_TEST(eina_cxx_ptrarray_insert)
{
  efl::eina::eina_init eina_init;

  efl::eina::ptr_array<int> array;
  assert(std::distance(array.begin(), array.end()) == 0u);

  efl::eina::ptr_array<int>::iterator it;

  it = array.insert(array.end(), new int(5)); // first element
  assert(it != array.end());
  ++it;
  assert(it == array.end());
  assert(array[0] == 5);
  assert(std::distance(array.begin(), array.end()) == 1u);

  it = array.insert(array.end(), new int(10));  // equivalent to push_back
  assert(it != array.end());
  ++it;
  assert(it == array.end());
  assert(array[0] == 5);
  assert(array[1] == 10);
  assert(std::distance(array.begin(), array.end()) == 2u);

  it = array.insert(array.begin(), new int(15)); // equivalent to push_front
  assert(it == array.begin());

  assert(array[1] == 5);
  assert(array[2] == 10);
  assert(array[0] == 15);
  assert(std::distance(array.begin(), array.end()) == 3u);

  array.insert(array.end() - 1, new int(20)); // insert before the last element
  assert(array[0] == 15);
  assert(array[1] == 5);
  assert(array[2] == 20);
  assert(array[3] == 10);
  assert(std::distance(array.begin(), array.end()) == 4u);

  int result[] = {15, 5, 20, 10};

  assert(array.size() == 4);
  assert(std::distance(array.begin(), array.end()) == 4u);
  assert(std::equal(array.begin(), array.end(), result));

  efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array2;
  it = array2.insert(array2.end(), array.begin(), array.end());
  assert(it == array2.begin());
  assert(array == array2);


  efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array3;
  array3.push_back(1);
  it = array3.insert(array3.end(), array.begin(), array.end());
  assert(array3.size() == 5);
  assert(array3.front() == 1);
  it = array3.begin();
  ++it;
  assert(std::equal(it, array3.end(), array.begin()));

  efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array4;
  array4.push_back(1);
  it = array4.insert(array4.begin(), array.begin(), array.end());
  assert(array4.size() == 5);
  assert(array4.back() == 1);
  assert(std::equal(array.begin(), array.end(), array4.begin()));
}
END_TEST

START_TEST(eina_cxx_ptrarray_constructors)
{
  efl::eina::eina_init eina_init;

  efl::eina::ptr_array<int> array1;
  assert(array1.empty());

  efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array2(10, 5);
  assert(array2.size() == 10);
  assert(std::find_if(array2.begin(), array2.end()
                      , std::not1(std::bind1st(std::equal_to<int>(), 5))) == array2.end());

  efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array3(array2);
  assert(array2 == array3);

  efl::eina::ptr_array<int, efl::eina::heap_copy_allocator> array4
    (array2.begin(), array2.end());
  assert(array2 == array4);
}
END_TEST

START_TEST(eina_cxx_ptrarray_erase)
{
  efl::eina::eina_init eina_init;

  efl::eina::ptr_array<int> array1;
  array1.push_back(new int(5));
  array1.push_back(new int(10));
  array1.push_back(new int(15));
  array1.push_back(new int(20));
  array1.push_back(new int(25));
  array1.push_back(new int(30));

  int result[] = {5, 10, 15, 20, 25, 30};
  assert(std::equal(array1.begin(), array1.end(), result));
  
  efl::eina::ptr_array<int>::iterator it = array1.erase(array1.begin());
  assert(it == array1.begin());
  assert(array1.size() == 5);
  assert(array1.front() == 10);

  assert(std::equal(array1.begin(), array1.end(), &result[1]));

  it = array1.erase(array1.begin() + 1);
  assert(*it == 20);
  assert(array1.size() == 4);
  
  it = array1.erase(array1.end() - 1);
  assert(it == array1.end());
  assert(array1.size() == 3);
  assert(array1.back() == 25);

  it = array1.erase(array1.begin() + 1, array1.end() - 1);
  assert(it == array1.end() - 1);
  assert(array1.size() == 2);
  assert(array1.front() == 10);
  assert(array1.back() == 25);
}
END_TEST

void
eina_test_ptrarray(TCase* tc)
{
  tcase_add_test(tc, eina_cxx_ptrarray_push_back);
  tcase_add_test(tc, eina_cxx_ptrarray_pop_back);
  tcase_add_test(tc, eina_cxx_ptrarray_insert);
  tcase_add_test(tc, eina_cxx_ptrarray_constructors);
  tcase_add_test(tc, eina_cxx_ptrarray_erase);
}
