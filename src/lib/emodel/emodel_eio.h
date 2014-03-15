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
   EMODEL_OBJ_EIO_SUB_ID_LAST,
};

/**
 * @def emodel_constructor
 * @since 1.9
 *
 * Contructor for Emodel.
 * @param[in] path to file
 *
 */
#define emodel_eio_constructor(path) EMODEL_ID(EMODEL_SUB_ID_CONSTRUCTOR), EO_TYPECHECK(const char *, path)

#ifdef __cplusplus
}
#endif
#endif //_EMODEL_EIO_H
