#include <Emodel.h>
#include <Eina.h>
#include <emodel_eio.h>
#include <Eio.h>
#include <Eo.h>

EAPI Eo_Op EMODEL_OBJ_EIO_BASE_ID = EO_NOOP;

#define MY_CLASS EMODEL_OBJ_EIO_CLASS
#define MY_CLASS_NAME "Emodel_Eio_Class"

enum {
   EMODEL_EIO_PROP_FILENAME,
   EMODEL_EIO_PROP_IS_DIR,
   EMODEL_EIO_PROP_IS_LNK,
   EMODEL_EIO_PROP_SIZE,
   EMODEL_EIO_PROP_MTIME
};

struct _Emodel_Eio
{
   Eina_Value *properties;
   Eo *obj;
   Eio_File *file;
   Eina_Hash *hash;
   const char *path;
   const Eina_Stat *stat;
};

typedef struct _Emodel_Eio Emodel_Eio;

static void
_hash_stat_pro_set(Emodel_Eio *priv, int prop_id, int pvalue)
{
   Emodel_Property_EVT evt;

   EINA_SAFETY_ON_NULL_RETURN(priv);
   eina_value_array_get(priv->properties, prop_id, &evt.prop);
   evt.value = eina_hash_find(priv->hash, evt.prop);
   eina_value_set(evt.value, pvalue);

   eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
}

static void
_stat_done_cb(void *data, Eio_File *handler, const Eina_Stat *stat)
{
   Emodel_Eio *priv = data;
   priv->stat = stat;

   _hash_stat_pro_set(priv, EMODEL_EIO_PROP_IS_DIR, eio_file_is_dir(stat));
   _hash_stat_pro_set(priv, EMODEL_EIO_PROP_IS_LNK, eio_file_is_lnk(stat));
   _hash_stat_pro_set(priv, EMODEL_EIO_PROP_SIZE, eio_file_size(stat));
   _hash_stat_pro_set(priv, EMODEL_EIO_PROP_MTIME, eio_file_mtime(stat));
}

static void
_eio_progress_cb(void *data, Eio_File *handler, const Eio_Progress *info)
{
}

static void
_eio_error_cb(void *data, Eio_File *handler, int error)
{
   Emodel_Eio *priv = data;
}

static void
_eio_move_done_cb(void *data, Eio_File *handler)
{
   Emodel_Property_EVT evt;
   Emodel_Eio *priv = data;

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &evt.prop);
   evt.value = eina_hash_find(priv->hash, evt.prop);
   eina_value_set(evt.value, priv->path);
   eio_file_direct_stat(priv->path, _stat_done_cb, _eio_error_cb, priv);

   eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
}

static void
_emodel_free_data(void *data)
{
   eina_value_free(data);
}

static void
_emodel_eio_constructor(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Eio *priv = class_data;
   const char *prop, *path = va_arg(*list, const char *);
   priv->path = path;
   Eina_Value *v;
   int i;
   
   eo_do_super(obj, MY_CLASS, eo_constructor()); 

   priv->properties = eina_value_array_new(EINA_VALUE_TYPE_STRING, 5);
   eina_value_array_set(priv->properties, EMODEL_EIO_PROP_FILENAME, "filename");
   eina_value_array_set(priv->properties, EMODEL_EIO_PROP_IS_DIR, "is_dir");
   eina_value_array_set(priv->properties, EMODEL_EIO_PROP_IS_LNK, "is_lnk");
   eina_value_array_set(priv->properties, EMODEL_EIO_PROP_SIZE, "size");
   eina_value_array_set(priv->properties, EMODEL_EIO_PROP_MTIME, "mtime");

   priv->hash = eina_hash_string_small_new(_emodel_free_data);

   for (i = 0; eina_value_array_count(priv->properties) < i; i++)
   {
       switch(i) {
       case EMODEL_EIO_PROP_FILENAME:
          v = eina_value_new(EINA_VALUE_TYPE_STRING);
          eina_value_set(v, path);
          break;
       case EMODEL_EIO_PROP_MTIME:
          v = eina_value_new(EINA_VALUE_TYPE_TIMEVAL);
          break;
       default:
          v = eina_value_new(EINA_VALUE_TYPE_INT);
       }

       eina_value_array_get(priv->properties, i, &prop);
       eina_hash_add(priv->hash, prop, v);
   }

   priv->obj = obj;
   eio_init();
}

static void
_emodel_eio_destructor(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Eio *priv = class_data;
   eina_hash_free(priv->hash);
   eo_do_super(obj, MY_CLASS, eo_destructor()); 
}

static void
_emodel_eio_properties_get(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Eio *priv = class_data;
   eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTIES_CHANGE_EVT, priv->properties, NULL));
}

static void
_emodel_eio_property_get(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Property_EVT evt;
   Emodel_Eio *priv = class_data;
   const char *prop_arg = va_arg(*list, const char*);

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &evt.prop);
   if (!strcmp(prop_arg, evt.prop)) {
        evt.value = eina_hash_find(priv->hash, evt.prop);
        eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
        return;
   }

   priv->file = eio_file_direct_stat(priv->path, _stat_done_cb, _eio_error_cb, priv);
}


static void
_emodel_eio_property_set(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Eio *priv = class_data;
   const char *dest, *prop, *prop_arg;
   prop_arg = va_arg(*list, const char*);
   dest = va_arg(*list, const char*);

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &prop);
   if (!strcmp(prop_arg, prop)) {
      const char *src;
      Eina_Value *value = eina_hash_find(priv->hash, prop);
      eina_value_get(value, &src);
      priv->path = dest;
      priv->file = eio_file_move(src, dest, _eio_progress_cb, _eio_move_done_cb, _eio_error_cb, priv);
   }
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
      EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _emodel_eio_constructor),
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
