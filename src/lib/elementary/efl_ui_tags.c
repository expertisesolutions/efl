#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_UI_FORMAT_PROTECTED

#include <Elementary.h>
#include "elm_priv.h"
#include "efl_ui_tags_private.h"
#include "elm_entry_eo.h"

#define MY_CLASS EFL_UI_TAGS_CLASS

#define MY_CLASS_NAME "Efl.Ui.Tags"

#define MIN_W_ENTRY 10

static const char PART_NAME_BUTTON[] = "btn";
static const char PART_NAME_LABEL[] = "label";
static const char PART_NAME_NUMBER[] = "number";

static Eina_Bool _efl_ui_tags_smart_focus_direction_enable = EINA_TRUE;

static void _entry_changed_cb(void *data, const Efl_Event *event);
static void _entry_focus_changed_cb(void *data, const Efl_Event *event);
static void _entry_clicked_cb(void *data, const Efl_Event *event);

EFL_CALLBACKS_ARRAY_DEFINE(_tags_cb,
   { ELM_ENTRY_EVENT_CHANGED, _entry_changed_cb },
   { EFL_UI_FOCUS_OBJECT_EVENT_FOCUS_CHANGED , _entry_focus_changed_cb },
   { EFL_INPUT_EVENT_CLICKED, _entry_clicked_cb }
);

static void
_shrink_mode_set(Eo *obj,
                 Eina_Bool shrink)
{
   int count;
   Eina_List *l;
   Eo *layout;

   Efl_Ui_Tags_Data *sd = efl_data_scope_get(obj, EFL_UI_TAGS_CLASS);

   if (!sd->layouts) return;
   if (sd->view_state == TAGS_VIEW_ENTRY)
     evas_object_hide(sd->entry);
   else if (sd->view_state == TAGS_VIEW_SHRINK)
     evas_object_hide(sd->end);

   if (shrink == EINA_TRUE)
     {
        Evas_Coord w = 0;
        unsigned int box_inner_item_width_padding = 0;
        Eina_Value val;

        efl_gfx_arrangement_content_padding_get(sd->box, &box_inner_item_width_padding, NULL);
        // unpack all items and entry
        efl_pack_unpack_all(sd->box);
        EINA_LIST_FOREACH(sd->layouts, l, layout)
          {
             evas_object_hide(layout);
          }
        // pack buttons only 1line
        w = sd->w_box;

        if (sd->label && sd->label_packed)
          {
             efl_pack(sd->box, sd->label);
             Eina_Size2D label_min =
                efl_gfx_hint_size_combined_min_get(sd->label);
             w -= label_min.w;
             w -= (Evas_Coord)box_inner_item_width_padding;
          }

        layout = NULL;
        count = eina_list_count(sd->layouts);

        EINA_LIST_FOREACH(sd->layouts, l, layout)
          {
             Evas_Coord w_label_count = 0, h = 0;

             efl_pack(sd->box, layout);
             evas_object_show(layout);

             Eina_Size2D item_min =
                efl_gfx_hint_size_combined_min_get(layout);

             w -= item_min.w;
             w -= box_inner_item_width_padding;
             count--;

             eina_value_setup(&val, EINA_VALUE_TYPE_INT);

             if (count > 0)
               {
                  Eina_Strbuf *strbuf = eina_strbuf_new();
                  eina_value_set(&val, count);
                  efl_ui_format_formatted_value_get(obj, strbuf, val);
                  edje_object_part_text_escaped_set(sd->end, "efl.text",
                                                    eina_strbuf_string_get(strbuf));
                  eina_strbuf_free(strbuf);

                  edje_object_size_min_calc(sd->end, &w_label_count, NULL);
                  elm_coords_finger_size_adjust(1, &w_label_count, 1, NULL);
               }

             if ((w < 0) || (w < w_label_count))
               {
                  Eina_Strbuf *strbuf = eina_strbuf_new();
                  efl_pack_unpack(sd->box, layout);
                  evas_object_hide(layout);
                  count++;

                  eina_value_set(&val, count);
                  efl_ui_format_formatted_value_get(obj, strbuf, val);
                  edje_object_part_text_escaped_set(sd->end, "efl.text",
                                                    eina_strbuf_string_get(strbuf));
                  eina_strbuf_free(strbuf);

                  edje_object_size_min_calc(sd->end, &w_label_count, &h);
                  elm_coords_finger_size_adjust(1, &w_label_count, 1, &h);
                  efl_gfx_hint_size_min_set(sd->end, EINA_SIZE2D(w_label_count, h));
                  efl_pack(sd->box, sd->end);
                  evas_object_show(sd->end);

                  break;
               }
          }

        if (sd->view_state != TAGS_VIEW_SHRINK)
          {
             sd->view_state = TAGS_VIEW_SHRINK;
             efl_event_callback_call
               (obj, EFL_UI_TAGS_EVENT_EXPAND_STATE_CHANGED, &shrink);
          }

        eina_value_flush(&val);
     }
   else
     {
        // unpack all items and entry
        efl_pack_unpack_all(sd->box);
        EINA_LIST_FOREACH(sd->layouts, l, layout)
          {
             evas_object_hide(layout);
          }
        evas_object_hide(sd->end);

        // pack buttons only 1line

        if (sd->label && sd->label_packed) efl_pack(sd->box, sd->label);

        // pack remain btns
        layout = NULL;
        EINA_LIST_FOREACH(sd->layouts, l, layout)
          {
             efl_pack(sd->box, layout);
             evas_object_show(layout);
          }

        if (sd->view_state == TAGS_VIEW_SHRINK)
          {
             sd->view_state = TAGS_VIEW_NONE;
             efl_event_callback_call
               (obj, EFL_UI_TAGS_EVENT_EXPAND_STATE_CHANGED, &shrink);
          }
     }

   if (sd->view_state == TAGS_VIEW_SHRINK)
     _efl_ui_tags_smart_focus_direction_enable = EINA_FALSE;
   else
     _efl_ui_tags_smart_focus_direction_enable = EINA_TRUE;
}

