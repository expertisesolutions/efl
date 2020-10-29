#include "efreet_cache_private.h"

void
_desktop_cache_build_signal_send(void)
{
   ecore_event_add(EFREET_EVENT_DESKTOP_CACHE_BUILD, NULL, NULL, NULL);
}

void
_icon_cache_update_signal_send(Eina_Bool update)
{
    /* TODO
   _broadcast(ipc, 2 /* icon cache update */, update, NULL, 0);
    */

}

void
_desktop_cache_update_signal_send(Eina_Bool update)
{
    /* TODO
   _broadcast(ipc, 3 /* desktop cache update */, update, NULL, 0);
    */
}

void
_mime_cache_build_signal_send(void)
{
    /* TODO
   _broadcast(ipc, 4 /* mime cache build */, 1, NULL, 0);
    */
}

