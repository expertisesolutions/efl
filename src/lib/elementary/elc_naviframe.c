#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_ACCESS_WIDGET_ACTION_PROTECTED
#define ELM_WIDGET_ITEM_PROTECTED
#define ELM_WIDGET_PROTECTED
#define EFL_UI_L10N_PROTECTED
#define EFL_PART_PROTECTED

#include <Elementary.h>
#include "elm_priv.h"
#include "elm_naviframe_eo.h"
#include "elm_naviframe_item_eo.h"
#include "elm_widget_naviframe.h"
#include "elm_widget_container.h"

#include "elm_naviframe_part.eo.h"
#include "elm_part_helper.h"

#define MY_CLASS ELM_NAVIFRAME_CLASS

#define MY_CLASS_NAME "Elm_Naviframe"
#define MY_CLASS_NAME_LEGACY "elm_naviframe"

static const char CONTENT_PART[] = "elm.swallow.content";
static const char PREV_BTN_PART[] = "elm.swallow.prev_btn";
static const char NEXT_BTN_PART[] = "elm.swallow.next_btn";
static const char ICON_PART[] = "elm.swallow.icon";
static const char TITLE_PART[] = "elm.text.title";
static const char SUBTITLE_PART[] = "elm.text.subtitle";
static const char TITLE_ACCESS_PART[] = "access.title";

static const char SIG_TRANSITION_FINISHED[] = "transition,finished";
static const char SIG_TITLE_TRANSITION_FINISHED[] = "title,transition,finished";
static const char SIG_TITLE_CLICKED[] = "title,clicked";
static const char SIG_ITEM_ACTIVATED[] = "item,activated";

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {SIG_TRANSITION_FINISHED, ""},
   {SIG_TITLE_TRANSITION_FINISHED, ""},
   {SIG_TITLE_CLICKED, ""},
   {SIG_WIDGET_LANG_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_WIDGET_ACCESS_CHANGED, ""}, /**< handled by elm_widget */
   {SIG_LAYOUT_FOCUSED, ""}, /**< handled by elm_layout */
   {SIG_LAYOUT_UNFOCUSED, ""}, /**< handled by elm_layout */
   {SIG_ITEM_ACTIVATED, ""},
   {NULL, NULL}
};

static void _on_item_back_btn_clicked(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);

static Eina_Bool _key_action_top_item_get(Evas_Object *obj, const char *params);
static Eina_Bool _key_action_item_pop(Evas_Object *obj, const char *params);

static const Elm_Action key_actions[] = {
   {"top_item_get", _key_action_top_item_get},
   {"item_pop", _key_action_item_pop},
   {NULL, NULL}
};

EOLIAN static void
_elm_naviframe_efl_ui_widget_resize_object_set(Eo *obj, Elm_Naviframe_Data *pd EINA_UNUSED,
                                            Efl_Canvas_Object *sobj)
{
   if (!sobj || !efl_finalized_get(obj)) goto super;
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   if (wd->resize_obj == sobj) return;

   // Detach the previous page as resize object to avoid any modifications to
   // it by the widget implementation. This preserves clip, smart membership,
   // focus, etc...
   wd->resize_obj = NULL;

super:
   efl_ui_widget_resize_object_set(efl_super(obj, MY_CLASS), sobj);
}

static void
_resize_object_reset(Evas_Object *obj, Elm_Naviframe_Item_Data *it)
{
   if (it)
     {
        elm_widget_resize_object_set(obj, VIEW(it));
        evas_object_raise(VIEW(it));
     }
}

static void
_prev_page_focus_recover(Elm_Naviframe_Item_Data *it)
{
   Elm_Naviframe *n = it->base->widget;

   efl_ui_focus_manager_focus_set(efl_ui_focus_object_focus_manager_get(n), n);
}

EOLIAN static void
_elm_naviframe_efl_ui_l10n_translation_update(Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd)
{
   Elm_Naviframe_Item_Data *it;

   EINA_INLIST_FOREACH(sd->stack, it)
     elm_wdg_item_translate(EO_OBJ(it));

   efl_ui_l10n_translation_update(efl_super(obj, MY_CLASS));
}

static void
_item_content_del_cb(void *data,
                     Evas *e EINA_UNUSED,
                     Evas_Object *obj EINA_UNUSED,
                     void *event_info EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it = data;

   it->content = NULL;
   elm_object_signal_emit(VIEW(it), "elm,state,content,hide", "elm");
}

static void
_item_title_prev_btn_del_cb(void *data,
                            Evas *e EINA_UNUSED,
                            Evas_Object *obj EINA_UNUSED,
                            void *event_info EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it = data;

   it->title_prev_btn = NULL;
   if (it->auto_pushed_btn) it->auto_pushed_btn = NULL;
   elm_object_signal_emit(VIEW(it), "elm,state,prev_btn,hide", "elm");
}

static void
_item_title_next_btn_del_cb(void *data,
                            Evas *e EINA_UNUSED,
                            Evas_Object *obj EINA_UNUSED,
                            void *event_info EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it = data;

   it->title_next_btn = NULL;
   elm_object_signal_emit(VIEW(it), "elm,state,next_btn,hide", "elm");
}

static void
_item_title_icon_del_cb(void *data,
                        Evas *e EINA_UNUSED,
                        Evas_Object *obj EINA_UNUSED,
                        void *event_info EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it = data;

   it->title_icon = NULL;
   elm_object_signal_emit(VIEW(it), "elm,state,icon,hide", "elm");
}

static void
_title_content_del(void *data,
                   Evas *e EINA_UNUSED,
                   Evas_Object *obj EINA_UNUSED,
                   void *event_info EINA_UNUSED)
{
   char buf[1024];
   Elm_Naviframe_Content_Item_Pair *pair = data;
   Elm_Naviframe_Item_Data *it = pair->it;
   snprintf(buf, sizeof(buf), "elm,state,%s,hide", pair->part);
   elm_object_signal_emit(VIEW(it), buf, "elm");
   it->content_list = eina_inlist_remove(it->content_list,
                                         EINA_INLIST_GET(pair));
   eina_stringshare_del(pair->part);
   free(pair);
}

static void
_item_free(Elm_Naviframe_Item_Data *it)
{
   Eina_Inlist *l;
   Elm_Naviframe_Text_Item_Pair *text_pair;

   eina_stringshare_del(it->title_label);
   eina_stringshare_del(it->subtitle_label);


   EINA_INLIST_FOREACH_SAFE(it->text_list, l, text_pair)
     {
        eina_stringshare_del(text_pair->part);
        free(text_pair);
     }
}

static void
_item_content_signals_emit(Elm_Naviframe_Item_Data *it)
{
   Elm_Naviframe_Content_Item_Pair *content_pair;
   char buf[1024];
   //content
   if (it->content)
     elm_object_signal_emit(VIEW(it), "elm,state,content,show", "elm");
   else
     elm_object_signal_emit(VIEW(it), "elm,state,content,hide", "elm");

   //prev button
   if (it->title_prev_btn)
     elm_object_signal_emit(VIEW(it), "elm,state,prev_btn,show", "elm");
   else
     elm_object_signal_emit(VIEW(it), "elm,state,prev_btn,hide", "elm");

   //next button
   if (it->title_next_btn)
     elm_object_signal_emit(VIEW(it), "elm,state,next_btn,show", "elm");
   else
     elm_object_signal_emit(VIEW(it), "elm,state,next_btn,hide", "elm");

   if (it->title_icon)
     elm_object_signal_emit(VIEW(it), "elm,state,icon,show", "elm");
   else
     elm_object_signal_emit(VIEW(it), "elm,state,icon,hide", "elm");

   EINA_INLIST_FOREACH(it->content_list, content_pair)
     {
        if (content_pair->content)
          snprintf(buf, sizeof(buf), "elm,state,%s,show", content_pair->part);
        else
          snprintf(buf, sizeof(buf), "elm,state,%s,hide", content_pair->part);
        elm_object_signal_emit(VIEW(it), buf, "elm");
     }
}

static void
_item_text_signals_emit(Elm_Naviframe_Item_Data *it)
{
   Elm_Naviframe_Text_Item_Pair *text_pair;
   char buf[1024];

   if ((it->title_label) && (it->title_label[0]))
     elm_object_signal_emit(VIEW(it), "elm,state,title_label,show", "elm");
   else
     elm_object_signal_emit(VIEW(it), "elm,state,title_label,hide", "elm");

   if ((it->subtitle_label) && (it->subtitle_label[0]))
     elm_object_signal_emit(VIEW(it), "elm,state,subtitle,show", "elm");
   else
     elm_object_signal_emit(VIEW(it), "elm,state,subtitle,hide", "elm");

   EINA_INLIST_FOREACH(it->text_list, text_pair)
     {
        if (elm_object_part_text_get(VIEW(it), text_pair->part))
          snprintf(buf, sizeof(buf), "elm,state,%s,show", text_pair->part);
        else
          snprintf(buf, sizeof(buf), "elm,state,%s,hide", text_pair->part);
        elm_object_signal_emit(VIEW(it), buf, "elm");
     }
}

