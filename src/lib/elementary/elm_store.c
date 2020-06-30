#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include <Elementary_Cursor.h>

#include "elm_priv.h"
#include "elm_genlist_eo.h"

typedef struct _Elm_Store_Filesystem           Elm_Store_Filesystem;
typedef struct _Elm_Store_Item_Filesystem      Elm_Store_Item_Filesystem;

#define ELM_STORE_MAGIC            0x3f89ea56
#define ELM_STORE_FILESYSTEM_MAGIC 0x3f89ea57
#define ELM_STORE_ITEM_MAGIC       0x5afe8c1d

struct _Elm_Store
{
   EINA_MAGIC;
   void           (*free)(Elm_Store *store);
   struct
     {
        void        (*free)(Elm_Store_Item *item);
     } item;
   Evas_Object   *genlist;
   Ecore_Thread  *list_th;
   Eina_Inlist   *items;
   Eina_List     *realized;
   int            realized_count;
   int            cache_max;
   struct
     {
        struct
          {
             Elm_Store_Item_List_Cb     func;
             void                      *data;
          } list;
        struct
          {
             Elm_Store_Item_Fetch_Cb    func;
             void                      *data;
          } fetch;
        struct
          {
             Elm_Store_Item_Unfetch_Cb  func;
             void                      *data;
          } unfetch;
     } cb;
   Eina_Bool sorted : 1;
   Eina_Bool fetch_thread : 1;
};

struct _Elm_Store_Item
{
   EINA_INLIST;
   EINA_MAGIC;
   Elm_Store                    *store;
   Elm_Object_Item              *item;
   Ecore_Thread                 *fetch_th;
   Ecore_Job                    *eval_job;
   const Elm_Store_Item_Mapping *mapping;
   void                         *data;
   Eina_Lock                     lock;
   Eina_Bool                     live : 1;
   Eina_Bool                     was_live : 1;
   Eina_Bool                     realized : 1;
   Eina_Bool                     fetched : 1;
};

struct _Elm_Store_Filesystem
{
   Elm_Store base;
   EINA_MAGIC;
   const char *dir;
};

struct _Elm_Store_Item_Filesystem
{
   Elm_Store_Item base;
   const char *path;
};

static Elm_Genlist_Item_Class _store_item_class;

static void
_store_cache_trim(Elm_Store *st)
{
   while ((st->realized) &&
          (((int)eina_list_count(st->realized) - st->realized_count)
           > st->cache_max))
     {
        Elm_Store_Item *sti = st->realized->data;
        if (sti->realized)
          {
             st->realized = eina_list_remove_list(st->realized, st->realized);
             sti->realized = EINA_FALSE;
          }
        eina_lock_take(&sti->lock);
        if (!sti->fetched)
          {
             eina_lock_release(&sti->lock);
             ELM_SAFE_FREE(sti->fetch_th, ecore_thread_cancel);
             eina_lock_take(&sti->lock);
          }
        sti->fetched = EINA_FALSE;
//// let fetch/unfetch do the locking
//        eina_lock_release(&sti->lock);
        if (st->cb.unfetch.func)
          st->cb.unfetch.func(st->cb.unfetch.data, sti);
//        eina_lock_take(&sti->lock);
        sti->data = NULL;
        eina_lock_release(&sti->lock);
     }
}

static void
_store_genlist_del(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Elm_Store *st = data;
   st->genlist = NULL;
   ELM_SAFE_FREE(st->list_th, ecore_thread_cancel);
   st->realized = eina_list_free(st->realized);
   while (st->items)
     {
        Elm_Store_Item *sti = (Elm_Store_Item *)st->items;
        ELM_SAFE_FREE(sti->eval_job, ecore_job_del);
        ELM_SAFE_FREE(sti->fetch_th, ecore_thread_cancel);
        if (sti->store->item.free) sti->store->item.free(sti);
        eina_lock_take(&sti->lock);
        if (sti->data)
          {
             if (st->cb.unfetch.func)
               st->cb.unfetch.func(st->cb.unfetch.data, sti);
             sti->data = NULL;
          }
        eina_lock_release(&sti->lock);
        eina_lock_free(&sti->lock);
        st->items = eina_inlist_remove(st->items, EINA_INLIST_GET(sti));
        free(sti);
     }
   // FIXME: kill threads and more
}

