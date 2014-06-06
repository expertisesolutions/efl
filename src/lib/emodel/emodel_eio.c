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
//#include <emodel.eo.c>

/* EAPI Eo_Op EMODEL_EIO_BASE_ID = EO_NOOP; */

/* #define MY_CLASS EMODEL_EIO_CLASS */
/* #define MY_CLASS_NAME "Emodel_Eio_Class" */

static void _eio_property_set_error_cb(void *, Eio_File *, int);

static void
_stat_pro_set(Emodel_Eio_Data *priv, int prop_id, int pvalue)
{
   Emodel_Property_EVT evt;

   EINA_SAFETY_ON_NULL_RETURN(priv);
   eina_value_array_get(priv->properties, prop_id, &evt.prop);
   evt.value = _emodel_property_value_get(priv, evt.prop);
   eina_value_set(evt.value, pvalue);
   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(priv->obj));
   eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));
}


/* /\** */
/*  *  Callbacks */
/*  *  Property  */
/*  *\/ */
static void
_eio_stat_done_cb(void *data, Eio_File *handler EINA_UNUSED, const Eina_Stat *stat)
{
   Emodel_Eio_Data *priv = data;
   priv->stat = stat;

   _stat_pro_set(priv, EMODEL_EIO_PROP_IS_DIR, eio_file_is_dir(stat));
   _stat_pro_set(priv, EMODEL_EIO_PROP_IS_LNK, eio_file_is_lnk(stat));
   _stat_pro_set(priv, EMODEL_EIO_PROP_SIZE, eio_file_size(stat));
   _stat_pro_set(priv, EMODEL_EIO_PROP_MTIME, eio_file_mtime(stat));

   Emodel_Property_EVT evt;
   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_ICON, &evt.prop);
   evt.value = _emodel_property_value_get(priv, evt.prop);
   if (eio_file_is_dir(stat))
        eina_value_set(evt.value, "icon folder");
   else
        eina_value_set(evt.value, "icon file");
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
   Emodel_Eio_Data *priv = data;

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &evt.prop);
   evt.value = _emodel_property_value_get(priv, evt.prop);
   eina_value_set(evt.value, priv->path);
   eio_file_direct_stat(priv->path, _eio_stat_done_cb, _eio_property_set_error_cb, priv);

   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(priv->obj));
   eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));
}

static void
_eio_error_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
   fprintf(stdout, "%s : %d\n", __FUNCTION__, error);
}

static void
_eio_property_set_error_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
   fprintf(stdout, "%s : %d\n", __FUNCTION__, error);
}

/* /\** */
/*  *  Callbacks */
/*  *  Child Add */
/*  *\/ */
static void
_eio_done_mkdir_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Child_Add *_data = (Emodel_Eio_Child_Add *)data;
   Eo *parent = _data->priv->obj;

   EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(parent));
   /* save child object in userdata, callback can ignore this field */
   _data->child = eo_add_custom(MY_CLASS, parent, emodel_eio_constructor(_data->fullpath));
   eo_do(_data->child, emodel_eio_children_filter_set(_data->priv->filter_cb, _data->priv->filter_userdata)); //XXX: set parent filter to child
   /* dispatch callback for user */
   _data->callback(_data->name, parent, _data->child);
   eo_unref(_data->child);
   _emodel_dealloc_memory(_data->fullpath, _data->name, _data, NULL);
}

static void
_eio_done_error_mkdir_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
   fprintf(stderr, "%s: err=%d\n", __FUNCTION__, error);
}


/* /\** */
/*  *  Callbacks */
/*  *  Ecore Events */
/*  *\/ */
static Eina_Bool
 _emodel_evt_added_ecore_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event EINA_UNUSED)
{
  Eina_Bool ret;
  Eio_Monitor_Event *evt = (Eio_Monitor_Event*)event;
  Emodel_Eio_Data *priv = data;
  Emodel_Children_EVT cevt;

  cevt.data = (void*)evt->filename;
  cevt.child = eo_add_custom(MY_CLASS, priv->obj, emodel_eio_constructor(evt->filename));
  cevt.idx = -1;

  ret = eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_CHILD_ADD, &cevt));
  eo_unref(cevt.child);

  return ret;
}