static Evas_Object *
_access_object_get(Elm_Naviframe_Item_Data *it, const char* part)
{
   Evas_Object *po, *ao, *o;

   o = elm_layout_edje_get(VIEW(it));
   edje_object_freeze(o);
   po = (Evas_Object *)edje_object_part_object_get(o, part);
   edje_object_thaw(o);
   ao = evas_object_data_get(po, "_part_access_obj");

   return ao;
}

static Eina_Bool
_access_info_has(Evas_Object *obj, int type)
{
   Elm_Access_Info *ac;
   Elm_Access_Item *ai;
   Eina_List *l;

   ac = _elm_access_info_get(obj);
   if (!ac) return EINA_FALSE;

   EINA_LIST_FOREACH(ac->items, l, ai)
     {
        if (ai->type == type)
          {
             if (ai->func || ai->data) return EINA_TRUE;
          }
     }

   return EINA_FALSE;
}

static void
_item_signals_emit(Elm_Naviframe_Item_Data *it)
{
   _item_text_signals_emit(it);
   _item_content_signals_emit(it);
}

/* FIXME: we need to handle the case when this function is called
 * during a transition */
static void
_item_style_set(Elm_Naviframe_Item_Data *it,
                const char *item_style)
{
   char buf[256];

   ELM_NAVIFRAME_DATA_GET(WIDGET(it), sd);

   if (!item_style)
     {
        strcpy(buf, "item/basic");
        elm_wdg_item_style_set(efl_super(EO_OBJ(it), ELM_NAVIFRAME_ITEM_CLASS), "basic");
     }
   else
     {
        snprintf(buf, sizeof(buf), "item/%s", item_style);
        elm_wdg_item_style_set(efl_super(EO_OBJ(it), ELM_NAVIFRAME_ITEM_CLASS),  item_style);
     }

   if (!elm_layout_theme_set(VIEW(it), "naviframe", buf,
                             elm_widget_style_get(WIDGET(it))))
     {
        /* Naviframe item theme name is combined with item style name and widget
         * style name.
         * Therefore, if the item theme name is not found, then set "item/basic"
         * item style name as a default item style name. */
        if (!elm_layout_theme_set(VIEW(it), "naviframe", "item/basic",
                                  elm_widget_style_get(WIDGET(it))))
          CRI("Failed to set layout!");
     }

   if (sd->freeze_events)
     evas_object_freeze_events_set(VIEW(it), EINA_FALSE);
}

static void
_on_item_title_transition_finished(void *data,
                                   Evas_Object *obj EINA_UNUSED,
                                   const char *emission EINA_UNUSED,
                                   const char *source EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it = data;

   efl_event_callback_legacy_call
         (WIDGET(it), ELM_NAVIFRAME_EVENT_TITLE_TRANSITION_FINISHED, EO_OBJ(it));
}

static void
_item_title_enabled_update(Elm_Naviframe_Item_Data *nit, Eina_Bool transition)
{
   transition = !!transition;
   if (transition)
     {
        if (nit->title_enabled)
          elm_object_signal_emit(VIEW(nit), "elm,action,title,show", "elm");
        else
          elm_object_signal_emit(VIEW(nit), "elm,action,title,hide", "elm");
     }
   else
     {
        if (nit->title_enabled)
          elm_object_signal_emit(VIEW(nit), "elm,state,title,show", "elm");
        else
          elm_object_signal_emit(VIEW(nit), "elm,state,title,hide", "elm");
     }
   edje_object_message_signal_process(elm_layout_edje_get(VIEW(nit)));
}

EOLIAN static Eina_Error
_elm_naviframe_efl_ui_widget_theme_apply(Eo *obj, Elm_Naviframe_Data *sd)
{
   Elm_Naviframe_Item_Data *it;
   const char *style = NULL, *sstyle = NULL;

   style = efl_ui_widget_style_get(obj);

   EINA_INLIST_FOREACH(sd->stack, it)
     {
        sstyle = efl_ui_widget_style_get(VIEW(it));
        if ((style && sstyle) && strcmp(style, sstyle))
          _item_style_set(it, elm_wdg_item_style_get(EO_OBJ(it)));
        _item_signals_emit(it);
        _item_title_enabled_update(it, EINA_FALSE);
     }

   elm_layout_sizing_eval(obj);
   return EFL_UI_THEME_APPLY_ERROR_NONE;
}

static char *
_access_info_cb(void *data, Evas_Object *obj EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *nit;
   Evas_Object *layout;
   Eina_Strbuf *buf;
   const char *info;
   char *ret;

   nit = data;
   if (!nit->title_enabled) return NULL;

   layout = VIEW(nit);
   info = elm_object_part_text_get(layout, TITLE_PART);
   if (!info) return NULL;

   buf = eina_strbuf_new();
   eina_strbuf_append(buf, info);
   eina_strbuf_append_printf(buf, ", %s", N_("Title"));

   info = elm_object_part_text_get(layout, SUBTITLE_PART);
   if (!info) goto end;
   if (!strcmp(info, "")) goto end;

   eina_strbuf_append_printf(buf, ", %s", info);
   eina_strbuf_append_printf(buf, ", %s", N_("Subtitle"));

end:
   ret = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);
   return ret;
}

static void
_access_obj_process(Elm_Naviframe_Item_Data *it, Eina_Bool is_access)
{
   Evas_Object *ao, *eo;

   if (is_access && (it->title_label || it->subtitle_label))
     {
        if (!_access_object_get(it, TITLE_ACCESS_PART))
          {
             eo = elm_layout_edje_get(VIEW(it));
             ao =_elm_access_edje_object_part_object_register(WIDGET(it), eo,
                                                            TITLE_ACCESS_PART);
            _elm_access_text_set(_elm_access_info_get(ao),
                                ELM_ACCESS_TYPE, E_("Title"));
            _elm_access_callback_set(_elm_access_info_get(ao),
                                     ELM_ACCESS_INFO, _access_info_cb, it);
            /* to access title access object, any idea? */
            it->base->access_obj = ao;
         }
     }
   else
     {
        /* to access title access object, any idea? */
        ao = it->base->access_obj;
        if (!ao) return;

        if (it->title_label || it->subtitle_label)
          _elm_access_edje_object_part_object_unregister
             (WIDGET(it), elm_layout_edje_get(VIEW(it)), TITLE_ACCESS_PART);
        evas_object_del(ao);
     }
}

static void
_elm_naviframe_item_elm_widget_item_part_text_set(Eo *eo_it,
                                                  Elm_Naviframe_Item_Data *it,
                                                  const char *part,
                                                  const char *label)
{
   Elm_Naviframe_Item_Data *nit = it;
   Elm_Naviframe_Text_Item_Pair *pair = NULL;
   char buf[1024];

   if ((!part) || (!strcmp(part, "default")) ||
       (!strcmp(part, TITLE_PART)))
     {
        eina_stringshare_replace(&nit->title_label, label);
        if (label)
          elm_object_signal_emit(VIEW(it), "elm,state,title_label,show", "elm");
        else
          elm_object_signal_emit(VIEW(it), "elm,state,title_label,hide", "elm");
        elm_object_part_text_set(VIEW(it), TITLE_PART, label);
     }
   else if ((!strcmp(part, "subtitle")) || (!strcmp(part, SUBTITLE_PART)))
     {
        eina_stringshare_replace(&nit->subtitle_label, label);
        if (label)
          elm_object_signal_emit(VIEW(it), "elm,state,subtitle,show", "elm");
        else
          elm_object_signal_emit(VIEW(it), "elm,state,subtitle,hide", "elm");
        elm_object_part_text_set(VIEW(it), SUBTITLE_PART, label);
     }
   else
     {
        EINA_INLIST_FOREACH(nit->text_list, pair)
          if (!strcmp(part, pair->part)) break;

        if (!pair)
          {
             pair = ELM_NEW(Elm_Naviframe_Text_Item_Pair);
             if (!pair)
               {
                  ERR("Failed to allocate new text part of the item! : naviframe=%p",
                  WIDGET(it));
                  return;
               }
             eina_stringshare_replace(&pair->part, part);
             nit->text_list = eina_inlist_append(nit->text_list,
                                                 EINA_INLIST_GET(pair));
          }
        if (label)
          snprintf(buf, sizeof(buf), "elm,state,%s,show", part);
        else
          snprintf(buf, sizeof(buf), "elm,state,%s,hide", part);
        elm_object_signal_emit(VIEW(it), buf, "elm");
        elm_object_part_text_set(VIEW(it), part, label);
     }

   /* access */
   if (_elm_config->access_mode)
     _access_obj_process(nit, EINA_TRUE);

   buf[0] = 0;
   if (nit->title_label)
     strncpy(buf, nit->title_label, sizeof(buf) - 1);
   if (nit->subtitle_label)
     {
        if ((nit->title_label) && (strlen(buf) < (sizeof(buf) - 2)))
          strcat(buf, " ");
        strncat(buf, nit->subtitle_label, sizeof(buf) - strlen(buf) - 2);
     }
   efl_access_object_i18n_name_set(eo_it, buf);

   elm_layout_sizing_eval(WIDGET(nit));
}

