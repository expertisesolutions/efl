/* EINA - EFL data type library
 * Copyright (C) 2007-2008 Jorge Luis Zapata Muga
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EINA_MODULE_H_
#define EINA_MODULE_H_

#include "eina_types.h"
#include "eina_array.h"
#include "eina_error.h"
#include "eina_inline_modinfo.x"

/**
 * @defgroup Eina_Module_Group Module
 * @ingroup Eina_Tools_Group Tools
 * @brief These functions provide module management.
 *
 * Eina module provides some helpers over POSIX dlopen(). It is not
 * meant to replace, abstract or make a "portable" version of
 * POSIX, but enhance its usage by defining some good practices.
 *
 * Modules are created with eina_module_new() and later loaded with
 * eina_module_load(). Loads are reference counted and there must be
 * the same number of eina_module_unload() in order to have it to call
 * dlclose(). This makes simple to have different users for the same
 * module.
 *
 * The loaded shared objects may have two visible functions that will
 * be called and might provide initialization and shutdown
 * procedures. The symbols are @c __eina_module_init and
 * @c __eina_module_shutdown and will be defined by the macros
 * EINA_MODULE_INIT() and EINA_MODULE_SHUTDOWN().
 *
 * There are some helpers to automatically create modules based on
 * directory listing. See eina_module_arch_list_get(),
 * eina_module_list_get() and eina_module_find().
 *
 * #EINA_MODULE_LICENSE, #EINA_MODULE_AUTHOR, #EINA_MODULE_VERSION and
 * #EINA_MODULE_DESCRIPTION allow you to define module information that can
 * be retrieved with the @c eina_modinfo tool.
   @code{.sh}
   $ eina_modinfo module.so
   version: 0.1
   description:   Entry test
   license: GPLv2
   author:  Enlightenment Community
   @endcode
 * @{
 */

/**
 * @typedef Eina_Module
 * Dynamic module loader handle.
 */
typedef struct _Eina_Module Eina_Module;

/**
 * @typedef Eina_Module_Cb
 * Dynamic module loader callback.
 */
typedef Eina_Bool         (*Eina_Module_Cb)(Eina_Module *m, void *data);

/**
 * @typedef Eina_Module_Init
 * If a function with this signature is exported by module as
 * __eina_module_init, it will be called on the first load after
 * dlopen() and if #EINA_FALSE is returned, load will fail, #EINA_TRUE
 * means the module was successfully initialized.
 * @see Eina_Module_Shutdown
 */
typedef Eina_Bool (*Eina_Module_Init)(void);

/**
 * @typedef Eina_Module_Shutdown
 * If a function with this signature is exported by module as
 * __eina_module_shutdown, it will be called before calling dlclose()
 * @see Eina_Module_Init
 */
typedef void (*Eina_Module_Shutdown)(void);

/**
 * @def EINA_MODULE_INIT
 * declares the given function as the module initializer (__eina_module_init).
 * It must be of signature #Eina_Module_Init
 */
#define EINA_MODULE_INIT(f) EXPORTAPI Eina_Module_Init __eina_module_init = &f

/**
 * @def EINA_MODULE_SHUTDOWN
 * declares the given function as to be called on shutdown
 * (__eina_module_shutdown). It must be of signature #Eina_Module_Shutdown
 */
#define EINA_MODULE_SHUTDOWN(f) EXPORTAPI Eina_Module_Shutdown __eina_module_shutdown = &f

EAPI Eina_Error EINA_ERROR_WRONG_MODULE;
EAPI Eina_Error EINA_ERROR_MODULE_INIT_FAILED;

/**
 * @brief Returns a new module.
 *
 * @param[in] file The name of the file module to load.
 * @return A new module. If @p file is @c NULL, or if it does not exist,
 * the function returns @c NULL, otherwise, it allocates an Eina_Module,
 * stores a duplicate string of @p file, sets its reference to @c 0 and
 * its handle to @c NULL.
 *
 * When the new module is not needed anymore, use eina_module_free()
 * to free the allocated memory.
 *
 * @see eina_module_load
 */
EAPI Eina_Module *
 eina_module_new(const char *file) EINA_MALLOC EINA_WARN_UNUSED_RESULT EINA_ARG_NONNULL(1);

