
EDJE_API void
edje_object_animation_set(Efl_Canvas_Layout *obj, Eina_Bool on)
{
   efl_canvas_layout_animated_set(obj, on);
}

EDJE_API Eina_Bool
edje_object_animation_get(const Efl_Canvas_Layout *obj)
{
   return efl_canvas_layout_animated_get(obj);
}

EDJE_API Efl_Input_Device *
edje_object_seat_get(const Efl_Canvas_Layout *obj, Eina_Stringshare *name)
{
   return efl_canvas_layout_seat_get(obj, name);
}

EDJE_API Eina_Stringshare *
edje_object_seat_name_get(const Efl_Canvas_Layout *obj, Efl_Input_Device *device)
{
   return efl_canvas_layout_seat_name_get(obj, device);
}

EDJE_API Eina_Error
edje_object_layout_load_error_get(const Efl_Canvas_Layout *obj)
{
   return efl_canvas_layout_load_error_get(obj);
}

EDJE_API Eina_Bool
edje_object_content_remove(Efl_Canvas_Layout *obj, Efl_Gfx_Entity *content)
{
   return efl_canvas_layout_content_remove(obj, content);
}
