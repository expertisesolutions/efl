#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_ACCESS_WIDGET_ACTION_PROTECTED
#define EFL_UI_WIDGET_PROTECTED
#define ELM_WIDGET_ITEM_PROTECTED
#define EFL_UI_L10N_PROTECTED
#define EFL_PART_PROTECTED

#include <Elementary.h>

#include "elm_priv.h"
#include "elm_widget_ctxpopup.h"
#include "elm_ctxpopup_item_eo.h"
#include "elm_ctxpopup_eo.h"

#include "elm_ctxpopup_part.eo.h"
#include "elm_part_helper.h"

#define MY_CLASS ELM_CTXPOPUP_CLASS

#define MY_CLASS_NAME "Elm_Ctxpopup"
#define MY_CLASS_NAME_LEGACY "elm_ctxpopup"

#define ELM_PRIV_CTXPOPUP_SIGNALS(cmd) \
   cmd(SIG_DISMISSED, "dismissed", "") \
   cmd(SIG_GEOMETRY_UPDATE, "geometry,update", "") \


ELM_PRIV_CTXPOPUP_SIGNALS(ELM_PRIV_STATIC_VARIABLE_DECLARE);

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   ELM_PRIV_CTXPOPUP_SIGNALS(ELM_PRIV_SMART_CALLBACKS_DESC)
   {SIG_WIDGET_LANG_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_WIDGET_ACCESS_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_LAYOUT_FOCUSED, ""}, /**< handled by elm_layout */
   {SIG_LAYOUT_UNFOCUSED, ""}, /**< handled by elm_layout */
   {NULL, NULL}
};
#undef ELM_PRIV_CTXPOPUP_SIGNALS

static Eina_Bool _key_action_escape(Evas_Object *obj, const char *params);

static const Elm_Action key_actions[] = {
   {"escape", _key_action_escape},
   {NULL, NULL}
};

EOLIAN static void
_elm_ctxpopup_efl_ui_l10n_translation_update(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   Eina_List *l;
   Elm_Object_Item *it;

   if (sd->auto_hide) evas_object_hide(obj);

   EINA_LIST_FOREACH(sd->items, l, it)
     elm_wdg_item_translate(it);

   efl_ui_l10n_translation_update(efl_super(obj, MY_CLASS));
}

static Eina_Bool
_key_action_escape(Evas_Object *obj, const char *params EINA_UNUSED)
{
   elm_ctxpopup_dismiss(obj);
   return EINA_TRUE;
}

static void
_x_pos_adjust(Evas_Coord_Point *pos,
              Evas_Coord_Point *base_size,
              Eina_Rectangle *hover_area)
{
   pos->x -= (base_size->x / 2);

   if (pos->x < hover_area->x)
     pos->x = hover_area->x;
   else if ((pos->x + base_size->x) > (hover_area->x + hover_area->w))
     pos->x = (hover_area->x + hover_area->w) - base_size->x;

   if (base_size->x > hover_area->w)
     base_size->x -= (base_size->x - hover_area->w);

   if (pos->x < hover_area->x)
     pos->x = hover_area->x;
}

static void
_y_pos_adjust(Evas_Coord_Point *pos,
              Evas_Coord_Point *base_size,
              Eina_Rectangle *hover_area)
{
   pos->y -= (base_size->y / 2);

   if (pos->y < hover_area->y)
     pos->y = hover_area->y;
   else if ((pos->y + base_size->y) > (hover_area->y + hover_area->h))
     pos->y = hover_area->y + hover_area->h - base_size->y;

   if (base_size->y > hover_area->h)
     base_size->y -= (base_size->y - hover_area->h);

   if (pos->y < hover_area->y)
     pos->y = hover_area->y;
}

