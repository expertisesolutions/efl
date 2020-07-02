#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <fcntl.h>
#include "ecore_wl_private.h"

/*
 * The subsurface protocol was moved into Wayland Core
 * around v1.3.90 (i.e. v1.4.0).
 * Test if subsurface protocol is part of wayland-client.h.
 * If not, we include our own copy of the protocol header.
 */
#include <wayland-client.h>
#ifndef WL_SUBSURFACE_ERROR_ENUM
# include <subsurface-client-protocol.h>
#endif

#include "xdg-shell-client-protocol.h"
#define XDG_VERSION 5

#include "session-recovery-client-protocol.h"

/* local function prototypes */
static int _ecore_wl_shutdown(Eina_Bool close);
static Eina_Bool _ecore_wl_cb_idle_enterer(void *data);
static Eina_Bool _ecore_wl_cb_handle_data(void *data, Ecore_Fd_Handler *hdl);
static void _ecore_wl_cb_handle_global(void *data, struct wl_registry *registry, unsigned int id, const char *interface, unsigned int version EINA_UNUSED);
static void _ecore_wl_cb_handle_global_remove(void *data, struct wl_registry *registry EINA_UNUSED, unsigned int id);
static Eina_Bool _ecore_wl_xkb_init(Ecore_Wl_Display *ewd);
static Eina_Bool _ecore_wl_xkb_shutdown(Ecore_Wl_Display *ewd);
static void _ecore_wl_sync_wait(Ecore_Wl_Display *ewd);
static void _ecore_wl_sync_callback(void *data, struct wl_callback *callback, uint32_t serial);
static void _ecore_wl_animator_tick_cb_begin(void *data EINA_UNUSED);
static void _ecore_wl_animator_tick_cb_end(void *data EINA_UNUSED);
static void _ecore_wl_animator_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED);
static Eina_Bool _ecore_wl_animator_window_add(const Eina_Hash *hash EINA_UNUSED, const void *key EINA_UNUSED, void *data, void *fdata EINA_UNUSED);
static void _ecore_wl_signal_exit(void);
static void _ecore_wl_signal_exit_free(void *data EINA_UNUSED, void *event);
static void _ecore_wl_init_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED);

/* local variables */
static int _ecore_wl_init_count = 0;
static Eina_Bool _ecore_wl_animator_busy = EINA_FALSE;
static Eina_Bool _ecore_wl_fatal_error = EINA_FALSE;
Eina_Bool _ecore_wl_server_mode = EINA_FALSE;

static const struct wl_registry_listener _ecore_wl_registry_listener =
{
   _ecore_wl_cb_handle_global,
   _ecore_wl_cb_handle_global_remove
};

static const struct wl_callback_listener _ecore_wl_sync_listener =
{
   _ecore_wl_sync_callback
};

static const struct wl_callback_listener _ecore_wl_init_sync_listener =
{
   _ecore_wl_init_callback
};

static const struct wl_callback_listener _ecore_wl_anim_listener =
{
   _ecore_wl_animator_callback
};

static void
xdg_shell_ping(void *data EINA_UNUSED, struct xdg_shell *shell, uint32_t serial)
{
   xdg_shell_pong(shell, serial);
}

static const struct xdg_shell_listener xdg_shell_listener =
{
   xdg_shell_ping,
};

/* external variables */
int _ecore_wl_log_dom = -1;
Ecore_Wl_Display *_ecore_wl_disp = NULL;

ECORE_WAYLAND_API int ECORE_WL_EVENT_MOUSE_IN = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_MOUSE_OUT = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_FOCUS_IN = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_FOCUS_OUT = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_WINDOW_CONFIGURE = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_DND_ENTER = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_DND_POSITION = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_DND_LEAVE = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_DND_DROP = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_DND_OFFER = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_DND_END = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_DATA_SOURCE_TARGET = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_DATA_SOURCE_SEND = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_SELECTION_DATA_READY = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_DATA_SOURCE_CANCELLED = 0;
ECORE_WAYLAND_API int ECORE_WL_EVENT_INTERFACES_BOUND = 0;

static void
_ecore_wl_init_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED)
{
   Ecore_Wl_Display *ewd = data;

   wl_callback_destroy(callback);
   ewd->init_done = EINA_TRUE;
}

