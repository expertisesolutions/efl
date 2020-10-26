#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <string.h>

#include <Ecore.h>
#include <Ecore_Getopt.h>
#include "ecore_private.h"

#include "Ecore_Evas.h"
#include "ecore_evas_private.h"
#define EFL_INTERNAL_UNSTABLE
#include "Evas_Internal.h"

static const char ASSOCIATE_KEY[] = "__Ecore_Evas_Associate";

static void _ecore_evas_object_associate(Ecore_Evas *ee, Evas_Object *obj, Ecore_Evas_Object_Associate_Flags flags);
static void _ecore_evas_object_dissociate(Ecore_Evas *ee, Evas_Object *obj);


static Evas_Object *
_ecore_evas_associate_get(const Ecore_Evas *ee)
{
   return ecore_evas_data_get(ee, ASSOCIATE_KEY);
}

static void
_ecore_evas_associate_set(Ecore_Evas *ee, Evas_Object *obj)
{
   ecore_evas_data_set(ee, ASSOCIATE_KEY, obj);
}

static void
_ecore_evas_associate_del(Ecore_Evas *ee)
{
   ecore_evas_data_set(ee, ASSOCIATE_KEY, NULL);
}

static Ecore_Evas *
_evas_object_associate_get(const Evas_Object *obj)
{
   return evas_object_data_get(obj, ASSOCIATE_KEY);
}

static void
_evas_object_associate_set(Evas_Object *obj, Ecore_Evas *ee)
{
   evas_object_data_set(obj, ASSOCIATE_KEY, ee);
}

static void
_evas_object_associate_del(Evas_Object *obj)
{
   evas_object_data_del(obj, ASSOCIATE_KEY);
}

/** Associated Events: ******************************************************/

/* Interceptors Callbacks */

static void
_ecore_evas_object_intercept_move(void *data, Evas_Object *obj, Evas_Coord x, Evas_Coord y)
{
   Ecore_Evas *ee = data;
   // FIXME: account for frame
   ecore_evas_move(ee, x, y);
   if (ecore_evas_override_get(ee)) evas_object_move(obj, x, y);
}

static void
_ecore_evas_object_intercept_raise(void *data, Evas_Object *obj EINA_UNUSED)
{
   Ecore_Evas *ee = data;
   ecore_evas_raise(ee);
}

static void
_ecore_evas_object_intercept_lower(void *data, Evas_Object *obj EINA_UNUSED)
{
   Ecore_Evas *ee = data;
   ecore_evas_lower(ee);
}

static void
_ecore_evas_object_intercept_stack_above(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, Evas_Object *above EINA_UNUSED)
{
   INF("TODO: %s", __func__);
}

static void
_ecore_evas_object_intercept_stack_below(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, Evas_Object *below EINA_UNUSED)
{
   INF("TODO: %s", __func__);
}

static void
_ecore_evas_object_intercept_layer_set(void *data, Evas_Object *obj EINA_UNUSED, int l)
{
   Ecore_Evas *ee = data;
   ecore_evas_layer_set(ee, l);
}

/* Event Callbacks */

static void
_ecore_evas_object_callback_show(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee = data;
   ecore_evas_show(ee);
}

static void
_ecore_evas_object_callback_hide(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee = data;
   ecore_evas_hide(ee);
}

static void
_ecore_evas_object_callback_resize(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee = data;
   Evas_Coord ow, oh;

   evas_object_geometry_get(obj, NULL, NULL, &ow, &oh);
   ecore_evas_resize(ee, ow, oh);
}

static void
_ecore_evas_object_callback_changed_size_hints(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee = data;
   Evas_Coord w, h;

   evas_object_size_hint_combined_min_get(obj, &w, &h);
   ecore_evas_size_min_set(ee, w, h);

   evas_object_size_hint_combined_max_get(obj, &w, &h);
   if (w < 1) w = -1;
   if (h < 1) h = -1;
   ecore_evas_size_max_set(ee, w, h);
}

