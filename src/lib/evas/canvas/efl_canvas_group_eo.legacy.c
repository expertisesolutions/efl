
void
evas_object_smart_need_recalculate_set(Efl_Canvas_Group *obj, Eina_Bool value)
{
   efl_canvas_group_need_recalculate_set(obj, value);
}

Eina_Bool
evas_object_smart_need_recalculate_get(const Efl_Canvas_Group *obj)
{
   return efl_canvas_group_need_recalculate_get(obj);
}

void
evas_object_smart_changed(Efl_Canvas_Group *obj)
{
   efl_canvas_group_change(obj);
}

void
evas_object_smart_calculate(Efl_Canvas_Group *obj)
{
   efl_canvas_group_calculate(obj);
}

Eina_Iterator *
evas_object_smart_iterator_new(const Efl_Canvas_Group *obj)
{
   return efl_canvas_group_members_iterate(obj);
}
