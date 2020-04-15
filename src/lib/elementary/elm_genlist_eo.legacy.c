
void
elm_genlist_homogeneous_set(Elm_Genlist *obj, Eina_Bool homogeneous)
{
   elm_obj_genlist_homogeneous_set(obj, homogeneous);
}

Eina_Bool
elm_genlist_homogeneous_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_homogeneous_get(obj);
}

void
elm_genlist_select_mode_set(Elm_Genlist *obj, Elm_Object_Select_Mode mode)
{
   elm_obj_genlist_select_mode_set(obj, mode);
}

Elm_Object_Select_Mode
elm_genlist_select_mode_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_select_mode_get(obj);
}

void
elm_genlist_focus_on_selection_set(Elm_Genlist *obj, Eina_Bool enabled)
{
   elm_obj_genlist_focus_on_selection_set(obj, enabled);
}

Eina_Bool
elm_genlist_focus_on_selection_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_focus_on_selection_get(obj);
}

void
elm_genlist_longpress_timeout_set(Elm_Genlist *obj, double timeout)
{
   elm_obj_genlist_longpress_timeout_set(obj, timeout);
}

double
elm_genlist_longpress_timeout_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_longpress_timeout_get(obj);
}

void
elm_genlist_multi_select_set(Elm_Genlist *obj, Eina_Bool multi)
{
   elm_obj_genlist_multi_select_set(obj, multi);
}

Eina_Bool
elm_genlist_multi_select_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_multi_select_get(obj);
}

void
elm_genlist_reorder_mode_set(Elm_Genlist *obj, Eina_Bool reorder_mode)
{
   elm_obj_genlist_reorder_mode_set(obj, reorder_mode);
}

Eina_Bool
elm_genlist_reorder_mode_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_reorder_mode_get(obj);
}

void
elm_genlist_decorate_mode_set(Elm_Genlist *obj, Eina_Bool decorated)
{
   elm_obj_genlist_decorate_mode_set(obj, decorated);
}

Eina_Bool
elm_genlist_decorate_mode_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_decorate_mode_get(obj);
}

void
elm_genlist_multi_select_mode_set(Elm_Genlist *obj, Elm_Object_Multi_Select_Mode mode)
{
   elm_obj_genlist_multi_select_mode_set(obj, mode);
}

Elm_Object_Multi_Select_Mode
elm_genlist_multi_select_mode_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_multi_select_mode_get(obj);
}

void
elm_genlist_block_count_set(Elm_Genlist *obj, int count)
{
   elm_obj_genlist_block_count_set(obj, count);
}

int
elm_genlist_block_count_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_block_count_get(obj);
}

void
elm_genlist_tree_effect_enabled_set(Elm_Genlist *obj, Eina_Bool enabled)
{
   elm_obj_genlist_tree_effect_enabled_set(obj, enabled);
}

Eina_Bool
elm_genlist_tree_effect_enabled_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_tree_effect_enabled_get(obj);
}

void
elm_genlist_highlight_mode_set(Elm_Genlist *obj, Eina_Bool highlight)
{
   elm_obj_genlist_highlight_mode_set(obj, highlight);
}

Eina_Bool
elm_genlist_highlight_mode_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_highlight_mode_get(obj);
}

void
elm_genlist_mode_set(Elm_Genlist *obj, Elm_List_Mode mode)
{
   elm_obj_genlist_mode_set(obj, mode);
}

Elm_List_Mode
elm_genlist_mode_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_mode_get(obj);
}

Elm_Widget_Item *
elm_genlist_decorated_item_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_decorated_item_get(obj);
}

Elm_Widget_Item *
elm_genlist_selected_item_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_selected_item_get(obj);
}

Elm_Widget_Item *
elm_genlist_first_item_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_first_item_get(obj);
}

Eina_List *
elm_genlist_realized_items_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_realized_items_get(obj);
}

const Eina_List *
elm_genlist_selected_items_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_selected_items_get(obj);
}

Elm_Widget_Item *
elm_genlist_last_item_get(const Elm_Genlist *obj)
{
   return elm_obj_genlist_last_item_get(obj);
}

Elm_Widget_Item *
elm_genlist_item_insert_before(Elm_Genlist *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Widget_Item *parent, Elm_Widget_Item *before_it, Elm_Genlist_Item_Type type, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_genlist_item_insert_before(obj, itc, data, parent, before_it, type, func, func_data);
}

void
elm_genlist_realized_items_update(Elm_Genlist *obj)
{
   elm_obj_genlist_realized_items_update(obj);
}

Elm_Widget_Item *
elm_genlist_item_insert_after(Elm_Genlist *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Widget_Item *parent, Elm_Widget_Item *after_it, Elm_Genlist_Item_Type type, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_genlist_item_insert_after(obj, itc, data, parent, after_it, type, func, func_data);
}

Elm_Widget_Item *
elm_genlist_at_xy_item_get(const Elm_Genlist *obj, int x, int y, int *posret)
{
   return elm_obj_genlist_at_xy_item_get(obj, x, y, posret);
}

void
elm_genlist_filter_set(Elm_Genlist *obj, void *key)
{
   elm_obj_genlist_filter_set(obj, key);
}

Eina_Iterator *
elm_genlist_filter_iterator_new(Elm_Genlist *obj)
{
   return elm_obj_genlist_filter_iterator_new(obj);
}

unsigned int
elm_genlist_filtered_items_count(const Elm_Genlist *obj)
{
   return elm_obj_genlist_filtered_items_count(obj);
}

unsigned int
elm_genlist_items_count(const Elm_Genlist *obj)
{
   return elm_obj_genlist_items_count(obj);
}

Elm_Widget_Item *
elm_genlist_item_prepend(Elm_Genlist *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Widget_Item *parent, Elm_Genlist_Item_Type type, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_genlist_item_prepend(obj, itc, data, parent, type, func, func_data);
}

void
elm_genlist_clear(Elm_Genlist *obj)
{
   elm_obj_genlist_clear(obj);
}

Elm_Widget_Item *
elm_genlist_item_append(Elm_Genlist *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Widget_Item *parent, Elm_Genlist_Item_Type type, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_genlist_item_append(obj, itc, data, parent, type, func, func_data);
}

Elm_Widget_Item *
elm_genlist_item_sorted_insert(Elm_Genlist *obj, const Elm_Genlist_Item_Class *itc, const void *data, Elm_Widget_Item *parent, Elm_Genlist_Item_Type type, Eina_Compare_Cb comp, Evas_Smart_Cb func, const void *func_data)
{
   return elm_obj_genlist_item_sorted_insert(obj, itc, data, parent, type, comp, func, func_data);
}

Elm_Widget_Item *
elm_genlist_search_by_text_item_get(Elm_Genlist *obj, Elm_Widget_Item *item_to_search_from, const char *part_name, const char *pattern, Elm_Glob_Match_Flags flags)
{
   return elm_obj_genlist_search_by_text_item_get(obj, item_to_search_from, part_name, pattern, flags);
}
