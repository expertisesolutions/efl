#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <evil_api.h>

#include <stdlib.h>

#include <evil_windows.h>

EVIL_API char* dirname (char* path)
{
  static char dir[MAX_PATH];
  _splitpath_s (path, NULL, 0, dir, MAX_PATH, NULL, 0, NULL, 0);
  return dir;
}
