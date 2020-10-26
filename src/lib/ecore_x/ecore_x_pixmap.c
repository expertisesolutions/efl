#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* ifdef HAVE_CONFIG_H */

#include "Ecore.h"
#include "ecore_x_private.h"
#include "Ecore_X.h"

/**
 * @defgroup Ecore_X_Pixmap_Group X Pixmap Functions
 * @ingroup Ecore_X_Group
 *
 * Functions that operate on pixmaps.
 */

/**
 * Creates a new pixmap.
 * @param   win Window used to determine which screen of the display the
 *              pixmap should be created on.  If 0, the default root window
 *              is used.
 * @param   w   Width of the new pixmap.
 * @param   h   Height of the new pixmap.
 * @param   dep Depth of the pixmap.  If 0, the default depth of the default
 *              screen is used.
 * @return  New pixmap.
 * @ingroup Ecore_X_Pixmap_Group
 */
ECORE_X_API Ecore_X_Pixmap
ecore_x_pixmap_new(Ecore_X_Window win,
                   int w,
                   int h,
                   int dep)
{
   Ecore_X_Pixmap pm;
   LOGFN;
   if (win == 0)
     win = DefaultRootWindow(_ecore_x_disp);

   if (dep == 0)
     dep = DefaultDepth(_ecore_x_disp, DefaultScreen(_ecore_x_disp));

   pm = XCreatePixmap(_ecore_x_disp, win, w, h, dep);
   if (_ecore_xlib_sync) ecore_x_sync();
   return pm;
}

/**
 * Deletes the reference to the given pixmap.
 *
 * If no other clients have a reference to the given pixmap, the server
 * will destroy it.
 *
 * @param   pmap The given pixmap.
 * @ingroup Ecore_X_Pixmap_Group
 */
ECORE_X_API void
ecore_x_pixmap_free(Ecore_X_Pixmap pmap)
{
   LOGFN;
   if (!pmap) return;
   XFreePixmap(_ecore_x_disp, pmap);
   if (_ecore_xlib_sync) ecore_x_sync();
}

/**
 * Pastes a rectangular area of the given pixmap onto the given drawable.
 * @param   pmap The given pixmap.
 * @param   dest The given drawable.
 * @param   gc   The graphics context which governs which operation will
 *               be used to paste the area onto the drawable.
 * @param   sx   The X position of the area on the pixmap.
 * @param   sy   The Y position of the area on the pixmap.
 * @param   w    The width of the area.
 * @param   h    The height of the area.
 * @param   dx   The X position at which to paste the area on @p dest.
 * @param   dy   The Y position at which to paste the area on @p dest.
 * @ingroup Ecore_X_Pixmap_Group
 */
ECORE_X_API void
ecore_x_pixmap_paste(Ecore_X_Pixmap pmap,
                     Ecore_X_Drawable dest,
                     Ecore_X_GC gc,
                     int sx,
                     int sy,
                     int w,
                     int h,
                     int dx,
                     int dy)
{
   LOGFN;
   XCopyArea(_ecore_x_disp, pmap, dest, gc, sx, sy, w, h, dx, dy);
   if (_ecore_xlib_sync) ecore_x_sync();
}

/**
 * Retrieves the size of the given pixmap.
 * @param   pmap The given pixmap.
 * @param   x    Pointer to an integer in which to store the X position.
 * @param   y    Pointer to an integer in which to store the Y position.
 * @param   w    Pointer to an integer in which to store the width.
 * @param   h    Pointer to an integer in which to store the height.
 * @ingroup Ecore_X_Pixmap_Group
 */
ECORE_X_API void
ecore_x_pixmap_geometry_get(Ecore_X_Pixmap pmap,
                            int *x,
                            int *y,
                            int *w,
                            int *h)
{
   LOGFN;
   if (!pmap) return;
   ecore_x_drawable_geometry_get(pmap, x, y, w, h);
   if (_ecore_xlib_sync) ecore_x_sync();
}

/**
 * Retrieves the depth of the given pixmap.
 * @param   pmap The given pixmap.
 * @return  The depth of the pixmap.
 * @ingroup Ecore_X_Pixmap_Group
 */
ECORE_X_API int
ecore_x_pixmap_depth_get(Ecore_X_Pixmap pmap)
{
   int ret;
   LOGFN;
   ret = ecore_x_drawable_depth_get(pmap);
   if (_ecore_xlib_sync) ecore_x_sync();
   return ret;
}