static void
_ecore_evas_object_callback_del(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee = data;
   _ecore_evas_object_dissociate(ee, obj);
   ecore_evas_free(ee);
}

static void
_ecore_evas_object_callback_del_dissociate(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee = data;
   _ecore_evas_object_dissociate(ee, obj);
}

static void
_ecore_evas_delete_request(Ecore_Evas *ee)
{
   Evas_Object *obj = _ecore_evas_associate_get(ee);
   _ecore_evas_object_dissociate(ee, obj);
   evas_object_del(obj);
   ecore_evas_free(ee);
}

static void
_ecore_evas_destroy(Ecore_Evas *ee)
{
   Evas_Object *obj = _ecore_evas_associate_get(ee);
   if (!obj)
     return;
   _ecore_evas_object_dissociate(ee, obj);
   evas_object_del(obj);
}

static void
_ecore_evas_resize(Ecore_Evas *ee)
{
   Evas_Object *obj = _ecore_evas_associate_get(ee);
   Evas_Coord w, h;
   ecore_evas_geometry_get(ee, NULL, NULL, &w, &h);
   evas_object_resize(obj, w, h);
}

static void
_ecore_evas_pre_free(Ecore_Evas *ee)
{
   Evas_Object *obj = _ecore_evas_associate_get(ee);
   if (!obj)
     return;
   _ecore_evas_object_dissociate(ee, obj);
   evas_object_del(obj);
}

static int
_ecore_evas_object_evas_check(const char *function EINA_UNUSED, const Ecore_Evas *ee, const Evas_Object *obj)
{
   const char *name, *type;
   Evas *e;

   e = evas_object_evas_get(obj);
   if (e == ee->evas)
     return 1;

   name = evas_object_name_get(obj);
   type = evas_object_type_get(obj);

   ERR("ERROR: %s(): object %p (name=\"%s\", type=\"%s\") evas "
       "is not the same as this Ecore_Evas evas: %p != %p",
       function, obj,
       name ? name : "", type ? type : "", e, ee->evas);
   fflush(stderr);
   if (getenv("ECORE_ERROR_ABORT")) abort();

   return 0;
}

ECORE_EVAS_API Eina_Bool
ecore_evas_object_associate(Ecore_Evas *ee, Evas_Object *obj, Ecore_Evas_Object_Associate_Flags flags)
{
   Ecore_Evas *old_ee;
   Evas_Object *old_obj;

   if (!ECORE_MAGIC_CHECK(ee, ECORE_MAGIC_EVAS))
   {
      ECORE_MAGIC_FAIL(ee, ECORE_MAGIC_EVAS, __func__);
      return EINA_FALSE;
   }

   CHECK_PARAM_POINTER_RETURN("obj", obj, EINA_FALSE);
   if (!_ecore_evas_object_evas_check(__func__, ee, obj))
     return EINA_FALSE;

   old_ee = _evas_object_associate_get(obj);
   if (old_ee)
     ecore_evas_object_dissociate(old_ee, obj);

   old_obj = _ecore_evas_associate_get(ee);
   if (old_obj)
     ecore_evas_object_dissociate(ee, old_obj);

   _ecore_evas_object_associate(ee, obj, flags);
   return EINA_TRUE;
}

ECORE_EVAS_API Eina_Bool
ecore_evas_object_dissociate(Ecore_Evas *ee, Evas_Object *obj)
{
   Ecore_Evas *old_ee;
   Evas_Object *old_obj;

   if (!ECORE_MAGIC_CHECK(ee, ECORE_MAGIC_EVAS))
   {
      ECORE_MAGIC_FAIL(ee, ECORE_MAGIC_EVAS, __func__);
      return EINA_FALSE;
   }

   CHECK_PARAM_POINTER_RETURN("obj", obj, EINA_FALSE);
   old_ee = _evas_object_associate_get(obj);
   if (ee != old_ee) {
      ERR("ERROR: trying to dissociate object that is not using "
          "this Ecore_Evas: %p != %p", ee, old_ee);
      return EINA_FALSE;
   }

   old_obj = _ecore_evas_associate_get(ee);
   if (old_obj != obj) {
      ERR("ERROR: trying to dissociate object that is not being "
          "used by this Ecore_Evas: %p != %p", old_obj, obj);
      return EINA_FALSE;
   }

   _ecore_evas_object_dissociate(ee, obj);

   return EINA_TRUE;
}