////// **** WARNING ***********************************************************
////   * This function runs inside a thread outside efl mainloop. Be careful! *
//     ************************************************************************
/* TODO: refactor lock part into core? this does not depend on filesystem part */
static void
_store_filesystem_fetch_do(void *data, Ecore_Thread *th EINA_UNUSED)
{
   Elm_Store_Item *sti = data;
   eina_lock_take(&sti->lock);
   if (sti->data)
     {
        eina_lock_release(&sti->lock);
        return;
     }
   if (!sti->fetched)
     {
//// let fetch/unfetch do the locking
//        eina_lock_release(&sti->lock);
        if (sti->store->cb.fetch.func)
          sti->store->cb.fetch.func(sti->store->cb.fetch.data, sti);
//        eina_lock_take(&sti->lock);
        sti->fetched = EINA_TRUE;
     }
   eina_lock_release(&sti->lock);
}
//     ************************************************************************
////   * End of separate thread function.                                     *
////// ************************************************************************
/* TODO: refactor lock part into core? this does not depend on filesystem part */
static void
_store_filesystem_fetch_end(void *data, Ecore_Thread *th)
{
   Elm_Store_Item *sti = data;
   eina_lock_take(&sti->lock);
   if (sti->data) elm_genlist_item_update(sti->item);
   eina_lock_release(&sti->lock);
   if (th == sti->fetch_th) sti->fetch_th = NULL;
}

/* TODO: refactor lock part into core? this does not depend on filesystem part */
static void
_store_filesystem_fetch_cancel(void *data, Ecore_Thread *th)
{
   Elm_Store_Item *sti = data;
   eina_lock_take(&sti->lock);
   if (th == sti->fetch_th) sti->fetch_th = NULL;
   if (sti->data) elm_genlist_item_update(sti->item);
   eina_lock_release(&sti->lock);
}

static void
_store_item_eval(void *data)
{
   Elm_Store_Item *sti = data;
   sti->eval_job = NULL;
   if (sti->live == sti->was_live) return;
   sti->was_live = sti->live;
   if (sti->live)
     {
        _store_cache_trim(sti->store);
        if (sti->realized)
          sti->store->realized = eina_list_remove(sti->store->realized, sti);
        sti->store->realized = eina_list_append(sti->store->realized, sti);
        sti->realized = EINA_TRUE;
        if ((sti->store->fetch_thread) && (!sti->fetch_th))
          sti->fetch_th = ecore_thread_run(_store_filesystem_fetch_do,
                                           _store_filesystem_fetch_end,
                                           _store_filesystem_fetch_cancel,
                                           sti);
        else if ((!sti->store->fetch_thread))
          {
             _store_filesystem_fetch_do(sti, NULL);
             _store_filesystem_fetch_end(sti, NULL);
          }
     }
   else
     {
        ELM_SAFE_FREE(sti->fetch_th, ecore_thread_cancel);
        _store_cache_trim(sti->store);
     }
}

static void
_store_genlist_item_realized(void *data, const Efl_Event *event)
{
   Elm_Store *st = data;
   Elm_Object_Item *gli = event->info;
   Elm_Store_Item *sti = elm_object_item_data_get(gli);
   if (!sti) return;
   st->realized_count++;
   sti->live = EINA_TRUE;
   ecore_job_del(sti->eval_job);
   sti->eval_job = ecore_job_add(_store_item_eval, sti);
}

static void
_store_genlist_item_unrealized(void *data, const Efl_Event *event)
{
   Elm_Store *st = data;
   Elm_Object_Item *gli = event->info;
   Elm_Store_Item *sti = elm_object_item_data_get(gli);
   if (!sti) return;
   st->realized_count--;
   sti->live = EINA_FALSE;
   ecore_job_del(sti->eval_job);
   sti->eval_job = ecore_job_add(_store_item_eval, sti);
}

static const Elm_Store_Item_Mapping *
_store_item_mapping_find(Elm_Store_Item *sti, const char *part)
{
   const Elm_Store_Item_Mapping *m;

   for (m = sti->mapping; m; m ++)
     {
        if (m->type == ELM_STORE_ITEM_MAPPING_NONE) break;
        if (!strcmp(part, m->part)) return m;
     }
   return NULL;
}

