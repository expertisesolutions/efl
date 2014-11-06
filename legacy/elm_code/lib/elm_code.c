#ifdef HAVE_CONFIG
# include "config.h"
#endif

#define EFL_BETA_API_SUPPORT
#include <Eo.h>

#include "Elm_Code.h"

#include "elm_code_private.h"

static int _elm_code_init = 0;
int _elm_code_lib_log_dom = -1;

EAPI int
elm_code_init(void)
{
   _elm_code_init++;
   if (_elm_code_init > 1) return _elm_code_init;

   eina_init();

   _elm_code_lib_log_dom = eina_log_domain_register("elm_code", EINA_COLOR_CYAN);
   if (_elm_code_lib_log_dom < 0)
     {
        EINA_LOG_ERR("Elm Code can not create its log domain.");
        goto shutdown_eina;
     }

   // Put here your initialization logic of your library

   eina_log_timing(_elm_code_lib_log_dom, EINA_LOG_STATE_STOP, EINA_LOG_STATE_INIT);

   return _elm_code_init;

   shutdown_eina:
   eina_shutdown();
   _elm_code_init--;

   return _elm_code_init;
}

EAPI int
elm_code_shutdown(void)
{
   _elm_code_init--;
   if (_elm_code_init != 0) return _elm_code_init;

   eina_log_timing(_elm_code_lib_log_dom,
                   EINA_LOG_STATE_START,
                   EINA_LOG_STATE_SHUTDOWN);

   // Put here your shutdown logic

   eina_log_domain_unregister(_elm_code_lib_log_dom);
   _elm_code_lib_log_dom = -1;

   eina_shutdown();

   return _elm_code_init;
}

EAPI Elm_Code *
elm_code_create(Elm_Code_File *file)
{
   Elm_Code *ret;

   ret = calloc(1, sizeof(Elm_Code));
   ret->file = file;
   file->parent = ret;

   return ret;
}

EAPI void
elm_code_free(Elm_Code *code)
{
   Eina_List *item;
   Evas_Object *widget;

   if (code->file)
     elm_code_file_free(code->file);

   EINA_LIST_FOREACH(code->widgets, item, widget)
     {
        evas_object_hide(widget);
        evas_object_del(widget);
     }

   free(code);
}

EAPI void
elm_code_callback_fire(Elm_Code *code, const char *signal, void *data)
{
   Eina_List *item;
   Evas_Object *widget;

   EINA_LIST_FOREACH(code->widgets, item, widget)
     {
        eo_do(widget, eo_event_callback_call(ELM_CODE_EVENT_LINE_SET_DONE, data));
     }
}

