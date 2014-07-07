#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Emodel.h>
#include <Eina.h>
#include <emodel_eio.h>
#include <Eio.h>
#include <Ecore.h>
#include <Eo.h>

#include "emodel_eio_private.h"

#define MY_CLASS EMODEL_EIO_CLASS
#define MY_CLASS_NAME "Emodel_Eio"

static Eina_Value_Struct_Desc *EMODEL_EIO_PROPERTIES_DESC = NULL;
static void _eio_prop_set_error_cb(void *, Eio_File *, int);

static void
_stat_pro_set(Emodel_Eio_Data *priv, int prop_id, const char *prop_name, int pvalue)
{
   Emodel_Property_EVT evt;
   EINA_SAFETY_ON_NULL_RETURN(priv);

   switch(prop_id)
     {
      case EMODEL_EIO_PROP_IS_DIR:
      case EMODEL_EIO_PROP_IS_LNK:
      case EMODEL_EIO_PROP_SIZE:
         eina_value_setup(&evt.value, EINA_VALUE_TYPE_INT);
         break;
      case EMODEL_EIO_PROP_MTIME:
         eina_value_setup(&evt.value, EINA_VALUE_TYPE_TIMEVAL);
         break;
      default:
         EINA_SAFETY_ON_NULL_RETURN(NULL); /**< error */
     }

   evt.prop = prop_name;
   eina_value_set(&evt.value, pvalue); /**< caller must call eina_value_flush */

   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(priv->obj));

   /**
    * Here property EMODEL_EVENT_PROPERTY_CHANGE is dispatched because
    * anyone can listen to these events by using eo_event_callback_add().
    */
   eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));
}

static void
_emodel_eio_root_set(Eo *model, Eo *root)
{
   EINA_SAFETY_ON_NULL_RETURN(model);
   EINA_SAFETY_ON_NULL_RETURN(root);
   Emodel_Eio_Data *priv = eo_data_scope_get(model, MY_CLASS);

   EINA_SAFETY_ON_NULL_RETURN(priv);
   priv->rootmodel = root;
}

/**
 *  Callbacks
 *  Property
 */
static void
_eio_stat_done_cb(void *data, Eio_File *handler EINA_UNUSED, const Eina_Stat *stat)
{
   Emodel_Eio_Data *priv = data;
   Eina_Value_Struct st;
   priv->stat = stat;

   _stat_pro_set(priv, EMODEL_EIO_PROP_IS_DIR, "is_dir", eio_file_is_dir(stat));
   _stat_pro_set(priv, EMODEL_EIO_PROP_IS_LNK, "is_lnk" ,eio_file_is_lnk(stat));
   _stat_pro_set(priv, EMODEL_EIO_PROP_SIZE, "size" ,eio_file_size(stat));
   _stat_pro_set(priv, EMODEL_EIO_PROP_MTIME, "mtime", eio_file_mtime(stat));

   Emodel_Property_EVT evt;

   memset(&st, 0, sizeof(Eina_Value_Struct));
   eina_value_pget(priv->properties, &st);

   evt.prop = st.desc->members[EMODEL_EIO_PROP_ICON].name;
   eina_value_setup(&evt.value, EINA_VALUE_TYPE_STRING);

   if (eio_file_is_dir(stat))
        eina_value_set(&evt.value, "icon folder");
   else
        eina_value_set(&evt.value, "icon file");
   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(priv->obj));
   eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));
}

static void
_eio_progress_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, const Eio_Progress *info EINA_UNUSED)
{
   //TODO: implement
}

