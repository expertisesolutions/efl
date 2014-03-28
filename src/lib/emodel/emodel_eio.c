#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

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

struct _Emodel_Eio_Item
{
   Eo *child;
   const char *path;
};

typedef struct _Emodel_Eio_Item Emodel_Eio_Item;


struct _Emodel_Eio
{
   Eina_Value *properties;
   Eo *obj;
   Eio_File *file;
   Eina_Hash *hash;
   Eina_List *list;
   const char *path;
   const Eina_Stat *stat;
};

typedef struct _Emodel_Eio Emodel_Eio;

struct _Emodel_Eio_Child_Add
{
   Emodel_Child_Add_Cb callback;
   void *data;
   const char *name;
   const char* fullpath;
   Emodel_Eio_Filetype filetype;
   Emodel_Eio *priv;
};

typedef struct _Emodel_Eio_Child_Add Emodel_Eio_Child_Add;

struct _Emodel_Eio_Children_Count
{
   Emodel_Eio *priv;
   size_t total;
};

typedef struct _Emodel_Eio_Children_Count Emodel_Eio_Children_Count;

struct _Emodel_Eio_Children_Slice
{
   Emodel_Eio *priv;
   Eo *child;
   int start;
   int count;
   int idx;
};

typedef struct _Emodel_Eio_Children_Slice Emodel_Eio_Children_Slice;

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
_eio_error_child_add_cb(void *data, Eio_File *handler, int error)
{
   //TODO: implement
}

static void
_eio_progress_cb(void *data, Eio_File *handler, const Eio_Progress *info)
{
   //TODO: implement
}

static void
_eio_error_cb(void *data, Eio_File *handler, int error)
{
   Emodel_Eio *priv = data;
}

static void
_eio_property_set_error_cb(void *data, Eio_File *handler, int error)
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
   eio_file_direct_stat(priv->path, _stat_done_cb, _eio_property_set_error_cb, priv);

   eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
}

static void
_eio_done_mkdir_cb(void *data, Eio_File *handler)
{
   Emodel_Eio_Child_Add *child = (Emodel_Eio_Child_Add *)data;

   child->callback(child->data, child->priv->obj, (void*)child->priv->path);
   eo_do(child->priv->obj, eo_event_callback_call(EMODEL_CHILD_ADD_EVT, child->data, NULL));
   free(child->fullpath);
   free(child);
}

static void
_eio_done_error_mkdir_cb(void *data, Eio_File *handler, int error)
{
}

/*
static void
_eio_done_open_cb(void *data, Eio_File *handler, Eina_File *file)
{
   Emodel_Eio_Child_Add *child = (Emodel_Eio_Child_Add *)data;

   child->callback(child->data, child->priv->obj, (void*)child->priv->path);
   eo_do(child->priv->obj, eo_event_callback_call(EMODEL_CHILD_ADD_EVT, child->data, NULL));
   free(child->fullpath);
   free(child);
}

static void
_eio_done_error_open_cb(void *data, Eio_File *handler, int error)
{
   //TODO: implement
}
*/

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

   priv->properties = eina_value_array_new(EINA_VALUE_TYPE_STRING, 0);
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_FILENAME, "filename");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_IS_DIR, "is_dir");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_IS_LNK, "is_lnk");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_SIZE, "size");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_MTIME, "mtime");

   priv->hash = eina_hash_string_small_new(_emodel_free_data);
   priv->list = NULL;

   for (i = 0; i < eina_value_array_count(priv->properties); i++)
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
   eio_shutdown();
   eo_do_super(obj, MY_CLASS, eo_destructor()); 
}

static void
_emodel_eio_properties_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list EINA_UNUSED)
{
   Emodel_Eio *priv = class_data;
   eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTIES_CHANGE_EVT, priv->properties, NULL));
}

static void
_emodel_eio_property_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Emodel_Property_EVT evt;
   Emodel_Eio *priv = class_data;
   const char *prop_arg = va_arg(*list, const char*);

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &evt.prop);
   if (!strncmp(prop_arg, evt.prop, strlen(evt.prop))) {
        evt.value = eina_hash_find(priv->hash, evt.prop);
        eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
        return;
   }

   priv->file = eio_file_direct_stat(priv->path, _stat_done_cb, _eio_error_cb, priv);
}


