#ifndef _EMODEL_EIO_PRIVATE_H
#define _EMODEL_EIO_PRIVATE_H

#define PROP_LIST_SIZE 6

typedef struct _Emodel_Eio_Data                 Emodel_Eio_Data;
typedef struct _Emodel_Eio_Properties           Emodel_Eio_Properties;
typedef struct _Emodel_Eio_Children_Data        Emodel_Eio_Children_Data;
typedef struct _Emodel_Eio_Child_Add            Emodel_Eio_Child_Add;
typedef struct _Emodel_Eio_Child_Add            Emodel_Eio_Child_Del;
typedef struct _Emodel_Eio_Children_Count       Emodel_Eio_Children_Count;
typedef struct _Emodel_Eio_Monitor_Data         Emodel_Eio_Monitor_Data;

enum {
   EMODEL_MON_DEL_DIR_DELETED,
   EMODEL_MON_DEL_FILE_DELETED,
};

struct _Emodel_Eio_Properties
{
   Eina_Value *v;
   const char *prop;
};

struct _Emodel_Eio_Monitor_Data
{
   Ecore_Event_Handler *ecore_child_add_handler[2];
   Ecore_Event_Handler *ecore_child_del_handler[3];
   int mon_event_child_add[3]; /**< plus EIO_MONITOR_ERROR */
   int mon_event_child_del[3]; /**< plus EIO_MONITOR_ERROR */
};

enum {
   EMODEL_EIO_PROP_FILENAME,
   EMODEL_EIO_PROP_IS_DIR,
   EMODEL_EIO_PROP_IS_LNK,
   EMODEL_EIO_PROP_SIZE,
   EMODEL_EIO_PROP_MTIME,
   EMODEL_EIO_PROP_ICON
};

struct _Emodel_Eio_Data
{
   Eo *obj;
   Eo *rootmodel;
   Eina_Value *properties;
   Eio_File *file;
   char *path;
   char *pathSelected;
   const Eina_Stat *stat;
   Eio_Monitor *monitor;
   Emodel_Cb emodel_cb; //XXX
   Emodel_Eio_Monitor_Data mon;
   Emodel_Eio_Properties proplist[PROP_LIST_SIZE];
   Eio_Filter_Direct_Cb filter_cb;
   void *filter_userdata;
   int cb_count_child_add; /**< monitor reference counter for child add event */
   int cb_count_child_del; /**< monitor reference counter for child del event*/
};

struct _Emodel_Eio_Child_Add
{
   Emodel_Cb callback;
   Eo *child;
   Emodel_Eio_Data *priv;
   char* fullpath;
   char *name;
};

struct _Emodel_Eio_Children_Count
{
   Emodel_Eio_Data *priv;
   size_t total;
};

struct _Emodel_Eio_Children_Data
{
   Emodel_Eio_Data *priv;
   Eio_File *lsref;
   void *data;
   Emodel_Cb callback;
   Eina_Bool dispatch;
   int start;
   int count;
   int idx;
   int cidx;
};

static inline void 
_emodel_dealloc_memory(void *ptr, ...)
{
   va_list al;
   void *data;
   va_start(al,ptr);

   for(data = ptr; data != NULL; data = va_arg(al, void*))
     {
        free(data);
        data = NULL;
     }
   va_end(al);
}

static inline Eina_Value*
_emodel_property_value_get(const Emodel_Eio_Data *priv, const char *prop)
{
   if((NULL == priv) || (NULL == prop)) return NULL;
   for(unsigned int i = 0; i != PROP_LIST_SIZE; ++i)
     {
        if(!strncmp(priv->proplist[i].prop, prop, strlen(priv->proplist[i].prop)))
          {
             return priv->proplist[i].v;
          }
     }
   return NULL;
}

#endif
