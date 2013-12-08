
#include "Eina.hh"

#include <cassert>
#include <algorithm>

#include <check.h>

bool no_args = false
  , args_1 = false
  , args_2 = false;

void thread_no_args()
{
  no_args = true;
}

struct test
{
  int x;
};

void thread_1_arg(int a0)
{
  args_1 = true;
  assert(a0 == 5);
}

void thread_2_arg(int a0, test t)
{
  args_2 = true;
  assert(a0 == 5);
  assert(t.x == 10);
}

START_TEST(eina_cxx_thread_constructors)
{
  {
    efl::eina::thread default_constructed_thread;
    assert(default_constructed_thread.get_id() == efl::eina::thread::id());
  }

  {
    efl::eina::thread thread_no_args(&::thread_no_args);
    thread_no_args.join();
    assert( ::no_args);
  }

  {
    efl::eina::thread thread_1_arg(&::thread_1_arg, 5);
    thread_1_arg.join();
    assert( ::args_1);
  }

  {
    test t = {10};
    efl::eina::thread thread_2_arg(&::thread_2_arg, 5, t);
    thread_2_arg.join();
    assert( ::args_2);
  }
}
END_TEST

void
eina_test_thread(TCase* tc)
{
  tcase_add_test(tc, eina_cxx_thread_constructors);
}