static char *
_store_item_text_get(void *data, Evas_Object *obj EINA_UNUSED, const char *part)
{
   Elm_Store_Item *sti = data;
   const char *s = "";
   eina_lock_take(&sti->lock);
   if (sti->data)
     {
        const Elm_Store_Item_Mapping *m = _store_item_mapping_find(sti, part);
        if (m)
          {
             switch (m->type)
               {
                case ELM_STORE_ITEM_MAPPING_LABEL:
                   s = *(char **)(((unsigned char *)sti->data) + m->offset);
                   break;
                case ELM_STORE_ITEM_MAPPING_CUSTOM:
                   if (m->details.custom.func)
                     s = m->details.custom.func(sti->data, sti, part);
                   break;
                default:
                   break;
               }
          }
     }
   eina_lock_release(&sti->lock);
   return s ? strdup(s) : NULL;
}

static Evas_Object *
_store_item_content_get(void *data, Evas_Object *obj, const char *part)
{
   Elm_Store_Item *sti = data;
   eina_lock_take(&sti->lock);
   if (sti->data)
     {
        const Elm_Store_Item_Mapping *m = _store_item_mapping_find(sti, part);
        if (m)
          {
             Evas_Object *ic = NULL;
             const char *s = NULL;

             switch (m->type)
               {
                case ELM_STORE_ITEM_MAPPING_ICON:
                   ic = elm_icon_add(obj);
                   s = *(char **)(((unsigned char *)sti->data) + m->offset);
                   evas_object_size_hint_aspect_set(ic,
                                                    EVAS_ASPECT_CONTROL_VERTICAL,
                                                    m->details.icon.w,
                                                    m->details.icon.h);
                   elm_image_smooth_set(ic, m->details.icon.smooth);
                   elm_image_no_scale_set(ic, m->details.icon.no_scale);
                   elm_image_resizable_set(ic,
                                      m->details.icon.scale_up,
                                      m->details.icon.scale_down);
                   if (s)
                     {
                        if (m->details.icon.standard_name)
                          elm_icon_standard_set(ic, s);
                        else
                          elm_image_file_set(ic, s, NULL);
                     }
                   break;
                case ELM_STORE_ITEM_MAPPING_PHOTO:
                   ic = elm_icon_add(obj);
                   s = *(char **)(((unsigned char *)sti->data) + m->offset);
                   elm_photo_size_set(ic, m->details.photo.size);
                   if (s)
                     elm_photo_file_set(ic, s);
                   break;
                case ELM_STORE_ITEM_MAPPING_CUSTOM:
                   if (m->details.custom.func)
                     ic = m->details.custom.func(sti->data, sti, part);
                   break;
                default:
                   break;
               }
             eina_lock_release(&sti->lock);
             return ic;
          }
     }
   eina_lock_release(&sti->lock);
   return NULL;
}

static void
_store_item_del(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED)
{
}

////// **** WARNING ***********************************************************
////   * This function runs inside a thread outside efl mainloop. Be careful! *
//     ************************************************************************
static int
_store_filesystem_sort_cb(void *d1, void *d2)
{
   Elm_Store_Item_Info *info1 = d1, *info2 = d2;
   if ((!info1->sort_id) || (!info2->sort_id)) return 0;
   return strcoll(info1->sort_id, info2->sort_id);
}

