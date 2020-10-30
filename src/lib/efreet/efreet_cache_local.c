#include "efreet_cache_private.h"

#include <Eina.h>

static Eina_Bool
_efreet_cache_reset_language(const char *s, int len)
{
/*
   if (ipc) ecore_ipc_server_del(ipc);
   ipc = NULL;
   if (!disable_cache)
     ipc = ecore_ipc_server_connect(ECORE_IPC_LOCAL_USER, "efreetd", 0, NULL);
   if (!ipc) return EINA_FALSE;
   return EINA_TRUE
*/
}

static void
_desktop_build_signal_send()
{
/*
   if (!ipc) return;
   ecore_ipc_server_send(ipc, 3 /* build desktop cache *\/, 0, 0, 0, 0, s, len);
*/
}

EAPI void
efreet_cache_disable(void)
{
   Eina_Bool prev = disable_cache;

   disable_cache = EINA_TRUE;

   if (_efreet_cache_log_dom < 0) return; // not yet initialized
   if (prev == disable_cache) return; // same value
   /* TODO ecore_ipc_server_del(ipc); */
}

EAPI void
efreet_cache_enable(void)
{
   Eina_Bool prev = disable_cache;

   disable_cache = EINA_FALSE;

   if (_efreet_cache_log_dom < 0) return; // not yet initialized
   if (prev == disable_cache) return; // same value
   /* TODO _ipc_launch(); */
}

static void
_server_config_signal_send()
{
/*
   if (disable_cache)
     ipc = NULL;
   else
     {
        ipc = ecore_ipc_server_connect(ECORE_IPC_LOCAL_USER, "efreetd", 0, NULL);
        if (!ipc) _ipc_launch();
     }
   if (ipc)
     {
        const char *s;
        int len = 0;

        hnd_add = ecore_event_handler_add(ECORE_IPC_EVENT_SERVER_ADD,
                                          _cb_server_add, NULL);
        hnd_del = ecore_event_handler_add(ECORE_IPC_EVENT_SERVER_DEL,
                                          _cb_server_del, NULL);
        hnd_data = ecore_event_handler_add(ECORE_IPC_EVENT_SERVER_DATA,
                                           _cb_server_data, NULL);
        s = efreet_language_get();
        if (s) len = strlen(s);
        ecore_ipc_server_send(ipc, 1, 0, 0, 0, 0, s, len);
     }
   else
     {
        Efreet_Event_Cache_Update *ev;

        if (!disable_cache)
          WRN("Can't contact efreetd daemon for desktop/icon etc. changes");
        ev = NEW(Efreet_Event_Cache_Update, 1);
        if (ev)
          {
             ev->error = 1;
             ecore_event_add(EFREET_EVENT_DESKTOP_CACHE_BUILD, ev, NULL, NULL);
          }
     }
 */
}

static void
_desktop_add_signal_send(cahr * path, int path_len)
{
    /*
   if (!ipc) return;
   ecore_ipc_server_send(ipc, 2, 0, 0, 0, 0, path, path_len);
   */
}

static void
_icon_exts_add_signal_send(char* buf, int length)
{
/*
   if (!ipc) return;
   ecore_ipc_server_send(ipc, 5 /* add icon exts , 0, 0, 0, 0, buf, lenght;
*/
}

static void
_icon_dirs_add_signal_send(char *buf, int length)
{
/*
   if (!ipc)) return;
   ecore_ipc_server_send(ipc, 4 /* add icon dirs , 0, 0, 0, 0,
                         eina_binbuf_string_get(buf),
                     eina_binbuf_length_get(buf));
*/
}
