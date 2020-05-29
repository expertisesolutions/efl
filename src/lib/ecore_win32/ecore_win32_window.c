#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <Eina.h>

#include "Ecore_Win32.h"
#include "ecore_win32_private.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */


typedef enum _Ecore_Win32_Window_Z_Order Ecore_Win32_Window_Z_Order;
enum _Ecore_Win32_Window_Z_Order
{
  ECORE_WIN32_WINDOW_Z_ORDER_BOTTOM,
  ECORE_WIN32_WINDOW_Z_ORDER_NOTOPMOST,
  ECORE_WIN32_WINDOW_Z_ORDER_TOP,
  ECORE_WIN32_WINDOW_Z_ORDER_TOPMOST
};

static Ecore_Win32_Window *
_ecore_win32_window_internal_new(Ecore_Win32_Window *parent,
                                int                 x,
                                int                 y,
                                int                 width,
                                int                 height,
                                DWORD               style)
{
   RECT                rect;
   Ecore_Win32_Window *w;

   w = (Ecore_Win32_Window *)calloc(1, sizeof(Ecore_Win32_Window));
   if (!w)
     {
        ERR("malloc() failed");
        return NULL;
     }

   rect.left = 0;
   rect.top = 0;
   rect.right = 0;
   rect.bottom = 0;
   if (!AdjustWindowRectEx(&rect, style, FALSE, 0))
     {
        ERR("AdjustWindowRect() failed");
        free(w);
        return NULL;
     }

   w->mininal_window_width = GetSystemMetrics(SM_CXMIN) - (rect.right - rect.left);

   rect.left = 0;
   rect.top = 0;
   rect.right = width;
   rect.bottom = height;
   if (!AdjustWindowRectEx(&rect, style, FALSE, 0))
     {
        ERR("AdjustWindowRect() failed");
        free(w);
        return NULL;
     }

   if (width < w->mininal_window_width)
     width = w->mininal_window_width;

   rect.left = 0;
   rect.top = 0;
   rect.right = width;
   rect.bottom = height;
   if (!AdjustWindowRectEx(&rect, style, FALSE, 0))
     {
        ERR("AdjustWindowRect() failed");
        free(w);
        return NULL;
     }

   w->window = CreateWindowEx(0,
                              ECORE_WIN32_WINDOW_CLASS, "",
                              style,
                              x, y,
                              rect.right - rect.left,
                              rect.bottom - rect.top,
                              parent ? parent->window : NULL,
                              NULL, _ecore_win32_instance, NULL);
   if (!w->window)
     {
        ERR("CreateWindowEx() failed");
        free(w);
        return NULL;
     }

   SetLastError(0);
   if (!SetWindowLongPtr(w->window, GWLP_USERDATA, (LONG_PTR)w) &&
       (GetLastError() != 0))
     {
        ERR("SetWindowLongPtr() failed");
        DestroyWindow(w->window);
        free(w);
        return NULL;
     }

   w->pos_hints.flags = 0;
   w->pos_hints.min_width   = w->mininal_window_width;
   w->pos_hints.min_height  = 0;
   w->pos_hints.max_width   = 32767;
   w->pos_hints.max_height  = 32767;
   w->pos_hints.base_width  = w->mininal_window_width;
   w->pos_hints.base_height = 0;
   w->pos_hints.step_width  = 0;
   w->pos_hints.step_height = 0;

   w->state.iconified         = 0;
   w->state.modal             = 0;
   w->state.sticky            = 0;
   w->state.maximized_vert    = 0;
   w->state.maximized_horz    = 0;
   w->state.shaded            = 0;
   w->state.hidden            = 0;
   w->state.fullscreen        = 0;
   w->state.above             = 0;
   w->state.below             = 0;
   w->state.demands_attention = 0;

   w->type.desktop = 0;
   w->type.dock    = 0;
   w->type.toolbar = 0;
   w->type.menu    = 0;
   w->type.utility = 0;
   w->type.splash  = 0;
   w->type.dialog  = 0;
   w->type.normal  = 0;

   w->pointer_is_in = 0;
   w->borderless    = 0;
   w->iconified     = 0;
   w->fullscreen    = 0;

   w->drag.x = x;
   w->drag.y = y;
   w->drag.w = rect.right - rect.left;
   w->drag.h = rect.bottom - rect.top;
   w->drag.current_mouse_x = -32768;
   w->drag.current_mouse_y = -32768;

   w->cursor = LoadCursor (NULL, IDC_ARROW);

   return w;
}

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

