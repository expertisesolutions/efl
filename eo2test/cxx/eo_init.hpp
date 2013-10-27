
#ifndef EO3_EO_INIT_HPP
#define EO3_EO_INIT_HPP

#include <Eo.h>

namespace efl { namespace eo {

struct eo_init
{
  eo_init() { ::eo_init(); }
  ~eo_init() { ::eo_shutdown(); }
};

} }

#endif // EO3_EO_INIT_HPP
