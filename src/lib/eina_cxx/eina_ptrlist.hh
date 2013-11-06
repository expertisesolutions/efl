#ifndef EINA_LIST_HH_
#define EINA_LIST_HH_

#include <Eina.h>
#include <eina_clone_allocators.hh>
#include <eina_lists_auxiliary.hh>
#include <eina_type_traits.hh>

#include <memory>
#include <iterator>

namespace efl { namespace eina {

struct _ptr_list_iterator_base
{
  typedef std::ptrdiff_t difference_type;
  typedef std::bidirectional_iterator_tag iterator_category;

  _ptr_list_iterator_base() : _list(0) {}
  _ptr_list_iterator_base(Eina_List* list, Eina_List* node)
    : _list(list), _node(node)
  {}

protected:
  Eina_List *_list, *_node;
};

template <typename T>
struct _ptr_list_iterator : _ptr_list_iterator_base
{
  typedef T value_type;
  typedef value_type* pointer;
  typedef value_type& reference;

  _ptr_list_iterator() {}
  explicit _ptr_list_iterator(Eina_List* list, Eina_List* node)
    : _ptr_list_iterator_base(list, node)
  {
  }
  _ptr_list_iterator(_ptr_list_iterator<typename remove_cv<value_type>::type> const& other)
    : _ptr_list_iterator_base(static_cast<_ptr_list_iterator_base const&>(other))
  {
  }

  _ptr_list_iterator<T>& operator++()
  {
    _node = eina_list_next(_node);
    return *this;
  }
  _ptr_list_iterator<T> operator++(int)
  {
    _ptr_list_iterator<T> tmp(*this);
    ++*this;
    return tmp;
  }
  _ptr_list_iterator<T>& operator--()
  {
    if(_node)
      _node = eina_list_prev(_node);
    else
      _node = eina_list_last(_list);
    return *this;
  }
  _ptr_list_iterator<T> operator--(int)
  {
    _ptr_list_iterator<T> tmp(*this);
    --*this;
    return tmp;
  }
  reference operator*() const
  {
    void* data = eina_list_data_get(_node);
    return *static_cast<pointer>(data);
  }
  pointer operator->() const
  {
    return &**this;
  }
  Eina_List* native_handle()
  {
    return _node;
  }
  Eina_List const* native_handle() const
  {
    return _node;
  }
  friend inline bool operator==(_ptr_list_iterator<T> lhs, _ptr_list_iterator<T> rhs)
  {
    return lhs._node == rhs._node;
  }
  friend inline bool operator!=(_ptr_list_iterator<T> lhs, _ptr_list_iterator<T> rhs)
  {
    return !(lhs == rhs);
  }
};

template <typename T, typename CloneAllocator>
struct _ptr_list_common_base
{
  typedef CloneAllocator clone_allocator_type;

  _ptr_list_common_base(CloneAllocator clone_allocator)
    : _impl(clone_allocator)
  {}
  _ptr_list_common_base(Eina_List* _list)
    : _impl(_list)
  {}
  _ptr_list_common_base() {}

  CloneAllocator& _get_clone_allocator()
  {
    return _impl;
  }
  CloneAllocator const& _get_clone_allocator() const
  {
    return _impl;
  }
  void _delete_clone(T const* p)
  {
    _get_clone_allocator().deallocate_clone(p);
  }
  T* _new_clone(T const& a)
  {
    return _get_clone_allocator().allocate_clone(a);
  }

  struct _ptr_list_impl : CloneAllocator
  {
    _ptr_list_impl() : _list(0) {}
    _ptr_list_impl(CloneAllocator allocator)
      : clone_allocator_type(allocator), _list(0) {}

    Eina_List* _list;
  };  