static void
_view_update(Efl_Ui_Tags_Data *sd)
{
   Eo *obj = sd->parent;

   if (sd->w_box <= 0) return;

   // update buttons in shrink mode
   if (sd->view_state == TAGS_VIEW_SHRINK)
     _shrink_mode_set(obj, EINA_TRUE);
}

static void
_current_item_state_change(Evas_Object *obj, Tags_It_State state)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(obj, EFL_UI_TAGS_CLASS);

   Eo *item = sd->selected_it;

   if (!item) return;

   switch (state)
     {
      case TAGS_IT_STATE_SELECTED:
        elm_layout_signal_emit(item, "efl,state,focused", "efl");
        efl_event_callback_call
          (obj, EFL_UI_TAGS_EVENT_ITEM_SELECTED, (void *)elm_object_part_text_get(item, "efl.btn.text"));
        break;
      case TAGS_IT_STATE_DEFAULT:
      default:
        elm_layout_signal_emit(item, "efl,state,default", "efl");
        sd->selected_it = NULL;
        break;
     }
}

static void
_current_item_change(Evas_Object *obj, Evas_Object *it)
{
   Eina_List *l;
   Eo *layout;

   Efl_Ui_Tags_Data *sd = efl_data_scope_get(obj, EFL_UI_TAGS_CLASS);

   // change the state of previous item to "default"
   _current_item_state_change(obj, TAGS_IT_STATE_DEFAULT);

   // change the current
   EINA_LIST_FOREACH(sd->layouts, l, layout)
     {
        if (layout == it)
          {
             sd->selected_it = it;
             break;
          }
     }
   // change the state of current item to "focused"
   _current_item_state_change(obj, TAGS_IT_STATE_SELECTED);
}

static void
_item_select(Evas_Object *obj, Evas_Object *it)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(obj, EFL_UI_TAGS_CLASS);

   if (it)
     {
        _current_item_change(obj, it);

        if (efl_ui_focus_object_focus_get(obj))
          {
             elm_object_focus_set(sd->entry, EINA_FALSE);
             elm_object_focus_set(it, EINA_TRUE);
          }
     }
   else
     {
        _current_item_state_change
           (obj, TAGS_IT_STATE_DEFAULT);
        if (efl_ui_focus_object_focus_get(obj) && sd->editable)
          elm_object_focus_set(sd->entry, EINA_TRUE);
     }
}

