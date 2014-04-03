#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Emodel.h>
#include <Eina.h>
#include <emodel_eio.h>
#include <Eio.h>
#include <Eo.h>
#include <assert.h>

#define _MODEL_ASSERT
#ifdef _MODEL_ASSERT
#define _assert_ref(x) assert(x > 0)
#else
static inline void _assert_ref(int num){}
#endif

EAPI Eo_Op EMODEL_EIO_BASE_ID = EO_NOOP;

#define MY_CLASS EMODEL_EIO_CLASS
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
   const char *path;
   const Eina_Stat *stat;
};

typedef struct _Emodel_Eio Emodel_Eio;

struct _Emodel_Eio_Child_Add
{
   Emodel_Cb callback;
   char* fullpath;
   Emodel_Eio *priv;
   Emodel_Child_Add *user;
};

typedef struct _Emodel_Eio_Child_Add Emodel_Eio_Child_Add;
typedef struct _Emodel_Eio_Child_Add Emodel_Eio_Child_Del;

struct _Emodel_Eio_Children_Count
{
   Emodel_Eio *priv;
   size_t total;
};

typedef struct _Emodel_Eio_Children_Count Emodel_Eio_Children_Count;

struct _Emodel_Eio_Children_Data
{
   Emodel_Eio *priv;
   Eio_File *lsref;
   void *data;
   Emodel_Cb callback;
   Eina_Bool dispatch;
   int start;
   int count;
   int idx;
   int cidx;
};

typedef struct _Emodel_Eio_Children_Data Emodel_Eio_Children_Data;

//static Eina_Lock eio_mutex;
static void 
_emodel_dealloc_memory(void *ptr, ...)
{
   va_list al;
   void *data;
   //eina_lock_take(&eio_mutex);
   va_start(al,ptr);

   for(data = ptr; data != NULL; data = va_arg(al, void*))
     {
        free(data);
        data = NULL;
     }
   va_end(al);
   //eina_lock_release(&eio_mutex);
}

static void
_hash_stat_pro_set(Emodel_Eio *priv, int prop_id, int pvalue)
{
   Emodel_Property_EVT evt;

   EINA_SAFETY_ON_NULL_RETURN(priv);
   eina_value_array_get(priv->properties, prop_id, &evt.prop);
   evt.value = eina_hash_find(priv->hash, evt.prop);
   eina_value_set(evt.value, pvalue);

   _assert_ref(eo_ref_get(priv->obj));
   eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
}

static void
_stat_done_cb(void *data, Eio_File *handler EINA_UNUSED, const Eina_Stat *stat)
{
   Emodel_Eio *priv = data;
   priv->stat = stat;

   _hash_stat_pro_set(priv, EMODEL_EIO_PROP_IS_DIR, eio_file_is_dir(stat));
   _hash_stat_pro_set(priv, EMODEL_EIO_PROP_IS_LNK, eio_file_is_lnk(stat));
   _hash_stat_pro_set(priv, EMODEL_EIO_PROP_SIZE, eio_file_size(stat));
   _hash_stat_pro_set(priv, EMODEL_EIO_PROP_MTIME, eio_file_mtime(stat));
}

/*
static void
_eio_error_child_add_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
}
*/

static void
_eio_progress_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, const Eio_Progress *info EINA_UNUSED)
{
   //TODO: implement
}

static void
_eio_error_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
   fprintf(stdout, "[cancel] eio_file_direct_ls : %d\n", error);
}

static void
_eio_property_set_error_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
}

static void
_eio_move_done_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Property_EVT evt;
   Emodel_Eio *priv = data;

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &evt.prop);
   evt.value = eina_hash_find(priv->hash, evt.prop);
   eina_value_set(evt.value, priv->path);
   eio_file_direct_stat(priv->path, _stat_done_cb, _eio_property_set_error_cb, priv);

   _assert_ref(eo_ref_get(priv->obj));
   eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTY_CHANGE_EVT, &evt, NULL));
}

