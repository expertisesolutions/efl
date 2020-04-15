
void
elm_slideshow_cache_after_set(Elm_Slideshow *obj, int count)
{
   elm_obj_slideshow_cache_after_set(obj, count);
}

int
elm_slideshow_cache_after_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_cache_after_get(obj);
}

void
elm_slideshow_cache_before_set(Elm_Slideshow *obj, int count)
{
   elm_obj_slideshow_cache_before_set(obj, count);
}

int
elm_slideshow_cache_before_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_cache_before_get(obj);
}

void
elm_slideshow_layout_set(Elm_Slideshow *obj, const char *layout)
{
   elm_obj_slideshow_layout_set(obj, layout);
}

const char *
elm_slideshow_layout_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_layout_get(obj);
}

void
elm_slideshow_transition_set(Elm_Slideshow *obj, const char *transition)
{
   elm_obj_slideshow_transition_set(obj, transition);
}

const char *
elm_slideshow_transition_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_transition_get(obj);
}

void
elm_slideshow_loop_set(Elm_Slideshow *obj, Eina_Bool loop)
{
   elm_obj_slideshow_items_loop_set(obj, loop);
}

Eina_Bool
elm_slideshow_loop_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_items_loop_get(obj);
}

void
elm_slideshow_timeout_set(Elm_Slideshow *obj, double timeout)
{
   elm_obj_slideshow_timeout_set(obj, timeout);
}

double
elm_slideshow_timeout_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_timeout_get(obj);
}

const Eina_List *
elm_slideshow_items_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_items_get(obj);
}

const Eina_List *
elm_slideshow_transitions_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_transitions_get(obj);
}

unsigned int
elm_slideshow_count_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_count_get(obj);
}

Elm_Widget_Item *
elm_slideshow_item_current_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_item_current_get(obj);
}

const Eina_List *
elm_slideshow_layouts_get(const Elm_Slideshow *obj)
{
   return elm_obj_slideshow_layouts_get(obj);
}

void
elm_slideshow_previous(Elm_Slideshow *obj)
{
   elm_obj_slideshow_previous(obj);
}

Elm_Widget_Item *
elm_slideshow_item_nth_get(const Elm_Slideshow *obj, unsigned int nth)
{
   return elm_obj_slideshow_item_nth_get(obj, nth);
}

void
elm_slideshow_next(Elm_Slideshow *obj)
{
   elm_obj_slideshow_next(obj);
}

void
elm_slideshow_clear(Elm_Slideshow *obj)
{
   elm_obj_slideshow_clear(obj);
}

Elm_Widget_Item *
elm_slideshow_item_add(Elm_Slideshow *obj, const Elm_Slideshow_Item_Class *itc, const void *data)
{
   return elm_obj_slideshow_item_add(obj, itc, data);
}

Elm_Widget_Item *
elm_slideshow_item_sorted_insert(Elm_Slideshow *obj, const Elm_Slideshow_Item_Class *itc, const void *data, Eina_Compare_Cb func)
{
   return elm_obj_slideshow_item_sorted_insert(obj, itc, data, func);
}
