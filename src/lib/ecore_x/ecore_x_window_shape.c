#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* ifdef HAVE_CONFIG_H */

#include <stdlib.h>

#include "Ecore.h"
#include "ecore_x_private.h"
#include "Ecore_X.h"

/**
 * @defgroup Ecore_X_Window_Shape X Window Shape Functions
 * @ingroup Ecore_X_Group
 *
 * These functions use the shape extension of the X server to change
 * shape of given windows.
 */

/**
 * Sets the shape of the given window to that given by the pixmap @p mask.
 * @param   win  The given window.
 * @param   mask A 2-bit depth pixmap that provides the new shape of the
 *               window.
 * @ingroup Ecore_X_Window_Shape
 */
EAPI void
ecore_x_window_shape_mask_set(Ecore_X_Window win,
                              Ecore_X_Pixmap mask)
{
   LOGFN;
   XShapeCombineMask(_ecore_x_disp, win, ShapeBounding, 0, 0, mask, ShapeSet);
   if (_ecore_xlib_sync) ecore_x_sync();
}

/**
 * Sets the input shape of the given window to that given by the pixmap @p mask.
 * @param   win  The given window.
 * @param   mask A 1-bit depth pixmap that provides the new input shape of the
 *               window.
 * @ingroup Ecore_X_Window_Shape
 */
EAPI void
ecore_x_window_shape_input_mask_set(Ecore_X_Window win,
                                    Ecore_X_Pixmap mask)
{
   LOGFN;
#ifdef ShapeInput
   XShapeCombineMask(_ecore_x_disp, win, ShapeInput, 0, 0, mask, ShapeSet);
   if (_ecore_xlib_sync) ecore_x_sync();
#else /* ifdef ShapeInput */
   return;
   win = mask = 0;
#endif /* ifdef ShapeInput */
}

EAPI void
ecore_x_window_shape_window_set(Ecore_X_Window win,
                                Ecore_X_Window shape_win)
{
   LOGFN;
   XShapeCombineShape(_ecore_x_disp,
                      win,
                      ShapeBounding,
                      0,
                      0,
                      shape_win,
                      ShapeBounding,
                      ShapeSet);
   if (_ecore_xlib_sync) ecore_x_sync();
}

EAPI void
ecore_x_window_shape_input_window_set(Ecore_X_Window win,
                                      Ecore_X_Window shape_win)
{
#ifdef ShapeInput
   LOGFN;
   XShapeCombineShape(_ecore_x_disp,
                      win,
                      ShapeInput,
                      0,
                      0,
                      shape_win,
                      ShapeInput,
                      ShapeSet);
   if (_ecore_xlib_sync) ecore_x_sync();
#else
   return;
   win = shape_win = 0;
#endif
}

EAPI void
ecore_x_window_shape_window_set_xy(Ecore_X_Window win,
                                   Ecore_X_Window shape_win,
                                   int x,
                                   int y)
{
   LOGFN;
   XShapeCombineShape(_ecore_x_disp,
                      win,
                      ShapeBounding,
                      x,
                      y,
                      shape_win,
                      ShapeBounding,
                      ShapeSet);
   if (_ecore_xlib_sync) ecore_x_sync();
}

EAPI void
ecore_x_window_shape_input_window_set_xy(Ecore_X_Window win,
                                         Ecore_X_Window shape_win,
                                         int x,
                                         int y)
{
#ifdef ShapeInput
   LOGFN;
   XShapeCombineShape(_ecore_x_disp,
                      win,
                      ShapeInput,
                      x,
                      y,
                      shape_win,
                      ShapeInput,
                      ShapeSet);
   if (_ecore_xlib_sync) ecore_x_sync();
#else
   return;
   win = shape_win = x = y = 0;
#endif
}