static void
_eio_move_done_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Property_EVT evt;
   Eina_Value_Struct st;
   Emodel_Eio_Data *priv = data;

   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(priv->obj));

   memset(&st, 0, sizeof(Eina_Value_Struct));
   eina_value_pget(priv->properties, &st);

   /**
    * When mv is executed we update our values and
    * notify both path and filename properties listeners.
    */

   evt.prop = st.desc->members[EMODEL_EIO_PROP_PATH].name;
   eina_value_struct_set(priv->properties, evt.prop, priv->path);
   eina_value_struct_value_get(priv->properties, evt.prop, &evt.value);
   eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));

   evt.prop = st.desc->members[EMODEL_EIO_PROP_FILENAME].name;
   eina_value_struct_set(priv->properties, evt.prop, basename(priv->path));
   eina_value_struct_value_get(priv->properties, evt.prop, &evt.value);
   eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));
}

static void
_eio_error_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error)
{
   if(error != 0)
     {
        fprintf(stderr, "Error: %s : %d: %s\n", __FUNCTION__, error, strerror(errno));
        EINA_SAFETY_ON_FALSE_RETURN(EINA_FALSE); /**< force check error only to be more verbose */
     }
   fprintf(stdout, "%s : %d\n", __FUNCTION__, error);
}

static void
_eio_prop_set_error_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
   fprintf(stdout, "%s : %d\n", __FUNCTION__, error);
}

/**
 *   Callbacks
 *   Child Add
 */
static void
_eio_done_mkdir_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Child_Add *_data = (Emodel_Eio_Child_Add *)data;
   Eo *parent = _data->priv->obj;
   Eo *root = _data->priv->rootmodel;

   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(parent));
   /* save child object in userdata, callback can ignore this field */
   if (root == NULL)
     root = parent;

   _data->child = eo_add_custom(MY_CLASS, NULL, emodel_eio_constructor(_data->fullpath));
   _emodel_eio_root_set(_data->child, root);
   eo_do(_data->child, emodel_eio_children_filter_set(_data->priv->filter_cb, _data->priv->filter_userdata)); //XXX: set parent filter to child
   /* dispatch callback to user */
   _data->callback(_data->name, parent, _data->child, 0);
   _emodel_dealloc_memory(_data->fullpath, _data->name, _data, NULL);
}

static void
_eio_done_error_mkdir_cb(void *data, Eio_File *handler EINA_UNUSED, int error)
{
   if(0 != error)
     {
        fprintf(stderr, "%s: err=%d\n", __FUNCTION__, error);
        Emodel_Eio_Child_Add *_data = (Emodel_Eio_Child_Add *)data;
        Eo *parent = _data->priv->obj;

        EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(parent));
        /* save child object in userdata, callback can ignore this field */
        _data->callback(_data->name, parent, NULL, error);
        _emodel_dealloc_memory(_data->fullpath, _data->name, _data, NULL);
     }
}

/**
 *  Callbacks
 *  Ecore Events
 */
static Eina_Bool
 _emodel_evt_added_ecore_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event EINA_UNUSED)
{
   Eio_Monitor_Event *evt = (Eio_Monitor_Event*)event;
   Emodel_Eio_Data *priv = data;
   Emodel_Children_EVT cevt;
   Eo *root = priv->rootmodel;

   if (root == NULL)
     root = priv->obj;

   cevt.child = eo_add_custom(MY_CLASS, NULL, emodel_eio_constructor(evt->filename));
   cevt.idx = -1;
   _emodel_eio_root_set(cevt.child, root);

   return eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_CHILD_ADD, &cevt));
}

static Eina_Bool
_emodel_evt_deleted_ecore_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event EINA_UNUSED)
{
   //Eio_Monitor_Event *evt = (Eio_Monitor_Event*)event;
   Emodel_Eio_Data *priv = data;
   Emodel_Children_EVT cevt;

   cevt.child = NULL;
   cevt.idx = -1;

   return eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_CHILD_DEL, &cevt));
}

