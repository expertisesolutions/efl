#ifndef EINA_INARRAY_HH_
#define EINA_INARRAY_HH_

#include <Eina.h>
#include <eina_type_traits.hh>

#include <boost/mpl/if.hpp>

#include <iterator>
#include <cstring>

namespace efl { namespace eina {

struct _inarray_common_base
{
  typedef std::size_t size_type;

  explicit _inarray_common_base(size_type member_size)
    : _array( ::eina_inarray_new(member_size, 0) )
  {
  }
  ~_inarray_common_base()
  {
    ::eina_inarray_free(_array);
  }
  
  size_type size() const
  {
    return ::eina_inarray_count(_array);
  }
  bool empty() const
  {
    return size() == 0u;
  }

  Eina_Inarray* _array;
private:
  _inarray_common_base(_inarray_common_base const& other);
  _inarray_common_base& operator=(_inarray_common_base const& other);
};

template <typename T>
class _pod_inarray : _inarray_common_base
{
  typedef _inarray_common_base _base_type;
public:
  typedef T value_type;
  typedef T& reference;
  typedef T const& const_reference;
  typedef T* pointer;
  typedef T const* const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  using _base_type::size;
  using _base_type::empty;

  _pod_inarray() : _base_type(sizeof(T))
  {
  }
  _pod_inarray(size_type n, value_type const& t) : _base_type(sizeof(T))
  {
    while(n--)
      push_back(t);
  }
  template <typename InputIterator>
  _pod_inarray(InputIterator i, InputIterator const& j
         , typename eina::enable_if_c<!eina::is_integral<InputIterator>::value>::type* = 0)
    : _base_type(sizeof(T))
  {
    while(i != j)
      {
        push_back(*i);
        ++i;
      }
  }
  _pod_inarray(_pod_inarray<T>const& other)
    : _base_type(sizeof(T))
  {
    insert(end(), other.begin(), other.end());
  }
  ~_pod_inarray()
  {
  }
  _pod_inarray<T>& operator=(_pod_inarray<T>const& other)
  {
    clear();
    insert(end(), other.begin(), other.end());
    return *this;
  }
  void clear()
  {
    ::eina_inarray_flush(_array);
  }
  void push_back(T const& value)
  {
    eina_inarray_push(_array, &value);
  }
  void pop_back()
  {
    eina_inarray_pop(_array);
  }
  iterator insert(iterator i, value_type const& t)
  {
    if(i != end())
    {
      T* q = static_cast<iterator>
        ( ::eina_inarray_alloc_at(_array, i - &front(), 1u));
      std::memcpy(q, &t, sizeof(t));
      return q;
    }
    else
    {
      push_back(t);
      return end()-1;
    }
  }
  iterator insert(iterator i, size_t n, value_type const& t)
  {
    T* q;
    if(i != end())
    {
      q = static_cast<iterator>
        ( ::eina_inarray_alloc_at(_array, i - &front(), n));
    }
    else
    {
      q = eina_inarray_grow(_array, n);
    }
    for(T* p = q; n; --n, ++p)
      std::memcpy(p, &t, sizeof(t));
    return q;
  }
  template <typename InputIterator>
  iterator insert(iterator p, InputIterator i, InputIterator j
                  , typename eina::enable_if_c<!eina::is_integral<InputIterator>::value>::type* = 0)
  {
    size_type n = 0;
    while(i != j)
      {
        p = insert(p, *i);
        ++p;
        ++i;
        ++n;
      }
    return p - n;
  }
  iterator erase(iterator q)
  {
    ::eina_inarray_remove_at(_array, q - &front());
    return q;
  }
  iterator erase(iterator i, iterator j)
  {
    while(i != j)
      {
        erase(--j);
      }
    return i;
  }
  template <typename InputIterator>
  void assign(InputIterator i, InputIterator j
              , typename eina::enable_if_c<!eina::is_integral<InputIterator>::value>::type* = 0);
  void assign(size_type n, value_type const& t);
  value_type& back()
  {
    assert(!empty());
    return *static_cast<value_type*>(eina_inarray_nth(_array, size()-1u));
  }
  value_type const& back() const
  {
    return const_cast<_pod_inarray<T>&>(*this).back();
  }
  value_type& front()
  {
    assert(!empty());
    return *static_cast<value_type*>(eina_inarray_nth(_array, 0u));
  }
  value_type const& front() const
  {
    return const_cast<_pod_inarray<T>&>(*this).front();
  }
  iterator begin()
  {
    return empty()? 0 : static_cast<iterator>(::eina_inarray_nth(_array, 0u));
  }
  iterator end()
  {
    return empty()? 0 : static_cast<iterator>(::eina_inarray_nth(_array, size()-1)) + 1;
  }
  const_iterator begin() const
  {
    return const_cast< _pod_inarray<T>&>(*this).begin();
  }
  const_iterator end() const
  {
    return const_cast< _pod_inarray<T>&>(*this).end();
  }
  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(end());
  }
  reverse_iterator rbegin()
  {
    return reverse_iterator(begin());
  }
  reverse_iterator rend()
  {
    return reverse_iterator(end());
  }
  const_iterator cbegin() const
  {
    return begin();
  }
  const_iterator cend() const
  {
    return end();
  }
  const_reverse_iterator crbegin() const
  {
    return rbegin();
  }
  const_reverse_iterator crend() const
  {
    return rend();
  }
  void swap(_pod_inarray<T>& other)
  {
    std::swap(_array, other._array);
  }
  size_type max_size() const { return -1; }

  Eina_Inarray* native_handle()
  {
    return this->_array;
  }
  Eina_Inarray const* native_handle() const
  {
    return this->_array;
  }
};

template <typename T>
class inarray : public ::boost::mpl::if_<eina::is_pod<T>, _pod_inarray<T>, void>::type
{
  typedef typename ::boost::mpl::if_<eina::is_pod<T>, _pod_inarray<T>, void>::type _base_type;
public:
  inarray() : _base_type() {}
  inarray(typename _base_type::size_type n, typename _base_type::value_type const& t)
    : _base_type(n, t) {}
  template <typename InputIterator>
  inarray(InputIterator i, InputIterator const& j
          , typename eina::enable_if_c<!eina::is_integral<InputIterator>::value>::type* = 0)
    : _base_type(i, j)
  {}
  
};

template <typename T>
bool operator==(inarray<T> const& lhs, inarray<T> const& rhs)
{
  return lhs.size() == rhs.size() &&
    std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T>
bool operator!=(inarray<T> const& lhs, inarray<T> const& rhs)
{
  return !(lhs == rhs);
}

template <typename T>
void swap(inarray<T>& lhs, inarray<T>& rhs)
{
  lhs.swap(rhs);
}



} }

#endif
