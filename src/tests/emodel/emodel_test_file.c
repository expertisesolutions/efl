//Compile with:
// gcc -o emodel_file_example emodel_file_example.c `pkg-config --cflags --libs emodel`

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Eo.h>
#include <Ecore.h>
#include <Emodel.h>
#include <emodel_eio.h>
#include <stdio.h>

#include <check.h>

#define EMODEL_TEST_FILENAME_PATH "/tmp"

static Eina_Bool
_prop_change_cb(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   static int i = 0;
   Emodel_Property_EVT *evt = event_info;
   i++;
   printf("property %s changed to %s\n", evt->prop, eina_value_to_string(evt->value));
   if (i > 3)
     ecore_main_loop_quit();

   return EINA_TRUE;
}

START_TEST(emodel_test_test_file)
{
   Eo *filemodel;

   ecore_init();
   filemodel = eo_add_custon(EMODEL_OBJ_EIO_CLASS, NULL, emodel_eio_constructor(EMODEL_TEST_FILENAME_PATH));
   eo_do(filemodel, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));
   eo_do(filemodel, emodel_property_get("filename"));

   ecore_main_loop_begin();
   eo_unref(filemodel);
}
END_TEST

void
emodel_test_file(TCase *tc)
{
   tcase_add_test(tc, emodel_test_test_file);
}

