
/// 
/// @file eo_base.hpp
/// 

#ifndef EO3_EO_BASE_HPP
#define EO3_EO_BASE_HPP

#include <assert.h>

#include <stdexcept>

#include <boost/optional.hpp>

namespace efl { namespace eo {

/// @addtogroup Efl_Cxx_API
/// @{

/// @brief A binding to the <em>EO Base Class</em>.
///
/// This class implements C++ wrappers to all the <em>EO Base</em>
/// operations.
///
struct base
{
  /// @brief Class constructor.
  /// 
  /// @param eo The <em>EO Object</em>.
  /// 
  /// efl::eo::base constructors semantics are that of stealing the
  /// <em>EO Object</em> lifecycle management. Its constructors do not
  /// increment the <em>EO</em> reference counter but the destructors
  /// do decrement.
  ///
  explicit base(Eo* eo) : _eo_raw(eo)
  {
    assert(eo != 0);
  }

  /// @brief Class destructor.
  ///
  ~base()
  {
    std::cout << "~base" << std::endl;
    eo_unref(_eo_raw);
  }

  /// @brief Assignment operator.
  ///
  base& operator=(base const& other)
  {
    eo_ref(_eo_raw);
    return *this;
  }

  /// @brief Return a pointer to the <em>EO Object</em> stored in this
  /// instance.
  ///
  /// @return A pointer to the opaque <em>EO Object</em>.
  ///
  Eo* _eo_ptr() const { return _eo_raw; }

  /// @brief Get the reference count of this object.
  ///
  /// @return The referencer count of this object.
  ///
  int ref_get() const { return ::eo_ref_get(_eo_raw); }

  /// @brief Set the parent of this object.
  /// 
  /// @param parent The new parent.
  ///
  void parent_set(base parent) 
  { 
    eo2_do(_eo_raw, ::eo2_parent_set(parent._eo_ptr())); 
  }

  /// @brief Get the parent of this object.
  ///
  /// @return An @ref efl::eo::base instance that binds the parent
  /// object.
  ///
  boost::optional<base> parent_get() 
  { 
    Eo *r = NULL;
    eo2_do(_eo_raw, r = ::eo2_parent_get());
    if(!r) return boost::none;
    else 
      { 
        eo_ref(r); // XXX eo2_parent_get does not call eo_ref so we may.
        return base(r);
      }
  }

  /// @brief Set generic data to object.
  /// 
  /// @param key The key associated with the data.
  /// @param data The data to set.
  /// @param free_func A pointer to the function that frees the
  /// data. @c (::eo_base_data_free_func*)0 is valid.
  ///
  void base_data_set(const char *key, const void *data, ::eo_base_data_free_func func)
  { 
    eo2_do(_eo_raw, ::eo2_base_data_set(key, data, func)); 
  }

  /// @brief Get generic data from object.
  /// 
  /// @param key The key associated with desired data.
  /// @return A void pointer to the data.
  ///
  void* base_data_get(const char *key)
  { 
    void *r = NULL;
    eo2_do(_eo_raw, r = ::eo2_base_data_get(key)); 
    return r;
  }

  /// @brief Delete generic data from object.
  ///
  /// @param key The key associated with the data.
  ///
  void base_data_del(const char *key) 
  { 
    eo2_do(_eo_raw, ::eo2_base_data_del(key));
  }

  /// @brief Freeze any event directed to this object.
  ///
  /// Prevents event callbacks from being called for this object.
  ///
  void event_freeze()
  {
    eo2_do(_eo_raw, ::eo2_event_freeze());
  }

  /// @brief Thaw the events of this object.
  ///
  /// Let event callbacks be called for this object.
  ///
  void event_thaw()
  {
    eo2_do(_eo_raw, ::eo2_event_thaw());
  }

  /// @brief Get the event freeze count for this object.
  ///
  /// @return The event freeze count for this object.
  ///
  int event_freeze_get()
  {
    int r = 0;
    eo2_do(_eo_raw, r = ::eo2_event_freeze_get());
    return r;
  }

  /// @brief Get debug information of this object.
  ///
  /// @return The root node of the debug information tree.
  ///
  Eo_Dbg_Info dbg_info_get()
  {
    Eo_Dbg_Info info;
    eo2_do(_eo_raw, ::eo2_dbg_info_get(&info));
    return info;
  }

protected:
  Eo* _eo_raw; ///< The opaque <em>EO Object</em>.
};


/// @brief Downcast @p U to @p T.
///
/// @param T An <em>EO C++ Class</em>.
/// @param U An <em>EO C++ Class</em>.
/// 
/// @param object The target object.
/// @return This function returns a new instance of @p T if the
/// downcast is successful --- otherwise it raises a @c
/// std::runtime_error.
/// 
template <typename T, typename U>
T downcast(U object)
{
  Eo *eo = object._eo_ptr();

  if(eo_isa(eo, T::_eo_class()))
    return T(eo_ref(eo));
  else
    throw std::runtime_error("Invalid cast");
}

/// @}

}}

#endif // EO3_EO_BASE_HPP