static Eina_Bool
_emodel_evt_deleted_ecore_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event EINA_UNUSED)
{
   Eio_Monitor_Event *evt = (Eio_Monitor_Event*)event;
   Emodel_Eio_Data *priv = data;
   Emodel_Children_EVT cevt;

   cevt.data = (void*)evt->filename;
   cevt.child = NULL; /* FIXME */
   cevt.idx = -1;

   return eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_CHILD_DEL, &cevt));
}

Eina_Bool
_eio_monitor_evt_added_cb(void *data EINA_UNUSED, Eo *obj, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
    Emodel_Eio_Data *priv = eo_data_scope_get(obj, MY_CLASS);
    const Eo_Callback_Array_Item *callback_array = event_info;

    if((priv->mon.cb_count_child_add == 0) && (priv->mon.cb_count_child_del == 0))
      {
         priv->monitor = eio_monitor_add(priv->path);
      }

    if(callback_array->desc == EMODEL_EVENT_CHILD_ADD)
      {
         if(priv->mon.cb_count_child_add == 0)
           {
             priv->ecore_child_add_handler = ecore_event_handler_add(EIO_MONITOR_DIRECTORY_CREATED, _emodel_evt_added_ecore_cb, priv);
           }
         priv->mon.cb_count_child_add++;
         return EO_CALLBACK_CONTINUE;
      }
    else if(callback_array->desc == EMODEL_EVENT_CHILD_DEL)
      {
         if(priv->mon.cb_count_child_del == 0)
           {
             priv->ecore_child_del_handler = ecore_event_handler_add(EIO_MONITOR_DIRECTORY_DELETED, _emodel_evt_deleted_ecore_cb, priv);
           }
         priv->mon.cb_count_child_del++;
         return EO_CALLBACK_CONTINUE;
      }

    // EO_CALLBACK_CONTINUE is defined to EINA_TRUE, @see Eo.h
    return EINA_TRUE;
}

Eina_Bool
_eio_monitor_evt_deleted_cb(void *data EINA_UNUSED, Eo *obj, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
    Emodel_Eio_Data *priv = eo_data_scope_get(obj, MY_CLASS);
    const Eo_Callback_Array_Item *callback_array = event_info;

    if(callback_array->desc == EMODEL_EVENT_CHILD_ADD)
      {
         priv->mon.cb_count_child_add--;
         if(priv->mon.cb_count_child_add == 0)
           {
             if(ecore_event_handler_del(priv->ecore_child_add_handler) == NULL)
               {
                  fprintf(stderr, "Error deleting ecore_event_handler : %p\n", priv->ecore_child_add_handler);
               }
           }
         return EO_CALLBACK_CONTINUE;
      }
    else if(callback_array->desc == EMODEL_EVENT_CHILD_DEL)
      {
         priv->mon.cb_count_child_del--;
         if(priv->mon.cb_count_child_del == 0)
           {
             if(ecore_event_handler_del(priv->ecore_child_del_handler) == NULL)
               {
                  fprintf(stderr, "Error deleting ecore_event_handler : %p\n", priv->ecore_child_del_handler);
               }
           }
         return EO_CALLBACK_CONTINUE;
      }

    if((priv->mon.cb_count_child_add == 0) && (priv->mon.cb_count_child_del == 0))
      {
         eio_monitor_del(priv->monitor);
      }

    return EINA_TRUE;
}

/* /\** */
/*  *  Callbacks */
/*  *  Children Get */
/*  *\/ */
static void
_eio_main_children_get_cb(void *data, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   Eo *child;

   EINA_SAFETY_ON_NULL_RETURN(cdata);
   EINA_SAFETY_ON_NULL_RETURN(cdata->priv->obj);

   child = eo_add_custom(MY_CLASS, cdata->priv->obj, emodel_eio_constructor(info->path));
   eo_do(child, emodel_eio_children_filter_set(cdata->priv->filter_cb, cdata->priv->filter_userdata)); //XXX: set parent filter to child
   cdata->callback(cdata->data, child, &cdata->cidx);
   eo_unref(child);
   cdata->cidx++;
}

 static Eina_Bool