EOLIAN static const char *
_elm_naviframe_item_elm_widget_item_part_text_get(const Eo *nit EINA_UNUSED,
                                             Elm_Naviframe_Item_Data *it,
                                             const char *part)
{
   char buf[1024];

   if (!part || !strcmp(part, "default"))
     snprintf(buf, sizeof(buf), TITLE_PART);
   else if (!strcmp("subtitle", part))
     snprintf(buf, sizeof(buf), SUBTITLE_PART);
   else
     snprintf(buf, sizeof(buf), "%s", part);

   return elm_object_part_text_get(VIEW(it), buf);
}

EOLIAN static void
_elm_naviframe_item_efl_object_destructor(Eo *eo_item, Elm_Naviframe_Item_Data *it)
{
   Eina_List *l;
   Elm_Naviframe_Op *nfo;
   Elm_Naviframe_Item_Data *nit = it;

   ELM_NAVIFRAME_DATA_GET(WIDGET(nit), sd);

   nit->delete_me = EINA_TRUE;

   // This should not happen, but just in case and by security
   // make sure there is no more reference to this item.
   EINA_LIST_FOREACH(sd->ops, l, nfo)
     {
        /* If an item is newly pushed and then deleted by elm_object_item_del()
         * before item push transition is not started, then the item push
         * transitions for both new item and current item should be cancelled.
         * Otherwise, the current item becomes invisible due to the item push
         * transition. */
        if ((nit->pushing) && (nfo->self == nit))
          {
             nfo->self = NULL;
             nfo->related = NULL;
          }
        else
          {
             if (nfo->self == nit)
               nfo->self = NULL;
             if (nfo->related == nit)
               nfo->related = NULL;
          }
     }

   _item_free(nit);

   efl_destructor(efl_super(eo_item, ELM_NAVIFRAME_ITEM_CLASS));
}

static void
_item_content_set(Elm_Naviframe_Item_Data *it,
                  Evas_Object *content)
{
   if (it->content == content) return;

   evas_object_del(it->content);
   it->content = content;

   if (!content) return;

   elm_object_part_content_set(VIEW(it), CONTENT_PART, content);
   elm_object_signal_emit(VIEW(it), "elm,state,content,show", "elm");

   evas_object_event_callback_add
     (content, EVAS_CALLBACK_DEL, _item_content_del_cb, it);
}

static void
_item_title_prev_btn_set(Elm_Naviframe_Item_Data *it,
                         Evas_Object *btn)
{
   if (it->title_prev_btn == btn) return;
   evas_object_del(it->title_prev_btn);
   it->title_prev_btn = btn;
   if (it->auto_pushed_btn && (it->auto_pushed_btn != btn))
     it->auto_pushed_btn = NULL;
   if (!btn) return;

   elm_object_part_content_set(VIEW(it), PREV_BTN_PART, btn);
   elm_object_signal_emit(VIEW(it), "elm,state,prev_btn,show", "elm");
   evas_object_event_callback_add
     (btn, EVAS_CALLBACK_DEL, _item_title_prev_btn_del_cb, it);

   //FIXME: set back button callback here after elm 2.0
}

static void
_item_title_next_btn_set(Elm_Naviframe_Item_Data *it,
                         Evas_Object *btn)
{
   if (it->title_next_btn == btn) return;
   evas_object_del(it->title_next_btn);
   it->title_next_btn = btn;
   if (!btn) return;

   elm_object_part_content_set(VIEW(it), NEXT_BTN_PART, btn);
   elm_object_signal_emit(VIEW(it), "elm,state,next_btn,show", "elm");

   evas_object_event_callback_add
     (btn, EVAS_CALLBACK_DEL, _item_title_next_btn_del_cb, it);
}

static void
_item_title_icon_set(Elm_Naviframe_Item_Data *it,
                     Evas_Object *icon)
{
   if (it->title_icon == icon) return;
   evas_object_del(it->title_icon);
   it->title_icon = icon;
   if (!icon) return;

   elm_object_part_content_set(VIEW(it), ICON_PART, icon);
   elm_object_signal_emit(VIEW(it), "elm,state,icon,show", "elm");

   evas_object_event_callback_add
     (icon, EVAS_CALLBACK_DEL, _item_title_icon_del_cb, it);
}

static Evas_Object *
_item_content_unset(Elm_Naviframe_Item_Data *it)
{
   Evas_Object *content = it->content;

   if (!content) return NULL;

   elm_object_part_content_unset(VIEW(it), CONTENT_PART);
   elm_object_signal_emit(VIEW(it), "elm,state,content,hide", "elm");

   evas_object_event_callback_del
     (content, EVAS_CALLBACK_DEL, _item_content_del_cb);

   it->content = NULL;
   return content;
}

static Evas_Object *
_item_title_prev_btn_unset(Elm_Naviframe_Item_Data *it)
{
   Evas_Object *content = it->title_prev_btn;

   if (!content) return NULL;

   elm_object_part_content_unset(VIEW(it), PREV_BTN_PART);
   elm_object_signal_emit(VIEW(it), "elm,state,prev_btn,hide", "elm");

   evas_object_event_callback_del
     (content, EVAS_CALLBACK_DEL, _item_title_prev_btn_del_cb);
   evas_object_smart_callback_del(content, "clicked", _on_item_back_btn_clicked);
   it->title_prev_btn = NULL;
   if (it->auto_pushed_btn) it->auto_pushed_btn = NULL;
   return content;
}

static Evas_Object *
_item_title_next_btn_unset(Elm_Naviframe_Item_Data *it)
{
   Evas_Object *content = it->title_next_btn;

   if (!content) return NULL;

   elm_object_part_content_unset(VIEW(it), NEXT_BTN_PART);
   elm_object_signal_emit(VIEW(it), "elm,state,next_btn,hide", "elm");

   evas_object_event_callback_del
     (content, EVAS_CALLBACK_DEL, _item_title_next_btn_del_cb);

   it->title_next_btn = NULL;
   return content;
}

static Evas_Object *
_item_title_icon_unset(Elm_Naviframe_Item_Data *it)
{
   Evas_Object *content = it->title_icon;

   if (!content) return NULL;

   elm_object_part_content_unset(VIEW(it), ICON_PART);
   elm_object_signal_emit(VIEW(it), "elm,state,icon,hide", "elm");

   evas_object_event_callback_del
     (content, EVAS_CALLBACK_DEL, _item_title_icon_del_cb);

   it->title_icon = NULL;
   return content;
}

/* since we have each item as layout, we can't reusing the layout's
 * aliasing, so let's do it ourselves */
static void
_part_aliasing_eval(const char **part)
{
   if (!*part || !strcmp(*part, "default"))
     *part = CONTENT_PART;
   else if (!strcmp(*part, "prev_btn"))
     *part = PREV_BTN_PART;
   else if (!strcmp(*part, "next_btn"))
     *part = NEXT_BTN_PART;
   else if (!strcmp(*part, "icon"))
     *part = ICON_PART;
}

static void
_title_content_set(Elm_Naviframe_Item_Data *it,
                   const char *part,
                   Evas_Object *content)
{
   Elm_Naviframe_Content_Item_Pair *pair = NULL;
   char buf[1024];

   EINA_INLIST_FOREACH(it->content_list, pair)
     if (!strcmp(part, pair->part)) break;
   if (pair)
     {
        if (pair->content == content) return;
        if (pair->content)
          evas_object_event_callback_del(pair->content,
                                         EVAS_CALLBACK_DEL,
                                         _title_content_del);
        if (content) elm_object_part_content_set(VIEW(it), part, content);
     }
   else
     {
        if (!content) return;

        //Remove the pair if new content was swallowed into other part.
        EINA_INLIST_FOREACH(it->content_list, pair)
          {
             if (pair->content == content)
               {
                  eina_stringshare_del(pair->part);
                  it->content_list = eina_inlist_remove(it->content_list,
                                                        EINA_INLIST_GET(pair));
                  evas_object_event_callback_del(pair->content,
                                                 EVAS_CALLBACK_DEL,
                                                 _title_content_del);
                  free(pair);
                  break;
               }
          }

        //New pair
        pair = ELM_NEW(Elm_Naviframe_Content_Item_Pair);
        if (!pair)
          {
             ERR("Failed to allocate new content part of the item! : naviframe=%p",
             WIDGET(it));
             return;
          }
        pair->it = it;
        eina_stringshare_replace(&pair->part, part);
        it->content_list = eina_inlist_append(it->content_list,
                                              EINA_INLIST_GET(pair));
        elm_object_part_content_set(VIEW(it), part, content);
        snprintf(buf, sizeof(buf), "elm,state,%s,show", part);
        elm_object_signal_emit(VIEW(it), buf, "elm");
     }
   pair->content = content;
   evas_object_event_callback_add(content,
                                  EVAS_CALLBACK_DEL,
                                  _title_content_del,
                                  pair);
}