static void
_store_filesystem_list_do(void *data, Ecore_Thread *th EINA_UNUSED)
{
   Elm_Store_Filesystem *st = data;
   Eina_Iterator *it;
   const Eina_File_Direct_Info *finf;
   Eina_List *sorted = NULL;
   Elm_Store_Item_Info_Filesystem *info;

   // FIXME: need a way to abstract the open, list, feed items from list
   // and maybe get initial sortable key vals etc.
   it = eina_file_stat_ls(st->dir);
   if (!it) return;
   EINA_ITERATOR_FOREACH(it, finf)
     {
        Eina_Bool ok;
        size_t pathsz = finf->path_length + 1;

        if (finf->path[finf->name_start] == '.') continue ;

        info = calloc(1, sizeof(Elm_Store_Item_Info_Filesystem) + pathsz);
        if (!info) continue;
        info->path = ((char *)info) + sizeof(Elm_Store_Item_Info_Filesystem);
        memcpy(info->path, finf->path, pathsz);
        ok = EINA_TRUE;
        if (st->base.cb.list.func)
          ok = st->base.cb.list.func(st->base.cb.list.data, &info->base);
        if (ok)
          {
             if (!st->base.sorted) ecore_thread_feedback(th, info);
             else sorted = eina_list_append(sorted, info);
          }
        else
          {
             free(info->base.sort_id);
             free(info);
          }
        if (ecore_thread_check(th)) break;
     }
   eina_iterator_free(it);
   if (sorted)
     {
        sorted = eina_list_sort(sorted, 0,
                                EINA_COMPARE_CB(_store_filesystem_sort_cb));
        EINA_LIST_FREE(sorted, info)
          {
             if (!ecore_thread_check(th)) ecore_thread_feedback(th, info);
          }
     }
}
//     ************************************************************************
////   * End of separate thread function.                                     *
////// ************************************************************************

static void
_store_filesystem_list_end(void *data, Ecore_Thread *th)
{
   Elm_Store *st = data;
   if (th == st->list_th) st->list_th = NULL;
}

static void
_store_filesystem_list_cancel(void *data, Ecore_Thread *th)
{
   Elm_Store *st = data;
   if (th == st->list_th) st->list_th = NULL;
}

static void
_store_filesystem_list_update(void *data, Ecore_Thread *th EINA_UNUSED, void *msg)
{
   Elm_Store *st = data;
   Elm_Store_Item_Filesystem *sti;
   Elm_Genlist_Item_Class *itc;
   Elm_Store_Item_Info_Filesystem *info = msg;

   sti = calloc(1, sizeof(Elm_Store_Item_Filesystem));
   if (!sti) goto done;
   eina_lock_new(&sti->base.lock);
   EINA_MAGIC_SET(&(sti->base), ELM_STORE_ITEM_MAGIC);
   sti->base.store = st;
   sti->base.data = info->base.data;
   sti->base.mapping = info->base.mapping;
   sti->path = eina_stringshare_add(info->path);

   itc = info->base.item_class;
   if (!itc) itc = &_store_item_class;
   else
     {
        itc->func.text_get = _store_item_text_get;
        itc->func.content_get  = _store_item_content_get;
        itc->func.state_get = NULL; // FIXME: support state gets later
        itc->func.del       = _store_item_del;
     }

   // FIXME: handle being a parent (tree)
   sti->base.item = elm_genlist_item_append(st->genlist, itc,
                                            sti/* item data */,
                                            NULL/* parent */,
                                            ELM_GENLIST_ITEM_NONE,
                                            NULL/* func */,
                                            NULL/* func data */);
   st->items = eina_inlist_append(st->items, (Eina_Inlist *)sti);
done:
   free(info->base.sort_id);
   free(info);
}

// public api calls
static Elm_Store *
_elm_store_new(size_t size)
{
   Elm_Store *st = calloc(1, size);
   EINA_SAFETY_ON_NULL_RETURN_VAL(st, NULL);

   // TODO: BEGIN - move to elm_store_init()
   eina_magic_string_set(ELM_STORE_MAGIC, "Elm_Store");
   eina_magic_string_set(ELM_STORE_FILESYSTEM_MAGIC, "Elm_Store_Filesystem");
   eina_magic_string_set(ELM_STORE_ITEM_MAGIC, "Elm_Store_Item");
   // setup default item class (always the same) if list cb doesn't provide one
   _store_item_class.item_style = "default";
   _store_item_class.func.text_get = _store_item_text_get;
   _store_item_class.func.content_get  = _store_item_content_get;
   _store_item_class.func.state_get = NULL; // FIXME: support state gets later
   _store_item_class.func.del       = _store_item_del;
   // TODO: END - move to elm_store_init()

   EINA_MAGIC_SET(st, ELM_STORE_MAGIC);
   st->cache_max = 128;
   st->fetch_thread = EINA_TRUE;
   return st;
}
#define elm_store_new(type) (type*)_elm_store_new(sizeof(type))

