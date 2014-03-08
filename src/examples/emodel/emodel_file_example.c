//Compile with:
// gcc -o emodel_file_example emodel_file_example.c `pkg-config --cflags --libs eio emodel`

#include <Eo.h>
#include <Eio.h>
#include <Emodel.h>
#include <stdio.h>


static Eina_Bool
_prop_change_cb(void *data, Eo *obj EINA_UNUSED, const Eo_Event_Description *desc EINA_UNUSED, void *event_info)
{
   Emodel_Property_EVT *evt = event_info;
   if (!strcmp(evt->prop, "filename"))
      printf("filename: %s\n", eina_value_to_string(evt->value));

   printf("HA\n");
   return EINA_TRUE;
}

int
main(int argc, const char *argv[])
{
   const char *filename = NULL;
   Eo *filemodel;

   if (argc < 2)
     {
        printf("Usage: %s filename\n", argv);
        return -1;
     }

   filename = argv[1];
   filemodel = eo_add_custon(EMODEL_EIO_CLASS, NULL, emodel_eio_constructor(filename));
   eo_do(filemodel, eo_event_callback_add(EMODEL_PROPERTY_CHANGE_EVT, _prop_change_cb, NULL));
   eo_do(filemodel, emodel_property_get("filename"));

   getchar();
   eo_unref(filemodel);

   return 0;
}
