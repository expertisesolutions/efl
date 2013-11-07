
/// 
/// @file eo_init.hpp
/// 

#ifndef EO3_EO_INIT_HPP
#define EO3_EO_INIT_HPP

#include <Eo.h>

namespace efl { namespace eo {

/// @addtogroup Efl_Cxx_API
/// @{

/// @brief A simple RAII implementation to initialize and terminate
/// the <em>EO Subsystem</em>.
///
struct eo_init
{
  /// @brief Default constructor.
  /// 
  /// Invokes @c ::eo_init().
  /// 
  eo_init() { ::eo_init(); }

  /// @brief Class destructor.
  /// 
  /// Invokes @c ::eo_shutdown().
  /// 
  ~eo_init() { ::eo_shutdown(); }
};

/// @}

} }

#endif // EO3_EO_INIT_HPP