_eio_filter_children_get_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
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
_eio_done_children_get_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   _emodel_dealloc_memory(cdata, NULL);
}

static void
_eio_error_children_get_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error)
{
   if(0 != error)
     {
        fprintf(stdout, "%s : %d\n", __FUNCTION__, error);
     }
}

/* /\** */
/*  *  Callbacks */
/*  *  Children Slice Get */
/*  *\/ */
static Eina_Bool
_eio_filter_children_slice_get_cb(void *data, Eio_File *handler, const Eina_File_Direct_Info *info)
{
   Eina_Bool ret = EINA_FALSE;
   Emodel_Eio_Children_Data *cdata = data;

   if (_eio_filter_children_get_cb(data, handler, info))
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
_eio_done_children_slice_get_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Children_Data *cdata = data;
   _emodel_dealloc_memory(cdata, NULL);
}

/* /\** */
/*  *  Callbacks */
/*  *  Children Count Get */
/*  *\/ */

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
   eo_do(count_data->priv->obj, eo_event_callback_call(EMODEL_EVENT_CHILDREN_COUNT_GET, &(count_data->total)));
   _emodel_dealloc_memory(count_data, NULL);
}

static void
_eio_error_children_count_get_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error EINA_UNUSED)
{
   fprintf(stdout, "%s : %d\n", __FUNCTION__, error);
}

/* /\\** */
/*  *  Callbacks */
/*  *  Child Del */
/*  *\\/ */
static Eina_Bool
_eio_filter_child_del_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, const Eina_File_Direct_Info *info EINA_UNUSED)
{
   return EINA_TRUE;
}

static void
_eio_progress_child_del_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, const Eio_Progress *info EINA_UNUSED)
{}

static void
_eio_done_unlink_cb(void *data, Eio_File *handler EINA_UNUSED)
{
   Emodel_Eio_Data *priv = data;

   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(priv->obj);
   EINA_SAFETY_ON_NULL_RETURN(priv->emodel_cb);

   // TODO: fix this callback call, same reference twice?
   // TODO: First argument should not be NULL
   priv->emodel_cb(NULL, priv->obj, priv->obj);

   // destruct / unref?
   //eo_do_super(priv->obj, MY_CLASS, eo_destructor());
}

static void
_eio_error_unlink_cb(void *data EINA_UNUSED, Eio_File *handler EINA_UNUSED, int error)
{
   fprintf(stdout, "%s : %d\n", __FUNCTION__, error);
}


/* /\\** */
/*  * Interfaces impl. */
/*  *\\/ */
static void
_emodel_eio_emodel_properties_get(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd)
{
   Emodel_Eio_Data *priv = _pd;
   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(priv->obj);
   eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTIES_CHANGE, priv->properties));
}

/* /\\** */
/*  * Property Get */
/*  *\\/ */
static void
_emodel_eio_emodel_property_get(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd, const char *property)
{
   Emodel_Property_EVT evt;
   Emodel_Eio_Data *priv = _pd;

   EINA_SAFETY_ON_NULL_RETURN(property);
   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(priv->obj);

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &evt.prop);
   if (!strncmp(property, evt.prop, strlen(evt.prop)))
     {
        evt.value = _emodel_property_value_get(priv, evt.prop);
        eo_do(priv->obj, eo_event_callback_call(EMODEL_EVENT_PROPERTY_CHANGE, &evt));
        return;
     }

   priv->file = eio_file_direct_stat(priv->path, _eio_stat_done_cb, _eio_error_cb, priv);
}


