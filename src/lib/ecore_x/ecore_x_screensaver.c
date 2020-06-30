/*
 * Screensaver code
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* ifdef HAVE_CONFIG_H */

#include "Ecore.h"
#include "ecore_x_private.h"
#include "Ecore_X.h"
#include "Ecore_X_Atoms.h"

static int _screensaver_available = -1;

ECORE_X_API Eina_Bool
ecore_x_screensaver_event_available_get(void)
{
   if (_screensaver_available >= 0)
     return _screensaver_available;

#ifdef ECORE_XSS
   int _screensaver_major, _screensaver_minor;

   LOGFN;
   _screensaver_major = 1;
   _screensaver_minor = 0;

   if (XScreenSaverQueryVersion(_ecore_x_disp, &_screensaver_major,
                                &_screensaver_minor))
     _screensaver_available = 1;
   else
     _screensaver_available = 0;

#else /* ifdef ECORE_XSS */
   _screensaver_available = 0;
#endif /* ifdef ECORE_XSS */
   return _screensaver_available;
}

ECORE_X_API int
ecore_x_screensaver_idle_time_get(void)
{
#ifdef ECORE_XSS
   XScreenSaverInfo *xss;
   unsigned long _idle;
   int dummy;
   int idle;

   LOGFN;
   xss = XScreenSaverAllocInfo();
   XScreenSaverQueryInfo(_ecore_x_disp,
                         RootWindow(_ecore_x_disp, DefaultScreen(
                                      _ecore_x_disp)), xss);

   _idle = xss->idle;
   XFree(xss);
   if (DPMSQueryExtension(_ecore_x_disp, &dummy, &dummy))
     {
        CARD16 standby, suspend, off;
        CARD16 state;
        BOOL onoff;

        if (DPMSCapable(_ecore_x_disp))
          {
             DPMSGetTimeouts(_ecore_x_disp, &standby, &suspend, &off);
             DPMSInfo(_ecore_x_disp, &state, &onoff);

             if (onoff)
               {
                  switch (state)
                    {
                     case DPMSModeStandby:
                        /* this check is a littlebit paranoid, but be sure */
                        if (_idle < (unsigned) (standby * 1000))
                          _idle += (standby * 1000);
                        break;
                     case DPMSModeSuspend:
                        if (_idle < (unsigned) ((suspend + standby) * 1000))
                          _idle += ((suspend + standby) * 1000);
                        break;
                     case DPMSModeOff:
                        if (_idle < (unsigned) ((off + suspend + standby) * 1000))
                          _idle += ((off + suspend + standby) * 1000);
                        break;
                     case DPMSModeOn:
                     default:
                        break;
                    }
               }
          }
     }
   idle = _idle / 1000;

   return idle;
#else
   return 0;
#endif /* ifdef ECORE_XSS */
}

ECORE_X_API void
ecore_x_screensaver_set(int timeout,
                        int interval,
                        int prefer_blanking,
                        int allow_exposures)
{
   LOGFN;
   XSetScreenSaver(_ecore_x_disp,
                   timeout,
                   interval,
                   prefer_blanking,
                   allow_exposures);
}

ECORE_X_API void
ecore_x_screensaver_timeout_set(int timeout)
{
   int pto, pint, pblank, pexpo;

   LOGFN;
   XGetScreenSaver(_ecore_x_disp, &pto, &pint, &pblank, &pexpo);
   XSetScreenSaver(_ecore_x_disp, timeout, pint, pblank, pexpo);
}

ECORE_X_API int
ecore_x_screensaver_timeout_get(void)
{
   int pto, pint, pblank, pexpo;

   LOGFN;
   XGetScreenSaver(_ecore_x_disp, &pto, &pint, &pblank, &pexpo);
   return pto;
}

ECORE_X_API void
ecore_x_screensaver_blank_set(int blank)
{
   int pto, pint, pblank, pexpo;

   LOGFN;
   XGetScreenSaver(_ecore_x_disp, &pto, &pint, &pblank, &pexpo);
   XSetScreenSaver(_ecore_x_disp, pto, pint, blank, pexpo);
}