static void
_eio_done_mkdir_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Child_Add *_data = (Emodel_Eio_Child_Add *)data;
   Eo *parent = _data->priv->obj;

   _assert_ref(eo_ref_get(parent));

   /* save child object in userdata, callback can ignore this field */
   _data->user->child = eo_add_custom(MY_CLASS, parent, emodel_eio_constructor(_data->priv->path));

   /* dispatch callback for user */
   _data->callback(_data->user, parent, _data->user->child);
   
   /* also dispatch event for listeners */
   eo_do(_data->priv->obj, eo_event_callback_call(EMODEL_CHILD_ADD_EVT, _data->user, NULL));

   _emodel_dealloc_memory(_data->fullpath, _data->user, _data, NULL);
}

static void
_eio_done_error_mkdir_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
   fprintf(stderr, "%s: err=%d\n", __FUNCTION__, error);
}

static void
_emodel_eio_constructor(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Eio *priv = class_data;
   const char *prop, *path = va_arg(*list, const char *);
   priv->path = path;
   Eina_Value *v;
   size_t i;

   eo_do_super(obj, MY_CLASS, eo_constructor()); 

   priv->properties = eina_value_array_new(EINA_VALUE_TYPE_STRING, 0);
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_FILENAME, "filename");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_IS_DIR, "is_dir");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_IS_LNK, "is_lnk");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_SIZE, "size");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_MTIME, "mtime");

#define __BUG__ 1
#if __BUG__
   /*
    * TODO/FIXME/XXX
    *
    * memory corruption
    * when freeing hash
    */
#warning "<leak>Please FIX this memory corruption!!</leak>"
   priv->hash = eina_hash_string_small_new(NULL);
#else
#warning "Please FIX this memory corruption!!"
   priv->hash = eina_hash_string_small_new(free);
#endif

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
_emodel_eio_destructor(Eo *obj , void *class_data, va_list *list EINA_UNUSED)
{
   Emodel_Eio *priv = class_data;
   
   eina_hash_free(priv->hash);
   priv->hash = NULL;
   
   eio_shutdown();
   eo_do_super(obj, MY_CLASS, eo_destructor()); 
}

static void
_emodel_eio_properties_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list EINA_UNUSED)
{
   Emodel_Eio *priv = class_data;
   _assert_ref(eo_ref_get(priv->obj));
   eo_do(priv->obj, eo_event_callback_call(EMODEL_PROPERTIES_CHANGE_EVT, priv->properties, NULL));
}

