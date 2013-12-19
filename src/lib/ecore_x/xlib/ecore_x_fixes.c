#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* ifdef HAVE_CONFIG_H */

#include <stdlib.h>

#include "ecore_x_private.h"
#include "Ecore_X.h"

static int _fixes_available;
#ifdef ECORE_XFIXES
static int _fixes_major, _fixes_minor;
#endif /* ifdef ECORE_XFIXES */

void
_ecore_x_fixes_init(void)
{
#ifdef ECORE_XFIXES
   _fixes_major = 3;
   _fixes_minor = 0;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   if (XFixesQueryVersion(_ecore_x_disp, &_fixes_major, &_fixes_minor))
     {
        _fixes_available = 1;

        ECORE_X_EVENT_FIXES_SELECTION_NOTIFY = ecore_event_type_new();
     }
   else
     _fixes_available = 0;

#else /* ifdef ECORE_XFIXES */
   _fixes_available = 0;
#endif /* ifdef ECORE_XFIXES */
}

#ifdef ECORE_XFIXES
/* I don't know what to call this function. */
static XRectangle *
_ecore_x_rectangle_ecore_to_x(Ecore_X_Rectangle *rects,
                              int num)
{
   XRectangle *xrect;
   int i;

   if (num == 0)
     return NULL;

   xrect = malloc(sizeof(XRectangle) * num);
   if (!xrect)
     return NULL;

   for (i = 0; i < num; i++)
     {
        xrect[i].x = rects[i].x;
        xrect[i].y = rects[i].y;
        xrect[i].width = rects[i].width;
        xrect[i].height = rects[i].height;
     }
   return xrect;
}

static Ecore_X_Rectangle *
_ecore_x_rectangle_x_to_ecore(XRectangle *xrect,
                              int num)
{
   Ecore_X_Rectangle *rects;
   int i;

   if (num == 0)
     return NULL;

   rects = malloc(sizeof(Ecore_X_Rectangle) * num);
   if (!rects)
     return NULL;

   for (i = 0; i < num; i++)
     {
        rects[i].x = xrect[i].x;
        rects[i].y = xrect[i].y;
        rects[i].width = xrect[i].width;
        rects[i].height = xrect[i].height;
     }
   return rects;
}

#endif /* ifdef ECORE_XFIXES */

EAPI Eina_Bool
ecore_x_fixes_selection_notification_request(Ecore_X_Atom selection)
{
#ifdef ECORE_XFIXES
   if (_fixes_available)
     {
        XFixesSelectSelectionInput (_ecore_x_disp,
                                    DefaultRootWindow(_ecore_x_disp),
                                    selection,
                                    XFixesSetSelectionOwnerNotifyMask |
                                    XFixesSelectionWindowDestroyNotifyMask |
                                    XFixesSelectionClientCloseNotifyMask);
        return EINA_TRUE;
     }
#endif
   return EINA_FALSE;
}

EAPI Ecore_X_Region
ecore_x_region_new(Ecore_X_Rectangle *rects,
                   int num)
{
#ifdef ECORE_XFIXES
   Ecore_X_Region region;
   XRectangle *xrect;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   xrect = _ecore_x_rectangle_ecore_to_x(rects, num);
   region = XFixesCreateRegion(_ecore_x_disp, xrect, num);
   free(xrect);
   return region;
#else /* ifdef ECORE_XFIXES */
   return 0;
#endif /* ifdef ECORE_XFIXES */
}

EAPI Ecore_X_Region
ecore_x_region_new_from_bitmap(Ecore_X_Pixmap bitmap)
{
#ifdef ECORE_XFIXES
   Ecore_X_Region region;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   region = XFixesCreateRegionFromBitmap(_ecore_x_disp, bitmap);
   if (_ecore_xlib_sync) ecore_x_sync();
   return region;
#else /* ifdef ECORE_XFIXES */
   return 0;
#endif /* ifdef ECORE_XFIXES */
}

EAPI Ecore_X_Region
ecore_x_region_new_from_window(Ecore_X_Window win,
                               Ecore_X_Region_Type type)
{
#ifdef ECORE_XFIXES
   Ecore_X_Region region;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   region = XFixesCreateRegionFromWindow(_ecore_x_disp, win, type);
   if (_ecore_xlib_sync) ecore_x_sync();
   return region;
#else /* ifdef ECORE_XFIXES */
   return 0;
#endif /* ifdef ECORE_XFIXES */
}