Eina_Bool
ecore_win32_window_drag(Ecore_Win32_Window *w, int ptx, int pty)
{
   if (w->drag.type == HTCAPTION)
     {
        int dx;
        int dy;

        dx = ptx - w->drag.px;
        dy = pty - w->drag.py;
        if ((dx == 0) && (dy == 0))
          return EINA_TRUE;

        ecore_win32_window_move(w, w->drag.x + dx, w->drag.y + dy);
        return EINA_TRUE;
     }
   if (w->drag.type == HTLEFT)
     {
        int dw;

        dw = ptx - w->drag.px;

        if (w->pos_hints.flags & ECORE_WIN32_POS_HINTS_STEP_SIZE)
          {
             int new_width;

             new_width = w->pos_hints.base_width + ((w->drag.w - w->pos_hints.base_width - dw) / w->pos_hints.step_width) * w->pos_hints.step_width;
             if ((new_width != w->drag.w) &&
                 (new_width >= w->pos_hints.base_width) &&
                 (new_width <= w->pos_hints.max_width))
               ecore_win32_window_move_resize(w, w->drag.x - (new_width - w->drag.w), w->drag.y, new_width, w->drag.h);
          }
        else
          {
             if (((w->drag.w - dw) >= w->pos_hints.min_width) &&
                 ((w->drag.w - dw) <= w->pos_hints.max_width))
               ecore_win32_window_move_resize(w, w->drag.x + dw, w->drag.y, w->drag.w - dw, w->drag.h);
          }
        return EINA_TRUE;
     }
   if (w->drag.type == HTRIGHT)
     {
        int dw;

        dw = ptx - w->drag.px;

        if (w->pos_hints.flags & ECORE_WIN32_POS_HINTS_STEP_SIZE)
          {
             int new_width;

             new_width = w->pos_hints.base_width + ((w->drag.w - w->pos_hints.base_width + dw) / w->pos_hints.step_width) * w->pos_hints.step_width;
             if ((new_width != w->drag.w) &&
                 (new_width >= w->pos_hints.base_width) &&
                 (new_width <= w->pos_hints.max_width))
               ecore_win32_window_resize(w, new_width, w->drag.h);
          }
        else
          {
             if (((w->drag.w + dw) >= w->pos_hints.min_width) &&
                 ((w->drag.w + dw) <= w->pos_hints.max_width))
               ecore_win32_window_resize(w, w->drag.w + dw, w->drag.h);
          }
        return EINA_TRUE;
     }
   if (w->drag.type == HTTOP)
     {
        int dh;

        dh = pty - w->drag.py;

        if (w->pos_hints.flags & ECORE_WIN32_POS_HINTS_STEP_SIZE)
          {
             int new_height;

             new_height = w->pos_hints.base_height + ((w->drag.h - w->pos_hints.base_height - dh) / w->pos_hints.step_height) * w->pos_hints.step_height;
             if ((new_height != w->drag.h) &&
                 (new_height >= w->pos_hints.base_height) &&
                 (new_height <= w->pos_hints.max_height))
               ecore_win32_window_move_resize(w, w->drag.x, w->drag.y - (new_height - w->drag.h), w->drag.w, new_height);
          }
        else
          {
             if ((dh != 0) &&
                 ((w->drag.h - dh) >= w->pos_hints.min_height) &&
                 ((w->drag.h - dh) <= w->pos_hints.max_height))
               ecore_win32_window_move_resize(w, w->drag.x, w->drag.y + dh, w->drag.w, w->drag.h - dh);
          }
        return EINA_TRUE;
     }
   if (w->drag.type == HTBOTTOM)
     {
        int dh;

        dh = pty - w->drag.py;

        if (w->pos_hints.flags & ECORE_WIN32_POS_HINTS_STEP_SIZE)
          {
             int new_height;

             new_height = w->pos_hints.base_height + ((w->drag.h - w->pos_hints.base_height + dh) / w->pos_hints.step_height) * w->pos_hints.step_height;
             if ((new_height != w->drag.h) &&
                 (new_height >= w->pos_hints.base_height) &&
                 (new_height <= w->pos_hints.max_height))
               ecore_win32_window_resize(w, w->drag.w, new_height);
          }
        else
          {
             if (((w->drag.h + dh) >= w->pos_hints.min_height) &&
                 ((w->drag.h + dh) <= w->pos_hints.max_height))
               ecore_win32_window_resize(w, w->drag.w, w->drag.h + dh);
          }
        return EINA_TRUE;
     }
   if (w->drag.type == HTTOPLEFT)
     {
        int dh;
        int dw;

        dw = ptx - w->drag.px;
        dh = pty - w->drag.py;

        if (w->pos_hints.flags & ECORE_WIN32_POS_HINTS_STEP_SIZE)
          {
             int new_width;
             int new_height;

             new_width = w->pos_hints.base_width + ((w->drag.w - w->pos_hints.base_width - dw) / w->pos_hints.step_width) * w->pos_hints.step_width;
             new_height = w->pos_hints.base_height + ((w->drag.h - w->pos_hints.base_height - dh) / w->pos_hints.step_height) * w->pos_hints.step_height;
             if ((new_width != w->drag.w) &&
                 (new_width >= w->pos_hints.base_width) &&
                 (new_width <= w->pos_hints.max_width) &&
                 (new_height != w->drag.h) &&
                 (new_height >= w->pos_hints.base_height) &&
                 (new_height <= w->pos_hints.max_height))
               ecore_win32_window_move_resize(w, w->drag.x - (new_width - w->drag.w), w->drag.y - (new_height - w->drag.h), new_width, new_height);
          }
        else
          {
             if (((w->drag.w - dw) >= w->pos_hints.min_width) &&
                 ((w->drag.w - dw) <= w->pos_hints.max_width) &&
                 ((w->drag.h - dh) >= w->pos_hints.min_height) &&
                 ((w->drag.h - dh) <= w->pos_hints.max_height))
               ecore_win32_window_move_resize(w, w->drag.x + dw, w->drag.y + dh, w->drag.w - dw, w->drag.h - dh);
          }
        return EINA_TRUE;
     }
   if (w->drag.type == HTTOPRIGHT)
     {
        int dh;
        int dw;

        dw = ptx - w->drag.px;
        dh = pty - w->drag.py;

        if (w->pos_hints.flags & ECORE_WIN32_POS_HINTS_STEP_SIZE)
          {
             int new_width;
             int new_height;

             new_width = w->pos_hints.base_width + ((w->drag.w - w->pos_hints.base_width + dw) / w->pos_hints.step_width) * w->pos_hints.step_width;
             new_height = w->pos_hints.base_height + ((w->drag.h - w->pos_hints.base_height - dh) / w->pos_hints.step_height) * w->pos_hints.step_height;
             if ((new_width != w->drag.w) &&
                 (new_width >= w->pos_hints.base_width) &&
                 (new_width <= w->pos_hints.max_width) &&
                 (new_height != w->drag.h) &&
                 (new_height >= w->pos_hints.base_height) &&
                 (new_height <= w->pos_hints.max_height))
               ecore_win32_window_move_resize(w, w->drag.x, w->drag.y - (new_height - w->drag.h), new_width, new_height);
          }
        else
          {
             if (((w->drag.w + dw) >= w->pos_hints.min_width) &&
                 ((w->drag.w + dw) <= w->pos_hints.max_width) &&
                 ((w->drag.h - dh) >= w->pos_hints.min_height) &&
                 ((w->drag.h - dh) <= w->pos_hints.max_height))
               ecore_win32_window_move_resize(w, w->drag.x, w->drag.y + dh, w->drag.w + dw, w->drag.h - dh);
          }
        return EINA_TRUE;
     }
   if (w->drag.type == HTBOTTOMLEFT)
     {
        int dh;
        int dw;

        dw = ptx - w->drag.px;
        dh = pty - w->drag.py;

        if (w->pos_hints.flags & ECORE_WIN32_POS_HINTS_STEP_SIZE)
          {
             int new_width;
             int new_height;

             new_width = w->pos_hints.base_width + ((w->drag.w - w->pos_hints.base_width - dw) / w->pos_hints.step_width) * w->pos_hints.step_width;
             new_height = w->pos_hints.base_height + ((w->drag.h - w->pos_hints.base_height + dh) / w->pos_hints.step_height) * w->pos_hints.step_height;
             if ((new_width != w->drag.w) &&
                 (new_width >= w->pos_hints.base_width) &&
                 (new_width <= w->pos_hints.max_width) &&
                 (new_height != w->drag.h) &&
                 (new_height >= w->pos_hints.base_height) &&
                 (new_height <= w->pos_hints.max_height))
               ecore_win32_window_move_resize(w, w->drag.x - (new_width - w->drag.w), w->drag.y, new_width, new_height);
          }
        else
          {
             if (((w->drag.w - dw) >= w->pos_hints.min_width) &&
                 ((w->drag.w - dw) <= w->pos_hints.max_width) &&
                 ((w->drag.h + dh) >= w->pos_hints.min_height) &&
                 ((w->drag.h + dh) <= w->pos_hints.max_height))
               ecore_win32_window_move_resize(w, w->drag.x + dw, w->drag.y, w->drag.w - dw, w->drag.h + dh);
          }
        return EINA_TRUE;
     }
   if (w->drag.type == HTBOTTOMRIGHT)
     {
        int dh;
        int dw;

        dw = ptx - w->drag.px;
        dh = pty - w->drag.py;

        if (w->pos_hints.flags & ECORE_WIN32_POS_HINTS_STEP_SIZE)
          {
             int new_width;
             int new_height;

             new_width = w->pos_hints.base_width + ((w->drag.w - w->pos_hints.base_width + dw) / w->pos_hints.step_width) * w->pos_hints.step_width;
             new_height = w->pos_hints.base_height + ((w->drag.h - w->pos_hints.base_height + dh) / w->pos_hints.step_height) * w->pos_hints.step_height;
             if ((new_width != w->drag.w) &&
                 (new_width >= w->pos_hints.base_width) &&
                 (new_width <= w->pos_hints.max_width) &&
                 (new_height != w->drag.h) &&
                 (new_height >= w->pos_hints.base_height) &&
                 (new_height <= w->pos_hints.max_height))
               ecore_win32_window_resize(w, new_width, new_height);
          }
        else
          {
             if (((w->drag.w + dw) >= w->pos_hints.min_width) &&
                 ((w->drag.w + dw) <= w->pos_hints.max_width) &&
                 ((w->drag.h + dh) >= w->pos_hints.min_height) &&
                 ((w->drag.h + dh) <= w->pos_hints.max_height))
               ecore_win32_window_resize(w, w->drag.w + dw, w->drag.h + dh);
          }
        return EINA_TRUE;
     }

   return EINA_FALSE;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

/**
 * @addtogroup Ecore_Win32_Group Ecore_Win32 library
 *
 * @{
 */

/**
 * @brief Creates a new window.
 *
 * @param parent The parent window.
 * @param x The x coordinate of the top-left corner of the window.
 * @param y The y coordinate of the top-left corner of the window.
 * @param width The width of the window.
 * @param height The height of the window.
 * @return A newly allocated window.
 *
 * This function creates a new window which parent is @p parent. @p width and
 * @p height are the size of the window content (the client part),
 * without the border and title bar. @p x and @p y are the system
 * coordinates of the top left cerner of the window (that is, of the
 * title bar). This function returns a newly created window on
 * success, and @c NULL on failure.
 */
ECORE_WIN32_API Ecore_Win32_Window *
ecore_win32_window_new(Ecore_Win32_Window *parent,
                       int                 x,
                       int                 y,
                       int                 width,
                       int                 height)
{
   INF("creating window with border");

   return _ecore_win32_window_internal_new(parent,
                                           x, y,
                                           width, height,
                                           WS_OVERLAPPEDWINDOW | WS_SIZEBOX);
}

/**
 * @brief Creates a new borderless window.
 *
 * @param parent The parent window.
 * @param x The x coordinate of the top-left corner of the window.
 * @param y The y coordinate of the top-left corner of the window.
 * @param width The width of the window.
 * @param height The height of the window.
 * @return A newly allocated window.
 *
 * This function is the same than ecore_win32_window_override_new()
 * but the returned window is borderless.
 */
ECORE_WIN32_API Ecore_Win32_Window *
ecore_win32_window_override_new(Ecore_Win32_Window *parent,
                                int                 x,
                                int                 y,
                                int                 width,
                                int                 height)
{
   INF("creating window without border");

   return _ecore_win32_window_internal_new(parent,
                                           x, y,
                                           width, height,
                                           WS_POPUP & ~(WS_CAPTION | WS_THICKFRAME));
}

/**
 * @brief Free the given window.
 *
 * @param window The window to free.
 *
 * This function frees @p window. If @p window is @c NULL, this
 * function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_free(Ecore_Win32_Window *window)
{
   if (!window) return;

   INF("destroying window");

   if (window->shape.mask)
      free(window->shape.mask);

   DestroyWindow(window->window);
   free(window);
}

/**
 * @brief Return the window HANDLE associated to the given window.
 *
 * @param window The window to retrieve the HANDLE from.
 *
 * This function returns the window HANDLE associated to @p window. If
 * @p window is @c NULL, this function returns @c NULL.
 *
 * @note The returned value is of type HWND.
 */
ECORE_WIN32_API void *
ecore_win32_window_hwnd_get(Ecore_Win32_Window *window)
{
   if (!window) return NULL;

   return window->window;
}

/*
void
ecore_win32_window_configure(Ecore_Win32_Window        *window,
                             Ecore_Win32_Window_Z_Order order,
                             int                        x,
                             int                        y,
                             int                        width,
                             int                        height)
{
  HWND w;

  switch (order)
    {
    case ECORE_WIN32_WINDOW_Z_ORDER_BOTTOM:
      w = HWND_BOTTOM;
      break;
    case ECORE_WIN32_WINDOW_Z_ORDER_NOTOPMOST:
      w = HWND_NOTOPMOST;
      break;
    case ECORE_WIN32_WINDOW_Z_ORDER_TOP:
      w = HWND_TOP;
      break;
    case ECORE_WIN32_WINDOW_Z_ORDER_TOPMOST:
      w = HWND_TOPMOST;
      break;
    default:
      return;
    }
  SetWindowPos((Ecore_Win32_Window *)window->window, w, x, y, width, height, ???);
}
*/

/**
 * @brief Move the given window to a given position.
 *
 * @param window The window to move.
 * @param x The x coordinate of the destination position.
 * @param y The y coordinate of the destination position.
 *
 * This function moves @p window to the new position of coordinates @p x
 * and @p y. If @p window is @c NULL, or if it is fullscreen, or on
 * error, this function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_move(Ecore_Win32_Window *window,
                        int                 x,
                        int                 y)
{
   RECT rect;

   /* FIXME: on fullscreen, should not move it */
   if (!window) return;

   INF("moving window (%dx%d)", x, y);

   if (!GetWindowRect(window->window, &rect))
     {
        ERR("GetWindowRect() failed");
        return;
     }

   if (!MoveWindow(window->window, x, y,
                   rect.right - rect.left,
                   rect.bottom - rect.top,
                   TRUE))
     {
        ERR("MoveWindow() failed");
     }
}

