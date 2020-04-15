
void
elm_clock_show_am_pm_set(Elm_Clock *obj, Eina_Bool am_pm)
{
   elm_obj_clock_show_am_pm_set(obj, am_pm);
}

Eina_Bool
elm_clock_show_am_pm_get(const Elm_Clock *obj)
{
   return elm_obj_clock_show_am_pm_get(obj);
}

void
elm_clock_first_interval_set(Elm_Clock *obj, double interval)
{
   elm_obj_clock_first_interval_set(obj, interval);
}

double
elm_clock_first_interval_get(const Elm_Clock *obj)
{
   return elm_obj_clock_first_interval_get(obj);
}

void
elm_clock_show_seconds_set(Elm_Clock *obj, Eina_Bool seconds)
{
   elm_obj_clock_show_seconds_set(obj, seconds);
}

Eina_Bool
elm_clock_show_seconds_get(const Elm_Clock *obj)
{
   return elm_obj_clock_show_seconds_get(obj);
}

void
elm_clock_edit_set(Elm_Clock *obj, Eina_Bool edit)
{
   elm_obj_clock_edit_set(obj, edit);
}

Eina_Bool
elm_clock_edit_get(const Elm_Clock *obj)
{
   return elm_obj_clock_edit_get(obj);
}

void
elm_clock_pause_set(Elm_Clock *obj, Eina_Bool paused)
{
   elm_obj_clock_pause_set(obj, paused);
}

Eina_Bool
elm_clock_pause_get(const Elm_Clock *obj)
{
   return elm_obj_clock_pause_get(obj);
}

void
elm_clock_time_set(Elm_Clock *obj, int hrs, int min, int sec)
{
   elm_obj_clock_time_set(obj, hrs, min, sec);
}

void
elm_clock_time_get(const Elm_Clock *obj, int *hrs, int *min, int *sec)
{
   elm_obj_clock_time_get(obj, hrs, min, sec);
}

void
elm_clock_edit_mode_set(Elm_Clock *obj, Elm_Clock_Edit_Mode digedit)
{
   elm_obj_clock_edit_mode_set(obj, digedit);
}

Elm_Clock_Edit_Mode
elm_clock_edit_mode_get(const Elm_Clock *obj)
{
   return elm_obj_clock_edit_mode_get(obj);
}
