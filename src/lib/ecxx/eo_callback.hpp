
/// 
/// @file eo_callback.hpp
/// 

#ifndef EO3_EO_CALLBACK_HPP
#define EO3_EO_CALLBACK_HPP

#include "Eina.h"

namespace efl { namespace eo {

/// @addtogroup Efl_Cxx_API
/// @{

/// @brief A token that holds the elements of a registered <em>EO
/// Event Callback</em>.
///
/// This structure is returned whenever the user adds a new event
/// handler to an <em>EO C++ Object</em>
///
struct callback_token
{
  /// @typedef
  typedef void(*free_type)(void*); 

  void* data;                      ///< User data passed to the
                                   ///callback when an Event is
                                   ///raised.
  void* function;                  ///< The callback function.
  free_type free_function;         ///< The procedure invoked to
                                   ///destroy user data @ref
                                   ///efl::eo::callback_token::data.
};

/// @}

namespace detail {

/// @addtogroup Efl_Cxx_Detail
/// @{

/// @internal
///
/// @brief Delete the event callback instance.
///
/// @param f The event callback instance.
///
template <typename F>
void free_callback(void* f)
{
  delete static_cast<F*>(f);
}

/// @internal
///
/// @brief A generic wrapper that knows how to invoke the callback
/// instance with proper arguments.
///
/// @params data The user-data to be passed to the callback.
/// @params a0 First argument to be passed to the callback.
/// 
template <typename F, typename A0>
static Eina_Bool callback_function_object1(void* data, A0 a0)
{
  F* f = static_cast<F*>(data);
  (*f)(a0);
  return true; // XXX
};

/// @}

}

} }

#endif