static Eina_Bool
_long_press_cb(void *data)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   sd->longpress_timer = NULL;

   efl_event_callback_call
      (data, EFL_UI_TAGS_EVENT_ITEM_LONGPRESSED,
	   (void *)elm_object_part_text_get(sd->downed_it, "efl.btn.text"));

   return ECORE_CALLBACK_CANCEL;
}

static void
_mouse_down_cb(void  *data,
               Evas *evas EINA_UNUSED,
               Evas_Object *obj,
               void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;

   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   if (ev->button != 1) return;

   ecore_timer_del(sd->longpress_timer);
   sd->downed_it = obj;
   sd->longpress_timer = ecore_timer_add
      (_elm_config->longpress_timeout, _long_press_cb, data);
}

static void
_mouse_up_cb(void *data,
             Evas *evas EINA_UNUSED,
             Evas_Object *obj EINA_UNUSED,
             void *event_info EINA_UNUSED)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   sd->downed_it = NULL;
   ELM_SAFE_FREE(sd->longpress_timer, ecore_timer_del);
}


static void
_on_item_focused(void *data,
                 Evas_Object *obj,
                 void *event_info EINA_UNUSED)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   sd->focused_it = obj;
}

static void
_on_item_unfocused(void *data,
                   Evas_Object *obj EINA_UNUSED,
                   void *event_info EINA_UNUSED)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   sd->focused_it = NULL;
}

static void
_on_item_deleted(void *data,
                 Evas_Object *obj,
                 const char *emission EINA_UNUSED,
                 const char *source EINA_UNUSED)
{
   Eina_List *l;
   Eo *item;

   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   // change the current
   EINA_LIST_FOREACH(sd->layouts, l, item)
     {
        if (item == obj)
          {
             sd->layouts = eina_list_remove(sd->layouts, item);
             efl_pack_unpack(sd->box, item);

             if (sd->selected_it == item)
               sd->selected_it = NULL;

             if (sd->focused_it == item)
               sd->focused_it = NULL;

             if (sd->view_state == TAGS_VIEW_SHRINK)
               _shrink_mode_set(data, EINA_TRUE);

             if (!sd->item_setting)
               efl_event_callback_call
                  (data, EFL_UI_TAGS_EVENT_ITEM_DELETED,
                   (void *)elm_object_part_text_get(obj, "efl.btn.text"));

             efl_del(item);

             break;
          }
     }
}

static void
_on_item_clicked(void *data,
                 Evas_Object *obj,
                 const char *emission EINA_UNUSED,
                 const char *source EINA_UNUSED)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);
   _item_select(data, obj);

   if (sd->selected_it)
     efl_event_callback_call
        (data, EFL_UI_TAGS_EVENT_ITEM_CLICKED, (void *)elm_object_part_text_get(obj, "efl.btn.text"));
}

