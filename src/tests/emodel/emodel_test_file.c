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

struct requeriments {
   int filename;
   int size;
   int properties;
   int propset;
   int count;
   int children;
} requeriments = {0, 0, 0, 0, 0};

static Eina_Bool
_try_quit(void *data EINA_UNUSED)
{
   printf("Try quit\n");
   fail_if (
       requeriments.filename
       && requeriments.size
       && requeriments.properties
//       && requeriments.propset
//       && requeriments.count
//       && requeriments.children == 0
   );
//   ecore_main_loop_quit();

   return EINA_TRUE;
}

static Eina_Bool
_properties_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Eina_Value *properties = event_info;
   const char *prop;
   unsigned int i;

   requeriments.properties = 1;
   for (i = 0; i < eina_value_array_count(properties); i++)
     {
        eina_value_array_get(properties, i, &prop);
        printf("property %d: %s\n", i, prop);
     }

   return EINA_TRUE;
}

static Eina_Bool
_prop_change_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Emodel_Property_EVT *evt = event_info;
   printf("property \"%s\" changed to \"%s\"\n", evt->prop, eina_value_to_string(evt->value));

   if (strcmp(evt->prop, "filename") == 0)
     {
        printf("received Filename\n");
        requeriments.filename = 1;
     }
   else if (strcmp(evt->prop, "size") > 0) //TODO Test all prop stat names
     {
        printf("received size\n");
        requeriments.size = 1;
     }

   return EINA_TRUE;
}

static Eina_Bool
_children_get_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Eo * child = event_info;
   printf("child received\n");
   eo_do(child, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));
   eo_do(child, emodel_property_get("filename"));
}

static Eina_Bool
_children_count_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   unsigned int *len = event_info;

   printf("children count len=%d\n", *len);
   requeriments.children = *len;
   requeriments.count = 1;
}


START_TEST(emodel_test_test_file)
{
   Eo *filemodel;
   Eina_Value *nameset;
//   Ecore_Timer *timer;

   ecore_init();
   filemodel = eo_add_custom(EMODEL_OBJ_EIO_CLASS, NULL, emodel_eio_constructor(EMODEL_TEST_FILENAME_PATH));
   eo_do(filemodel, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));
   eo_do(filemodel, eo_event_callback_add(EMODEL_PROPERTIES_CHANGE_EVT, _properties_cb, NULL));
   eo_do(filemodel, eo_event_callback_add(EMODEL_CHILDREN_GET_EVT, _children_get_cb, NULL));
   eo_do(filemodel, eo_event_callback_add(EMODEL_CHILDREN_COUNT_GET_EVT, _children_count_cb, NULL));
   eo_do(filemodel, emodel_property_get("filename"));
   eo_do(filemodel, emodel_property_get("size"));
   eo_do(filemodel, emodel_properties_get());
   eo_do(filemodel, emodel_children_get());
   eo_do(filemodel, emodel_children_count_get());

   nameset = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(nameset, "/tmp/dir");
//   eo_do(filemodel, emodel_property_set("filename", nameset));

//   timer = ecore_timer_add(0.1, _try_quit, NULL);
//   fail_if(timer == NULL);

   ecore_main_loop_begin();

   eo_unref(filemodel);
}
END_TEST

void
emodel_test_file(TCase *tc)
{
   tcase_add_test(tc, emodel_test_test_file);
}