static void
_ecore_wl_init_wait(void)
{
   int ret;
   while (!_ecore_wl_disp->init_done)
     {
        ret = wl_display_dispatch(_ecore_wl_disp->wl.display);
        if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
          {
             /* raise exit signal */
             ERR("Wayland socket error: %s", strerror(errno));
             abort();
             break;
          }
     }
}

ECORE_WAYLAND_API int
ecore_wl_init(const char *name)
{
   struct wl_callback *callback;
   LOGFN;

   if (++_ecore_wl_init_count != 1) return _ecore_wl_init_count;

   if (!eina_init()) return --_ecore_wl_init_count;

   _ecore_wl_log_dom =
     eina_log_domain_register("ecore_wl", ECORE_WL_DEFAULT_LOG_COLOR);
   if (_ecore_wl_log_dom < 0)
     {
        EINA_LOG_ERR("Cannot create a log domain for Ecore Wayland");
        goto exit_eina;
     }

   if (!ecore_init())
     {
        ERR("Could not initialize ecore");
        goto exit_ecore;
     }

   if (!ecore_event_init())
     {
        ERR("Could not initialize ecore_event");
        goto exit_ecore_event;
     }

   ECORE_WL_EVENT_MOUSE_IN = ecore_event_type_new();
   ECORE_WL_EVENT_MOUSE_OUT = ecore_event_type_new();
   ECORE_WL_EVENT_FOCUS_IN = ecore_event_type_new();
   ECORE_WL_EVENT_FOCUS_OUT = ecore_event_type_new();
   ECORE_WL_EVENT_WINDOW_CONFIGURE = ecore_event_type_new();
   ECORE_WL_EVENT_DND_ENTER = ecore_event_type_new();
   ECORE_WL_EVENT_DND_POSITION = ecore_event_type_new();
   ECORE_WL_EVENT_DND_LEAVE = ecore_event_type_new();
   ECORE_WL_EVENT_DND_DROP = ecore_event_type_new();
   ECORE_WL_EVENT_DND_OFFER = ecore_event_type_new();
   ECORE_WL_EVENT_DND_END = ecore_event_type_new();
   ECORE_WL_EVENT_DATA_SOURCE_TARGET = ecore_event_type_new();
   ECORE_WL_EVENT_DATA_SOURCE_SEND = ecore_event_type_new();
   ECORE_WL_EVENT_SELECTION_DATA_READY = ecore_event_type_new();
   ECORE_WL_EVENT_DATA_SOURCE_CANCELLED = ecore_event_type_new();
   ECORE_WL_EVENT_INTERFACES_BOUND = ecore_event_type_new();

   if (!(_ecore_wl_disp = calloc(1, sizeof(Ecore_Wl_Display))))
     {
        ERR("Could not allocate memory for Ecore_Wl_Display structure");
        goto exit_ecore_disp;
     }

   if (!(_ecore_wl_disp->wl.display = wl_display_connect(name)))
     {
        ERR("Could not connect to Wayland display");
        goto exit_ecore_disp_connect;
     }

   _ecore_wl_disp->fd = wl_display_get_fd(_ecore_wl_disp->wl.display);

   _ecore_wl_disp->fd_hdl =
     ecore_main_fd_handler_add(_ecore_wl_disp->fd,
                               ECORE_FD_READ | ECORE_FD_WRITE | ECORE_FD_ERROR,
                               _ecore_wl_cb_handle_data, _ecore_wl_disp,
                               NULL, NULL);

   _ecore_wl_disp->idle_enterer =
     ecore_idle_enterer_add(_ecore_wl_cb_idle_enterer, _ecore_wl_disp);

   _ecore_wl_disp->wl.registry =
     wl_display_get_registry(_ecore_wl_disp->wl.display);
   wl_registry_add_listener(_ecore_wl_disp->wl.registry,
                            &_ecore_wl_registry_listener, _ecore_wl_disp);

   if (!_ecore_wl_xkb_init(_ecore_wl_disp))
     {
        ERR("Could not initialize XKB");
        goto exit_ecore_disp_connect;
     }

   _ecore_wl_window_init();
   _ecore_wl_events_init();

   _ecore_wl_disp->init_done = EINA_TRUE;
   if (!_ecore_wl_server_mode)
     {
        _ecore_wl_disp->init_done = EINA_FALSE;
        callback = wl_display_sync(_ecore_wl_disp->wl.display);
        wl_callback_add_listener(callback, &_ecore_wl_init_sync_listener,
                                 _ecore_wl_disp);
     }

   return _ecore_wl_init_count;

exit_ecore_disp_connect:
   free(_ecore_wl_disp);

exit_ecore_disp:
   ecore_event_shutdown();

exit_ecore_event:
   ecore_shutdown();

exit_ecore:
   eina_log_domain_unregister(_ecore_wl_log_dom);
   _ecore_wl_log_dom = -1;

exit_eina:
   eina_shutdown();
   return --_ecore_wl_init_count;
}