EOLIAN static void
_elm_naviframe_item_elm_widget_item_part_content_set(Eo *eo_nit EINA_UNUSED,
                                                     Elm_Naviframe_Item_Data *nit,
                                                     const char *part,
                                                     Evas_Object *content)
{
   _part_aliasing_eval(&part);

   //specified parts
   if (!strcmp(part, CONTENT_PART))
     _item_content_set(nit, content);
   else if (!strcmp(part, PREV_BTN_PART))
     _item_title_prev_btn_set(nit, content);
   else if (!strcmp(part, NEXT_BTN_PART))
     _item_title_next_btn_set(nit, content);
   else if (!strcmp(part, ICON_PART))
     _item_title_icon_set(nit, content);
   else
     _title_content_set(nit, part, content);

   elm_layout_sizing_eval(WIDGET(nit));
}

EOLIAN static Evas_Object *
_elm_naviframe_item_elm_widget_item_part_content_get(const Eo *eo_nit EINA_UNUSED,
                                                     Elm_Naviframe_Item_Data *nit,
                                                     const char *part)
{
   _part_aliasing_eval(&part);

   //specified parts
   if (!strcmp(part, CONTENT_PART))
     return nit->content;
   else if (!strcmp(part, PREV_BTN_PART))
     return nit->title_prev_btn;
   else if (!strcmp(part, NEXT_BTN_PART))
     return nit->title_next_btn;
   else if (!strcmp(part, ICON_PART))
     return nit->title_icon;

   //common parts
   return elm_object_part_content_get(VIEW(nit), part);
}

static Evas_Object *
_title_content_unset(Elm_Naviframe_Item_Data *it, const char *part)
{
   Elm_Naviframe_Content_Item_Pair *pair = NULL;
   char buf[1028];
   Evas_Object *content = NULL;

   EINA_INLIST_FOREACH(it->content_list, pair)
     {
        if (!strcmp(part, pair->part))
          {
             content = pair->content;
             eina_stringshare_del(pair->part);
             it->content_list = eina_inlist_remove(it->content_list,
                                                   EINA_INLIST_GET(pair));
             free(pair);
             break;
          }
     }

   if (!content) return NULL;

   elm_object_part_content_unset(VIEW(it), part);
   snprintf(buf, sizeof(buf), "elm,state,%s,hide", part);
   elm_object_signal_emit(VIEW(it), buf, "elm");
   evas_object_event_callback_del(content,
                                  EVAS_CALLBACK_DEL,
                                  _title_content_del);
   return content;
}

EOLIAN static Evas_Object *
_elm_naviframe_item_elm_widget_item_part_content_unset(Eo *eo_nit EINA_UNUSED,
                                                  Elm_Naviframe_Item_Data *nit,
                                                  const char *part)
{
   Evas_Object *o = NULL;

   _part_aliasing_eval(&part);

   //specified parts
   if (!strcmp(part, CONTENT_PART))
     o = _item_content_unset(nit);
   else if (!strcmp(part, PREV_BTN_PART))
     o = _item_title_prev_btn_unset(nit);
   else if (!strcmp(part, NEXT_BTN_PART))
     o = _item_title_next_btn_unset(nit);
   else if (!strcmp(part, ICON_PART))
     o = _item_title_icon_unset(nit);
   else
     o = _title_content_unset(nit, part);

   elm_layout_sizing_eval(WIDGET(nit));

   return o;
}

EOLIAN static void
_elm_naviframe_item_elm_widget_item_signal_emit(Eo *eo_it EINA_UNUSED,
                                                Elm_Naviframe_Item_Data *it,
                                                const char *emission,
                                                const char *source)
{
   elm_object_signal_emit(VIEW(it), emission, source);
}

EOLIAN static void
_elm_naviframe_efl_canvas_group_group_calculate(Eo *obj, Elm_Naviframe_Data *sd)
{
   Evas_Coord minw = 0, minh = 0;
   Elm_Naviframe_Item_Data *it, *top;
   Evas_Coord x, y, w, h;

   if (sd->on_deletion) return;
   if (!sd->stack) return;

   top = (EINA_INLIST_CONTAINER_GET(sd->stack->last, Elm_Naviframe_Item_Data));
   evas_object_geometry_get(obj, &x, &y, &w, &h);
   EINA_INLIST_FOREACH(sd->stack, it)
     {
        evas_object_geometry_set(VIEW(it), x, y, w, h);

        if (it == top)
          {
             edje_object_size_min_calc(elm_layout_edje_get(VIEW(it)),
                                       &it->minw, &it->minh);
             minw = it->minw;
             minh = it->minh;
          }
     }

   evas_object_size_hint_min_set(obj, minw, minh);
   evas_object_size_hint_max_set(obj, -1, -1);
}

static void
_on_item_back_btn_clicked(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   /* Since edje has the event queue, clicked event could be happened
      multiple times on some heavy environment. This callback del will
      prevent those scenario and guarantee only one clicked for it's own
      page. */
   evas_object_smart_callback_del(obj, "clicked", _on_item_back_btn_clicked);
   elm_naviframe_item_pop(data);
}

static Evas_Object *
_back_btn_new(Evas_Object *obj, const char *title_label)
{
   Evas_Object *btn, *ed;
   char buf[1024];

   btn = elm_button_add(obj);

   if (!btn) return NULL;
   evas_object_smart_callback_add(btn, "clicked", _on_item_back_btn_clicked, obj);
   snprintf
     (buf, sizeof(buf), "naviframe/back_btn/%s", elm_widget_style_get(obj));
   elm_object_style_set(btn, buf);
   if (title_label)
     elm_layout_text_set(btn, NULL, title_label);
   else
     elm_object_domain_translatable_text_set(btn, PACKAGE, N_("Back"));

   /* HACK NOTE: this explicit check only exists to avoid an ERR()
    * message from elm_layout_content_set().
    *
    * The button was ALWAYS supposed to support an elm.swallow.content, but
    * default naviframe/back_btn/default theme did not provide such, then
    * old themes would emit such error message.
    *
    * Once we can break the theme API, remove this check and always
    * set an icon.
    */
   ed = elm_layout_edje_get(btn);
   if (edje_object_part_exists(ed, CONTENT_PART))
     {
        Evas_Object *ico = elm_icon_add(btn);
        elm_icon_standard_set(ico, "go-previous");
        elm_layout_content_set(btn, CONTENT_PART, ico);
     }

   return btn;
}

EOLIAN static void
_elm_naviframe_efl_layout_signal_signal_emit(Eo *obj, Elm_Naviframe_Data *sd EINA_UNUSED, const char *emission, const char *source)
{
   Elm_Object_Item *eo_top_it;

   eo_top_it = elm_naviframe_top_item_get(obj);
   if (!eo_top_it) return;
   ELM_NAVIFRAME_ITEM_DATA_GET(eo_top_it, top_it);

   efl_layout_signal_emit(VIEW(top_it), emission, source);
}

/* content/text smart functions proxying things to the top item, which
 * is the resize object of the layout */
static Eina_Bool
_elm_naviframe_text_set(Eo *obj, Elm_Naviframe_Data *sd EINA_UNUSED, const char *part, const char *label)
{
   Elm_Object_Item *it;
   const char *text = NULL;

   it = elm_naviframe_top_item_get(obj);
   if (!it) return EINA_FALSE;

   elm_object_item_part_text_set(it, part, label);
   text = elm_object_item_part_text_get(it, part);
   if ((text) && !strcmp(text, label))
     return EINA_TRUE;
   return EINA_FALSE;
}

static const char *
_elm_naviframe_text_get(Eo *obj, Elm_Naviframe_Data *sd EINA_UNUSED, const char *part)
{
   Elm_Object_Item *it = elm_naviframe_top_item_get(obj);
   if (!it) return NULL;

   return elm_object_item_part_text_get(it, part);
}

/* we have to keep a "manual" set here because of the callbacks on the
 * children */
static Eina_Bool
_elm_naviframe_content_set(Eo *obj, Elm_Naviframe_Data *sd EINA_UNUSED, const char *part, Evas_Object *content)
{
   Elm_Object_Item *it;

   it = elm_naviframe_top_item_get(obj);
   if (!it) return EINA_FALSE;

   elm_object_item_part_content_set(it, part, content);

   if (content == elm_object_item_part_content_get(it, part))
     return EINA_TRUE;

   return EINA_FALSE;
}

static Evas_Object*
_elm_naviframe_content_get(Eo *obj, Elm_Naviframe_Data *sd EINA_UNUSED, const char *part)
{
   Elm_Object_Item *it = elm_naviframe_top_item_get(obj);

   if (!it) return NULL;

   return elm_object_item_part_content_get(it, part);
}

