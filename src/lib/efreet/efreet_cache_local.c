#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Eina.h>
#include <Eet.h>
#include <Ecore.h>
#include <Ecore_File.h>

/* define macros and variable for using the eina logging system  */
#define EFREET_MODULE_LOG_DOM _efreet_cache_log_dom

#include "Efreet.h"
#include "efreet_cache_private.h"
#include "efreet_private.h"
#include "../../static_libs/buildsystem/buildsystem.h"

#include "efreetd.h"
#include "efreetd_cache.h"

Eina_Bool
_efreet_cache_reset_language(const char *data, int size)
{
   // input: str -> lang
   if (disable_cache) return EINA_FALSE;

   char *s;
   fprintf(efreetd_log_file, "[%09.3f] Client register lang\n", ecore_time_get());
   fflush(efreetd_log_file);
   if ((s = _parse_str(data, size)))
     {
        setenv("LANG", s, 1);
        free(s);
     }
   // return if desktop cache exists (bool as minor)
   if (cache_desktop_exists() == 1)
     {
        ecore_event_add(EFREET_EVENT_DESKTOP_CACHE_BUILD, NULL, NULL, NULL);
     }
   return EINA_TRUE;
}

void
_server_config_signal_send()
{
   if (disable_cache)
     {
        Efreet_Event_Cache_Update *ev;

        ev = NEW(Efreet_Event_Cache_Update, 1);
        if (ev)
          {
             ev->error = 1;
             ecore_event_add(EFREET_EVENT_DESKTOP_CACHE_BUILD, ev, NULL, NULL);
          }
        return;
     }

   const char *s = efreet_language_get();
   int len = 0;

   if (s) len = strlen(s);
   _efreet_cache_reset_language(s, len);
}

void
_desktop_add_signal_send(char *data, int size)
{
   // input: array of str -> dirs
   if (disable_cache) return;

    _desktop_add(data, size);
}

void
_desktop_build_signal_send(const char *data, int size)
{
   // input: str -> lang
   if (disable_cache) return;

   _desktop_build(data, size);
}

void
_icon_dirs_add_signal_send(const unsigned char* data, int size)
{
   if (disable_cache) return;

   _icon_dirs_add(data, size);
}

void
_icon_exts_add_signal_send(const unsigned char *data, int size)
{
   if (disable_cache) return;

   _icon_exts_add(data, size);
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