static Elm_Ctxpopup_Direction
_base_geometry_calc(Evas_Object *obj,
                    Eina_Rectangle *rect)
{
   Elm_Ctxpopup_Direction dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;
   Eina_Rectangle hover_area;
   Evas_Coord_Point pos = {0, 0};
   Evas_Coord_Point arrow_size;
   Evas_Coord_Point base_size;
   Evas_Coord_Point max_size;
   Evas_Coord_Point min_size;
   Evas_Coord_Point temp;
   int idx;

   ELM_CTXPOPUP_DATA_GET(obj, sd);
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, ELM_CTXPOPUP_DIRECTION_DOWN);

   if (!rect || !sd->parent) return ELM_CTXPOPUP_DIRECTION_DOWN;

   edje_object_part_geometry_get(sd->arrow, "base", NULL, NULL,
                                 &arrow_size.x, &arrow_size.y);
   evas_object_resize(sd->arrow, arrow_size.x, arrow_size.y);

   //Initialize Area Rectangle.
   evas_object_geometry_get
     (sd->parent, &hover_area.x, &hover_area.y, &hover_area.w,
     &hover_area.h);
   if (sd->parent && efl_isa(sd->parent, EFL_UI_WIN_CLASS))
     hover_area.x = hover_area.y = 0;

   evas_object_geometry_get(obj, &pos.x, &pos.y, NULL, NULL);

   //recalc the edje
   edje_object_size_min_calc
     (wd->resize_obj, &base_size.x, &base_size.y);
   evas_object_smart_calculate(wd->resize_obj);

   //Limit to Max Size
   evas_object_size_hint_max_get(obj, &max_size.x, &max_size.y);

   if ((max_size.y > 0) && (base_size.y > max_size.y))
     base_size.y = max_size.y;

   if ((max_size.x > 0) && (base_size.x > max_size.x))
     base_size.x = max_size.x;
   //Limit to Min Size
   evas_object_size_hint_combined_min_get(obj, &min_size.x, &min_size.y);

   if ((min_size.y > 0) && (base_size.y < min_size.y))
     base_size.y = min_size.y;

   if ((min_size.x > 0) && (base_size.x < min_size.x))
     base_size.x = min_size.x;

   //Check available directions. If there is one, it adjusts ctxpopup's position
   // and size.
   for (idx = 0; idx < 4; idx++)
     {
        switch (sd->dir_priority[idx])
          {
           case ELM_CTXPOPUP_DIRECTION_UP:
             temp.y = (pos.y - base_size.y);
             if ((temp.y - arrow_size.y) < hover_area.y)
               continue;

             _x_pos_adjust(&pos, &base_size, &hover_area);
             pos.y -= base_size.y;
             dir = ELM_CTXPOPUP_DIRECTION_UP;
             break;

           case ELM_CTXPOPUP_DIRECTION_LEFT:
             temp.x = (pos.x - base_size.x);
             if ((temp.x - arrow_size.x) < hover_area.x)
               continue;

             _y_pos_adjust(&pos, &base_size, &hover_area);
             pos.x -= base_size.x;
             dir = ELM_CTXPOPUP_DIRECTION_LEFT;
             break;

           case ELM_CTXPOPUP_DIRECTION_RIGHT:
             temp.x = (pos.x + base_size.x);
             if ((temp.x + arrow_size.x) >
                 (hover_area.x + hover_area.w))
               continue;

             _y_pos_adjust(&pos, &base_size, &hover_area);
             dir = ELM_CTXPOPUP_DIRECTION_RIGHT;
             break;

           case ELM_CTXPOPUP_DIRECTION_DOWN:
             temp.y = (pos.y + base_size.y);
             if ((temp.y + arrow_size.y) >
                 (hover_area.y + hover_area.h))
               continue;

             _x_pos_adjust(&pos, &base_size, &hover_area);
             dir = ELM_CTXPOPUP_DIRECTION_DOWN;
             break;

           default:
             continue;
          }
        break;
     }

   //In this case, all directions are invalid because of lack of space.
   if (idx == 4)
     {
        Evas_Coord length[2];

        if (!sd->horizontal)
          {
             length[0] = pos.y - hover_area.y;
             length[1] = (hover_area.y + hover_area.h) - pos.y;

             // ELM_CTXPOPUP_DIRECTION_UP
             if (length[0] > length[1])
               {
                  _x_pos_adjust(&pos, &base_size, &hover_area);
                  pos.y -= base_size.y;
                  dir = ELM_CTXPOPUP_DIRECTION_UP;
                  if (pos.y < (hover_area.y + arrow_size.y))
                    {
                       base_size.y -= ((hover_area.y + arrow_size.y) - pos.y);
                       pos.y = hover_area.y + arrow_size.y;
                    }
               }
             //ELM_CTXPOPUP_DIRECTION_DOWN
             else
               {
                  _x_pos_adjust(&pos, &base_size, &hover_area);
                  dir = ELM_CTXPOPUP_DIRECTION_DOWN;
                  if ((pos.y + arrow_size.y + base_size.y) >
                      (hover_area.y + hover_area.h))
                    base_size.y -=
                      ((pos.y + arrow_size.y + base_size.y) -
                       (hover_area.y + hover_area.h));
               }
          }
        else
          {
             length[0] = pos.x - hover_area.x;
             length[1] = (hover_area.x + hover_area.w) - pos.x;

             //ELM_CTXPOPUP_DIRECTION_LEFT
             if (length[0] > length[1])
               {
                  _y_pos_adjust(&pos, &base_size, &hover_area);
                  pos.x -= base_size.x;
                  dir = ELM_CTXPOPUP_DIRECTION_LEFT;
                  if (pos.x < (hover_area.x + arrow_size.x))
                    {
                       base_size.x -= ((hover_area.x + arrow_size.x) - pos.x);
                       pos.x = hover_area.x + arrow_size.x;
                    }
               }
             //ELM_CTXPOPUP_DIRECTION_RIGHT
             else
               {
                  _y_pos_adjust(&pos, &base_size, &hover_area);
                  dir = ELM_CTXPOPUP_DIRECTION_RIGHT;
                  if (pos.x + (arrow_size.x + base_size.x) >
                      hover_area.x + hover_area.w)
                    base_size.x -=
                      ((pos.x + arrow_size.x + base_size.x) -
                       (hover_area.x + hover_area.w));
               }
          }
     }

   //Final position and size.
   rect->x = pos.x;
   rect->y = pos.y;
   rect->w = base_size.x;
   rect->h = base_size.y;

   return dir;
}

static void
_arrow_update(Evas_Object *obj,
              Elm_Ctxpopup_Direction dir,
              Eina_Rectangle base_size)
{
   Eina_Rectangle arrow_size;
   Evas_Coord x, y;
   double drag;

   ELM_CTXPOPUP_DATA_GET(obj, sd);
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   evas_object_geometry_get(obj, &x, &y, NULL, NULL);
   evas_object_geometry_get
     (sd->arrow, NULL, NULL, &arrow_size.w, &arrow_size.h);

   /* arrow is not being kept as sub-object on purpose, here. the
    * design of the widget does not help with the contrary */

   switch (dir)
     {
      case ELM_CTXPOPUP_DIRECTION_RIGHT:
        edje_object_signal_emit(sd->arrow, "elm,state,left", "elm");
        if (!edje_object_part_swallow(wd->resize_obj,
            (efl_ui_mirrored_get(obj) ? "elm.swallow.arrow_right" :
             "elm.swallow.arrow_left"), sd->arrow))
          {
             evas_object_hide(sd->arrow);
             break;
          }

        if (base_size.h > 0)
          {
             if (y < ((arrow_size.h * 0.5) + base_size.y))
               y = 0;
             else if (y > base_size.y + base_size.h - (arrow_size.h * 0.5))
               y = base_size.h - arrow_size.h;
             else
               y = y - base_size.y - (arrow_size.h * 0.5);
             drag = (double)(y) / (double)(base_size.h - arrow_size.h);
             edje_object_part_drag_value_set
                (wd->resize_obj,
                 (efl_ui_mirrored_get(obj) ? "elm.swallow.arrow_right" :
                  "elm.swallow.arrow_left"), 1, drag);
          }
        break;

      case ELM_CTXPOPUP_DIRECTION_LEFT:
        edje_object_signal_emit(sd->arrow, "elm,state,right", "elm");
        if (!edje_object_part_swallow(wd->resize_obj,
            (efl_ui_mirrored_get(obj) ? "elm.swallow.arrow_left" :
             "elm.swallow.arrow_right"), sd->arrow))
          {
             evas_object_hide(sd->arrow);
             break;
          }

        if (base_size.h > 0)
          {
             if (y < ((arrow_size.h * 0.5) + base_size.y))
               y = 0;
             else if (y > (base_size.y + base_size.h - (arrow_size.h * 0.5)))
               y = base_size.h - arrow_size.h;
             else
               y = y - base_size.y - (arrow_size.h * 0.5);
             drag = (double)(y) / (double)(base_size.h - arrow_size.h);
             edje_object_part_drag_value_set
                (wd->resize_obj,
                 (efl_ui_mirrored_get(obj) ? "elm.swallow.arrow_left" :
                  "elm.swallow.arrow_right"), 0, drag);
          }
        break;

      case ELM_CTXPOPUP_DIRECTION_DOWN:
        edje_object_signal_emit(sd->arrow, "elm,state,top", "elm");
        if (!edje_object_part_swallow(wd->resize_obj, "elm.swallow.arrow_up", sd->arrow))
          {
             evas_object_hide(sd->arrow);
             break;
          }

        if (base_size.w > 0)
          {
             if (x < ((arrow_size.w * 0.5) + base_size.x))
               x = 0;
             else if (x > (base_size.x + base_size.w - (arrow_size.w * 0.5)))
               x = base_size.w - arrow_size.w;
             else
               x = x - base_size.x - (arrow_size.w * 0.5);
             drag = (double)(x) / (double)(base_size.w - arrow_size.w);
             edje_object_part_drag_value_set
               (wd->resize_obj, "elm.swallow.arrow_up", drag,
               1);
          }
        break;

      case ELM_CTXPOPUP_DIRECTION_UP:
        edje_object_signal_emit(sd->arrow, "elm,state,bottom", "elm");
        if (!edje_object_part_swallow(wd->resize_obj, "elm.swallow.arrow_down", sd->arrow))
          {
             evas_object_hide(sd->arrow);
             break;
          }

        if (base_size.w > 0)
          {
             if (x < ((arrow_size.w * 0.5) + base_size.x))
               x = 0;
             else if (x > (base_size.x + base_size.w - (arrow_size.w * 0.5)))
               x = base_size.w - arrow_size.w;
             else x = x - base_size.x - (arrow_size.w * 0.5);
             drag = (double)(x) / (double)(base_size.w - arrow_size.w);
             edje_object_part_drag_value_set
               (wd->resize_obj, "elm.swallow.arrow_down",
               drag, 0);
          }
        break;

      default:
        break;
     }

   //should be here for getting accurate geometry value
   evas_object_smart_calculate(wd->resize_obj);
}

