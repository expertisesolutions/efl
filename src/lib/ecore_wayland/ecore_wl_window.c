#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ecore_wl_private.h"

/* local function prototypes */
static void _ecore_wl_window_cb_ping(void *data EINA_UNUSED, struct wl_shell_surface *shell_surface, unsigned int serial);
static void _ecore_wl_window_cb_configure(void *data, struct wl_shell_surface *shell_surface EINA_UNUSED, unsigned int edges, int w, int h);
static void _ecore_wl_window_cb_popup_done(void *data, struct wl_shell_surface *shell_surface EINA_UNUSED);
static void _ecore_wl_window_cb_surface_enter(void *data, struct wl_surface *surface, struct wl_output *output EINA_UNUSED);
static void _ecore_wl_window_cb_surface_leave(void *data, struct wl_surface *surface, struct wl_output *output EINA_UNUSED);
static void _ecore_wl_window_configure_send(Ecore_Wl_Window *win, int w, int h, int edges);
static char *_ecore_wl_window_id_str_get(unsigned int win_id);

/* local variables */
static Eina_Hash *_windows = NULL;

/* wayland listeners */
static const struct wl_surface_listener _ecore_wl_surface_listener = 
{
   _ecore_wl_window_cb_surface_enter,
   _ecore_wl_window_cb_surface_leave
};

static const struct wl_shell_surface_listener _ecore_wl_shell_surface_listener = 
{
   _ecore_wl_window_cb_ping,
   _ecore_wl_window_cb_configure,
   _ecore_wl_window_cb_popup_done
};

/* internal functions */
void 
_ecore_wl_window_init(void)
{
   if (!_windows) 
     _windows = eina_hash_string_superfast_new(NULL);
}

void 
_ecore_wl_window_shutdown(void)
{
   eina_hash_free(_windows);
   _windows = NULL;
}

Eina_Hash *
_ecore_wl_window_hash_get(void)
{
   return _windows;
}

EAPI Ecore_Wl_Window *
ecore_wl_window_new(Ecore_Wl_Window *parent, int x, int y, int w, int h, int buffer_type)
{
   Ecore_Wl_Window *win;
   static int _win_id = 1;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!(win = malloc(sizeof(Ecore_Wl_Window))))
     {
        ERR("Failed to allocate an Ecore Wayland Window");
        return NULL;
     }

   memset(win, 0, sizeof(Ecore_Wl_Window));

   win->display = _ecore_wl_disp;
   win->parent = parent;
   win->allocation.x = x;
   win->allocation.y = y;
   win->allocation.w = w;
   win->allocation.h = h;
   win->saved.w = w;
   win->saved.h = h;
   win->transparent = EINA_FALSE;
   win->type = ECORE_WL_WINDOW_TYPE_TOPLEVEL;
   win->buffer_type = buffer_type;
   win->id = _win_id++;
   win->rotation = 0;

   win->opaque.x = x;
   win->opaque.y = y;
   win->opaque.w = w;
   win->opaque.h = h;

   win->title = NULL;
   win->class_name = NULL;

   eina_hash_add(_windows, _ecore_wl_window_id_str_get(win->id), win);
   return win;
}

EAPI void 
ecore_wl_window_free(Ecore_Wl_Window *win)
{
   Ecore_Wl_Input *input;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   eina_hash_del(_windows, _ecore_wl_window_id_str_get(win->id), win);

   EINA_INLIST_FOREACH(_ecore_wl_disp->inputs, input)
     {
        if ((input->pointer_focus) && (input->pointer_focus == win))
          input->pointer_focus = NULL;
        if ((input->keyboard_focus) && (input->keyboard_focus == win))
          input->keyboard_focus = NULL;
     }

   if (win->anim_callback) wl_callback_destroy(win->anim_callback);
   win->anim_callback = NULL;

   if (win->subsurfs) _ecore_wl_subsurfs_del_all(win);

   if (win->shell_surface) wl_shell_surface_destroy(win->shell_surface);
   win->shell_surface = NULL;
   if (win->surface) wl_surface_destroy(win->surface);
   win->surface = NULL;

   if (win->title) eina_stringshare_del(win->title);
   if (win->class_name) eina_stringshare_del(win->class_name);

   /* HMMM, why was this disabled ? */
   free(win);
}

