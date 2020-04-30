//Compile with:
// gcc -o emodel_test_file emodel_test_file.c `pkg-config --cflags --libs emodel`
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Eo.h>
#include <Eio.h>
#include <Ecore.h>
#include <Efl.h>
#include <stdio.h>

#include "eio_suite.h"

Eina_Tmpstr* temp_filename = NULL;
const char* tmpdir = NULL;
Eina_Bool children_deleted = EINA_FALSE;

// This will ensure that the child have the time to be automatically destroyed
static Eina_Value
_delayed_quit(Eo *o EINA_UNUSED, void *data EINA_UNUSED, const Eina_Value v)
{
   ecore_main_loop_quit();

   return v;
}

static Eina_Value
_children_got(Eo *o, void *data EINA_UNUSED, const Eina_Value v)
{
   Eo *child = NULL;
   unsigned int i, len;
   Eina_Value r = (Eina_Value) v;

   EINA_VALUE_ARRAY_FOREACH(&v, len, i, child)
     {
        Eina_Value *path;
        char *str;

        path = efl_model_property_get(child, "path");
        fail_if(path == NULL);
        str = eina_value_to_string(path);
        fail_if(str == NULL);

        if (temp_filename && strcmp(str, temp_filename) == 0)
          r = eina_future_as_value(efl_future_then(efl_loop_get(o),
                                                   efl_loop_job(efl_loop_get(o)),
                                                   .success = _delayed_quit));


        free(str);
        eina_value_free(path);
     }

   return r;
}

static Eina_Value
_children_failed(Eo *o EINA_UNUSED, void *data EINA_UNUSED, const Eina_Error err)
{
   if (err != ECANCELED)
     ck_abort_msg( "Failed to get the child with '%s'.\n", eina_error_msg_get(err));
   return eina_value_error_init(err);
}

static void
_children_removed_cb(void *data EINA_UNUSED, const Efl_Event* event)
{
   Efl_Model_Children_Event* evt = event->info;
   Eina_Value *path;
   char *str;

   if (!evt->child)
     {
        Eina_Future *f;

        // Last time we can fetch the object
        f = efl_future_then(event->object,
                            efl_model_children_slice_get(event->object, evt->index, 1),
                            .success = _children_got,
                            .error = _children_failed,
                            .success_type = EINA_VALUE_TYPE_ARRAY);
        fail_if(f == NULL);
        return;
     }

   fail_if(evt->child == NULL);
   path = efl_model_property_get(evt->child, "path");
   fail_if(path == NULL);
   str = eina_value_to_string(path);
   fail_if(str == NULL);

   if (temp_filename && strcmp(str, temp_filename) == 0)
     efl_future_then(efl_loop_get(evt->child),
                     efl_loop_job(efl_loop_get(evt->child)),
                     .success = _delayed_quit);

   free(str);
   eina_value_free(path);
}

static Eina_Bool started_up = EINA_FALSE;

static Eina_Value
_children_get(void *data,
              const Eina_Value v,
              const Eina_Future *dead_future EINA_UNUSED)
{
   Efl_Model *filemodel = data;
   Eo *child = NULL;
   unsigned int i, len;

   if (eina_value_type_get(&v) == EINA_VALUE_TYPE_ERROR)
     {
        Eina_Error err = 0;

        fail_if(!eina_value_error_get(&v, &err));
        fail_if(err != ECANCELED);
        return v;
     }

   fail_if(eina_value_type_get(&v) != EINA_VALUE_TYPE_ARRAY);

   EINA_VALUE_ARRAY_FOREACH(&v, len, i, child)
     {
        Eina_Value *path;
        char *str;

        path = efl_model_property_get(child, "path");
        fail_if(path == NULL);
        str = eina_value_to_string(path);
        fail_if(str == NULL);

        if (started_up && strcmp(temp_filename, str) == 0)
          {
             children_deleted = EINA_TRUE;
             efl_model_child_del(filemodel, child);
          }
        free(str);
        eina_value_free(path);
     }

   if (!started_up)
     {
        int fd;

        fd = eina_file_mkstemp("prefixXXXXXX.ext", &temp_filename);
        ck_assert_int_gt(fd, 0);
        close(fd);
     }
   started_up = EINA_TRUE;

   return v;
}

static void
_children_added_cb(void *d EINA_UNUSED, const Efl_Event* event)
{
   Efl_Model_Children_Event* evt = event->info;
   Eina_Future *future;

   future = efl_model_children_slice_get(event->object, evt->index, 1);
   eina_future_then(future, _children_get, event->object, NULL);
}

EFL_START_TEST(efl_io_model_test_test_monitor_add)
{
   Eo *filemodel = NULL;

   tmpdir = eina_environment_tmp_get();

   filemodel = efl_add(EFL_IO_MODEL_CLASS,
                       efl_main_loop_get(),
                       efl_io_model_path_set(efl_added, tmpdir));
   fail_if(!filemodel, "ERROR: Cannot init model!\n");

   efl_event_callback_add(filemodel, EFL_MODEL_EVENT_CHILD_ADDED, &_children_added_cb, filemodel);
   efl_event_callback_add(filemodel, EFL_MODEL_EVENT_CHILD_REMOVED, &_children_removed_cb, NULL);

   ecore_main_loop_begin();

   efl_del(filemodel);

   fail_if(!children_deleted);
}
EFL_END_TEST

void
efl_io_model_test_monitor_add(TCase *tc)
{
   tcase_add_test(tc, efl_io_model_test_test_monitor_add);
}
