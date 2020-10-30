#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Eina.h>
#include <Eet.h>
#include <Ecore.h>
#include <Ecore_File.h>
#include <Ecore_Con.h>
#include <Ecore_Ipc.h>

/* define macros and variable for using the eina logging system  */
#define EFREET_MODULE_LOG_DOM _efreet_cache_log_dom

#include "Efreet.h"
#include "efreet_cache_private.h"
#include "efreet_private.h"
#include "../../static_libs/buildsystem/buildsystem.h"

static Ecore_Ipc_Server    *ipc = NULL;
static Ecore_Event_Handler *hnd_add = NULL;
static Ecore_Event_Handler *hnd_del = NULL;
static Ecore_Event_Handler *hnd_data = NULL;

static int relaunch_try = 0;

#define IPC_HEAD(_type) \
   Ecore_Ipc_Event_Server_##_type *e = event; \
   if (e->server != ipc) \
     return ECORE_CALLBACK_PASS_ON

static void
_ipc_launch(void)
{
   char buf[PATH_MAX];
   int num;
   int try_gap = 10000; // 10ms
   int tries = 1000; // 1000 * 10ms == 10sec
   const char *s;

   if (relaunch_try == 0)
     {
        ipc = ecore_ipc_server_connect(ECORE_IPC_LOCAL_USER, "efreetd", 0, NULL);
        if (ipc)
          {
             relaunch_try++;
             return;
          }
     }
   relaunch_try--;
   s = getenv("EFREETD_CONNECT_TRIES");
   if (s)
     {
        num = atoi(s);
        if (num >= 0) tries = num;
     }
   s = getenv("EFREETD_CONNECT_TRY_GAP");
   if (s)
     {
        num = atoi(s);
        if (num >= 0) try_gap = num;
     }
   if (run_in_tree)
     bs_binary_get(buf, sizeof(buf), "efreet", "efreetd");
   else
     snprintf(buf, sizeof(buf), PACKAGE_BIN_DIR "/efreetd");
   ecore_exe_run(buf, NULL);
   num = 0;
   while ((!ipc) && (num < tries))
     {
        num++;
        usleep(try_gap);
        ipc = ecore_ipc_server_connect(ECORE_IPC_LOCAL_USER, "efreetd", 0, NULL);
     }
   if (!ipc) ERR("Timeout in trying to start and then connect to efreetd");
}

static Eina_Bool
_cb_server_add(void *data EINA_UNUSED, int type EINA_UNUSED, void *event)
{
   IPC_HEAD(Add);
   relaunch_try--;
   return ECORE_CALLBACK_DONE;
}

static Ecore_Timer *reconnect_timer = NULL;
static unsigned int reconnect_count = 0;

static Eina_Bool
_cb_server_reconnect(void *data EINA_UNUSED)
{
   if (reconnect_timer) ecore_timer_del(reconnect_timer);
   reconnect_timer = NULL;
   reconnect_count++;
   _ipc_launch();
   if (ipc)
     {
        const char *s;
        int len = 0;

        s = efreet_language_get();
        if (s) len = strlen(s);
        ecore_ipc_server_send(ipc, 1, 0, 0, 0, 0, s, len);
        efreet_icon_extensions_refresh();
     }
   return EINA_FALSE;
}

static Eina_Bool
_cb_server_del(void *data EINA_UNUSED, int type EINA_UNUSED, void *event)
{
   double t;
   IPC_HEAD(Del);
   ipc = NULL;

   if (disable_cache) return ECORE_CALLBACK_RENEW;
   if (reconnect_count > 10)
     {
        char *address = ecore_con_local_path_new(EINA_FALSE, "efreetd", 0);
        reconnect_timer = NULL;
        ERR("efreetd connection failed 10 times! check for stale socket file at %s", address);
        free(address);
        return EINA_FALSE;
     }
   // random 0.5 -> 1.0 sec from now
   t = (((double)((rand() + (int)getpid()) & 0xff) / 255.0) * 0.5) + 0.5;
   if (reconnect_timer) ecore_timer_del(reconnect_timer);
   reconnect_timer = ecore_timer_add(t, _cb_server_reconnect, NULL);
   return ECORE_CALLBACK_DONE;
}

