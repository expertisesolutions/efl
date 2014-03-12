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
#define EMODEL_OBJ_CLASS emodel_obj_class_get()
const Eo_Class *emodel_obj_class_get(void) EINA_CONST;
extern EAPI Eo_Op EMODEL_OBJ_BASE_ID;

enum
{
   EMODEL_OBJ_SUB_ID_CONSTRUCTOR,
   EMODEL_OBJ_SUB_ID_PROPERTIES_GET,
   EMODEL_OBJ_SUB_ID_PROPERTY_GET,
   EMODEL_OBJ_SUB_ID_PROPERTY_SET,
   EMODEL_OBJ_SUB_ID_LOAD,
   EMODEL_OBJ_SUB_ID_UNLOAD,
   EMODEL_OBJ_SUB_ID_CHILD_ADD,
   EMODEL_OBJ_SUB_ID_CHILDREN_GET,
   EMODEL_OBJ_SUB_ID_CHILDREN_SLICE_GET,
   EMODEL_OBJ_SUB_ID_CHILDREN_COUNT_GET,
   EMODEL_OBJ_SUB_ID_LAST,
};

#define EMODEL_OBJ_ID(sub_id) (EMODEL_OBJ_BASE_ID + sub_id)

/**
 * @def emodel_constructor
 * @since 1.9
 *
 * Contructor for Emodel.
 *
 */
#define emodel_constructor() EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_CONSTRUCTOR)

/**
 * @def emodel_properties_get
 * @since 1.9
 *
 */
#define emodel_properties_get() EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_PROPERTIES_GET)

/**
 * @def emodel_property_get
 * @since 1.9
 *
 */
#define emodel_property_get() EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_PROPERTY_GET)

/**
 * @def emodel_property_set
 * @since 1.9
 *
 */
#define emodel_property_set() EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_PROPERTY_SET)

/**
 * @def emodel_load
 * @since 1.9
 *
 */
#define emodel_load() EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_LOAD)

/**
 * @def emodel_unload
 * @since 1.9
 *
 */
#define emodel_unload() EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_UNLOAD)

/**
 * @def emodel_child_add
 * @since 1.9
 *
 */
#define emodel_child_add() EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_CHILD_ADD)

/**
 * @def emodel_children_get
 * @since 1.9
 *
 */
#define emodel_children_get() EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_CHILDREN_GET)

/**
 * @def emodel_children_slice_get
 * @since 1.9
 *
 */
#define emodel_children_slice_get(start, count) EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_CHILDREN_SLICE_GET), EO_TYPECHECK(int, start), EO_TYPECHECK(int, cont)

/**
 * @def emodel_children_count_get
 * @since 1.9
 *
 */
#define emodel_children_count_get() EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_CHILDREN_COUNT_GET)

extern const Eo_Event_Description _EMODEL_PROPERTIES_CHANGE_EVT;
#define EMODEL_PROPERTIES_CHANGE_EVT (&(_EMODEL_PROPERTIES_CHANGE_EVT))

extern const Eo_Event_Description _EMODEL_PROPERTY_CHANGE_EVT;
#define EMODEL_PROPERTY_CHANGE_EVT (&(_EMODEL_PROPERTY_CHANGE_EVT))

extern const Eo_Event_Description _EMODEL_CHILD_ADD_EVT;
#define EMODEL_CHILD_ADD_EVT (&(_EMODEL_CHILD_ADD_EVT))

extern const Eo_Event_Description _EMODEL_CHILD_DEL_EVT;
#define EMODEL_CHILD_DEL_EVT (&(_EMODEL_CHILD_DEL_EVT))

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif
