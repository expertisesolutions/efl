#ifndef _EMODEL_EIO_PRIVATE_H
#define _EMODEL_EIO_PRIVATE_H

#define PROP_LIST_SIZE 5

enum {
   EMODEL_EIO_PROP_FILENAME,
   EMODEL_EIO_PROP_IS_DIR,
   EMODEL_EIO_PROP_IS_LNK,
   EMODEL_EIO_PROP_SIZE,
   EMODEL_EIO_PROP_MTIME
};

typedef struct _Emodel_Eio                      Emodel_Eio;
typedef struct _Emodel_Eio_Properties           Emodel_Eio_Properties;
typedef struct _Emodel_Eio_Children_Data        Emodel_Eio_Children_Data;
typedef struct _Emodel_Eio_Child_Add            Emodel_Eio_Child_Add;
typedef struct _Emodel_Eio_Child_Add            Emodel_Eio_Child_Del;
typedef struct _Emodel_Eio_Children_Count       Emodel_Eio_Children_Count;

struct _Emodel_Eio_Properties
{
   Eina_Value *v;
   const char *prop;
};

struct _Emodel_Eio
{
   Eo *obj;
   Eina_Value *properties;
   Eio_File *file;
   char *path;
   const Eina_Stat *stat;
   Eio_Monitor *monitor;
   Emodel_Cb emodel_cb; //XXX
   Ecore_Event_Handler *ecore_child_add_handler;
   Ecore_Event_Handler *ecore_child_del_handler;
   Emodel_Eio_Monitor_Data mon;
   Emodel_Eio_Properties proplist[PROP_LIST_SIZE];
};


struct _Emodel_Eio_Child_Add
{
   Emodel_Cb callback;
   Eo *child;
   Emodel_Eio *priv;
   char* fullpath;
   char *name;
};

struct _Emodel_Eio_Children_Count
{
   Emodel_Eio *priv;
   size_t total;
};

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
_emodel_property_value_get(const Emodel_Eio *priv, const char *prop)
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
