
///
/// @file eo_wref.hpp
///

#ifndef EO3_WREF_HPP
#define EO3_WREF_HPP

#include <boost/optional.hpp>

namespace efl { namespace eo {

/// @addtogroup Efl_Cxx_API
/// @{

/// @brief Weak references to an <em>EO Object</em>.
///
template<typename T>
struct wref {

  /// @brief Class constructor.
  ///
  /// @param obj The <em>EO Object</em> to be referenced.
  ///
  /// Create a weak reference to @p obj. 
  ///
  explicit wref(Eo* obj) : _eo_wref(obj)
  {
    _add();
  }

  /// @brief Class constructor.
  ///
  /// @param obj The <em>EO C++ Object</em> to be referenced.
  /// 
  /// Create a weak reference to @p obj.
  ///
  wref(T obj) : _eo_wref(obj._eo_ptr())
  {
    _add();
  }

  /// @brief Class destructor.
  ///
  ~wref()
  {
    if(boost::optional<T> p = lock())
      _del();
  }

  /// @brief Try to acquire a strong reference to the underlying
  /// <em>EO Object</em>.
  ///
  /// This function checkes whether the weak reference still points
  /// to a valid <em>EO Object</em>. If the reference is still valid
  /// it increments the reference counter of the object and returns
  /// a pointer to it.
  ///
  /// @return If the lock was successfully acquired it returns a
  /// strong reference to the <em>EO Object</em>. Otherwise it returns
  /// @p boost::none.
  /// 
  boost::optional<T> lock()
  {
    if(_eo_wref) // XXX eo_ref() should work on multi-threaded environments
      eo_ref(_eo_wref);
    else
      return boost::none;
    return T(_eo_wref);
  }

  /// @brief Copy constructor.
  /// 
  wref(wref const& other)
    : _eo_wref(other._eo_wref)
  {
    if(boost::optional<T> p = lock())
      _add();
    else
      _eo_wref = 0;
  }

  /// @brief Assignment operator.
  ///
  wref& operator=(wref const& other)
  {
    _eo_wref = other._eo_wref;
    if(boost::optional<T> p = lock())
      _add();
    else
      _eo_wref = 0;
  }

private:
  void _add()
  {
    eo2_do(_eo_wref, eo2_wref_add(&_eo_wref));
  }
  void _del()
  {
    eo2_do(_eo_wref, eo2_wref_del(&_eo_wref));
  }

    Eo* _eo_wref; ///< The weak reference.

//   wref(T obj)
//   {
//     _eo_wref.reset(new Eo*);
//     *_eo_wref = obj._eo_ptr();
//   }
//
//   boost::optional<T> lock()
//   {
//     eo_ref(*wref);
//     if(!*wref) 
//       return boost::none;
//     return T(*wref);
//   }
//
// private:
//   void _w(Eo* obj)
//   {
//     eo2_do(obj, eo2_wref_add(_eo_wref.get()));
//   }
//   boost::shared_ptr<Eo*> _eo_wref;
};

/// @}

} } // namespace efl { namespace eo {

#endif // EO3_WREF_HPP