static Evas_Object*
_elm_naviframe_content_unset(Eo *obj, Elm_Naviframe_Data *sd EINA_UNUSED, const char *part)
{
   Elm_Object_Item *it = elm_naviframe_top_item_get(obj);

   if (!it) return NULL;

   return elm_object_item_part_content_unset(it, part);
}

static void
_on_item_title_clicked(void *data,
                       Evas_Object *obj EINA_UNUSED,
                       const char *emission EINA_UNUSED,
                       const char *source EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it = data;

   efl_event_callback_legacy_call(WIDGET(it), ELM_NAVIFRAME_EVENT_TITLE_CLICKED, EO_OBJ(it));
}

/* "elm,state,cur,pushed"
 */
static void
_on_item_push_finished(void *data,
                       Evas_Object *obj EINA_UNUSED,
                       const char *emission EINA_UNUSED,
                       const char *source EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it = data;

   if (!it) return;

   ELM_NAVIFRAME_DATA_GET(WIDGET(it), sd);

   evas_object_hide(VIEW(it));

   elm_object_signal_emit(VIEW(it), "elm,state,invisible", "elm");

   if (sd->freeze_events)
     evas_object_freeze_events_set(VIEW(it), EINA_FALSE);
}

/* "elm,state,cur,popped"
 */
static void
_on_item_pop_finished(void *data,
                      Evas_Object *obj EINA_UNUSED,
                      const char *emission EINA_UNUSED,
                      const char *source EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it = data;
   Elm_Object_Item *eo_prev_it = NULL;

   ELM_NAVIFRAME_DATA_GET(WIDGET(it), sd);

   eo_prev_it = elm_naviframe_top_item_get(WIDGET(it));
   if (eo_prev_it)
     {
        ELM_NAVIFRAME_ITEM_DATA_GET(eo_prev_it, prev_it);

        evas_object_raise(VIEW(prev_it));
     }

   if (sd->preserve)
     elm_widget_tree_unfocusable_set(VIEW(it), EINA_FALSE);
   sd->popping = eina_list_remove(sd->popping, it);

   efl_del(EO_OBJ(it));
}

/* "elm,state,new,pushed",
 * "elm,state,prev,popped
 */
static void
_on_item_show_finished(void *data,
                       Evas_Object *obj EINA_UNUSED,
                       const char *emission EINA_UNUSED,
                       const char *source EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it = data;

   ELM_NAVIFRAME_DATA_GET(WIDGET(it), sd);

   elm_object_signal_emit(VIEW(it), "elm,state,visible", "elm");

   elm_widget_tree_unfocusable_set(VIEW(it), EINA_FALSE);
   _prev_page_focus_recover(it);

   if (sd->freeze_events)
     evas_object_freeze_events_set(VIEW(it), EINA_FALSE);

   it->pushing = EINA_FALSE;

   efl_event_callback_legacy_call(WIDGET(it), ELM_NAVIFRAME_EVENT_TRANSITION_FINISHED, EO_OBJ(it));

   if (EO_OBJ(it) == elm_naviframe_top_item_get(WIDGET(it)))
     efl_event_callback_legacy_call(WIDGET(it), ELM_NAVIFRAME_EVENT_ITEM_ACTIVATED, EO_OBJ(it));
}

static void
_on_item_size_hints_changed(void *data,
                            Evas *e EINA_UNUSED,
                            Evas_Object *obj EINA_UNUSED,
                            void *event_info EINA_UNUSED)
{
   elm_layout_sizing_eval(data);
}

static void
_item_dispmode_set(Elm_Naviframe_Item_Data *it, Evas_Display_Mode dispmode)
{
   if (it->dispmode == dispmode) return;
   switch (dispmode)
     {
      case EVAS_DISPLAY_MODE_COMPRESS:
         elm_object_signal_emit(VIEW(it), "elm,state,display,compress", "elm");
         break;
      default:
         elm_object_signal_emit(VIEW(it), "elm,state,display,default", "elm");
         break;
     }
   it->dispmode = dispmode;
}

static char *
_access_prev_btn_info_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED)
{
   return strdup(E_("Back"));
}

EOLIAN static void
_elm_naviframe_item_efl_object_invalidate(Eo *eo_item, Elm_Naviframe_Item_Data *it)
{
   Elm_Naviframe_Item_Data *prev_it = NULL;
   Elm_Naviframe_Content_Item_Pair *content_pair;
   Eina_Inlist *l;
   Eina_Bool top;

   ELM_NAVIFRAME_DATA_GET(WIDGET(it), sd);
   if (it->title_prev_btn)
     evas_object_event_callback_del(it->title_prev_btn, EVAS_CALLBACK_DEL, _item_title_prev_btn_del_cb);
   if (it->title_next_btn)
     evas_object_event_callback_del(it->title_next_btn, EVAS_CALLBACK_DEL, _item_title_next_btn_del_cb);
   if (it->title_icon)
     evas_object_event_callback_del(it->title_icon, EVAS_CALLBACK_DEL, _item_title_icon_del_cb);
   EINA_INLIST_FOREACH_SAFE(it->content_list, l, content_pair)
     {
        if (content_pair->content)
          {
             /* content object will be destroyed naturally */
             evas_object_event_callback_del(content_pair->content,
                                            EVAS_CALLBACK_DEL,
                                            _title_content_del);
             evas_object_del(content_pair->content);
          }
        eina_stringshare_del(content_pair->part);
        free(content_pair);
     }
   if (it->content)
     {
        evas_object_event_callback_del(it->content, EVAS_CALLBACK_DEL, _item_content_del_cb);
        if ((sd->preserve) && (!sd->on_deletion))
          {
             /* so that elm does not delete the contents with the item's
              * view after the destructor */
             elm_object_part_content_unset(VIEW(it), CONTENT_PART);
             evas_object_hide(it->content);
          }
     }
   if (evas_object_data_get(VIEW(it), "out_of_list"))
     goto end;

   top = (eo_item == elm_naviframe_top_item_get(WIDGET(it)));
   sd->stack = eina_inlist_remove(sd->stack, EINA_INLIST_GET(it));

   if (top && !sd->on_deletion) /* must raise another one */
     {
        if (sd->stack && sd->stack->last)
          prev_it = EINA_INLIST_CONTAINER_GET(sd->stack->last,
                                              Elm_Naviframe_Item_Data);
        if (!prev_it)
          {
             elm_widget_tree_unfocusable_set(VIEW(it), EINA_TRUE);
             goto end;
          }

        elm_widget_tree_unfocusable_set(VIEW(prev_it), EINA_FALSE);
        elm_widget_tree_unfocusable_set(VIEW(it), EINA_TRUE);

        if (sd->freeze_events)
          evas_object_freeze_events_set(VIEW(prev_it), EINA_FALSE);
        _resize_object_reset(WIDGET(prev_it), prev_it);
        evas_object_show(VIEW(prev_it));

        _prev_page_focus_recover(prev_it);

        elm_object_signal_emit(VIEW(prev_it), "elm,state,visible", "elm");

        efl_event_callback_legacy_call(WIDGET(prev_it), ELM_NAVIFRAME_EVENT_ITEM_ACTIVATED, EO_OBJ(prev_it));
     }
end:
   efl_invalidate(efl_super(eo_item, ELM_NAVIFRAME_ITEM_CLASS));
}

EOLIAN static Eo *
_elm_naviframe_item_efl_object_constructor(Eo *eo_item, Elm_Naviframe_Item_Data *it)
{
   eo_item = efl_constructor(efl_super(eo_item, ELM_NAVIFRAME_ITEM_CLASS));
   it->base = efl_data_scope_get(eo_item, ELM_WIDGET_ITEM_CLASS);

   return eo_item;
}

