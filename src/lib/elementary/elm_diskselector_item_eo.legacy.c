
ELM_API Elm_Widget_Item *
elm_diskselector_item_prev_get(const Elm_Diskselector_Item *obj)
{
   return elm_obj_diskselector_item_prev_get(obj);
}

ELM_API Elm_Widget_Item *
elm_diskselector_item_next_get(const Elm_Diskselector_Item *obj)
{
   return elm_obj_diskselector_item_next_get(obj);
}

ELM_API void
elm_diskselector_item_selected_set(Elm_Diskselector_Item *obj, Eina_Bool selected)
{
   elm_obj_diskselector_item_selected_set(obj, selected);
}

ELM_API Eina_Bool
elm_diskselector_item_selected_get(const Elm_Diskselector_Item *obj)
{
   return elm_obj_diskselector_item_selected_get(obj);
}
