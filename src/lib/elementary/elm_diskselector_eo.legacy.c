
ELM_API void
elm_diskselector_side_text_max_length_set(Elm_Diskselector *obj, int len)
{
   elm_obj_diskselector_side_text_max_length_set(obj, len);
}

ELM_API int
elm_diskselector_side_text_max_length_get(const Elm_Diskselector *obj)
{
   return elm_obj_diskselector_side_text_max_length_get(obj);
}

ELM_API void
elm_diskselector_round_enabled_set(Elm_Diskselector *obj, Eina_Bool enabled)
{
   elm_obj_diskselector_round_enabled_set(obj, enabled);
}

ELM_API Eina_Bool
elm_diskselector_round_enabled_get(const Elm_Diskselector *obj)
{
   return elm_obj_diskselector_round_enabled_get(obj);
}

ELM_API void
elm_diskselector_display_item_num_set(Elm_Diskselector *obj, int num)
{
   elm_obj_diskselector_display_item_num_set(obj, num);
}

ELM_API int
elm_diskselector_display_item_num_get(const Elm_Diskselector *obj)
{
   return elm_obj_diskselector_display_item_num_get(obj);
}

ELM_API Elm_Widget_Item *
elm_diskselector_first_item_get(const Elm_Diskselector *obj)
{
   return elm_obj_diskselector_first_item_get(obj);
}

ELM_API const Eina_List *
elm_diskselector_items_get(const Elm_Diskselector *obj)
{
   return elm_obj_diskselector_items_get(obj);
}

ELM_API Elm_Widget_Item *
elm_diskselector_last_item_get(const Elm_Diskselector *obj)
{
   return elm_obj_diskselector_last_item_get(obj);
}

ELM_API Elm_Widget_Item *
elm_diskselector_selected_item_get(const Elm_Diskselector *obj)
{
   return elm_obj_diskselector_selected_item_get(obj);
}

ELM_API Elm_Widget_Item *
elm_diskselector_item_append(Elm_Diskselector *obj, const char *label, Efl_Canvas_Object *icon, Evas_Smart_Cb func, const void *data)
{
   return elm_obj_diskselector_item_append(obj, label, icon, func, data);
}

ELM_API void
elm_diskselector_clear(Elm_Diskselector *obj)
{
   elm_obj_diskselector_clear(obj);
}
