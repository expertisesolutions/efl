
EVAS_API Evas_Vg_Node *
evas_vg_container_child_get(Evas_Vg_Container *obj, const char *name)
{
   return efl_canvas_vg_container_child_get(obj, name);
}

EVAS_API Eina_Iterator *
evas_vg_container_children_get(Evas_Vg_Container *obj)
{
   return efl_canvas_vg_container_children_get(obj);
}
