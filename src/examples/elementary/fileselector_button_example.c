/**
 * Simple Elementary's <b>file selector button widget</b> example,
 * illustrating its usage and API.
 *
 * See stdout/stderr for output. Compile with:
 *
 * @verbatim
 * gcc -g file selector_button_example.c -o file selector_button_example `pkg-config --cflags --libs elementary`
 * @endverbatim
 */

#include <Elementary.h>

static void /* hook on the sole smart callback */
_file_chosen(void *data, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Evas_Object *entry = data;
   const char *file = event_info;
   if (file)
     {
        elm_object_text_set(entry, file);
        printf("File chosen: %s\n", file);
     }
   else
     printf("File selection canceled.\n");
}

/* toggle inwin mode */
static void
_inwin_mode_toggle(void *data, Evas_Object *obj EINA_UNUSED,
                   void *event_info EINA_UNUSED)
{
   Evas_Object *fs_bt = data;
   Eina_Bool old_val = elm_fileselector_button_inwin_mode_get(fs_bt);

   elm_fileselector_button_inwin_mode_set(fs_bt, !old_val);
   printf("Inwin mode set to: %s\n", old_val ? "false" : "true");
}

static void
_current_sel_toggle(void *data, Evas_Object *obj EINA_UNUSED,
                    void *event_info EINA_UNUSED)
{
   Evas_Object *fs_bt = data;
   Eina_Bool old_val = elm_fileselector_is_save_get(fs_bt);
   elm_fileselector_is_save_set(fs_bt, !old_val);

   printf("%s text entry with selected item's name\n",
          old_val ? "Disabling" : "Enabling");
}

static void
_folder_only_toggle(void *data,
                    Evas_Object *obj EINA_UNUSED,
                    void *event_info EINA_UNUSED)
{
   Evas_Object *fs_bt = data;
   Eina_Bool old_val = elm_fileselector_folder_only_get(fs_bt);
   elm_fileselector_folder_only_set(fs_bt, !old_val);

   printf("Folder only mode set to: %s\n", old_val ? "false" : "true");
}

static void
_expandable_toggle(void *data, Evas_Object *obj EINA_UNUSED,
                   void *event_info EINA_UNUSED)
{
   Evas_Object *fs_bt = data;
   Eina_Bool old_val = elm_fileselector_expandable_get(fs_bt);
   elm_fileselector_expandable_set(fs_bt, !old_val);

   printf("Expandable folders mode set to: %s\n", old_val ? "false" : "true");
}

EAPI_MAIN int
elm_main(int argc EINA_UNUSED, char **argv EINA_UNUSED)
{
   Evas_Object *win, *vbox, *hbox, *ic, *ck, *fs_bt, *en, *lb, *sep;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

   win = elm_win_util_standard_add("fileselector-button", "File Selector Button Example");
   elm_win_autodel_set(win, EINA_TRUE);

   vbox = elm_box_add(win);
   evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, vbox);
   evas_object_show(vbox);

   ic = elm_icon_add(win);
   elm_icon_standard_set(ic, "file");
   evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

   /* file selector button */
   fs_bt = elm_fileselector_button_add(win);
   elm_fileselector_path_set(fs_bt, eina_environment_tmp_get());
   elm_object_text_set(fs_bt, "Select a file");
   elm_object_part_content_set(fs_bt, "icon", ic);

   elm_box_pack_end(vbox, fs_bt);
   evas_object_show(fs_bt);
   evas_object_show(ic);

   /* attribute setting knobs */
   sep = elm_separator_add(win);
   elm_separator_horizontal_set(sep, EINA_TRUE);
   elm_box_pack_end(vbox, sep);
   evas_object_show(sep);

   hbox = elm_box_add(win);
   elm_box_horizontal_set(hbox, EINA_TRUE);
   elm_box_pack_end(vbox, hbox);
   evas_object_show(hbox);

   ck = elm_check_add(win);
   elm_object_text_set(ck, "editable selection");
   elm_check_state_set(ck, elm_fileselector_is_save_get(fs_bt));
   evas_object_smart_callback_add(ck, "changed", _current_sel_toggle, fs_bt);
   elm_box_pack_end(hbox, ck);
   evas_object_show(ck);

   ck = elm_check_add(win);
   elm_object_text_set(ck, "\"inwin\" mode");
   elm_check_state_set(ck, elm_fileselector_button_inwin_mode_get(fs_bt));
   evas_object_smart_callback_add(ck, "changed", _inwin_mode_toggle, fs_bt);
   elm_box_pack_end(hbox, ck);
   evas_object_show(ck);

   ck = elm_check_add(win);
   elm_object_text_set(ck, "folders only");
   elm_check_state_set(ck, elm_fileselector_folder_only_get(fs_bt));
   evas_object_smart_callback_add(ck, "changed", _folder_only_toggle, fs_bt);
   elm_box_pack_end(hbox, ck);
   evas_object_show(ck);

   ck = elm_check_add(win);
   elm_object_text_set(ck, "expandable");
   elm_check_state_set(ck, elm_fileselector_expandable_get(fs_bt));
   evas_object_smart_callback_add(ck, "changed", _expandable_toggle, fs_bt);
   elm_box_pack_end(hbox, ck);
   evas_object_show(ck);

   lb = elm_label_add(win);
   elm_object_text_set(lb, "Last selection:");
   elm_box_pack_end(vbox, lb);
   evas_object_show(lb);

   en = elm_entry_add(win);
   elm_entry_line_wrap_set(en, EINA_FALSE);
   elm_entry_editable_set(en, EINA_FALSE);
   evas_object_smart_callback_add(fs_bt, "file,chosen", _file_chosen, en);
   elm_box_pack_end(vbox, en);
   evas_object_show(en);

   evas_object_resize(win, 400, 400);
   evas_object_show(win);

   elm_run();

   return 0;
}
ELM_MAIN()
