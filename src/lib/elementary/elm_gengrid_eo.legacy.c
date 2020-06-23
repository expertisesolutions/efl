
ELM_API void
elm_gengrid_align_set(Elm_Gengrid *obj, double align_x, double align_y)
{
   elm_obj_gengrid_align_set(obj, align_x, align_y);
}

ELM_API void
elm_gengrid_align_get(const Elm_Gengrid *obj, double *align_x, double *align_y)
{
   elm_obj_gengrid_align_get(obj, align_x, align_y);
}

ELM_API void
elm_gengrid_filled_set(Elm_Gengrid *obj, Eina_Bool fill)
{
   elm_obj_gengrid_filled_set(obj, fill);
}

ELM_API Eina_Bool
elm_gengrid_filled_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_filled_get(obj);
}

ELM_API void
elm_gengrid_multi_select_set(Elm_Gengrid *obj, Eina_Bool multi)
{
   elm_obj_gengrid_multi_select_set(obj, multi);
}

ELM_API Eina_Bool
elm_gengrid_multi_select_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_multi_select_get(obj);
}

ELM_API void
elm_gengrid_group_item_size_set(Elm_Gengrid *obj, int w, int h)
{
   elm_obj_gengrid_group_item_size_set(obj, w, h);
}

ELM_API void
elm_gengrid_group_item_size_get(const Elm_Gengrid *obj, int *w, int *h)
{
   elm_obj_gengrid_group_item_size_get(obj, w, h);
}

ELM_API void
elm_gengrid_select_mode_set(Elm_Gengrid *obj, Elm_Object_Select_Mode mode)
{
   elm_obj_gengrid_select_mode_set(obj, mode);
}

ELM_API Elm_Object_Select_Mode
elm_gengrid_select_mode_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_select_mode_get(obj);
}

ELM_API void
elm_gengrid_reorder_mode_set(Elm_Gengrid *obj, Eina_Bool reorder_mode)
{
   elm_obj_gengrid_reorder_mode_set(obj, reorder_mode);
}

ELM_API Eina_Bool
elm_gengrid_reorder_mode_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_reorder_mode_get(obj);
}

ELM_API void
elm_gengrid_highlight_mode_set(Elm_Gengrid *obj, Eina_Bool highlight)
{
   elm_obj_gengrid_highlight_mode_set(obj, highlight);
}

ELM_API Eina_Bool
elm_gengrid_highlight_mode_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_highlight_mode_get(obj);
}

ELM_API void
elm_gengrid_reorder_type_set(Elm_Gengrid *obj, Elm_Gengrid_Reorder_Type type)
{
   elm_obj_gengrid_reorder_type_set(obj, type);
}

ELM_API void
elm_gengrid_item_size_set(Elm_Gengrid *obj, int w, int h)
{
   elm_obj_gengrid_item_size_set(obj, w, h);
}

ELM_API void
elm_gengrid_item_size_get(const Elm_Gengrid *obj, int *w, int *h)
{
   elm_obj_gengrid_item_size_get(obj, w, h);
}

ELM_API void
elm_gengrid_multi_select_mode_set(Elm_Gengrid *obj, Elm_Object_Multi_Select_Mode mode)
{
   elm_obj_gengrid_multi_select_mode_set(obj, mode);
}

ELM_API Elm_Object_Multi_Select_Mode
elm_gengrid_multi_select_mode_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_multi_select_mode_get(obj);
}

ELM_API void
elm_gengrid_horizontal_set(Elm_Gengrid *obj, Eina_Bool horizontal)
{
   elm_obj_gengrid_horizontal_set(obj, horizontal);
}

ELM_API Eina_Bool
elm_gengrid_horizontal_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_horizontal_get(obj);
}

ELM_API Elm_Widget_Item *
elm_gengrid_selected_item_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_selected_item_get(obj);
}

ELM_API Eina_List *
elm_gengrid_realized_items_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_realized_items_get(obj);
}

ELM_API Elm_Widget_Item *
elm_gengrid_first_item_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_first_item_get(obj);
}

ELM_API const Eina_List *
elm_gengrid_selected_items_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_selected_items_get(obj);
}

ELM_API Elm_Widget_Item *
elm_gengrid_last_item_get(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_last_item_get(obj);
}

ELM_API Elm_Widget_Item *
elm_gengrid_item_insert_before(Elm_Gengrid *obj, const Elm_Gengrid_Item_Class *itc, const void *data, Elm_Widget_Item *relative, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_gengrid_item_insert_before(obj, itc, data, relative, func, func_data);
}

ELM_API void
elm_gengrid_realized_items_update(Elm_Gengrid *obj)
{
   elm_obj_gengrid_realized_items_update(obj);
}

ELM_API Elm_Widget_Item *
elm_gengrid_item_insert_after(Elm_Gengrid *obj, const Elm_Gengrid_Item_Class *itc, const void *data, Elm_Widget_Item *relative, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_gengrid_item_insert_after(obj, itc, data, relative, func, func_data);
}

ELM_API unsigned int
elm_gengrid_items_count(const Elm_Gengrid *obj)
{
   return elm_obj_gengrid_items_count(obj);
}

ELM_API Elm_Widget_Item *
elm_gengrid_at_xy_item_get(const Elm_Gengrid *obj, int x, int y, int *xposret, int *yposret)
{
   return elm_obj_gengrid_at_xy_item_get(obj, x, y, xposret, yposret);
}

ELM_API Elm_Widget_Item *
elm_gengrid_item_append(Elm_Gengrid *obj, const Elm_Gengrid_Item_Class *itc, const void *data, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_gengrid_item_append(obj, itc, data, func, func_data);
}

ELM_API Elm_Widget_Item *
elm_gengrid_item_prepend(Elm_Gengrid *obj, const Elm_Gengrid_Item_Class *itc, const void *data, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_gengrid_item_prepend(obj, itc, data, func, func_data);
}

ELM_API void
elm_gengrid_clear(Elm_Gengrid *obj)
{
   elm_obj_gengrid_clear(obj);
}

ELM_API Elm_Widget_Item *
elm_gengrid_item_sorted_insert(Elm_Gengrid *obj, const Elm_Gengrid_Item_Class *itc, const void *data, Eina_Compare_Cb comp, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_gengrid_item_sorted_insert(obj, itc, data, comp, func, func_data);
}

ELM_API Elm_Widget_Item *
elm_gengrid_search_by_text_item_get(Elm_Gengrid *obj, Elm_Widget_Item *item_to_search_from, const char *part_name, const char *pattern, Elm_Glob_Match_Flags flags)
{
   return elm_obj_gengrid_search_by_text_item_get(obj, item_to_search_from, part_name, pattern, flags);
}

ELM_API void
elm_gengrid_reorder_mode_start(Elm_Gengrid *obj, Ecore_Pos_Map tween_mode)
{
   elm_obj_gengrid_reorder_mode_start(obj, tween_mode);
}

ELM_API void
elm_gengrid_reorder_mode_stop(Elm_Gengrid *obj)
{
   elm_obj_gengrid_reorder_mode_stop(obj);
}