static Eo *
_item_new(Efl_Ui_Tags_Data *sd,
                 const char *str)
{
   Evas_Object *obj;
   Eina_Size2D min;
   Eina_Rect r;
   Eo *layout;

   obj = sd->parent;

   if (!str) return NULL;

   layout = efl_add(EFL_UI_LAYOUT_CLASS, obj);

   if (elm_widget_element_update(obj, layout, PART_NAME_BUTTON) == EFL_UI_THEME_APPLY_ERROR_GENERIC)
     CRI("Failed to set layout!");

   efl_text_set(efl_part(layout, "efl.btn.text"), str);

   //entry is cleared when text is made to button
   efl_text_set(sd->entry, "");

   elm_layout_signal_callback_add
     (layout, "mouse,clicked,1", "*", _on_item_clicked, obj);
   elm_layout_signal_callback_add
     (layout, "efl,deleted", "efl", _on_item_deleted, obj);
   evas_object_smart_callback_add
     (layout, "focused", _on_item_focused, obj);
   evas_object_smart_callback_add
     (layout, "unfocused", _on_item_unfocused, obj);
   evas_object_event_callback_add
      (layout,
       EVAS_CALLBACK_MOUSE_DOWN, _mouse_down_cb, obj);
   evas_object_event_callback_add
      (layout,
       EVAS_CALLBACK_MOUSE_UP, _mouse_up_cb, obj);

   efl_gfx_hint_align_set(layout, 0.5, 0.5);
   efl_gfx_hint_weight_set(layout, 0.0, 0.0);

   evas_object_show(layout);

   evas_object_smart_calculate(layout);
   min = efl_gfx_hint_size_combined_min_get(layout);
   r = efl_gfx_entity_geometry_get(sd->box);

   if (sd->w_box && min.w > r.w)
     {
        elm_coords_finger_size_adjust(1, &r.w, 1, &min.h);
        efl_gfx_hint_size_min_set(layout, EINA_SIZE2D(r.w, min.h));
        efl_gfx_entity_size_set(layout, EINA_SIZE2D(r.w, min.h));
     }

   elm_object_focus_allow_set(layout, EINA_TRUE);

   sd->layouts = eina_list_append(sd->layouts, layout);
   if (sd->view_state == TAGS_VIEW_SHRINK)
     {
        elm_widget_sub_object_add(obj, layout);
        evas_object_hide(layout);
     }
   else
     {
        if (sd->editable)
          efl_pack_before(sd->box, layout, sd->entry);
        else
          efl_pack(sd->box, layout);
     }

   if (!efl_ui_focus_object_focus_get(obj) && sd->view_state == TAGS_VIEW_SHRINK && sd->w_box)
     _shrink_mode_set(obj, EINA_TRUE);

   if (!sd->item_setting)
     efl_event_callback_call
       (obj, EFL_UI_TAGS_EVENT_ITEM_ADDED, (void *)elm_object_part_text_get(layout, "efl.btn.text"));

   return layout;
}

//FIXME: having an empty event handling function and reacting on Evas
//events on specific objects is crazy, someone should fix that.
EOLIAN static Eina_Bool
_efl_ui_tags_efl_ui_widget_widget_input_event_handler(Eo *obj EINA_UNUSED, Efl_Ui_Tags_Data *sd EINA_UNUSED, const Efl_Event *eo_event EINA_UNUSED, Evas_Object *src EINA_UNUSED)
{
   //lets stop eating all events
   return EINA_FALSE;
}

static void
_mouse_clicked_signal_cb(void *data EINA_UNUSED,
                         Evas_Object *obj,
                         const char *emission EINA_UNUSED,
                         const char *source EINA_UNUSED)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(obj, EFL_UI_TAGS_CLASS);

   if (sd->editable) efl_input_text_input_panel_show(sd->entry);

   efl_event_callback_call(obj, EFL_INPUT_EVENT_CLICKED, NULL);
}

static void
_box_resize_cb(void *data,
               Evas *evas EINA_UNUSED,
               Evas_Object *obj,
               void *event EINA_UNUSED)
{
   Eina_Size2D min;
   Eina_Rect r;
   Eina_List *l;
   Eo *layout;
   unsigned int hpad;

   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   r = efl_gfx_entity_geometry_get(sd->box);
   if ((r.w <= elm_config_finger_size_get()) || (r.h <= elm_config_finger_size_get())) return;

   efl_gfx_arrangement_content_padding_get(obj, &hpad, NULL);

   if (sd->h_box < r.h)
     efl_event_callback_call
       (sd->parent, EFL_UI_TAGS_EVENT_EXPANDED, NULL);
   else if (sd->h_box > r.h)
     efl_event_callback_call
       (sd->parent, EFL_UI_TAGS_EVENT_CONTRACTED, NULL);

   if (sd->layouts && sd->w_box != r.w)
     {
        EINA_LIST_FOREACH (sd->layouts, l, layout)
          {
             efl_canvas_group_calculate(layout);

             min = efl_gfx_hint_size_combined_min_get(layout);

             if (min.w > r.w - (int)hpad)
               {
                  min.w = r.w - (int)hpad;
                  efl_gfx_hint_size_min_set(layout, EINA_SIZE2D(min.w, min.h));
                  efl_gfx_entity_size_set(layout, EINA_SIZE2D(min.w, min.h));
               }
          }
     }

   sd->w_box = r.w;
   sd->h_box = r.h;

   if (sd->view_state == TAGS_VIEW_SHRINK)
     _shrink_mode_set(data, EINA_TRUE);
}

