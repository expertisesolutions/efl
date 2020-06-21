#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>

#include "elm_priv.h"

#define ELM_TRANSIT_CHECK_OR_RETURN(transit, ...) \
   do { \
      if (!transit) { \
         CRI("Elm_Transit " # transit " is NULL!"); \
         return __VA_ARGS__; \
      } \
      if (!EINA_MAGIC_CHECK(transit, ELM_TRANSIT_MAGIC)) { \
         EINA_MAGIC_FAIL(transit, ELM_TRANSIT_MAGIC); \
         return __VA_ARGS__; \
      } \
      if (transit->deleted){ \
         ERR("Elm_Transit " # transit " has already been deleted!"); \
         return __VA_ARGS__; \
      } \
   } while (0)

#define _TRANSIT_FOCAL 2000

struct _Elm_Transit
{
#define ELM_TRANSIT_MAGIC 0xd27f190a
   EINA_MAGIC;

   Ecore_Animator *animator;
   Ecore_Timer *go_in_timer; /**< Timer used by elm_transit_go_in() */
   Eina_Inlist *effect_list;
   Eina_List *objs;
   Elm_Transit *prev_chain_transit;
   Eina_List *next_chain_transits;
   Elm_Transit_Tween_Mode tween_mode;
   struct
     {
        Elm_Transit_Del_Cb func;
        void *arg;
     } del_data;
   struct
     {
        double delayed;
        double paused;
        double duration;
        double begin;
        double current;
        double revert_start;
        double revert_elapsed;
        double revert_paused;
        double revert_delayed;
     } time;
   struct
     {
        int count;
        int current;
        Eina_Bool reverse;
     } repeat;
   double progress;
   double inter_progress;
   double base_progress;
   double revert_begin_progress;
   double revert_duration;
   double total_revert_time;
   unsigned int effects_pending_del;
   int walking;
   double v[4];
   Eina_Bool auto_reverse : 1;
   Eina_Bool event_enabled : 1;
   Eina_Bool deleted : 1;
   Eina_Bool state_keep : 1;
   Eina_Bool finished : 1;
   Eina_Bool smooth : 1;
   Eina_Bool revert_mode : 1;
};

struct _Elm_Transit_Effect_Module
{
   EINA_INLIST;
   Elm_Transit_Effect_Transition_Cb transition_cb;
   Elm_Transit_Effect_End_Cb end_cb;
   Elm_Transit_Effect *effect;
   Eina_Bool deleted : 1;
};

struct _Elm_Transit_Obj_Data
{
   struct {
      Evas_Coord x, y, w, h;
      int r,g,b,a;
      Evas_Map *map;
      Eina_Bool map_enabled : 1;
      Eina_Bool visible : 1;
      Eina_Bool freeze_events : 1;
   } state;
   int ref;
};

typedef struct _Elm_Transit_Effect_Module Elm_Transit_Effect_Module;
typedef struct _Elm_Transit_Obj_Data Elm_Transit_Obj_Data;

static void _transit_obj_data_save(Evas_Object *obj);
static void _transit_obj_data_recover(Elm_Transit *transit, Evas_Object *obj);
static void _transit_obj_remove_cb(void *data, const Efl_Event *ev);
static void _transit_obj_remove(Elm_Transit *transit, Evas_Object *obj);
static void _transit_effect_del(Elm_Transit *transit, Elm_Transit_Effect_Module *effect_module);
static void _transit_remove_dead_effects(Elm_Transit *transit);
static void _transit_chain_go(Elm_Transit *transit);
static void _transit_del(Elm_Transit *transit);
static Eina_Bool _transit_animate_op(Elm_Transit *transit, double progress);
static Eina_Bool _transit_animate_cb(void *data);

static char *_transit_key= "_elm_transit_key";

static void
_transit_obj_data_save(Evas_Object *obj)
{
   Elm_Transit_Obj_Data *obj_data = evas_object_data_get(obj, _transit_key);

   if (obj_data)
     {
        obj_data->ref++;
        return;
     }

   obj_data = ELM_NEW(Elm_Transit_Obj_Data);
   if (!obj_data)
     {
        ERR("Failed to allocate memory");
        return;
     }

   evas_object_geometry_get(obj, &obj_data->state.x, &obj_data->state.y,
                            &obj_data->state.w, &obj_data->state.h);
   evas_object_color_get(obj, &obj_data->state.r, &obj_data->state.g,
                         &obj_data->state.b, &obj_data->state.a);
   obj_data->state.visible = evas_object_visible_get(obj);
   obj_data->state.freeze_events = evas_object_freeze_events_get(obj);
   obj_data->state.map_enabled = evas_object_map_enable_get(obj);

   ELM_SAFE_FREE(obj_data->state.map, evas_map_free);

   if (evas_object_map_get(obj))
     obj_data->state.map = evas_map_dup(evas_object_map_get(obj));

   obj_data->ref++;

   evas_object_data_set(obj, _transit_key, obj_data);
}

static void
_remove_obj_from_list(Elm_Transit *transit, Evas_Object *obj)
{
   //Remove duplicated objects
   //TODO: Need to consider about optimizing here
   while (1)
     {
        if (!eina_list_data_find_list(transit->objs, obj))
          break;
        transit->objs = eina_list_remove(transit->objs, obj);
        efl_event_callback_del(obj, EFL_EVENT_DEL,
                               _transit_obj_remove_cb,
                               transit);
     }
}

static void
_transit_obj_remove_cb(void *data, const Efl_Event *ev)
{
   Eo* obj = ev->object;
   Elm_Transit *transit = data;
   Elm_Transit_Obj_Data *obj_data = evas_object_data_get(obj, _transit_key);
   if (obj_data)
     {
        obj_data->ref--;
        if (obj_data->ref == 0)
          {
             free(obj_data);
             evas_object_data_del(obj, _transit_key);
          }
     }
   _remove_obj_from_list(transit, obj);
   if (!transit->objs && !transit->deleted) elm_transit_del(transit);
}

static void
_transit_obj_data_recover(Elm_Transit *transit, Evas_Object *obj)
{
   Elm_Transit_Obj_Data *obj_data;

   obj_data = evas_object_data_get(obj, _transit_key);
   if (!obj_data) return;

   obj_data->ref--;

   //recover the states of the object.
   if (!transit->state_keep)
     {
        evas_object_geometry_set(obj, obj_data->state.x, obj_data->state.y,
                                 obj_data->state.w, obj_data->state.h);
        evas_object_color_set(obj, obj_data->state.r, obj_data->state.g,
                              obj_data->state.b, obj_data->state.a);
        if (obj_data->state.visible) evas_object_show(obj);
        else evas_object_hide(obj);
        evas_object_map_enable_set(obj, obj_data->state.map_enabled);
        evas_object_map_set(obj, obj_data->state.map);
     }

   if (obj_data->ref == 0)
     {
        ELM_SAFE_FREE(obj_data->state.map, evas_map_free);
        evas_object_data_del(obj, _transit_key);
        evas_object_freeze_events_set(obj, obj_data->state.freeze_events);
        free(obj_data);
     }
}

static void
_transit_obj_remove(Elm_Transit *transit, Evas_Object *obj)
{
   _remove_obj_from_list(transit, obj);
   _transit_obj_data_recover(transit, obj);
}

static void
_transit_effect_del(Elm_Transit *transit, Elm_Transit_Effect_Module *effect_module)
{
   if (effect_module->end_cb)
     effect_module->end_cb(effect_module->effect, transit);
   free(effect_module);
}

static void
_transit_remove_dead_effects(Elm_Transit *transit)
{
   Elm_Transit_Effect_Module *effect_module = NULL;
   Eina_Inlist *ll;

   EINA_INLIST_FOREACH_SAFE(transit->effect_list, ll, effect_module)
     {
        if (effect_module->deleted)
          {
             _transit_effect_del(transit, effect_module);
             transit->effects_pending_del--;
             if (!transit->effects_pending_del) return;
          }
     }
}

static void
_transit_chain_go(Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   elm_transit_go(transit);
}

static void
_transit_del(Elm_Transit *transit)
{
   Elm_Transit_Effect_Module *effect_module;
   Elm_Transit *chain_transit;
   Eina_List *elist, *elist_next;

   transit->deleted = EINA_TRUE;

   ELM_SAFE_FREE(transit->animator, ecore_animator_del);

   //remove effects
   while (transit->effect_list)
     {
        effect_module = EINA_INLIST_CONTAINER_GET(transit->effect_list, Elm_Transit_Effect_Module);
        transit->effect_list = eina_inlist_remove(transit->effect_list, transit->effect_list);
        _transit_effect_del(transit, effect_module);
     }

   //remove objects.
   while (transit->objs)
     _transit_obj_remove(transit, eina_list_data_get(transit->objs));

   if (transit->del_data.func)
     transit->del_data.func(transit->del_data.arg, transit);

   ecore_timer_del(transit->go_in_timer);

   //cut off the chain transit relationship
   EINA_LIST_FOREACH_SAFE(transit->next_chain_transits, elist, elist_next, chain_transit)
     chain_transit->prev_chain_transit = NULL;

   if (transit->prev_chain_transit)
     transit->prev_chain_transit->next_chain_transits =
        eina_list_remove(transit->prev_chain_transit->next_chain_transits, transit);

   // run chain transits
   if (transit->finished && transit->next_chain_transits)
     {
        EINA_LIST_FOREACH_SAFE(transit->next_chain_transits, elist, elist_next, chain_transit)
          _transit_chain_go(chain_transit);
     }

   eina_list_free(transit->next_chain_transits);

   EINA_MAGIC_SET(transit, EINA_MAGIC_NONE);
   free(transit);
}

//If the transit is deleted then EINA_FALSE is returned.
static Eina_Bool
_transit_animate_op(Elm_Transit *transit, double progress)
{
   Elm_Transit_Effect_Module *effect_module;

   transit->walking++;
   EINA_INLIST_FOREACH(transit->effect_list, effect_module)
     {
        if (transit->deleted) break;
        if (!effect_module->deleted)
          effect_module->transition_cb(effect_module->effect, transit, progress);
     }
   transit->walking--;

   if (transit->walking) return EINA_TRUE;

   if (transit->deleted)
     {
        _transit_del(transit);
        return EINA_FALSE;
     }

   else if (transit->effects_pending_del) _transit_remove_dead_effects(transit);

   return EINA_TRUE;
}

static Eina_Bool
_transit_animate_cb(void *data)
{
   Elm_Transit *transit = data;
   double elapsed_time, duration, revert_progress;

   transit->time.current = ecore_loop_time_get();
   elapsed_time = (transit->time.current - transit->time.begin - transit->time.delayed)
      - (2 * transit->total_revert_time);
   duration = transit->time.duration;
   transit->progress = elapsed_time / duration;

   if (transit->revert_mode && EINA_DBL_EQ(transit->revert_begin_progress, 0))
     {
        transit->revert_begin_progress = transit->progress;
        transit->time.revert_start = transit->time.current;
     }

   if (transit->revert_mode)
     {
        transit->time.revert_elapsed =
           (transit->time.current - transit->time.revert_start - transit->time.revert_delayed);
        revert_progress = transit->time.revert_elapsed / duration;
        transit->progress = transit->revert_begin_progress - revert_progress;
     }

   /* Intervention Progress */
   transit->base_progress = transit->progress;
   transit->progress += transit->inter_progress;

   switch (transit->tween_mode)
     {
      case ELM_TRANSIT_TWEEN_MODE_LINEAR:
         transit->progress = ecore_animator_pos_map(transit->progress,
                                                    ECORE_POS_MAP_LINEAR,
                                                    0, 0);
         break;
      case ELM_TRANSIT_TWEEN_MODE_ACCELERATE:
         transit->progress =
            ecore_animator_pos_map(transit->progress,
                                   ECORE_POS_MAP_ACCELERATE_FACTOR,
                                   transit->v[0], 0);
         break;
      case ELM_TRANSIT_TWEEN_MODE_DECELERATE:
         transit->progress =
            ecore_animator_pos_map(transit->progress,
                                   ECORE_POS_MAP_DECELERATE_FACTOR,
                                   transit->v[0], 0);
         break;
      case ELM_TRANSIT_TWEEN_MODE_SINUSOIDAL:
         transit->progress =
            ecore_animator_pos_map(transit->progress,
                                   ECORE_POS_MAP_SINUSOIDAL_FACTOR,
                                   transit->v[0], 0);
         break;
      case ELM_TRANSIT_TWEEN_MODE_DIVISOR_INTERP:
         transit->progress = ecore_animator_pos_map(transit->progress,
                                                    ECORE_POS_MAP_DIVISOR_INTERP,
                                                    transit->v[0], transit->v[1]);
         break;
      case ELM_TRANSIT_TWEEN_MODE_BOUNCE:
         transit->progress = ecore_animator_pos_map(transit->progress,
                                                    ECORE_POS_MAP_BOUNCE,
                                                    transit->v[0], transit->v[1]);
         break;
      case ELM_TRANSIT_TWEEN_MODE_SPRING:
         transit->progress = ecore_animator_pos_map(transit->progress,
                                                    ECORE_POS_MAP_SPRING,
                                                    transit->v[0], transit->v[1]);
         break;
      case ELM_TRANSIT_TWEEN_MODE_BEZIER_CURVE:
         transit->progress = ecore_animator_pos_map_n(transit->progress,
                                                      ECORE_POS_MAP_CUBIC_BEZIER,
                                                      4, transit->v);
         break;
      default:
         break;
     }

   /* Reverse? */
   if (transit->repeat.reverse) transit->progress = 1 - transit->progress;

   if (!_transit_animate_op(transit, transit->progress))
     return ECORE_CALLBACK_CANCEL;

   if (transit->revert_mode && (transit->progress <= 0 || transit->progress >= 1))
     {
        transit->revert_mode = EINA_FALSE;
        transit->time.begin = ecore_loop_time_get();
        transit->time.revert_delayed = 0;
        transit->total_revert_time = 0;
        transit->inter_progress = 0;
        if ((transit->repeat.count >= 0) &&
            (transit->repeat.current == transit->repeat.count) &&
            ((!transit->auto_reverse) || transit->repeat.reverse))
          {
             transit->finished = EINA_TRUE;
             elm_transit_del(transit);
             return ECORE_CALLBACK_CANCEL;
          }
        else
          return ECORE_CALLBACK_RENEW;
     }

   /* Not end. Keep going. */
   if ((!transit->repeat.reverse && transit->progress < 1) ||
        (transit->repeat.reverse && transit->progress > 0) ||
        transit->revert_mode) return ECORE_CALLBACK_RENEW;

   /* Repeat and reverse and time done! */
   if ((transit->repeat.count >= 0) &&
       (transit->repeat.current == transit->repeat.count) &&
       ((!transit->auto_reverse) || transit->repeat.reverse))
     {
        transit->finished = EINA_TRUE;
        elm_transit_del(transit);
        return ECORE_CALLBACK_CANCEL;
     }

   /* Repeat Case */
   if (!transit->auto_reverse || transit->repeat.reverse)
     {
        transit->repeat.current++;
        transit->repeat.reverse = EINA_FALSE;
     }
   else transit->repeat.reverse = EINA_TRUE;

   transit->time.begin = ecore_loop_time_get();
   transit->time.delayed = 0;
   transit->total_revert_time = 0;
   transit->inter_progress = 0;

   return ECORE_CALLBACK_RENEW;
}

static void
_recover_image_uv_by_y(Evas_Map *map, int iw, int ih)
{
   evas_map_point_image_uv_set(map, 0, iw, 0);
   evas_map_point_image_uv_set(map, 1, 0, 0);
   evas_map_point_image_uv_set(map, 2, 0, ih);
   evas_map_point_image_uv_set(map, 3, iw, ih);
}

static void
_recover_image_uv_by_x(Evas_Map *map, int iw, int ih)
{
   evas_map_point_image_uv_set(map, 0, 0, ih);
   evas_map_point_image_uv_set(map, 1, iw, ih);
   evas_map_point_image_uv_set(map, 2, iw, 0);
   evas_map_point_image_uv_set(map, 3, 0, 0);
}

static Eina_Bool
_recover_image_uv(Evas_Object *obj, Evas_Map *map, Eina_Bool revert, Eina_Bool by_x)
{
   //Since the map is not proper for all types of objects,
   //Need to handle uvs only for image objects
   int iw, ih;
   int x, y, w, h;

   const char *type = evas_object_type_get(obj);
   if ((!type) || (strcmp(type, "image"))) return EINA_FALSE;
   if (evas_object_image_source_get(obj)) return EINA_FALSE;

   evas_object_image_size_get(obj, &iw, &ih);

   if (revert)
     {
        if (by_x)
          _recover_image_uv_by_x(map, iw, ih);
        else
          _recover_image_uv_by_y(map, iw, ih);
     }
   else
     {
        if (evas_object_image_filled_get(obj))
          {
             x = 0;
             y = 0;
             w = iw;
             h = ih;
          }
        //Zooming image fill area.
        else
          {
             Eina_Rect fill;
             Eina_Size2D sz;

             fill = efl_gfx_fill_get(obj);
             sz = efl_gfx_entity_size_get(obj);
             w = sz.w;
             h = sz.h;

             EINA_SAFETY_ON_FALSE_RETURN_VAL(eina_rectangle_is_valid(&fill.rect), EINA_FALSE);
             double rate_x = (double) w / (double) fill.w;
             double rate_y = (double) h / (double) fill.h;
             double rate_x2 = (double) iw / (double) fill.w;
             double rate_y2 = (double) ih / (double) fill.h;

             x = -(int)((double) fill.x * rate_x2);
             y = -(int)((double) fill.y * rate_y2);
             w = (int)(((double) iw) * rate_x) + x;
             h = (int)(((double) ih) * rate_y) + y;
          }

        evas_map_point_image_uv_set(map, 0, x, y);
        evas_map_point_image_uv_set(map, 1, w, y);
        evas_map_point_image_uv_set(map, 2, w, h);
        evas_map_point_image_uv_set(map, 3, x, h);
     }
   return EINA_TRUE;
}

static Eina_Bool
_go_in_timer_cb(void *data)
{
   Elm_Transit *transit = data;

   transit->go_in_timer = NULL;
   elm_transit_go(transit);

   return ECORE_CALLBACK_CANCEL;
}

ELM_API Elm_Transit *
elm_transit_add(void)
{
   Elm_Transit *transit = ELM_NEW(Elm_Transit);
   if (!transit)
     {
        ERR("Failed to allocate a elm_transit object!");
        return NULL;
     }

   EINA_MAGIC_SET(transit, ELM_TRANSIT_MAGIC);

   elm_transit_tween_mode_set(transit, ELM_TRANSIT_TWEEN_MODE_LINEAR);

   transit->v[0] = 1.0;
   transit->v[1] = 0.0;
   transit->smooth = EINA_TRUE;

   return transit;
}

ELM_API void
elm_transit_del(Elm_Transit *transit)
{
   if (!transit) return;

   ELM_TRANSIT_CHECK_OR_RETURN(transit);

   transit->deleted = EINA_TRUE;
   if (transit->walking) return;
   _transit_del(transit);
}

ELM_API void
elm_transit_effect_add(Elm_Transit *transit, Elm_Transit_Effect_Transition_Cb transition_cb, Elm_Transit_Effect *effect, Elm_Transit_Effect_End_Cb end_cb)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   EINA_SAFETY_ON_NULL_RETURN(transition_cb);
   Elm_Transit_Effect_Module *effect_module;

   EINA_INLIST_FOREACH(transit->effect_list, effect_module)
     if ((effect_module->transition_cb == transition_cb) && (effect_module->effect == effect))
       {
          WRN("elm_transit does not allow one to add the duplicated effect! : transit=%p", transit);
          return;
       }

   effect_module = ELM_NEW(Elm_Transit_Effect_Module);
   if (!effect_module)
     {
        ERR("Failed to allocate a new effect!: transit=%p", transit);
        return;
     }

   effect_module->end_cb = end_cb;
   effect_module->transition_cb = transition_cb;
   effect_module->effect = effect;

   transit->effect_list = eina_inlist_append(transit->effect_list, (Eina_Inlist*) effect_module);
}

ELM_API void
elm_transit_effect_del(Elm_Transit *transit, Elm_Transit_Effect_Transition_Cb transition_cb, Elm_Transit_Effect *effect)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   EINA_SAFETY_ON_NULL_RETURN(transition_cb);
   Elm_Transit_Effect_Module *effect_module;

   EINA_INLIST_FOREACH(transit->effect_list, effect_module)
     {
        if ((effect_module->transition_cb == transition_cb) && (effect_module->effect == effect))
          {
             if (transit->walking)
               {
                  effect_module->deleted = EINA_TRUE;
                  transit->effects_pending_del++;
               }
             else
               {
                  _transit_effect_del(transit, effect_module);
                  if (!transit->effect_list) elm_transit_del(transit);
               }
             return;
          }
     }
}