  _ptr_list_impl _impl;

private:
  _ptr_list_common_base(_ptr_list_common_base const& other);
  _ptr_list_common_base& operator=(_ptr_list_common_base const& other);
};

template <typename T, typename CloneAllocator = no_clone_allocator>
class ptr_list : protected _ptr_list_common_base<T, CloneAllocator>
{
  typedef _ptr_list_common_base<T, CloneAllocator> _base_type;
public:
  typedef T value_type;
  typedef T& reference;
  typedef T const& const_reference;
  typedef _ptr_list_iterator<T const> const_iterator;
  typedef _ptr_list_iterator<T> iterator;
  typedef T* pointer;
  typedef T const* const_pointer;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef CloneAllocator clone_allocator_type;

  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  ptr_list() {}
  ptr_list(size_type n, const_reference t)
  {
    while(n--)
      push_back(t);
  }
  template <typename InputIterator>
  ptr_list(InputIterator i, InputIterator const& j
           , clone_allocator_type const& alloc = clone_allocator_type()
           , typename eina::enable_if_c<!eina::is_integral<InputIterator>::value>::type* = 0)
    : _base_type(alloc)
  {
    while(i != j)
      {
        push_back(*i);
        ++i;
      }
  }
  ptr_list(ptr_list<T>const& other)
    : _base_type()
  {
    insert(end(), other.begin(), other.end());
  }
  ptr_list<T, CloneAllocator>& operator=(ptr_list<T, CloneAllocator>const& other)
  {
    clear();
    insert(end(), other.begin(), other.end());
    return *this;
  }
  
  void clear()
  {
    for(iterator first = begin(), last = end(); first != last; ++first)
      this->_delete_clone(&*first);
    eina_list_free(this->_impl._list);
    this->_impl._list = 0;
  }
  std::size_t size() const
  {
    return eina_list_count(this->_impl._list);
  }
  bool empty() const
  {
    return size() == 0u;
  }
  clone_allocator_type get_clone_allocator() const
  {
    return clone_allocator_type(this->_get_clone_allocator());
  }
  void push_back(const_reference a)
  {
    push_back(this->_new_clone(a));
  }
  void push_back(pointer p)
  {
    std::auto_ptr<value_type> p1(p);
    push_back(p1);
  }
  void push_back(std::auto_ptr<T>& p)
  {
    Eina_List* new_list = eina_list_append(this->_impl._list, p.get());
    if(new_list)
    {
      this->_impl._list = new_list;
      p.release();
    }
    else
      throw std::bad_alloc();
  }
  void push_front(const_reference a)
  {
    push_front(this->new_clone(a));
  }
  void push_front(pointer p)
  {
    std::auto_ptr<value_type> p1(p);
    push_front(p1);
  }
  void push_front(std::auto_ptr<T>& p)
  {
    Eina_List* new_list = eina_list_prepend(this->_impl._list, p.get());
    if(new_list)
    {
      this->_impl._list = new_list;
      p.release();
    }
    else
      throw std::bad_alloc();
  }
  void pop_back()
  {
    this->_impl._list = eina_list_remove_list(this->_impl._list, eina_list_last(this->_impl._list));
  }
  void pop_front()
  {
    this->_impl._list = eina_list_remove_list(this->_impl._list, this->_impl._list);
  }
  iterator insert(iterator i, value_type const& t)
  {
    return insert(i, this->_new_clone(t));
  }  
  iterator insert(iterator i, pointer pv)
  {
    std::auto_ptr<value_type> p(pv);
    return insert(i, p);
  }
  iterator insert(iterator i, std::auto_ptr<value_type> p)
  {
    this->_impl._list = _eina_list_prepend_relative_list
      (this->_impl._list, p.get(), i.native_handle());
    if(this->_impl._list)
      p.release();
    else
      throw std::bad_alloc();
    return iterator(this->_impl._list
                    , i.native_handle()
                    ? ::eina_list_prev(i.native_handle())
                    : ::eina_list_last(this->_impl._list)
                    );
  }  
  iterator insert(iterator i, size_t n, value_type const& t)
  {
    iterator r = i;
    if(n--)
      r = insert(i, t);
    while(n--)
      insert(i, t);
    return r;
  }
  iterator insert(iterator i, size_t n, pointer p)
  {
    iterator r = i;
    if(n--)
      r = insert(i, p);
    while(n--)
      insert(i, this->_new_clone(p));
    return r;
  }
  template <typename InputIterator>
  iterator insert(iterator p, InputIterator i, InputIterator j
                  , typename eina::enable_if_c<!eina::is_integral<InputIterator>::value>::type* = 0)
  {
    iterator r = p;
    if(i != j)
      {
        r = insert(p, *i);
        ++i;
      }
    while(i != j)
      {
        insert(p, this->_new_clone(*i));
        ++i;
      }
    return r;
  }
  iterator erase(iterator q)
  {
    if(q.native_handle())
      {
        iterator r(this->_impl._list, eina_list_next(q.native_handle()));
        this->_impl._list = eina_list_remove_list(this->_impl._list, q.native_handle());
        return r;
      }
    else
      return q;
  }
  iterator erase(iterator i, iterator j)
  {
    while(i != j)
      i = erase(i);
    if(j.native_handle())
      return j;
    else
      return end();
  }
  template <typename InputIterator>
  void assign(InputIterator i, InputIterator j
              , typename eina::enable_if_c<!eina::is_integral<InputIterator>::value>::type* = 0)
  {
    clear();
    insert(end(), i, j);
  }
  void assign(size_type n, value_type const& t)
  {
    clear();
    insert(end(), n, t);
  }

