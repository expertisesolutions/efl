#ifndef UNPOSIX_SYS_STAT_H
#define UNPOSIX_SYS_STAT_H

#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <windows.h>
#endif
#include <sys/types.h>
#include_next <sys/stat.h>
#include <corecrt.h>

#include <evil_macro_wrapper.h>

#ifndef stat64
#define stat64 _stat64
#endif

// Missing definitions:
// Note: some pieces of code were based on LibreSSL-Portable's compat lib and
// adapted to EFL standards.
#if defined(_MSC_VER)
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


// Stolen from LibreSSL
/*
 * File type macros.  Note that block devices, sockets and links cannot be
 * distinguished on Windows and the macros S_ISBLK, S_ISSOCK and S_ISLNK are
 * only defined for compatibility.  These macros should always return false
 * on Windows.
 */
#define	S_ISFIFO(mode) (((mode) & S_IFMT) == S_IFIFO)
#define	S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#define	S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#define	S_ISLNK(mode)  (((mode) & S_IFMT) == S_IFLNK)
#define	S_ISSOCK(mode) (((mode) & S_IFMT) == S_IFSOCK)
#define	S_ISCHR(mode)  (((mode) & S_IFMT) == S_IFCHR)
#define	S_ISBLK(mode)  (((mode) & S_IFMT) == S_IFBLK)

#endif