static Elm_Object_Item *
_item_new(Evas_Object *obj,
          const Elm_Object_Item *eo_prev_it,
          const char *title_label,
          Evas_Object *prev_btn,
          Evas_Object *next_btn,
          Evas_Object *content,
          const char *item_style)
{
   Eo *eo_item;

   ELM_NAVIFRAME_DATA_GET(obj, sd);

   eo_item = efl_add(ELM_NAVIFRAME_ITEM_CLASS, obj);

   if (!eo_item)
     {
        ERR("Failed to allocate new item! : naviframe=%p", obj);
        return NULL;
     }

   ELM_NAVIFRAME_ITEM_DATA_GET(eo_item, it);

   //item base layout
   Eo *elem = elm_layout_add(obj);
   VIEW_SET(it, elem);
   evas_object_smart_member_add(VIEW(it), obj);

   efl_access_object_role_set(elem, EFL_ACCESS_ROLE_PAGE_TAB);
   efl_access_object_i18n_name_set(elem, (char*)title_label);

   if (!elm_widget_sub_object_add(obj, VIEW(it)))
     ERR("could not add %p as sub object of %p", VIEW(it), obj);

   evas_object_event_callback_add
     (VIEW(it), EVAS_CALLBACK_CHANGED_SIZE_HINTS,
     _on_item_size_hints_changed, obj);

   elm_object_signal_callback_add
     (VIEW(it), "elm,action,show,finished", "*", _on_item_show_finished, it);
   elm_object_signal_callback_add
     (VIEW(it), "elm,action,pushed,finished", "*", _on_item_push_finished, it);
   elm_object_signal_callback_add
     (VIEW(it), "elm,action,popped,finished", "*", _on_item_pop_finished, it);
   elm_object_signal_callback_add
     (VIEW(it), "elm,action,title,transition,finished", "*", _on_item_title_transition_finished, it);
   elm_object_signal_callback_add
     (VIEW(it), "elm,action,title,clicked", "*", _on_item_title_clicked, it);

   _item_style_set(it, item_style);

   if (title_label)
     elm_wdg_item_part_text_set(eo_item, TITLE_PART, title_label);

   //title buttons
   if ((!prev_btn) && sd->auto_pushed && eo_prev_it)
     {
        ELM_NAVIFRAME_ITEM_DATA_GET(eo_prev_it, prev_it);
        const char *prev_title = prev_it->title_label;
        prev_btn = _back_btn_new(obj, prev_title);
        it->auto_pushed_btn = prev_btn;
     }

   if (prev_btn)
     {
        elm_wdg_item_part_content_set(eo_item, PREV_BTN_PART, prev_btn);
        if (!elm_layout_text_get(prev_btn, NULL))
          {
             if (!_access_info_has(prev_btn, ELM_ACCESS_INFO))
               {
                  /* set access info */
                  _elm_access_callback_set
                     (_elm_access_info_get(prev_btn), ELM_ACCESS_INFO,
                      _access_prev_btn_info_cb, it);
               }
          }
     }

   if (next_btn)
     {
        elm_wdg_item_part_content_set(eo_item, NEXT_BTN_PART, next_btn);

        if (!elm_layout_text_get(next_btn, NULL))
          {
             if (!_access_info_has(next_btn, ELM_ACCESS_INFO))
               {
                  /* set access info */
                  _elm_access_text_set
                     (_elm_access_info_get(next_btn), ELM_ACCESS_INFO, E_("Next"));
               }
          }
     }

   _item_content_set(it, content);
   _item_dispmode_set(it, sd->dispmode);

   it->title_enabled = EINA_TRUE;

   return EO_OBJ(it);
}

static void
_on_obj_size_hints_changed(void *data EINA_UNUSED, Evas *e EINA_UNUSED,
                           Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Elm_Naviframe_Item_Data *it;
   Evas_Display_Mode dispmode;

   ELM_NAVIFRAME_DATA_GET(obj, sd);

   dispmode = evas_object_size_hint_display_mode_get(obj);
   if (sd->dispmode == dispmode) return;

   sd->dispmode = dispmode;

   EINA_INLIST_FOREACH(sd->stack, it)
     _item_dispmode_set(it, dispmode);
}

EOLIAN static void
_elm_naviframe_efl_canvas_group_group_add(Eo *obj, Elm_Naviframe_Data *priv)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   efl_canvas_group_add(efl_super(obj, MY_CLASS));

   priv->dummy_edje = wd->resize_obj;
   evas_object_smart_member_add(priv->dummy_edje, obj);

   priv->auto_pushed = _elm_config->naviframe_prev_btn_auto_pushed;
   priv->freeze_events = EINA_TRUE;

   evas_object_event_callback_add(obj, EVAS_CALLBACK_CHANGED_SIZE_HINTS,
                                  _on_obj_size_hints_changed, obj);
   elm_widget_can_focus_set(obj, EINA_FALSE);
}

static void
_send_signal(Elm_Naviframe_Item_Data *it, const char *sig)
{
   if (!it) return ;
   elm_object_signal_emit(VIEW(it), sig, "elm");
   edje_object_message_signal_process(elm_layout_edje_get(VIEW(it)));
}

static void
_deferred(void *data, const Efl_Event *event EINA_UNUSED)
{
   Elm_Naviframe_Data *nfd = data;
   Elm_Naviframe_Op *nfo;

   EINA_LIST_FREE(nfd->ops, nfo)
     {
        const char *signals_cur[] = {
          "elm,state,cur,popped,deferred",
          "elm,state,cur,pushed,deferred"
        };
        const char *signals_prev = "elm,state,prev,popped,deferred";
        const char *signals_new = "elm,state,new,pushed,deferred";
        Elm_Naviframe_Item_Data *cur;
        Elm_Naviframe_Item_Data *other;

        cur = nfo->push ? nfo->related : nfo->self;
        other = nfo->push ? nfo->self : nfo->related;

        _send_signal(cur, signals_cur[nfo->push]);
        _send_signal(other, nfo->push ? signals_new : signals_prev);

        free(nfo);
     }

   efl_event_callback_del(nfd->obj, EFL_CANVAS_OBJECT_EVENT_ANIMATOR_TICK, _deferred, nfd);
}

EOLIAN static void
_elm_naviframe_efl_canvas_group_group_del(Eo *obj, Elm_Naviframe_Data *sd)
{
   Elm_Naviframe_Item_Data *it;
   Elm_Naviframe_Op *nfo;
   Eina_Inlist *l = NULL;

   sd->on_deletion = EINA_TRUE;
   if (sd->stack) l = sd->stack->last;

   while (l)
     {
        it = EINA_INLIST_CONTAINER_GET(l, Elm_Naviframe_Item_Data);
        l = l->prev;
        efl_del(EO_OBJ(it));
     }

   // No need to cleanup animator as it is an event on myself
   EINA_LIST_FREE(sd->ops, nfo)
     free(nfo);
   EINA_LIST_FREE(sd->popping, it)
     efl_del(EO_OBJ(it));

   evas_object_del(sd->dummy_edje);

   efl_canvas_group_del(efl_super(obj, MY_CLASS));
}

//Show only the top item view
EOLIAN static void
_elm_naviframe_efl_gfx_entity_visible_set(Eo *obj, Elm_Naviframe_Data *sd EINA_UNUSED, Eina_Bool vis)
{
   Elm_Object_Item *eo_top;

   if (_evas_object_intercept_call(obj, EVAS_OBJECT_INTERCEPT_CB_VISIBLE, 0, vis))
     return;

   efl_gfx_entity_visible_set(efl_super(obj, MY_CLASS), vis);
   if (!vis) return;

   eo_top = elm_naviframe_top_item_get(obj);
   ELM_NAVIFRAME_ITEM_DATA_GET(eo_top, top);

   if (top && !top->delete_me)
     evas_object_show(VIEW(top));
}

static Eina_Bool
_key_action_top_item_get(Evas_Object *obj, const char *params EINA_UNUSED)
{
   Elm_Object_Item *eo_item = NULL;
   eo_item = elm_obj_naviframe_top_item_get(obj);
   if (!eo_item) return EINA_FALSE;

   //FIXME: Replace this below code to elm_naviframe_item_pop() at elm 2.0.
   ///Leave for compatibility.
   ELM_NAVIFRAME_ITEM_DATA_GET(eo_item, it);
   if (it->title_prev_btn)
     efl_event_callback_legacy_call(it->title_prev_btn, EFL_INPUT_EVENT_CLICKED, NULL);

   return EINA_TRUE;
}

static Eina_Bool
_key_action_item_pop(Evas_Object *obj, const char *params EINA_UNUSED)
{
   Elm_Object_Item *eo_item = NULL;
   eo_item = elm_naviframe_top_item_get(obj);
   if (!eo_item) return EINA_FALSE;

   ELM_NAVIFRAME_ITEM_DATA_GET(eo_item, it);

   if (it->pushing || it->popping) return EINA_FALSE;

   elm_naviframe_item_pop(obj);

   return EINA_TRUE;
}

EOLIAN static void
_elm_naviframe_efl_ui_widget_on_access_update(Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd, Eina_Bool is_access)
{
   Elm_Naviframe_Item_Data *it;

   EINA_INLIST_FOREACH(sd->stack, it)
     _access_obj_process(it, is_access);
}

static void
_schedule_deferred(Elm_Naviframe_Op *nfo, Elm_Naviframe_Data *sd)
{
   if (!sd->ops)
     efl_event_callback_add(sd->obj, EFL_CANVAS_OBJECT_EVENT_ANIMATOR_TICK, _deferred, sd);

   sd->ops = eina_list_append(sd->ops, nfo);
}