ELM_API void
elm_transit_object_add(Elm_Transit *transit, Evas_Object *obj)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   EINA_SAFETY_ON_NULL_RETURN(obj);

   // Prevent adding duplicates
   if (eina_list_data_find_list(transit->objs, obj))
     {
        WRN("Object(%p) is already added", obj);
        return;
     }

   if (transit->animator)
     {
        if (!evas_object_data_get(obj, _transit_key))
          {
             _transit_obj_data_save(obj);
             evas_object_freeze_events_set(obj, EINA_TRUE);
          }
     }

   efl_event_callback_add(obj, EFL_EVENT_DEL,
                          _transit_obj_remove_cb,
                          transit);

   transit->objs = eina_list_append(transit->objs, obj);
}

ELM_API void
elm_transit_object_remove(Elm_Transit *transit, Evas_Object *obj)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   EINA_SAFETY_ON_NULL_RETURN(obj);

   _transit_obj_remove(transit, obj);
   if (!transit->objs) elm_transit_del(transit);
}

ELM_API const Eina_List *
elm_transit_objects_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   return transit->objs;
}

ELM_API void
elm_transit_smooth_set(Elm_Transit *transit, Eina_Bool smooth)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   transit->smooth = !!smooth;
}

ELM_API Eina_Bool
elm_transit_smooth_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, EINA_FALSE);
   return transit->smooth;
}