/**
 * @brief Deletes a module.
 *
 * @param[in,out] module The module to delete.
 * @return #EINA_TRUE on success, #EINA_FALSE otherwise.
 *
 * This function calls eina_module_unload() if @p module has been previously
 * loaded and frees the allocated memory. On success this function
 * returns #EINA_TRUE and #EINA_FALSE otherwise. If @p module is @c NULL, the
 * function returns immediately.
 */
EAPI Eina_Bool
 eina_module_free(Eina_Module *module) EINA_ARG_NONNULL(1);

/**
 * @brief Loads a module.
 *
 * @param[in,out] module The module to load.
 * @return #EINA_TRUE on success, #EINA_FALSE otherwise.
 *
 * This function load the shared file object passed in
 * eina_module_new(). If it is a internal Eina module (like the
 * mempools), it also initializes it. If the shared file object cannot
 * be loaded, #EINA_FALSE is returned. If it is an internal Eina module and the
 * module cannot be initialized, #EINA_FALSE is returned. If the module has
 * already been loaded, its reference counter is increased by one and
 * #EINA_TRUE is returned. If @p module is @c NULL, the function returns
 * immediately #EINA_FALSE.
 *
 * When the symbols of the shared file objects are not needed
 * anymore, call eina_module_unload() to unload the module.
 */
EAPI Eina_Bool
 eina_module_load(Eina_Module *module) EINA_ARG_NONNULL(1);

/**
 * @brief Unloads a module.
 *
 * @param[in,out] module The module to load.
 * @return #EINA_TRUE on success, #EINA_FALSE otherwise.
 *
 * This function unload the module @p module that has been previously
 * loaded by eina_module_load(). If the reference counter of @p module is
 * strictly greater than @c 1, #EINA_FALSE is returned. Otherwise, the
 * shared object file is closed and if it is a internal Eina module, it
 * is shut down just before. In that case, #EINA_TRUE is
 * returned. In all case, the reference counter is decreased. If @p module
 * is @c NULL, the function returns immediately #EINA_FALSE.
 */
EAPI Eina_Bool
 eina_module_unload(Eina_Module *module) EINA_ARG_NONNULL(1);

/**
 * @brief Retrieves the data associated with a symbol.
 *
 * @param[in] module The module.
 * @param[in] symbol The symbol.
 * @return The data associated with the symbol, or @c NULL on failure.
 *
 * This function returns the data associated with @p symbol of @p module. @p
 * module must have been loaded before with eina_module_load(). If @p module
 * is @c NULL, or if it has not been correctly loaded before, the
 * function returns immediately @c NULL.
 */
EAPI void *
 eina_module_symbol_get(const Eina_Module *module, const char *symbol) EINA_PURE EINA_ARG_NONNULL(1, 2) EINA_WARN_UNUSED_RESULT;

/**
 * @brief Returns the file name associated with the module.
 *
 * @param[in] module The module.
 * @return The file name.
 *
 * This function returns the file name passed in eina_module_new(). If
 * @p module is @c NULL, the function returns immediately @c NULL. The
 * returned value must no be freed.
 */
EAPI const char *
 eina_module_file_get(const Eina_Module *module) EINA_PURE EINA_WARN_UNUSED_RESULT EINA_ARG_NONNULL(1);

/**
 * @brief Defines if on module load we should expose all symbol
 *
 * @param[in,out] module The module to turn off/on symbol to be exposed
 * @param[in] global @c true to turn on symbol to be exposed, @c false otherwise
 *
 * @since 1.11
 */
EAPI void eina_module_symbol_global_set(Eina_Module *module, Eina_Bool global) EINA_ARG_NONNULL(1);

/**
 * @brief Returns the path built from the location of a library and a
 * given sub directory.
 *
 * @param[in] symbol The symbol to search for.
 * @param[in] sub_dir The subdirectory to append.
 * @return The built path on success, @c NULL otherwise.
 *
 * This function returns the path built by concatenating the path of
 * the library containing the symbol @p symbol and @p sub_dir. @p sub_dir
 * can be @c NULL. The returned path must be freed when not used
 * anymore. If the symbol is not found, or dl_addr() is not supported,
 * or allocation fails, this function returns @c NULL.
 */
