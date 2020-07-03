#ifndef EFREET_TRASH_H
#define EFREET_TRASH_H

#include <efreet_api.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file Efreet_Trash.h
 * @brief Contains the methods used to support the FDO trash specification.
 * @addtogroup Efreet_Trash Efreet_Trash: The XDG Trash Specification
 * Efreet_Trash.h provides all of the necessary headers and includes to
 * work with Efreet_Trash.
 *
 * @ingroup Efreet
 * @{
 */

/**
 * @return @c 1 on success or @c 0 on failure.
 * @brief Initializes the efreet trash system
 */
EFREET_API int         efreet_trash_init(void);

/**
 * @return No value.
 * @brief Cleans up the efreet trash system
 */
EFREET_API int         efreet_trash_shutdown(void);

/**
 * @return The XDG Trash local directory or @c NULL on errors.
 * Return value must be freed with eina_stringshare_del.
 * @brief Retrieves the XDG Trash local directory
 */
EFREET_API const char *efreet_trash_dir_get(const char *for_file);

/**
 * @param uri The local uri to move in the trash
 * @param force_delete If you set this to @c 1 than files on different filesystems
 * will be deleted permanently
 * @return @c 1 on success, @c 0 on failure or @c -1 in case the uri is not on
 * the same filesystem and force_delete is not set.
 * @brief This function try to move the given uri to the trash. Files on
 * different filesystem can't be moved to trash. If force_delete
 * is @c 0 than non-local files will be ignored and @c -1 is returned, if you set
 * force_delete to @c 1 non-local files will be deleted without asking.
 */
EFREET_API int         efreet_trash_delete_uri(Efreet_Uri *uri, int force_delete);

/**
 * @return A list of strings with filename (remember to free the list
 * when you don't need anymore).
 * @brief List all the files and directory currently inside the trash.
 */
EFREET_API Eina_List  *efreet_trash_ls(void);

/**
 * @return @c 1 if the trash is empty or @c 0 if some file are in.
 * @brief Check if the trash is currently empty
 */
EFREET_API int         efreet_trash_is_empty(void);

/**
 * @return @c 1 on success or @c 0 on failure.
 * @brief Delete all the files inside the trash.
 */
EFREET_API int         efreet_trash_empty_trash(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