static void
_entry_resize_cb(void *data,
                 Evas *e EINA_UNUSED,
                 Evas_Object *obj EINA_UNUSED,
                 void *event_info EINA_UNUSED)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   if (efl_ui_focus_object_focus_get(sd->parent))
     elm_widget_show_region_set(sd->entry, efl_gfx_entity_geometry_get(sd->entry), EINA_TRUE);
}

static void
_entry_changed_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);
   const char *str;

   str = efl_text_get(sd->entry);
   sd->n_str = str ? strlen(str) : 0;
}

static void
_entry_focus_changed_cb(void *data, const Efl_Event *event)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   if (efl_ui_focus_object_focus_get(event->object))
     {
        Eo *item;

        if (sd->selected_it)
          {
             item = sd->selected_it;
             elm_object_focus_set(sd->entry, EINA_FALSE);
             elm_object_focus_set(item, EINA_TRUE);
          }
     }
   else
     {
        const char *str;

        str = efl_text_get(sd->entry);
        if (str && str[0])
          _item_new(sd, str);
     }
}

static void
_entry_clicked_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   _current_item_state_change(sd->parent, TAGS_IT_STATE_DEFAULT);
   elm_object_focus_set(sd->entry, EINA_TRUE);
}

static void
_layout_key_down_cb(void *data,
                  Evas *e EINA_UNUSED,
                  Evas_Object *obj EINA_UNUSED,
                  void *event_info)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);
   Evas_Event_Key_Up *ev = (Evas_Event_Key_Up *)event_info;

   if (!sd->box) return;

   if (sd->last_it_select)
     {
        if (sd->selected_it &&
            ((!strcmp(ev->key, "BackSpace")) ||
             (!strcmp(ev->key, "Delete"))))
          {
             Eo *item = sd->selected_it;
             if (item && sd->editable)
               {
                  _on_item_deleted(data, item, NULL, NULL);
                  elm_object_focus_set(sd->entry, EINA_TRUE);
               }
          }
        else if (sd->focused_it &&
            ((!strcmp(ev->key, "KP_Enter")) ||
             (!strcmp(ev->key, "Return"))))
          {
             Eo *item = sd->focused_it;
             if (item)
               _on_item_clicked(item, NULL, NULL, NULL);
          }
        else if (((!sd->selected_it && (sd->n_str == 0) &&
                   (!strcmp(ev->key, "BackSpace"))) ||
                  (!strcmp(ev->key, "Delete"))))
          {
             Eo *item = eina_list_data_get(eina_list_last(sd->layouts));
             if (item)
               {
                  _item_select(sd->parent, item);
               }
          }
     }
   else
     sd->last_it_select = EINA_TRUE;
}

static void
_entry_key_down_cb(void *data,
                   Evas *e EINA_UNUSED,
                   Evas_Object *obj EINA_UNUSED,
                   void *event_info)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);
   Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event_info;

   if (sd->n_str == 1 &&
       (!strcmp(ev->key, "BackSpace") || !strcmp(ev->key, "Delete")))
     sd->last_it_select = EINA_FALSE;
}

static void
_entry_key_up_cb(void *data,
                 Evas *e EINA_UNUSED,
                 Evas_Object *obj EINA_UNUSED,
                 void *event_info)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);
   Evas_Event_Key_Up *ev = (Evas_Event_Key_Up *)event_info;
   const char *str;

   if (!sd->box) return;

   str = efl_text_get(sd->entry);
   if (!str) return;

   if (strlen(str) &&
       (!strcmp(ev->key, "KP_Enter") || !strcmp(ev->key, "Return")))
     {
        _item_new(sd, str);
        sd->n_str = 0;
     }
}

