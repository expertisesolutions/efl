
void
elm_list_item_separator_set(Elm_List_Item *obj, Eina_Bool setting)
{
   elm_obj_list_item_separator_set(obj, setting);
}

Eina_Bool
elm_list_item_separator_get(const Elm_List_Item *obj)
{
   return elm_obj_list_item_separator_get(obj);
}

void
elm_list_item_selected_set(Elm_List_Item *obj, Eina_Bool selected)
{
   elm_obj_list_item_selected_set(obj, selected);
}

Eina_Bool
elm_list_item_selected_get(const Elm_List_Item *obj)
{
   return elm_obj_list_item_selected_get(obj);
}

Efl_Canvas_Object *
elm_list_item_object_get(const Elm_List_Item *obj)
{
   return elm_obj_list_item_object_get(obj);
}

Elm_Widget_Item *
elm_list_item_prev(const Elm_List_Item *obj)
{
   return elm_obj_list_item_prev_get(obj);
}

Elm_Widget_Item *
elm_list_item_next(const Elm_List_Item *obj)
{
   return elm_obj_list_item_next_get(obj);
}

void
elm_list_item_show(Elm_List_Item *obj)
{
   elm_obj_list_item_show(obj);
}

void
elm_list_item_bring_in(Elm_List_Item *obj)
{
   elm_obj_list_item_bring_in(obj);
}