static void
_eio_monitors_list_load(Emodel_Eio_Data *priv)
{
   priv->mon.mon_event_child_add[0] = EIO_MONITOR_DIRECTORY_CREATED;
   priv->mon.mon_event_child_add[1] = EIO_MONITOR_FILE_CREATED;
   priv->mon.mon_event_child_add[2] = EIO_MONITOR_ERROR;
   priv->mon.mon_event_child_del[0] = EIO_MONITOR_DIRECTORY_DELETED;
   priv->mon.mon_event_child_del[1] = EIO_MONITOR_FILE_DELETED;
   priv->mon.mon_event_child_del[2] = EIO_MONITOR_ERROR;
}

Eina_Bool
_eio_monitor_evt_added_cb(void *data EINA_UNUSED, Eo *obj, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Emodel_Eio_Data *priv = eo_data_scope_get(obj, MY_CLASS);
   const Eo_Callback_Array_Item *callback_array = event_info;
   unsigned int i;

   if((callback_array->desc != EMODEL_EVENT_CHILD_ADD) && (callback_array->desc != EMODEL_EVENT_CHILD_DEL))
     return EO_CALLBACK_CONTINUE;

   if((priv->cb_count_child_add == 0) && (priv->cb_count_child_del == 0))
     {
        Eio_Monitor *_mon  = eio_monitor_add(priv->path);
        if(!_mon) return EO_CALLBACK_CONTINUE;
        priv->monitor = _mon;
     }

   if(callback_array->desc == EMODEL_EVENT_CHILD_ADD)
     {
        for(i = 0; priv->mon.mon_event_child_add[i] != EIO_MONITOR_ERROR ; ++i)
          {
             priv->mon.ecore_child_add_handler[i] =
                 ecore_event_handler_add(priv->mon.mon_event_child_add[i], _emodel_evt_added_ecore_cb, priv);
          }
        priv->cb_count_child_add++;
     }
   else if(callback_array->desc == EMODEL_EVENT_CHILD_DEL)
     {
        for(i = 0; priv->mon.mon_event_child_del[i] != EIO_MONITOR_ERROR ; ++i)
          {
             priv->mon.ecore_child_add_handler[i] =
                 ecore_event_handler_add(priv->mon.mon_event_child_del[i], _emodel_evt_deleted_ecore_cb, priv);
          }
        priv->cb_count_child_del++;
     }

    return EO_CALLBACK_CONTINUE;
}

Eina_Bool
_eio_monitor_evt_deleted_cb(void *data EINA_UNUSED, Eo *obj, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
    Emodel_Eio_Data *priv = eo_data_scope_get(obj, MY_CLASS);
    const Eo_Callback_Array_Item *callback_array = event_info;
    unsigned int i;

    if((callback_array->desc != EMODEL_EVENT_CHILD_ADD) && (callback_array->desc != EMODEL_EVENT_CHILD_DEL))
         return EO_CALLBACK_CONTINUE;

    if(!priv->monitor)
        return EO_CALLBACK_CONTINUE;

    if(callback_array->desc == EMODEL_EVENT_CHILD_ADD)
      {
         if(priv->cb_count_child_add > 0)
           {
              for(i = 0; priv->mon.mon_event_child_add[i] != EIO_MONITOR_ERROR ; ++i)
                {
                   ecore_event_handler_del(priv->mon.ecore_child_add_handler[i]);
                }
              priv->cb_count_child_add--;
           }
      }
    else if(callback_array->desc == EMODEL_EVENT_CHILD_DEL)
      {
         if(priv->cb_count_child_del > 0)
           {
              for(i = 0; priv->mon.mon_event_child_del[i] != EIO_MONITOR_ERROR ; ++i)
                {
                   ecore_event_handler_del(priv->mon.ecore_child_del_handler[i]);
                }
              priv->cb_count_child_del--;
           }
      }

    if((priv->cb_count_child_add == 0) && (priv->cb_count_child_del == 0))
      {
         eio_monitor_del(priv->monitor);
      }

    return EO_CALLBACK_CONTINUE;
}

/*
 *  Callbacks
 *  Children Get
 */