static void
_item_push_helper(Elm_Naviframe_Item_Data *item)
{
   Elm_Object_Item *eo_top_item;
   Evas_Object *obj = WIDGET(item);
   ELM_NAVIFRAME_DATA_GET(obj, sd);
   eo_top_item = elm_naviframe_top_item_get(obj);
   evas_object_show(VIEW(item));

   ELM_NAVIFRAME_ITEM_DATA_GET(eo_top_item, top_item);
   _resize_object_reset(obj, item);
   if (top_item)
     {
        Elm_Naviframe_Op *nfo = calloc(1, sizeof (Elm_Naviframe_Op));

        elm_widget_tree_unfocusable_set(VIEW(item), EINA_TRUE);
        elm_widget_tree_unfocusable_set(VIEW(top_item), EINA_TRUE);

        if (sd->freeze_events)
          {
             evas_object_freeze_events_set(VIEW(item), EINA_TRUE);
             evas_object_freeze_events_set(VIEW(top_item), EINA_TRUE);
          }
        elm_object_signal_emit(VIEW(top_item), "elm,state,cur,pushed", "elm");
        elm_object_signal_emit(VIEW(item), "elm,state,new,pushed", "elm");
        edje_object_message_signal_process(elm_layout_edje_get(VIEW(top_item)));
        edje_object_message_signal_process(elm_layout_edje_get(VIEW(item)));

        nfo->self = item;
        nfo->related = top_item;
        nfo->push = EINA_TRUE;

        _schedule_deferred(nfo, sd);

        if (top_item) top_item->pushing = EINA_FALSE;
        item->pushing = EINA_TRUE;
     }
   else
     {
        if (elm_object_focus_allow_get(VIEW(item)))
          elm_object_focus_set(VIEW(item), EINA_TRUE);
        else
          elm_object_focus_set(WIDGET(item), EINA_TRUE);
     }

   sd->stack = eina_inlist_append(sd->stack, EINA_INLIST_GET(item));

   if (!top_item)
     elm_object_signal_emit(VIEW(item), "elm,state,visible", "elm");

   elm_layout_sizing_eval(obj);

   if (!top_item)
     efl_event_callback_legacy_call(obj, ELM_NAVIFRAME_EVENT_ITEM_ACTIVATED, EO_OBJ(item));
}

Evas_Object *
elm_naviframe_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   return elm_legacy_add(MY_CLASS, parent);
}

EOLIAN static Eo *
_elm_naviframe_efl_object_constructor(Eo *obj, Elm_Naviframe_Data *sd)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   sd->obj = obj;
   legacy_child_focus_handle(obj);
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   evas_object_smart_callbacks_descriptions_set(obj, _smart_callbacks);
   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_PAGE_TAB_LIST);

   return obj;
}

EOLIAN static Elm_Object_Item*
_elm_naviframe_item_push(Eo *obj, Elm_Naviframe_Data *sd EINA_UNUSED, const char *title_label, Evas_Object *prev_btn, Evas_Object *next_btn, Evas_Object *content, const char *item_style)
{
   Elm_Object_Item *top_item, *eo_item;

   top_item = elm_naviframe_top_item_get(obj);
   eo_item = _item_new(obj, top_item,
                  title_label, prev_btn, next_btn, content, item_style);
   ELM_NAVIFRAME_ITEM_DATA_GET(eo_item, item);
   if (!item) return NULL;
   _item_push_helper(item);
   return eo_item;
}

EOLIAN static Elm_Object_Item*
_elm_naviframe_item_insert_before(Eo *obj, Elm_Naviframe_Data *sd, Elm_Object_Item *eo_before, const char *title_label, Evas_Object *prev_btn, Evas_Object *next_btn, Evas_Object *content, const char *item_style)
{
   Elm_Object_Item *eo_it;
   Elm_Naviframe_Item_Data *prev_it = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(eo_before, NULL);
   ELM_NAVIFRAME_ITEM_DATA_GET(eo_before, before);

   ELM_NAVIFRAME_ITEM_CHECK_OR_RETURN(before, NULL);

   eo_it = eo_before;
   ELM_NAVIFRAME_ITEM_DATA_GET(eo_it, it);
   if (EINA_INLIST_GET(it)->prev)
     prev_it = EINA_INLIST_CONTAINER_GET(EINA_INLIST_GET(it)->prev,
                                         Elm_Naviframe_Item_Data);
   eo_it = _item_new(obj, EO_OBJ(prev_it),
                  title_label, prev_btn, next_btn, content, item_style);
   if (!eo_it) return NULL;

   it = efl_data_scope_get(eo_it, ELM_NAVIFRAME_ITEM_CLASS);

   sd->stack = eina_inlist_prepend_relative
       (sd->stack, EINA_INLIST_GET(it),
       EINA_INLIST_GET(before));

   elm_widget_tree_unfocusable_set(VIEW(it), EINA_TRUE);
   evas_object_hide(VIEW(it));
   elm_object_signal_emit(VIEW(it), "elm,state,invisible", "elm");

   elm_layout_sizing_eval(obj);

   return eo_it;
}

EOLIAN static Elm_Object_Item*
_elm_naviframe_item_insert_after(Eo *obj, Elm_Naviframe_Data *sd, Elm_Object_Item *eo_after, const char *title_label, Evas_Object *prev_btn, Evas_Object *next_btn, Evas_Object *content, const char *item_style)
{
   Elm_Object_Item *eo_item;
   Eina_Bool top_inserted = EINA_FALSE;

   EINA_SAFETY_ON_NULL_RETURN_VAL(eo_after, NULL);
   ELM_NAVIFRAME_ITEM_DATA_GET(eo_after, after);

   ELM_NAVIFRAME_ITEM_CHECK_OR_RETURN(after, NULL);

   eo_item = _item_new(obj, eo_after,
                  title_label, prev_btn, next_btn, content, item_style);
   if (!eo_item) return NULL;

   ELM_NAVIFRAME_ITEM_DATA_GET(eo_item, it);

   if (elm_naviframe_top_item_get(obj) == eo_after) top_inserted = EINA_TRUE;

   sd->stack = eina_inlist_append_relative
       (sd->stack, EINA_INLIST_GET(it),
       EINA_INLIST_GET(after));


   if (top_inserted)
     {
        elm_widget_tree_unfocusable_set(VIEW(after), EINA_TRUE);
        _resize_object_reset(obj, it);
        evas_object_show(VIEW(it));
        evas_object_hide(VIEW(after));
        if (elm_object_focus_allow_get(VIEW(it)))
          elm_object_focus_set(VIEW(it), EINA_TRUE);
        else
          elm_object_focus_set(WIDGET(it), EINA_TRUE);
        elm_object_signal_emit(VIEW(it), "elm,state,visible", "elm");
        elm_object_signal_emit(VIEW(after), "elm,state,invisible", "elm");
     }
   else
     elm_object_signal_emit(VIEW(it), "elm,state,invisible", "elm");

   elm_layout_sizing_eval(obj);

   if (top_inserted)
     efl_event_callback_legacy_call(obj, ELM_NAVIFRAME_EVENT_ITEM_ACTIVATED, eo_item);

   return eo_item;
}

EOLIAN static Evas_Object*
_elm_naviframe_item_pop(Eo *obj, Elm_Naviframe_Data *sd)
{
   Elm_Object_Item *eo_item;
   Elm_Naviframe_Item_Data *prev_it = NULL;
   Evas_Object *content = NULL;

   eo_item = elm_naviframe_top_item_get(obj);
   if (!eo_item) return NULL;

   ELM_NAVIFRAME_ITEM_DATA_GET(eo_item, it);

   if (it->pushing || it->popping) return NULL;
   it->popping = EINA_TRUE;

   evas_object_ref(obj);
   if (it->pop_cb)
     {
        efl_ref(eo_item);
        if (!it->pop_cb(it->pop_data, eo_item))
          {
             efl_unref(eo_item);
             if (!it->delete_me)
               {
                  /* To avoid multiple item pops, the auto pushed button deletes
                     its clicked callback once it is called.
                     Since the item is not popped or deleted here, the deleted
                     callback of the auto pushed button should be restored. */
                  if (it->auto_pushed_btn)
                    evas_object_smart_callback_add(it->auto_pushed_btn, "clicked", _on_item_back_btn_clicked, obj);
                  it->popping = EINA_FALSE;
               }
             evas_object_unref(obj);
             return NULL;
          }
        efl_unref(eo_item);
     }
   evas_object_unref(obj);

   if (sd->preserve)
     content = it->content;

   evas_object_data_set(VIEW(it), "out_of_list", (void *)1);

   if (sd->stack->last->prev)
     prev_it = EINA_INLIST_CONTAINER_GET
         (sd->stack->last->prev, Elm_Naviframe_Item_Data);

   sd->stack = eina_inlist_remove(sd->stack, EINA_INLIST_GET(it));

   if (prev_it)
     {
        Elm_Naviframe_Op *nfo = calloc(1, sizeof (Elm_Naviframe_Op));

        elm_widget_tree_unfocusable_set(VIEW(it), EINA_TRUE);

        if (sd->freeze_events)
          {
             evas_object_freeze_events_set(VIEW(it), EINA_TRUE);
             evas_object_freeze_events_set(VIEW(prev_it), EINA_TRUE);
          }

        elm_widget_resize_object_set(obj, VIEW(prev_it));

        /* these 2 signals MUST take place simultaneously */
        elm_object_signal_emit(VIEW(it), "elm,state,cur,popped", "elm");
        elm_object_signal_emit(VIEW(prev_it), "elm,state,prev,popped", "elm");
        edje_object_message_signal_process(elm_layout_edje_get(VIEW(it)));
        edje_object_message_signal_process(elm_layout_edje_get(VIEW(prev_it)));

        //Show hidden previous view when pop transition begins.
        evas_object_show(VIEW(prev_it));

        if (!nfo) goto on_error;

        nfo->self = it;
        nfo->related = prev_it;
        nfo->push = EINA_FALSE;

        sd->popping = eina_list_append(sd->popping, it);

        _schedule_deferred(nfo, sd);
     }
   else
     efl_del(eo_item);

 on_error:
   return content;
}

