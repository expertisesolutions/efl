/**
 * @page emodel_eio_main Emodel_Eio
 *
 * @date 2014 (created)
 *
 * @brief Emodel_Eio Library Public API Calls
 *
 * @section toc Table of Contents
 *
 * @li @ref emodel_eio_main_intro
 *
 * @section eo_main_intro Introduction
 *
 * This module targets file operations using Emodel.

 *
 * Recommended reading:
 *
 * @li @ref Emodel
 * @li @ref Eo
 * @li @ref Eina
 *
 * @defgroup Emodel_Eio
 *

 *
 *
 * @addtogroup Emodel_Eio
 * @{
 */

#ifndef _EMODEL_EIO_H
#define _EMODEL_EIO_H

#include <Eo.h>
#include <Emodel.h>
#include <Eio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EMODEL_EIO_CLASS emodel_obj_eio_class_get()
const Eo_Class *emodel_obj_eio_class_get(void) EINA_CONST;

extern EAPI Eo_Op EMODEL_EIO_BASE_ID;

enum
{
   EMODEL_EIO_OBJ_SUB_ID_CONSTRUCTOR,
   EMODEL_EIO_OBJ_SUB_ID_DIR_ADD,
   EMODEL_EIO_OBJ_SUB_ID_CHILD_DEL,
   EMODEL_EIO_OBJ_SUB_ID_CHILDREN_FILTER_SET,
   EMODEL_EIO_OBJ_SUB_ID_LAST,
};

enum _Emodel_Eio_Filetype
{
   EMODEL_EIO_FILE_TYPE_DIR,
   EMODEL_EIO_FILE_TYPE_FILE,
};

typedef enum _Emodel_Eio_Filetype Emodel_Eio_Filetype;

#define EMODEL_EIO_ID(sub_id) (EMODEL_EIO_BASE_ID + sub_id)

/**
 * @def emodel_eio_constructor
 * @since 1.10
 *
 * Constructor for Emodel_Eio
 *
 * @param[in] path constructor
 *
 */
#define emodel_eio_constructor(path) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(const char *, path)

/**
 * @def emodel_eio_dir_add
 * @since 1.10
 *
 * Create new empty directory
 *
 * @param[in] child_add_cb
 * @param[in] name 
 *
 */
#define emodel_eio_dir_add(child_add_cb, name) EMODEL_EIO_ID(EMODEL_EIO_OBJ_SUB_ID_DIR_ADD), EO_TYPECHECK(Emodel_Cb , child_add_cb), EO_TYPECHECK(const char *, name)

//new
//#define emodel_eio_file_add(child_add_cb, name) EMODEL_EIO_ID(EMODEL_EIO_OBJ_SUB_ID_DIR_ADD), EO_TYPECHECK(Emodel_Cb , child_add_cb), EO_TYPECHECK(const char *, name)

/**
 * @def emodel_eio_child_del
 * @since 1.10
 *
 * Delete child
 *
 * @param[in] child_del_cb
 *
 */
#define emodel_eio_child_del(child_del_cb) EMODEL_EIO_ID(EMODEL_EIO_OBJ_SUB_ID_CHILD_DEL), EO_TYPECHECK(Emodel_Cb, child_del_cb)

/**
 * @def emodel_eio_children_filter_set
 * @since 1.10
 *
 * Set a children filter callback
 *
 * @param[in] filter_cb
 * @param[in] userdata
 *
 */
#define emodel_eio_children_filter_set(filter_cb, userdata) EMODEL_EIO_ID(EMODEL_EIO_OBJ_SUB_ID_CHILDREN_FILTER_SET), EO_TYPECHECK(Eio_Filter_Direct_Cb, filter_cb), EO_TYPECHECK(void *, userdata)


/**
 * @}
 */


#ifdef __cplusplus
}
#endif
#endif //_EMODEL_EIO_H
