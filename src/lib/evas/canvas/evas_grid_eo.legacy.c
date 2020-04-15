
void
evas_object_grid_size_set(Evas_Grid *obj, int w, int h)
{
   evas_obj_grid_size_set(obj, w, h);
}

void
evas_object_grid_size_get(const Evas_Grid *obj, int *w, int *h)
{
   evas_obj_grid_size_get(obj, w, h);
}

Eina_List *
evas_object_grid_children_get(const Evas_Grid *obj)
{
   return evas_obj_grid_children_get(obj);
}

Eina_Accessor *
evas_object_grid_accessor_new(const Evas_Grid *obj)
{
   return evas_obj_grid_accessor_new(obj);
}

void
evas_object_grid_clear(Evas_Grid *obj, Eina_Bool clear)
{
   evas_obj_grid_clear(obj, clear);
}

Eina_Iterator *
evas_object_grid_iterator_new(const Evas_Grid *obj)
{
   return evas_obj_grid_iterator_new(obj);
}

Efl_Canvas_Object *
evas_object_grid_add_to(Evas_Grid *obj)
{
   return evas_obj_grid_add_to(obj);
}

Eina_Bool
evas_object_grid_unpack(Evas_Grid *obj, Efl_Canvas_Object *child)
{
   return evas_obj_grid_unpack(obj, child);
}

Eina_Bool
evas_object_grid_pack_get(const Evas_Grid *obj, Efl_Canvas_Object *child, int *x, int *y, int *w, int *h)
{
   return evas_obj_grid_pack_get(obj, child, x, y, w, h);
}

Eina_Bool
evas_object_grid_pack(Evas_Grid *obj, Efl_Canvas_Object *child, int x, int y, int w, int h)
{
   return evas_obj_grid_pack(obj, child, x, y, w, h);
}
