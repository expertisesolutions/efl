
void
elm_panel_orient_set(Elm_Panel *obj, Elm_Panel_Orient orient)
{
   elm_obj_panel_orient_set(obj, orient);
}

Elm_Panel_Orient
elm_panel_orient_get(const Elm_Panel *obj)
{
   return elm_obj_panel_orient_get(obj);
}

void
elm_panel_hidden_set(Elm_Panel *obj, Eina_Bool hidden)
{
   elm_obj_panel_hidden_set(obj, hidden);
}

Eina_Bool
elm_panel_hidden_get(const Elm_Panel *obj)
{
   return elm_obj_panel_hidden_get(obj);
}

void
elm_panel_scrollable_set(Elm_Panel *obj, Eina_Bool scrollable)
{
   elm_obj_panel_scrollable_set(obj, scrollable);
}

Eina_Bool
elm_panel_scrollable_get(const Elm_Panel *obj)
{
   return elm_obj_panel_scrollable_get(obj);
}

void
elm_panel_scrollable_content_size_set(Elm_Panel *obj, double ratio)
{
   elm_obj_panel_scrollable_content_size_set(obj, ratio);
}

double
elm_panel_scrollable_content_size_get(const Elm_Panel *obj)
{
   return elm_obj_panel_scrollable_content_size_get(obj);
}

void
elm_panel_toggle(Elm_Panel *obj)
{
   elm_obj_panel_toggle(obj);
}