ECORE_EVAS_API Evas_Object *
ecore_evas_object_associate_get(const Ecore_Evas *ee)
{
   if (!ECORE_MAGIC_CHECK(ee, ECORE_MAGIC_EVAS))
   {
      ECORE_MAGIC_FAIL(ee, ECORE_MAGIC_EVAS, __func__);
      return NULL;
   }
   return _ecore_evas_associate_get(ee);
}

static void
_ecore_evas_object_associate(Ecore_Evas *ee, Evas_Object *obj, Ecore_Evas_Object_Associate_Flags flags)
{
   evas_object_event_callback_add
     (obj, EVAS_CALLBACK_SHOW,
      _ecore_evas_object_callback_show, ee);
   evas_object_event_callback_add
     (obj, EVAS_CALLBACK_HIDE,
      _ecore_evas_object_callback_hide, ee);
   evas_object_event_callback_add
     (obj, EVAS_CALLBACK_RESIZE,
      _ecore_evas_object_callback_resize, ee);
   evas_object_event_callback_add
     (obj, EVAS_CALLBACK_CHANGED_SIZE_HINTS,
      _ecore_evas_object_callback_changed_size_hints, ee);
   if (flags & ECORE_EVAS_OBJECT_ASSOCIATE_DEL)
     evas_object_event_callback_add
       (obj, EVAS_CALLBACK_DEL, _ecore_evas_object_callback_del, ee);
   else
     evas_object_event_callback_add
       (obj, EVAS_CALLBACK_DEL, _ecore_evas_object_callback_del_dissociate, ee);

   evas_object_intercept_move_callback_add
     (obj, _ecore_evas_object_intercept_move, ee);

   if (flags & ECORE_EVAS_OBJECT_ASSOCIATE_STACK)
     {
        evas_object_intercept_raise_callback_add
          (obj, _ecore_evas_object_intercept_raise, ee);
        evas_object_intercept_lower_callback_add
          (obj, _ecore_evas_object_intercept_lower, ee);
        evas_object_intercept_stack_above_callback_add
          (obj, _ecore_evas_object_intercept_stack_above, ee);
        evas_object_intercept_stack_below_callback_add
          (obj, _ecore_evas_object_intercept_stack_below, ee);
     }

   if (flags & ECORE_EVAS_OBJECT_ASSOCIATE_LAYER)
     evas_object_intercept_layer_set_callback_add
       (obj, _ecore_evas_object_intercept_layer_set, ee);

   if (flags & ECORE_EVAS_OBJECT_ASSOCIATE_DEL)
     {
        ecore_evas_callback_delete_request_set(ee, _ecore_evas_delete_request);
        ecore_evas_callback_destroy_set(ee, _ecore_evas_destroy);
     }
   ecore_evas_callback_pre_free_set(ee, _ecore_evas_pre_free);
   ecore_evas_callback_resize_set(ee, _ecore_evas_resize);

   _evas_object_associate_set(obj, ee);
   _ecore_evas_associate_set(ee, obj);
}