static void
_callbacks_register(Evas_Object *obj)
{
   Efl_Ui_Tags_Data *sd = efl_data_scope_get(obj, EFL_UI_TAGS_CLASS);
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   elm_layout_signal_callback_add
     (obj, "mouse,clicked,1", "*", _mouse_clicked_signal_cb, NULL);

   evas_object_event_callback_add
     (wd->resize_obj, EVAS_CALLBACK_KEY_DOWN,
     _layout_key_down_cb, obj);

   evas_object_event_callback_add
     (sd->box, EVAS_CALLBACK_RESIZE, _box_resize_cb, obj);

   evas_object_event_callback_add
     (sd->entry, EVAS_CALLBACK_KEY_UP, _entry_key_up_cb, obj);
   evas_object_event_callback_add
     (sd->entry, EVAS_CALLBACK_KEY_DOWN, _entry_key_down_cb, obj);
   evas_object_event_callback_add
     (sd->entry, EVAS_CALLBACK_RESIZE, _entry_resize_cb, obj);
   efl_event_callback_array_add(sd->entry, _tags_cb(), obj);
}

static void
_label_set(Evas_Object *obj,
           const char *str)
{
   Evas_Coord width, height;

   Efl_Ui_Tags_Data *sd = efl_data_scope_get(obj, EFL_UI_TAGS_CLASS);

   if (!str) return;

   eina_stringshare_replace(&sd->label_str, str);

   edje_object_part_text_escaped_set(sd->label, "efl.text", str);

   if (!strlen(str))
     {
        sd->label_packed = EINA_FALSE;
        efl_pack_unpack(sd->box, sd->label);
        evas_object_hide(sd->label);
     }
   else
     {
        if (sd->label_packed)
          efl_pack_unpack(sd->box, sd->label);
        sd->label_packed = EINA_TRUE;
        edje_object_size_min_calc(sd->label, &width, &height);
        efl_gfx_hint_size_min_set(sd->label, EINA_SIZE2D(width, height));
        efl_pack_begin(sd->box, sd->label);
        evas_object_show(sd->label);
     }

   _view_update(sd);
}

static void
_view_init(Evas_Object *obj, Efl_Ui_Tags_Data *sd)
{
   const char *str;
   int hpad = 0, vpad = 0;

   //FIXME: efl_ui_box doesn't support box_layout customizing.
   //       So i use legacy box here.
   sd->box = efl_add(EFL_UI_BOX_FLOW_CLASS, obj,
     efl_ui_layout_orientation_set(efl_added, EFL_UI_LAYOUT_ORIENTATION_HORIZONTAL),
     efl_gfx_arrangement_content_align_set(efl_added, 0, 0),
     efl_gfx_hint_align_set(efl_added, 0, 0));

   if (!sd->box) return;

   str = elm_layout_data_get(obj, "horizontal_pad");
   if (str) hpad = atoi(str);
   str = elm_layout_data_get(obj, "vertical_pad");
   if (str) vpad = atoi(str);
   efl_gfx_arrangement_content_padding_set(sd->box, hpad, vpad);

   efl_content_set(efl_part(obj, "efl.box"), sd->box);

   sd->label = edje_object_add(evas_object_evas_get(obj));
   if (!sd->label) return;
   elm_widget_element_update(obj, sd->label, PART_NAME_LABEL);

   sd->entry = efl_add(EFL_UI_TEXTBOX_CLASS, sd->box,
                       efl_text_multiline_set(efl_added, EINA_FALSE),
                       efl_text_set(efl_added, ""),
                       efl_ui_textbox_cnp_dnd_mode_set(efl_added, EFL_UI_TEXTBOX_CNP_CONTENT_MARKUP),
                       efl_input_text_input_panel_autoshow_set(efl_added, EINA_FALSE),
                       efl_text_interactive_editable_set(efl_added, EINA_TRUE),
                       efl_composite_attach(obj, efl_added));

   efl_gfx_hint_size_min_set(sd->entry, EINA_SIZE2D(MIN_W_ENTRY, 0));
   efl_gfx_hint_weight_set(sd->entry, EFL_GFX_HINT_EXPAND, 0);
   efl_gfx_hint_fill_set(sd->entry, EINA_TRUE, EINA_TRUE);

   efl_pack(sd->box, sd->entry);

   sd->view_state = TAGS_VIEW_ENTRY;

     {
        Evas_Coord button_min_width = 0, button_min_height = 0;

        sd->end = edje_object_add(evas_object_evas_get(obj));
        if (!sd->end) return;
        elm_widget_element_update(obj, sd->end, PART_NAME_NUMBER);

        edje_object_size_min_calc(sd->end, &button_min_width, &button_min_height);
        elm_coords_finger_size_adjust(1, &button_min_width, 1, &button_min_height);
        efl_gfx_hint_size_min_set(sd->end, EINA_SIZE2D(button_min_width, button_min_height));
        elm_widget_sub_object_add(obj, sd->end);
     }
}

