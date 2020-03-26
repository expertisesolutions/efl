#ifndef __EVIL_STAT_H__
#define __EVIL_STAT_H__

// Missing definitions:
// Note: some pieces of code were based on LibreSSL-Portable's compat lib and
// adapted to EFL standards.
#ifdef _MSC_VER
# define S_IRWXU  0                           /* RWX user */
# define S_IRUSR  S_IREAD                     /* Read user */
# define S_IWUSR  S_IWRITE                    /* Write user */
# define S_IXUSR  0                           /* Execute user */
# define S_IRWXG  0                           /* RWX group */
# define S_IRGRP  0                           /* Read group */
# define S_IWGRP  0                           /* Write group */
# define S_IXGRP  0                           /* Execute group */
# define S_IRWXO  0                           /* RWX others */
# define S_IROTH  0                           /* Read others */
# define S_IWOTH  0                           /* Write others */
# define S_IXOTH  0                           /* Execute others */
#endif

#endif

#include <corecrt.h>
#include <sys/types.h>
#include_next <sys/stat.h>
