#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef _WIN32
# include <evil_private.h> /* setenv */
#endif

#include <Eet.h>
#include <Ecore.h>

/* define macros and variable for using the eina logging system  */
#define EFREET_MODULE_LOG_DOM _efreet_cache_log_dom

#include "Efreet.h"
#include "efreet_cache_private.h"
#include "efreetd_cache.h"
#include "efreet_private.h"
#include "../../static_libs/buildsystem/buildsystem.h"

void
_desktop_cache_build_signal_send(void)
{
   ecore_event_add(EFREET_EVENT_DESKTOP_CACHE_BUILD, NULL, NULL, NULL);
}

void
_icon_cache_update_signal_send(Eina_Bool update)
{
   if (update == 1)
     {
        _icon_desktop_cache_update_event_add(EFREET_EVENT_ICON_CACHE_UPDATE);
     }
   else
     {
        ecore_event_add(EFREET_EVENT_ICON_CACHE_UPDATE, NULL, NULL, NULL);
     }
}

void
_desktop_cache_update_signal_send(Eina_Bool update)
{
   _icon_desktop_cache_update_event_add(EFREET_EVENT_DESKTOP_CACHE_UPDATE);
}

void
_mime_cache_build_signal_send(void)
{
   if (_efreet_mime_update_func) _efreet_mime_update_func();
}

