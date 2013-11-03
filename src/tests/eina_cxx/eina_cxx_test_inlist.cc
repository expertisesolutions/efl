
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
    efl::eina::inlist<bigger_than_pointer> list;
    assert(list.size() == 0);
    assert(list.empty());

    bigger_than_pointer elem = {5, 10};
    list.push_back(elem);
    assert(list.size() == 1);
    assert(!list.empty());
    assert(std::distance(list.begin(), list.end()) == 1);
    assert(list.begin()->a == 5 && list.begin()->b == 10);

    bigger_than_pointer e2 = {10, 15};
    list.insert(list.end(), e2);
    assert(list.size() == 2);
    assert(std::distance(list.begin(), list.end()) == 2);
    assert(list.back().a == 10 && list.back().b == 15);

    bigger_than_pointer e3 = {15, 20};
    list.insert(list.begin(), e3);
    assert(list.size() == 3);
    assert(std::distance(list.begin(), list.end()) == 3);
    assert(list.front().a == 15 && list.front().b == 20);
    
  }
}
