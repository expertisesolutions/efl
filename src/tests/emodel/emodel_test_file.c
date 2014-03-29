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
__attribute__((unused))_try_quit(void *data EINA_UNUSED)
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
        fprintf(stdout, "property %d: %s\n", i, prop);
     }

   return EINA_TRUE;
}

static Eina_Bool
_prop_change_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Emodel_Property_EVT *evt = event_info;
   fprintf(stdout, "property \"%s\" changed to \"%s\"\n", evt->prop, eina_value_to_string(evt->value));

   if (strncmp(evt->prop, "filename", strlen("filename")) == 0)
     {
        fprintf(stdout, "received Filename\n");
        requeriments.filename = 1;
     }
   else if (strncmp(evt->prop, "size", strlen("size")) == 0) //TODO Test all prop stat names
     {
        fprintf(stdout, "received Size\n");
        requeriments.size = 1;
     }

   return EINA_TRUE;
}
   
static Eina_Bool
_children_get_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Emodel_Children_EVT *evt = (Emodel_Children_EVT*)event_info;
   fprintf(stdout, "child received: child=%p, idx=%d\n", evt->child, evt->idx);
   eo_do(evt->child, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));
   eo_do(evt->child, emodel_property_get("filename"));
   return EINA_TRUE;
}

static Eina_Bool
_children_count_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   unsigned int *len = event_info;

   fprintf(stdout, "children count len=%d\n", *len);
   requeriments.children = *len;
   requeriments.count = 1;
   return EINA_TRUE;
}

static Eina_Bool
_child_add_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   //Eo * child = event_info;
   const char *s = (const char *)event_info;
   fprintf(stdout, "child added:%s\n", s);
   return EINA_TRUE;
}

static void
_emodel_child_add_cb(void *data, Eo *child EINA_UNUSED, void *event_info EINA_UNUSED)
{
   const char *s = (const char *)data;
   fprintf(stdout, "my child added:%s\n", s);
}


START_TEST(emodel_test_test_file)
{
   Eo *filemodel;
   //Eina_Value *nameset;
   static const char *dirs[] = {"emodel_test_dir_00", "emodel_test_dir_01", "emodel_test_dir_02"};
   //Ecore_Timer *timer;

   ecore_init();


   filemodel = eo_add_custom(EMODEL_OBJ_EIO_CLASS, NULL, emodel_eio_constructor(EMODEL_TEST_FILENAME_PATH));
   eo_do(filemodel, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));
   eo_do(filemodel, eo_event_callback_add(EMODEL_PROPERTIES_CHANGE_EVT, _properties_cb, NULL));
   eo_do(filemodel, eo_event_callback_add(EMODEL_CHILDREN_GET_EVT, _children_get_cb, NULL));
   eo_do(filemodel, eo_event_callback_add(EMODEL_CHILDREN_COUNT_GET_EVT, _children_count_cb, NULL));
   eo_do(filemodel, eo_event_callback_add(EMODEL_CHILD_ADD_EVT, _child_add_cb, NULL));

   eo_do(filemodel, emodel_property_get("filename"));
   eo_do(filemodel, emodel_property_get("size"));
   eo_do(filemodel, emodel_properties_get());
   
   
   //eo_do(filemodel, emodel_children_get());
   
   
   eo_do(filemodel, emodel_children_count_get());
   eo_do(filemodel, emodel_children_slice_get(0,2));
   eo_do(filemodel, emodel_children_slice_get(10,6));
   //eo_do(filemodel, emodel_children_slice_get(10,5));
   //eo_do(filemodel, emodel_children_slice_get(15,30));
   //eo_do(filemodel, emodel_children_slice_get(20,25));


   //eo_do(filemodel, emodel_eio_child_add(_emodel_child_add_cb, NULL, dirs[0], EMODEL_EIO_FILE_TYPE_DIR));
   //eo_do(filemodel, emodel_eio_child_add(_emodel_child_add_cb, dirs[1], dirs[1], EMODEL_EIO_FILE_TYPE_DIR));
   //eo_do(filemodel, emodel_eio_child_add(_emodel_child_add_cb, dirs[2], dirs[2], EMODEL_EIO_FILE_TYPE_DIR));

   /**
    * The following test works however 
    * it is going to rename (move) the original directory to
    * new one so '/tmp' doesn't work , you'll need to use
    * '/tmp/some_other_dir' instead as source.
    */
//#define _RUN_LOCAL_TEST
#ifdef _RUN_LOCAL_TEST
   nameset = eina_value_new(EINA_VALUE_TYPE_STRING);
   eina_value_set(nameset, "/tmp/DIRAC");
   eo_do(filemodel, emodel_property_set("filename", nameset));
   eo_do(filemodel, emodel_property_get("filename"));
#endif
   //eo_do(filemodel, emodel_property_get("filename"));

//   timer = ecore_timer_add(0.1, _try_quit, NULL);
//   fail_if(timer == NULL);

   ecore_main_loop_begin();

   eo_unref(filemodel);
   ecore_shutdown();
}
END_TEST

void
emodel_test_file(TCase *tc)
{
   tcase_add_test(tc, emodel_test_test_file);
}

