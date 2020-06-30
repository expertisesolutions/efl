
ELM_API Elm_Widget_Item *
elm_toolbar_item_prev_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_prev_get(obj);
}

ELM_API Elm_Widget_Item *
elm_toolbar_item_next_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_next_get(obj);
}

ELM_API void
elm_toolbar_item_selected_set(Elm_Toolbar_Item *obj, Eina_Bool selected)
{
   elm_obj_toolbar_item_selected_set(obj, selected);
}

ELM_API Eina_Bool
elm_toolbar_item_selected_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_selected_get(obj);
}

ELM_API void
elm_toolbar_item_priority_set(Elm_Toolbar_Item *obj, int priority)
{
   elm_obj_toolbar_item_priority_set(obj, priority);
}

ELM_API int
elm_toolbar_item_priority_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_priority_get(obj);
}

ELM_API void
elm_toolbar_item_icon_set(Elm_Toolbar_Item *obj, const char *icon)
{
   elm_obj_toolbar_item_icon_set(obj, icon);
}

ELM_API const char *
elm_toolbar_item_icon_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_icon_get(obj);
}

ELM_API Efl_Canvas_Object *
elm_toolbar_item_object_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_object_get(obj);
}

ELM_API Efl_Canvas_Object *
elm_toolbar_item_icon_object_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_icon_object_get(obj);
}

ELM_API void
elm_toolbar_item_separator_set(Elm_Toolbar_Item *obj, Eina_Bool separator)
{
   elm_obj_toolbar_item_separator_set(obj, separator);
}

ELM_API Eina_Bool
elm_toolbar_item_separator_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_separator_get(obj);
}

ELM_API Efl_Canvas_Object *
elm_toolbar_item_menu_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_menu_get(obj);
}

ELM_API Eina_Bool
elm_toolbar_item_state_set(Elm_Toolbar_Item *obj, Elm_Toolbar_Item_State *state)
{
   return elm_obj_toolbar_item_state_set(obj, state);
}

ELM_API Elm_Toolbar_Item_State *
elm_toolbar_item_state_get(const Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_state_get(obj);
}

ELM_API Eina_Bool
elm_toolbar_item_icon_memfile_set(Elm_Toolbar_Item *obj, const void *img, size_t size, const char *format, const char *key)
{
   return elm_obj_toolbar_item_icon_memfile_set(obj, img, size, format, key);
}

ELM_API Eina_Bool
elm_toolbar_item_icon_file_set(Elm_Toolbar_Item *obj, const char *file, const char *key)
{
   return elm_obj_toolbar_item_icon_file_set(obj, file, key);
}

ELM_API Elm_Toolbar_Item_State *
elm_toolbar_item_state_add(Elm_Toolbar_Item *obj, const char *icon, const char *label, Evas_Smart_Cb func, const void *data)
{
   return elm_obj_toolbar_item_state_add(obj, icon, label, func, data);
}

ELM_API Eina_Bool
elm_toolbar_item_state_del(Elm_Toolbar_Item *obj, Elm_Toolbar_Item_State *state)
{
   return elm_obj_toolbar_item_state_del(obj, state);
}

ELM_API Elm_Toolbar_Item_State *
elm_toolbar_item_state_next(Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_state_next(obj);
}

ELM_API Elm_Toolbar_Item_State *
elm_toolbar_item_state_prev(Elm_Toolbar_Item *obj)
{
   return elm_obj_toolbar_item_state_prev(obj);
}

ELM_API void
elm_toolbar_item_show(Elm_Toolbar_Item *obj, Elm_Toolbar_Item_Scrollto_Type scrollto)
{
   elm_obj_toolbar_item_show(obj, scrollto);
}

ELM_API void
elm_toolbar_item_bring_in(Elm_Toolbar_Item *obj, Elm_Toolbar_Item_Scrollto_Type scrollto)
{
   elm_obj_toolbar_item_bring_in(obj, scrollto);
}

ELM_API void
elm_toolbar_item_menu_set(Elm_Toolbar_Item *obj, Eina_Bool menu)
{
   elm_obj_toolbar_item_menu_set(obj, menu);
}