static void
_show_signals_emit(Evas_Object *obj,
                   Elm_Ctxpopup_Direction dir)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (!sd->visible) return;
   if ((sd->list) && (!sd->list_visible)) return;
   if (sd->emitted) return;

   sd->emitted = EINA_TRUE;
   switch (dir)
     {
      case ELM_CTXPOPUP_DIRECTION_UP:
        elm_layout_signal_emit(obj, "elm,state,show,up", "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_LEFT:
        elm_layout_signal_emit(obj,
              (efl_ui_mirrored_get(obj) ? "elm,state,show,right" :
               "elm,state,show,left"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_RIGHT:
        elm_layout_signal_emit(obj,
              (efl_ui_mirrored_get(obj) ? "elm,state,show,left" :
               "elm,state,show,right"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_DOWN:
        elm_layout_signal_emit(obj, "elm,state,show,down", "elm");
        break;

      default:
        break;
     }

   edje_object_signal_emit(sd->bg, "elm,state,show", "elm");
   elm_layout_signal_emit(obj, "elm,state,show", "elm");
}

static void
_hide_signals_emit(Evas_Object *obj,
                   Elm_Ctxpopup_Direction dir)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (!sd->visible) return;

   switch (dir)
     {
      case ELM_CTXPOPUP_DIRECTION_UP:
        elm_layout_signal_emit(obj, "elm,state,hide,up", "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_LEFT:
        elm_layout_signal_emit(obj,
              (efl_ui_mirrored_get(obj) ? "elm,state,hide,right" :
               "elm,state,hide,left"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_RIGHT:
        elm_layout_signal_emit(obj,
              (efl_ui_mirrored_get(obj) ? "elm,state,hide,left" :
               "elm,state,hide,right"), "elm");
        break;

      case ELM_CTXPOPUP_DIRECTION_DOWN:
        elm_layout_signal_emit(obj, "elm,state,hide,down", "elm");
        break;

      default:
        break;
     }
   edje_object_signal_emit(sd->bg, "elm,state,hide", "elm");
   elm_layout_signal_emit(obj, "elm,state,hide", "elm");
}

static void
_base_shift_by_arrow(Evas_Object *arrow,
                     Elm_Ctxpopup_Direction dir,
                     Eina_Rectangle *rect)
{
   Evas_Coord arrow_w, arrow_h;

   evas_object_geometry_get(arrow, NULL, NULL, &arrow_w, &arrow_h);
   switch (dir)
     {
      case ELM_CTXPOPUP_DIRECTION_RIGHT:
        rect->x += arrow_w;
        break;

      case ELM_CTXPOPUP_DIRECTION_LEFT:
        rect->x -= arrow_w;
        break;

      case ELM_CTXPOPUP_DIRECTION_DOWN:
        rect->y += arrow_h;
        break;

      case ELM_CTXPOPUP_DIRECTION_UP:
        rect->y -= arrow_h;
        break;

      default:
        break;
     }
}

EOLIAN static Eina_Bool
_elm_ctxpopup_efl_ui_widget_widget_sub_object_add(Eo *obj, Elm_Ctxpopup_Data *_pd EINA_UNUSED, Evas_Object *sobj)
{
   /* Skipping elm_layout widget_sub_object_add in order to ignore size hint changes.
    * Note: It is not clear WHY we are doing this. Same reason as genlist?
    */
   return elm_widget_sub_object_add(efl_cast(obj, EFL_UI_WIDGET_CLASS), sobj);
}

EOLIAN static void
_elm_ctxpopup_efl_canvas_group_group_calculate(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   Eina_Rectangle rect = { 0, 0, 1, 1 };
   Evas_Coord_Point list_size = { 0, 0 }, parent_size = {0, 0};

   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   if (!sd->arrow) return;  /* simple way to flag "under deletion" */
   if (!sd->parent) return; /* do not calculate sizes unless parent is set */

   //Base
   sd->dir = _base_geometry_calc(obj, &rect);

   _arrow_update(obj, sd->dir, rect);

   _base_shift_by_arrow(sd->arrow, sd->dir, &rect);

   if ((sd->list) && (sd->list_visible))
     {
        evas_object_geometry_get(sd->list, 0, 0, &list_size.x, &list_size.y);
        if ((list_size.x >= rect.w) || (list_size.y >= rect.h))
          {
             elm_list_mode_set(sd->list, ELM_LIST_COMPRESS);
             evas_object_size_hint_min_set(obj, list_size.x, list_size.y);
          }
        /*
            Decrease height of main object initiate recalc of size (sizing_eval).
            At each iteration of calculating size height of the object
            will be closer to the height of the list.
         */
        if (list_size.y < rect.h) rect.h--;
     }
   else if (sd->content)
     {
        evas_object_geometry_get(sd->content, 0, 0, &list_size.x, &list_size.y);
        if ((list_size.x >= rect.w) || (list_size.y >= rect.h))
          evas_object_size_hint_min_set(obj, list_size.x, list_size.y);
     }

   evas_object_geometry_get(sd->parent, NULL, NULL, &parent_size.x, &parent_size.y);
   evas_object_resize(sd->bg, parent_size.x, parent_size.y);

   evas_object_geometry_set(wd->resize_obj, rect.x, rect.y, rect.w, rect.h);

   _show_signals_emit(obj, sd->dir);

   efl_event_callback_legacy_call(obj, ELM_CTXPOPUP_EVENT_GEOMETRY_UPDATE, &rect);
}

static void
_on_parent_del(void *data,
               Evas *e EINA_UNUSED,
               Evas_Object *obj EINA_UNUSED,
               void *event_info EINA_UNUSED)
{
   evas_object_del(data);
}

static void
_on_parent_move(void *data,
                Evas *e EINA_UNUSED,
                Evas_Object *obj EINA_UNUSED,
                void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(data);
}

static void
_on_parent_resize(void *data,
                  Evas *e EINA_UNUSED,
                  Evas_Object *obj EINA_UNUSED,
                  void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);

   if (sd->auto_hide)
     {
        sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

        evas_object_hide(data);
        efl_event_callback_legacy_call(data, ELM_CTXPOPUP_EVENT_DISMISSED, NULL);
     }
   else
     {
        if (sd->visible)
          elm_layout_sizing_eval(data);
     }
}

static void
_parent_detach(Evas_Object *obj)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (!sd->parent) return;

   evas_object_event_callback_del_full
     (sd->parent, EVAS_CALLBACK_DEL, _on_parent_del, obj);
   evas_object_event_callback_del_full
     (sd->parent, EVAS_CALLBACK_MOVE, _on_parent_move, obj);
   evas_object_event_callback_del_full
     (sd->parent, EVAS_CALLBACK_RESIZE, _on_parent_resize, obj);
}

static void
_on_content_del(void *data,
                Evas *e EINA_UNUSED,
                Evas_Object *obj EINA_UNUSED,
                void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);

   sd->content = NULL;
   elm_box_recalculate(sd->box);
   elm_layout_sizing_eval(data);
}

static void
_on_content_resized(void *data,
                    Evas *e EINA_UNUSED,
                    Evas_Object *obj EINA_UNUSED,
                    void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);

   elm_box_recalculate(sd->box);
   elm_layout_sizing_eval(data);
}

//FIXME: lost the content size when theme hook is called.
EOLIAN static Eina_Error
_elm_ctxpopup_efl_ui_widget_theme_apply(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   Eina_Error int_ret = EFL_UI_THEME_APPLY_ERROR_GENERIC;

   int_ret = efl_ui_widget_theme_apply(efl_super(obj, MY_CLASS));
   if (int_ret == EFL_UI_THEME_APPLY_ERROR_GENERIC) return int_ret;

   elm_widget_theme_object_set
     (obj, sd->bg, "ctxpopup", "bg", elm_widget_style_get(obj));
   elm_widget_theme_object_set
     (obj, sd->arrow, "ctxpopup", "arrow", elm_widget_style_get(obj));

   if (sd->list)
     {
        if (eina_str_has_prefix(elm_object_style_get(obj),"default"))
          elm_object_style_set(sd->list, "ctxpopup");
        else
          elm_object_style_set(sd->list, elm_object_style_get(obj));
     }

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(obj);

   return int_ret;
}

/* kind of a big and tricky override here: an internal box will hold
 * the actual content. content aliases won't be of much help here */
static Eina_Bool
_elm_ctxpopup_content_set(Eo *obj, Elm_Ctxpopup_Data *sd, const char *part, Evas_Object *content)
{
   if ((part) && (strcmp(part, "elm.swallow.content")))
     {
        return efl_content_set(efl_part(efl_super(obj, MY_CLASS), part), content);
     }

   if (!content) return EINA_FALSE;

   if (content == sd->content) return EINA_TRUE;

   evas_object_del(sd->content);
   if (sd->content == sd->list) sd->list = NULL;

   evas_object_size_hint_weight_set
     (content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_fill_set
     (content, EVAS_HINT_FILL, EVAS_HINT_FILL);

   elm_box_pack_end(sd->box, content);
   evas_object_show(content);

   sd->content = content;
   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   evas_object_event_callback_add(content, EVAS_CALLBACK_DEL, _on_content_del, obj);

   if (sd->visible) elm_layout_sizing_eval(obj);

   return EINA_TRUE;
}

static Evas_Object*
_elm_ctxpopup_content_get(Eo *obj, Elm_Ctxpopup_Data *sd, const char *part)
{
   if ((part) && (strcmp(part, "elm.swallow.content")))
     return efl_content_get(efl_part(efl_super(obj, MY_CLASS), part));

   return sd->content;
}

static Evas_Object*
_elm_ctxpopup_content_unset(Eo *obj, Elm_Ctxpopup_Data *sd, const char *part)
{
   Evas_Object *content = NULL;

   if ((part) && (strcmp(part, "elm.swallow.content")))
     return efl_content_unset(efl_part(efl_super(obj, MY_CLASS), part));

   content = sd->content;
   if (!content) return content;

   evas_object_event_callback_del(sd->content, EVAS_CALLBACK_DEL, _on_content_del);

   elm_box_unpack(sd->box, content);
   sd->content = NULL;
   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(obj);

   return content;
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_part_text_set(Eo *eo_ctxpopup_it EINA_UNUSED,
                                                 Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                 const char *part,
                                                 const char *label)
{
   if ((part) && (strcmp(part, "default"))) return;

   ELM_CTXPOPUP_DATA_GET(WIDGET(ctxpopup_it), sd);

   elm_object_item_part_text_set(ctxpopup_it->list_item, "default", label);
   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(WIDGET(ctxpopup_it));
}

EOLIAN static const char *
_elm_ctxpopup_item_elm_widget_item_part_text_get(const Eo *eo_ctxpopup_it EINA_UNUSED,
                                                 Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                 const char *part)
{
   if (part && strcmp(part, "default")) return NULL;

   return elm_object_item_part_text_get(ctxpopup_it->list_item, "default");
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_part_content_set(Eo *eo_ctxpopup_it EINA_UNUSED,
                                                    Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                    const char *part,
                                                    Evas_Object *content)
{
   if ((part) && (strcmp(part, "icon"))
       && (strcmp(part, "start"))
       && (strcmp(part, "end"))) return;

   ELM_CTXPOPUP_DATA_GET(WIDGET(ctxpopup_it), sd);

   if ((part) && (!strcmp(part, "end")))
     elm_object_item_part_content_set(ctxpopup_it->list_item, "end", content);
   else
     elm_object_item_part_content_set
       (ctxpopup_it->list_item, "start", content);

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(WIDGET(ctxpopup_it));
}

EOLIAN static Evas_Object *
_elm_ctxpopup_item_elm_widget_item_part_content_get(const Eo *eo_ctxpopup_it EINA_UNUSED,
                                                    Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                    const char *part)
{
   if (part && strcmp(part, "icon") && strcmp(part, "start")
       && strcmp(part, "end")) return NULL;

   if (part && !strcmp(part, "end"))
     return elm_object_item_part_content_get(ctxpopup_it->list_item, "end");
   else
     return elm_object_item_part_content_get(ctxpopup_it->list_item, "start");
}

EOLIAN static Evas_Object *
_elm_ctxpopup_item_elm_widget_item_part_content_unset(Eo *eo_ctxpopup_it EINA_UNUSED,
                                                      Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                                      const char *part)
{
   Evas_Object *content = NULL;

   if ((part) && (strcmp(part, "icon"))
       && (strcmp(part, "start"))
       && (strcmp(part, "end"))) return NULL;

   ELM_CTXPOPUP_DATA_GET(WIDGET(ctxpopup_it), sd);

   if ((part) && (!strcmp(part, "end")))
     content = elm_object_item_part_content_unset(ctxpopup_it->list_item, "end");
   else
     content = elm_object_item_part_content_unset(ctxpopup_it->list_item, "start");

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(WIDGET(ctxpopup_it));

   return content;
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_disable(Eo *eo_ctxpopup_it,
                                           Elm_Ctxpopup_Item_Data *ctxpopup_it)
{
   elm_object_item_disabled_set
     (ctxpopup_it->list_item, elm_wdg_item_disabled_get(eo_ctxpopup_it));
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_del_pre(Eo *eo_ctxpopup_it EINA_UNUSED,
                                           Elm_Ctxpopup_Item_Data *ctxpopup_it)
{
   if (ctxpopup_it->list_item)
     {
        Elm_Ctxpopup_Data *sd = efl_data_scope_safe_get(WIDGET(ctxpopup_it), MY_CLASS);
        if (sd)
          {
             if (sd->list)
               elm_object_item_del(ctxpopup_it->list_item);
             ctxpopup_it->list_item = NULL;
          }
     }
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_signal_emit(Eo *eo_ctxpopup_it EINA_UNUSED,
                                               Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                               const char *emission,
                                               const char *source)
{
   elm_object_item_signal_emit(ctxpopup_it->list_item, emission, source);
}

EOLIAN static void
_elm_ctxpopup_item_elm_widget_item_item_focus_set(Eo *eo_ctxpopup_it EINA_UNUSED,
                                             Elm_Ctxpopup_Item_Data *ctxpopup_it,
                                             Eina_Bool focused)
{
   elm_object_item_focus_set(ctxpopup_it->list_item, focused);
}

EOLIAN static Eina_Bool
_elm_ctxpopup_item_elm_widget_item_item_focus_get(const Eo *eo_ctxpopup_it EINA_UNUSED,
                                             Elm_Ctxpopup_Item_Data *ctxpopup_it)
{
   return elm_object_item_focus_get(ctxpopup_it->list_item);
}

static void
_bg_clicked_cb(void *data,
               Evas_Object *obj EINA_UNUSED,
               const char *emission EINA_UNUSED,
               const char *source EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);

   if (sd->auto_hide)
     _hide_signals_emit(data, sd->dir);
}

static void
_on_show(void *data EINA_UNUSED,
         Evas *e EINA_UNUSED,
         Evas_Object *obj,
         void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (sd->list)
     {
        elm_list_go(sd->list);
        sd->visible = EINA_TRUE;
        /*
         * XXX: Giving focus to the list when it has nothing selected makes
         * it select the first of its items, which makes the popup in
         * Terminology never open and instead just trigger the first option.
         * I'll let as an exercise to the reader to figure out why that
         * is so fucking annoying. Extra points for noting why this is my
         * choice of a "fix" instead of fixing the actual focus/select issue
         * that seems to be spread all over Elementary.
         */
        //elm_object_focus_set(sd->list, EINA_TRUE);
        return;
     }

   if (!sd->content) return;

   sd->emitted = EINA_FALSE;
   sd->visible = EINA_TRUE;

   _show_signals_emit(obj, sd->dir);

   elm_layout_sizing_eval(obj);
   /*
    * XXX: see above comment, but for any swallowed list-type object
    */
   //elm_object_focus_set(obj, EINA_TRUE);
}

static void
_on_hide(void *data EINA_UNUSED,
         Evas *e EINA_UNUSED,
         Evas_Object *obj,
         void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(obj, sd);

   if (!sd->visible) return;

   sd->visible = EINA_FALSE;
   sd->list_visible = EINA_FALSE;
}

static void
_on_move(void *data EINA_UNUSED,
         Evas *e EINA_UNUSED,
         Evas_Object *obj,
         void *event_info EINA_UNUSED)
{
   elm_layout_sizing_eval(obj);
}

static void
_hide_finished_cb(void *data,
                  Evas_Object *obj EINA_UNUSED,
                  const char *emission EINA_UNUSED,
                  const char *source EINA_UNUSED)
{
   evas_object_hide(data);
   efl_event_callback_legacy_call(data, ELM_CTXPOPUP_EVENT_DISMISSED, NULL);
}

static void
_list_resize_cb(void *data,
                Evas *e EINA_UNUSED,
                Evas_Object *obj EINA_UNUSED,
                void *event_info EINA_UNUSED)
{
   ELM_CTXPOPUP_DATA_GET(data, sd);

   if (!sd->visible) return;
   if (sd->list_visible) return;

   sd->list_visible = EINA_TRUE;

   elm_layout_sizing_eval(data);
}

static void
_list_del(Elm_Ctxpopup_Data *sd)
{
   ELM_SAFE_FREE(sd->list, evas_object_del);
}

EOLIAN static void
_elm_ctxpopup_item_efl_object_destructor(Eo *eo_ctxpopup_it,
                                      Elm_Ctxpopup_Item_Data *ctxpopup_it)
{
   Evas_Object *list;

   ELM_CTXPOPUP_DATA_GET(WIDGET(ctxpopup_it), sd);

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   list = elm_object_item_widget_get(ctxpopup_it->list_item);

   if (ctxpopup_it->list_item)
     {
        elm_object_item_del(ctxpopup_it->list_item);
        ctxpopup_it->list_item = NULL;
     }
   sd->items = eina_list_remove(sd->items, eo_ctxpopup_it);
   if (list && eina_list_count(elm_list_items_get(list)) < 2)
     {
        evas_object_hide(WIDGET(ctxpopup_it));
     }
   else
     {
        if (sd->list_visible) elm_layout_sizing_eval(WIDGET(ctxpopup_it));
     }

   efl_destructor(efl_super(eo_ctxpopup_it, ELM_CTXPOPUP_ITEM_CLASS));
}

EOLIAN static void
_elm_ctxpopup_efl_canvas_group_group_add(Eo *obj, Elm_Ctxpopup_Data *priv)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   efl_canvas_group_add(efl_super(obj, MY_CLASS));

   if (!elm_layout_theme_set
       (obj, "ctxpopup", "base", elm_widget_style_get(obj)))
     CRI("Failed to set layout!");

   elm_layout_signal_callback_add
     (obj, "elm,action,hide,finished", "*", _hide_finished_cb, obj);

   //Background
   priv->bg = edje_object_add(evas_object_evas_get(obj));
   if (elm_widget_theme_object_set(obj, priv->bg, "ctxpopup", "bg", "default"))
     CRI("ctxpopup(%p) failed to set theme [efl/ctxpopup/bg/default]!", obj);

   edje_object_signal_callback_add
     (priv->bg, "elm,action,click", "*", _bg_clicked_cb, obj);
   evas_object_smart_member_add(priv->bg, obj);
   evas_object_stack_below(priv->bg, wd->resize_obj);

   //Arrow
   priv->arrow = edje_object_add(evas_object_evas_get(obj));
   elm_widget_theme_object_set
     (obj, priv->arrow, "ctxpopup", "arrow", "default");
   evas_object_smart_member_add(priv->arrow, obj);

   priv->dir_priority[0] = ELM_CTXPOPUP_DIRECTION_UP;
   priv->dir_priority[1] = ELM_CTXPOPUP_DIRECTION_LEFT;
   priv->dir_priority[2] = ELM_CTXPOPUP_DIRECTION_RIGHT;
   priv->dir_priority[3] = ELM_CTXPOPUP_DIRECTION_DOWN;
   priv->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;
   priv->auto_hide = EINA_TRUE;

   priv->box = elm_box_add(obj);
   evas_object_size_hint_weight_set
     (priv->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   evas_object_event_callback_add
     (priv->box, EVAS_CALLBACK_RESIZE, _on_content_resized, obj);

   /* box will be our content placeholder, thus the parent's version call */
   efl_content_set(efl_part(efl_super(obj, MY_CLASS), "elm.swallow.content"), priv->box);

   evas_object_event_callback_add(obj, EVAS_CALLBACK_SHOW, _on_show, NULL);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_HIDE, _on_hide, NULL);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_MOVE, _on_move, NULL);

   elm_widget_can_focus_set(obj, EINA_TRUE);
}

EOLIAN static void
_elm_ctxpopup_efl_canvas_group_group_del(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   Elm_Object_Item *it;

   if (sd->content) evas_object_event_callback_del(sd->content, EVAS_CALLBACK_DEL, _on_content_del);
   _parent_detach(obj);

   //clear the items before clearing the ctxpopup as this will remove the list and all items
   EINA_LIST_FREE(sd->items, it)
     efl_del(it);

   elm_ctxpopup_clear(obj);
   ELM_SAFE_FREE(sd->arrow, evas_object_del); /* stops _sizing_eval() from going on on deletion */
   ELM_SAFE_FREE(sd->bg, evas_object_del);

   efl_canvas_group_del(efl_super(obj, MY_CLASS));
}

ELM_API Evas_Object *
elm_ctxpopup_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = elm_legacy_add(MY_CLASS, parent);

   /* access: parent could be any object such as elm_list which does
      not know elc_ctxpopup as its child object in the focus_next(); */
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, NULL);
   wd->highlight_root = EINA_TRUE;

   return obj;
}

EOLIAN static Eo *
_elm_ctxpopup_efl_object_constructor(Eo *obj, Elm_Ctxpopup_Data *_pd EINA_UNUSED)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   evas_object_smart_callbacks_descriptions_set(obj, _smart_callbacks);
   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_POPUP_MENU);
   //default parent is to be hover parent
   elm_ctxpopup_hover_parent_set(obj, efl_parent_get(obj));
   return obj;
}

EOLIAN static void
_elm_ctxpopup_hover_parent_set(Eo *obj, Elm_Ctxpopup_Data *sd, Evas_Object *parent)
{
   Evas_Coord x, y, w, h;

   if (!parent) return;

   _parent_detach(obj);

   evas_object_event_callback_add
     (parent, EVAS_CALLBACK_DEL, _on_parent_del, obj);
   evas_object_event_callback_add
     (parent, EVAS_CALLBACK_MOVE, _on_parent_move, obj);
   evas_object_event_callback_add
     (parent, EVAS_CALLBACK_RESIZE, _on_parent_resize, obj);

   sd->parent = parent;

   //Update Background
   evas_object_geometry_get(parent, &x, &y, &w, &h);
   if (parent && efl_isa(parent, EFL_UI_WIN_CLASS))
     x = y = 0;
   evas_object_geometry_set(sd->bg, x, y, w, h);

   if (sd->visible) elm_layout_sizing_eval(obj);
}

EOLIAN static Evas_Object*
_elm_ctxpopup_hover_parent_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   return sd->parent;
}

