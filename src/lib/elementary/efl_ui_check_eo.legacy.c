
void
elm_check_selected_set(Efl_Ui_Check *obj, Eina_Bool value)
{
   efl_ui_selectable_selected_set(obj, value);
}

Eina_Bool
elm_check_selected_get(const Efl_Ui_Check *obj)
{
   return efl_ui_selectable_selected_get(obj);
}
