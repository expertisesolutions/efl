/**
   @brief Emodel Library Public API Calls

   These routines are used for Emodel Library interaction
 */

/**

   @page emodel_main Emodel

   @date 2014 (created)

   @section toc Table of Contents

 */

#ifndef _EMODEL_H
#define _EMODEL_H

#include <Eo.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup Emode
 *
 * @{
 */
#define EMODEL_CLASS emodel_class_get()
const Eo_Class *emodel_class_get(void) EINA_CONST;
extern EAPI Eo_Op EMODEL_BASE_ID;

enum
{
   EMODEL_SUB_ID_CONSTRUCTOR,
   EMODEL_SUB_ID_PROPERTIES_GET,
   EMODEL_SUB_ID_PROPERTY_GET,
   EMODEL_SUB_ID_PROPERTY_SET,
   EMODEL_SUB_ID_LOAD,
   EMODEL_SUB_ID_UNLOAD,
   EMODEL_SUB_ID_CHILD_ADD,
   EMODEL_SUB_ID_CHILDREN_GET,
   EMODEL_SUB_ID_CHILDREN_SPLIT_GET,
   EMODEL_SUB_ID_CHILDREN_COUNT_GET,
   EMODEL_SUB_ID_LAST,
};

#define EMODEL_ID(sub_id) (EMODEL_BASE_ID + sub_id)

/**
 * @def emodel_constructor
 * @since 1.9
 *
 * Contructor with parameters for Emodel.
 *
 *
 */
#define emodel_constructor() EMODEL_ID(EMODEL_SUB_ID_CONSTRUCTOR)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
