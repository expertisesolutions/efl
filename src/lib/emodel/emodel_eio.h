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

#define EMODEL_EIO_CLASS emodel_obj_eio_class_get()
const Eo_Class *emodel_obj_eio_class_get(void) EINA_CONST;

extern EAPI Eo_Op EMODEL_EIO_BASE_ID;

enum
{
   EMODEL_OBJ_EIO_SUB_ID_CONSTRUCTOR,
   EMODEL_OBJ_EIO_SUB_ID_CHILD_ADD,
   EMODEL_OBJ_EIO_SUB_ID_CHILD_DEL,
   EMODEL_OBJ_EIO_SUB_ID_LAST,
};

enum _Emodel_Eio_Filetype
{
   EMODEL_EIO_FILE_TYPE_DIR,
   EMODEL_EIO_FILE_TYPE_FILE,
};

typedef enum _Emodel_Eio_Filetype Emodel_Eio_Filetype;

struct _Emodel_Eio_Public_Data
{
   int cb_count_child_add;
   int cb_count_child_del;
};

typedef struct _Emodel_Eio_Public_Data Emodel_Eio_Public_Data;

struct _Emodel_Child_Add
{
   Eo *child;
   const char *name;
   Emodel_Eio_Filetype filetype;
};

typedef struct _Emodel_Child_Add Emodel_Child_Add;


#define EMODEL_EIO_ID(sub_id) (EMODEL_EIO_BASE_ID + sub_id)

/**
 * @def emodel_eio_constructor
 * @since 1.9
 *
 * Contructor for Emodel.
 * @param[in] path to file
 *
 */
#define emodel_eio_constructor(path) EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(const char *, path)

/**
 * @def emodel_eio_child_add
 * @since 1.9
 *
 * Add new child.
 * @param[in] child_add_cb callback function
 * @param[in]  data unmodified user data
 *
 */
#define emodel_eio_child_add(child_add_cb, data) EMODEL_EIO_ID(EMODEL_OBJ_EIO_SUB_ID_CHILD_ADD), EO_TYPECHECK(Emodel_Cb , child_add_cb), EO_TYPECHECK(void *, data)

/**
 * @def emodel_eio_child_del
 * @since 1.9
 *
 * Delete child.
 *
 *
 */
//#define emodel_eio_child_del(child_add_cb, data) EMODEL_EIO_ID(EMODEL_OBJ_EIO_SUB_ID_CHILD_ADD), EO_TYPECHECK(Emodel_Cb , child_add_cb), EO_TYPECHECK(void *, data)

#ifdef __cplusplus
}
#endif
#endif //_EMODEL_EIO_H
