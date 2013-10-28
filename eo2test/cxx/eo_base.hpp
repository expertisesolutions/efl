
#ifndef EO3_EO_BASE_HPP
#define EO3_EO_BASE_HPP

#include <assert.h>

#include <boost/optional.hpp>

namespace efl { namespace eo {

struct base
{
  explicit base(Eo* eo) : _eo_raw(eo)
  {
    assert(eo != 0);
  }

  ~base()
  {
    std::cout << "~base" << std::endl;
    eo_unref(_eo_raw);
  }

  base& operator=(base const& other)
  {
    eo_ref(_eo_raw);
    return *this;
  }

  Eo* _eo_ptr() const { return _eo_raw; }

  int ref_get() const { return ::eo_ref_get(_eo_raw); }

  void parent_set(base parent) 
  { 
    eo2_do(_eo_raw, ::eo2_parent_set(parent._eo_ptr())); 
  }

  boost::optional<base> parent_get() 
  { 
    Eo *r = NULL;
    eo2_do(_eo_raw, r = ::eo2_parent_get());
    if(!r) return boost::none;
    else return base(r);
  }

  void base_data_set(const char *key, const void *data, ::eo_base_data_free_func func)
  { 
    eo2_do(_eo_raw, ::eo2_base_data_set(key, data, func)); 
  }

  void* base_data_get(const char *key)
  { 
    void *r = NULL;
    eo2_do(_eo_raw, r = ::eo2_base_data_get(key)); 
    return r;
  }

  void base_data_del(const char *key) 
  { 
    eo2_do(_eo_raw, ::eo2_base_data_del(key));
  }

  void event_freeze()
  {
    eo2_do(_eo_raw, ::eo2_event_freeze());
  }

  void event_thaw()
  {
    eo2_do(_eo_raw, ::eo2_event_thaw());
  }

  bool event_freeze_get()
  {
    bool r = false;
    eo2_do(_eo_raw, r = ::eo2_event_freeze_get());
    return r;
  }

  Eo_Dbg_Info dbg_info_get()
  {
    Eo_Dbg_Info info;
    eo2_do(_eo_raw, ::eo2_dbg_info_get(&info));
    return info;
  }

protected:
  Eo* _eo_raw;
};

}}

#endif // EO3_EO_BASE_HPP
