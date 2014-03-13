#include <Emodel.h>

EAPI Eo_Op EMODEL_OBJ_BASE_ID = EO_NOOP;

#define MY_CLASS EMODEL_OBJ_CLASS
#define MY_CLASS_NAME "Emodel"

struct _E_Model
{
   // TODO: Implement this
};

typedef struct _E_Model E_Model;

EAPI const Eo_Event_Description _EMODEL_PROPERTIES_CHANGE_EVT =
   EO_EVENT_DESCRIPTION("emodel, properties change","List properties.");

EAPI const Eo_Event_Description _EMODEL_PROPERTY_CHANGE_EVT =
   EO_EVENT_DESCRIPTION("emodel, property change","Property value is available.");

EAPI const Eo_Event_Description _EMODEL_CHILD_ADD_EVT =
   EO_EVENT_DESCRIPTION("emodel, child add","New child is added.");

EAPI const Eo_Event_Description _EMODEL_CHILD_DEL_EVT =
   EO_EVENT_DESCRIPTION("emodel, child del","Child was removed.");

static void
_emodel_constructor(Eo *obj, void *class_data, va_list *list)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());
}

static void
_emodel_properties_get(Eo *obj, void *class_data, va_list *list)
{

}

static void
_emodel_property_get(Eo *obj, void *class_data, va_list *list)
{

}

static void
_emodel_property_set(Eo *obj, void *class_data, va_list *list)
{

}

static void
_emodel_load(Eo *obj, void *class_data, va_list *list)
{

}

static void
_emodel_unload(Eo *obj, void *class_data, va_list *list)
{

}

static void
_emodel_child_add(Eo *obj, void *class_data, va_list *list)
{

}

static void
_emodel_children_get(Eo *obj, void *class_data, va_list *list)
{

}

static void
_emodel_children_slice_get(Eo *obj, void *class_data, va_list *list)
{

}

static void
_emodel_children_count_get(Eo *obj, void *class_data, va_list *list)
{

}

static void
_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_desc[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _emodel_constructor),
      EO_OP_FUNC(EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_PROPERTIES_GET), _emodel_properties_get),
      EO_OP_FUNC(EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_PROPERTY_GET), _emodel_property_get),
      EO_OP_FUNC(EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_PROPERTY_SET), _emodel_property_set),
      EO_OP_FUNC(EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_LOAD), _emodel_load),
      EO_OP_FUNC(EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_UNLOAD), _emodel_unload),
      EO_OP_FUNC(EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_CHILD_ADD), _emodel_child_add),
      EO_OP_FUNC(EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_CHILDREN_GET), _emodel_children_get),
      EO_OP_FUNC(EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_CHILDREN_SLICE_GET), _emodel_children_slice_get),
      EO_OP_FUNC(EMODEL_OBJ_ID(EMODEL_OBJ_SUB_ID_CHILDREN_COUNT_GET), _emodel_children_count_get),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_desc);
}

static const Eo_Op_Description op_desc[] = {
      EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_PROPERTIES_GET, ""),
      EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_PROPERTY_GET, ""),
      EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_PROPERTY_SET, ""),
      EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_LOAD, ""),
      EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_UNLOAD, ""),
      EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_CHILD_ADD, ""),
      EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_CHILDREN_GET, ""),
      EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_CHILDREN_SLICE_GET, ""),
      EO_OP_DESCRIPTION(EMODEL_OBJ_SUB_ID_CHILDREN_COUNT_GET, ""),
      EO_OP_DESCRIPTION_SENTINEL
};

static const Eo_Event_Description *event_descs[] = {
   EMODEL_PROPERTIES_CHANGE_EVT,
   EMODEL_PROPERTY_CHANGE_EVT,
   EMODEL_CHILD_ADD_EVT,
   EMODEL_CHILD_DEL_EVT,
   NULL
};

static const Eo_Class_Description class_desc = {
     EO_VERSION,
     MY_CLASS_NAME,
     EO_CLASS_TYPE_REGULAR,
     EO_CLASS_DESCRIPTION_OPS(&EMODEL_OBJ_BASE_ID, op_desc, EMODEL_OBJ_SUB_ID_LAST),
     event_descs,
     sizeof(E_Model),
     _class_constructor,
     NULL
};

EO_DEFINE_CLASS(emodel_obj_class_get, &class_desc, EO_BASE_CLASS, NULL);