EOLIAN static void
_elm_ctxpopup_clear(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   _list_del(sd);
   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;
}

EOLIAN static void
_elm_ctxpopup_horizontal_set(Eo *obj, Elm_Ctxpopup_Data *sd, Eina_Bool horizontal)
{
   sd->horizontal = !!horizontal;

   if (!sd->list) return;

   elm_list_horizontal_set(sd->list, sd->horizontal);

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;

   if (sd->visible) elm_layout_sizing_eval(obj);
}

EOLIAN static Eina_Bool
_elm_ctxpopup_horizontal_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   return sd->horizontal;
}

static void
_item_select(Elm_Ctxpopup_Item_Data *item)
{
   Elm_Object_Item *eo_item2;
   Eina_List *l;

   ELM_CTXPOPUP_DATA_GET_OR_RETURN(item->wcb.cobj, sd);
   EINA_LIST_FOREACH(sd->items, l, eo_item2)
     {
        ELM_CTXPOPUP_ITEM_DATA_GET(eo_item2, item2);
        item2->selected = EINA_FALSE;
     }
   item->selected = EINA_TRUE;

   if (!item->wcb.org_func_cb) return;
   item->wcb.org_func_cb((void *)item->wcb.org_data, item->wcb.cobj, EO_OBJ(item));
}