static void
_emodel_eio_property_get(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{
   Emodel_Property_EVT evt;
   Emodel_Eio *priv = class_data;
   const char *prop_arg = va_arg(*list, const char*);

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &evt.prop);
   if (!strncmp(prop_arg, evt.prop, strlen(evt.prop))) 
     {
        evt.value = eina_hash_find(priv->hash, evt.prop);
        _assert_ref(eo_ref_get(priv->obj));
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
_emodel_eio_load(Eo *obj  EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   //TODO: implement
}

static void
_emodel_eio_unload(Eo *obj  EINA_UNUSED, void *class_data EINA_UNUSED, va_list *list EINA_UNUSED)
{
   //TODO: implement
}

/**
 * @brief _emodel_eio_child_add overrides  emodel_child_add and 
 * executes its own event and callback.
 */
static void
_emodel_eio_child_add(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{ 
   size_t len;
   Emodel_Eio_Child_Add *child = calloc(1, sizeof(Emodel_Eio_Child_Add));
   EINA_SAFETY_ON_NULL_RETURN(child);

   child->callback = va_arg(*list, Emodel_Cb);
   
   child->user = calloc(1, sizeof(Emodel_Child_Add));
   EINA_SAFETY_ON_NULL_GOTO(child->user, cleanup_bottom);
   
   memcpy(child->user, va_arg(*list, Emodel_Child_Add *), sizeof(Emodel_Child_Add));
   child->priv = class_data;

   len = strlen(child->priv->path) + strlen(child->user->name);

   switch(child->user->filetype)
     {
      case EMODEL_EIO_FILE_TYPE_DIR:
         {
            // len + '/' + '\0'
            child->fullpath = calloc(1, len+2);
            EINA_SAFETY_ON_NULL_GOTO(child->fullpath, cleanup_top);

            //gets current mask
            mode_t mode = umask(0);
            umask(mode);

            //_eio_done_mkdir_cb frees memory
            strncpy(child->fullpath, child->priv->path, strlen(child->priv->path));
            strncat(child->fullpath, "/", 1);
            strncat(child->fullpath, child->user->name, strlen(child->user->name));
            eio_file_mkdir(child->fullpath, /* TODO check this */ (0777 - mode), 
                           _eio_done_mkdir_cb, _eio_done_error_mkdir_cb, child);
         }
         break;
      case EMODEL_EIO_FILE_TYPE_FILE:
         {
            fprintf(stdout,"EMODEL_EIO_FILE_TYPE_FILE: unimplemented\n"); 
            goto cleanup_top; //TODO/FIXME/XXX: remove this temporary jmp
            
            //eio_file_open(child->fullpath, 
            //               EINA_FALSE, _eio_done_open_cb, _eio_done_error_open_cb, child);
         }
         break;
      default:
         fprintf(stdout, "Invalid filetype\n");
         goto cleanup_top; 
     }

   // Ok
   return;

   //cleanup the mess
cleanup_top:
   _emodel_dealloc_memory(child->user, child, NULL);
cleanup_bottom:
   _emodel_dealloc_memory(child, NULL);

}

/**
 * Child Del
 */
static void
_emodel_eio_child_del(Eo *obj EINA_UNUSED, void *class_data, va_list *list)
{ 
   //size_t len;
   //Emodel_Eio_Child_Del *child = calloc(1, sizeof(Emodel_Eio_Child_Del));
   //EINA_SAFETY_ON_NULL_RETURN(child);

#if 0
EAPI Eio_File *eio_dir_unlink(const char *path,
                              Eio_Filter_Direct_Cb filter_cb,
			      Eio_Progress_Cb progress_cb,
			      Eio_Done_Cb done_cb,
			      Eio_Error_Cb error_cb,
			      const void *data);
#endif
}

/**
 * Children Get
 */
static Eina_Bool 
_eio_filter_children_get_cb(void *data, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   cdata->idx++;

   return EINA_TRUE;
}

static void
_eio_main_children_get_cb(void *data, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   Eo *child;

   _assert_ref(eo_ref_get(cdata->priv->obj));
   child = eo_add_custom(MY_CLASS, cdata->priv->obj, emodel_eio_constructor(info->path));

   _assert_ref(eo_ref_get(cdata->priv->obj));

   cdata->callback(&cdata->cidx, child, cdata->data);
   cdata->cidx++;
}


static void 
_eio_done_children_get_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   _emodel_dealloc_memory(cdata, NULL);
}

static void
_eio_error_children_get_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error)
{
   fprintf(stderr, "%s: err=%d\n", __FUNCTION__, error);
}

static void
_emodel_eio_children_get(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Cb callback = va_arg(*list, Emodel_Cb);
   EINA_SAFETY_ON_NULL_RETURN(callback);


   Emodel_Eio_Children_Data *cdata = calloc(1, sizeof(Emodel_Eio_Children_Data));
   EINA_SAFETY_ON_NULL_RETURN(cdata);

   cdata->callback = callback;
   cdata->data = va_arg(*list, void *);
   cdata->priv = class_data;
   cdata->priv->obj = obj;
   cdata->start = 0;
   cdata->count = 0;
   cdata->idx = 0;
   cdata->cidx = 0;

   _assert_ref(eo_ref_get(cdata->priv->obj));
   cdata->lsref = eio_file_direct_ls(cdata->priv->path, _eio_filter_children_get_cb, 
                      _eio_main_children_get_cb, _eio_done_children_get_cb, _eio_error_children_get_cb, cdata);
}


/**
 * Children Slice Get
 */
static Eina_Bool 
_eio_filter_children_slice_get_cb(void *data, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info EINA_UNUSED)
{
   Eina_Bool ret = EINA_FALSE;
   Emodel_Eio_Children_Data *cdata = data;

   if(cdata->idx >= cdata->start 
      && cdata->idx < cdata->count)
     {
        ret = EINA_TRUE;
     }

   if(cdata->idx == cdata->count)
     eio_file_cancel(cdata->lsref);

   cdata->idx++;

   return ret;
}

static void 
_eio_done_children_slice_get_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   _emodel_dealloc_memory(cdata, NULL);
}

