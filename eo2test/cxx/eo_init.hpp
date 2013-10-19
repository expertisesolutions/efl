
#include <Eo.h>

namespace efl { namespace eo {

struct eo_init
{
  eo_init() { ::eo_init(); }
  ~eo_init() { ::eo_shutdown(); }
};

} }