EAPI void
ecore_x_window_shape_rectangle_set(Ecore_X_Window win,
                                   int x,
                                   int y,
                                   int w,
                                   int h)
{
   XRectangle rect;

   LOGFN;
   rect.x = x;
   rect.y = y;
   rect.width = w;
   rect.height = h;
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeBounding,
                           0,
                           0,
                           &rect,
                           1,
                           ShapeSet,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
}

EAPI void
ecore_x_window_shape_input_rectangle_set(Ecore_X_Window win,
                                         int x,
                                         int y,
                                         int w,
                                         int h)
{
#ifdef ShapeInput
   XRectangle rect;

   LOGFN;
   rect.x = x;
   rect.y = y;
   rect.width = w;
   rect.height = h;
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeInput,
                           0,
                           0,
                           &rect,
                           1,
                           ShapeSet,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
#else
   return;
   win = x = y = w = h = 0;
#endif
}

EAPI void
ecore_x_window_shape_rectangles_set(Ecore_X_Window win,
                                    Ecore_X_Rectangle *rects,
                                    int num)
{
#ifdef ShapeInput
   XRectangle *rect = NULL;
   int i;

   LOGFN;
   if (!rects) return;
   if (num > 0)
     {
        rect = malloc(sizeof(XRectangle) * num);
        if (!rect) return;
        for (i = 0; i < num; i++)
          {
             rect[i].x = rects[i].x;
             rect[i].y = rects[i].y;
             rect[i].width = rects[i].width;
             rect[i].height = rects[i].height;
          }
     }
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeBounding,
                           0,
                           0,
                           rect,
                           num,
                           ShapeSet,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
   if (rect) free(rect);
#else
   return;
   win = rects = num = 0;
#endif
}

EAPI void
ecore_x_window_shape_input_rectangles_set(Ecore_X_Window win,
                                          Ecore_X_Rectangle *rects,
                                          int num)
{
#ifdef ShapeInput
   XRectangle *rect = NULL;
   int i;

   LOGFN;
   if (!rects) return;
   if (num > 0)
     {
        rect = malloc(sizeof(XRectangle) * num);
        if (!rect) return;
        for (i = 0; i < num; i++)
          {
             rect[i].x = rects[i].x;
             rect[i].y = rects[i].y;
             rect[i].width = rects[i].width;
             rect[i].height = rects[i].height;
          }
     }
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeInput,
                           0,
                           0,
                           rect,
                           num,
                           ShapeSet,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
   if (rect) free(rect);
#else
   return;
   win = rects = num = 0;
#endif
}

EAPI void
ecore_x_window_shape_rectangle_subtract(Ecore_X_Window win,
                                        int x,
                                        int y,
                                        int w,
                                        int h)
{
   XRectangle rect;

   LOGFN;
   rect.x = x;
   rect.y = y;
   rect.width = w;
   rect.height = h;
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeBounding,
                           0,
                           0,
                           &rect,
                           1,
                           ShapeSubtract,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
}

EAPI void
ecore_x_window_shape_input_rectangle_subtract(Ecore_X_Window win,
                                              int x,
                                              int y,
                                              int w,
                                              int h)
{
#ifdef ShapeInput
   XRectangle rect;

   LOGFN;
   rect.x = x;
   rect.y = y;
   rect.width = w;
   rect.height = h;
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeInput,
                           0,
                           0,
                           &rect,
                           1,
                           ShapeSubtract,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
#else
   return;
   win = x = y = w = h = 0;
#endif
}

EAPI void
ecore_x_window_shape_window_add(Ecore_X_Window win,
                                Ecore_X_Window shape_win)
{
   LOGFN;
   XShapeCombineShape(_ecore_x_disp,
                      win,
                      ShapeBounding,
                      0,
                      0,
                      shape_win,
                      ShapeBounding,
                      ShapeUnion);
   if (_ecore_xlib_sync) ecore_x_sync();
}

