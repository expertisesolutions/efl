
void
evas_image_cache_set(Evas_Canvas *obj, int size)
{
   evas_canvas_image_cache_set(obj, size);
}

int
evas_image_cache_get(const Evas_Canvas *obj)
{
   return evas_canvas_image_cache_get(obj);
}

void
evas_event_default_flags_set(Evas_Canvas *obj, Evas_Event_Flags flags)
{
   evas_canvas_event_default_flags_set(obj, (Efl_Input_Flags)flags);
}

Evas_Event_Flags
evas_event_default_flags_get(const Evas_Canvas *obj)
{
   return (Evas_Event_Flags)evas_canvas_event_default_flags_get(obj);
}

void
evas_font_cache_set(Evas_Canvas *obj, int size)
{
   evas_canvas_font_cache_set(obj, size);
}

int
evas_font_cache_get(const Evas_Canvas *obj)
{
   return evas_canvas_font_cache_get(obj);
}

void
evas_data_attach_set(Evas_Canvas *obj, void *data)
{
   evas_canvas_data_attach_set(obj, data);
}

void *
evas_data_attach_get(const Evas_Canvas *obj)
{
   return evas_canvas_data_attach_get(obj);
}

Efl_Canvas_Object *
evas_focus_get(const Evas_Canvas *obj)
{
   return evas_canvas_focus_get(obj);
}

Efl_Canvas_Object *
evas_seat_focus_get(const Evas_Canvas *obj, Evas_Device *seat)
{
   return evas_canvas_seat_focus_get(obj, seat);
}

Efl_Canvas_Object *
evas_object_top_get(const Evas_Canvas *obj)
{
   return evas_canvas_object_top_get(obj);
}

void
evas_pointer_canvas_xy_by_device_get(const Evas_Canvas *obj, Evas_Device *dev, int *x, int *y)
{
   evas_canvas_pointer_canvas_xy_by_device_get(obj, dev, x, y);
}

void
evas_pointer_canvas_xy_get(const Evas_Canvas *obj, int *x, int *y)
{
   evas_canvas_pointer_canvas_xy_get(obj, x, y);
}

int
evas_event_down_count_get(const Evas_Canvas *obj)
{
   return evas_canvas_event_down_count_get(obj);
}

int
evas_smart_objects_calculate_count_get(const Evas_Canvas *obj)
{
   return evas_canvas_smart_objects_calculate_count_get(obj);
}

Eina_Bool
evas_focus_state_get(const Evas_Canvas *obj)
{
   return evas_canvas_focus_state_get(obj);
}

Eina_Bool
evas_seat_focus_state_get(const Evas_Canvas *obj, Evas_Device *seat)
{
   return evas_canvas_seat_focus_state_get(obj, seat);
}

Eina_Bool
evas_changed_get(const Evas_Canvas *obj)
{
   return evas_canvas_changed_get(obj);
}

void
evas_pointer_output_xy_by_device_get(const Evas_Canvas *obj, Evas_Device *dev, int *x, int *y)
{
   evas_canvas_pointer_output_xy_by_device_get(obj, dev, x, y);
}

void
evas_pointer_output_xy_get(const Evas_Canvas *obj, int *x, int *y)
{
   evas_canvas_pointer_output_xy_get(obj, x, y);
}

Efl_Canvas_Object *
evas_object_bottom_get(const Evas_Canvas *obj)
{
   return evas_canvas_object_bottom_get(obj);
}

unsigned int
evas_pointer_button_down_mask_by_device_get(const Evas_Canvas *obj, Evas_Device *dev)
{
   return evas_canvas_pointer_button_down_mask_by_device_get(obj, dev);
}

unsigned int
evas_pointer_button_down_mask_get(const Evas_Canvas *obj)
{
   return evas_canvas_pointer_button_down_mask_get(obj);
}

Eina_List *
evas_tree_objects_at_xy_get(Evas_Canvas *obj, Efl_Canvas_Object *stop, int x, int y)
{
   return evas_canvas_tree_objects_at_xy_get(obj, stop, x, y);
}

void
evas_key_lock_on(Evas_Canvas *obj, const char *keyname)
{
   evas_canvas_key_lock_on(obj, keyname);
}

void
evas_seat_key_lock_on(Evas_Canvas *obj, const char *keyname, Evas_Device *seat)
{
   evas_canvas_seat_key_lock_on(obj, keyname, seat);
}

void
evas_seat_key_lock_off(Evas_Canvas *obj, const char *keyname, Evas_Device *seat)
{
   evas_canvas_seat_key_lock_off(obj, keyname, seat);
}

void
evas_key_modifier_add(Evas_Canvas *obj, const char *keyname)
{
   evas_canvas_key_modifier_add(obj, keyname);
}

void
evas_key_modifier_off(Evas_Canvas *obj, const char *keyname)
{
   evas_canvas_key_modifier_off(obj, keyname);
}

