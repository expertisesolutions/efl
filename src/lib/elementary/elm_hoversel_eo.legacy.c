
void
elm_hoversel_horizontal_set(Elm_Hoversel *obj, Eina_Bool horizontal)
{
   elm_obj_hoversel_horizontal_set(obj, horizontal);
}

Eina_Bool
elm_hoversel_horizontal_get(const Elm_Hoversel *obj)
{
   return elm_obj_hoversel_horizontal_get(obj);
}

void
elm_hoversel_hover_parent_set(Elm_Hoversel *obj, Efl_Canvas_Object *parent)
{
   elm_obj_hoversel_hover_parent_set(obj, parent);
}

Efl_Canvas_Object *
elm_hoversel_hover_parent_get(const Elm_Hoversel *obj)
{
   return elm_obj_hoversel_hover_parent_get(obj);
}

Eina_Bool
elm_hoversel_expanded_get(const Elm_Hoversel *obj)
{
   return elm_obj_hoversel_expanded_get(obj);
}

const Eina_List *
elm_hoversel_items_get(const Elm_Hoversel *obj)
{
   return elm_obj_hoversel_items_get(obj);
}

void
elm_hoversel_auto_update_set(Elm_Hoversel *obj, Eina_Bool auto_update)
{
   elm_obj_hoversel_auto_update_set(obj, auto_update);
}

Eina_Bool
elm_hoversel_auto_update_get(const Elm_Hoversel *obj)
{
   return elm_obj_hoversel_auto_update_get(obj);
}

void
elm_hoversel_hover_begin(Elm_Hoversel *obj)
{
   elm_obj_hoversel_hover_begin(obj);
}

void
elm_hoversel_clear(Elm_Hoversel *obj)
{
   elm_obj_hoversel_clear(obj);
}

void
elm_hoversel_hover_end(Elm_Hoversel *obj)
{
   elm_obj_hoversel_hover_end(obj);
}

Elm_Widget_Item *
elm_hoversel_item_add(Elm_Hoversel *obj, const char *label, const char *icon_file, Elm_Icon_Type icon_type, Evas_Smart_Cb func, const void *data)
{
   return elm_obj_hoversel_item_add(obj, label, icon_file, icon_type, func, data);
}