static void
_eio_main_children_fetch_cb(void *data, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   Eo *root, *child;

   EINA_SAFETY_ON_NULL_RETURN(cdata);
   EINA_SAFETY_ON_NULL_RETURN(cdata->priv->obj);
   root = cdata->priv->rootmodel;

   if (root == NULL)
     root = cdata->priv->obj;

   child = eo_add_custom(MY_CLASS, NULL, emodel_eio_constructor(info->path));
   _emodel_eio_root_set(child, root);
   eo_do(child, emodel_eio_children_filter_set(cdata->priv->filter_cb, cdata->priv->filter_userdata)); //XXX: set parent filter to child
   cdata->callback(cdata->data, child, &cdata->cidx, 0);
   cdata->cidx++;
}

 static Eina_Bool
_eio_filter_children_fetch_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   Emodel_Eio_Children_Data *cdata = data;
   EINA_SAFETY_ON_NULL_RETURN_VAL(cdata, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(cdata->priv, EINA_FALSE);

   if (cdata->priv->filter_cb)
     {
         return cdata->priv->filter_cb(cdata->priv->filter_userdata, handler, info);
     }

   return EINA_TRUE;
}

static void
_eio_done_children_fetch_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   _emodel_dealloc_memory(cdata, NULL);
}

static void
_eio_error_children_fetch_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error)
{
   if(0 != error)
     {
        fprintf(stderr, "%s: err=%d\n", __FUNCTION__, error);
        Emodel_Eio_Child_Add *_data = (Emodel_Eio_Child_Add *)data;
        Eo *parent = _data->priv->obj;

        EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(parent));
        /* save child object in userdata, callback can ignore this field */

        _data->callback(_data->name, parent, NULL, error);
        _emodel_dealloc_memory(_data->fullpath, _data->name, _data, NULL);
     }
}

/**
 *  Callbacks
 *  Children Slice Get
 */
static Eina_Bool
_eio_filter_children_slice_fetch_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   Eina_Bool ret = EINA_FALSE;
   Emodel_Eio_Children_Data *cdata = data;

   if (_eio_filter_children_fetch_cb(data, handler, info))
     {
        if(cdata->idx >= cdata->start && cdata->idx < cdata->count)
          {
             ret = EINA_TRUE;
          }

        if(cdata->idx == cdata->count)
          {
             eio_file_cancel(cdata->lsref);
          }

        cdata->idx++;
     }

   return ret;
}

static void
_eio_done_children_slice_fetch_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   _emodel_dealloc_memory(cdata, NULL);
}

/**
 *  Callbacks
 *  Children Count Get
 */

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
   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(count_data->priv->obj));

   if (count_data->priv->count != count_data->total)
     {
        count_data->priv->count = count_data->total;
        eo_do(count_data->priv->obj, eo_event_callback_call(EMODEL_EVENT_CHILDREN_COUNT_CHANGE, &(count_data->total)));
     }

   _emodel_dealloc_memory(count_data, NULL);
}

static void
_eio_error_children_count_get_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
   fprintf(stdout, "%s : %d\n", __FUNCTION__, error);
}

/**
 *  Callbacks
 *  Child Del
 */
static Eina_Bool
_eio_filter_child_del_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info EINA_UNUSED)
{
   return EINA_TRUE;
}

static void
_eio_progress_child_del_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, const Eio_Progress *info EINA_UNUSED)
{}

static Eina_Bool
_null_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info)
{
   (void) desc;
   (void) obj;
   (void) data;
   (void) event_info;
   return EO_CALLBACK_CONTINUE;
}