static void
_emodel_eio_property_set(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Emodel_Eio *priv = class_data;
   const char *dest, *prop, *prop_arg;
   prop_arg = va_arg(*list, const char*);
   Eina_Value *v = va_arg(*list, const char*);

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &prop);
   if (!strncmp(prop_arg, prop, strlen(prop))) {
      const char *src;
      Eina_Value *value = eina_hash_find(priv->hash, prop);
      eina_value_get(value, &src);
      eina_value_get(v, &dest);
      priv->path = dest;
      priv->file = eio_file_move(src, dest, _eio_progress_cb, _eio_move_done_cb, _eio_property_set_error_cb, priv);
   }
}

static void
_emodel_eio_load(Eo *obj , void *class_data, va_list *list)
{
   //TODO: implement
}

static void
_emodel_eio_unload(Eo *obj , void *class_data, va_list *list)
{
   //TODO: implement
}

static Eina_Bool 
_emodel_list_item_find(Eina_List *list, const char *path, size_t len)
{
   Eina_Iterator *it;
   void *data;

   EINA_SAFETY_ON_NULL_RETURN_VAL(list, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(path, EINA_FALSE);

   it = eina_list_iterator_new(list);
   EINA_SAFETY_ON_NULL_RETURN_VAL(it, EINA_FALSE);

   EINA_ITERATOR_FOREACH(it, data)
     {
        Emodel_Eio_Item *tmp = (Emodel_Eio_Item*)data;
        if(!strncmp(tmp->path, path, len))
          {
             eina_iterator_free(it);
             return EINA_TRUE;
          }
     }

   eina_iterator_free(it);
   return EINA_FALSE;
}

static void 
_emodel_add_item(Emodel_Eio *priv, Eina_List **list, const char *path, Eo *child, size_t len)
{
   Emodel_Eio_Item *item;
   Eina_Bool copy = EINA_FALSE;

   if((NULL == *list) ||
      (_emodel_list_item_find(*list, path, strlen(path)) == EINA_FALSE))
     {
        item = calloc(1, sizeof(Emodel_Eio_Item));
        EINA_SAFETY_ON_NULL_RETURN(item);
        item->child = child;
        item->path = path;
        *list = eina_list_append(*list, item);
     } 
}

//TODO
static void
_emodel_eio_child_add(Eo *obj , void *class_data, va_list *list)
{ 
   size_t len;
   Emodel_Eio_Child_Add *child = calloc(1, sizeof(Emodel_Eio_Child_Add));
   EINA_SAFETY_ON_NULL_RETURN(child);

   child->callback = va_arg(*list, Emodel_Child_Add_Cb);
   child->name = va_arg(*list, const char *);
   child->filetype = va_arg(*list, Emodel_Eio_Filetype);


   child->priv = class_data;

   EINA_SAFETY_ON_NULL_RETURN(child->name);

   len = strlen(child->priv->path) + strlen(child->name);

   switch(child->filetype)
     {
      case EMODEL_EIO_FILE_TYPE_DIR:
         {
            // len + '/' + '\0'
            child->fullpath = calloc(1, len+2);
            mode_t mode = umask(0);
            umask(mode);

            //_eio_done_mkdir_cb frees memory
            strncpy(child->fullpath, child->priv->path, strlen(child->priv->path));
            strncat(child->fullpath, "/", 1);
            strncat(child->fullpath, child->name, strlen(child->name));
            eio_file_mkdir(child->fullpath, /* TODO check this */ (0777 - mode), 
                           _eio_done_mkdir_cb, _eio_done_error_mkdir_cb, child);
         }
         break;
      case EMODEL_EIO_FILE_TYPE_FILE:
         {
            fprintf(stdout,"EMODEL_EIO_FILE_TYPE_FILE: unimplemented\n"); 
            //eio_file_open(child->fullpath, 
            //               EINA_FALSE, _eio_done_open_cb, _eio_done_error_open_cb, child);
         }

         break;
      default:
         fprintf(stdout, "Invalid filetype");
         break;
     }
}

/**
 * Children Get
 */
static Eina_Bool 
_eio_filter_children_get_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   // filter everything
   fprintf(stdout, "path: %s\n", info->path);
   return EINA_TRUE;
}

//TODO:  Create new _EVT to inform we're child get?
static void
_eio_main_children_get_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   Emodel_Eio *priv = data;

   Eo *child = eo_add_custom(MY_CLASS, priv->obj, emodel_eio_constructor(info->path));
   eo_do(priv->obj, eo_event_callback_call(EMODEL_CHILDREN_GET_EVT, child, EINA_FALSE));
}