EAPI Ecore_X_Region
ecore_x_region_new_from_gc(Ecore_X_GC gc)
{
#ifdef ECORE_XFIXES
   Ecore_X_Region region;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   region = XFixesCreateRegionFromGC(_ecore_x_disp, gc);
   if (_ecore_xlib_sync) ecore_x_sync();
   return region;
#else /* ifdef ECORE_XFIXES */
   return 0;
#endif /* ifdef ECORE_XFIXES */
}

EAPI Ecore_X_Region
ecore_x_region_new_from_picture(Ecore_X_Picture picture)
{
#ifdef ECORE_XFIXES
   Ecore_X_Region region;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   region = XFixesCreateRegionFromPicture(_ecore_x_disp, picture);
   if (_ecore_xlib_sync) ecore_x_sync();
   return region;
#else /* ifdef ECORE_XFIXES */
   return 0;
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_free(Ecore_X_Region region)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesDestroyRegion(_ecore_x_disp, region);
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_set(Ecore_X_Region region,
                   Ecore_X_Rectangle *rects,
                   int num)
{
#ifdef ECORE_XFIXES
   XRectangle *xrect = _ecore_x_rectangle_ecore_to_x(rects, num);
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesSetRegion(_ecore_x_disp, region, xrect, num);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_copy(Ecore_X_Region dest,
                    Ecore_X_Region source)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesCopyRegion(_ecore_x_disp, dest, source);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_combine(Ecore_X_Region dest,
                       Ecore_X_Region source1,
                       Ecore_X_Region source2)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesUnionRegion(_ecore_x_disp, dest, source1, source2);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_intersect(Ecore_X_Region dest,
                         Ecore_X_Region source1,
                         Ecore_X_Region source2)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesIntersectRegion(_ecore_x_disp, dest, source1, source2);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_subtract(Ecore_X_Region dest,
                        Ecore_X_Region source1,
                        Ecore_X_Region source2)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesSubtractRegion(_ecore_x_disp, dest, source1, source2);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_invert(Ecore_X_Region dest,
                      Ecore_X_Rectangle *bounds,
                      Ecore_X_Region source)
{
#ifdef ECORE_XFIXES
   XRectangle *xbound;
   int num = 1;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   xbound = _ecore_x_rectangle_ecore_to_x(bounds, num);

   XFixesInvertRegion(_ecore_x_disp, dest, xbound, source);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_translate(Ecore_X_Region region,
                         int dx,
                         int dy)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesTranslateRegion(_ecore_x_disp, region, dx, dy);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_extents(Ecore_X_Region dest,
                       Ecore_X_Region source)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesRegionExtents(_ecore_x_disp, dest, source);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI Ecore_X_Rectangle *
ecore_x_region_fetch(Ecore_X_Region region,
                     int *num,
                     Ecore_X_Rectangle *bounds){
#ifdef ECORE_XFIXES
   Ecore_X_Rectangle *rects;
   XRectangle *xrect, xbound;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   xrect = XFixesFetchRegionAndBounds(_ecore_x_disp, region, num, &xbound);
   if (_ecore_xlib_sync) ecore_x_sync();
   rects = _ecore_x_rectangle_x_to_ecore(xrect, *num);
   (*bounds).x = xbound.x;
   (*bounds).y = xbound.y;
   (*bounds).width = xbound.width;
   (*bounds).height = xbound.height;
   return rects;
#else /* ifdef ECORE_XFIXES */
   return NULL;
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_expand(Ecore_X_Region dest,
                      Ecore_X_Region source,
                      unsigned int left,
                      unsigned int right,
                      unsigned int top,
                      unsigned int bottom)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesExpandRegion(_ecore_x_disp, dest, source, left, right, top, bottom);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_gc_clip_set(Ecore_X_Region region,
                           Ecore_X_GC gc,
                           int x_origin,
                           int y_origin)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesSetGCClipRegion(_ecore_x_disp, gc, x_origin, y_origin, region);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_window_shape_set(Ecore_X_Region region,
                                Ecore_X_Window win,
                                Ecore_X_Shape_Type type,
                                int x_offset,
                                int y_offset)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesSetWindowShapeRegion(_ecore_x_disp,
                              win,
                              type,
                              x_offset,
                              y_offset,
                              region);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

EAPI void
ecore_x_region_picture_clip_set(Ecore_X_Region region,
                                Ecore_X_Picture picture,
                                int x_origin,
                                int y_origin)
{
#ifdef ECORE_XFIXES
   LOGFN(__FILE__, __LINE__, __FUNCTION__);
   XFixesSetPictureClipRegion(_ecore_x_disp,
                              picture,
                              x_origin,
                              y_origin,
                              region);
   if (_ecore_xlib_sync) ecore_x_sync();
#endif /* ifdef ECORE_XFIXES */
}

