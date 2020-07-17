#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <evil_private.h>

#include <stdlib.h>
#include <string.h> /* strlen */

EVIL_API char *
evil_dirname(char *path)
{
   // Expected from posix
   if ((path == NULL) || (*path == '\0') ) return ".";

   // Search for the last slash ignoring trailing '\\'
   unsigned int len = strlen(path);
   while(--len) if ((path[len] == '/') || (path[len] == '\\')) break;
   path[len] = '\0';

   return path;
}