ECORE_WAYLAND_API int
ecore_wl_shutdown(void)
{
   LOGFN;

   return _ecore_wl_shutdown(EINA_TRUE);
}

ECORE_WAYLAND_API void
ecore_wl_flush(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display)) return;
   wl_display_flush(_ecore_wl_disp->wl.display);
}

ECORE_WAYLAND_API void
ecore_wl_sync(void)
{
   int ret;
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display)) return;
   _ecore_wl_sync_wait(_ecore_wl_disp);
   while (_ecore_wl_disp->sync_ref_count > 0)
     {
        ret = wl_display_dispatch(_ecore_wl_disp->wl.display);
        if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
          {
             /* raise exit signal */
             ERR("Wayland socket error: %s", strerror(errno));
             abort();
             break;
          }
     }
}

ECORE_WAYLAND_API struct wl_shm *
ecore_wl_shm_get(void)
{
   if (!_ecore_wl_disp) return NULL;

   _ecore_wl_init_wait();

   return _ecore_wl_disp->wl.shm;
}

ECORE_WAYLAND_API struct wl_display *
ecore_wl_display_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;
   return _ecore_wl_disp->wl.display;
}

ECORE_WAYLAND_API Eina_Inlist *
ecore_wl_globals_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;

   _ecore_wl_init_wait();

   return _ecore_wl_disp->globals;
}

ECORE_WAYLAND_API struct wl_registry *
ecore_wl_registry_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;
   return _ecore_wl_disp->wl.registry;
}

struct wl_compositor *
_ecore_wl_compositor_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;

   _ecore_wl_init_wait();

   return _ecore_wl_disp->wl.compositor;
}

struct wl_subcompositor *
_ecore_wl_subcompositor_get(void)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display))
     return NULL;

   _ecore_wl_init_wait();

   return _ecore_wl_disp->wl.subcompositor;
}

ECORE_WAYLAND_API void
ecore_wl_screen_size_get(int *w, int *h)
{
   Ecore_Wl_Output *out;
   Eina_Inlist *tmp;
   int ow = 0, oh = 0;

   LOGFN;

   if (w) *w = 0;
   if (h) *h = 0;

   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display)) return;

   _ecore_wl_init_wait();

   // the first sync is in case registry replies are not back yet
   if (!_ecore_wl_disp->output)
     {
        // second sync is in case bound object replies in registry are not back
        ecore_wl_sync();
        if (!_ecore_wl_disp->output) ecore_wl_sync();
     }

   EINA_INLIST_FOREACH_SAFE(_ecore_wl_disp->outputs, tmp, out)
     {
        switch (out->transform)
          {
           case WL_OUTPUT_TRANSFORM_90:
           case WL_OUTPUT_TRANSFORM_270:
           case WL_OUTPUT_TRANSFORM_FLIPPED_90:
           case WL_OUTPUT_TRANSFORM_FLIPPED_270:
             /* Swap width and height */
             ow += out->allocation.h;
             oh += out->allocation.w;
             break;
           default:
             ow += out->allocation.w;
             oh += out->allocation.h;
          }
     }

   if (w) *w = ow;
   if (h) *h = oh;
}

/* @since 1.2 */
ECORE_WAYLAND_API void
ecore_wl_pointer_xy_get(int *x, int *y)
{
   LOGFN;

   _ecore_wl_input_pointer_xy_get(x, y);
}

ECORE_WAYLAND_API int
ecore_wl_dpi_get(void)
{
   int w, mw;

   LOGFN;

   if (!_ecore_wl_disp) return 0;

   _ecore_wl_init_wait();

   if (!_ecore_wl_disp->output) return 75;

   mw = _ecore_wl_disp->output->mw;
   if (mw <= 0) return 75;

   w = _ecore_wl_disp->output->allocation.w;
   /* FIXME: NB: Hrrrmmm, need to verify this. xorg code is using a different
    * formula to calc this */
   return (((w * 254) / mw) + 5) / 10;
}