EAPI void 
ecore_wl_window_move(Ecore_Wl_Window *win, int x, int y)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   ecore_wl_window_update_location(win, x, y);

   if (win->shell_surface)
     {
        Ecore_Wl_Input *input;

        if (!(input = win->keyboard_device))
          {
             if (win->parent)
               {
                  if (!(input = win->parent->keyboard_device))
                    input = win->parent->pointer_device;
               }
          }

        if ((!input) || (!input->seat)) return;

        _ecore_wl_input_grab_release(input, win);
        wl_shell_surface_move(win->shell_surface, input->seat,
                              input->display->serial);
     }
}

EAPI void 
ecore_wl_window_resize(Ecore_Wl_Window *win, int w, int h, int location)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   ecore_wl_window_update_size(win, w, h);

   if (win->shell_surface)
     {
        Ecore_Wl_Input *input;

        if (!(input = win->keyboard_device))
          {
             if (win->parent)
               {
                  if (!(input = win->parent->keyboard_device))
                    input = win->parent->pointer_device;
               }
          }

        if ((!input) || (!input->seat)) return;

        _ecore_wl_input_grab_release(input, win);
        wl_shell_surface_resize(win->shell_surface, input->seat, 
                                input->display->serial, location);
     }
}

EAPI void 
ecore_wl_window_damage(Ecore_Wl_Window *win, int x, int y, int w, int h)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   if (win->surface) wl_surface_damage(win->surface, x, y, w, h);
}

EAPI void 
ecore_wl_window_commit(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   if ((win->surface) && (win->has_buffer)) 
     wl_surface_commit(win->surface);
}

EAPI void 
ecore_wl_window_buffer_attach(Ecore_Wl_Window *win, struct wl_buffer *buffer, int x, int y)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   switch (win->buffer_type)
     {
      case ECORE_WL_WINDOW_BUFFER_TYPE_EGL_WINDOW:
        break;
      case ECORE_WL_WINDOW_BUFFER_TYPE_EGL_IMAGE:
      case ECORE_WL_WINDOW_BUFFER_TYPE_SHM:
        if (win->surface)
          {
             win->has_buffer = (buffer != NULL);

             /* if (buffer) */
             wl_surface_attach(win->surface, buffer, x, y);
             wl_surface_damage(win->surface, 0, 0, 
                               win->allocation.w, win->allocation.h);
             wl_surface_commit(win->surface);
          }
        break;
      default:
        return;
     }
}

EAPI struct wl_surface*
ecore_wl_window_surface_create(Ecore_Wl_Window *win)
{
   if (!win) return NULL;
   if (win->surface) return win->surface;
   win->surface = wl_compositor_create_surface(_ecore_wl_compositor_get());
   win->surface_id = wl_proxy_get_id((struct wl_proxy *)win->surface);
   return win->surface;
}

EAPI void 
ecore_wl_window_show(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   ecore_wl_window_surface_create(win);

   if ((win->type != ECORE_WL_WINDOW_TYPE_DND) &&
       (win->type != ECORE_WL_WINDOW_TYPE_NONE))
     {
        if ((!win->shell_surface) && (_ecore_wl_disp->wl.shell))
          {
             win->shell_surface = 
               wl_shell_get_shell_surface(_ecore_wl_disp->wl.shell, 
                                          win->surface);
             if (!win->shell_surface) return;

             if (win->title)
               wl_shell_surface_set_title(win->shell_surface, win->title);
             if (win->class_name)
               wl_shell_surface_set_class(win->shell_surface, win->class_name);
          }

        if (win->shell_surface)
          wl_shell_surface_add_listener(win->shell_surface, 
                                        &_ecore_wl_shell_surface_listener, win);
     }

   /* trap for valid shell surface */
   if (!win->shell_surface) return;

   switch (win->type)
     {
      case ECORE_WL_WINDOW_TYPE_FULLSCREEN:
        wl_shell_surface_set_fullscreen(win->shell_surface, 
                                        WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT,
                                        0, NULL);
        break;
      case ECORE_WL_WINDOW_TYPE_MAXIMIZED:
        wl_shell_surface_set_maximized(win->shell_surface, NULL);
        break;
      case ECORE_WL_WINDOW_TYPE_TRANSIENT:
        wl_shell_surface_set_transient(win->shell_surface, 
                                       win->parent->surface, 
                                       win->allocation.x, win->allocation.y, 0);
        break;
      case ECORE_WL_WINDOW_TYPE_MENU:
        wl_shell_surface_set_popup(win->shell_surface, 
                                   _ecore_wl_disp->input->seat,
                                   _ecore_wl_disp->serial,
                                   win->parent->surface, 
                                   win->allocation.x, win->allocation.y, 0);
        break;
      case ECORE_WL_WINDOW_TYPE_TOPLEVEL:
        wl_shell_surface_set_toplevel(win->shell_surface);
        break;
      default:
        break;
     }
}