/**
 * @brief Resize the given window to a given size.
 *
 * @param window The window to resize.
 * @param width The new width.
 * @param height The new height.
 *
 * This function resizes @p window to the new @p width and @p height.
 * If @p window is @c NULL, or if it is fullscreen, or on error, this
 * function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_resize(Ecore_Win32_Window *window,
                          int                 width,
                          int                 height)
{
   RECT rect;
   DWORD style;
   int x;
   int y;

   /* FIXME: on fullscreen, should not resize it */
   if (!window) return;

   INF("resizing window (%dx%d)", width, height);

   if (!GetWindowRect(window->window, &rect))
     {
        ERR("GetWindowRect() failed");
        return;
     }

   x = rect.left;
   y = rect.top;
   rect.left = 0;
   rect.top = 0;
   if (width < window->pos_hints.min_width) width = window->pos_hints.min_width;
   if (width > window->pos_hints.max_width) width = window->pos_hints.max_width;
   if (height < window->pos_hints.min_height) height = window->pos_hints.min_height;
   if (height > window->pos_hints.max_height) height = window->pos_hints.max_height;
   rect.right = width;
   rect.bottom = height;
   if (!(style = GetWindowLong(window->window, GWL_STYLE)))
     {
        ERR("GetWindowLong() failed");
        return;
     }
   if (!AdjustWindowRect(&rect, style, FALSE))
     {
        ERR("AdjustWindowRect() failed");
        return;
     }

   if (!MoveWindow(window->window, x, y,
                   rect.right - rect.left,
                   rect.bottom - rect.top,
                   TRUE))
     {
        ERR("MoveWindow() failed");
     }
}