static void
_eio_done_unlink_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Data *priv = (Emodel_Eio_Data *)data;

   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(priv->obj);
   EINA_SAFETY_ON_NULL_RETURN(priv->emodel_cb);

   /** use dummy callback */
   /**
    * We generate these events here because _eio_monitor_evt_added_cb and
    * _eio_monitor_evt_deleted_cb can add or delete, respectively,
    * I/O event handlers for directories.
    */
   eo_do(priv->obj, eo_event_callback_add(EMODEL_EVENT_CHILD_ADD, _null_cb, NULL));
   eo_do(priv->obj, eo_event_callback_del(EMODEL_EVENT_CHILD_ADD, _null_cb, NULL));
   eo_do(priv->obj, eo_event_callback_add(EMODEL_EVENT_CHILD_DEL, _null_cb, NULL));
   eo_do(priv->obj, eo_event_callback_del(EMODEL_EVENT_CHILD_DEL, _null_cb, NULL));

   priv->emodel_cb(NULL, priv->obj, priv->obj, 0);
}

static void
_eio_error_unlink_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error)
{
   fprintf(stdout, "%s : %d\n", __FUNCTION__, error);
}


/**
 * Interfaces impl.
 */
static void
_emodel_eio_emodel_prop_list(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd)
{
   Emodel_Eio_Data *priv = _pd;
   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(priv->obj);
   eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTIES_CHANGE, priv->properties));
}

/**
 * Property Fetch
 */
static void
_emodel_eio_emodel_prop_fetch(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd, const char *property)
{
   Emodel_Property_EVT evt;
   Eina_Value_Struct st;
   size_t proplen;
   Emodel_Eio_Data *priv = _pd;

   EINA_SAFETY_ON_NULL_RETURN(property);
   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(priv->obj);

   proplen = strlen(property);
   memset(&st, 0, sizeof(Eina_Value_Struct));
   eina_value_pget(priv->properties, &st);

   evt.prop = st.desc->members[EMODEL_EIO_PROP_FILENAME].name;
   if((evt.prop != NULL) && (proplen == strlen(evt.prop)))
     {
        if(!strcmp(property, evt.prop))
          {
             eina_value_struct_value_get(priv->properties, evt.prop, &evt.value);
             eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));
             return;
          }
     }

   evt.prop = st.desc->members[EMODEL_EIO_PROP_PATH].name;
   if((evt.prop != NULL) && (proplen == strlen(evt.prop)))
     {
        if(!strcmp(property, evt.prop))
          {
             eina_value_struct_value_get(priv->properties, evt.prop, &evt.value);
             eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));
             return;
          }
     }

   priv->file = eio_file_direct_stat(priv->path, _eio_stat_done_cb, _eio_error_cb, priv);
}


/**
 * Property Set
 */
static void
_emodel_eio_emodel_prop_set(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd, const char *property, const Eina_Value *value)
{
   Eina_Value_Struct st;
   Emodel_Eio_Data *priv = _pd;
   const char *dest, *prop = NULL;

   EINA_SAFETY_ON_NULL_RETURN(value);
   EINA_SAFETY_ON_NULL_RETURN(property);

   memset(&st, 0, sizeof(Eina_Value_Struct));
   eina_value_pget(priv->properties, &st);

   prop = st.desc->members[EMODEL_EIO_PROP_PATH].name;
   EINA_SAFETY_ON_NULL_RETURN(prop);

   /* Both strings must be equal in size as well.
    * Case insensitive check 
    */
   if(strlen(property) != strlen(prop)) return;
   if (!strcmp(property, prop))
     {
        const char *src;
        eina_value_struct_get(priv->properties, "path", &src);
        eina_value_get(value, &dest);
        _emodel_dealloc_memory(priv->path, NULL);
        priv->path = strdup(dest);
        priv->file = eio_file_move(src, dest, _eio_progress_cb, _eio_move_done_cb, _eio_prop_set_error_cb, priv);
     }
}

/**
 * Load
 */
 static void
_emodel_eio_emodel_load(Eo *obj  EINA_UNUSED, Emodel_Eio_Data *_pd EINA_UNUSED)
{
   //TODO: implement
}