ECORE_X_API int
ecore_x_screensaver_blank_get(void)
{
   int pto, pint, pblank, pexpo;

   LOGFN;
   XGetScreenSaver(_ecore_x_disp, &pto, &pint, &pblank, &pexpo);
   return pblank;
}

ECORE_X_API void
ecore_x_screensaver_expose_set(int expose)
{
   int pto, pint, pblank, pexpo;

   LOGFN;
   XGetScreenSaver(_ecore_x_disp, &pto, &pint, &pblank, &pexpo);
   XSetScreenSaver(_ecore_x_disp, pto, pint, pblank, expose);
}

ECORE_X_API int
ecore_x_screensaver_expose_get(void)
{
   int pto, pint, pblank, pexpo;

   LOGFN;
   XGetScreenSaver(_ecore_x_disp, &pto, &pint, &pblank, &pexpo);
   return pexpo;
}

ECORE_X_API void
ecore_x_screensaver_interval_set(int interval)
{
   int pto, pint, pblank, pexpo;

   LOGFN;
   XGetScreenSaver(_ecore_x_disp, &pto, &pint, &pblank, &pexpo);
   XSetScreenSaver(_ecore_x_disp, pto, interval, pblank, pexpo);
}

ECORE_X_API int
ecore_x_screensaver_interval_get(void)
{
   int pto, pint, pblank, pexpo;

   LOGFN;
   XGetScreenSaver(_ecore_x_disp, &pto, &pint, &pblank, &pexpo);
   return pint;
}

ECORE_X_API void
ecore_x_screensaver_event_listen_set(Eina_Bool on)
{
#ifdef ECORE_XSS
   Ecore_X_Window root;

   LOGFN;
   root = DefaultRootWindow(_ecore_x_disp);
   if (on)
     XScreenSaverSelectInput(_ecore_x_disp, root,
                             ScreenSaverNotifyMask | ScreenSaverCycle);
   else
     XScreenSaverSelectInput(_ecore_x_disp, root, 0);
#else
   return;
   on = EINA_FALSE;
#endif /* ifdef ECORE_XSS */
}


ECORE_X_API Eina_Bool
ecore_x_screensaver_custom_blanking_enable(void)
{
#ifdef ECORE_XSS
   XSetWindowAttributes attr;
   
   XScreenSaverSetAttributes(_ecore_x_disp,
                             DefaultRootWindow(_ecore_x_disp),
                             -9999, -9999, 1, 1, 0,
                             CopyFromParent, InputOnly, CopyFromParent,
                             0, &attr);
   return EINA_TRUE;
#else
   return EINA_FALSE;
#endif /* ifdef ECORE_XSS */
}

ECORE_X_API Eina_Bool
ecore_x_screensaver_custom_blanking_disable(void)
{
#ifdef ECORE_XSS
   XScreenSaverUnsetAttributes(_ecore_x_disp,
                               DefaultRootWindow(_ecore_x_disp));
   return EINA_TRUE;
#else
   return EINA_FALSE;
#endif /* ifdef ECORE_XSS */
}

EINA_DEPRECATED ECORE_X_API void
ecore_x_screensaver_supend(void)
{
   ecore_x_screensaver_suspend();
}

ECORE_X_API void 
ecore_x_screensaver_suspend(void)
{
#ifdef ECORE_XSS
   XScreenSaverSuspend(_ecore_x_disp, 1);
#endif /* ifdef ECORE_XSS */
}

ECORE_X_API void
ecore_x_screensaver_resume(void)
{
#ifdef ECORE_XSS
   XScreenSaverSuspend(_ecore_x_disp, 0);
#endif /* ifdef ECORE_XSS */
}

ECORE_X_API void
ecore_x_screensaver_reset(void)
{
   XResetScreenSaver(_ecore_x_disp);
}

ECORE_X_API void
ecore_x_screensaver_activate(void)
{
   XActivateScreenSaver(_ecore_x_disp);
}
