#ifndef EINA_LIST_HH_
#define EINA_LIST_HH_

#include <Eina.h>

#include <memory>
#include <iterator>

namespace efl { namespace eina {

struct _eina_list_iterator_base
{
  typedef std::ptrdiff_t difference_type;
  typedef std::bidirectional_iterator_tag iterator_category;

  _eina_list_iterator_base(Eina_List* list)
    : _list(list)
  {}

protected:
  Eina_List* _list;
};

template <typename T>
struct _eina_list_iterator : _eina_list_iterator_base
{
  typedef T const value_type;
  typedef value_type* pointer;
  typedef value_type& reference;

  _eina_list_iterator() {}
  explicit _eina_list_iterator(Eina_List* list)
    : _eina_list_iterator_base(list)
  {
  }

  _eina_list_iterator<T>& operator++()
  {
    _list = eina_list_next(_list);
    return *this;
  }
  _eina_list_iterator<T> operator++(int)
  {
    _eina_list_iterator<T> tmp(*this);
    _list = eina_list_next(_list);
    return tmp;
  }
  _eina_list_iterator<T>& operator--()
  {
    _list = eina_list_prev(_list);
  }
  _eina_list_iterator<T> operator--(int)
  {
    _eina_list_iterator<T> tmp(*this);
    _list = eina_list_prev(_list);
    return tmp;
  }
  reference operator*() const
  {
    void* data = eina_list_data_get(_list);
    return *static_cast<pointer>(data);
  }
  pointer operator->() const
  {
    return &**this;
  }
  friend inline bool operator==(_eina_list_iterator<T> lhs, _eina_list_iterator<T> rhs)
  {
    return lhs._list == rhs._list;
  }
  friend inline bool operator!=(_eina_list_iterator<T> lhs, _eina_list_iterator<T> rhs)
  {
    return !(lhs == rhs);
  }
};

struct _common_list_base
{
  _common_list_base() : _list(0) {}
  _common_list_base(_common_list_base const& other)
    : _list(eina_list_clone(other._list))
  {}
  _common_list_base& operator=(_common_list_base const& other)
  {
    _list = eina_list_clone(other._list);
    return *this;
  }

  std::size_t size() const
  {
    return eina_list_count(_list);
  }
  bool empty() const
  {
    return size() == 0u;
  }
protected:
  Eina_List* _list;
};

template <typename T>
struct ptr_list : _common_list_base
{
  typedef eina::_eina_list_iterator<T> const_iterator;
  const_iterator begin() const
  {
    return const_iterator(_list);
  }
  const_iterator end() const
  {
    return const_iterator(0);
  }

  void push_back(T* value)
  {
    std::auto_ptr<T> p(value);
    push_back(p);
  }
  void push_back(std::auto_ptr<T>& value)
  {
    Eina_List* new_list = eina_list_append(_list, value.get());
    if(new_list)
    {
      _list = new_list;
      value.release();
    }
    else
      throw std::bad_alloc();
  }
};

} }

#endif