ECORE_WAYLAND_API void
ecore_wl_display_iterate(void)
{
   int ret;
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->wl.display)) return;
   if (!_ecore_wl_server_mode)
     {
        ret = wl_display_dispatch(_ecore_wl_disp->wl.display);
        if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
          {
             /* raise exit signal */
             ERR("Wayland socket error: %s", strerror(errno));
             abort();
          }
     }
}

/* @since 1.8 */
ECORE_WAYLAND_API Eina_Bool
ecore_wl_animator_source_set(Ecore_Animator_Source source)
{
   LOGFN;

   if (_ecore_wl_server_mode) return EINA_FALSE;

   /* FIXME: check existing source. If custom, disable anim_callbacks */

   /* based on the animator source we are using, setup or destroy callbacks */
   switch (source)
     {
      case ECORE_ANIMATOR_SOURCE_CUSTOM:
        ecore_animator_custom_source_tick_begin_callback_set
          (_ecore_wl_animator_tick_cb_begin, NULL);
        ecore_animator_custom_source_tick_end_callback_set
          (_ecore_wl_animator_tick_cb_end, NULL);
        break;
      case ECORE_ANIMATOR_SOURCE_TIMER:
        ecore_animator_custom_source_tick_begin_callback_set(NULL, NULL);
        ecore_animator_custom_source_tick_end_callback_set(NULL, NULL);
        break;
      default:
        break;
     }

   /* set the source of the animator */
   ecore_animator_source_set(source);

   return EINA_TRUE;
}

ECORE_WAYLAND_API struct wl_cursor *
ecore_wl_cursor_get(const char *cursor_name)
{
   if ((!_ecore_wl_disp) || (!_ecore_wl_disp->cursor_theme))
     return NULL;

   return wl_cursor_theme_get_cursor(_ecore_wl_disp->cursor_theme,
                                     cursor_name);
}

ECORE_WAYLAND_API void
ecore_wl_server_mode_set(Eina_Bool on)
{
   _ecore_wl_server_mode = on;
}