/**
 * @brief Move and resize the given window to a given position and size.
 *
 * @param window The window to move and resize.
 * @param x The x coordinate of the destination position.
 * @param y The x coordinate of the destination position.
 * @param width The new width.
 * @param height The new height.
 *
 * This function resizes @p window to the new position of coordinates @p x
 * and @p y and the new @p width and @p height. If @p window is @c NULL,
 * or if it is fullscreen, or on error, this function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_move_resize(Ecore_Win32_Window *window,
                               int                 x,
                               int                 y,
                               int                 width,
                               int                 height)
{
   RECT rect;
   DWORD style;

   /* FIXME: on fullscreen, should not move/resize it */
   if (!window) return;

   INF("moving and resizing window (%dx%d %dx%d)", x, y, width, height);

   rect.left = 0;
   rect.top = 0;
   if (width < window->pos_hints.min_width) width = window->pos_hints.min_width;
   if (width > window->pos_hints.max_width) width = window->pos_hints.max_width;
   if (height < window->pos_hints.min_height) height = window->pos_hints.min_height;
   if (height > window->pos_hints.max_height) height = window->pos_hints.max_height;
   rect.right = width;
   rect.bottom = height;
   if (!(style = GetWindowLong(window->window, GWL_STYLE)))
     {
        ERR("GetWindowLong() failed");
        return;
     }
   if (!AdjustWindowRect(&rect, style, FALSE))
     {
        ERR("AdjustWindowRect() failed");
        return;
     }

   if (!MoveWindow(window->window, x, y,
                   rect.right - rect.left,
                   rect.bottom - rect.top,
                   TRUE))
     {
        ERR("MoveWindow() failed");
     }
}

/**
 * @brief Get the geometry of the given window.
 *
 * @param window The window to retrieve the geometry from.
 * @param x The x coordinate of the position.
 * @param y The x coordinate of the position.
 * @param width The width.
 * @param height The height.
 *
 * This function retrieves the position and size of @p window. @p x,
 * @p y, @p width and @p height can be buffers that will be filled with
 * the corresponding values. If one of them is @c NULL, nothing will
 * be done for that parameter. If @p window is @c NULL, and if the
 * buffers are not @c NULL, they will be filled with respectively 0,
 * 0, the size of the screen and the height of the screen.
 */
ECORE_WIN32_API void
ecore_win32_window_geometry_get(Ecore_Win32_Window *window,
                                int                *x,
                                int                *y,
                                int                *width,
                                int                *height)
{
   RECT rect;
   int  w;
   int  h;

   INF("getting window geometry");

   if (!window)
     {
        if (x) *x = 0;
        if (y) *y = 0;
        if (width) *width = 0;
        if (height) *height = 0;

        return;
     }

   if (!GetClientRect(window->window, &rect))
     {
        ERR("GetClientRect() failed");

        if (x) *x = 0;
        if (y) *y = 0;
        if (width) *width = 0;
        if (height) *height = 0;

        return;
     }

   w = rect.right - rect.left;
   h = rect.bottom - rect.top;

   if (!GetWindowRect(window->window, &rect))
     {
        ERR("GetWindowRect() failed");

        if (x) *x = 0;
        if (y) *y = 0;
        if (width) *width = 0;
        if (height) *height = 0;

        return;
     }

   if (x) *x = rect.left;
   if (y) *y = rect.top;
   if (width) *width = w;
   if (height) *height = h;
}

/**
 * @brief Get the size of the given window.
 *
 * @param window The window to retrieve the size from.
 * @param width The width.
 * @param height The height.
 *
 * This function retrieves the size of @p window. @p width and
 * @p height can be buffers that will be filled with the corresponding
 * values. If one of them is @c NULL, nothing will be done for that
 * parameter. If @p window is @c NULL, and if the buffers are not
 * @c NULL, they will be filled with respectively the size of the screen
 * and the height of the screen.
 */
ECORE_WIN32_API void
ecore_win32_window_size_get(Ecore_Win32_Window *window,
                            int                *width,
                            int                *height)
{
   RECT rect;

   INF("getting window size");

   if (!window)
     {
        if (width) *width = 0;
        if (height) *height = 0;

        return;
     }

   if (!GetClientRect(window->window, &rect))
     {
        ERR("GetClientRect() failed");

        if (width) *width = 0;
        if (height) *height = 0;
     }

   if (width) *width = rect.right - rect.left;
   if (height) *height = rect.bottom - rect.top;
}