static void
_item_wrap_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Elm_Ctxpopup_Item_Data *item = data;
   _item_select(item);
}

EOLIAN static Eo *
_elm_ctxpopup_item_efl_object_constructor(Eo *obj, Elm_Ctxpopup_Item_Data *it)
{
   obj = efl_constructor(efl_super(obj, ELM_CTXPOPUP_ITEM_CLASS));
   it->base = efl_data_scope_get(obj, ELM_WIDGET_ITEM_CLASS);

   return obj;
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_item_insert_before(Eo *obj, Elm_Ctxpopup_Data *sd, Elm_Object_Item *eo_before, const char *label, Evas_Object *icon, Evas_Smart_Cb func, const void *data)
{
   Eo *eo_item;

   EINA_SAFETY_ON_NULL_RETURN_VAL(eo_before, NULL);
   ELM_CTXPOPUP_ITEM_DATA_GET(eo_before, before_it);
   ELM_CTXPOPUP_ITEM_CHECK_OR_RETURN(before_it,  NULL);

   if (!before_it->list_item) return NULL;

   eo_item = efl_add(ELM_CTXPOPUP_ITEM_CLASS, obj, elm_obj_ctxpopup_item_init(efl_added, func, data));
   if (!eo_item) return NULL;

   ELM_CTXPOPUP_ITEM_DATA_GET(eo_item, item);

   item->list_item =
     elm_list_item_insert_before(sd->list, before_it->list_item, label, icon, NULL, _item_wrap_cb, item);
   sd->items = eina_list_prepend_relative(sd->items, eo_item, eo_before);

   if (sd->visible) elm_layout_sizing_eval(obj);

   return eo_item;
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_item_insert_after(Eo *obj, Elm_Ctxpopup_Data *sd, Elm_Object_Item *eo_after, const char *label, Evas_Object *icon, Evas_Smart_Cb func, const void *data)
{
   Eo *eo_item;

   EINA_SAFETY_ON_NULL_RETURN_VAL(eo_after, NULL);
   ELM_CTXPOPUP_ITEM_DATA_GET(eo_after, after_it);
   ELM_CTXPOPUP_ITEM_CHECK_OR_RETURN(after_it,  NULL);

   if (!after_it->list_item) return NULL;

   eo_item = efl_add(ELM_CTXPOPUP_ITEM_CLASS, obj, elm_obj_ctxpopup_item_init(efl_added, func, data));
   if (!eo_item) return NULL;

   ELM_CTXPOPUP_ITEM_DATA_GET(eo_item, item);

   item->list_item =
     elm_list_item_insert_after(sd->list, after_it->list_item, label, icon, NULL, _item_wrap_cb, item);
   sd->items = eina_list_append_relative(sd->items, eo_item, eo_after);

   if (sd->visible) elm_layout_sizing_eval(obj);

   return eo_item;
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_item_append(Eo *obj, Elm_Ctxpopup_Data *sd, const char *label, Evas_Object *icon, Evas_Smart_Cb func, const void *data)
{
   Eo *eo_item;

   eo_item = efl_add(ELM_CTXPOPUP_ITEM_CLASS, obj, elm_obj_ctxpopup_item_init(efl_added, func, data));
   if (!eo_item) return NULL;

   ELM_CTXPOPUP_ITEM_DATA_GET(eo_item, item);

   item->list_item =
     elm_list_item_append(sd->list, label, icon, NULL, _item_wrap_cb, item);
   sd->items = eina_list_append(sd->items, eo_item);

   if (sd->visible) elm_layout_sizing_eval(obj);

   return eo_item;
}

EOLIAN static void
_elm_ctxpopup_direction_priority_set(Eo *obj, Elm_Ctxpopup_Data *sd, Elm_Ctxpopup_Direction first, Elm_Ctxpopup_Direction second, Elm_Ctxpopup_Direction third, Elm_Ctxpopup_Direction fourth)
{
   sd->dir_priority[0] = first;
   sd->dir_priority[1] = second;
   sd->dir_priority[2] = third;
   sd->dir_priority[3] = fourth;

   if (sd->visible) elm_layout_sizing_eval(obj);
}

EOLIAN static void
_elm_ctxpopup_direction_priority_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd, Elm_Ctxpopup_Direction *first, Elm_Ctxpopup_Direction *second, Elm_Ctxpopup_Direction *third, Elm_Ctxpopup_Direction *fourth)
{
   if (first) *first = sd->dir_priority[0];
   if (second) *second = sd->dir_priority[1];
   if (third) *third = sd->dir_priority[2];
   if (fourth) *fourth = sd->dir_priority[3];
}

EOLIAN static Elm_Ctxpopup_Direction
_elm_ctxpopup_direction_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   return sd->dir;
}

EOLIAN static void
_elm_ctxpopup_dismiss(Eo *obj, Elm_Ctxpopup_Data *sd)
{
   _hide_signals_emit(obj, sd->dir);
}

EOLIAN static void
_elm_ctxpopup_auto_hide_disabled_set(Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd, Eina_Bool disabled)
{
   if (sd->auto_hide == !disabled) return;
   sd->auto_hide = !disabled;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_auto_hide_disabled_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   return !sd->auto_hide;
}

EOLIAN static void
_elm_ctxpopup_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

EOLIAN static const Eina_List *
_elm_ctxpopup_items_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   return sd->items;
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_first_item_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   if (!sd->items) return NULL;

   return eina_list_data_get(sd->items);
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_last_item_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   if (!sd->items) return NULL;

   return eina_list_data_get(eina_list_last(sd->items));
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_selected_item_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   Eina_List *l;
   Elm_Object_Item *eo_item;

   EINA_LIST_FOREACH(sd->items, l, eo_item)
     {
        ELM_CTXPOPUP_ITEM_DATA_GET(eo_item, item);
        if (item->selected) return eo_item;
     }

   return NULL;
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_elm_widget_item_container_focused_item_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd)
{
   if (!sd->list) return NULL;

   return elm_object_focused_item_get(sd->list);
}

