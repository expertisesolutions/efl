
ELM_API void
elm_widget_resize_object_set(Efl_Ui_Widget *obj, Efl_Canvas_Object *sobj)
{
   efl_ui_widget_resize_object_set(obj, sobj);
}

ELM_API void
elm_widget_disabled_set(Efl_Ui_Widget *obj, Eina_Bool disabled)
{
   efl_ui_widget_disabled_set(obj, disabled);
}

ELM_API Eina_Bool
elm_widget_disabled_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_disabled_get(obj);
}

ELM_API Eina_Error
elm_widget_style_set(Efl_Ui_Widget *obj, const char *style)
{
   return efl_ui_widget_style_set(obj, style);
}

ELM_API const char *
elm_widget_style_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_style_get(obj);
}

ELM_API void
elm_widget_can_focus_set(Efl_Ui_Widget *obj, Eina_Bool can_focus)
{
   efl_ui_widget_focus_allow_set(obj, can_focus);
}

ELM_API Eina_Bool
elm_widget_can_focus_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_focus_allow_get(obj);
}

ELM_API void
elm_widget_parent_set(Efl_Ui_Widget *obj, Efl_Ui_Widget *parent)
{
   efl_ui_widget_parent_set(obj, parent);
}

ELM_API Efl_Ui_Widget *
elm_widget_parent_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_parent_get(obj);
}

ELM_API Eina_Bool
elm_widget_sub_object_add(Efl_Ui_Widget *obj, Efl_Canvas_Object *sub_obj)
{
   return efl_ui_widget_sub_object_add(obj, sub_obj);
}

ELM_API Eina_Bool
elm_widget_sub_object_del(Efl_Ui_Widget *obj, Efl_Canvas_Object *sub_obj)
{
   return efl_ui_widget_sub_object_del(obj, sub_obj);
}

ELM_API Eina_Error
elm_widget_theme_apply(Efl_Ui_Widget *obj)
{
   return efl_ui_widget_theme_apply(obj);
}

ELM_API Eina_Rect
elm_widget_focus_region_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_interest_region_get(obj);
}

ELM_API Eina_Rect
elm_widget_focus_highlight_geometry_get(const Efl_Ui_Widget *obj)
{
   return efl_ui_widget_focus_highlight_geometry_get(obj);
}

ELM_API Eina_Bool
elm_widget_focus_state_apply(Efl_Ui_Widget *obj, Efl_Ui_Widget_Focus_State current_state, Efl_Ui_Widget_Focus_State *configured_state, Efl_Ui_Widget *redirect)
{
   return efl_ui_widget_focus_state_apply(obj, current_state, configured_state, redirect);
}
