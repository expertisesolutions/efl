
int
elm_segment_control_item_count_get(const Elm_Segment_Control *obj)
{
   return elm_obj_segment_control_item_count_get(obj);
}

Elm_Widget_Item *
elm_segment_control_item_selected_get(const Elm_Segment_Control *obj)
{
   return elm_obj_segment_control_item_selected_get(obj);
}

const char *
elm_segment_control_item_label_get(const Elm_Segment_Control *obj, int idx)
{
   return elm_obj_segment_control_item_label_get(obj, idx);
}

Elm_Widget_Item *
elm_segment_control_item_insert_at(Elm_Segment_Control *obj, Efl_Canvas_Object *icon, const char *label, int idx)
{
   return elm_obj_segment_control_item_insert_at(obj, icon, label, idx);
}

Elm_Widget_Item *
elm_segment_control_item_get(const Elm_Segment_Control *obj, int idx)
{
   return elm_obj_segment_control_item_get(obj, idx);
}

void
elm_segment_control_item_del_at(Elm_Segment_Control *obj, int idx)
{
   elm_obj_segment_control_item_del_at(obj, idx);
}

Elm_Widget_Item *
elm_segment_control_item_add(Elm_Segment_Control *obj, Efl_Canvas_Object *icon, const char *label)
{
   return elm_obj_segment_control_item_add(obj, icon, label);
}

Efl_Canvas_Object *
elm_segment_control_item_icon_get(const Elm_Segment_Control *obj, int idx)
{
   return elm_obj_segment_control_item_icon_get(obj, idx);
}