EAPI void 
ecore_wl_window_hide(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   if (win->shell_surface) wl_shell_surface_destroy(win->shell_surface);
   win->shell_surface = NULL;
   if (win->surface) wl_surface_destroy(win->surface);
   win->surface = NULL;
}

EAPI void 
ecore_wl_window_raise(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   if (win->shell_surface) 
     wl_shell_surface_set_toplevel(win->shell_surface);
}

EAPI void 
ecore_wl_window_maximized_set(Ecore_Wl_Window *win, Eina_Bool maximized)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   if ((win->type == ECORE_WL_WINDOW_TYPE_MAXIMIZED) == maximized) return;
   if (win->type == ECORE_WL_WINDOW_TYPE_TOPLEVEL)
     {
        win->saved.w = win->allocation.w;
        win->saved.h = win->allocation.h;
        if (win->shell_surface) 
          wl_shell_surface_set_maximized(win->shell_surface, NULL);
        win->type = ECORE_WL_WINDOW_TYPE_MAXIMIZED;
     }
   else if (win->type == ECORE_WL_WINDOW_TYPE_MAXIMIZED)
     {
        if (win->shell_surface) 
          wl_shell_surface_set_toplevel(win->shell_surface);
        win->type = ECORE_WL_WINDOW_TYPE_TOPLEVEL;
        _ecore_wl_window_configure_send(win, win->saved.w, win->saved.h, 0);
     }
}

EAPI Eina_Bool
ecore_wl_window_maximized_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return EINA_FALSE;

   if (win->type == ECORE_WL_WINDOW_TYPE_MAXIMIZED)
     return EINA_TRUE;

   return EINA_FALSE;
}

EAPI void 
ecore_wl_window_fullscreen_set(Ecore_Wl_Window *win, Eina_Bool fullscreen)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   if ((win->type == ECORE_WL_WINDOW_TYPE_FULLSCREEN) == fullscreen) return;
   if (fullscreen)
     {
        win->type = ECORE_WL_WINDOW_TYPE_FULLSCREEN;
        win->saved.w = win->allocation.w;
        win->saved.h = win->allocation.h;
        if (win->shell_surface)
          wl_shell_surface_set_fullscreen(win->shell_surface, 
                                          WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT,
                                          0, NULL);
     }
   else 
     {
        if (win->shell_surface)
          wl_shell_surface_set_toplevel(win->shell_surface);
        win->type = ECORE_WL_WINDOW_TYPE_TOPLEVEL;
        _ecore_wl_window_configure_send(win, win->saved.w, win->saved.h, 0);
     }
}

EAPI Eina_Bool
ecore_wl_window_fullscreen_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return EINA_FALSE;

   if (win->type == ECORE_WL_WINDOW_TYPE_FULLSCREEN)
     return EINA_TRUE;

   return EINA_FALSE;
}

EAPI void 
ecore_wl_window_transparent_set(Ecore_Wl_Window *win, Eina_Bool transparent)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   win->transparent = transparent;
   ecore_wl_window_opaque_region_set(win, win->opaque.x, win->opaque.y, 
                                     win->opaque.w, win->opaque.h);
}

EAPI Eina_Bool
ecore_wl_window_alpha_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return EINA_FALSE;

   return win->alpha;
}

EAPI void
ecore_wl_window_alpha_set(Ecore_Wl_Window *win, Eina_Bool alpha)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   win->alpha = alpha;
   ecore_wl_window_opaque_region_set(win, win->opaque.x, win->opaque.y, 
                                     win->opaque.w, win->opaque.h);
}

EAPI Eina_Bool
ecore_wl_window_transparent_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return EINA_FALSE;

   return win->transparent;
}

EAPI void 
ecore_wl_window_update_size(Ecore_Wl_Window *win, int w, int h)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   win->allocation.w = w;
   win->allocation.h = h;
}