ELM_API void
elm_transit_event_enabled_set(Elm_Transit *transit, Eina_Bool enabled)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);

   Eina_List *list;
   Evas_Object *obj;

   if (transit->event_enabled == enabled) return;
   transit->event_enabled = !!enabled;
   if (!transit->animator) return;

   EINA_LIST_FOREACH(transit->objs, list, obj)
     evas_object_freeze_events_set(obj, enabled);
}

ELM_API Eina_Bool
elm_transit_event_enabled_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, EINA_FALSE);
   return transit->event_enabled;
}

ELM_API void
elm_transit_del_cb_set(Elm_Transit *transit, void (*cb) (void *data, Elm_Transit *transit), void *data)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   transit->del_data.func = cb;
   transit->del_data.arg = data;
}

ELM_API void
elm_transit_auto_reverse_set(Elm_Transit *transit, Eina_Bool reverse)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   transit->auto_reverse = reverse;
}

ELM_API Eina_Bool
elm_transit_auto_reverse_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, EINA_FALSE);
   return transit->auto_reverse;
}

ELM_API void
elm_transit_repeat_times_set(Elm_Transit *transit, int repeat)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   transit->repeat.count = repeat;
   transit->repeat.current = 0;
}

ELM_API int
elm_transit_current_repeat_times_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, 0);
   return transit->repeat.current;
}

ELM_API int
elm_transit_repeat_times_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, 0);
   return transit->repeat.count;
}

ELM_API void
elm_transit_tween_mode_set(Elm_Transit *transit, Elm_Transit_Tween_Mode tween_mode)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   transit->tween_mode = tween_mode;
}

ELM_API Elm_Transit_Tween_Mode
elm_transit_tween_mode_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, ELM_TRANSIT_TWEEN_MODE_LINEAR);
   return transit->tween_mode;
}

ELM_API void
elm_transit_tween_mode_factor_set(Elm_Transit *transit, double v1, double v2)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   transit->v[0] = v1;
   transit->v[1] = v2;
}

ELM_API void
elm_transit_tween_mode_factor_get(const Elm_Transit *transit, double *v1, double *v2)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   if (v1) *v1 = transit->v[0];
   if (v2) *v2 = transit->v[1];
}

ELM_API void
elm_transit_tween_mode_factor_n_set(Elm_Transit *transit, unsigned int v_size, double *v)
{
   unsigned int i;
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   if (v_size > 4) v_size = 4;
   for (i = 0; i < v_size; i++)
     transit->v[i] = v[i];
}

ELM_API void
elm_transit_duration_set(Elm_Transit *transit, double duration)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   transit->time.duration = duration;
}

ELM_API double
elm_transit_duration_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, 0.0);
   return transit->time.duration;
}

ELM_API Eina_Bool
elm_transit_revert(Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, EINA_FALSE);

   if (!transit->animator) return EINA_FALSE;

   if (transit->revert_mode)
     {
        transit->total_revert_time += transit->time.revert_elapsed;
        transit->revert_mode = EINA_FALSE;
     }
   else
     {
        transit->revert_mode = EINA_TRUE;
        transit->time.revert_elapsed = 0;
        transit->revert_begin_progress = 0;
     }

   return EINA_TRUE;
}

ELM_API void
elm_transit_go(Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);

   ELM_SAFE_FREE(transit->go_in_timer, ecore_timer_del);

   Ecore_Evas *ee, *first_ee = NULL;
   Eina_List *elist;
   Evas_Object *obj, *first = NULL;
   Eina_Bool same = EINA_TRUE;

   ELM_SAFE_FREE(transit->animator, ecore_animator_del);

   if (transit->objs)
     {
        first = eina_list_data_get(transit->objs);
        first_ee = ecore_evas_ecore_evas_get(evas_object_evas_get(first));
     }
   EINA_LIST_FOREACH(transit->objs, elist, obj)
     {
        _transit_obj_data_save(obj);
        ee = ecore_evas_ecore_evas_get(evas_object_evas_get(obj));
        if (ee != first_ee)
          same = EINA_FALSE;
     }
   if (!transit->event_enabled)
     {
        EINA_LIST_FOREACH(transit->objs, elist, obj)
          evas_object_freeze_events_set(obj, EINA_TRUE);
     }

   transit->time.paused = 0;
   transit->time.delayed = 0;
   transit->time.revert_paused = 0;
   transit->time.revert_delayed = 0;
   transit->total_revert_time = 0;
   transit->revert_mode = EINA_FALSE;
   transit->time.begin = ecore_loop_time_get();

   if (!same || !first)
     transit->animator = ecore_animator_add(_transit_animate_cb, transit);
   else
     transit->animator = ecore_evas_animator_add(first,
                                                 _transit_animate_cb,
                                                 transit);
   _transit_animate_cb(transit);
}

ELM_API void
elm_transit_go_in(Elm_Transit *transit, double in)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);

   ecore_timer_del(transit->go_in_timer);
   transit->go_in_timer = ecore_timer_add(in, _go_in_timer_cb, transit);
}

ELM_API void
elm_transit_paused_set(Elm_Transit *transit, Eina_Bool paused)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);

   if (!transit->animator) return;

   if (paused)
     {
        if (transit->revert_mode)
          {
             if (transit->time.revert_paused > 0)return;
             transit->time.revert_paused = ecore_loop_time_get();
          }
        else
          {
             if (transit->time.paused > 0)return;
             transit->time.paused = ecore_loop_time_get();
          }
        ecore_animator_freeze(transit->animator);
     }
   else
     {
        if (transit->revert_mode)
          {
             if (EINA_DBL_EQ(transit->time.revert_paused, 0)) return;
             ecore_animator_thaw(transit->animator);
             transit->time.revert_delayed += (ecore_loop_time_get() - transit->time.revert_paused);
             transit->time.revert_paused = 0;
          }
        else
          {
             if (EINA_DBL_EQ(transit->time.paused, 0)) return;
             ecore_animator_thaw(transit->animator);
             transit->time.delayed += (ecore_loop_time_get() - transit->time.paused);
             transit->time.paused = 0;
          }
     }
}

ELM_API Eina_Bool
elm_transit_paused_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, EINA_FALSE);

   if (EINA_DBL_EQ(transit->time.paused, 0))
     return EINA_FALSE;

   return EINA_TRUE;
}

ELM_API void
elm_transit_progress_value_set(Elm_Transit *transit, double progress)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);

   if (progress < 0) progress = 0;
   else if (progress > 1) progress = 1;

   transit->inter_progress = (progress - transit->base_progress);
}

ELM_API double
elm_transit_progress_value_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, 0);

   return transit->progress;
}

ELM_API void
elm_transit_objects_final_state_keep_set(Elm_Transit *transit, Eina_Bool state_keep)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);

   if (transit->state_keep == state_keep) return;
   if (transit->animator)
     {
        WRN("elm_transit does not allow one to change final state keep mode in operating! : transit=%p", transit);
        return;
     }
   transit->state_keep = !!state_keep;
}

ELM_API Eina_Bool
elm_transit_objects_final_state_keep_get(const Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, EINA_FALSE);
   return transit->state_keep;
}

ELM_API void
elm_transit_chain_transit_add(Elm_Transit *transit, Elm_Transit *chain_transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   ELM_TRANSIT_CHECK_OR_RETURN(chain_transit);

   if (transit == chain_transit)
     {
        WRN("You add a same transit as a chain transit! : transit=%p, chain_transit=%p", transit, chain_transit);
        return;
     }
   if (transit == chain_transit->prev_chain_transit)
     return;

   if (chain_transit->prev_chain_transit)
     chain_transit->prev_chain_transit->next_chain_transits = eina_list_remove(chain_transit->prev_chain_transit->next_chain_transits, chain_transit);

   chain_transit->prev_chain_transit = transit;
   transit->next_chain_transits = eina_list_append(transit->next_chain_transits, chain_transit);
}

ELM_API void
elm_transit_chain_transit_del(Elm_Transit *transit, Elm_Transit *chain_transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit);
   ELM_TRANSIT_CHECK_OR_RETURN(chain_transit);

   if (chain_transit->prev_chain_transit != transit)
     {
        WRN("A pair of transits does not have the chain relationship! : transit=%p, chain_transit=%p", transit, chain_transit);
        return;
     }

   chain_transit->prev_chain_transit = NULL;
   transit->next_chain_transits = eina_list_remove(transit->next_chain_transits, chain_transit);
}

ELM_API Eina_List *
elm_transit_chain_transits_get(const Elm_Transit * transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   return transit->next_chain_transits;
}

///////////////////////////////////////////////////////////////////////////
//Resizing Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Resizing Elm_Transit_Effect_Resizing;

struct _Elm_Transit_Effect_Resizing
{
   struct _size {
      Evas_Coord w, h;
   } from, to;
};

static void
_transit_effect_resizing_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED)
{
   Elm_Transit_Effect_Resizing *resizing = effect;
   free(resizing);
}

