
void
elm_video_remember_position_set(Efl_Ui_Video *obj, Eina_Bool remember)
{
   efl_ui_video_remember_position_set(obj, remember);
}

Eina_Bool
elm_video_remember_position_get(const Efl_Ui_Video *obj)
{
   return efl_ui_video_remember_position_get(obj);
}

Efl_Canvas_Object *
elm_video_emotion_get(const Efl_Ui_Video *obj)
{
   return efl_ui_video_emotion_get(obj);
}

const char *
elm_video_title_get(const Efl_Ui_Video *obj)
{
   return efl_ui_video_title_get(obj);
}
