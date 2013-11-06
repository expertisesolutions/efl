#ifndef EINA_DEFAULT_DELETE_HH_
#define EINA_DEFAULT_DELETE_HH_

namespace efl { namespace eina {

struct heap_copy_allocator
{
  template <typename T>
  static T* allocate_clone(T const& v)
  {
    return new T(v);
  }

  template <typename T>
  static T* deallocate_clone(T* p)
  {
    delete p;
  }
};

struct heap_clone_allocator
{
  template <typename T>
  static T* allocate_clone(T const& v)
  {
    return new_clone(v);
  }
  template <typename T>
  static void deallocate_clone(T* p)
  {
    delete_clone(p);
  }
};

struct view_clone_allocator
{
  template <typename T>
  static T* allocate_clone(T const& v)
  {
    return const_cast<T*>(&v);
  }
  template <typename T>
  static void deallocate_clone(T* p)
  {
  }
};

struct no_clone_allocator
{
};    

} }

#endif