static void
_transit_effect_resizing_op(Elm_Transit_Effect *effect, Elm_Transit *transit, double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Evas_Coord w, h;
   Evas_Object *obj;
   Eina_List *elist;
   Elm_Transit_Effect_Resizing *resizing = effect;

   w = resizing->from.w + (resizing->to.w * progress);
   h = resizing->from.h + (resizing->to.h * progress);

   EINA_LIST_FOREACH(transit->objs, elist, obj)
     evas_object_resize(obj, w, h);
}

static Elm_Transit_Effect *
_transit_effect_resizing_context_new(Evas_Coord from_w, Evas_Coord from_h, Evas_Coord to_w, Evas_Coord to_h)
{
   Elm_Transit_Effect_Resizing *resizing;

   resizing = ELM_NEW(Elm_Transit_Effect_Resizing);
   if (!resizing) return NULL;

   resizing->from.w = from_w;
   resizing->from.h = from_h;
   resizing->to.w = to_w - from_w;
   resizing->to.h = to_h - from_h;

   return resizing;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_resizing_add(Elm_Transit *transit, Evas_Coord from_w, Evas_Coord from_h, Evas_Coord to_w, Evas_Coord to_h)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   Elm_Transit_Effect *effect = _transit_effect_resizing_context_new(from_w, from_h, to_w, to_h);

   if (!effect)
     {
        ERR("Failed to allocate resizing effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_resizing_op, effect,
                          _transit_effect_resizing_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//Translation Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Translation Elm_Transit_Effect_Translation;
typedef struct _Elm_Transit_Effect_Translation_Node Elm_Transit_Effect_Translation_Node;

struct _Elm_Transit_Effect_Translation_Node
{
   Evas_Object *obj;
   Evas_Coord x, y;
};

struct _Elm_Transit_Effect_Translation
{
   struct _position_variation {
      Evas_Coord dx, dy;
   } from, to;
   Eina_List *nodes;
};

static void
_translation_object_del_cb(void *data, const Efl_Event *ev)
{
   Eo* obj = ev->object;
   Elm_Transit_Effect_Translation *translation = data;
   Eina_List *elist;
   Elm_Transit_Effect_Translation_Node *translation_node;

   EINA_LIST_FOREACH(translation->nodes, elist, translation_node)
     {
        if (translation_node->obj != obj) continue;
        translation->nodes = eina_list_remove_list(translation->nodes, elist);
        free(translation_node);
        break;
     }
}

static Eina_List *
_translation_nodes_build(Elm_Transit *transit, Elm_Transit_Effect_Translation *translation)
{
   Elm_Transit_Effect_Translation_Node *translation_node;
   const Eina_List *elist;
   Evas_Object *obj;
   Eina_List *data_list = NULL;
   const Eina_List *objs = elm_transit_objects_get(transit);

   EINA_LIST_FOREACH(objs, elist, obj)
     {
        translation_node = ELM_NEW(Elm_Transit_Effect_Translation_Node);
        if (!translation_node)
          {
             eina_list_free(data_list);
             return NULL;
          }
        translation_node->obj = obj;
        evas_object_geometry_get(obj, &(translation_node->x),
                                 &(translation_node->y), NULL, NULL);
        data_list = eina_list_append(data_list, translation_node);
        efl_event_callback_add(obj, EFL_EVENT_DEL,
                               _translation_object_del_cb, translation);
     }
   return data_list;
}

static void
_transit_effect_translation_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   Elm_Transit_Effect_Translation *translation = effect;
   Eina_List *elist, *elist_next;
   Elm_Transit_Effect_Translation_Node *translation_node;

   EINA_LIST_FOREACH_SAFE(translation->nodes,
                          elist, elist_next, translation_node)
     {
        efl_event_callback_del(translation_node->obj,
                               EFL_EVENT_DEL, _translation_object_del_cb, translation);
        translation->nodes = eina_list_remove_list(translation->nodes, elist);
        free(translation_node);
     }
   free(translation);
}

static void
_transit_effect_translation_op(Elm_Transit_Effect *effect, Elm_Transit *transit, double progress EINA_UNUSED)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Evas_Coord x, y;
   Elm_Transit_Effect_Translation *translation = effect;
   Elm_Transit_Effect_Translation_Node *translation_node;
   Eina_List *elist;

   if (!translation->nodes)
     translation->nodes = _translation_nodes_build(transit, translation);

   EINA_LIST_FOREACH(translation->nodes, elist, translation_node)
     {
        x = translation_node->x + translation->from.dx
           + (translation->to.dx * progress);
        y = translation_node->y + translation->from.dy
           + (translation->to.dy * progress);
        evas_object_move(translation_node->obj, x, y);
     }
}

static Elm_Transit_Effect *
_transit_effect_translation_context_new(Evas_Coord from_dx, Evas_Coord from_dy, Evas_Coord to_dx, Evas_Coord to_dy)
{
   Elm_Transit_Effect_Translation *translation;

   translation = ELM_NEW(Elm_Transit_Effect_Translation);
   if (!translation) return NULL;

   translation->from.dx = from_dx;
   translation->from.dy = from_dy;
   translation->to.dx = to_dx - from_dx;
   translation->to.dy = to_dy - from_dy;

   return translation;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_translation_add(Elm_Transit *transit, Evas_Coord from_dx, Evas_Coord from_dy, Evas_Coord to_dx, Evas_Coord to_dy)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   Elm_Transit_Effect *effect = _transit_effect_translation_context_new(from_dx, from_dy, to_dx, to_dy);

   if (!effect)
     {
        ERR("Failed to allocate translation effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_translation_op, effect,
                          _transit_effect_translation_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//Zoom Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Zoom Elm_Transit_Effect_Zoom;

struct _Elm_Transit_Effect_Zoom
{
   float from, to;
};

static void
_transit_effect_zoom_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED)
{
   Elm_Transit_Effect_Zoom *zoom = effect;
   free(zoom);
}

static void
_transit_effect_zoom_op(Elm_Transit_Effect *effect, Elm_Transit *transit , double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Evas_Object *obj;
   Eina_List *elist;
   Elm_Transit_Effect_Zoom *zoom = effect;
   Elm_Transit_Obj_Data *obj_data;
   Evas_Map *map, *base_map;
   Evas_Coord x, y, w, h;
   double zoom_rate = (zoom->from * (1.0 - progress)) + (zoom->to * progress);

   EINA_LIST_FOREACH(transit->objs, elist, obj)
     {
        obj_data = evas_object_data_get(obj, _transit_key);
        if (obj_data && obj_data->state.map_enabled)
          {
             base_map = obj_data->state.map;
             if (!base_map) return;
             map = evas_map_dup(base_map);
             if (!map) return;
          }
        else
          {
             map = evas_map_new(4);
             if (!map) return;
             evas_map_util_points_populate_from_object_full(map, obj, 0);
          }
        evas_map_util_object_move_sync_set(map, EINA_TRUE);

        evas_object_geometry_get(obj, &x, &y, &w, &h);
        _recover_image_uv(obj, map, EINA_FALSE, EINA_FALSE);
        evas_map_util_zoom(map, zoom_rate, zoom_rate, x + (w / 2), y + (h / 2));
        if (!transit->smooth) evas_map_smooth_set(map, EINA_FALSE);
        evas_object_map_set(obj, map);
        evas_object_map_enable_set(obj, EINA_TRUE);

        evas_map_free(map);
     }
}

static Elm_Transit_Effect *
_transit_effect_zoom_context_new(float from_rate, float to_rate)
{
   Elm_Transit_Effect_Zoom *zoom;

   zoom = ELM_NEW(Elm_Transit_Effect_Zoom);
   if (!zoom) return NULL;

   zoom->from = from_rate;
   zoom->to = to_rate;

   return zoom;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_zoom_add(Elm_Transit *transit, float from_rate, float to_rate)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   Elm_Transit_Effect *effect = _transit_effect_zoom_context_new(from_rate,
                                                                 to_rate);
   if (!effect)
     {
        ERR("Failed to allocate zoom effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_zoom_op, effect,
                          _transit_effect_zoom_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//Flip Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Flip Elm_Transit_Effect_Flip;

struct _Elm_Transit_Effect_Flip
{
   Elm_Transit_Effect_Flip_Axis axis;
   Eina_Bool cw : 1;
};

static void
_transit_effect_flip_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Elm_Transit_Effect_Flip *flip = effect;
   Evas_Object *front, *back;
   int i;
   int count = eina_list_count(transit->objs);

   for (i = 0; i < (count - 1); i += 2)
     {
        front = eina_list_nth(transit->objs, i);
        back = eina_list_nth(transit->objs, i+1);
        evas_object_map_enable_set(front, EINA_FALSE);
        evas_object_map_enable_set(back, EINA_FALSE);
     }
   free(flip);
}

static void
_transit_effect_flip_op(Elm_Transit_Effect *effect, Elm_Transit *transit, double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Evas_Object *obj, *front, *back;
   int count, i;
   Elm_Transit_Effect_Flip *flip = effect;
   Evas_Map *map;
   float degree;
   Evas_Coord x, y, w, h;

   map = evas_map_new(4);
   if (!map) return;

   evas_map_util_object_move_sync_set(map, EINA_TRUE);

   if (flip->cw) degree = (float)(progress * 180);
   else degree = (float)(progress * -180);

   count = eina_list_count(transit->objs);

   for (i = 0; i < (count - 1); i += 2)
     {
        Evas_Coord half_w, half_h;

        front = eina_list_nth(transit->objs, i);
        back = eina_list_nth(transit->objs, i+1);

        if ((degree < 90) && (degree > -90))
          {
             obj = front;
             if (front != back)
               {
                  evas_object_hide(back);
                  evas_object_show(front);
               }
          }
        else
          {
             obj = back;
             if (front != back)
               {
                  evas_object_hide(front);
                  evas_object_show(back);
               }
          }

        evas_map_util_points_populate_from_object_full(map, obj, 0);
        evas_object_geometry_get(obj, &x, &y, &w, &h);
        half_w = (w / 2);
        half_h = (h / 2);

        if (flip->axis == ELM_TRANSIT_EFFECT_FLIP_AXIS_Y)
          {
             if ((degree >= 90) || (degree <= -90))
               {
                  if (!_recover_image_uv(obj, map, EINA_TRUE, EINA_FALSE))
                    {
                       evas_map_point_image_uv_set(map, 0, w, 0);
                       evas_map_point_image_uv_set(map, 1, 0, 0);
                       evas_map_point_image_uv_set(map, 2, 0, h);
                       evas_map_point_image_uv_set(map, 3, w, h);
                    }
               }
             else
               _recover_image_uv(obj, map, EINA_FALSE, EINA_FALSE);
             evas_map_util_3d_rotate(map, 0, degree,
                                     0, x + half_w, y + half_h, 0);
          }
        else
          {
             if ((degree >= 90) || (degree <= -90))
               {
                  if (!_recover_image_uv(obj, map, EINA_TRUE, EINA_TRUE))
                    {
                       evas_map_point_image_uv_set(map, 0, 0, h);
                       evas_map_point_image_uv_set(map, 1, w, h);
                       evas_map_point_image_uv_set(map, 2, w, 0);
                       evas_map_point_image_uv_set(map, 3, 0, 0);
                    }
               }
             else
               _recover_image_uv(obj, map, EINA_FALSE, EINA_FALSE);
             evas_map_util_3d_rotate(map, degree,
                                     0, 0, x + half_w, y + half_h, 0);
          }
        evas_map_util_3d_perspective(map, x + half_w, y + half_h, 0, _TRANSIT_FOCAL);
        evas_object_map_enable_set(front, EINA_TRUE);
        evas_object_map_enable_set(back, EINA_TRUE);
        if (!transit->smooth) evas_map_smooth_set(map, EINA_FALSE);
        evas_object_map_set(obj, map);
     }
   evas_map_free(map);
}

static Elm_Transit_Effect *
_transit_effect_flip_context_new(Elm_Transit_Effect_Flip_Axis axis, Eina_Bool cw)
{
   Elm_Transit_Effect_Flip *flip;

   flip = ELM_NEW(Elm_Transit_Effect_Flip);
   if (!flip) return NULL;

   flip->cw = cw;
   flip->axis = axis;

   return flip;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_flip_add(Elm_Transit *transit, Elm_Transit_Effect_Flip_Axis axis, Eina_Bool cw)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   Elm_Transit_Effect *effect = _transit_effect_flip_context_new(axis, cw);

   if (!effect)
     {
        ERR("Failed to allocate flip effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_flip_op, effect,
                          _transit_effect_flip_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//ResizableFlip Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Resizable_Flip Elm_Transit_Effect_ResizableFlip;
typedef struct _Elm_Transit_Effect_Resizable_Flip_Node Elm_Transit_Effect_ResizableFlip_Node;

struct _Elm_Transit_Effect_Resizable_Flip_Node
{
   Evas_Object *front;
   Evas_Object *back;
   struct _vector2d {
      float x, y;
   } from_pos, from_size, to_pos, to_size;
};

struct _Elm_Transit_Effect_Resizable_Flip
{
   Eina_List *nodes;
   Eina_Bool cw : 1;
   Elm_Transit_Effect_Flip_Axis axis;
};

static void
_resizable_flip_object_del_cb(void *data, const Efl_Event *ev)
{
   Eo* obj = ev->object;
   Elm_Transit_Effect_ResizableFlip *resizable_flip = data;
   Eina_List *elist;
   Elm_Transit_Effect_ResizableFlip_Node *resizable_flip_node;

   EINA_LIST_FOREACH(resizable_flip->nodes, elist, resizable_flip_node)
     {
        if (resizable_flip_node->front == obj)
          efl_event_callback_del(resizable_flip_node->back,
                                 EFL_EVENT_DEL, _resizable_flip_object_del_cb, resizable_flip);
        else if (resizable_flip_node->back == obj)
          efl_event_callback_del(resizable_flip_node->front,
                                 EFL_EVENT_DEL, _resizable_flip_object_del_cb, resizable_flip);
        else continue;

        resizable_flip->nodes = eina_list_remove_list(resizable_flip->nodes,
                                                      elist);
        free(resizable_flip_node);
        break;
     }
}

static Eina_List *
_resizable_flip_nodes_build(Elm_Transit *transit, Elm_Transit_Effect_ResizableFlip *resizable_flip)
{
   Elm_Transit_Effect_ResizableFlip_Node *resizable_flip_node;
   Eina_List *data_list = NULL;
   Evas_Coord front_x, front_y, front_w, front_h;
   Evas_Coord back_x, back_y, back_w, back_h;
   int i, count;

   count = eina_list_count(transit->objs);
   for (i = 0; i < (count - 1); i += 2)
     {
        resizable_flip_node = ELM_NEW(Elm_Transit_Effect_ResizableFlip_Node);
        if (!resizable_flip_node)
          {
             eina_list_free(data_list);
             return NULL;
          }

        resizable_flip_node->front = eina_list_nth(transit->objs, i);
        resizable_flip_node->back = eina_list_nth(transit->objs, i+1);

        evas_object_geometry_get(resizable_flip_node->front,
                                 &front_x, &front_y, &front_w, &front_h);
        evas_object_geometry_get(resizable_flip_node->back,
                                 &back_x, &back_y, &back_w, &back_h);

        resizable_flip_node->from_pos.x = front_x;
        resizable_flip_node->from_pos.y = front_y;
        resizable_flip_node->to_pos.x = back_x - front_x;
        resizable_flip_node->to_pos.y = back_y - front_y;

        resizable_flip_node->from_size.x = front_w;
        resizable_flip_node->from_size.y = front_h;
        resizable_flip_node->to_size.x = back_w - front_w;
        resizable_flip_node->to_size.y = back_h - front_h;

        data_list = eina_list_append(data_list, resizable_flip_node);

        efl_event_callback_add(resizable_flip_node->back,
                               EFL_EVENT_DEL, _resizable_flip_object_del_cb, resizable_flip);
        efl_event_callback_add(resizable_flip_node->front,
                               EFL_EVENT_DEL, _resizable_flip_object_del_cb, resizable_flip);
     }

   return data_list;
}

static void
_set_image_uv_by_axis_y(Evas_Map *map, Elm_Transit_Effect_ResizableFlip_Node *flip, Eina_Bool revert)
{
   if (revert)
     {
        evas_map_point_image_uv_set(map, 0,
                                    (flip->from_size.x * 2) + flip->to_size.x,
                                    0);
        evas_map_point_image_uv_set(map, 1, 0, 0);
        evas_map_point_image_uv_set(map, 2, 0,
                                    (flip->from_size.y * 2) + flip->to_size.y);
        evas_map_point_image_uv_set(map, 3,
                                    (flip->from_size.x * 2) + flip->to_size.x,
                                    (flip->from_size.y * 2) + flip->to_size.y);
     }
   else
     {
        evas_map_point_image_uv_set(map, 0, 0, 0);
        evas_map_point_image_uv_set(map, 1, flip->from_size.x, 0);
        evas_map_point_image_uv_set(map, 2, flip->from_size.x,
                                    flip->from_size.y);
        evas_map_point_image_uv_set(map, 3, 0, flip->from_size.y);
     }
}

static void
_set_image_uv_by_axis_x(Evas_Map *map, Elm_Transit_Effect_ResizableFlip_Node *flip, Eina_Bool revert)
{
   if (revert)
     {
        evas_map_point_image_uv_set(map, 0, 0,
                                    (flip->from_size.y * 2) + flip->to_size.y);
        evas_map_point_image_uv_set(map, 1,
                                    (flip->from_size.x * 2) + flip->to_size.x,
                                    (flip->from_size.y * 2) + flip->to_size.y);
        evas_map_point_image_uv_set(map, 2,
                                    (flip->from_size.x * 2) + flip->to_size.x,
                                    0);
        evas_map_point_image_uv_set(map, 3, 0, 0);
     }
   else
     {
        evas_map_point_image_uv_set(map, 0, 0, 0);
        evas_map_point_image_uv_set(map, 1, flip->from_size.x, 0);
        evas_map_point_image_uv_set(map, 2, flip->from_size.x,
                                    flip->from_size.y);
        evas_map_point_image_uv_set(map, 3, 0, flip->from_size.y);
     }
}

static void
_transit_effect_resizable_flip_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);

   Elm_Transit_Effect_ResizableFlip *resizable_flip = effect;
   Eina_List *elist, *elist_next;
   Elm_Transit_Effect_ResizableFlip_Node *resizable_flip_node;

   EINA_LIST_FOREACH_SAFE(resizable_flip->nodes,
                          elist, elist_next, resizable_flip_node)
     {
        evas_object_map_enable_set(resizable_flip_node->front, EINA_FALSE);
        evas_object_map_enable_set(resizable_flip_node->back, EINA_FALSE);

        resizable_flip->nodes = eina_list_remove_list(resizable_flip->nodes,
                                                      elist);

        efl_event_callback_del(resizable_flip_node->back,
                               EFL_EVENT_DEL, _resizable_flip_object_del_cb, resizable_flip);
        efl_event_callback_del(resizable_flip_node->front,
                               EFL_EVENT_DEL, _resizable_flip_object_del_cb, resizable_flip);
        free(resizable_flip_node);
     }
   free(resizable_flip);
}

static void
_transit_effect_resizable_flip_op(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED, double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   Evas_Map *map;
   Evas_Object *obj;
   float x, y, w, h;
   float degree;
   Evas_Coord half_w, half_h;
   Elm_Transit_Effect_ResizableFlip *resizable_flip = effect;
   Elm_Transit_Effect_ResizableFlip_Node *resizable_flip_node;
   Eina_List *elist;

   map = evas_map_new(4);
   if (!map) return;

   evas_map_util_object_move_sync_set(map, EINA_TRUE);

   if (resizable_flip->cw) degree = (float)(progress * 180);
   else degree = (float)(progress * -180);

   if (!resizable_flip->nodes)
     resizable_flip->nodes = _resizable_flip_nodes_build(transit,
                                                         resizable_flip);

   EINA_LIST_FOREACH(resizable_flip->nodes, elist, resizable_flip_node)
     {
        if ((degree < 90) && (degree > -90))
          {
             obj = resizable_flip_node->front;
             if (resizable_flip_node->front != resizable_flip_node->back)
               {
                  evas_object_hide(resizable_flip_node->back);
                  evas_object_show(resizable_flip_node->front);
               }
          }
        else
          {
             obj = resizable_flip_node->back;
             if (resizable_flip_node->front != resizable_flip_node->back)
               {
                  evas_object_hide(resizable_flip_node->front);
                  evas_object_show(resizable_flip_node->back);
               }
          }

        x = resizable_flip_node->from_pos.x +
           (resizable_flip_node->to_pos.x * progress);
        y = resizable_flip_node->from_pos.y +
           (resizable_flip_node->to_pos.y * progress);
        w = resizable_flip_node->from_size.x +
           (resizable_flip_node->to_size.x * progress);
        h = resizable_flip_node->from_size.y +
           (resizable_flip_node->to_size.y * progress);
        evas_map_point_coord_set(map, 0, x, y, 0);
        evas_map_point_coord_set(map, 1, x + w, y, 0);
        evas_map_point_coord_set(map, 2, x + w, y + h, 0);
        evas_map_point_coord_set(map, 3, x, y + h, 0);

        half_w = (Evas_Coord)(w / 2);
        half_h = (Evas_Coord)(h / 2);

        if (resizable_flip->axis == ELM_TRANSIT_EFFECT_FLIP_AXIS_Y)
          {
             if ((degree >= 90) || (degree <= -90))
                {
                   if (!_recover_image_uv(obj, map, EINA_TRUE, EINA_FALSE))
                     _set_image_uv_by_axis_y(map, resizable_flip_node,
                                             EINA_TRUE);
                }
             else
               {
                  if (!_recover_image_uv(obj, map, EINA_FALSE, EINA_FALSE))
                    _set_image_uv_by_axis_y(map, resizable_flip_node,
                                            EINA_FALSE);
               }
             evas_map_util_3d_rotate(map, 0, degree,
                                     0, x + half_w, y + half_h, 0);
          }
        else
          {
             if ((degree >= 90) || (degree <= -90))
                {
                   if (!_recover_image_uv(obj, map, EINA_TRUE, EINA_TRUE))
                     _set_image_uv_by_axis_x(map, resizable_flip_node,
                                             EINA_TRUE);
                }
             else
               {
                   if (!_recover_image_uv(obj, map, EINA_FALSE, EINA_TRUE))
                     _set_image_uv_by_axis_x(map, resizable_flip_node,
                                             EINA_FALSE);
               }
             evas_map_util_3d_rotate(map, degree, 0,
                                     0, x + half_w, y + half_h, 0);
          }

        evas_map_util_3d_perspective(map, x + half_w, y + half_h, 0,
                                     _TRANSIT_FOCAL);
        evas_object_map_enable_set(resizable_flip_node->front, EINA_TRUE);
        evas_object_map_enable_set(resizable_flip_node->back, EINA_TRUE);
        if (!transit->smooth) evas_map_smooth_set(map, EINA_FALSE);
        evas_object_map_set(obj, map);
     }
   evas_map_free(map);
}

static Elm_Transit_Effect *
_transit_effect_resizable_flip_context_new(Elm_Transit_Effect_Flip_Axis axis, Eina_Bool cw)
{
   Elm_Transit_Effect_ResizableFlip *resizable_flip;

   resizable_flip = ELM_NEW(Elm_Transit_Effect_ResizableFlip);
   if (!resizable_flip) return NULL;

   resizable_flip->cw = cw;
   resizable_flip->axis = axis;

   return resizable_flip;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_resizable_flip_add(Elm_Transit *transit, Elm_Transit_Effect_Flip_Axis axis, Eina_Bool cw)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   Elm_Transit_Effect *effect = _transit_effect_resizable_flip_context_new(axis, cw);

   if (!effect)
     {
        ERR("Failed to allocate resizable_flip effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_resizable_flip_op, effect,
                          _transit_effect_resizable_flip_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//Wipe Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Wipe Elm_Transit_Effect_Wipe;

struct _Elm_Transit_Effect_Wipe
{
   Elm_Transit_Effect_Wipe_Type type;
   Elm_Transit_Effect_Wipe_Dir dir;
};

static void
_elm_fx_wipe_hide(Evas_Map * map, Elm_Transit_Effect_Wipe_Dir dir, Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h, float progress)
{
   Evas_Coord w2, h2;

   switch (dir)
     {
      case ELM_TRANSIT_EFFECT_WIPE_DIR_LEFT:
         w2 = w - (w * progress);
         h2 = (y + h);
         evas_map_point_image_uv_set(map, 0, 0, 0);
         evas_map_point_image_uv_set(map, 1, w2, 0);
         evas_map_point_image_uv_set(map, 2, w2, h);
         evas_map_point_image_uv_set(map, 3, 0, h);
         evas_map_point_coord_set(map, 0, x, y, 0);
         evas_map_point_coord_set(map, 1, x + w2, y, 0);
         evas_map_point_coord_set(map, 2, x + w2, h2, 0);
         evas_map_point_coord_set(map, 3, x, h2, 0);
         break;
      case ELM_TRANSIT_EFFECT_WIPE_DIR_RIGHT:
         w2 = (w * progress);
         h2 = (y + h);
         evas_map_point_image_uv_set(map, 0, w2, 0);
         evas_map_point_image_uv_set(map, 1, w, 0);
         evas_map_point_image_uv_set(map, 2, w, h);
         evas_map_point_image_uv_set(map, 3, w2, h);
         evas_map_point_coord_set(map, 0, x + w2, y, 0);
         evas_map_point_coord_set(map, 1, x + w, y, 0);
         evas_map_point_coord_set(map, 2, x + w, h2, 0);
         evas_map_point_coord_set(map, 3, x + w2, h2, 0);
         break;
      case ELM_TRANSIT_EFFECT_WIPE_DIR_UP:
         w2 = (x + w);
         h2 = h - (h * progress);
         evas_map_point_image_uv_set(map, 0, 0, 0);
         evas_map_point_image_uv_set(map, 1, w, 0);
         evas_map_point_image_uv_set(map, 2, w, h2);
         evas_map_point_image_uv_set(map, 3, 0, h2);
         evas_map_point_coord_set(map, 0, x, y, 0);
         evas_map_point_coord_set(map, 1, w2, y, 0);
         evas_map_point_coord_set(map, 2, w2, y+h2, 0);
         evas_map_point_coord_set(map, 3, x, y+h2, 0);
         break;
      case ELM_TRANSIT_EFFECT_WIPE_DIR_DOWN:
         w2 = (x + w);
         h2 = (h * progress);
         evas_map_point_image_uv_set(map, 0, 0, h2);
         evas_map_point_image_uv_set(map, 1, w, h2);
         evas_map_point_image_uv_set(map, 2, w, h);
         evas_map_point_image_uv_set(map, 3, 0, h);
         evas_map_point_coord_set(map, 0, x, y + h2, 0);
         evas_map_point_coord_set(map, 1, w2, y + h2, 0);
         evas_map_point_coord_set(map, 2, w2, y + h, 0);
         evas_map_point_coord_set(map, 3, x, y + h, 0);
         break;
      default:
         break;
     }
   evas_map_util_3d_perspective(map, x + (w / 2), y + (h / 2), 0, _TRANSIT_FOCAL);
}

static void
_elm_fx_wipe_show(Evas_Map *map, Elm_Transit_Effect_Wipe_Dir dir, Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h, float progress)
{
   Evas_Coord w2, h2;

   switch (dir)
     {
      case ELM_TRANSIT_EFFECT_WIPE_DIR_LEFT:
         w2 = (w - (w * progress));
         h2 = (y + h);
         evas_map_point_image_uv_set(map, 0, w2, 0);
         evas_map_point_image_uv_set(map, 1, w, 0);
         evas_map_point_image_uv_set(map, 2, w, h);
         evas_map_point_image_uv_set(map, 3, w2, h);
         evas_map_point_coord_set(map, 0, x + w2, y, 0);
         evas_map_point_coord_set(map, 1, w, y, 0);
         evas_map_point_coord_set(map, 2, w, h2, 0);
         evas_map_point_coord_set(map, 3, x + w2, h2, 0);
         break;
      case ELM_TRANSIT_EFFECT_WIPE_DIR_RIGHT:
         w2 = (w * progress);
         h2 = (y + h);
         evas_map_point_image_uv_set(map, 0, 0, 0);
         evas_map_point_image_uv_set(map, 1, w2, 0);
         evas_map_point_image_uv_set(map, 2, w2, h);
         evas_map_point_image_uv_set(map, 3, 0, h);
         evas_map_point_coord_set(map, 0, x, y, 0);
         evas_map_point_coord_set(map, 1, x + w2, y, 0);
         evas_map_point_coord_set(map, 2, x + w2, h2, 0);
         evas_map_point_coord_set(map, 3, x, h2, 0);
         break;
      case ELM_TRANSIT_EFFECT_WIPE_DIR_UP:
         w2 = (x + w);
         h2 = (h - (h * progress));
         evas_map_point_image_uv_set(map, 0, 0, h2);
         evas_map_point_image_uv_set(map, 1, w, h2);
         evas_map_point_image_uv_set(map, 2, w, h);
         evas_map_point_image_uv_set(map, 3, 0, h);
         evas_map_point_coord_set(map, 0, x, y + h2, 0);
         evas_map_point_coord_set(map, 1, w2, y + h2, 0);
         evas_map_point_coord_set(map, 2, w2, y + h, 0);
         evas_map_point_coord_set(map, 3, x, y + h, 0);
         break;
      case ELM_TRANSIT_EFFECT_WIPE_DIR_DOWN:
         w2 = (x + w);
         h2 = (h * progress);
         evas_map_point_image_uv_set(map, 0, 0, 0);
         evas_map_point_image_uv_set(map, 1, w, 0);
         evas_map_point_image_uv_set(map, 2, w, h2);
         evas_map_point_image_uv_set(map, 3, 0, h2);
         evas_map_point_coord_set(map, 0, x, y, 0);
         evas_map_point_coord_set(map, 1, w2, y, 0);
         evas_map_point_coord_set(map, 2, w2, y + h2, 0);
         evas_map_point_coord_set(map, 3, x, y + h2, 0);
         break;
      default:
         break;
     }
   evas_map_util_3d_perspective(map, x + (w / 2), y + (h / 2), 0, _TRANSIT_FOCAL);
}

static void
_transit_effect_wipe_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Eina_List *elist;
   Evas_Object *obj;
   Elm_Transit_Effect_Wipe *wipe = effect;
   Eina_Bool reverse = transit->auto_reverse;

   EINA_LIST_FOREACH(transit->objs, elist, obj)
     {
        if ((wipe->type == ELM_TRANSIT_EFFECT_WIPE_TYPE_SHOW && !reverse)
            || (wipe->type == ELM_TRANSIT_EFFECT_WIPE_TYPE_HIDE && reverse))
          evas_object_show(obj);
        else evas_object_hide(obj);
        evas_object_map_enable_set(obj, EINA_FALSE);
     }

   free(wipe);
}

static void
_transit_effect_wipe_op(Elm_Transit_Effect *effect, Elm_Transit *transit, double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Elm_Transit_Effect_Wipe *wipe = effect;
   Evas_Map *map;
   Evas_Coord _x, _y, _w, _h;
   Eina_List *elist;
   Evas_Object *obj;
   const char *type;

   map = evas_map_new(4);
   if (!map) return;

   evas_map_util_object_move_sync_set(map, EINA_TRUE);

   EINA_LIST_FOREACH(transit->objs, elist, obj)
     {
        type = evas_object_type_get(obj);
        if ((!type) || (strcmp(type, "image")) ||
            evas_object_image_source_get(obj))
          evas_object_geometry_get(obj, &_x, &_y, &_w, &_h);
        else
          {
             evas_object_image_size_get(obj, &_w, &_h);
             _x = 0;
             _y = 0;
          }
        if (wipe->type == ELM_TRANSIT_EFFECT_WIPE_TYPE_SHOW)
          _elm_fx_wipe_show(map, wipe->dir, _x, _y, _w, _h, (float)progress);
        else
          _elm_fx_wipe_hide(map, wipe->dir, _x, _y, _w, _h, (float)progress);
        if (!transit->smooth) evas_map_smooth_set(map, EINA_FALSE);
        evas_object_map_enable_set(obj, EINA_TRUE);
        evas_object_map_set(obj, map);
     }
   evas_map_free(map);
}

static Elm_Transit_Effect *
_transit_effect_wipe_context_new(Elm_Transit_Effect_Wipe_Type type, Elm_Transit_Effect_Wipe_Dir dir)
{
   Elm_Transit_Effect_Wipe *wipe;

   wipe = ELM_NEW(Elm_Transit_Effect_Wipe);
   if (!wipe) return NULL;

   wipe->type = type;
   wipe->dir = dir;

   return wipe;
}

ELM_API void *
elm_transit_effect_wipe_add(Elm_Transit *transit, Elm_Transit_Effect_Wipe_Type type, Elm_Transit_Effect_Wipe_Dir dir)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   void *effect = _transit_effect_wipe_context_new(type, dir);

   if (!effect)
     {
        ERR("Failed to allocate wipe effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_wipe_op, effect,
                          _transit_effect_wipe_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//Color Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Color Elm_Transit_Effect_Color;

struct _Elm_Transit_Effect_Color
{
   struct _unsigned_color {
      unsigned int r, g, b, a;
   } from;
   struct _signed_color {
      int r, g, b, a;
   } to;
};

static void
_transit_effect_color_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED)
{
   Elm_Transit_Effect_Color *color = effect;
   free(color);
}

static void
_transit_effect_color_op(Elm_Transit_Effect *effect, Elm_Transit *transit, double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Elm_Transit_Effect_Color *color = effect;
   Evas_Object *obj;
   Eina_List *elist;
   unsigned int r, g, b, a;

   r = (color->from.r + (int)((float)color->to.r * progress));
   g = (color->from.g + (int)((float)color->to.g * progress));
   b = (color->from.b + (int)((float)color->to.b * progress));
   a = (color->from.a + (int)((float)color->to.a * progress));

   EINA_LIST_FOREACH(transit->objs, elist, obj)
     evas_object_color_set(obj, r, g, b, a);
}

static Elm_Transit_Effect *
_transit_effect_color_context_new(unsigned int from_r, unsigned int from_g, unsigned int from_b, unsigned int from_a, unsigned int to_r, unsigned int to_g, unsigned int to_b, unsigned int to_a)
{
   Elm_Transit_Effect_Color *color;

   color = ELM_NEW(Elm_Transit_Effect_Color);
   if (!color) return NULL;

   color->from.r = from_r;
   color->from.g = from_g;
   color->from.b = from_b;
   color->from.a = from_a;
   color->to.r = to_r - from_r;
   color->to.g = to_g - from_g;
   color->to.b = to_b - from_b;
   color->to.a = to_a - from_a;

   return color;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_color_add(Elm_Transit *transit, unsigned int from_r, unsigned int from_g, unsigned int from_b, unsigned int from_a, unsigned int to_r, unsigned int to_g, unsigned int to_b, unsigned int to_a)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   Elm_Transit_Effect *effect = _transit_effect_color_context_new(from_r, from_g, from_b, from_a, to_r, to_g, to_b, to_a);

   if (!effect)
     {
        ERR("Failed to allocate color effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_color_op, effect,
                          _transit_effect_color_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//Fade Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Fade Elm_Transit_Effect_Fade;
typedef struct _Elm_Transit_Effect_Fade_Node Elm_Transit_Effect_Fade_Node;

struct _Elm_Transit_Effect_Fade_Node
{
   Evas_Object *before;
   Evas_Object *after;
   struct _signed_color before_color, after_color;
   int before_alpha;
   int after_alpha;
   Eina_Bool inversed : 1;
};

struct _Elm_Transit_Effect_Fade
{
   Eina_List *nodes;
};

static void
_fade_object_del_cb(void *data, const Efl_Event *ev)
{
   Eo* obj = ev->object;
   Elm_Transit_Effect_Fade *fade = data;
   Eina_List *elist;
   Elm_Transit_Effect_Fade_Node *fade_node;

   EINA_LIST_FOREACH(fade->nodes, elist, fade_node)
     {
        if (fade_node->before == obj)
          efl_event_callback_del(fade_node->after,
                                 EFL_EVENT_DEL, _fade_object_del_cb, fade);
        else if (fade_node->after == obj)
          efl_event_callback_del(fade_node->before,
                                 EFL_EVENT_DEL, _fade_object_del_cb, fade);
        else continue;

        fade->nodes = eina_list_remove_list(fade->nodes, elist);
        free(fade_node);
        break;
     }
}

static Eina_List *
_fade_nodes_build(Elm_Transit *transit, Elm_Transit_Effect_Fade *fade_data)
{
   Elm_Transit_Effect_Fade_Node *fade;
   Eina_List *data_list = NULL;
   int i, count;

   count = eina_list_count(transit->objs);
   for (i = 0; i < count; i += 2)
     {
        fade = ELM_NEW(Elm_Transit_Effect_Fade_Node);
        if (!fade)
          {
             eina_list_free(data_list);
             return NULL;
          }

        fade->before = eina_list_nth(transit->objs, i);
        fade->after = eina_list_nth(transit->objs, i+1);

        evas_object_color_get(fade->before,
                              &fade->before_color.r, &fade->before_color.g,
                              &fade->before_color.b, &fade->before_color.a);
        evas_object_color_get(fade->after,
                              &fade->after_color.r, &fade->after_color.g,
                              &fade->after_color.b, &fade->after_color.a);

        fade->before_alpha = (255 - fade->before_color.a);
        fade->after_alpha = (255 - fade->after_color.a);

        data_list = eina_list_append(data_list, fade);

        efl_event_callback_add(fade->before,
                               EFL_EVENT_DEL, _fade_object_del_cb, fade_data);
        efl_event_callback_add(fade->after,
                               EFL_EVENT_DEL, _fade_object_del_cb, fade_data);
     }
   return data_list;
}

static void
_transit_effect_fade_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   Elm_Transit_Effect_Fade *fade = effect;
   Elm_Transit_Effect_Fade_Node *fade_node;
   Eina_List *elist, *elist_next;

   EINA_LIST_FOREACH_SAFE(fade->nodes, elist, elist_next, fade_node)
     {
        evas_object_color_set(fade_node->before, fade_node->before_color.r,
                              fade_node->before_color.g,
                              fade_node->before_color.b,
                              fade_node->before_color.a);
        evas_object_color_set(fade_node->after, fade_node->after_color.r,
                              fade_node->after_color.g,
                              fade_node->after_color.b,
                              fade_node->after_color.a);

        fade->nodes = eina_list_remove_list(fade->nodes, elist);
        efl_event_callback_del(fade_node->before,
                               EFL_EVENT_DEL, _fade_object_del_cb, fade);
        efl_event_callback_del(fade_node->after,
                               EFL_EVENT_DEL, _fade_object_del_cb, fade);
        free(fade_node);
     }

   free(fade);
}

static void
_transit_effect_fade_op(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED, double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   Elm_Transit_Effect_Fade *fade = effect;
   Eina_List *elist;
   Elm_Transit_Effect_Fade_Node *fade_node;
   float _progress;

   if (!fade->nodes)
     fade->nodes = _fade_nodes_build(transit, fade);

   EINA_LIST_FOREACH(fade->nodes, elist, fade_node)
     {
        if (progress < 0.5)
          {
             if (!fade_node->inversed)
               {
                  evas_object_hide(fade_node->after);
                  evas_object_show(fade_node->before);
                  fade_node->inversed = EINA_TRUE;
               }

             _progress = (1 - (progress * 2));

             evas_object_color_set(fade_node->before,
                                   fade_node->before_color.r * _progress,
                                   fade_node->before_color.g * _progress,
                                   fade_node->before_color.b * _progress,
                                   fade_node->before_color.a +
                                   fade_node->before_alpha * (1 - _progress));
          }
        else
          {
             if (fade_node->inversed)
               {
                  evas_object_hide(fade_node->before);
                  evas_object_show(fade_node->after);
                  fade_node->inversed = EINA_FALSE;
               }

             _progress = ((progress - 0.5) * 2);

             evas_object_color_set(fade_node->after,
                                   fade_node->after_color.r * _progress,
                                   fade_node->after_color.g * _progress,
                                   fade_node->after_color.b * _progress,
                                   fade_node->after_color.a +
                                   fade_node->after_alpha * (1 - _progress));
          }
     }
}

static Elm_Transit_Effect *
_transit_effect_fade_context_new(void)
{
   Elm_Transit_Effect_Fade *fade;
   fade = ELM_NEW(Elm_Transit_Effect_Fade);
   if (!fade) return NULL;
   return fade;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_fade_add(Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);

   Elm_Transit_Effect *effect = _transit_effect_fade_context_new();

   if (!effect)
     {
        ERR("Failed to allocate fade effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_fade_op, effect,
                          _transit_effect_fade_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//Blend Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Blend Elm_Transit_Effect_Blend;
typedef struct _Elm_Transit_Effect_Blend_Node Elm_Transit_Effect_Blend_Node;

struct _Elm_Transit_Effect_Blend_Node
{
   Evas_Object *before;
   Evas_Object *after;
   struct _signed_color from, to;
};

struct _Elm_Transit_Effect_Blend
{
   Eina_List *nodes;
};

static void
_blend_object_del_cb(void *data, const Efl_Event *ev)
{
   Eo* obj = ev->object;
   Elm_Transit_Effect_Blend *blend = data;
   Eina_List *elist;
   Elm_Transit_Effect_Blend_Node *blend_node;

   EINA_LIST_FOREACH(blend->nodes, elist, blend_node)
     {
        if (blend_node->after == obj)
          efl_event_callback_del(blend_node->before,
                                 EFL_EVENT_DEL, _blend_object_del_cb, blend);
        else if (blend_node->before == obj)
          efl_event_callback_del(blend_node->after,
                                 EFL_EVENT_DEL, _blend_object_del_cb, blend);
        else continue;

        blend->nodes = eina_list_remove_list(blend->nodes, elist);
        free(blend_node);
        break;
     }
}

static Eina_List *
_blend_nodes_build(Elm_Transit *transit, Elm_Transit_Effect_Blend *blend)
{
   Elm_Transit_Effect_Blend_Node *blend_node;
   Eina_List *data_list = NULL;
   int i, count;

   count = eina_list_count(transit->objs);
   for (i = 0; i < (count - 1); i += 2)
     {
        blend_node = ELM_NEW(Elm_Transit_Effect_Blend_Node);
        if (!blend_node)
          {
             eina_list_free(data_list);
             return NULL;
          }

        blend_node->before = eina_list_nth(transit->objs, i);
        blend_node->after = eina_list_nth(transit->objs, i + 1);
        evas_object_show(blend_node->before);
        evas_object_show(blend_node->after);

        evas_object_color_get(blend_node->before, &blend_node->from.r,
                              &blend_node->from.g, &blend_node->from.b,
                              &blend_node->from.a);
        evas_object_color_get(blend_node->after, &blend_node->to.r,
                              &blend_node->to.g, &blend_node->to.b,
                              &blend_node->to.a);

        data_list = eina_list_append(data_list, blend_node);

        efl_event_callback_add(blend_node->before,
                               EFL_EVENT_DEL, _blend_object_del_cb, blend);
        efl_event_callback_add(blend_node->after,
                               EFL_EVENT_DEL, _blend_object_del_cb, blend);
     }
   return data_list;
}

static void
_transit_effect_blend_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   Elm_Transit_Effect_Blend *blend = effect;
   Elm_Transit_Effect_Blend_Node *blend_node;
   Eina_List *elist, *elist_next;

   EINA_LIST_FOREACH_SAFE(blend->nodes, elist, elist_next, blend_node)
     {
        evas_object_color_set(blend_node->before,
                              blend_node->from.r, blend_node->from.g,
                              blend_node->from.b, blend_node->from.a);
        evas_object_color_set(blend_node->after, blend_node->to.r,
                              blend_node->to.g, blend_node->to.b,
                              blend_node->to.a);

        if (transit->auto_reverse)
          evas_object_hide(blend_node->after);
        else
          evas_object_hide(blend_node->before);

        blend->nodes = eina_list_remove_list(blend->nodes, elist);

        efl_event_callback_del(blend_node->before,
                               EFL_EVENT_DEL, _blend_object_del_cb, blend);
        efl_event_callback_del(blend_node->after,
                               EFL_EVENT_DEL, _blend_object_del_cb, blend);
        free(blend_node);
     }
   free(blend);
}

static void
_transit_effect_blend_op(Elm_Transit_Effect *effect, Elm_Transit *transit, double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Elm_Transit_Effect_Blend *blend = effect;
   Elm_Transit_Effect_Blend_Node *blend_node;
   Eina_List *elist;

   if (!blend->nodes) blend->nodes = _blend_nodes_build(transit, blend);

   EINA_LIST_FOREACH(blend->nodes, elist, blend_node)
     {
        evas_object_color_set(blend_node->before,
                              (int)(blend_node->from.r * (1 - progress)),
                              (int)(blend_node->from.g * (1 - progress)),
                              (int)(blend_node->from.b * (1 - progress)),
                              (int)(blend_node->from.a * (1 - progress)));
        evas_object_color_set(blend_node->after,
                              (int)(blend_node->to.r * progress),
                              (int)(blend_node->to.g * progress),
                              (int)(blend_node->to.b * progress),
                              (int)(blend_node->to.a * progress));
     }
}

static Elm_Transit_Effect *
_transit_effect_blend_context_new(void)
{
   Elm_Transit_Effect_Blend *blend;

   blend = ELM_NEW(Elm_Transit_Effect_Blend);
   if (!blend) return NULL;
   return blend;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_blend_add(Elm_Transit *transit)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   Elm_Transit_Effect *effect = _transit_effect_blend_context_new();

   if (!effect)
     {
        ERR("Failed to allocate blend effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_blend_op, effect,
                          _transit_effect_blend_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//Rotation Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Rotation Elm_Transit_Effect_Rotation;

struct _Elm_Transit_Effect_Rotation
{
   float from, to;
};

static void
_transit_effect_rotation_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED)
{
   Elm_Transit_Effect_Rotation *rotation = effect;
   free(rotation);
}

static void
_transit_effect_rotation_op(Elm_Transit_Effect *effect, Elm_Transit *transit, double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Elm_Transit_Effect_Rotation *rotation = effect;
   Elm_Transit_Obj_Data *obj_data;
   Evas_Map *map, *base_map;
   Evas_Coord x, y, w, h;
   float degree;
   float half_w, half_h;
   Eina_List *elist;
   Evas_Object *obj;

   EINA_LIST_FOREACH(transit->objs, elist, obj)
     {
        obj_data = evas_object_data_get(obj, _transit_key);
        if (obj_data && obj_data->state.map_enabled)
          {
             base_map = obj_data->state.map;
             if (!base_map) return;
             map = evas_map_dup(base_map);
             if (!map) return;
          }
        else
          {
             map = evas_map_new(4);
             if (!map) return;
             evas_map_util_points_populate_from_object_full(map, obj, 0);
          }
        evas_map_util_object_move_sync_set(map, EINA_TRUE);

        degree = rotation->from + (float)(progress * rotation->to);

        evas_object_geometry_get(obj, &x, &y, &w, &h);

        half_w = (float)w * 0.5;
        half_h = (float)h * 0.5;

        evas_map_util_rotate(map, degree, x + half_w, y + half_h);
        if (!transit->smooth) evas_map_smooth_set(map, EINA_FALSE);
        evas_object_map_enable_set(obj, EINA_TRUE);
        evas_object_map_set(obj, map);

        evas_map_free(map);
     }
}

static Elm_Transit_Effect *
_transit_effect_rotation_context_new(float from_degree, float to_degree)
{
   Elm_Transit_Effect_Rotation *rotation;

   rotation = ELM_NEW(Elm_Transit_Effect_Rotation);
   if (!rotation) return NULL;

   rotation->from = from_degree;
   rotation->to = to_degree - from_degree;

   return rotation;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_rotation_add(Elm_Transit *transit, float from_degree, float to_degree)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   Elm_Transit_Effect *effect = _transit_effect_rotation_context_new(from_degree, to_degree);

   if (!effect)
     {
        ERR("Failed to allocate rotation effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_rotation_op, effect,
                          _transit_effect_rotation_context_free);
   return effect;
}

///////////////////////////////////////////////////////////////////////////
//ImageAnimation Effect
///////////////////////////////////////////////////////////////////////////
typedef struct _Elm_Transit_Effect_Image_Animation Elm_Transit_Effect_Image_Animation;

struct _Elm_Transit_Effect_Image_Animation
{
   Eina_List *images;
   unsigned int prev_idx;
};

static void
_transit_effect_image_animation_context_free(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   Elm_Transit_Effect_Image_Animation *image_animation = effect;
   const char *image;
   Eina_List *elist, *elist_next;

   //FIXME: Reset the image to first one.

   EINA_LIST_FOREACH_SAFE(image_animation->images, elist, elist_next, image)
     {
        image_animation->images =
           eina_list_remove_list(image_animation->images, elist);
        eina_stringshare_del(image);
     }

   free(image_animation);
}

static void
_transit_effect_image_animation_op(Elm_Transit_Effect *effect, Elm_Transit *transit, double progress)
{
   EINA_SAFETY_ON_NULL_RETURN(effect);
   EINA_SAFETY_ON_NULL_RETURN(transit);
   Eina_List *elist;
   Evas_Object *obj;
   const char *type, *type_deprecated;
   Elm_Transit_Effect_Image_Animation *image_animation = effect;
   unsigned int idx = 0;
   int len;

   len = eina_list_count(image_animation->images);
   if (len <= 0) return;

   idx = floor(progress * len);
   if ((int)idx >= len) return;

   if (image_animation->prev_idx == idx) return;

   type = eina_stringshare_add("elm_image");
   //FIXME: Remove later when elm_icon is cleared.
   type_deprecated = eina_stringshare_add("elm_icon");

   EINA_LIST_FOREACH(transit->objs, elist, obj)
     {
        if (elm_widget_type_check(obj, type, __func__) ||
            elm_widget_type_check(obj, type_deprecated, __func__))
          {
             const char *file = eina_list_nth(image_animation->images,
                                              idx);

             elm_image_preload_disabled_set(obj, EINA_TRUE);
             elm_image_file_set(obj, file, NULL);
          }
     }

   eina_stringshare_del(type);
   eina_stringshare_del(type_deprecated);

   image_animation->prev_idx = idx;
}

static Elm_Transit_Effect *
_transit_effect_image_animation_context_new(Eina_List *images)
{
   Elm_Transit_Effect_Image_Animation *image_animation;
   image_animation = ELM_NEW(Elm_Transit_Effect_Image_Animation);

   if (!image_animation) return NULL;
   image_animation->images = images;
   image_animation->prev_idx = -1;
   return image_animation;
}

ELM_API Elm_Transit_Effect *
elm_transit_effect_image_animation_add(Elm_Transit *transit, Eina_List *images)
{
   ELM_TRANSIT_CHECK_OR_RETURN(transit, NULL);
   Elm_Transit_Effect *effect = _transit_effect_image_animation_context_new(images);

   if (!effect)
     {
        ERR("Failed to allocate image_animation effect! : transit=%p", transit);
        return NULL;
     }
   elm_transit_effect_add(transit,
                          _transit_effect_image_animation_op, effect,
                          _transit_effect_image_animation_context_free);
   return effect;
}
