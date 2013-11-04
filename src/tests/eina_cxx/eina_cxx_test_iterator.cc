
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

    efl::eina::iterator<bigger_than_pointer> iterator
      (eina_list_iterator_new(list.native_handle())), last;

    assert(std::distance(iterator, last) == 1);
    iterator = efl::eina::iterator<bigger_than_pointer>
      (eina_list_iterator_new(list.native_handle()));
    bigger_than_pointer r = *iterator;
    assert(r.a == 5 && r.b == 10);
  }
}
