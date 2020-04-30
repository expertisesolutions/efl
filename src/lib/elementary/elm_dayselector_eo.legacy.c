
void
elm_dayselector_week_start_set(Elm_Dayselector *obj, Elm_Dayselector_Day day)
{
   elm_obj_dayselector_week_start_set(obj, day);
}

Elm_Dayselector_Day
elm_dayselector_week_start_get(const Elm_Dayselector *obj)
{
   return elm_obj_dayselector_week_start_get(obj);
}

void
elm_dayselector_weekend_length_set(Elm_Dayselector *obj, unsigned int length)
{
   elm_obj_dayselector_weekend_length_set(obj, length);
}

unsigned int
elm_dayselector_weekend_length_get(const Elm_Dayselector *obj)
{
   return elm_obj_dayselector_weekend_length_get(obj);
}

void
elm_dayselector_weekend_start_set(Elm_Dayselector *obj, Elm_Dayselector_Day day)
{
   elm_obj_dayselector_weekend_start_set(obj, day);
}

Elm_Dayselector_Day
elm_dayselector_weekend_start_get(const Elm_Dayselector *obj)
{
   return elm_obj_dayselector_weekend_start_get(obj);
}

void
elm_dayselector_weekdays_names_set(Elm_Dayselector *obj, const char **weekdays)
{
   elm_obj_dayselector_weekdays_names_set(obj, weekdays);
}

Eina_List *
elm_dayselector_weekdays_names_get(const Elm_Dayselector *obj)
{
   return elm_obj_dayselector_weekdays_names_get(obj);
}

void
elm_dayselector_day_selected_set(Elm_Dayselector *obj, Elm_Dayselector_Day day, Eina_Bool selected)
{
   elm_obj_dayselector_day_selected_set(obj, day, selected);
}

Eina_Bool
elm_dayselector_day_selected_get(const Elm_Dayselector *obj, Elm_Dayselector_Day day)
{
   return elm_obj_dayselector_day_selected_get(obj, day);
}