EAPI void
ecore_x_window_shape_window_add_xy(Ecore_X_Window win,
                                   Ecore_X_Window shape_win,
                                   int x,
                                   int y)
{
   LOGFN;
   XShapeCombineShape(_ecore_x_disp,
                      win,
                      ShapeBounding,
                      x,
                      y,
                      shape_win,
                      ShapeBounding,
                      ShapeUnion);
   if (_ecore_xlib_sync) ecore_x_sync();
}

EAPI void
ecore_x_window_shape_input_window_add_xy(Ecore_X_Window win,
                                         Ecore_X_Window shape_win,
                                         int x,
                                         int y)
{
#ifdef ShapeInput
   LOGFN;
   XShapeCombineShape(_ecore_x_disp,
                      win,
                      ShapeInput,
                      x,
                      y,
                      shape_win,
                      ShapeInput,
                      ShapeUnion);
   if (_ecore_xlib_sync) ecore_x_sync();
#else
   return;
   win = shape_win = x = y = 0;
#endif
}

EAPI void
ecore_x_window_shape_rectangle_add(Ecore_X_Window win,
                                   int x,
                                   int y,
                                   int w,
                                   int h)
{
   XRectangle rect;

   LOGFN;
   rect.x = x;
   rect.y = y;
   rect.width = w;
   rect.height = h;
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeBounding,
                           0,
                           0,
                           &rect,
                           1,
                           ShapeUnion,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
}

EAPI void
ecore_x_window_shape_input_rectangle_add(Ecore_X_Window win,
                                         int x,
                                         int y,
                                         int w,
                                         int h)
{
#ifdef ShapeInput
   XRectangle rect;

   LOGFN;
   rect.x = x;
   rect.y = y;
   rect.width = w;
   rect.height = h;
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeInput,
                           0,
                           0,
                           &rect,
                           1,
                           ShapeUnion,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
#else
   return;
   win = x = y = w = h = 0;
#endif
}

EAPI void
ecore_x_window_shape_rectangle_clip(Ecore_X_Window win,
                                    int x,
                                    int y,
                                    int w,
                                    int h)
{
   XRectangle rect;

   LOGFN;
   rect.x = x;
   rect.y = y;
   rect.width = w;
   rect.height = h;
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeBounding,
                           0,
                           0,
                           &rect,
                           1,
                           ShapeIntersect,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
}

EAPI void
ecore_x_window_shape_input_rectangle_clip(Ecore_X_Window win,
                                          int x,
                                          int y,
                                          int w,
                                          int h)
{
#ifdef ShapeInput
   XRectangle rect;

   LOGFN;
   rect.x = x;
   rect.y = y;
   rect.width = w;
   rect.height = h;
   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeInput,
                           0,
                           0,
                           &rect,
                           1,
                           ShapeIntersect,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
#else
   return;
   win = x = y = w = h = 0;
#endif
}

EAPI void
ecore_x_window_shape_rectangles_add(Ecore_X_Window win,
                                    Ecore_X_Rectangle *rects,
                                    int num)
{
   XRectangle *rect = NULL;
   int i;

   LOGFN;
   if (num > 0)
     {
        rect = malloc(sizeof(XRectangle) * num);
        if (!rect) return;
        for (i = 0; i < num; i++)
          {
             rect[i].x = rects[i].x;
             rect[i].y = rects[i].y;
             rect[i].width = rects[i].width;
             rect[i].height = rects[i].height;
          }
     }

   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeBounding,
                           0,
                           0,
                           rect,
                           num,
                           ShapeUnion,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
   if (rect) free(rect);
}

