
ELM_API Elm_Widget_Item *
elm_menu_selected_item_get(const Elm_Menu *obj)
{
   return elm_obj_menu_selected_item_get(obj);
}

ELM_API Elm_Widget_Item *
elm_menu_first_item_get(const Elm_Menu *obj)
{
   return elm_obj_menu_first_item_get(obj);
}

ELM_API Elm_Widget_Item *
elm_menu_last_item_get(const Elm_Menu *obj)
{
   return elm_obj_menu_last_item_get(obj);
}

ELM_API const Eina_List *
elm_menu_items_get(const Elm_Menu *obj)
{
   return elm_obj_menu_items_get(obj);
}

ELM_API void
elm_menu_move(Elm_Menu *obj, int x, int y)
{
   elm_obj_menu_relative_move(obj, x, y);
}

ELM_API Elm_Widget_Item *
elm_menu_item_add(Elm_Menu *obj, Elm_Widget_Item *parent, const char *icon, const char *label, Evas_Smart_Cb func, const void *data)
{
   return elm_obj_menu_item_add(obj, parent, icon, label, func, data);
}

ELM_API void
elm_menu_open(Elm_Menu *obj)
{
   elm_obj_menu_open(obj);
}

ELM_API void
elm_menu_close(Elm_Menu *obj)
{
   elm_obj_menu_close(obj);
}

ELM_API Elm_Widget_Item *
elm_menu_item_separator_add(Elm_Menu *obj, Elm_Widget_Item *parent)
{
   return elm_obj_menu_item_separator_add(obj, parent);
}