static void 
_eio_done_children_get_cb(void *data, Eio_File *handler)
{
   //TODO: Create new _EVT to inform we're done?
}

static void
_eio_error_children_get_cb(void *data, Eio_File *handler, int error)
{
   //TODO: Implement
}

static void
_emodel_eio_children_get(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Eio *priv = class_data;

   eio_file_direct_ls(priv->path, _eio_filter_children_get_cb, 
                      _eio_main_children_get_cb, _eio_done_children_get_cb, _eio_error_children_get_cb, priv);
}


/**
 * Children Slice Get
 */
static Eina_Bool 
_eio_filter_children_slice_get_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   // filter everything
   fprintf(stdout, "path: %s\n", info->path);
   return EINA_TRUE;
}

static void
_eio_main_children_slice_get_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   Emodel_Eio_Children_Slice *slice = data;
   
   if(slice->idx >= slice->start && slice->idx <= slice->count)
     {
        slice->child = eo_add_custom(MY_CLASS, slice->priv->obj, emodel_eio_constructor(slice->priv->path));
        _emodel_add_item(slice->priv, &slice->priv->list, slice->priv->path, slice->child, strlen(slice->priv->path));
     }

   if(slice->idx == slice->count)
         eo_do(slice->priv->obj, eo_event_callback_call(EMODEL_CHILDREN_SLICE_GET_EVT, slice->child, EINA_FALSE));

   slice->idx++;
}

static void 
_eio_done_children_slice_get_cb(void *data, Eio_File *handler)
{
   Emodel_Eio_Children_Slice *slice = data;
   free(slice);
}

static void
_eio_error_children_slice_get_cb(void *data, Eio_File *handler, int error)
{
   //TODO: Implement
}

static void
_emodel_eio_children_slice_get(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Eio_Children_Slice *slice = calloc(1, sizeof(Emodel_Eio_Children_Slice));

   slice->priv = class_data;
   slice->start = va_arg(*list, int);
   slice->count = va_arg(*list, int);
   slice->idx = 0;

   eio_file_direct_ls(slice->priv->path, _eio_filter_children_slice_get_cb, 
         _eio_main_children_slice_get_cb, _eio_done_children_slice_get_cb, _eio_error_children_slice_get_cb, slice);
}


/**
 * Children Count Get
 */
static Eina_Bool
_eio_filter_children_count_get_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(info->path, EINA_FALSE);
   return EINA_TRUE;
}

//TODO
static void
_eio_main_children_count_get_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   Emodel_Eio_Children_Count *count_data = (Emodel_Eio_Children_Count *)data;
   EINA_SAFETY_ON_NULL_RETURN(count_data);
   count_data->total++;
}


static void 
_eio_done_children_count_get_cb(void *data, Eio_File *handler)
{
   Emodel_Eio_Children_Count *count_data = (Emodel_Eio_Children_Count *)data;
   EINA_SAFETY_ON_NULL_RETURN(count_data);
   eo_do(count_data->priv->obj, eo_event_callback_call(EMODEL_CHILDREN_COUNT_GET_EVT, &(count_data->total), EINA_FALSE));
   free(count_data);
}

static void
_eio_error_children_count_get_cb(void *data, Eio_File *handler, int error)
{
}

static void
_emodel_eio_children_count_get(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Eio *priv = class_data;
   Emodel_Eio_Children_Count *count_data = calloc(1, sizeof(Emodel_Eio_Children_Count));
   count_data->priv = priv;

   eio_file_direct_ls(priv->path, _eio_filter_children_count_get_cb,
                      _eio_main_children_count_get_cb, _eio_done_children_count_get_cb,
                      _eio_error_children_count_get_cb, count_data);
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
      
      // TODO/FIXME/XXX: Shouldn't we be using EMODEL_EIO_ID and EMODEL_OBJ_EIO_SUB_ID_CHILD_ADD?
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
     EO_CLASS_DESCRIPTION_OPS(&EMODEL_OBJ_EIO_BASE_ID, op_desc, 0),
     NULL,
     sizeof(Emodel_Eio),
     _emodel_eio_class_constructor,
     NULL
};

EO_DEFINE_CLASS(emodel_obj_eio_class_get, &class_desc, EO_BASE_CLASS, EMODEL_CLASS, NULL);