/**
 * @brief Set the minimum size of the given window.
 *
 * @param window The window.
 * @param min_width The minimal width.
 * @param min_height The minimal height.
 *
 * This function sets the minimum size of @p window to @p min_width
 * and *p min_height. If @p window is @c NULL, this functions does
 * nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_size_min_set(Ecore_Win32_Window *window,
                                int                 min_width,
                                int                 min_height)
{
   if (!window) return;

   INF("setting minimum window size to %dx%d", min_width, min_height);

   if ((min_width > 0) && (min_height > 0))
     {
        if (min_width < window->mininal_window_width)
          min_width = window->mininal_window_width;

        window->pos_hints.flags |= ECORE_WIN32_POS_HINTS_MIN_SIZE;
        window->pos_hints.min_width = min_width;
        window->pos_hints.min_height = min_height;
        if (!(window->pos_hints.flags & ECORE_WIN32_POS_HINTS_BASE_SIZE))
          {
             window->pos_hints.base_width = min_width;
             window->pos_hints.base_height = min_height;
          }
     }
}

/**
 * @brief Get the minimum size of the given window.
 *
 * @param window The window.
 * @param min_width The minimal width.
 * @param min_height The minimal height.
 *
 * This function fills the minimum size of @p window in the buffers
 * @p min_width and *p min_height. They both can be @c NULL. If
 * @p window is @c NULL, this functions does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_size_min_get(Ecore_Win32_Window *window,
                                int                *min_width,
                                int                *min_height)
{
   if (!window || !(window->pos_hints.flags & ECORE_WIN32_POS_HINTS_MIN_SIZE))
     {
        if (min_width) *min_width = 0;
        if (min_height) *min_height = 0;
        return;
     }

   INF("getting minimum window size: %dx%d", window->pos_hints.min_width, window->pos_hints.min_height);

   if (min_width) *min_width = window->pos_hints.min_width;
   if (min_height) *min_height = window->pos_hints.min_height;
}

/**
 * @brief Set the maximum size of the given window.
 *
 * @param window The window.
 * @param max_width The maximal width.
 * @param max_height The maximal height.
 *
 * This function sets the maximum size of @p window to @p max_width
 * and *p max_height. If @p window is @c NULL, this functions does
 * nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_size_max_set(Ecore_Win32_Window *window,
                                int                 max_width,
                                int                 max_height)
{
   if (!window) return;

   INF("setting maximum window size to %dx%d", max_width, max_height);

   if ((max_width > 0) && (max_height > 0))
     {
        window->pos_hints.flags |= ECORE_WIN32_POS_HINTS_MAX_SIZE;
        window->pos_hints.max_width = max_width;
        window->pos_hints.max_height = max_height;
     }
}

/**
 * @brief Get the maximum size of the given window.
 *
 * @param window The window.
 * @param max_width The maximal width.
 * @param max_height The maximal height.
 *
 * This function fills the maximum size of @p window in the buffers
 * @p max_width and *p max_height. They both can be @c NULL. If
 * @p window is @c NULL, this functions does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_size_max_get(Ecore_Win32_Window *window,
                                int                *max_width,
                                int                *max_height)
{
   if (!window || !(window->pos_hints.flags & ECORE_WIN32_POS_HINTS_MAX_SIZE))
     {
        if (max_width) *max_width = 0;
        if (max_height) *max_height = 0;
        return;
     }

   INF("getting maximum window size: %dx%d", window->pos_hints.max_width, window->pos_hints.max_height);

   if (max_width) *max_width = window->pos_hints.max_width;
   if (max_height) *max_height = window->pos_hints.max_height;
}

/**
 * @brief Set the base size of the given window.
 *
 * @param window The window.
 * @param base_width The base width.
 * @param base_height The base height.
 *
 * This function sets the base size of @p window to @p base_width
 * and *p base_height. If @p window is @c NULL, this functions does
 * nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_size_base_set(Ecore_Win32_Window *window,
                                 int                 base_width,
                                 int                 base_height)
{
   if (!window) return;

   INF("setting base window size to %dx%d", base_width, base_height);

   if ((base_width > 0) && (base_height > 0))
     {
        window->pos_hints.flags |= ECORE_WIN32_POS_HINTS_BASE_SIZE;
        window->pos_hints.base_width = base_width;
        window->pos_hints.base_height = base_height;
     }
}

/**
 * @brief Get the base size of the given window.
 *
 * @param window The window.
 * @param base_width The base width.
 * @param base_height The bas height.
 *
 * This function fills the base size of @p window in the buffers
 * @p base_width and *p base_height. They both can be @c NULL. If
 * @p window is @c NULL, this functions does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_size_base_get(Ecore_Win32_Window *window,
                                 int                *base_width,
                                 int                *base_height)
{
   if (!window || !(window->pos_hints.flags & ECORE_WIN32_POS_HINTS_BASE_SIZE))
     {
        if (base_width) *base_width = 0;
        if (base_height) *base_height = 0;
        return;
     }

   INF("getting base window size: %dx%d", window->pos_hints.base_width, window->pos_hints.base_height);

   if (base_width) *base_width = window->pos_hints.base_width;
   if (base_height) *base_height = window->pos_hints.base_height;
}

/**
 * @brief Set the step size of the given window.
 *
 * @param window The window.
 * @param step_width The step width.
 * @param step_height The step height.
 *
 * This function sets the step size of @p window to @p step_width
 * and *p step_height. If @p window is @c NULL, this functions does
 * nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_size_step_set(Ecore_Win32_Window *window,
                                 int                 step_width,
                                 int                 step_height)
{
   if (!window) return;

   INF("setting step window size to %dx%d", step_width, step_height);

   if ((step_width > 0) && (step_height > 0))
     {
        window->pos_hints.flags |= ECORE_WIN32_POS_HINTS_STEP_SIZE;
        window->pos_hints.step_width = step_width;
        window->pos_hints.step_height = step_height;
     }
}

/**
 * @brief Get the step size of the given window.
 *
 * @param window The window.
 * @param step_width The step width.
 * @param step_height The bas height.
 *
 * This function fills the step size of @p window in the buffers
 * @p step_width and *p step_height. They both can be @c NULL. If
 * @p window is @c NULL, this functions does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_size_step_get(Ecore_Win32_Window *window,
                                 int                *step_width,
                                 int                *step_height)
{
   if (!window || !(window->pos_hints.flags & ECORE_WIN32_POS_HINTS_STEP_SIZE))
     {
        if (step_width) *step_width = 0;
        if (step_height) *step_height = 0;
        return;
     }

   INF("getting step window size: %dx%d", window->pos_hints.step_width, window->pos_hints.step_height);

   if (step_width) *step_width = window->pos_hints.step_width;
   if (step_height) *step_height = window->pos_hints.step_height;
}

/**
 * @brief Show the given window.
 *
 * @param window The window to show.
 *
 * This function shows @p window. If @p window is @c NULL, or on
 * error, this function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_show(Ecore_Win32_Window *window)
{
   if (!window) return;

   INF("showing window");

   ShowWindow(window->window, SW_SHOWNORMAL);
   if (!UpdateWindow(window->window))
     {
        ERR("UpdateWindow() failed");
     }
}

/* FIXME: seems to block the taskbar */
/**
 * @brief Hide the given window.
 *
 * @param window The window to show.
 *
 * This function hides @p window. If @p window is @c NULL, or on
 * error, this function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_hide(Ecore_Win32_Window *window)
{
   if (!window) return;

   INF("hiding window");

   ShowWindow(window->window, SW_HIDE);
}

/**
 * @brief Place the given window at the top of the Z order.
 *
 * @param window The window to place at the top.
 *
 * This function places @p window at the top of the Z order. If
 * @p window is @c NULL, this function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_raise(Ecore_Win32_Window *window)
{
   if (!window) return;

   INF("raising window");

   if (!SetWindowPos(window->window,
                     HWND_TOP, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE))
     {
        ERR("SetWindowPos() failed");
     }
}

/**
 * @brief Place the given window at the bottom of the Z order.
 *
 * @param window The window to place at the bottom.
 *
 * This function places @p window at the bottom of the Z order. If
 * @p window is @c NULL, this function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_lower(Ecore_Win32_Window *window)
{
   if (!window) return;

   INF("lowering window");

   if (!SetWindowPos(window->window,
                     HWND_BOTTOM, 0, 0, 0, 0,
                     SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE))
     {
        ERR("SetWindowPos() failed");
     }
}

/**
 * @brief Set the title of the given window.
 *
 * @param window The window to set the title.
 * @param title The new title.
 *
 * This function sets the title of @p window to @p title. If @p window
 * is @c NULL, or if @p title is @c NULL or empty, or on error, this
 * function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_title_set(Ecore_Win32_Window *window,
                             const char         *title)
{
   if (!window) return;

   if (!title || !title[0]) return;

   INF("setting window title");

   if (!SetWindowText(window->window, title))
     {
        ERR("SetWindowText() failed");
     }
}

/**
 * @brief Set the focus to the given window.
 *
 * @param window The window to give focus to.
 *
 * This function gives the focus to @p window. If @p window is
 * @c NULL, this function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_focus(Ecore_Win32_Window *window)
{
   if (!window) return;

   INF("focusing window");

   if (!SetFocus(window->window))
     {
        ERR("SetFocus() failed");
     }
}

/**
 * @brief Get the current focused window.
 *
 * @return The window that has focus.
 *
 * This function returns the window that has focus. If the calling
 * thread's message queue does not have an associated window with the
 * keyboard focus, the return value is @c NULL.
 *
 * @note Even if the returned value is @c NULL, another thread's queue
 * may be associated with a window that has the keyboard focus.
 *
 * @note The returned value is of type HWND.
 */
