//Compile with:
//gcc -o bg_example_03 bg_example_03.c -g `pkg-config --cflags --libs elementary`
//where directory is the a path where images/plant_01.jpg can be found.

#include <Elementary.h>

static void
_cb_radio_changed(void *data, Evas_Object *obj, void *event EINA_UNUSED)
{
   Evas_Object *o_bg = data;

   elm_bg_option_set(o_bg, elm_radio_value_get((Evas_Object *)obj));
}

static void
_cb_overlay_changed(void *data, Evas_Object *obj, void *event EINA_UNUSED)
{
   Evas_Object *o_bg = data;

   if (elm_check_state_get(obj))
     {
        Evas_Object *parent, *over;
        char buff[PATH_MAX];

        snprintf(buff, sizeof(buff), "%s/objects/test.edj", elm_app_data_dir_get());
        parent = elm_object_parent_widget_get(o_bg);
        over = edje_object_add(evas_object_evas_get(parent));
        edje_object_file_set(over, buff, "bg_overlay");
        elm_object_part_content_set(o_bg, "overlay", over);
     }
   else
     elm_object_part_content_set(o_bg, "overlay", NULL);
}

static void
_cb_color_changed(void *data, Evas_Object *obj, void *event EINA_UNUSED)
{
   Evas_Object *o_bg = data;
   double val = 0.0;

   val = elm_spinner_value_get(obj);
   if (EINA_DBL_EQ(val, 1.0))
     elm_bg_color_set(o_bg, 255, 255, 255);
   else if (EINA_DBL_EQ(val, 2.0))
     elm_bg_color_set(o_bg, 255, 0, 0);
   else if (EINA_DBL_EQ(val, 3.0))
     elm_bg_color_set(o_bg, 0, 0, 255);
   else if (EINA_DBL_EQ(val, 4.0))
     elm_bg_color_set(o_bg, 0, 255, 0);
}

EAPI_MAIN int
elm_main(int argc EINA_UNUSED, char **argv EINA_UNUSED)
{
   Evas_Object *win;
   Evas_Object *box, *hbox, *o_bg;
   Evas_Object *rd, *rdg;
   char buf[PATH_MAX];

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   elm_app_info_set(elm_main, "elementary", "objects/test.edj");

   win = elm_win_util_standard_add("bg-options", "Bg Options");
   elm_win_autodel_set(win, EINA_TRUE);

   box = elm_box_add(win);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, box);
   evas_object_show(box);

   o_bg = elm_bg_add(win);
   snprintf(buf, sizeof(buf), "%s/images/plant_01.jpg", elm_app_data_dir_get());
   elm_bg_file_set(o_bg, buf, NULL);
   evas_object_size_hint_weight_set(o_bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(o_bg, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, o_bg);
   evas_object_show(o_bg);

   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   evas_object_size_hint_weight_set(hbox, EVAS_HINT_EXPAND, EVAS_HINT_FILL);
   evas_object_size_hint_align_set(hbox, EVAS_HINT_FILL, EVAS_HINT_FILL);

   rd = elm_radio_add(win);
   elm_radio_state_value_set(rd, ELM_BG_OPTION_CENTER);
   elm_object_text_set(rd, "Center");
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_FILL);
   evas_object_smart_callback_add(rd, "changed", _cb_radio_changed, o_bg);
   elm_box_pack_end(hbox, rd);
   evas_object_show(rd);
   rdg = rd;

   rd = elm_radio_add(win);
   elm_radio_state_value_set(rd, ELM_BG_OPTION_SCALE);
   elm_radio_group_add(rd, rdg);
   elm_object_text_set(rd, "Scale");
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_FILL);
   evas_object_smart_callback_add(rd, "changed", _cb_radio_changed, o_bg);
   elm_box_pack_end(hbox, rd);
   evas_object_show(rd);

   rd = elm_radio_add(win);
   elm_radio_state_value_set(rd, ELM_BG_OPTION_STRETCH);
   elm_radio_group_add(rd, rdg);
   elm_object_text_set(rd, "Stretch");
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_FILL);
   evas_object_smart_callback_add(rd, "changed", _cb_radio_changed, o_bg);
   elm_box_pack_end(hbox, rd);
   evas_object_show(rd);

   rd = elm_radio_add(win);
   elm_radio_state_value_set(rd, ELM_BG_OPTION_TILE);
   elm_radio_group_add(rd, rdg);
   elm_object_text_set(rd, "Tile");
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_FILL);
   evas_object_smart_callback_add(rd, "changed", _cb_radio_changed, o_bg);
   elm_box_pack_end(hbox, rd);
   evas_object_show(rd);

   elm_radio_value_set(rdg, ELM_BG_OPTION_SCALE);

   rd = elm_check_add(win);
   elm_object_text_set(rd, "Show Overlay");
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_FILL);
   evas_object_smart_callback_add(rd, "changed", _cb_overlay_changed, o_bg);
   elm_box_pack_end(hbox, rd);
   evas_object_show(rd);

   /* color choices ... this is ghetto, but we don't have a 'colorpicker'
    * widget yet :( */
   rd = elm_spinner_add(win);
   elm_object_style_set(rd, "vertical");
   elm_spinner_min_max_set(rd, 1, 4);
   elm_spinner_label_format_set(rd, "%.0f");
   elm_spinner_editable_set(rd, EINA_FALSE);
   elm_spinner_special_value_add(rd, 1, "White");
   elm_spinner_special_value_add(rd, 2, "Red");
   elm_spinner_special_value_add(rd, 3, "Blue");
   elm_spinner_special_value_add(rd, 4, "Green");
   evas_object_size_hint_weight_set(rd, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(rd, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(rd, "changed", _cb_color_changed, o_bg);
   elm_box_pack_end(hbox, rd);
   evas_object_show(rd);

   elm_box_pack_end(box, hbox);
   evas_object_show(hbox);

   evas_object_resize(win, 460, 320);
   evas_object_show(win);

   elm_run();

   return 0;
}
ELM_MAIN()