static void
_ecore_evas_object_dissociate(Ecore_Evas *ee, Evas_Object *obj)
{
   evas_object_event_callback_del_full
     (obj, EVAS_CALLBACK_SHOW,
      _ecore_evas_object_callback_show, ee);
   evas_object_event_callback_del_full
     (obj, EVAS_CALLBACK_HIDE,
      _ecore_evas_object_callback_hide, ee);
   evas_object_event_callback_del_full
     (obj, EVAS_CALLBACK_RESIZE,
      _ecore_evas_object_callback_resize, ee);
   evas_object_event_callback_del_full
     (obj, EVAS_CALLBACK_CHANGED_SIZE_HINTS,
      _ecore_evas_object_callback_changed_size_hints, ee);
   evas_object_event_callback_del_full
     (obj, EVAS_CALLBACK_DEL, _ecore_evas_object_callback_del, ee);
   evas_object_event_callback_del_full
     (obj, EVAS_CALLBACK_DEL, _ecore_evas_object_callback_del_dissociate, ee);

   evas_object_intercept_move_callback_del
     (obj, _ecore_evas_object_intercept_move);

   evas_object_intercept_raise_callback_del
     (obj, _ecore_evas_object_intercept_raise);
   evas_object_intercept_lower_callback_del
     (obj, _ecore_evas_object_intercept_lower);
   evas_object_intercept_stack_above_callback_del
     (obj, _ecore_evas_object_intercept_stack_above);
   evas_object_intercept_stack_below_callback_del
     (obj, _ecore_evas_object_intercept_stack_below);

   evas_object_intercept_layer_set_callback_del
     (obj, _ecore_evas_object_intercept_layer_set);

   if (!ECORE_MAGIC_CHECK(ee, ECORE_MAGIC_EVAS))
   {
      ECORE_MAGIC_FAIL(ee, ECORE_MAGIC_EVAS, __func__);
   }
   else
   {
      if (ee->func.fn_delete_request == _ecore_evas_delete_request)
        ecore_evas_callback_delete_request_set(ee, NULL);
      if (ee->func.fn_destroy == _ecore_evas_destroy)
        ecore_evas_callback_destroy_set(ee, NULL);
      if (ee->func.fn_resize == _ecore_evas_resize)
        ecore_evas_callback_resize_set(ee, NULL);
      if (ee->func.fn_pre_free == _ecore_evas_pre_free)
        ecore_evas_callback_pre_free_set(ee, NULL);

      _ecore_evas_associate_del(ee);
   }

   _evas_object_associate_del(obj);
}

/**
 * Helper ecore_getopt callback to list available Ecore_Evas engines.
 *
 * This will list all available engines except buffer, this is useful
 * for applications to let user choose how they should create windows
 * with ecore_evas_new().
 *
 * @c callback_data value is used as @c FILE* and says where to output
 * messages, by default it is @c stdout. You can specify this value
 * with ECORE_GETOPT_CALLBACK_FULL() or ECORE_GETOPT_CALLBACK_ARGS().
 *
 * If there is a boolean storage provided, then it is marked with 1
 * when this option is executed.
 * @param parser This parameter isn't in use.
 * @param desc This parameter isn't in use.
 * @param str This parameter isn't in use.
 * @param data The data to be used.
 * @param storage The storage to be used.
 * @return The function returns 1, when storage is NULL it returns 0.
 */
unsigned char
ecore_getopt_callback_ecore_evas_list_engines(const Ecore_Getopt *parser EINA_UNUSED, const Ecore_Getopt_Desc *desc EINA_UNUSED, const char *str EINA_UNUSED, void *data, Ecore_Getopt_Value *storage)
{
   Eina_List  *lst, *n;
   const char *engine;

   if (!storage)
     {
        ERR("Storage is missing");
        return 0;
     }

   FILE *fp = data;
   if (!fp)
     fp = stdout;

   lst = ecore_evas_engines_get();

   fputs("supported engines:\n", fp);
   EINA_LIST_FOREACH(lst, n, engine)
     if (strcmp(engine, "buffer") != 0)
       fprintf(fp, "\t%s\n", engine);

   ecore_evas_engines_free(lst);

   if (storage->boolp)
     *storage->boolp = 1;

   return 1;
}
