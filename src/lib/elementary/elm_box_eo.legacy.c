
void
elm_box_homogeneous_set(Elm_Box *obj, Eina_Bool homogeneous)
{
   elm_obj_box_homogeneous_set(obj, homogeneous);
}

Eina_Bool
elm_box_homogeneous_get(const Elm_Box *obj)
{
   return elm_obj_box_homogeneous_get(obj);
}

void
elm_box_align_set(Elm_Box *obj, double horizontal, double vertical)
{
   elm_obj_box_align_set(obj, horizontal, vertical);
}

void
elm_box_align_get(const Elm_Box *obj, double *horizontal, double *vertical)
{
   elm_obj_box_align_get(obj, horizontal, vertical);
}

void
elm_box_horizontal_set(Elm_Box *obj, Eina_Bool horizontal)
{
   elm_obj_box_horizontal_set(obj, horizontal);
}

Eina_Bool
elm_box_horizontal_get(const Elm_Box *obj)
{
   return elm_obj_box_horizontal_get(obj);
}

void
elm_box_padding_set(Elm_Box *obj, int horizontal, int vertical)
{
   elm_obj_box_padding_set(obj, horizontal, vertical);
}

void
elm_box_padding_get(const Elm_Box *obj, int *horizontal, int *vertical)
{
   elm_obj_box_padding_get(obj, horizontal, vertical);
}

Eina_List *
elm_box_children_get(const Elm_Box *obj)
{
   return elm_obj_box_children_get(obj);
}

void
elm_box_pack_end(Elm_Box *obj, Efl_Canvas_Object *subobj)
{
   elm_obj_box_pack_end(obj, subobj);
}

void
elm_box_unpack_all(Elm_Box *obj)
{
   elm_obj_box_unpack_all(obj);
}

void
elm_box_unpack(Elm_Box *obj, Efl_Canvas_Object *subobj)
{
   elm_obj_box_unpack(obj, subobj);
}

void
elm_box_pack_after(Elm_Box *obj, Efl_Canvas_Object *subobj, Efl_Canvas_Object *after)
{
   elm_obj_box_pack_after(obj, subobj, after);
}

void
elm_box_pack_start(Elm_Box *obj, Efl_Canvas_Object *subobj)
{
   elm_obj_box_pack_start(obj, subobj);
}

void
elm_box_recalculate(Elm_Box *obj)
{
   elm_obj_box_recalculate(obj);
}

void
elm_box_pack_before(Elm_Box *obj, Efl_Canvas_Object *subobj, Efl_Canvas_Object *before)
{
   elm_obj_box_pack_before(obj, subobj, before);
}

void
elm_box_clear(Elm_Box *obj)
{
   elm_obj_box_clear(obj);
}