static void
_elm_store_filesystem_free(Elm_Store *store)
{
   Elm_Store_Filesystem *st = (Elm_Store_Filesystem *)store;
   eina_stringshare_del(st->dir);
}

static void
_elm_store_filesystem_item_free(Elm_Store_Item *item)
{
   Elm_Store_Item_Filesystem *sti = (Elm_Store_Item_Filesystem *)item;
   eina_stringshare_del(sti->path);
}

ELM_API Elm_Store *
elm_store_filesystem_new(void)
{
   Elm_Store_Filesystem *st = elm_store_new(Elm_Store_Filesystem);
   EINA_SAFETY_ON_NULL_RETURN_VAL(st, NULL);

   EINA_MAGIC_SET(st, ELM_STORE_FILESYSTEM_MAGIC);
   st->base.free = _elm_store_filesystem_free;
   st->base.item.free = _elm_store_filesystem_item_free;

   return &st->base;
}

ELM_API void
elm_store_free(Elm_Store *st)
{
   void (*item_free)(Elm_Store_Item *);
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return;
   ELM_SAFE_FREE(st->list_th, ecore_thread_cancel);
   st->realized = eina_list_free(st->realized);
   item_free = st->item.free;
   while (st->items)
     {
        Elm_Store_Item *sti = (Elm_Store_Item *)st->items;
        ELM_SAFE_FREE(sti->eval_job, ecore_job_del);
        ELM_SAFE_FREE(sti->fetch_th, ecore_thread_cancel);
        if (item_free) item_free(sti);
        eina_lock_take(&sti->lock);
        if (sti->data)
          {
             if (st->cb.unfetch.func)
               st->cb.unfetch.func(st->cb.unfetch.data, sti);
             sti->data = NULL;
          }
        eina_lock_release(&sti->lock);
        eina_lock_free(&sti->lock);
        st->items = eina_inlist_remove(st->items, EINA_INLIST_GET(sti));
        free(sti);
     }
   if (st->genlist)
     {
        evas_object_event_callback_del_full(st->genlist, EVAS_CALLBACK_DEL, _store_genlist_del, st);
        efl_event_callback_del(st->genlist, ELM_GENLIST_EVENT_REALIZED, _store_genlist_item_realized, st);
        efl_event_callback_del(st->genlist, ELM_GENLIST_EVENT_UNREALIZED, _store_genlist_item_unrealized, st);
        elm_genlist_clear(st->genlist);
        st->genlist = NULL;
     }
   if (st->free) st->free(st);
   free(st);
}

ELM_API void
elm_store_target_genlist_set(Elm_Store *st, Evas_Object *obj)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return;
   if (st->genlist == obj) return;
   if (st->genlist)
     {
        evas_object_event_callback_del_full(st->genlist, EVAS_CALLBACK_DEL, _store_genlist_del, st);
        efl_event_callback_del(st->genlist, ELM_GENLIST_EVENT_REALIZED, _store_genlist_item_realized, st);
        efl_event_callback_del(st->genlist, ELM_GENLIST_EVENT_UNREALIZED, _store_genlist_item_unrealized, st);
        elm_genlist_clear(st->genlist);
     }
   st->genlist = obj;
   if (!st->genlist) return;
   efl_event_callback_add(st->genlist, ELM_GENLIST_EVENT_REALIZED, _store_genlist_item_realized, st);
   efl_event_callback_add(st->genlist, ELM_GENLIST_EVENT_UNREALIZED, _store_genlist_item_unrealized, st);
   evas_object_event_callback_add(st->genlist, EVAS_CALLBACK_DEL, _store_genlist_del, st);
   elm_genlist_clear(st->genlist);
}

ELM_API void
elm_store_filesystem_directory_set(Elm_Store *store, const char *dir)
{
   Elm_Store_Filesystem *st = (Elm_Store_Filesystem *)store;
   if (!EINA_MAGIC_CHECK(store, ELM_STORE_MAGIC)) return;
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_FILESYSTEM_MAGIC)) return;
   ELM_SAFE_FREE(store->list_th, ecore_thread_cancel);
   if (!eina_stringshare_replace(&st->dir, dir)) return;
   store->list_th = ecore_thread_feedback_run(_store_filesystem_list_do,
                                              _store_filesystem_list_update,
                                              _store_filesystem_list_end,
                                              _store_filesystem_list_cancel,
                                              st, EINA_TRUE);
}