/**
 * Unload
 */
static void
_emodel_eio_emodel_unload(Eo *obj  EINA_UNUSED, Emodel_Eio_Data *_pd EINA_UNUSED)
{
   //TODO: implement
}

/**
 * Child Add
 */
static void
_emodel_eio_dir_add(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd, Emodel_Cb child_add_cb, const char *name)
{
   size_t total_len, name_len;
   Emodel_Eio_Child_Add *child = calloc(1, sizeof(Emodel_Eio_Child_Add));
   EINA_SAFETY_ON_NULL_RETURN(child);

   child->callback = child_add_cb;
   name_len = strlen(name);

   child->name = calloc(1, name_len);
   EINA_SAFETY_ON_NULL_GOTO(child->name, cleanup_bottom);

   strncpy(child->name, name, name_len);
   child->priv = _pd;
   total_len = strlen(child->priv->path) + name_len + 2; /**< +2 is: '/' + 0x00 */

   child->fullpath = calloc(1, total_len);
   EINA_SAFETY_ON_NULL_GOTO(child->fullpath, cleanup_top);

   //_eio_done_mkdir_cb frees memory
   eina_str_join(child->fullpath, total_len, '/', child->priv->path, child->name);

   /*  read/write/search permissions for owner and group,
    * and with read/search permissions for others */
   eio_file_mkdir(child->fullpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH,
                  _eio_done_mkdir_cb, _eio_done_error_mkdir_cb, child);

   // Ok */
   return;

   //cleanup the mess */
cleanup_top:
   _emodel_dealloc_memory((char*)child->name, child, NULL);
cleanup_bottom:
   _emodel_dealloc_memory(child, NULL);
}

static void
_emodel_eio_children_filter_set(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd, Eio_Filter_Direct_Cb filter_cb, void *data)
{
   Emodel_Eio_Data *priv = _pd;

   priv->filter_cb = filter_cb;
   priv->filter_userdata = data;
}

/**
 * Child Del
 */
static void
_emodel_eio_emodel_child_del(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd EINA_UNUSED, Emodel_Cb cb, Eo *child)
{
   EINA_SAFETY_ON_NULL_RETURN(cb);
   EINA_SAFETY_ON_NULL_RETURN(child);

   Emodel_Eio_Data *child_priv = eo_data_scope_get(child, MY_CLASS);
   EINA_SAFETY_ON_NULL_RETURN(child_priv);

   child_priv->emodel_cb = cb;
   EINA_SAFETY_ON_NULL_RETURN(child_priv->emodel_cb);

   eio_dir_unlink(child_priv->path,
                  _eio_filter_child_del_cb,
                  _eio_progress_child_del_cb,
                  _eio_done_unlink_cb,
                  _eio_error_unlink_cb,
                  child_priv);
}

/**
 * Children Get
 */
static void
_emodel_eio_emodel_children_fetch(Eo *obj , Emodel_Eio_Data *_pd, Emodel_Cb callback, const void *data)
{
   EINA_SAFETY_ON_NULL_RETURN(callback);

   Emodel_Eio_Children_Data *cdata = calloc(1, sizeof(Emodel_Eio_Children_Data));
   EINA_SAFETY_ON_NULL_RETURN(cdata);

   cdata->callback = callback;
   cdata->data = data;
   cdata->priv = _pd;
   cdata->priv->obj = obj;
   cdata->start = 0;
   cdata->count = 0;
   cdata->idx = 0;
   cdata->cidx = 0;

   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(cdata->priv->obj));
   cdata->lsref = eio_file_direct_ls(cdata->priv->path, _eio_filter_children_fetch_cb,
                      _eio_main_children_fetch_cb, _eio_done_children_fetch_cb, _eio_error_children_fetch_cb, cdata);
}


/**
 * Children Slice Get
 */