EAPI char *
 eina_module_symbol_path_get(const void *symbol, const char *sub_dir) EINA_MALLOC EINA_ARG_NONNULL(1, 2);

/**
 * @brief Returns the path built from the value of an environment variable and a
 * given sub directory.
 *
 * @param[in] env The environment variable to expand.
 * @param[in] sub_dir The subdirectory to append.
 * @return The built path on success, @c NULL otherwise.
 *
 * This function returns the path built by concatenating the value of
 * the environment variable named @p env and @p sub_dir. @p sub_dir
 * can be @c NULL. The returned path must be freed when not used
 * anymore. If the symbol is not found, or @p env does not exist, or
 * allocation fails, this function returns @c NULL.
 */
EAPI char *
 eina_module_environment_path_get(const char *env, const char *sub_dir) EINA_MALLOC EINA_ARG_NONNULL(1, 2);


/**
 * @brief Gets an array of modules found on the directory path matching an arch type.
 *
 * @param[in,out] array The array that stores the list of the modules.
 * @param[in] path The directory's path to search for modules.
 * @param[in] arch The architecture string.
 * @return The array of modules found in @p path matching @p arch.
 *
 * This function adds to @p array the module names found in @p path
 * which match the cpu architecture @p arch. If @p path or @p arch is
 * @c NULL, the function returns immediately @p array. @p array can be
 * @c NULL. In that case, it is created with 4 elements.
 */
EAPI Eina_Array *
 eina_module_arch_list_get(Eina_Array *array, const char *path, const char *arch);

/**
 * @brief Gets a list of modules found on the directory path.
 *
 * @param[in,out] array The array that stores the list of the modules.
 * @param[in] path The directory's path to search for modules.
 * @param[in] recursive Iterate recursively on the path.
 * @param[in] cb Callback function to call on each module.
 * @param[in] data Data passed to the callback function.
 * @return The array of modules found in @p path.
 *
 * This function adds to @p array the list of modules found in
 * @p path. If @p recursive is #EINA_TRUE, then recursive search is
 * done. The callback @p cb is called on each module found, and @p data
 * is passed to @p cb. If @p path is @c NULL, the function returns
 * immediately @p array. If the returned value of @p cb is @c 0, the
 * module will not be added to the list, otherwise it will be added.
 * @p array can be @c NULL. In that case, it is created with 4
 * elements. @p cb can be @c NULL.
 */
EAPI Eina_Array *
 eina_module_list_get(Eina_Array *array, const char *path, Eina_Bool recursive, Eina_Module_Cb cb, void *data) EINA_MALLOC EINA_WARN_UNUSED_RESULT;

/**
 * @brief Loads every module on the list of modules.
 *
 * @param[in,out] array The array of modules to load.
 *
 * This function calls eina_module_load() on each element found in
 * @p array. If @p array is @c NULL, this function does nothing.
 */
EAPI void
 eina_module_list_load(Eina_Array *array) EINA_ARG_NONNULL(1);

/**
 * @brief Unloads every module on the list of modules.
 *
 * @param[in,out] array The array of modules to unload.
 *
 * This function calls eina_module_unload() on each element found in
 * @p array. If @p array is @c NULL, this function does nothing.
 */
EAPI void
 eina_module_list_unload(Eina_Array *array) EINA_ARG_NONNULL(1);

/**
 * @p Frees every module on the list of modules.
 *
 * @param[in,out] array The array of modules to free.
 *
 * This function calls eina_module_free() on each element found in
 * @p array. If @p array is @c NULL, this function does nothing.
 */
EAPI void
 eina_module_list_free(Eina_Array *array) EINA_ARG_NONNULL(1);

/**
 * @brief Finds a module in array.
 *
 * @param[in] array The array to find the module.
 * @param[in] module The name of module to be searched.
 * @return The module to find on success, @c NULL otherwise.
 *
 * This function finds a @p module in @p array.
 * If the element is found  the function returns the module, else
 * @c NULL is returned.
 */
EAPI Eina_Module *
 eina_module_find(const Eina_Array *array, const char *module) EINA_ARG_NONNULL(1, 2);

/**
 * @}
 */

#endif /*EINA_MODULE_H_*/
