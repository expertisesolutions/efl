#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Eo.h>
#include <Emodel.h>
#include <Eina.h>

#define MY_CLASS_NAME "Emodel"

struct _Emodel_Data
{
};
typedef struct _Emodel_Data Emodel_Data;

static void
_emodel_property_set(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED, const char *property EINA_UNUSED, Eina_Value *value EINA_UNUSED){}

static void
_emodel_property_get(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED, const char *property EINA_UNUSED){}

static void
_emodel_children_slice_get(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED, Emodel_Cb children_slice_get_cb EINA_UNUSED, int start EINA_UNUSED, int count EINA_UNUSED, void *data EINA_UNUSED){}

static void
_emodel_child_del(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED, Emodel_Cb child_del_cb EINA_UNUSED, Eo *child EINA_UNUSED){}

static void
_emodel_properties_get(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED){}

static void
_emodel_load(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED){}

static void
_emodel_unload(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED){}

static void
_emodel_children_get(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED, Emodel_Cb children_get_cb EINA_UNUSED, void *data EINA_UNUSED){}

static void
_emodel_children_count_get(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED){}

static void
_emodel_child_select_set(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED, Eo *child EINA_UNUSED){}

static void
_emodel_child_select_get(Eo *obj EINA_UNUSED, Emodel_Data *pd EINA_UNUSED){}

/* EAPI Eo_Op EMODEL_BASE_ID = EO_NOOP; */

/* #define MY_CLASS EMODEL_CLASS */
/* #define MY_CLASS_NAME "Emodel" */


/* typedef struct _E_Model E_Model; */

/* EAPI const Eo_Event_Description _EMODEL_PROPERTIES_CHANGE_EVT = */
/*    EO_EVENT_DESCRIPTION("emodel, properties change","Properties list is available."); */

/* EAPI const Eo_Event_Description _EMODEL_PROPERTY_CHANGE_EVT = */
/*    EO_EVENT_DESCRIPTION("emodel, property change","Property value is available."); */

/* EAPI const Eo_Event_Description _EMODEL_CHILD_ADD_EVT = */
/*    EO_EVENT_DESCRIPTION("emodel, child add","New child is added."); */

/* EAPI const Eo_Event_Description _EMODEL_CHILD_DEL_EVT = */
/*    EO_EVENT_DESCRIPTION("emodel, child del","Child was removed."); */

/* EAPI const Eo_Event_Description _EMODEL_CHILDREN_GET_EVT = */
/*    EO_EVENT_DESCRIPTION("emodel, children get","Children available."); */

/*    EAPI const Eo_Event_Description _EMODEL_CHILDREN_COUNT_GET_EVT = */
/*    EO_EVENT_DESCRIPTION("emodel, children count get","Children count available."); */

/* static const Eo_Op_Description op_desc[] = { */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_PROPERTIES_GET, "Get list of properties."), */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_PROPERTY_GET, "Get value from given property."), */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_PROPERTY_SET, "Set property value."), */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_LOAD, "Load emodel"), */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_UNLOAD, "Unload emodel"), */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_CHILD_ADD, "Add new child."), */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_CHILD_DEL, "Del child."), */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_CHILDREN_GET, "Get all children."), */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_CHILDREN_SLICE_GET, "Get children slice."), */
/*       EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_CHILDREN_COUNT_GET, "Count number of children."), */
/*       EO_OP_DESCRIPTION_SENTINEL */
/* }; */

/* static const Eo_Event_Description *event_descs[] = { */
/*    EMODEL_PROPERTIES_CHANGE_EVT, */
/*    EMODEL_PROPERTY_CHANGE_EVT, */
/*    EMODEL_CHILD_ADD_EVT, */
/*    EMODEL_CHILD_DEL_EVT, */
/*    EMODEL_CHILDREN_GET_EVT, */
/*    EMODEL_CHILDREN_COUNT_GET_EVT, */
/*    NULL */
/* }; */

/* static const Eo_Class_Description class_desc = { */
/*      EO_VERSION, */
/*      MY_CLASS_NAME, */
/*      EO_CLASS_TYPE_INTERFACE, */
/*      EO_CLASS_DESCRIPTION_OPS(&EMODEL_BASE_ID, op_desc, EMODEL_OBJ_SUB_ID_LAST), */
/*      event_descs, */
/*      0, */
/*      NULL, */
/*      NULL */
/* }; */

/* EO_DEFINE_CLASS(emodel_class_get, &class_desc, NULL, NULL); */

#include "emodel.eo.c"