static void
_emodel_eio_emodel_children_slice_fetch(Eo *obj, Emodel_Eio_Data *_pd, Emodel_Cb children_slice_fetch_cb,
                                        int start, int count, const void *data)
{
    Emodel_Eio_Children_Data *cdata;

    EINA_SAFETY_ON_NULL_RETURN(children_slice_fetch_cb);

    cdata = calloc(1, sizeof(Emodel_Eio_Children_Data));
    EINA_SAFETY_ON_NULL_RETURN(cdata);

    EINA_SAFETY_ON_FALSE_RETURN(start >= 0);
    EINA_SAFETY_ON_FALSE_RETURN(count > 0);

    cdata->data = data;
    cdata->callback = children_slice_fetch_cb;
    cdata->priv = _pd;
    cdata->priv->obj = obj;
    cdata->start = start;
    cdata->count = count + start;
    cdata->idx = 0;
    cdata->cidx = cdata->start;

    EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(cdata->priv->obj));
    cdata->lsref = eio_file_direct_ls(cdata->priv->path,
                       _eio_filter_children_slice_fetch_cb,
                       _eio_main_children_fetch_cb,
                       _eio_done_children_slice_fetch_cb,
                       _eio_error_children_fetch_cb,
                       cdata);
}

/**
 * Children Count Get
 */
static int /**< XXX: unsigned int */
_emodel_eio_emodel_children_count_get(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd)
{
   Emodel_Eio_Data *priv = _pd;
   Emodel_Eio_Children_Count *count_data = calloc(1, sizeof(Emodel_Eio_Children_Count));
   EINA_SAFETY_ON_NULL_RETURN_VAL(count_data, priv->count);
   count_data->priv = priv;

   eio_file_direct_ls(priv->path, _eio_filter_children_fetch_cb,
                      _eio_main_children_count_get_cb, _eio_done_children_count_get_cb,
                      _eio_error_children_count_get_cb, count_data);

   return priv->count;
}

/**
 * Child select
 */
static void
_emodel_eio_emodel_child_select(Eo *obj, Emodel_Eio_Data *_pd, Eo *child)
{
   Emodel_Eio_Data *priv = _pd;
   Emodel_Children_EVT cevt;
   const char *path;
   Eo *childSelected = child;

   if (priv->rootmodel != NULL)
     {
         eo_do(priv->rootmodel, emodel_child_select(childSelected));
         return;
     }

   //XXX: verify if eo* is a real child
   eo_do(childSelected, emodel_eio_path_get(&path));
   free(priv->pathSelected);
   priv->pathSelected = strdup(path);

   cevt.child = eo_add_custom(MY_CLASS, NULL, emodel_eio_constructor(path));
   _emodel_eio_root_set(cevt.child, obj);
   cevt.idx = 0;

   eo_do(cevt.child, emodel_eio_children_filter_set(priv->filter_cb, priv->filter_userdata));
   eo_do(obj, eo_event_callback_call(EMODEL_EVENT_CHILD_SELECTED, &cevt));
}

/**
 * Child select get
 */
static void
_emodel_eio_emodel_child_select_eval(Eo *obj EINA_UNUSED, Emodel_Eio_Data *priv)
{
   Emodel_Children_EVT cevt;

   if (priv->rootmodel != NULL)
     {
         eo_do(priv->rootmodel, emodel_child_select_eval());
         return;
     }

   if (priv->pathSelected != NULL)
     {
         cevt.child = eo_add_custom(MY_CLASS, NULL, emodel_eio_constructor(priv->path));
         _emodel_eio_root_set(cevt.child, obj);
         cevt.idx = 0;

         eo_do(cevt.child, emodel_eio_children_filter_set(priv->filter_cb, priv->filter_userdata));
         eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_CHILD_SELECTED, &cevt));
     }
}

/**
 * Path get
 */
static void
_emodel_eio_path_get(Eo *obj EINA_UNUSED, Emodel_Eio_Data *priv, const char **path)
{
   *path = priv->path;
}

