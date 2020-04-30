/*
 * Xinerama code
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* ifdef HAVE_CONFIG_H */

#include "Ecore.h"
#include "ecore_x_private.h"
#include "Ecore_X.h"
#include "Ecore_X_Atoms.h"

#ifdef ECORE_XINERAMA
static XineramaScreenInfo *_xin_info = NULL;
static int _xin_scr_num = 0;
#endif /* ifdef ECORE_XINERAMA */

int
ecore_x_xinerama_screen_count_get(void)
{
#ifdef ECORE_XINERAMA
   int event_base, error_base;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   if (_xin_info)
     XFree(_xin_info);

   _xin_info = NULL;
   if (XineramaQueryExtension(_ecore_x_disp, &event_base, &error_base))
     {
        if (_ecore_xlib_sync) ecore_x_sync();
        _xin_info = XineramaQueryScreens(_ecore_x_disp, &_xin_scr_num);
        if (_ecore_xlib_sync) ecore_x_sync();
        if (_xin_info)
          return _xin_scr_num;
     }
   if (_ecore_xlib_sync) ecore_x_sync();

#endif /* ifdef ECORE_XINERAMA */
   return 0;
}

Eina_Bool
ecore_x_xinerama_screen_geometry_get(int screen EINA_UNUSED, // if no xinerama
                                     int *x,
                                     int *y,
                                     int *w,
                                     int *h)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
#ifdef ECORE_XINERAMA
   if (_xin_info)
     {
        int i;

        for (i = 0; i < _xin_scr_num; i++)
          {
             if (_xin_info[i].screen_number == screen)
               {
                  if (x)
                    *x = _xin_info[i].x_org;

                  if (y)
                    *y = _xin_info[i].y_org;

                  if (w)
                    *w = _xin_info[i].width;

                  if (h)
                    *h = _xin_info[i].height;

                  return EINA_TRUE;
               }
          }
     }

#endif /* ifdef ECORE_XINERAMA */
   if (x)
     *x = 0;

   if (y)
     *y = 0;

   if (w)
     *w = DisplayWidth(_ecore_x_disp, 0);

   if (h)
     *h = DisplayHeight(_ecore_x_disp, 0);

   return EINA_FALSE;
}