/* local functions */
static int
_ecore_wl_shutdown(Eina_Bool close)
{
   LOGFN;

   if (_ecore_wl_init_count < 1)
     {
        ERR("Ecore_Wayland Shutdown called without Ecore_Wayland Init");
        return 0;
     }

   if (--_ecore_wl_init_count != 0) return _ecore_wl_init_count;
   if (!_ecore_wl_disp) return _ecore_wl_init_count;

   ecore_event_type_flush(ECORE_WL_EVENT_MOUSE_IN,
                          ECORE_WL_EVENT_MOUSE_OUT,
                          ECORE_WL_EVENT_FOCUS_IN,
                          ECORE_WL_EVENT_FOCUS_OUT,
                          ECORE_WL_EVENT_WINDOW_CONFIGURE,
                          ECORE_WL_EVENT_DND_ENTER,
                          ECORE_WL_EVENT_DND_POSITION,
                          ECORE_WL_EVENT_DND_LEAVE,
                          ECORE_WL_EVENT_DND_DROP,
                          ECORE_WL_EVENT_DND_OFFER,
                          ECORE_WL_EVENT_DND_END,
                          ECORE_WL_EVENT_DATA_SOURCE_TARGET,
                          ECORE_WL_EVENT_DATA_SOURCE_SEND,
                          ECORE_WL_EVENT_SELECTION_DATA_READY,
                          ECORE_WL_EVENT_DATA_SOURCE_CANCELLED,
                          ECORE_WL_EVENT_INTERFACES_BOUND);

   _ecore_wl_events_shutdown();
   _ecore_wl_window_shutdown();

   if (_ecore_wl_disp->fd_hdl)
     ecore_main_fd_handler_del(_ecore_wl_disp->fd_hdl);
   if (_ecore_wl_disp->idle_enterer)
      ecore_idle_enterer_del(_ecore_wl_disp->idle_enterer);

   if ((close) && (!_ecore_wl_fatal_error))
     {
        Ecore_Wl_Output *out;
        Ecore_Wl_Input *in;
        Ecore_Wl_Global *global;
        Eina_Inlist *tmp;

        EINA_INLIST_FOREACH_SAFE(_ecore_wl_disp->outputs, tmp, out)
          _ecore_wl_output_del(out);

        EINA_INLIST_FOREACH_SAFE(_ecore_wl_disp->inputs, tmp, in)
          _ecore_wl_input_del(in);

        EINA_INLIST_FOREACH_SAFE(_ecore_wl_disp->globals, tmp, global)
          {
             _ecore_wl_disp->globals =
               eina_inlist_remove(_ecore_wl_disp->globals,
                                  EINA_INLIST_GET(global));
             free(global->interface);
             free(global);
          }

        _ecore_wl_xkb_shutdown(_ecore_wl_disp);

        if (_ecore_wl_disp->wl.session_recovery)
          zwp_e_session_recovery_destroy(_ecore_wl_disp->wl.session_recovery);
#ifdef USE_IVI_SHELL
        if (_ecore_wl_disp->wl.ivi_application)
          ivi_application_destroy(_ecore_wl_disp->wl.ivi_application);
#endif
        if (_ecore_wl_disp->wl.xdg_shell)
          xdg_shell_destroy(_ecore_wl_disp->wl.xdg_shell);
        if (_ecore_wl_disp->wl.shell)
          wl_shell_destroy(_ecore_wl_disp->wl.shell);
        if (_ecore_wl_disp->wl.shm) wl_shm_destroy(_ecore_wl_disp->wl.shm);
        if (_ecore_wl_disp->wl.data_device_manager)
          wl_data_device_manager_destroy(_ecore_wl_disp->wl.data_device_manager);
        if (_ecore_wl_disp->wl.compositor)
          wl_compositor_destroy(_ecore_wl_disp->wl.compositor);
        if (_ecore_wl_disp->wl.subcompositor)
          wl_subcompositor_destroy(_ecore_wl_disp->wl.subcompositor);
        if (_ecore_wl_disp->cursor_theme)
          wl_cursor_theme_destroy(_ecore_wl_disp->cursor_theme);
        if (_ecore_wl_disp->wl.display)
          {
             wl_registry_destroy(_ecore_wl_disp->wl.registry);
             wl_display_flush(_ecore_wl_disp->wl.display);
             wl_display_disconnect(_ecore_wl_disp->wl.display);
          }
        free(_ecore_wl_disp);
     }

   ecore_event_shutdown();
   ecore_shutdown();

   eina_log_domain_unregister(_ecore_wl_log_dom);
   _ecore_wl_log_dom = -1;
   eina_shutdown();

   return _ecore_wl_init_count;
}

static Eina_Bool
_ecore_wl_cb_idle_enterer(void *data)
{
   Ecore_Wl_Display *ewd;
   int ret = 0;

   if (_ecore_wl_fatal_error) return ECORE_CALLBACK_CANCEL;

   if (!(ewd = data)) return ECORE_CALLBACK_RENEW;

   ret = wl_display_get_error(ewd->wl.display);
   if (ret < 0) goto err;

   ret = wl_display_dispatch_pending(ewd->wl.display);
   if (ret < 0) goto err;

   ret = wl_display_flush(ewd->wl.display);
   if ((ret < 0) && (errno == EAGAIN))
     ecore_main_fd_handler_active_set(ewd->fd_hdl,
                                      (ECORE_FD_READ | ECORE_FD_WRITE));

   return ECORE_CALLBACK_RENEW;

err:
   if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
     {
        _ecore_wl_fatal_error = EINA_TRUE;

        /* raise exit signal */
        ERR("Wayland socket error: %s", strerror(errno));
        _ecore_wl_signal_exit();

        return ECORE_CALLBACK_CANCEL;
     }

   return ECORE_CALLBACK_RENEW;
}

static Eina_Bool
_ecore_wl_cb_handle_data(void *data, Ecore_Fd_Handler *hdl)
{
   Ecore_Wl_Display *ewd;
   int ret = 0;

   /* LOGFN; */

   if (_ecore_wl_fatal_error) return ECORE_CALLBACK_CANCEL;

   if (!(ewd = data)) return ECORE_CALLBACK_RENEW;

   if (ecore_main_fd_handler_active_get(hdl, ECORE_FD_ERROR))
     {
        ERR("Received error on wayland display fd");
        _ecore_wl_fatal_error = EINA_TRUE;
        _ecore_wl_signal_exit();

        return ECORE_CALLBACK_CANCEL;
     }

   /* wl_display_dispatch_pending(ewd->wl.display); */

   if (ecore_main_fd_handler_active_get(hdl, ECORE_FD_READ))
     ret = wl_display_dispatch(ewd->wl.display);
   else if (ecore_main_fd_handler_active_get(hdl, ECORE_FD_WRITE))
     {
        ret = wl_display_flush(ewd->wl.display);
        if (ret == 0)
          ecore_main_fd_handler_active_set(hdl, ECORE_FD_READ);
     }

   if ((ret < 0) && ((errno != EAGAIN) && (errno != EINVAL)))
     {
        _ecore_wl_fatal_error = EINA_TRUE;

        /* raise exit signal */
        _ecore_wl_signal_exit();

        return ECORE_CALLBACK_CANCEL;
     }

   return ECORE_CALLBACK_RENEW;
}

