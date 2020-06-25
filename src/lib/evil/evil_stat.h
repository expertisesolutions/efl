#ifndef __EVIL_STAT_H__
#define __EVIL_STAT_H__

#include <evil_windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <evil_macro_wrapper.h>
#include <evil_api.h>
#include <errno.h>

#ifndef _MSC_VER
# include <corecrt_io.h>
# include <corecrt.h>
#endif


typedef  int  mode_t ; 
#ifndef stat64
# define stat64 _stat64
#endif

EVIL_API_H int fstatat(int fd, const char *restrict path, struct stat *restrict buf, int flag);

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

#define	S_ISFIFO(mode) (((mode) & S_IFMT) == S_IFIFO)
#define	S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#define	S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#define	S_ISLNK(mode)  (((mode) & S_IFMT) == S_IFLNK)
#define	S_ISSOCK(mode) (((mode) & S_IFMT) == S_IFSOCK)
#define	S_ISCHR(mode)  (((mode) & S_IFMT) == S_IFCHR)
#define	S_ISBLK(mode)  (((mode) & S_IFMT) == S_IFBLK)

#endif
