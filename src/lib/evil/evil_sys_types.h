#ifndef __EVIL_SYS_TYPES_H__
#define __EVIL_SYS_TYPES_H__

/**
 * @file evil_sys_time.h @brief The file that provides functions/types ported
 * from Unix in sys/types.h.
 * @defgroup Evil_Time_Group Time.h functions
 * @ingroup Evil
 *
 * This header provides functions ported from Unix in sys/types.h.
 *
 * @{
 */


# ifndef pid_t
#  ifdef _MSC_VER
typedef int pid_t;
#  endif
# endif

#endif
