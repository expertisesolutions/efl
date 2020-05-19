#ifndef __EVIL_STAT_H__
#define __EVIL_STAT_H__

#include <evil_windows.h>
#include <../ucrt/sys/types.h>
#include <evil_windows.h>
#include <sys/types.h>
#include_next <sys/stat.h>
#include <corecrt.h>
#include <evil_macro_wrapper.h>
#include <errno.h>
// loading as functions: chmod and umask (for umask, it is necessary to include: errno.h)
//  fstat ja incluido em <sys/stat.h>
#include <corecrt_io.h>

#ifndef stat64
# define stat64 _stat64
#endif

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



/*

// ------------------------------------------------------------------------

// Sinalizadores de tipo de arquivo para d_type  // ver a questão de st_mode

#define DT_UNKNOWN  0
#define DT_REG      S_IFREG
#define DT_DIR      S_IFDIR
#define DT_FIFO     S_IFIFO
#define DT_SOCK     S_IFSOCK  //ver 
#define DT_CHR      S_IFCHR
#define DT_BLK      S_IFBLK   //ver
#define DT_LNK      S_IFLNK   //ver 

*/