EAPI void
ecore_x_window_shape_input_rectangles_add(Ecore_X_Window win,
                                          Ecore_X_Rectangle *rects,
                                          int num)
{
#ifdef ShapeInput
   XRectangle *rect = NULL;
   int i;

   LOGFN;
   if (num > 0)
     {
        rect = malloc(sizeof(XRectangle) * num);
        if (!rect) return;
        for (i = 0; i < num; i++)
          {
             rect[i].x = rects[i].x;
             rect[i].y = rects[i].y;
             rect[i].width = rects[i].width;
             rect[i].height = rects[i].height;
          }
     }

   XShapeCombineRectangles(_ecore_x_disp,
                           win,
                           ShapeInput,
                           0,
                           0,
                           rect,
                           num,
                           ShapeUnion,
                           Unsorted);
   if (_ecore_xlib_sync) ecore_x_sync();
   if (rect) free(rect);
#else
   return;
   win = rects = num = 0;
#endif
}

EAPI Ecore_X_Rectangle *
ecore_x_window_shape_rectangles_get(Ecore_X_Window win,
                                    int *num_ret)
{
   XRectangle *rect;
   Ecore_X_Rectangle *rects = NULL;
   int i, num = 0, ord;

   LOGFN;
   rect = XShapeGetRectangles(_ecore_x_disp, win, ShapeBounding, &num, &ord);
   if (_ecore_xlib_sync) ecore_x_sync();
   if (rect)
     {
        if (num < 1)
          {
             XFree(rect);
             if (num_ret) *num_ret = 0;
             return NULL;
          }
        rects = malloc(sizeof(Ecore_X_Rectangle) * num);
        if (!rects)
          {
             XFree(rect);
             if (num_ret) *num_ret = 0;
             return NULL;
          }
        for (i = 0; i < num; i++)
          {
             rects[i].x = rect[i].x;
             rects[i].y = rect[i].y;
             rects[i].width = rect[i].width;
             rects[i].height = rect[i].height;
          }
        XFree(rect);
     }
   if (num_ret) *num_ret = num;
   return rects;
}

EAPI Ecore_X_Rectangle *
ecore_x_window_shape_input_rectangles_get(Ecore_X_Window win,
                                          int *num_ret)
{
   Ecore_X_Rectangle *rects = NULL;
#ifdef ShapeInput
   XRectangle *rect;
   int i, num = 0, ord;

   LOGFN;
   rect = XShapeGetRectangles(_ecore_x_disp, win, ShapeInput, &num, &ord);
   if (_ecore_xlib_sync) ecore_x_sync();
   if (rect)
     {
        if (num < 1)
          {
             XFree(rect);
             if (num_ret) *num_ret = 0;
             return NULL;
          }
        rects = malloc(sizeof(Ecore_X_Rectangle) * num);
        if (!rects)
          {
             XFree(rect);
             if (num_ret) *num_ret = 0;
             return NULL;
          }
        for (i = 0; i < num; i++)
          {
             rects[i].x = rect[i].x;
             rects[i].y = rect[i].y;
             rects[i].width = rect[i].width;
             rects[i].height = rect[i].height;
          }
        XFree(rect);
     }
   if (num_ret) *num_ret = num;
   return rects;
#else
   // have to return fake shape input rect of size of window
   Window dw;
   unsigned int di;

   if (num_ret) *num_ret = 0;
   rects = malloc(sizeof(Ecore_X_Rectangle));
   if (!rects) return NULL;
   if (!XGetGeometry(_ecore_x_disp, win, &dw,
                     &(rects[0].x), &(rects[0].y),
                     &(rects[0].width), &(rects[0].height),
                     &di, &di))
     {
        if (_ecore_xlib_sync) ecore_x_sync();
        free(rects);
        return NULL;
     }
      if (_ecore_xlib_sync) ecore_x_sync();
   if (num_ret) *num_ret = 1;
   return rects;
#endif
}

EAPI void
ecore_x_window_shape_events_select(Ecore_X_Window win,
                                   Eina_Bool on)
{
   LOGFN;
   if (on)
     XShapeSelectInput(_ecore_x_disp, win, ShapeNotifyMask);
   else
     XShapeSelectInput(_ecore_x_disp, win, 0);
      if (_ecore_xlib_sync) ecore_x_sync();
}