EAPI void 
ecore_wl_window_update_location(Ecore_Wl_Window *win, int x, int y)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   win->allocation.x = x;
   win->allocation.y = y;
}

EAPI struct wl_surface *
ecore_wl_window_surface_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return NULL;
   return win->surface;
}

/* @since 1.2 */
EAPI struct wl_shell_surface *
ecore_wl_window_shell_surface_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return NULL;
   return win->shell_surface;
}

EAPI Ecore_Wl_Window *
ecore_wl_window_find(unsigned int id)
{
   Ecore_Wl_Window *win = NULL;

   if (!_windows) return NULL;
   win = eina_hash_find(_windows, _ecore_wl_window_id_str_get(id));
   return win;
}

EAPI void 
ecore_wl_window_type_set(Ecore_Wl_Window *win, Ecore_Wl_Window_Type type)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   win->type = type;
}

EAPI void 
ecore_wl_window_pointer_set(Ecore_Wl_Window *win, struct wl_surface *surface, int hot_x, int hot_y)
{
   Ecore_Wl_Input *input;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   win->pointer.surface = surface;
   win->pointer.hot_x = hot_x;
   win->pointer.hot_y = hot_y;
   win->pointer.set = EINA_TRUE;

   if ((input = win->pointer_device))
     ecore_wl_input_pointer_set(input, surface, hot_x, hot_y);
}

EAPI void
ecore_wl_window_cursor_from_name_set(Ecore_Wl_Window *win, const char *cursor_name)
{
   Ecore_Wl_Input *input;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   win->pointer.set = EINA_FALSE;

   if ((input = win->pointer_device))
     ecore_wl_input_cursor_from_name_set(input, cursor_name);
}

EAPI void
ecore_wl_window_cursor_default_restore(Ecore_Wl_Window *win)
{
   Ecore_Wl_Input *input;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   win->pointer.set = EINA_FALSE;

   if ((input = win->pointer_device))
     ecore_wl_input_cursor_default_restore(input);
}

/* @since 1.2 */
EAPI void 
ecore_wl_window_parent_set(Ecore_Wl_Window *win, Ecore_Wl_Window *parent)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   win->parent = parent;
}

EAPI Ecore_Wl_Window *
ecore_wl_window_surface_find(struct wl_surface *surface)
{
   Eina_Iterator *itr;
   Ecore_Wl_Window *win = NULL;
   void *data;

   itr = eina_hash_iterator_data_new(_windows);
   while (eina_iterator_next(itr, &data))
     {
        if (((Ecore_Wl_Window *)data)->surface == surface)
          {
             win = data;
             break;
          }
     }

   eina_iterator_free(itr);

   return win;
}

/* @since 1.8 */
EAPI void 
ecore_wl_window_input_region_set(Ecore_Wl_Window *win, int x, int y, int w, int h)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   if ((win->type != ECORE_WL_WINDOW_TYPE_FULLSCREEN) || 
       (win->type != ECORE_WL_WINDOW_TYPE_DND))
     {
        if ((w > 0) && (h > 0))
          {
             struct wl_region *region = NULL;

             region = 
               wl_compositor_create_region(_ecore_wl_compositor_get());
             wl_region_add(region, x, y, w, h);
             wl_surface_set_input_region(win->surface, region);
             wl_region_destroy(region);
          }
        else
          wl_surface_set_input_region(win->surface, NULL);
     }

   ecore_wl_window_commit(win);
}

/* @since 1.8 */
EAPI void 
ecore_wl_window_opaque_region_set(Ecore_Wl_Window *win, int x, int y, int w, int h)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;

   win->opaque.x = x;
   win->opaque.y = y;
   win->opaque.w = w;
   win->opaque.h = h;

   if ((!win->transparent) && (!win->alpha))
     {
        struct wl_region *region = NULL;

        region = 
          wl_compositor_create_region(_ecore_wl_compositor_get());

        switch (win->rotation)
          {
           case 0:
             wl_region_add(region, x, y, w, h);
             break;
           case 180:
             wl_region_add(region, x, x + y, w, h);
             break;
           case 90:
             wl_region_add(region, y, x, h, w);
             break;
           case 270:
             wl_region_add(region, x + y, x, h, w);
             break;
          }

        wl_surface_set_opaque_region(win->surface, region);
        wl_region_destroy(region);
     }
   else
     wl_surface_set_opaque_region(win->surface, NULL);

   ecore_wl_window_commit(win);
}