/* /\\** */
/*  * Property Set */
/*  *\\/ */
static void
_emodel_eio_emodel_property_set(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd, const char *property, Eina_Value *value)
{
   Emodel_Eio_Data *priv = _pd;
   const char *dest, *prop = NULL;

   EINA_SAFETY_ON_NULL_RETURN(value);
   EINA_SAFETY_ON_NULL_RETURN(property);

   eina_value_array_get(priv->properties, EMODEL_EIO_PROP_FILENAME, &prop);
   EINA_SAFETY_ON_NULL_RETURN(prop);

   if (!strncmp(property, prop, strlen(prop)))
     {
        const char *src;
        Eina_Value *value = _emodel_property_value_get(priv, prop);
        eina_value_get(value, &src);
        eina_value_get(value, &dest);
        _emodel_dealloc_memory(priv->path, NULL);
        priv->path = strdup(dest);
        priv->file = eio_file_move(src, dest, _eio_progress_cb, _eio_move_done_cb, _eio_property_set_error_cb, priv);
     }
}

/* /\\** */
/*  * Load */
/*  *\\/ */
 static void
_emodel_eio_emodel_load(Eo *obj  EINA_UNUSED, Emodel_Eio_Data *_pd EINA_UNUSED)
{
   //TODO: implement
}

/* /\\** */
/*  * Unload */
/*  *\\/ */
static void
_emodel_eio_emodel_unload(Eo *obj  EINA_UNUSED, Emodel_Eio_Data *_pd EINA_UNUSED)
{
   //TODO: implement
}

