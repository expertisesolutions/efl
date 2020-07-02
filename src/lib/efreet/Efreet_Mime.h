#ifndef EFREET_MIME_H
#define EFREET_MIME_H

/**
 * @file Efreet_Mime.h
 * @brief The file that must be included by any project wishing to use
 * @addtogroup Efreet_Mime Efreet_Mime: The XDG Shared Mime Info standard
 * Efreet Mime is a library designed to help apps work with the
 * Freedesktop.org Shared Mime Info standard.
 * Efreet_Mime.h provides all of the necessary headers and
 * includes to work with Efreet_Mime.
 *
 * @ingroup Efreet
 * @{
 */

#include <efreet_api.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @return @c 1 on success or @c 0 on failure.
 * @brief Initializes the efreet mime settings
 */
EFREET_API int         efreet_mime_init(void);

/**
 * @return The number of times the init function has been called minus the
 * corresponding init call.
 * @brief Shuts down Efreet mime settings system if a balanced number of
 * init/shutdown calls have been made
 */
EFREET_API int         efreet_mime_shutdown(void);

/**
 * @param file The file to find the mime type
 * @return Mime type as a string or NULL if the file doesn't exist.
 * @brief Retrieve the mime type of a file
 * @note The return value of this function is not guaranteed to be stringshared.
 */
EFREET_API const char *efreet_mime_type_get(const char *file);

/**
 * @param file The file to check the mime type
 * @return Mime type as a string.
 * @brief Retrieve the mime type of a file using magic
 */
EFREET_API const char *efreet_mime_magic_type_get(const char *file);

/**
 * @param file The file to check the mime type
 * @return Mime type as a string.
 * @brief Retrieve the mime type of a file using globs
 */
EFREET_API const char *efreet_mime_globs_type_get(const char *file);

/**
 * @param file The file to check the mime type
 * @return Mime type as a string.
 * @brief Retrieve the special mime type of a file
 */
EFREET_API const char *efreet_mime_special_type_get(const char *file);

/**
 * @param file The file to check the mime type
 * @return Mime type as a string.
 * @brief Retrieve the fallback mime type of a file.
 */
EFREET_API const char *efreet_mime_fallback_type_get(const char *file);


/**
 * @param mime The name of the mime type
 * @param theme The name of the theme to search icons in
 * @param size The wanted size of the icon
 * @return Mime type icon path as a string.
 * @brief Retrieve the mime type icon for a file.
 */
EFREET_API const char *efreet_mime_type_icon_get(const char *mime, const char *theme,
                                           unsigned int size);

/**
 * @brief Clear mime icons mapping cache
 */
EFREET_API void efreet_mime_type_cache_clear(void);

/**
 * @brief Flush mime icons mapping cache
 *
 * Flush timeout is defined at compile time by
 * EFREET_MIME_ICONS_FLUSH_TIMEOUT
 */
EFREET_API void efreet_mime_type_cache_flush(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