EOLIAN static void
_elm_naviframe_item_pop_to(Eo *eo_it, Elm_Naviframe_Item_Data *it)
{
   Eina_Inlist *l;

   ELM_NAVIFRAME_DATA_GET(WIDGET(it), sd);

   if (eo_it == elm_naviframe_top_item_get(WIDGET(it))) return;

   l = sd->stack->last->prev;

   sd->on_deletion = EINA_TRUE;

   while (l)
     {
        Elm_Naviframe_Item_Data *iit = EINA_INLIST_CONTAINER_GET
            (l, Elm_Naviframe_Item_Data);

        if (iit == it) break;

        l = l->prev;

        efl_del(EO_OBJ(iit));
     }

   sd->on_deletion = EINA_FALSE;

   elm_naviframe_item_pop(WIDGET(it));
}

EOLIAN static void
_elm_naviframe_item_promote(Eo *eo_it, Elm_Naviframe_Item_Data *it)
{
   Elm_Object_Item *eo_prev_top;
   Elm_Naviframe_Item_Data *nit = it;

   ELM_NAVIFRAME_DATA_GET(WIDGET(nit), sd);

   eo_prev_top = elm_naviframe_top_item_get(WIDGET(nit));
   if (eo_it == eo_prev_top) return;

   sd->stack = eina_inlist_remove(sd->stack, EINA_INLIST_GET(nit));
   _item_push_helper(nit);
}

EOLIAN static void
_elm_naviframe_item_simple_promote(Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd, Evas_Object *content)
{
   Elm_Naviframe_Item_Data *itr;

   EINA_INLIST_FOREACH(sd->stack, itr)
     {
        if (elm_object_item_content_get(EO_OBJ(itr)) == content)
          {
             elm_naviframe_item_promote(EO_OBJ(itr));
             break;
          }
     }
}

EOLIAN static void
_elm_naviframe_content_preserve_on_pop_set(Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd, Eina_Bool preserve)
{
   sd->preserve = !!preserve;
}

EOLIAN static Eina_Bool
_elm_naviframe_content_preserve_on_pop_get(const Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd)
{
   return sd->preserve;
}

EOLIAN static Elm_Object_Item*
_elm_naviframe_top_item_get(const Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd)
{
   if (!sd->stack) return NULL;
   Elm_Naviframe_Item_Data *ret_it = EINA_INLIST_CONTAINER_GET
                                (sd->stack->last, Elm_Naviframe_Item_Data);
   return EO_OBJ(ret_it);
}

EOLIAN static Elm_Object_Item*
_elm_naviframe_bottom_item_get(const Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd)
{
   if (!sd->stack) return NULL;
   Elm_Naviframe_Item_Data *ret_it = EINA_INLIST_CONTAINER_GET
                                (sd->stack, Elm_Naviframe_Item_Data);
   return EO_OBJ(ret_it);
}

EOLIAN static void
_elm_naviframe_item_elm_widget_item_style_set(Eo *eo_item EINA_UNUSED,
                              Elm_Naviframe_Item_Data *nit,
                              const char *item_style)
{
   if (item_style && eina_streq(item_style, elm_wdg_item_style_get(EO_OBJ(nit)))) return;

   if (!item_style)
     if (eina_streq("basic", elm_wdg_item_style_get(EO_OBJ(nit)))) return;

   _item_style_set(nit, item_style);
   _item_signals_emit(nit);
   _item_title_enabled_update(nit, EINA_FALSE);
}

void
elm_naviframe_item_style_set(Elm_Object_Item *obj, const char *style)
{
   elm_wdg_item_style_set(obj, style);
}

const char *
elm_naviframe_item_style_get(const Elm_Object_Item *obj)
{
   return elm_wdg_item_style_get(obj);
}

EINA_DEPRECATED void
elm_naviframe_item_title_visible_set(Elm_Object_Item *it,
                                     Eina_Bool visible)
{
   elm_naviframe_item_title_enabled_set(it, visible, EINA_FALSE);
}

EINA_DEPRECATED Eina_Bool
elm_naviframe_item_title_visible_get(const Elm_Object_Item *it)
{
   return elm_naviframe_item_title_enabled_get(it);
}

EOLIAN static void
_elm_naviframe_item_title_enabled_set(Eo *eo_item EINA_UNUSED,
                                      Elm_Naviframe_Item_Data *nit,
                                      Eina_Bool enabled,
                                      Eina_Bool transition)
{
   enabled = !!enabled;
   if (nit->title_enabled == enabled) return;

   nit->title_enabled = enabled;

   transition = !!transition;
   _item_title_enabled_update(nit, transition);
}

EOLIAN static Eina_Bool
_elm_naviframe_item_title_enabled_get(const Eo *eo_item EINA_UNUSED, Elm_Naviframe_Item_Data *nit)
{
   return nit->title_enabled;
}

EOLIAN static void
_elm_naviframe_item_pop_cb_set(Eo *eo_item EINA_UNUSED,
                               Elm_Naviframe_Item_Data *nit,
                               Elm_Naviframe_Item_Pop_Cb func,
                               void *data)
{
   nit->pop_cb = func;
   nit->pop_data = data;
}

EOLIAN static Eina_List*
_elm_naviframe_item_efl_access_object_access_children_get(const Eo *eo_item EINA_UNUSED, Elm_Naviframe_Item_Data *nit)
{
   Eina_List *ret = NULL;

   ret = eina_list_append(ret, VIEW(nit));
   return ret;
}

EOLIAN static void
_elm_naviframe_prev_btn_auto_pushed_set(Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd, Eina_Bool auto_pushed)
{
   sd->auto_pushed = !!auto_pushed;
}

EOLIAN static Eina_Bool
_elm_naviframe_prev_btn_auto_pushed_get(const Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd)
{
   return sd->auto_pushed;
}

EOLIAN static Eina_List*
_elm_naviframe_items_get(const Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd)
{
   Eina_List *ret = NULL;
   Elm_Naviframe_Item_Data *itr;

   EINA_INLIST_FOREACH(sd->stack, itr)
     ret = eina_list_append(ret, EO_OBJ(itr));

   return ret;
}

EOLIAN static void
_elm_naviframe_event_enabled_set(Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd, Eina_Bool enabled)
{
   enabled = !!enabled;
   if (sd->freeze_events == !enabled) return;
   sd->freeze_events = !enabled;
}

EOLIAN static Eina_Bool
_elm_naviframe_event_enabled_get(const Eo *obj EINA_UNUSED, Elm_Naviframe_Data *sd)
{
   return !sd->freeze_events;
}

static void
_elm_naviframe_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

EOLIAN const Efl_Access_Action_Data *
_elm_naviframe_efl_access_widget_action_elm_actions_get(const Eo *obj EINA_UNUSED, Elm_Naviframe_Data *pd EINA_UNUSED)
{
   static Efl_Access_Action_Data atspi_actions[] = {
          { "top_item_get", "top_item_get", NULL, _key_action_top_item_get },
          { "item_pop", "item_pop", NULL, _key_action_item_pop },
          { NULL, NULL, NULL, NULL }
   };
   return &atspi_actions[0];
}

/* Standard widget overrides */

ELM_WIDGET_KEY_DOWN_DEFAULT_IMPLEMENT(elm_naviframe, Elm_Naviframe_Data)

/* Efl.Part begin */

ELM_PART_OVERRIDE(elm_naviframe, ELM_NAVIFRAME, Elm_Naviframe_Data)
ELM_PART_OVERRIDE_CONTENT_SET(elm_naviframe, ELM_NAVIFRAME, Elm_Naviframe_Data)
ELM_PART_OVERRIDE_CONTENT_GET(elm_naviframe, ELM_NAVIFRAME, Elm_Naviframe_Data)
ELM_PART_OVERRIDE_CONTENT_UNSET(elm_naviframe, ELM_NAVIFRAME, Elm_Naviframe_Data)
ELM_PART_OVERRIDE_TEXT_SET(elm_naviframe, ELM_NAVIFRAME, Elm_Naviframe_Data)
ELM_PART_OVERRIDE_TEXT_GET(elm_naviframe, ELM_NAVIFRAME, Elm_Naviframe_Data)
#include "elm_naviframe_part.eo.c"

/* Efl.Part end */

/* Internal EO APIs and hidden overrides */

#define ELM_NAVIFRAME_EXTRA_OPS \
   EFL_CANVAS_GROUP_CALC_OPS(elm_naviframe), \
   EFL_CANVAS_GROUP_ADD_DEL_OPS(elm_naviframe)

#include "elm_naviframe_item_eo.c"
#include "elm_naviframe_eo.c"
