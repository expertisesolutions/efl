#ifndef EINA_ACCESSOR_HH_
#define EINA_ACCESSOR_HH_

#include <Eina.h>
#include <eina_error.hh>

#include <memory>
#include <iterator>
#include <cstdlib>
#include <cassert>

namespace efl { namespace eina {

template <typename T>
struct accessor
{
  typedef unsigned int key_type;
  typedef T mapped_type;
  typedef T value_type;
  typedef std::size_t size_type;

  accessor() : _impl(0) {}
  explicit accessor(Eina_Accessor* impl)
    : _impl(impl)
  {
    assert(_impl != 0);
  }
  
  mapped_type& operator[](size_type i) const
  {
    assert(_impl != 0);
    void* p;
    if(!eina_accessor_data_get(_impl, i, &p))
      {
        eina::error_code ec = efl::eina::get_error_code();
        throw eina::system_error(ec, "EFL Eina Error");
      }
    return *static_cast<mapped_type*>(p);
  }

  Eina_Accessor* native_handle() const;
private:
  typedef Eina_Accessor*(accessor<T>::*unspecified_bool_type)() const;
public:
  operator unspecified_bool_type() const
  {
    return native_handle() ? &accessor<T>::native_handle : 0 ;
  }
private:
  Eina_Accessor* _impl;
};

template <typename T>
struct accessor_iterator
{
  typedef T value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef std::ptrdiff_t difference_type;
  typedef std::random_access_iterator_tag iterator_category;

  accessor_iterator(accessor<T> const& a, unsigned int pos = 0u)
    : _accessor(&a), _index(pos)
  {}

  accessor_iterator<T>& operator++()
  {
    ++_index;
    return *this;
  }
  accessor_iterator<T>& operator--()
  {
    --_index;
    return *this;
  }
  accessor_iterator<T>& operator++(int)
  {
    accessor_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  accessor_iterator<T>& operator--(int)
  {
    accessor_iterator<T> tmp(*this);
    --*this;
    return tmp;
  }
  value_type& operator*() const
  {
    return (*_accessor)[_index];
  }
  pointer operator->() const
  {
    return &**this;
  }
private:
  accessor<T> const* _accessor;
  unsigned int _index;

  template <typename U>
  friend bool operator==(accessor_iterator<U> const& lhs, accessor_iterator<U> const& rhs)
  {
    return lhs._index == rhs._index;
  }
};

template <typename U>
bool operator!=(accessor_iterator<U> const& lhs, accessor_iterator<U> const& rhs)
{
  return !(lhs == rhs);
}

} }

#endif
