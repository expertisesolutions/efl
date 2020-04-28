#ifndef __EVIL_UNISTD_H__
#define __EVIL_UNISTD_H__

#include "evil_eapi.h"
#include "sys/types.h"
#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <io.h>
# include <process.h>
# include <winsock2.h>
# include <Windows.h>
# undef WIN32_LEAN_AND_MEAN
# endif

#define F_OK 0
#define X_OK 0
#define W_OK 2
#define R_OK 4
#define STDIN_FILENO  0 
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

char optarg; //temporary
int optind; //temporary
int opterr; //temporary
int optopt; //temporary
/**
 * @file evil_unistd.h
 * @brief The file that provides functions ported from Unix in unistd.h.
 * @defgroup Evil_Unistd_Group Unistd.h functions
 * @ingroup Evil
 *
 * This header provides functions ported from Unix in unistd.h.
 *
 * @{
 */


/*
 * Time related functions
 *
 */

/**
 * @brief Return the time spent since the Evil library has been initialized.
 *
 * @return The time spent since the Evil library has been initialized.
 *
 * This function returns the time spent since the Evil library has
 * been initialized. It uses a high-resolution timer and then can have
 * a precision up to the nano-second. The precision is processor
 * dependant. This function can be used to benchmark parts of code in
 * with high precision.
 *
 * Conformity: Not appliclable.
 *
 * Supported OS: Windows XP.
 */
EAPI double evil_time_get(void);

/*
 * Sockets and pipe related functions
 *
 */

/**
 * @brief Initiates the use of Windows sockets.
 *
 * @return 1 on success, 0 otherwise.
 *
 * Initiates the use of Windows sockets. If the function succeeds,
 * it returns 1, otherwise it returns 0.
 *
 * Conformity: Non applicable.
 *
 * Supported OS: Windows XP.
 */
EAPI int evil_sockets_init(void);

/**
 * @brief Shutdown the Windows socket system.
 *
 * Shutdown the Windows socket system.
 *
 * Conformity: Non applicable.
 *
 * Supported OS: Windows XP.
 */
EAPI void evil_sockets_shutdown(void);

/**
 * @brief Create a pair of sockets.
 *
 * @param fds A pointer that contains two sockets.
 *
 * Create a pair of sockets that can be use with select().
 * Hence, evil_sockets_init() must have been caled at least
 * once before. Contrary to Unix, that functions does not
 * create a pair of file descriptors.
 *
 * Conformity: Not applicable.
 *
 * Supported OS: Windows XP.
 */
EAPI int evil_pipe(int *fds);

/*
 * Wrapper related functions from posix unistd
 */

/**
 * @brief Suspends the execution of calling thread until time specified by seconds.
 * @param[in] Time in seconds for which execution is to be suspendend.
 * 
 * @since 1.15
 */

EAPI void evil_sleep(unsigned int seconds);

/**
 * @brief Suspends the execution of calling thread until time specified by microseconds.
 * @param[in] Time in microseconds for which execution is to be suspendend.
 * 
 * @since 1.15
 */
EAPI void evil_usleep(unsigned int seconds);

/**
 * @brief Changes the ownership of the file specified by pathname.
 * @param[in] The directory path to change owner.
 * @param[in] The new user to set.
 * @param[in] The new group to set.
 * 
 * @since 1.15
 */
EAPI int evil_chown(const char *pathname, uid_t owner, gid_t group);

/**
 * @brief Places the contents of the symbolic link pathname in the buffer buf, which has size bufsiz.
 * @param[in] The symbolic link pathname.
 * @param[out] The destination buffer to store pathname.
 * @param[in] The size of the destination buf.
 * @return Return the numbers of bytes placed in buf. If returned value equals bufsize, then truncation may have ocurred.
 * 
 * @since 1.15
 */
EAPI int evil_readlink(const char *pathname, char *buf, size_t bufsize);

/**
 * @brief Creates a new session if the calling process is not a process group leader.
 * @return On success, the (new) session ID of the calling process is returned. On error, 
 * (pid_t) -1 is returned, and errno is set to indicate the error.
 * 
 * @since 1.15
 */
EAPI int evil_setsid(void);

/**
 * @}
 */


#endif /* __EVIL_UNISTD_H__ */