/* /\\** */
/*  * Child Add */
/*  *\\/ */
static void
_emodel_eio_dir_add(Eo *obj, Emodel_Eio_Data *_pd, Emodel_Cb child_add_cb, const char *name)
{
   size_t len;
   Emodel_Eio_Child_Add *child = calloc(1, sizeof(Emodel_Eio_Child_Add));
   EINA_SAFETY_ON_NULL_RETURN(child);

   child->callback = child_add_cb;

   child->name = calloc(1, strlen(name)+1);
   EINA_SAFETY_ON_NULL_GOTO(child->name, cleanup_bottom);

   strncpy(child->name, name, strlen(name));
   child->priv = _pd;
   len = strlen(child->priv->path) + strlen(child->name);

   // len + '/' + '\0'
   child->fullpath = calloc(1, len+2);
   EINA_SAFETY_ON_NULL_GOTO(child->fullpath, cleanup_top);

   //gets current mask
   mode_t mode = umask(0);
   umask(mode);

   //_eio_done_mkdir_cb frees memory
   strncpy(child->fullpath, child->priv->path, strlen(child->priv->path));
   strncat(child->fullpath, "/", 1);
   strncat(child->fullpath, child->name, strlen(child->name));

   eio_file_mkdir(child->fullpath, (0777 - mode),
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
_emodel_eio_do_child_del(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd, Emodel_Cb child_del_cb)
{
   Emodel_Eio_Data *priv = _pd;
   priv->emodel_cb = child_del_cb;

   EINA_SAFETY_ON_NULL_RETURN(priv);
   EINA_SAFETY_ON_NULL_RETURN(priv->emodel_cb);

   /*
    * TODO:
    * eio_file_unlink can't delete directories
    * In order to use a more correct approach I need
    * einther to find proper EIO API or implement
    * verification by hand (hmm bad).
    * !! this will delete directories recursively too !!
    */
   eio_dir_unlink(priv->path,
                  _eio_filter_child_del_cb,
                  _eio_progress_child_del_cb,
                  _eio_done_unlink_cb,
                  _eio_error_unlink_cb,
                  priv);
}

static void
_emodel_eio_children_filter_set(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd, Eio_Filter_Direct_Cb filter_cb, void *data)
{
   Emodel_Eio_Data *priv = _pd;

   priv->filter_cb = filter_cb;
   priv->filter_userdata = data;
}

/* /\\** */
/*  * Child Del */
/*  *\\/ */
static void
_emodel_eio_emodel_child_del(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd EINA_UNUSED, Emodel_Cb cb, Eo *child)
{
   EINA_SAFETY_ON_NULL_RETURN(cb);
   EINA_SAFETY_ON_NULL_RETURN(child);

   eo_do(child, emodel_eio_child_del(cb));
}

/* /\\** */
/*  * Children Get */
/*  *\\/ */
static void
_emodel_eio_emodel_children_get(Eo *obj , Emodel_Eio_Data *_pd, Emodel_Cb callback, void *data)
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
   cdata->lsref = eio_file_direct_ls(cdata->priv->path, _eio_filter_children_get_cb,
                      _eio_main_children_get_cb, _eio_done_children_get_cb, _eio_error_children_get_cb, cdata);
}


/* /\\** */
/*  * Children Slice Get */
/*  *\\/ */
static void
_emodel_eio_emodel_children_slice_get(Eo *obj, Emodel_Eio_Data *_pd, Emodel_Cb children_slice_get_cb, int start, int count, void *data)
{
    Emodel_Eio_Children_Data *cdata;

    EINA_SAFETY_ON_NULL_RETURN(children_slice_get_cb);

    cdata = calloc(1, sizeof(Emodel_Eio_Children_Data));
    EINA_SAFETY_ON_NULL_RETURN(cdata);

    EINA_SAFETY_ON_FALSE_RETURN(start >= 0);
    EINA_SAFETY_ON_FALSE_RETURN(count > 0);

    cdata->data = data;
    cdata->callback = children_slice_get_cb;
    cdata->priv = _pd;
    cdata->priv->obj = obj;
    cdata->start = start;
    cdata->count = count + start;
    cdata->idx = 0;
    cdata->cidx = cdata->start;

    EINA_SAFETY_ON_FALSE_RETURN(eo_ref_get(cdata->priv->obj));
    cdata->lsref = eio_file_direct_ls(cdata->priv->path,
                       _eio_filter_children_slice_get_cb,
                       _eio_main_children_get_cb,
                       _eio_done_children_slice_get_cb,
                       _eio_error_children_get_cb,
                       cdata);
}

/* /\\** */
/*  * Children Count Get */
/*  *\\/ */
static void
_emodel_eio_emodel_children_count_get(Eo *obj EINA_UNUSED, Emodel_Eio_Data *_pd)
{
   Emodel_Eio_Data *priv = _pd;
   Emodel_Eio_Children_Count *count_data = calloc(1, sizeof(Emodel_Eio_Children_Count));
   EINA_SAFETY_ON_NULL_RETURN(count_data);
   count_data->priv = priv;

   eio_file_direct_ls(priv->path, _eio_filter_children_get_cb,
                      _eio_main_children_count_get_cb, _eio_done_children_count_get_cb,
                      _eio_error_children_count_get_cb, count_data);
}

/* /\\** */
/*  * Class definitions */
/*  *\\/ */
static void
_emodel_eio_constructor(Eo *obj , Emodel_Eio_Data *_pd, const char *path)
{ 
   Emodel_Eio_Data *priv = _pd;
   const char *prop;
   priv->path = strdup(path);
   Eina_Value *v;
   size_t i;

   eo_do_super(obj, MY_CLASS, eo_constructor());

   priv->properties = eina_value_array_new(EINA_VALUE_TYPE_STRING, 0);
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_FILENAME, "filename");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_IS_DIR, "is_dir");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_IS_LNK, "is_lnk");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_SIZE, "size");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_MTIME, "mtime");
   eina_value_array_insert(priv->properties, EMODEL_EIO_PROP_ICON, "icon");

   for (i = 0; i != PROP_LIST_SIZE; i++)
     {
        switch(i) {
        case EMODEL_EIO_PROP_FILENAME:
          v = eina_value_new(EINA_VALUE_TYPE_STRING);
          eina_value_set(v, path);
          break;
        case EMODEL_EIO_PROP_MTIME:
          v = eina_value_new(EINA_VALUE_TYPE_TIMEVAL);
          break;
        case EMODEL_EIO_PROP_ICON:
          v = eina_value_new(EINA_VALUE_TYPE_STRING);
          break;
        default:
          v = eina_value_new(EINA_VALUE_TYPE_INT);
        }

        eina_value_array_get(priv->properties, i, &prop);
        priv->proplist[i].v = v;
        priv->proplist[i].prop = prop;
     }

   eo_do(obj, eo_event_callback_add(EO_EV_CALLBACK_ADD, _eio_monitor_evt_added_cb, NULL),
         eo_event_callback_add(EO_EV_CALLBACK_DEL, _eio_monitor_evt_deleted_cb, NULL));

   priv->obj = obj;
   eio_init();
}