EOLIAN static Elm_Object_Item*
_elm_ctxpopup_item_prepend(Eo *obj, Elm_Ctxpopup_Data *sd, const char *label, Evas_Object *icon, Evas_Smart_Cb func, const void *data)
{
   Eo *eo_item;

   eo_item = efl_add(ELM_CTXPOPUP_ITEM_CLASS, obj, elm_obj_ctxpopup_item_init(efl_added, func, data));
   if (!eo_item) return NULL;

   ELM_CTXPOPUP_ITEM_DATA_GET(eo_item, item);

   item->list_item =
     elm_list_item_prepend(sd->list, label, icon, NULL, _item_wrap_cb, item);
   sd->items = eina_list_prepend(sd->items, eo_item);

   if (sd->visible) elm_layout_sizing_eval(obj);

   return eo_item;
}

EOLIAN static Elm_Object_Item *
_elm_ctxpopup_item_prev_get(const Eo *eo_item EINA_UNUSED, Elm_Ctxpopup_Item_Data *item)
{
   Eina_List *l;

   ELM_CTXPOPUP_DATA_GET(WIDGET(item), sd);

   if (!sd->items) return NULL;

   l = eina_list_data_find_list(sd->items, eo_item);
   if (l && l->prev) return eina_list_data_get(l->prev);

   return NULL;
}

