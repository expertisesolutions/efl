
void
elm_menu_item_icon_name_set(Elm_Menu_Item *obj, const char *icon)
{
   elm_obj_menu_item_icon_name_set(obj, icon);
}

const char *
elm_menu_item_icon_name_get(const Elm_Menu_Item *obj)
{
   return elm_obj_menu_item_icon_name_get(obj);
}

Elm_Widget_Item *
elm_menu_item_prev_get(const Elm_Menu_Item *obj)
{
   return elm_obj_menu_item_prev_get(obj);
}

Elm_Widget_Item *
elm_menu_item_next_get(const Elm_Menu_Item *obj)
{
   return elm_obj_menu_item_next_get(obj);
}

void
elm_menu_item_selected_set(Elm_Menu_Item *obj, Eina_Bool selected)
{
   elm_obj_menu_item_selected_set(obj, selected);
}

Eina_Bool
elm_menu_item_selected_get(const Elm_Menu_Item *obj)
{
   return elm_obj_menu_item_selected_get(obj);
}

unsigned int
elm_menu_item_index_get(const Elm_Menu_Item *obj)
{
   return elm_obj_menu_item_index_get(obj);
}

void
elm_menu_item_subitems_clear(Elm_Menu_Item *obj)
{
   elm_obj_menu_item_subitems_clear(obj);
}

const Eina_List *
elm_menu_item_subitems_get(const Elm_Menu_Item *obj)
{
   return elm_obj_menu_item_subitems_get(obj);
}

Eina_Bool
elm_menu_item_is_separator(const Elm_Menu_Item *obj)
{
   return elm_obj_menu_item_is_separator(obj);
}

Efl_Canvas_Object *
elm_menu_item_object_get(const Elm_Menu_Item *obj)
{
   return elm_obj_menu_item_object_get(obj);
}