ECORE_WIN32_API void *
ecore_win32_window_focus_get(void)
{
   HWND focused;

   INF("getting focused window");

   focused = GetFocus();
   if (!focused)
     {
        ERR("GetFocus() failed");
        return NULL;
     }

   return focused;
}

/**
 * @brief Iconify or restore the given window.
 *
 * @param window The window.
 * @param on @c EINA_TRUE to iconify the window, @c EINA_FALSE to restore it.
 *
 * This function iconifies or restores @p window. If @p on is set to
 * @c EINA_TRUE, the window will be iconified, if it is set to @c EINA_FALSE,
 * it will be restored. If @p window is @c NULL or if the state does not change
 * (like iconifying the window while it is already iconified), this function
 * does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_iconified_set(Ecore_Win32_Window *window,
                                 Eina_Bool           on)
{
   if (!window) return;

   if (((window->iconified) && (on)) ||
       ((!window->iconified) && (!on)))
     return;

   INF("iconifying window: %s", on ? "yes" : "no");

   ShowWindow(window->window, on ? SW_MINIMIZE : SW_RESTORE);
   window->iconified = on;
}

/**
 * @brief Remove or restore the border of the given window.
 *
 * @param window The window.
 * @param on @c EINA_TRUE to remove the border, @c EINA_FALSE to restore it.
 *
 * This function removes or restores the border of @p window. If @p on is set to
 * @c EINA_TRUE, the window will have no border, if it is set to @c EINA_FALSE,
 * it will have a border. If @p window is @c NULL or if the state does not
 * change (like setting to borderless while the window has no border), this
 * function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_borderless_set(Ecore_Win32_Window *window,
                                  Eina_Bool           on)
{
   RECT                rect;
   DWORD               style;

   if (!window) return;

   if (((window->borderless) && (on)) ||
       ((!window->borderless) && (!on)))
     return;

   INF("setting window without border: %s", on ? "yes" : "no");

   style = GetWindowLong(window->window, GWL_STYLE);
   if (on)
     {
        if (!GetClientRect(window->window, &rect))
          {
             ERR("GetClientRect() failed");
             return;
          }
        SetLastError(0);
        if (!SetWindowLongPtr(window->window, GWL_STYLE, style & ~(WS_CAPTION | WS_THICKFRAME)) &&
            (GetLastError() != 0))
          {
             ERR("SetWindowLongPtr() failed");
             return;
          }
     }
   else
     {
        if (!GetWindowRect(window->window, &rect))
          {
             ERR("GetWindowRect() failed");
             return;
          }
        style |= WS_CAPTION | WS_THICKFRAME;
        if (!AdjustWindowRect (&rect, style, FALSE))
          {
             ERR("AdjustWindowRect() failed");
             return;
          }
        SetLastError(0);
        if (!SetWindowLongPtr(window->window, GWL_STYLE, style) &&
            (GetLastError() != 0))
          {
             ERR("SetWindowLongPtr() failed");
             return;
          }
     }
   if (!SetWindowPos(window->window, HWND_TOPMOST,
                     rect.left, rect.top,
                     rect.right - rect.left, rect.bottom - rect.top,
                     SWP_NOMOVE | SWP_FRAMECHANGED))
     {
        ERR("SetWindowPos() failed");
        return;
     }

   window->borderless = on;
}

/**
 * @brief Set the given window to fullscreen.
 *
 * @param window The window.
 * @param on @c EINA_TRUE for fullscreen mode, @c EINA_FALSE for windowed mode.
 *
 * This function sets @p window to fullscreen or windowed mode. If @p on is set
 * to @c EINA_TRUE, the window will be fullscreen, if it is set to
 * @c EINA_FALSE, it will be windowed. If @p window is @c NULL or if the state
 * does not change (like setting to fullscreen while the window is already
 * fullscreen), this function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_fullscreen_set(Ecore_Win32_Window *window,
                                  Eina_Bool           on)
{
   if (!window) return;

   if (((window->fullscreen) && (on)) ||
       ((!window->fullscreen) && (!on)))
     return;

   INF("setting fullscreen: %s", on ? "yes" : "no");

   window->fullscreen = !!on;
   window->state.fullscreen = !!on;

   if (on)
     {
        DWORD style;

        if (!GetWindowRect(window->window, &window->rect))
          {
             ERR("GetWindowRect() failed");
             return;
          }
        if (!(window->style = GetWindowLong(window->window, GWL_STYLE)))
          {
             ERR("GetWindowLong() failed");
             return;
          }
        style = window->style & ~WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX;
        style |= WS_VISIBLE | WS_POPUP;
        SetLastError(0);
        if (!SetWindowLongPtr(window->window, GWL_STYLE, style) &&
            (GetLastError() != 0))
          {
             ERR("SetWindowLongPtr() failed");
             return;
          }
        SetLastError(0);
        if (!SetWindowLongPtr(window->window, GWL_EXSTYLE, WS_EX_TOPMOST) &&
            (GetLastError() != 0))
          {
             ERR("SetWindowLongPtr() failed");
             return;
          }
        if (!SetWindowPos(window->window, HWND_TOPMOST, 0, 0,
                          GetSystemMetrics (SM_CXSCREEN),
                          GetSystemMetrics (SM_CYSCREEN),
                          SWP_NOCOPYBITS | SWP_SHOWWINDOW))
          {
             ERR("SetWindowPos() failed");
             return;
          }
     }
   else
     {
        SetLastError(0);
        if (!SetWindowLongPtr(window->window, GWL_STYLE, window->style) &&
            (GetLastError() != 0))
          {
             ERR("SetWindowLongPtr() failed");
             return;
          }
        SetLastError(0);
        if (!SetWindowLongPtr(window->window, GWL_EXSTYLE, 0) &&
            (GetLastError() != 0))
          {
             ERR("SetWindowLongPtr() failed");
             return;
          }
        if (!SetWindowPos(window->window, HWND_NOTOPMOST,
                          window->rect.left,
                          window->rect.top,
                          window->rect.right - window->rect.left,
                          window->rect.bottom - window->rect.top,
                          SWP_NOCOPYBITS | SWP_SHOWWINDOW))
          {
             ERR("SetWindowPos() failed");
             return;
          }
     }
}

/**
 * @brief Set the given cursor to the given window.
 *
 * @param window The window to modify the cursor.
 * @param cursor The new cursor.
 *
 * This function sets @p cursor to @p window. @p cursor must have been
 * obtained by ecore_win32_cursor_new() or
 * ecore_win32_cursor_shaped_new() or
 * ecore_win32_cursor_x11_shaped_new(). If @p window or @p cursor is
 * @c NULL, the function does nothing.
 *
 * @see ecore_win32_cursor_new()
 * @see ecore_win32_cursor_shaped_new()
 * @see ecore_win32_cursor_x11_shaped_new()
 */