static void
_emodel_eio_children_slice_get(Eo *obj , void *class_data, va_list *list)
{
   Emodel_Eio_Children_Data *cdata;
   int start, count;

   Emodel_Cb callback = va_arg(*list, Emodel_Cb);
   EINA_SAFETY_ON_NULL_RETURN(callback);

   cdata = calloc(1, sizeof(Emodel_Eio_Children_Data));
   EINA_SAFETY_ON_NULL_RETURN(cdata);
   
   start = va_arg(*list, int);
   count = va_arg(*list, int);
   EINA_SAFETY_ON_FALSE_RETURN(start >= 0);
   EINA_SAFETY_ON_FALSE_RETURN(count > 0);

   cdata->data = va_arg(*list, void *);
   cdata->callback = callback;
   cdata->priv = class_data;
   cdata->priv->obj = obj;
   cdata->start = start;
   cdata->count = count + start;
   cdata->idx = 0;
   cdata->cidx = cdata->start;

   _assert_ref(eo_ref_get(cdata->priv->obj));
   cdata->lsref = eio_file_direct_ls(cdata->priv->path, 
                      _eio_filter_children_slice_get_cb, 
                      _eio_main_children_get_cb, 
                      _eio_done_children_slice_get_cb, 
                      _eio_error_children_get_cb, 
                      cdata);
}

/**
 * Children Count Get
 */
static Eina_Bool
_eio_filter_children_count_get_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(info->path, EINA_FALSE);
   return EINA_TRUE;
}

//TODO
static void
_eio_main_children_count_get_cb(void *data, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info EINA_UNUSED)
{
   Emodel_Eio_Children_Count *count_data = (Emodel_Eio_Children_Count *)data;
   EINA_SAFETY_ON_NULL_RETURN(count_data);
   count_data->total++;
}


static void 
_eio_done_children_count_get_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Children_Count *count_data = (Emodel_Eio_Children_Count *)data;
   EINA_SAFETY_ON_NULL_RETURN(count_data);
   _assert_ref(eo_ref_get(count_data->priv->obj));
   eo_do(count_data->priv->obj, eo_event_callback_call(EMODEL_CHILDREN_COUNT_GET_EVT, &(count_data->total), EINA_FALSE));
   _emodel_dealloc_memory(count_data, NULL);
}

static void
_eio_error_children_count_get_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
   fprintf(stdout, "[cancel] eio_file_direct_ls : %d\n", error);
}

static void
_emodel_eio_children_count_get(Eo *obj , void *class_data, va_list *list EINA_UNUSED)
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
      EO_OP_FUNC(EMODEL_EIO_ID(EMODEL_OBJ_EIO_SUB_ID_CHILD_ADD), _emodel_eio_child_add),
      EO_OP_FUNC(EMODEL_EIO_ID(EMODEL_OBJ_EIO_SUB_ID_CHILD_DEL), _emodel_eio_child_del),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_CHILDREN_GET), _emodel_eio_children_get),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_CHILDREN_SLICE_GET), _emodel_eio_children_slice_get),
      EO_OP_FUNC(EMODEL_ID(EMODEL_SUB_ID_CHILDREN_COUNT_GET), _emodel_eio_children_count_get),
      EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_descs);
}

static const Eo_Op_Description op_desc[] = {
     EO_OP_DESCRIPTION(EMODEL_OBJ_EIO_SUB_ID_CONSTRUCTOR, "Eio file constructor."),
     EO_OP_DESCRIPTION(EMODEL_OBJ_EIO_SUB_ID_CHILD_ADD, "Add new child."),
     EO_OP_DESCRIPTION(EMODEL_OBJ_EIO_SUB_ID_CHILD_DEL, "Delete child."),
     EO_OP_DESCRIPTION_SENTINEL
};

static const Eo_Class_Description class_desc = {
     EO_VERSION,
     MY_CLASS_NAME,
     EO_CLASS_TYPE_REGULAR,
     EO_CLASS_DESCRIPTION_OPS(&EMODEL_EIO_BASE_ID, op_desc, EMODEL_OBJ_EIO_SUB_ID_LAST),
     NULL,
     sizeof(Emodel_Eio),
     _emodel_eio_class_constructor,
     NULL
};

EO_DEFINE_CLASS(emodel_obj_eio_class_get, &class_desc, EO_BASE_CLASS, EMODEL_CLASS, NULL);
