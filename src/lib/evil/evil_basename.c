#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "evil_private.h"

#include <stdlib.h>


EVIL_API char* basename (char* path)
{
  static char base[MAX_PATH];
  _splitpath_s (path, NULL, 0, NULL, 0, base, MAX_PATH, NULL, 0);
  return base;
}