Eina_Bool
_efreet_cache_reset_language(const char *s, int len)
{
   if (ipc) ecore_ipc_server_del(ipc);
   ipc = NULL;
   if (!disable_cache)
     ipc = ecore_ipc_server_connect(ECORE_IPC_LOCAL_USER, "efreetd", 0, NULL);
   if (!ipc) return EINA_FALSE;
   ecore_ipc_server_send(ipc, 1, 0, 0, 0, 0, s, len);
   return EINA_TRUE;
}

EAPI void (*_efreet_mime_update_func) (void) = NULL;

static Eina_Bool
_cb_server_data(void *data EINA_UNUSED, int type EINA_UNUSED, void *event)
{
   IPC_HEAD(Data);
   if (e->major == 1) // registration
     {
        if (e->minor == 1)
          ecore_event_add(EFREET_EVENT_DESKTOP_CACHE_BUILD, NULL, NULL, NULL);
     }
   else if (e->major == 2) // icon cache update
     {
        if (e->minor == 1)
          _icon_desktop_cache_update_event_add(EFREET_EVENT_ICON_CACHE_UPDATE);
        else
          ecore_event_add(EFREET_EVENT_ICON_CACHE_UPDATE, NULL, NULL, NULL);
     }
   else if (e->major == 3) // desktop cache update
     {
        _icon_desktop_cache_update_event_add(EFREET_EVENT_DESKTOP_CACHE_UPDATE);
     }
   else if (e->major == 4) // mime cache update
     {
        if (_efreet_mime_update_func) _efreet_mime_update_func();
     }
   return ECORE_CALLBACK_DONE;
}

void
_desktop_build_signal_send(const char *s, int len)
{
   if (disable_cache || !ipc) return;
   ecore_ipc_server_send(ipc, 3 /* build desktop cache */, 0, 0, 0, 0, s, len);
}

void
_server_config_signal_send()
{
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
}

EAPI void
efreet_cache_disable(void)
{
   Eina_Bool prev = disable_cache;

   disable_cache = EINA_TRUE;

   if (_efreet_cache_log_dom < 0) return; // not yet initialized
   if (prev == disable_cache) return; // same value
   if (ipc)
     {
        ecore_ipc_server_del(ipc);
        ipc = NULL;
     }
}

EAPI void
efreet_cache_enable(void)
{
   Eina_Bool prev = disable_cache;

   disable_cache = EINA_FALSE;

   if (_efreet_cache_log_dom < 0) return; // not yet initialized
   if (prev == disable_cache) return; // same value
   if (!ipc) _ipc_launch();
}

inline void
_ipc_data_clean()
{
   if (ipc) ecore_ipc_server_del(ipc);
   if (hnd_add) ecore_event_handler_del(hnd_add);
   if (hnd_del) ecore_event_handler_del(hnd_del);
   if (hnd_data) ecore_event_handler_del(hnd_data);

   ipc = NULL;
   hnd_add = NULL;
   hnd_del = NULL;
   hnd_data = NULL;
}

void
_desktop_add_signal_send(char * path, int path_len)
{
   if (disable_cache || !ipc) return;
   ecore_ipc_server_send(ipc, 2, 0, 0, 0, 0, path, path_len);
}

void
_icon_exts_add_signal_send(const unsigned char * buf, int length)
{
   if (disable_cache || !ipc) return;
   ecore_ipc_server_send(ipc, 5 /* add icon exts */, 0, 0, 0, 0, buf, length);
}

void
_icon_dirs_add_signal_send(const unsigned char *buf, int length)
{
   if (disable_cache || !ipc) return;
   ecore_ipc_server_send(ipc, 4 /* add icon dirs */, 0, 0, 0, 0, buf, length);

}