static void
_ecore_wl_cb_handle_global(void *data, struct wl_registry *registry, unsigned int id, const char *interface, unsigned int version)
{
   Ecore_Wl_Display *ewd;
   Ecore_Wl_Global *global;

   LOGFN;

   ewd = data;

   if (!(global = calloc(1, sizeof(Ecore_Wl_Global)))) return;

   global->id = id;
   global->interface = strdup(interface);
   global->version = version;
   ewd->globals = eina_inlist_append(ewd->globals, EINA_INLIST_GET(global));

   if (!strcmp(interface, "wl_compositor"))
     {
        ewd->wl.compositor =
          wl_registry_bind(registry, id, &wl_compositor_interface, 3);
     }
   else if (!strcmp(interface, "wl_subcompositor"))
     {
        ewd->wl.subcompositor =
           wl_registry_bind(registry, id, &wl_subcompositor_interface, 1);
     }
   else if (!strcmp(interface, "wl_output"))
     _ecore_wl_output_add(ewd, id);
   else if (!strcmp(interface, "wl_seat"))
     _ecore_wl_input_add(ewd, id);
   else if (!strcmp(interface, "zwp_e_session_recovery") && getenv("EFL_WAYLAND_SESSION_RECOVERY"))
     {
        ewd->wl.session_recovery =
          wl_registry_bind(registry, id, &zwp_e_session_recovery_interface, 1);
     }
#ifdef USE_IVI_SHELL
   else if (!strcmp(interface, "ivi_application"))
     {
        ewd->wl.ivi_application =
          wl_registry_bind(registry, id, &ivi_application_interface, 1);
     }
#endif
   else if (!strcmp(interface, "xdg_shell") && !getenv("EFL_WAYLAND_DONT_USE_XDG_SHELL"))
     {
        Eina_Hash *h;
        Eina_Iterator *it;
        Ecore_Wl_Window *win;

        ewd->wl.xdg_shell =
          wl_registry_bind(registry, id, &xdg_shell_interface, 1);
        xdg_shell_use_unstable_version(ewd->wl.xdg_shell, XDG_VERSION);
        xdg_shell_add_listener(ewd->wl.xdg_shell, &xdg_shell_listener,
                               ewd->wl.display);
        h = _ecore_wl_window_hash_get();
        it = eina_hash_iterator_data_new(h);
        EINA_ITERATOR_FOREACH(it, win)
          if (win->surface)
            _ecore_wl_window_shell_surface_init(win);
     }
   else if (!strcmp(interface, "wl_shell"))
     {
        ewd->wl.shell =
          wl_registry_bind(registry, id, &wl_shell_interface, 1);
     }
   else if (!strcmp(interface, "wl_shm"))
     {
        ewd->wl.shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);

        if (ewd->input)
          _ecore_wl_input_setup(ewd->input);
        else if (!ewd->cursor_theme)
          {
             ewd->cursor_theme =
               wl_cursor_theme_load(NULL, ECORE_WL_DEFAULT_CURSOR_SIZE,
                                    ewd->wl.shm);
          }
     }
   else if (!strcmp(interface, "wl_data_device_manager"))
     {
        ewd->wl.data_device_manager =
          wl_registry_bind(registry, id, &wl_data_device_manager_interface, 1);
     }

   if ((ewd->wl.compositor) && (ewd->wl.shm) &&
       ((ewd->wl.shell) || (ewd->wl.xdg_shell)))
     {
        Ecore_Wl_Event_Interfaces_Bound *ev;

        if (!(ev = calloc(1, sizeof(Ecore_Wl_Event_Interfaces_Bound))))
          return;

        ev->compositor = (ewd->wl.compositor != NULL);
        ev->shm = (ewd->wl.shm != NULL);
        ev->shell = ((ewd->wl.shell != NULL) || (ewd->wl.xdg_shell != NULL));
        ev->output = (ewd->output != NULL);
        ev->seat = (ewd->input != NULL);
        ev->data_device_manager = (ewd->wl.data_device_manager != NULL);
        ev->subcompositor = (ewd->wl.subcompositor != NULL);

        ecore_event_add(ECORE_WL_EVENT_INTERFACES_BOUND, ev, NULL, NULL);
     }
}