/* @since 1.8 */
EAPI void 
ecore_wl_window_rotation_set(Ecore_Wl_Window *win, int rotation)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   win->rotation = rotation;
}

/* @since 1.8 */
EAPI int 
ecore_wl_window_rotation_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return 0;
   return win->rotation;
}

/* @since 1.8 */
EAPI int
ecore_wl_window_id_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return 0;
   return win->id;
}

/* @since 1.8 */
EAPI int
ecore_wl_window_surface_id_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return 0;
   return win->surface_id;
}

/* @since 1.8 */
EAPI void
ecore_wl_window_title_set(Ecore_Wl_Window *win, const char *title)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   eina_stringshare_replace(&win->title, title);

   if ((win->shell_surface) && (win->title))
     wl_shell_surface_set_title(win->shell_surface, win->title);
}

/* @since 1.8 */
EAPI void
ecore_wl_window_class_name_set(Ecore_Wl_Window *win, const char *class_name)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return;
   eina_stringshare_replace(&win->class_name, class_name);

   if ((win->shell_surface) && (win->class_name))
     wl_shell_surface_set_class(win->shell_surface, win->class_name);
}

/* @since 1.8 */
/* Maybe we need an ecore_wl_window_pointer_get() too */
EAPI Ecore_Wl_Input *
ecore_wl_window_keyboard_get(Ecore_Wl_Window *win)
{
   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!win) return 0;
   return win->keyboard_device;
}


/* local functions */
static void 
_ecore_wl_window_cb_ping(void *data EINA_UNUSED, struct wl_shell_surface *shell_surface, unsigned int serial)
{
   if (!shell_surface) return;
   wl_shell_surface_pong(shell_surface, serial);
}

static void 
_ecore_wl_window_cb_configure(void *data, struct wl_shell_surface *shell_surface EINA_UNUSED, unsigned int edges, int w, int h)
{
   Ecore_Wl_Window *win;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!(win = data)) return;

   if ((w <= 0) || (h <= 0)) return;

   if ((win->allocation.w != w) || (win->allocation.h != h))
     {
        _ecore_wl_window_configure_send(win, w, h, edges);
     }
}

static void 
_ecore_wl_window_cb_popup_done(void *data, struct wl_shell_surface *shell_surface EINA_UNUSED)
{
   Ecore_Wl_Window *win;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!shell_surface) return;
   if (!(win = data)) return;
   ecore_wl_input_ungrab(win->pointer_device);
}

static void 
_ecore_wl_window_cb_surface_enter(void *data, struct wl_surface *surface, struct wl_output *output EINA_UNUSED)
{
   Ecore_Wl_Window *win;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!surface) return;
   if (!(win = data)) return;
}

static void 
_ecore_wl_window_cb_surface_leave(void *data, struct wl_surface *surface, struct wl_output *output EINA_UNUSED)
{
   Ecore_Wl_Window *win;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!surface) return;
   if (!(win = data)) return;
}

static void 
_ecore_wl_window_configure_send(Ecore_Wl_Window *win, int w, int h, int edges)
{
   Ecore_Wl_Event_Window_Configure *ev;

   LOGFN(__FILE__, __LINE__, __FUNCTION__);

   if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Window_Configure)))) return;
   ev->win = win->id;
   ev->event_win = win->id;
   ev->x = win->allocation.x;
   ev->y = win->allocation.y;
   ev->w = w;
   ev->h = h;
   ev->edges = edges;
   ecore_event_add(ECORE_WL_EVENT_WINDOW_CONFIGURE, ev, NULL, NULL);
}

static char *
_ecore_wl_window_id_str_get(unsigned int win_id)
{
   const char *vals = "qWeRtYuIoP5$&<~";
   static char id[9];
   unsigned int val;

   val = win_id;
   id[0] = vals[(val >> 28) & 0xf];
   id[1] = vals[(val >> 24) & 0xf];
   id[2] = vals[(val >> 20) & 0xf];
   id[3] = vals[(val >> 16) & 0xf];
   id[4] = vals[(val >> 12) & 0xf];
   id[5] = vals[(val >> 8) & 0xf];
   id[6] = vals[(val >> 4) & 0xf];
   id[7] = vals[(val) & 0xf];
   id[8] = 0;

   return id;
}
