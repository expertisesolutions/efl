
#include "Eina.hh"

#include <cassert>
#include <algorithm>

struct bigger_than_pointer
{
  std::ptrdiff_t a,b;
};

int main()
{
  efl::eina::eina_init eina_init;
  {
    efl::eina::ptr_list<bigger_than_pointer> list;
    assert(list.size() == 0);
    assert(list.empty());

    std::auto_ptr<bigger_than_pointer> elem
      (new bigger_than_pointer);
    elem->a = 5; elem->b = 10;
    list.push_back(elem);
    assert(list.size() == 1);
    assert(!list.empty());
    assert(std::distance(list.begin(), list.end()) == 1);
    assert(list.begin()->a == 5 && list.begin()->b == 10);
  }
  {
    efl::eina::ptr_list<int> list;
    assert(list.size() == 0);
    assert(list.empty());

    std::auto_ptr<int> p(new int(5));
    list.push_back(p);
    assert(list.size() == 1);
    assert(!list.empty());
    assert(std::distance(list.begin(), list.end()) == 1);
    assert(*list.begin() == 5);
  }  
}