static void
_ecore_wl_cb_handle_global_remove(void *data, struct wl_registry *registry EINA_UNUSED, unsigned int id)
{
   Ecore_Wl_Display *ewd;
   Ecore_Wl_Global *global;
   Eina_Inlist *tmp;

   LOGFN;

   ewd = data;

   EINA_INLIST_FOREACH_SAFE(ewd->globals, tmp, global)
     {
        if (global->id != id) continue;
        ewd->globals =
          eina_inlist_remove(ewd->globals, EINA_INLIST_GET(global));
        free(global->interface);
        free(global);
     }
}

static Eina_Bool
_ecore_wl_xkb_init(Ecore_Wl_Display *ewd)
{
   LOGFN;

   if (!(ewd->xkb.context = xkb_context_new(0)))
     return EINA_FALSE;

   return EINA_TRUE;
}

static Eina_Bool
_ecore_wl_xkb_shutdown(Ecore_Wl_Display *ewd)
{
   LOGFN;

   xkb_context_unref(ewd->xkb.context);

   return EINA_TRUE;
}

static void
_ecore_wl_sync_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED)
{
   Ecore_Wl_Display *ewd = data;

   ewd->sync_ref_count--;
   wl_callback_destroy(callback);
}

static void
_ecore_wl_sync_wait(Ecore_Wl_Display *ewd)
{
   struct wl_callback *callback;

   ewd->sync_ref_count++;
   callback = wl_display_sync(ewd->wl.display);
   wl_callback_add_listener(callback, &_ecore_wl_sync_listener, ewd);
}

static void
_ecore_wl_animator_tick_cb_begin(void *data EINA_UNUSED)
{
   Eina_Hash *windows;

   _ecore_wl_animator_busy = EINA_TRUE;

   windows = _ecore_wl_window_hash_get();
   eina_hash_foreach(windows, _ecore_wl_animator_window_add, NULL);
}

static void
_ecore_wl_animator_tick_cb_end(void *data EINA_UNUSED)
{
   _ecore_wl_animator_busy = EINA_FALSE;
}

static void
_ecore_wl_animator_callback(void *data, struct wl_callback *callback, uint32_t serial EINA_UNUSED)
{
   Ecore_Wl_Window *win;

   if (!(win = data)) return;

   ecore_animator_custom_tick();

   wl_callback_destroy(callback);
   win->anim_callback = NULL;

   if (_ecore_wl_animator_busy)
     {
        win->anim_callback = wl_surface_frame(win->surface);
        wl_callback_add_listener(win->anim_callback,
                                 &_ecore_wl_anim_listener, win);
        ecore_wl_window_commit(win);
     }
}

static Eina_Bool
_ecore_wl_animator_window_add(const Eina_Hash *hash EINA_UNUSED, const void *key EINA_UNUSED, void *data, void *fdata EINA_UNUSED)
{
   Ecore_Wl_Window *win;

   if (!(win = data)) return EINA_TRUE;
   if (!win->surface) return EINA_TRUE;
   if (win->anim_callback) return EINA_TRUE;

   win->anim_callback = wl_surface_frame(win->surface);
   wl_callback_add_listener(win->anim_callback, &_ecore_wl_anim_listener, win);
   ecore_wl_window_commit(win);

   return EINA_TRUE;
}

static void
_ecore_wl_signal_exit(void)
{
   Ecore_Event_Signal_Exit *ev;

   if (!(ev = calloc(1, sizeof(Ecore_Event_Signal_Exit))))
     return;

   ev->quit = 1;
   ecore_event_add(ECORE_EVENT_SIGNAL_EXIT, ev,
                   _ecore_wl_signal_exit_free, NULL);
}

static void
_ecore_wl_signal_exit_free(void *data EINA_UNUSED, void *event)
{
   free(event);
}