static void
_legacy_focused(void *data, const Efl_Event *ev)
{
   Efl_Ui_Focus_Object *new_focus;
   Eina_Bool meaningful_focus_in = EINA_FALSE, meaningful_focus_out = EINA_FALSE;
   Efl_Ui_Tags_Data *pd = efl_data_scope_get(data, EFL_UI_TAGS_CLASS);

   new_focus = efl_ui_focus_manager_focus_get(ev->object);

   if (efl_isa(ev->info, EFL_UI_WIDGET_CLASS) && elm_widget_parent_get(ev->info) == pd->box)
     {
        meaningful_focus_out = EINA_TRUE;
     }

   if (efl_isa(new_focus, EFL_UI_WIDGET_CLASS) && elm_widget_parent_get(new_focus) == pd->box)
     {
        meaningful_focus_in = EINA_TRUE;
     }

   if (meaningful_focus_in && !meaningful_focus_out)
     {
        evas_object_smart_callback_call(data, "focused", NULL);
     }

   if (!meaningful_focus_in && meaningful_focus_out)
     {
        evas_object_smart_callback_call(data, "unfocused", NULL);
     }
}

static void
_legacy_manager_changed_cb(void *data EINA_UNUSED, const Efl_Event *ev)
{
   efl_event_callback_del(ev->info, EFL_UI_FOCUS_MANAGER_EVENT_MANAGER_FOCUS_CHANGED, _legacy_focused, ev->object);
   efl_event_callback_add(efl_ui_focus_object_focus_manager_get(ev->object), EFL_UI_FOCUS_MANAGER_EVENT_MANAGER_FOCUS_CHANGED, _legacy_focused, ev->object);
}

EOLIAN static Eo *
_efl_ui_tags_efl_object_constructor(Eo *obj, Efl_Ui_Tags_Data *sd)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd, NULL);

   if (!elm_widget_theme_klass_get(obj))
     elm_widget_theme_klass_set(obj, "tags");

   obj = efl_constructor(efl_super(obj, MY_CLASS));

   if (elm_widget_theme_object_set(obj, wd->resize_obj,
                                       elm_widget_theme_klass_get(obj),
                                       elm_widget_theme_element_get(obj),
                                       elm_widget_theme_style_get(obj)) == EFL_UI_THEME_APPLY_ERROR_GENERIC)
     CRI("Failed to set layout!");

   elm_widget_can_focus_set(obj, EINA_FALSE);

   sd->last_it_select = EINA_TRUE;
   sd->editable = EINA_TRUE;
   sd->parent = obj;
   sd->it_array = eina_array_new(4);

   _view_init(obj, sd);
   _callbacks_register(obj);

   //listen to manager changes here
   efl_event_callback_add(obj, EFL_UI_FOCUS_OBJECT_EVENT_FOCUS_MANAGER_CHANGED, _legacy_manager_changed_cb, NULL);

   return obj;
}