EOLIAN static Elm_Object_Item *
_elm_ctxpopup_item_next_get(const Eo *eo_item EINA_UNUSED, Elm_Ctxpopup_Item_Data *item)
{
   Eina_List *l;

   ELM_CTXPOPUP_DATA_GET(WIDGET(item), sd);

   if (!sd->items) return NULL;

   l = eina_list_data_find_list(sd->items, eo_item);
   if (l && l->next) return eina_list_data_get(l->next);

   return NULL;
}

EOLIAN static void
_elm_ctxpopup_item_selected_set(Eo *eo_item EINA_UNUSED,
                                            Elm_Ctxpopup_Item_Data *item,
                                            Eina_Bool selected)
{
   Elm_Object_Item *eo_item2;
   Eina_List *l;

   if (selected == item->selected) return;

   if (selected)
     {
        ELM_CTXPOPUP_DATA_GET_OR_RETURN(item->wcb.cobj, sd);
        EINA_LIST_FOREACH(sd->items, l, eo_item2)
          {
             ELM_CTXPOPUP_ITEM_DATA_GET(eo_item2, item2);
             item2->selected = EINA_FALSE;
          }
     }
   elm_list_item_selected_set(item->list_item, selected);
   item->selected = selected;
}

EOLIAN static Eina_Bool
_elm_ctxpopup_item_selected_get(const Eo *eo_item EINA_UNUSED, Elm_Ctxpopup_Item_Data *item)
{
   return item->selected;
}