ECORE_WIN32_API void
ecore_win32_window_cursor_set(Ecore_Win32_Window *window,
                              Ecore_Win32_Cursor *cursor)
{
   INF("setting cursor");

   if (!window || !cursor)
     return;

   window->cursor = cursor;
}

/**
 * @brief Set the state of the given window.
 *
 * @param window The window to modify the state.
 * @param state An array of the new states.
 * @param num The number of states in the array.
 *
 * This function sets the state of @p window. @p state is an array of
 * states of size @p num. If @p window or @p state are @c NULL, or if
 * @p num is less or equal than 0, the function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_state_set(Ecore_Win32_Window       *window,
                             Ecore_Win32_Window_State *state,
                             unsigned int              num)
{
   unsigned int i;

   if (!window || !state || (num == 0))
     return;

   INF("setting window state");

   for (i = 0; i < num; i++)
     {
        switch (state[i])
          {
          case ECORE_WIN32_WINDOW_STATE_ICONIFIED:
            window->state.iconified = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_MODAL:
            window->state.modal = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_STICKY:
            window->state.sticky = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_MAXIMIZED_VERT:
            window->state.maximized_vert = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_MAXIMIZED_HORZ:
            window->state.maximized_horz = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_MAXIMIZED:
            window->state.maximized_horz = 1;
            window->state.maximized_vert = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_SHADED:
            window->state.shaded = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_HIDDEN:
            window->state.hidden = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_FULLSCREEN:
            window->state.fullscreen = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_ABOVE:
            window->state.above = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_BELOW:
            window->state.below = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_DEMANDS_ATTENTION:
            window->state.demands_attention = 1;
            break;
          case ECORE_WIN32_WINDOW_STATE_UNKNOWN:
            /* nothing to be done */
            break;
          }
     }
}

 /**
 * @brief Get the states of the given window.
 *
 * @param window The window to retrieve the state from.
 * @param state A pointer to an array of the states.
 * @param num A pointer to the number of states in the array.
 *
 * This function gets the states of @p window. @p state is a pointer to an array
 * of states of size @p num. If @p window is @c NULL, @p state and @p num must
 * not be @c NULL and point to respectively @c NULL and 0.
 *
 * @since 1.20
 */
ECORE_WIN32_API void
ecore_win32_window_state_get(Ecore_Win32_Window        *window,
                             Ecore_Win32_Window_State **state,
                             unsigned int              *num)
{
   Ecore_Win32_Window_State *st;
   unsigned int i;

   *state = NULL;
   *num = 0;

   if (!window)
     return;

   INF("getting window state");

   i = 0;
   /* we get the number of states which are set */
   if (window->state.iconified)
     i++;
   if (window->state.modal)
     i++;
   if (window->state.sticky)
     i++;
   if (window->state.maximized_vert)
     i++;
   if (window->state.maximized_horz)
     i++;
   if (window->state.maximized)
     i++;
   if (window->state.shaded)
     i++;
   if (window->state.hidden)
     i++;
   if (window->state.fullscreen)
     i++;
   if (window->state.above)
     i++;
   if (window->state.below)
     i++;
   if (window->state.demands_attention)
     i++;

   st = (Ecore_Win32_Window_State *)malloc(i * sizeof(Ecore_Win32_Window_State));
   if (!st)
     return;

   *num = i;
   *state = st;

   i = 0;

   if (window->state.iconified)
     st[i++] = ECORE_WIN32_WINDOW_STATE_ICONIFIED;
   if (window->state.modal)
     st[i++] = ECORE_WIN32_WINDOW_STATE_MODAL;
   if (window->state.sticky)
     st[i++] = ECORE_WIN32_WINDOW_STATE_STICKY;
   if (window->state.maximized_vert)
     st[i++] = ECORE_WIN32_WINDOW_STATE_MAXIMIZED_VERT;
   if (window->state.maximized_horz)
     st[i++] = ECORE_WIN32_WINDOW_STATE_MAXIMIZED_HORZ;
   if (window->state.maximized)
     st[i++] = ECORE_WIN32_WINDOW_STATE_MAXIMIZED;
   if (window->state.shaded)
     st[i++] = ECORE_WIN32_WINDOW_STATE_SHADED;
   if (window->state.hidden)
     st[i++] = ECORE_WIN32_WINDOW_STATE_HIDDEN;
   if (window->state.fullscreen)
     st[i++] = ECORE_WIN32_WINDOW_STATE_FULLSCREEN;
   if (window->state.above)
     st[i++] = ECORE_WIN32_WINDOW_STATE_ABOVE;
   if (window->state.below)
     st[i++] = ECORE_WIN32_WINDOW_STATE_BELOW;
   if (window->state.demands_attention)
     st[i++] = ECORE_WIN32_WINDOW_STATE_DEMANDS_ATTENTION;
}

