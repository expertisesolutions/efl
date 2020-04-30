
void
elm_flip_interaction_set(Elm_Flip *obj, Elm_Flip_Interaction mode)
{
   efl_ui_flip_interaction_set(obj, mode);
}

Elm_Flip_Interaction
elm_flip_interaction_get(const Elm_Flip *obj)
{
   return efl_ui_flip_interaction_get(obj);
}

Eina_Bool
elm_flip_front_visible_get(const Elm_Flip *obj)
{
   return efl_ui_flip_front_visible_get(obj);
}

void
elm_flip_go(Elm_Flip *obj, Elm_Flip_Mode mode)
{
   efl_ui_flip_go(obj, mode);
}

void
elm_flip_go_to(Elm_Flip *obj, Eina_Bool front, Elm_Flip_Mode mode)
{
   efl_ui_flip_go_to(obj, front, mode);
}