EOLIAN static void
_elm_ctxpopup_item_init(Eo *eo_item,
          Elm_Ctxpopup_Item_Data *item,
          Evas_Smart_Cb func,
          const void *data)
{
   Eo *obj;
   obj = efl_parent_get(eo_item);
   Elm_Ctxpopup_Data *sd = efl_data_scope_get(obj, ELM_CTXPOPUP_CLASS);
   if (!sd->list)
     {
        sd->list = elm_list_add(obj);
        if (eina_str_has_prefix(elm_object_style_get(obj),"default"))
          elm_object_style_set(sd->list, "ctxpopup");
        else elm_object_style_set(sd->list, elm_object_style_get(obj));
        elm_list_mode_set(sd->list, ELM_LIST_EXPAND);
        elm_list_horizontal_set(sd->list, sd->horizontal);
        evas_object_event_callback_add
          (sd->list, EVAS_CALLBACK_RESIZE, _list_resize_cb, obj);
        elm_layout_content_set(obj, "default", sd->list);
     }

   item->wcb.org_func_cb = func;
   item->wcb.org_data = data;
   item->wcb.cobj = obj;

   sd->dir = ELM_CTXPOPUP_DIRECTION_UNKNOWN;
}

EOLIAN static const Efl_Access_Action_Data*
_elm_ctxpopup_efl_access_widget_action_elm_actions_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Data *sd EINA_UNUSED)
{
   static Efl_Access_Action_Data atspi_actions[] = {
          { "escape", "escape", NULL, _key_action_escape},
          { NULL, NULL, NULL, NULL }
   };
   return &atspi_actions[0];
}

EOLIAN static Efl_Access_State_Set
_elm_ctxpopup_efl_access_object_state_set_get(const Eo *obj, Elm_Ctxpopup_Data *sd EINA_UNUSED)
{
   Efl_Access_State_Set ret;
   ret = efl_access_object_state_set_get(efl_super(obj, MY_CLASS));

   STATE_TYPE_SET(ret, EFL_ACCESS_STATE_TYPE_MODAL);

   return ret;
}

static Eina_Bool
_item_access_action_activate(Evas_Object *obj, const char *params EINA_UNUSED)
{
   ELM_CTXPOPUP_ITEM_DATA_GET(obj, it);
   if (!it) return EINA_FALSE;

   _item_select(it);
   return EINA_TRUE;
}

EOLIAN static const Efl_Access_Action_Data*
_elm_ctxpopup_item_efl_access_widget_action_elm_actions_get(const Eo *obj EINA_UNUSED, Elm_Ctxpopup_Item_Data *sd EINA_UNUSED)
{
   static Efl_Access_Action_Data atspi_actions[] = {
          { "activate", NULL, NULL, _item_access_action_activate},
          { NULL, NULL, NULL, NULL }
   };
   return &atspi_actions[0];
}

/* Internal EO APIs and hidden overrides */

ELM_WIDGET_KEY_DOWN_DEFAULT_IMPLEMENT(elm_ctxpopup, Elm_Ctxpopup_Data)

/* Efl.Part begin */

ELM_PART_OVERRIDE(elm_ctxpopup, ELM_CTXPOPUP, Elm_Ctxpopup_Data)
ELM_PART_OVERRIDE_CONTENT_SET(elm_ctxpopup, ELM_CTXPOPUP, Elm_Ctxpopup_Data)
ELM_PART_OVERRIDE_CONTENT_GET(elm_ctxpopup, ELM_CTXPOPUP, Elm_Ctxpopup_Data)
ELM_PART_OVERRIDE_CONTENT_UNSET(elm_ctxpopup, ELM_CTXPOPUP, Elm_Ctxpopup_Data)
#include "elm_ctxpopup_part.eo.c"

/* Efl.Part end */

/* Internal EO APIs and hidden overrides */

#define ELM_CTXPOPUP_EXTRA_OPS \
   EFL_CANVAS_GROUP_CALC_OPS(elm_ctxpopup), \
   EFL_CANVAS_GROUP_ADD_DEL_OPS(elm_ctxpopup)

#include "elm_ctxpopup_item_eo.c"
#include "elm_ctxpopup_eo.c"
