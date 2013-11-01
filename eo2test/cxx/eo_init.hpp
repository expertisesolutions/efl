
#ifndef EO3_EO_INIT_HPP
#define EO3_EO_INIT_HPP

#include <Eo.h>

namespace efl { namespace eo {

/// @brief A simple RAII implementation that initializes and terminates
/// the <em>EO Subsystem</em>.
///
struct eo_init
{
  /// @brief Default constructor.
  /// Invokes @c ::eo_init().
  eo_init() { ::eo_init(); }

  /// @brief Class destructor.
  /// Invokes @c ::eo_shutdown().
  ~eo_init() { ::eo_shutdown(); }
};

} }

#endif // EO3_EO_INIT_HPP
