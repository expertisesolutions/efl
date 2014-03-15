#include <Emodel.h>
#include <Eina.h>
#include <emodel_eio.h>

EAPI Eo_Op EMODEL_OBJ_EIO_BASE_ID = EO_NOOP;

#define MY_CLASS EMODEL_OBJ_EIO_CLASS
#define MY_CLASS_NAME "Emodel_Eio_Class"

struct _Emodel_Eio
{
   //TODO: implement this
};

typedef struct _Emodel_Eio Emodel_Eio;


static void
_emodel_eio_constructor(Eo *obj , void *class_data, va_list *list)
{
}

static void
_emodel_eio_destructor(Eo *obj , void *class_data, va_list *list)
{
}

static void
_emodel_eio_properties_get(Eo *obj , void *class_data, va_list *list)
{
}

static void
_emodel_eio_property_get(Eo *obj , void *class_data, va_list *list)
{
}

static void
_emodel_eio_property_set(Eo *obj , void *class_data, va_list *list)
{
}

static void
_emodel_eio_load(Eo *obj , void *class_data, va_list *list)
{
}

static void
_emodel_eio_unload(Eo *obj , void *class_data, va_list *list)
{
}

static void
_emodel_eio_child_add(Eo *obj , void *class_data, va_list *list)
{
}


static void
_emodel_eio_children_get(Eo *obj , void *class_data, va_list *list)
{
}


static void
_emodel_eio_children_slice_get(Eo *obj , void *class_data, va_list *list)
{
}

static void
_emodel_eio_children_count_get(Eo *obj , void *class_data, va_list *list)
{
}

static void
_emodel_eio_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_descs[] = {
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _emodel_eio_destructor),
      EO_OP_FUNC(EMODEL_EIO_ID(EMODEL_OBJ_EIO_SUB_ID_CONSTRUCTOR), _emodel_eio_constructor),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_PROPERTIES_GET), _emodel_eio_properties_get),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_PROPERTY_GET), _emodel_eio_property_get),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_PROPERTY_SET), _emodel_eio_property_set),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_LOAD), _emodel_eio_load),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_UNLOAD), _emodel_eio_unload),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_CHILD_ADD), _emodel_eio_child_add),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_CHILDREN_GET), _emodel_eio_children_get),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_CHILDREN_SLICE_GET), _emodel_eio_children_slice_get),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_CHILDREN_COUNT_GET), _emodel_eio_children_count_get),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description op_desc[] = {
      EO_OP_DESCRIPTION_SENTINEL
};

static const Eo_Class_Description class_desc = {
     EO_VERSION,
     MY_CLASS_NAME,
     EO_CLASS_TYPE_REGULAR,
     EO_CLASS_DESCRIPTION_OPS(&EMODEL_OBJ_EIO_BASE_ID, op_desc, EMODEL_OBJ_EIO_SUB_ID_LAST),
     NULL,
     sizeof(Emodel_Eio),
     _emodel_eio_class_constructor,
     NULL
};

EO_DEFINE_CLASS(emodel_obj_eio_class_get, &class_desc, EO_BASE_CLASS, EMODEL_CLASS, NULL);