  value_type& back()
  {
    return *static_cast<pointer>(eina_list_data_get(eina_list_last(this->_impl._list)));
  }
  value_type const& back() const
  {
    return const_cast<ptr_list<T, CloneAllocator>&>(*this).back();
  }
  value_type& front()
  {
    return *static_cast<pointer>(eina_list_data_get(this->_impl._list));
  }
  value_type const& front() const
  {
    return const_cast<ptr_list<T, CloneAllocator>&>(*this).front();
  }

  const_iterator begin() const
  {
    return const_iterator(this->_impl._list, this->_impl._list);
  }
  const_iterator end() const
  {
    return const_iterator(this->_impl._list, 0);
  }
  iterator begin()
  {
    return iterator(this->_impl._list, this->_impl._list);
  }
  iterator end()
  {
    return iterator(this->_impl._list, 0);
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
  eina::iterator<T> ibegin()
  {
    return eina::iterator<T>( ::eina_list_iterator_new(this->_impl._list) );
  }
  eina::iterator<T> iend()
  {
    return eina::iterator<T>();
  }
  eina::iterator<T const> ibegin() const
  {
    return eina::iterator<T const>( ::eina_list_iterator_new(this->_impl._list) );
  }
  eina::iterator<T const> iend() const
  {
    return eina::iterator<T const>();
  }
  eina::iterator<T const> cibegin() const
  {
    return ibegin();
  }
  eina::iterator<T const> ciend() const
  {
    return iend();
  }
  void swap(ptr_list<T, CloneAllocator>& other)
  {
    std::swap(this->_impl._list, other._impl._list);
  }
  size_type max_size() const { return -1; }

  Eina_List* native_handle()
  {
    return this->_impl._list;
  }
  Eina_List const* native_handle() const
  {
    return this->_impl._list;
  }
};

template <typename T, typename CloneAllocator1, typename CloneAllocator2>
bool operator==(ptr_list<T, CloneAllocator1> const& lhs, ptr_list<T, CloneAllocator2> const& rhs)
{
  return lhs.size() == rhs.size()
    && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename CloneAllocator1, typename CloneAllocator2>
bool operator!=(ptr_list<T, CloneAllocator1> const& lhs, ptr_list<T, CloneAllocator2> const& rhs)
{
  return !(lhs == rhs);
}

template <typename T, typename CloneAllocator>
void swap(ptr_list<T, CloneAllocator>& lhs, ptr_list<T, CloneAllocator>& rhs)
{
  lhs.swap(rhs);
}

} }

#endif