static void
_emodel_eio_destructor(Eo *obj , Emodel_Eio_Data *_pd EINA_UNUSED)
{
   //TODO: free data
   eio_shutdown();
   eo_do_super(obj, MY_CLASS, eo_destructor());
}

/* static void */
/* _emodel_eio_class_constructor(Eo_Class *klass) */
/* { */
/*    const Eo_Op_Func_Description func_descs[] = { */
/*       EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _emodel_eio_destructor), */
/*       EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _emodel_eio_constructor), */
/*       EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_PROPERTIES_GET), _emodel_eio_properties_get), */
/*       EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_PROPERTY_GET), _emodel_eio_property_get), */
/*       EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_PROPERTY_SET), _emodel_eio_property_set), */
/*       EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_LOAD), _emodel_eio_load), */
/*       EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_UNLOAD), _emodel_eio_unload), */

/*       //test */
/*       EO_OP_FUNC(EMODEL_EIO_ID(EMODEL_EIO_OBJ_SUB_ID_DIR_ADD), _emodel_eio_dir_add), */
/*       EO_OP_FUNC(EMODEL_EIO_ID(EMODEL_EIO_OBJ_SUB_ID_CHILD_DEL), _emodel_eio_do_child_del), */
/*       EO_OP_FUNC(EMODEL_EIO_ID(EMODEL_EIO_OBJ_SUB_ID_CHILDREN_FILTER_SET), _emodel_eio_children_filter_set), */

/*       EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_CHILD_DEL), _emodel_eio_del), */
/*       EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_CHILDREN_GET), _emodel_eio_children_get), */
/*       EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_CHILDREN_SLICE_GET), _emodel_eio_children_slice_get), */
/*       EO_OP_FUNC(EMODEL_ID(EMODEL_OBJ_SUB_ID_CHILDREN_COUNT_GET), _emodel_eio_children_count_get), */
/*       EO_OP_FUNC_SENTINEL */
/*    }; */

/*    eo_class_funcs_set(klass, func_descs); */
/* } */

/* static const Eo_Op_Description op_desc[] = { */
/*      EO_OP_DESCRIPTION(EMODEL_EIO_OBJ_SUB_ID_CONSTRUCTOR, "Eio file constructor."), */
/*      EO_OP_DESCRIPTION(EMODEL_EIO_OBJ_SUB_ID_DIR_ADD, "Add new child."), */
/*      EO_OP_DESCRIPTION(EMODEL_EIO_OBJ_SUB_ID_CHILD_DEL, "Delete child."), */
/*      EO_OP_DESCRIPTION(EMODEL_EIO_OBJ_SUB_ID_CHILDREN_FILTER_SET, "Set filter children callback."), */
/*      EO_OP_DESCRIPTION_SENTINEL */
/* }; */

/* static const Eo_Class_Description class_desc = { */
/*      EO_VERSION, */
/*      MY_CLASS_NAME, */
/*      EO_CLASS_TYPE_REGULAR, */
/*      EO_CLASS_DESCRIPTION_OPS(&EMODEL_EIO_BASE_ID, op_desc, EMODEL_EIO_OBJ_SUB_ID_LAST), */
/*      NULL, */
/*      sizeof(Emodel_Eio), */
/*      _emodel_eio_class_constructor, */
/*      NULL */
/* }; */

/* EO_DEFINE_CLASS(emodel_obj_eio_class_get, &class_desc, EO_BASE_CLASS, EMODEL_CLASS, NULL); */

#include "emodel_eio.eo.c"
