/**
   @brief Emodel Library Public API Calls

   These routines are used for Emodel Library interaction
 */

/**

   @page emodel_main Emodel EIO

   @date 2014 (created)

   @section toc Table of Contents

 */

#ifndef _EMODEL_EIO_H
#define _EMODEL_EIO_H

#include <Eo.h>
#include <Emodel.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EMODEL_OBJ_EIO_CLASS emodel_obj_eio_class_get()
const Eo_Class *emodel_obj_eio_class_get(void) EINA_CONST;

extern EAPI Eo_Op EMODEL_OBJ_EIO_BASE_ID;

enum
{
   EMODEL_OBJ_EIO_SUB_ID_CONSTRUCTOR,
   EMODEL_OBJ_EIO_SUB_ID_CHILD_ADD,
   EMODEL_OBJ_EIO_SUB_ID_LAST,
};

enum _Emodel_Eio_Filetype
{
   EMODEL_EIO_FILE_TYPE_DIR,
   EMODEL_EIO_FILE_TYPE_FILE,
};

typedef enum _Emodel_Eio_Filetype Emodel_Eio_Filetype;


#define EMODEL_EIO_ID(sub_id) (EMODEL_OBJ_EIO_BASE_ID + sub_id)

/**
 * @def emodel_constructor
 * @since 1.9
 *
 * Contructor for Emodel.
 * @param[in] path to file
 *
 */
#define emodel_eio_constructor(path) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(const char *, path)

// TODO/FIXME/XXX: Shouldn't we be using EMODEL_EIO_ID and EMODEL_OBJ_EIO_SUB_ID_CHILD_ADD
#define emodel_eio_child_add(child_add_cb, data, name, filetype) EMODEL_ID(EMODEL_SUB_ID_CHILD_ADD), EO_TYPECHECK(Emodel_Child_Add_Cb *, child_add_cb), EO_TYPECHECK(void *, data), EO_TYPECHECK(const char *, name), EO_TYPECHECK(Emodel_Eio_Filetype, filetype)

#ifdef __cplusplus
}
#endif
#endif //_EMODEL_EIO_H