ELM_API const char *
elm_store_filesystem_directory_get(const Elm_Store *store)
{
   const Elm_Store_Filesystem *st = (const Elm_Store_Filesystem *)store;
   if (!EINA_MAGIC_CHECK(store, ELM_STORE_MAGIC)) return NULL;
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_FILESYSTEM_MAGIC)) return NULL;
   return st->dir;
}

ELM_API void
elm_store_cache_set(Elm_Store *st, int max)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return;
   if (max < 0) max = 0;
   st->cache_max = max;
   _store_cache_trim(st);
}

ELM_API int
elm_store_cache_get(const Elm_Store *st)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return 0;
   return st->cache_max;
}

ELM_API void
elm_store_list_func_set(Elm_Store *st, Elm_Store_Item_List_Cb func, const void *data)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return;
   st->cb.list.func = func;
   st->cb.list.data = (void *)data;
}

ELM_API void
elm_store_fetch_func_set(Elm_Store *st, Elm_Store_Item_Fetch_Cb func, const void *data)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return;
   st->cb.fetch.func = func;
   st->cb.fetch.data = (void *)data;
}

ELM_API void
elm_store_fetch_thread_set(Elm_Store *st, Eina_Bool use_thread)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return;
   st->fetch_thread = !!use_thread;
}

ELM_API Eina_Bool
elm_store_fetch_thread_get(const Elm_Store *st)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return EINA_FALSE;
   return st->fetch_thread;
}

ELM_API void
elm_store_unfetch_func_set(Elm_Store *st, Elm_Store_Item_Unfetch_Cb func, const void *data)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return;
   st->cb.unfetch.func = func;
   st->cb.unfetch.data = (void *)data;
}

ELM_API void
elm_store_sorted_set(Elm_Store *st, Eina_Bool sorted)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return;
   st->sorted = sorted;
}

ELM_API Eina_Bool
elm_store_sorted_get(const Elm_Store *st)
{
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_MAGIC)) return EINA_FALSE;
   return st->sorted;
}

ELM_API void
elm_store_item_data_set(Elm_Store_Item *sti, void *data)
{
   if (!EINA_MAGIC_CHECK(sti, ELM_STORE_ITEM_MAGIC)) return;
//// let fetch/unfetch do the locking
//   eina_lock_take(&sti->lock);
   sti->data = data;
//   eina_lock_release(&sti->lock);
}

ELM_API void *
elm_store_item_data_get(Elm_Store_Item *sti)
{
   if (!EINA_MAGIC_CHECK(sti, ELM_STORE_ITEM_MAGIC)) return NULL;
   void *d;
//// let fetch/unfetch do the locking
//   eina_lock_take(&sti->lock);
   d = sti->data;
//   eina_lock_release(&sti->lock);
   return d;
}

ELM_API const Elm_Store *
elm_store_item_store_get(const Elm_Store_Item *sti)
{
   if (!EINA_MAGIC_CHECK(sti, ELM_STORE_ITEM_MAGIC)) return NULL;
   // dont need lock
   return sti->store;
}

ELM_API const Elm_Object_Item *
elm_store_item_genlist_item_get(const Elm_Store_Item *sti)
{
   if (!EINA_MAGIC_CHECK(sti, ELM_STORE_ITEM_MAGIC)) return NULL;
   // dont need lock
   return sti->item;
}

ELM_API const char *
elm_store_item_filesystem_path_get(const Elm_Store_Item *item)
{
   Elm_Store_Item_Filesystem *sti = (Elm_Store_Item_Filesystem *)item;
   Elm_Store_Filesystem *st;
   if (!EINA_MAGIC_CHECK(item, ELM_STORE_ITEM_MAGIC)) return NULL;
   if (!EINA_MAGIC_CHECK(item->store, ELM_STORE_MAGIC)) return NULL;
   /* ensure we're dealing with filesystem item */
   st = (Elm_Store_Filesystem *)item->store;
   if (!EINA_MAGIC_CHECK(st, ELM_STORE_FILESYSTEM_MAGIC)) return NULL;
   // dont need lock
   return sti->path;
}
