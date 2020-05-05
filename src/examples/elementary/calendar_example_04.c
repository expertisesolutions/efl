/**
 * Elementary's <b>calendar widget</b> example, regarding date selection.
 * Shows how to disable day selection by user and how to select a date.
 * It selects two days from current day.
 *
 * See stdout/stderr for output. Compile with:
 *
 * @verbatim
 * gcc -o calendar_example_04 calendar_example_04.c -g `pkg-config --cflags --libs elementary`
 * @endverbatim
 */

#include <Eina.h> /* for eina_localtime_r */
#include <Elementary.h>

#define SECS_DAY 86400

EAPI_MAIN int
elm_main(int argc EINA_UNUSED, char **argv EINA_UNUSED)
{
   Evas_Object *win, *bx, *cal, *cal2;
   struct tm selected_time;
   time_t current_time;

   win = elm_win_util_standard_add("calendar", "Calendar Day Selection Example");
   elm_win_autodel_set(win, EINA_TRUE);
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   evas_object_show(bx);

   cal = elm_calendar_add(win);
   evas_object_size_hint_weight_set(cal, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(cal, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_calendar_select_mode_set(cal, ELM_CALENDAR_SELECT_MODE_NONE);
   evas_object_show(cal);
   elm_box_pack_end(bx, cal);

   cal2 = elm_calendar_add(win);
   evas_object_size_hint_weight_set(cal2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(cal2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   current_time = time(NULL) + 2 * SECS_DAY;
   eina_localtime_r(&current_time, &selected_time);
   elm_calendar_selected_time_set(cal2, &selected_time);
   evas_object_show(cal2);
   elm_box_pack_end(bx, cal2);

   evas_object_show(win);

   elm_run();

   return 0;
}
ELM_MAIN()