static void
_emodel_eio_emodel_constructor(Eo *obj, Emodel_Eio_Data *_pd EINA_UNUSED)
{
   fprintf(stderr, "Error: Implement this '%s' default class constructor.", MY_CLASS_NAME);
   eo_error_set(obj);
}
/**
 * Class definitions
 */
static void
_emodel_eio_constructor(Eo *obj , Emodel_Eio_Data *priv, const char *path)
{
   eo_do_super(obj, MY_CLASS, eo_constructor());

   priv->rootmodel = NULL;
   priv->path = strdup(path);
   priv->pathSelected = NULL;

   typedef struct _This_Eio_Properties
     {
        const char *filename;
        const char *path;
        const char *icon;
        double mtime;
        int is_dir;
        int is_lnk;
        int size;
     } This_Eio_Properties;

   static Eina_Value_Struct_Member prop_members[] = {
     EINA_VALUE_STRUCT_MEMBER(NULL, This_Eio_Properties, filename),
     EINA_VALUE_STRUCT_MEMBER(NULL, This_Eio_Properties, path),
     EINA_VALUE_STRUCT_MEMBER(NULL, This_Eio_Properties, icon), //XXX
     EINA_VALUE_STRUCT_MEMBER(NULL, This_Eio_Properties, mtime),
     EINA_VALUE_STRUCT_MEMBER(NULL, This_Eio_Properties, is_dir),
     EINA_VALUE_STRUCT_MEMBER(NULL, This_Eio_Properties, is_lnk),
     EINA_VALUE_STRUCT_MEMBER(NULL, This_Eio_Properties, size)
   };
   prop_members[EMODEL_EIO_PROP_FILENAME].type = EINA_VALUE_TYPE_STRING;
   prop_members[EMODEL_EIO_PROP_PATH].type = EINA_VALUE_TYPE_STRING;
   prop_members[EMODEL_EIO_PROP_ICON].type = EINA_VALUE_TYPE_STRING;
   prop_members[EMODEL_EIO_PROP_MTIME].type = EINA_VALUE_TYPE_TIMEVAL;
   prop_members[EMODEL_EIO_PROP_IS_DIR].type = EINA_VALUE_TYPE_INT;
   prop_members[EMODEL_EIO_PROP_IS_LNK].type = EINA_VALUE_TYPE_INT;
   prop_members[EMODEL_EIO_PROP_SIZE].type = EINA_VALUE_TYPE_INT;

   static Eina_Value_Struct_Desc prop_desc = {
     EINA_VALUE_STRUCT_DESC_VERSION,
     NULL, // no special operations
     prop_members,
     EINA_C_ARRAY_LENGTH(prop_members),
     sizeof(This_Eio_Properties)
   };
   EMODEL_EIO_PROPERTIES_DESC = &prop_desc;

   priv->properties = eina_value_struct_new(EMODEL_EIO_PROPERTIES_DESC);
   EINA_SAFETY_ON_NULL_RETURN(priv->properties);

   eina_value_struct_set(priv->properties, "path", path);
   eina_value_struct_set(priv->properties, "filename", basename(path));

   _eio_monitors_list_load(priv);
   eo_do(obj, eo_event_callback_add(EO_EV_CALLBACK_ADD, _eio_monitor_evt_added_cb, NULL));
   eo_do(obj, eo_event_callback_add(EO_EV_CALLBACK_DEL, _eio_monitor_evt_deleted_cb, NULL));
   priv->obj = obj;
}

static void
_emodel_eio_emodel_destructor(Eo *obj , Emodel_Eio_Data *priv)
{
   if(priv && priv->monitor)
     {
        eio_monitor_del(priv->monitor);
     }

   eina_value_free(priv->properties);

   eo_do_super(obj, MY_CLASS, eo_destructor());
}

#include "emodel_eio.eo.c"