EOLIAN static void
_efl_ui_tags_efl_object_destructor(Eo *obj, Efl_Ui_Tags_Data *sd)
{
   Eina_List *l;
   Eo *layout;

   EINA_LIST_FOREACH(sd->layouts, l, layout)
     evas_object_del(layout);

   sd->layouts = eina_list_free(sd->layouts);
   eina_array_free(sd->it_array);

   sd->selected_it = NULL;
   sd->focused_it = NULL;

   eina_stringshare_del(sd->label_str);
   evas_object_del(sd->entry);
   evas_object_del(sd->label);
   evas_object_del(sd->end);
   ecore_timer_del(sd->longpress_timer);

   efl_destructor(efl_super(obj, MY_CLASS));
}

EOLIAN static void
_efl_ui_tags_efl_text_text_set(Eo *obj, Efl_Ui_Tags_Data *sd EINA_UNUSED, const char *label)
{
   if (label) _label_set(obj, label);
}

EOLIAN static const char *
_efl_ui_tags_efl_text_text_get(const Eo *obj EINA_UNUSED, Efl_Ui_Tags_Data *sd)
{
   return (sd->label_str ? sd->label_str : NULL);
}

EOLIAN static Eina_Bool
_efl_ui_tags_expanded_get(const Eo *obj EINA_UNUSED, Efl_Ui_Tags_Data *sd)
{
   return sd->view_state == TAGS_VIEW_SHRINK ?
          EINA_FALSE : EINA_TRUE;
}

EOLIAN static void
_efl_ui_tags_expanded_set(Eo *obj, Efl_Ui_Tags_Data *sd, Eina_Bool expanded)
{
   if (((sd->view_state == TAGS_VIEW_SHRINK) ?
        EINA_FALSE : EINA_TRUE) == expanded) return;

   if (expanded)
     _shrink_mode_set(obj, EINA_FALSE);
   else
     _shrink_mode_set(obj, EINA_TRUE);
}

EOLIAN static void
_efl_ui_tags_editable_set(Eo *obj EINA_UNUSED, Efl_Ui_Tags_Data *sd, Eina_Bool editable)
{
   editable = !!editable;
   if (sd->editable == editable) return;
   sd->editable = editable;

   if (sd->editable && (sd->view_state != TAGS_VIEW_SHRINK))
     {
        efl_pack(sd->box, sd->entry);
        evas_object_show(sd->entry);
     }
   else
     {
        efl_pack_unpack(sd->box, sd->entry);
        evas_object_hide(sd->entry);
     }
}

EOLIAN static Eina_Bool
_efl_ui_tags_editable_get(const Eo *obj EINA_UNUSED, Efl_Ui_Tags_Data *sd)
{
   return sd->editable;
}

EOLIAN static void
_efl_ui_tags_items_set(Eo *obj, Efl_Ui_Tags_Data *sd, const Eina_Array *items)
{
   EINA_SAFETY_ON_NULL_RETURN(items);

   Eina_List *l;
   Eo *layout;
   Eina_Array_Iterator iterator;
   const char *it_text;
   unsigned int i;

   sd->item_setting = EINA_TRUE;

   if (sd->layouts)
     EINA_LIST_FOREACH(sd->layouts, l, layout)
       _on_item_deleted(obj, layout, NULL, NULL);

   if (eina_array_count(items))
     EINA_ARRAY_ITER_NEXT(items, i, it_text, iterator)
       _item_new(sd, it_text);

   sd->item_setting = EINA_FALSE;
}

EOLIAN static const Eina_Array*
_efl_ui_tags_items_get(const Eo *obj EINA_UNUSED, Efl_Ui_Tags_Data *sd)
{
   Eina_List *l;
   Eo *layout;

   eina_array_clean(sd->it_array);

   if (sd->layouts)
     EINA_LIST_FOREACH(sd->layouts, l, layout)
       eina_array_push(sd->it_array, elm_object_part_text_get(layout, "efl.btn.text"));

   return sd->it_array;
}
EOLIAN static void
_efl_ui_tags_efl_ui_format_apply_formatted_value(Eo *obj EINA_UNUSED, Efl_Ui_Tags_Data *pd)
{
   _view_update(pd);
}

#include "efl_ui_tags.eo.c"