Eina_Bool
evas_render_async(Evas_Canvas *obj)
{
   return evas_canvas_render_async(obj);
}

void
evas_focus_out(Evas_Canvas *obj)
{
   evas_canvas_focus_out(obj);
}

void
evas_norender(Evas_Canvas *obj)
{
   evas_canvas_norender(obj);
}

void
evas_nochange_pop(Evas_Canvas *obj)
{
   evas_canvas_nochange_pop(obj);
}

void
evas_key_lock_off(Evas_Canvas *obj, const char *keyname)
{
   evas_canvas_key_lock_off(obj, keyname);
}

void
evas_nochange_push(Evas_Canvas *obj)
{
   evas_canvas_nochange_push(obj);
}

void
evas_font_cache_flush(Evas_Canvas *obj)
{
   evas_canvas_font_cache_flush(obj);
}

void
evas_key_modifier_on(Evas_Canvas *obj, const char *keyname)
{
   evas_canvas_key_modifier_on(obj, keyname);
}

void
evas_seat_key_modifier_on(Evas_Canvas *obj, const char *keyname, Evas_Device *seat)
{
   evas_canvas_seat_key_modifier_on(obj, keyname, seat);
}

void
evas_seat_key_modifier_off(Evas_Canvas *obj, const char *keyname, Evas_Device *seat)
{
   evas_canvas_seat_key_modifier_off(obj, keyname, seat);
}

Eina_List *
evas_font_available_list(const Evas_Canvas *obj)
{
   return evas_canvas_font_available_list(obj);
}

Efl_Canvas_Object *
evas_object_name_find(const Evas_Canvas *obj, const char *name)
{
   return evas_canvas_object_name_find(obj, name);
}

void
evas_font_path_append(Evas_Canvas *obj, const char *path)
{
   evas_canvas_font_path_append(obj, path);
}

void
evas_font_path_clear(Evas_Canvas *obj)
{
   evas_canvas_font_path_clear(obj);
}

void
evas_key_lock_del(Evas_Canvas *obj, const char *keyname)
{
   evas_canvas_key_lock_del(obj, keyname);
}

void
evas_damage_rectangle_add(Evas_Canvas *obj, int x, int y, int w, int h)
{
   evas_canvas_damage_rectangle_add(obj, x, y, w, h);
}

void
evas_sync(Evas_Canvas *obj)
{
   evas_canvas_sync(obj);
}

const Eina_List *
evas_font_path_list(const Evas_Canvas *obj)
{
   return evas_canvas_font_path_list(obj);
}

void
evas_image_cache_reload(Evas_Canvas *obj)
{
   evas_canvas_image_cache_reload(obj);
}

int
evas_coord_world_x_to_screen(const Evas_Canvas *obj, int x)
{
   return evas_canvas_coord_world_x_to_screen(obj, x);
}

Eina_List *
evas_render_updates(Evas_Canvas *obj)
{
   return evas_canvas_render_updates(obj);
}

void
evas_image_cache_flush(Evas_Canvas *obj)
{
   evas_canvas_image_cache_flush(obj);
}

int
evas_coord_screen_y_to_world(const Evas_Canvas *obj, int y)
{
   return evas_canvas_coord_screen_y_to_world(obj, y);
}

void
evas_key_modifier_del(Evas_Canvas *obj, const char *keyname)
{
   evas_canvas_key_modifier_del(obj, keyname);
}

void
evas_focus_in(Evas_Canvas *obj)
{
   evas_canvas_focus_in(obj);
}

void
evas_obscured_rectangle_add(Evas_Canvas *obj, int x, int y, int w, int h)
{
   evas_canvas_obscured_rectangle_add(obj, x, y, w, h);
}

void
evas_render_dump(Evas_Canvas *obj)
{
   evas_canvas_render_dump(obj);
}

void
evas_render(Evas_Canvas *obj)
{
   evas_canvas_render(obj);
}

void
evas_font_path_prepend(Evas_Canvas *obj, const char *path)
{
   evas_canvas_font_path_prepend(obj, path);
}

void
evas_obscured_clear(Evas_Canvas *obj)
{
   evas_canvas_obscured_clear(obj);
}

int
evas_coord_screen_x_to_world(const Evas_Canvas *obj, int x)
{
   return evas_canvas_coord_screen_x_to_world(obj, x);
}

void
evas_key_lock_add(Evas_Canvas *obj, const char *keyname)
{
   evas_canvas_key_lock_add(obj, keyname);
}

void
evas_render_idle_flush(Evas_Canvas *obj)
{
   evas_canvas_render_idle_flush(obj);
}

Evas_Device *
evas_default_device_get(const Evas_Canvas *obj, Evas_Device_Class type)
{
   return evas_canvas_default_device_get(obj, type);
}

int
evas_coord_world_y_to_screen(const Evas_Canvas *obj, int y)
{
   return evas_canvas_coord_world_y_to_screen(obj, y);
}