/**
 * @brief Apply the modification of the state to the given window.
 *
 * @param window The window.
 * @param state The state to apply changes.
 * @param set The value of the state change.
 *
 * This function applies the modification of the state @p state of
 * @p window. @p set is used only for
 * #ECORE_WIN32_WINDOW_STATE_ICONIFIED and
 * #ECORE_WIN32_WINDOW_STATE_FULLSCREEN. If @p window is @c NULL, the
 * function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_state_request_send(Ecore_Win32_Window      *window,
                                      Ecore_Win32_Window_State state,
                                      unsigned int             set)
{
   if (!window) return;

   INF("sending cursor state");

   switch (state)
     {
      case ECORE_WIN32_WINDOW_STATE_ICONIFIED:
         if (window->state.iconified)
           ecore_win32_window_iconified_set(window, set);
         break;
      case ECORE_WIN32_WINDOW_STATE_MODAL:
         window->state.modal = 1;
         break;
      case ECORE_WIN32_WINDOW_STATE_STICKY:
         window->state.sticky = 1;
         break;
      case ECORE_WIN32_WINDOW_STATE_MAXIMIZED_VERT:
         if (window->state.maximized_vert)
           {
              RECT rect;
              int  y;
              int  height;

              if (!SystemParametersInfo(SPI_GETWORKAREA, 0,
                                        &rect, 0))
                {
                   ERR("SystemParametersInfo() failed");
                   break;
                }
              y = rect.top;
              height = rect.bottom - rect.top;

              if (!GetClientRect(window->window, &rect))
                {
                   ERR("GetClientRect() failed");
                   break;
                }

              if (!MoveWindow(window->window, rect.left, y,
                              rect.right - rect.left,
                              height,
                              TRUE))
                {
                   ERR("MoveWindow() failed");
                }
           }
         break;
      case ECORE_WIN32_WINDOW_STATE_MAXIMIZED_HORZ:
         if (window->state.maximized_horz)
           {
              RECT rect;

              if (!GetClientRect(window->window, &rect))
                {
                   ERR("GetClientRect() failed");
                   break;
                }

              if (!MoveWindow(window->window, 0, rect.top,
                              GetSystemMetrics(SM_CXSCREEN),
                              rect.bottom - rect.top,
                              TRUE))
                {
                   ERR("MoveWindow() failed");
                }
           }
         break;
      case ECORE_WIN32_WINDOW_STATE_MAXIMIZED:
         if (window->state.maximized_vert && window->state.maximized_horz)
           {
              RECT rect;

              if (!SystemParametersInfo(SPI_GETWORKAREA, 0,
                                        &rect, 0))
                {
                   ERR("SystemParametersInfo() failed");
                   break;
                }

              if (!MoveWindow(window->window, 0, 0,
                              GetSystemMetrics(SM_CXSCREEN),
                              rect.bottom - rect.top,
                              TRUE))
                {
                   ERR("MoveWindow() failed");
                }
           }
         break;
      case ECORE_WIN32_WINDOW_STATE_SHADED:
         window->state.shaded = 1;
         break;
      case ECORE_WIN32_WINDOW_STATE_HIDDEN:
         window->state.hidden = 1;
         break;
      case ECORE_WIN32_WINDOW_STATE_FULLSCREEN:
         if (window->state.fullscreen)
           ecore_win32_window_fullscreen_set(window, set);
         break;
      case ECORE_WIN32_WINDOW_STATE_ABOVE:
         if (window->state.above)
           if (!SetWindowPos(window->window, HWND_TOP,
                             0, 0,
                             0, 0,
                             SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW))
             {
                ERR("SetWindowPos() failed");
             }
         break;
      case ECORE_WIN32_WINDOW_STATE_BELOW:
         if (window->state.below)
           if (!SetWindowPos(window->window, HWND_BOTTOM,
                             0, 0,
                             0, 0,
                             SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW))
             {
                ERR("SetWindowPos() failed");
             }
         break;
      case ECORE_WIN32_WINDOW_STATE_DEMANDS_ATTENTION:
         window->state.demands_attention = 1;
         break;
      case ECORE_WIN32_WINDOW_STATE_UNKNOWN:
         /* nothing to be done */
         break;
     }
}

/**
 * @brief Set the type of the given window.
 *
 * @param window The window to modify the type.
 * @param type The nwindow types.
 *
 * This function sets the type of @p window to @p type. If
 * @p window is @c NULL, the function does nothing.
 */
ECORE_WIN32_API void
ecore_win32_window_type_set(Ecore_Win32_Window      *window,
                            Ecore_Win32_Window_Type  type)
{
   if (!window)
     return;

   INF("setting window type");

   switch (type)
     {
     case ECORE_WIN32_WINDOW_TYPE_DESKTOP:
       window->type.desktop = 1;
       break;
     case ECORE_WIN32_WINDOW_TYPE_DOCK:
       window->type.dock = 1;
       break;
     case ECORE_WIN32_WINDOW_TYPE_TOOLBAR:
       window->type.toolbar = 1;
       break;
     case ECORE_WIN32_WINDOW_TYPE_MENU:
       window->type.menu = 1;
       break;
     case ECORE_WIN32_WINDOW_TYPE_UTILITY:
       window->type.utility = 1;
       break;
     case ECORE_WIN32_WINDOW_TYPE_SPLASH:
       window->type.splash = 1;
       break;
     case ECORE_WIN32_WINDOW_TYPE_DIALOG:
       window->type.dialog = 1;
       break;
     case ECORE_WIN32_WINDOW_TYPE_NORMAL:
       window->type.normal = 1;
       break;
     case ECORE_WIN32_WINDOW_TYPE_UNKNOWN:
       window->type.normal = 1;
       break;
     }
}

/**
 * @}
 */
