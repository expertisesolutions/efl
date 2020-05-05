#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

#include <X11/Xlib.h>
#include <Eina.h>
#include <Ecore_X.h>
#include <Ecore_Evas.h>

#include <ibus.h>
#include "ibus_imcontext.h"

struct _IBusIMContext
{
   /* instance members */
   Ecore_IMF_Context *ctx;

   IBusInputContext *ibuscontext;

   /* preedit status */
   char            *preedit_string;
   Eina_List       *preedit_attrs;
   int              preedit_cursor_pos;
   Eina_Bool        preedit_visible;

   int              cursor_x;
   int              cursor_y;
   int              cursor_w;
   int              cursor_h;

   Eina_Bool        has_focus;

   Ecore_X_Window   client_window;
   Evas            *client_canvas;

   int              caps;
};

typedef struct _KeyEvent KeyEvent;

struct _KeyEvent
{
   int keysym;
   int keycode;
   int state;
};

static Eina_Bool _sync_mode_use = EINA_TRUE;

static Ecore_IMF_Context *_focus_im_context = NULL;
static IBusBus           *_bus = NULL;

/* functions prototype */
/* static methods*/
static void _ecore_imf_context_ibus_create(IBusIMContext      *context);
static void _ecore_imf_context_ibus_cursor_location_set(Ecore_IMF_Context  *ctx);
static void _ecore_imf_context_ibus_bus_connected_cb(IBusBus *bus, IBusIMContext *context);
static XKeyEvent _ecore_imf_ibus_x_key_event_generate(Window win,
                                                      Eina_Bool press,
                                                      int keysym,
                                                      int keycode,
                                                      int modifiers);

static unsigned int
utf8_offset_to_index(const char *str, int offset)
{
   int index = 0;
   int i;
   for (i = 0; i < offset; i++)
     eina_unicode_utf8_next_get(str, &index);

   return index;
}

static int
sort_cb(const void *d1, const void *d2)
{
   const Ecore_IMF_Preedit_Attr *attr1 = d1;
   const Ecore_IMF_Preedit_Attr *attr2 = d2;

   if (!attr1) return 1;
   if (!attr2) return -1;

   if (attr1->start_index < attr2->start_index)
     return -1;
   else
     return 1;
}

static void
_ecore_imf_ibus_window_to_screen_geometry_get(Ecore_X_Window client_win,
                                              int *x,
                                              int *y)
{
   Ecore_X_Window root_window, win;
   int win_x, win_y;
   int sum_x = 0, sum_y = 0;

   if (!ecore_x_display_get()) goto end;

   root_window = ecore_x_window_root_get(client_win);
   win = client_win;

   while (root_window != win)
     {
        ecore_x_window_geometry_get(win, &win_x, &win_y, NULL, NULL);
        sum_x += win_x;
        sum_y += win_y;
        win = ecore_x_window_parent_get(win);
     }

end:
   if (x)
     *x = sum_x;
   if (y)
     *y = sum_y;
}

static unsigned int
_ecore_imf_modifier_to_ibus_modifier(unsigned int modifier)
{
   unsigned int state = 0;

   /**< "Control" is pressed */
   if (modifier & ECORE_IMF_KEYBOARD_MODIFIER_CTRL)
     state |= IBUS_CONTROL_MASK;

   /**< "Alt" is pressed */
   if (modifier & ECORE_IMF_KEYBOARD_MODIFIER_ALT)
     state |= IBUS_MOD1_MASK;

   /**< "Shift" is pressed */
   if (modifier & ECORE_IMF_KEYBOARD_MODIFIER_SHIFT)
     state |= IBUS_SHIFT_MASK;

   /**< "Win" (between "Ctrl" and "Alt") */
   if (modifier & ECORE_IMF_KEYBOARD_MODIFIER_WIN)
     state |= IBUS_SUPER_MASK;

   /**< "AltGr" is pressed */
   if (modifier & ECORE_IMF_KEYBOARD_MODIFIER_ALTGR)
     state |= IBUS_MOD5_MASK;

   return state;
}

static unsigned int
_ecore_imf_locks_to_ibus_modifier(unsigned int locks)
{
   unsigned int state = 0;

   /**< "Num lock" is pressed */
   if (locks & ECORE_IMF_KEYBOARD_LOCK_NUM)
     state |= IBUS_MOD2_MASK;

   /**< "Caps lock" is pressed */
   if (locks & ECORE_IMF_KEYBOARD_LOCK_CAPS)
     state |= IBUS_LOCK_MASK;

   return state;
}

static void
_ecore_imf_ibus_key_event_put(int keysym, int keycode, int state)
{
   // Find the window which has the current keyboard focus.
   Window winFocus = 0;
   int revert = RevertToParent;

   if (!ecore_x_display_get()) return;
   XGetInputFocus(ecore_x_display_get(), &winFocus, &revert);

   XKeyEvent event;
   if (state & IBUS_RELEASE_MASK)
     {
        event = _ecore_imf_ibus_x_key_event_generate(winFocus,
                                                     EINA_FALSE,
                                                     keysym,
                                                     keycode,
                                                     state);
        XSendEvent(event.display, event.window, True, KeyReleaseMask, (XEvent *)&event);
     }
   else
     {
        event = _ecore_imf_ibus_x_key_event_generate(winFocus,
                                                     EINA_TRUE,
                                                     keysym,
                                                     keycode,
                                                     state);
        XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
     }
}

static KeyEvent *
_ecore_imf_ibus_key_event_copy(int keysym, int keycode, int state)
{
   KeyEvent *kev = calloc(1, sizeof(KeyEvent));
   kev->keysym = keysym;
   kev->keycode = keycode;
   kev->state = state;

   return kev;
}

static void
_ecore_imf_ibus_process_key_event_done(GObject      *object,
                                       GAsyncResult *res,
                                       gpointer      user_data)
{
   IBusInputContext *context = (IBusInputContext *)object;
   KeyEvent *event = (KeyEvent *)user_data;

   GError *error = NULL;
   Eina_Bool retval = ibus_input_context_process_key_event_async_finish(context,
                                                                        res,
                                                                        &error);

   if (error != NULL)
     {
        g_warning("Process Key Event failed: %s.", error->message);
        g_error_free(error);
     }

   if (retval == EINA_FALSE)
     {
        _ecore_imf_ibus_key_event_put(event->keysym,
                                      event->keycode,
                                      event->state);
     }
   free(event);
}

static void
_request_surrounding_text(IBusIMContext *ibusimcontext)
{
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext->ibuscontext);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext->ctx);

   if ((ibusimcontext->caps & IBUS_CAP_SURROUNDING_TEXT) != 0 &&
       ibus_input_context_needs_surrounding_text(ibusimcontext->ibuscontext))
     {
        char *surrounding = NULL;
        int cursor_pos;
        IBusText *ibustext;

        EINA_LOG_DBG ("requesting surrounding text...\n");

        if (ecore_imf_context_surrounding_get(ibusimcontext->ctx,
                                              &surrounding,
                                              &cursor_pos))
          {
             if (!surrounding)
               return;

             if (cursor_pos < 0)
               {
                  free(surrounding);
                  return;
               }

             ibustext = ibus_text_new_from_string(surrounding);

             ibus_input_context_set_surrounding_text(ibusimcontext->ibuscontext,
                                                     ibustext,
                                                     cursor_pos,
                                                     cursor_pos);

             free(surrounding);
          }
        else
          {
             ibusimcontext->caps &= ~IBUS_CAP_SURROUNDING_TEXT;
             ibus_input_context_set_capabilities(ibusimcontext->ibuscontext,
                                                 ibusimcontext->caps);
          }
     }
}

IBusIMContext *
ecore_imf_context_ibus_new(void)
{
   EINA_LOG_DBG("%s", __FUNCTION__);

   IBusIMContext *context = calloc(1, sizeof(IBusIMContext));

   /* init bus object */
   if (_bus == NULL)
     {
        char *display_name = NULL;

        if ((display_name = getenv("DISPLAY")))
          ibus_set_display(display_name);
        else
          ibus_set_display(":0.0");

        _bus = ibus_bus_new();
     }

   return context;
}

void
ecore_imf_context_ibus_shutdown(void)
{
   if (_bus)
     {
        g_object_unref(_bus);
        _bus = NULL;
     }
}

void
ecore_imf_context_ibus_add(Ecore_IMF_Context *ctx)
{
   EINA_LOG_DBG("%s", __FUNCTION__);

   char *s = NULL;
   IBusIMContext *ibusimcontext = (IBusIMContext *)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   ibusimcontext->client_window = 0;

   // Init preedit status
   ibusimcontext->preedit_string = NULL;
   ibusimcontext->preedit_attrs = NULL;
   ibusimcontext->preedit_cursor_pos = 0;
   ibusimcontext->preedit_visible = EINA_FALSE;

   // Init cursor area
   ibusimcontext->cursor_x = -1;
   ibusimcontext->cursor_y = -1;
   ibusimcontext->cursor_w = 0;
   ibusimcontext->cursor_h = 0;

   ibusimcontext->ibuscontext = NULL;
   ibusimcontext->has_focus = EINA_FALSE;
   ibusimcontext->caps = IBUS_CAP_PREEDIT_TEXT | IBUS_CAP_FOCUS | IBUS_CAP_SURROUNDING_TEXT;;
   ibusimcontext->ctx = ctx;

   s = getenv("IBUS_ENABLE_SYNC_MODE");
   if (s)
     _sync_mode_use = !!atoi(s);

   if (ibus_bus_is_connected(_bus))
     _ecore_imf_context_ibus_create(ibusimcontext);

   g_signal_connect(_bus, "connected", G_CALLBACK (_ecore_imf_context_ibus_bus_connected_cb), ibusimcontext);
}

void
ecore_imf_context_ibus_del(Ecore_IMF_Context *ctx)
{
   EINA_LOG_DBG("%s", __FUNCTION__);

   IBusIMContext *ibusimcontext = (IBusIMContext*)ecore_imf_context_data_get(ctx);
   Ecore_IMF_Preedit_Attr *attr = NULL;

   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   g_signal_handlers_disconnect_by_func(_bus,
                                        G_CALLBACK(_ecore_imf_context_ibus_bus_connected_cb),
                                        ibusimcontext);

   if (ibusimcontext->ibuscontext)
     ibus_proxy_destroy((IBusProxy *)ibusimcontext->ibuscontext);

   // release preedit
   if (ibusimcontext->preedit_string)
     free(ibusimcontext->preedit_string);
   ibusimcontext->preedit_string = NULL;

   if (ibusimcontext->preedit_attrs)
     {
        EINA_LIST_FREE(ibusimcontext->preedit_attrs, attr)
           free(attr);
     }

   if (_focus_im_context == ctx)
     _focus_im_context = NULL;

   free(ibusimcontext);
}

Eina_Bool
ecore_imf_context_ibus_filter_event(Ecore_IMF_Context *ctx,
                                    Ecore_IMF_Event_Type type,
                                    Ecore_IMF_Event *event)
{
   IBusIMContext *ibusimcontext = (IBusIMContext*)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN_VAL(ibusimcontext, EINA_FALSE);

   if (!ecore_x_display_get()) return EINA_FALSE;
   if (type != ECORE_IMF_EVENT_KEY_UP && type != ECORE_IMF_EVENT_KEY_DOWN)
     return EINA_FALSE;

   EINA_LOG_DBG("%s", __FUNCTION__);

   if (G_LIKELY(ibusimcontext->ibuscontext && ibusimcontext->has_focus))
     {
        /* If context does not have focus, ibus will process key event in sync mode.
         * It is a workaround for increase search in treeview.
         */
        Eina_Bool retval = EINA_FALSE;
        int keycode;
        int keysym;
        unsigned int state = 0;

        if (type == ECORE_IMF_EVENT_KEY_UP)
          {
             Ecore_IMF_Event_Key_Up *ev = (Ecore_IMF_Event_Key_Up *)event;
             if (ev->timestamp == 0)
               return EINA_FALSE;

             keycode = ecore_x_keysym_keycode_get(ev->keyname);
             keysym = XStringToKeysym(ev->key);
             state = _ecore_imf_modifier_to_ibus_modifier(ev->modifiers) |
                     _ecore_imf_locks_to_ibus_modifier(ev->locks) | IBUS_RELEASE_MASK;

             if (_sync_mode_use)
               {
                  retval = ibus_input_context_process_key_event(ibusimcontext->ibuscontext,
                                                                keysym,
                                                                keycode - 8,
                                                                state);
               }
             else
               {
                  ibus_input_context_process_key_event_async(ibusimcontext->ibuscontext,
                                                             keysym,
                                                             keycode - 8,
                                                             state,
                                                             -1,
                                                             NULL,
                                                             _ecore_imf_ibus_process_key_event_done,
                                                             _ecore_imf_ibus_key_event_copy(keysym, keycode, state));
                  retval = EINA_TRUE;
               }
          }
        else if (type == ECORE_IMF_EVENT_KEY_DOWN)
          {
             Ecore_IMF_Event_Key_Down *ev = (Ecore_IMF_Event_Key_Down *)event;
             if (ev->timestamp == 0)
               return EINA_FALSE;

             _request_surrounding_text(ibusimcontext);

             keycode = ecore_x_keysym_keycode_get(ev->keyname);
             keysym = XStringToKeysym(ev->key);
             state = _ecore_imf_modifier_to_ibus_modifier(ev->modifiers) |
                     _ecore_imf_locks_to_ibus_modifier(ev->locks);

             if (_sync_mode_use)
               {
                  retval = ibus_input_context_process_key_event(ibusimcontext->ibuscontext,
                                                                keysym,
                                                                keycode - 8,
                                                                state);
               }
             else
               {
                  ibus_input_context_process_key_event_async(ibusimcontext->ibuscontext,
                                                             keysym,
                                                             keycode - 8,
                                                             state,
                                                             -1,
                                                             NULL,
                                                             _ecore_imf_ibus_process_key_event_done,
                                                             _ecore_imf_ibus_key_event_copy(keysym, keycode, state));
                  retval = EINA_TRUE;
               }
          }

        if (retval)
          return EINA_TRUE;
        else
          return EINA_FALSE;
     }
   else
     return EINA_FALSE;
}

void
ecore_imf_context_ibus_focus_in(Ecore_IMF_Context *ctx)
{
   EINA_LOG_DBG("ctx : %p", ctx);

   IBusIMContext *ibusimcontext = (IBusIMContext*)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (ibusimcontext->has_focus)
     return;

   if (_focus_im_context != NULL)
     ecore_imf_context_focus_out(_focus_im_context);

   ibusimcontext->has_focus = EINA_TRUE;
   if (ibusimcontext->ibuscontext)
     ibus_input_context_focus_in(ibusimcontext->ibuscontext);

   _request_surrounding_text(ibusimcontext);

   if (_focus_im_context != ctx)
     _focus_im_context = ctx;
}

void
ecore_imf_context_ibus_focus_out(Ecore_IMF_Context *ctx)
{
   EINA_LOG_DBG("ctx : %p", ctx);

   IBusIMContext *ibusimcontext = (IBusIMContext *)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (ibusimcontext->has_focus == EINA_FALSE)
     return;

   if (_focus_im_context == ctx)
     _focus_im_context = NULL;

   ibusimcontext->has_focus = EINA_FALSE;
   if (ibusimcontext->ibuscontext)
     ibus_input_context_focus_out(ibusimcontext->ibuscontext);
}

void
ecore_imf_context_ibus_reset(Ecore_IMF_Context *ctx)
{
   IBusIMContext *ibusimcontext = (IBusIMContext*)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (ibusimcontext->ibuscontext)
     ibus_input_context_reset(ibusimcontext->ibuscontext);
}

void
ecore_imf_context_ibus_preedit_string_get(Ecore_IMF_Context *ctx,
                                          char          **str,
                                          int            *cursor_pos)
{
   IBusIMContext *ibusimcontext = (IBusIMContext*)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (ibusimcontext->preedit_visible)
     {
        if (str)
          *str = strdup(ibusimcontext->preedit_string ? ibusimcontext->preedit_string: "");

        if (cursor_pos)
          *cursor_pos = ibusimcontext->preedit_cursor_pos;
     }
   else
     {
        if (str)
          *str = strdup("");

        if (cursor_pos)
          *cursor_pos = 0;
     }

   if (str)
     EINA_LOG_DBG("str : %s", *str);

   if (cursor_pos)
     EINA_LOG_DBG("cursor_pos : %d", *cursor_pos);
}

void
ecore_imf_context_ibus_preedit_string_with_attributes_get(Ecore_IMF_Context *ctx,
                                                          char          **str,
                                                          Eina_List     **attrs,
                                                          int            *cursor_pos)
{
   IBusIMContext *ibusimcontext = (IBusIMContext*)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);
   Eina_List *l;
   Ecore_IMF_Preedit_Attr *attr1 = NULL, *attr2 = NULL;

   ecore_imf_context_ibus_preedit_string_get(ctx, str, cursor_pos);

   if (attrs)
     {
        if (ibusimcontext->preedit_attrs)
          {
             EINA_LIST_FOREACH(ibusimcontext->preedit_attrs, l, attr1)
               {
                  attr2 = (Ecore_IMF_Preedit_Attr *)calloc(1, sizeof(Ecore_IMF_Preedit_Attr));
                  if (!attr2) continue;
                  attr2->preedit_type = attr1->preedit_type;
                  attr2->start_index = attr1->start_index;
                  attr2->end_index = attr1->end_index;

                  *attrs = eina_list_append(*attrs, (void *)attr2);
               }
          }
        else
          *attrs = NULL;
     }
}

void
ecore_imf_context_ibus_client_window_set(Ecore_IMF_Context *ctx, void *window)
{
   EINA_LOG_DBG("canvas : %p", window);
   IBusIMContext *ibusimcontext = (IBusIMContext *)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (window != NULL)
     ibusimcontext->client_window = (Ecore_X_Window)(Ecore_Window)window;
}

void
ecore_imf_context_ibus_client_canvas_set(Ecore_IMF_Context *ctx, void *canvas)
{
   EINA_LOG_DBG("canvas : %p", canvas);
   IBusIMContext *ibusimcontext = (IBusIMContext *)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (canvas != NULL)
     ibusimcontext->client_canvas = canvas;
}

static void
_ecore_imf_context_ibus_cursor_location_set(Ecore_IMF_Context *ctx)
{
   IBusIMContext *ibusimcontext = (IBusIMContext *)ecore_imf_context_data_get(ctx);
   Ecore_Evas *ee;
   int canvas_x, canvas_y;
   Ecore_X_Window client_window = 0;

   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (ibusimcontext->ibuscontext == NULL)
     return;

   if (ibusimcontext->client_window)
     client_window = ibusimcontext->client_window;
   else
     {
        if (ibusimcontext->client_canvas)
          {
             ee = ecore_evas_ecore_evas_get(ibusimcontext->client_canvas);
             if (ee)
               client_window = (Ecore_X_Window)ecore_evas_window_get(ee);
          }
     }

   _ecore_imf_ibus_window_to_screen_geometry_get(client_window,
                                                 &canvas_x,
                                                 &canvas_y);

   ibus_input_context_set_cursor_location(ibusimcontext->ibuscontext,
                                          ibusimcontext->cursor_x + canvas_x,
                                          ibusimcontext->cursor_y + canvas_y,
                                          ibusimcontext->cursor_w,
                                          ibusimcontext->cursor_h);
}

void
ecore_imf_context_ibus_cursor_location_set(Ecore_IMF_Context *ctx,
                                           int x,
                                           int y,
                                           int w,
                                           int h)
{
   EINA_LOG_DBG("x : %d, y : %d, w, %d, h :%d", x, y, w, h);
   IBusIMContext *ibusimcontext = (IBusIMContext *)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (ibusimcontext->cursor_x != x ||
       ibusimcontext->cursor_y != y ||
       ibusimcontext->cursor_w != w ||
       ibusimcontext->cursor_h != h)
     {
        ibusimcontext->cursor_x = x;
        ibusimcontext->cursor_y = y;
        ibusimcontext->cursor_w = w;
        ibusimcontext->cursor_h = h;

        _ecore_imf_context_ibus_cursor_location_set(ctx);
     }
}

void
ecore_imf_context_ibus_use_preedit_set(Ecore_IMF_Context *ctx, Eina_Bool use_preedit)
{
   EINA_LOG_DBG("preedit : %d", use_preedit);
   IBusIMContext *ibusimcontext = (IBusIMContext *)ecore_imf_context_data_get(ctx);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (ibusimcontext->ibuscontext)
     {
        if (use_preedit)
          ibusimcontext->caps |= IBUS_CAP_PREEDIT_TEXT;
        else
          ibusimcontext->caps &= ~IBUS_CAP_PREEDIT_TEXT;

        ibus_input_context_set_capabilities(ibusimcontext->ibuscontext, ibusimcontext->caps);
     }
}

static void
_ecore_imf_context_ibus_bus_connected_cb(IBusBus       *bus EINA_UNUSED,
                                         IBusIMContext *ibusimcontext)
{
   EINA_LOG_DBG("ibus is connected");

   if (ibusimcontext)
     _ecore_imf_context_ibus_create(ibusimcontext);
}

static void
_ecore_imf_context_ibus_commit_text_cb(IBusInputContext *ibuscontext EINA_UNUSED,
                                       IBusText         *text,
                                       IBusIMContext    *ibusimcontext)
{
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);
   EINA_SAFETY_ON_NULL_RETURN(text);
   char *commit_str = text->text ? text->text : "";

   EINA_LOG_DBG("commit string : %s", commit_str);

   if (ibusimcontext->ctx)
     {
        ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                              ECORE_IMF_CALLBACK_COMMIT,
                                              (void *)commit_str);

        _request_surrounding_text(ibusimcontext);
     }
}

static XKeyEvent _ecore_imf_ibus_x_key_event_generate(Window win,
                                                      Eina_Bool press,
                                                      int keysym,
                                                      int keycode,
                                                      int modifiers)
{
   XKeyEvent event;
   Display *display = ecore_x_display_get();

   event.display     = display;
   event.window      = win;
   event.root        = display ? ecore_x_window_root_get(win) : 0;
   event.subwindow   = None;
   event.time        = 0;
   event.x           = 1;
   event.y           = 1;
   event.x_root      = 1;
   event.y_root      = 1;
   event.same_screen = EINA_TRUE;
   if (keycode == -1)
     {
        event.keycode     = display ? XKeysymToKeycode(display, keysym) : 0;
        event.state       = 0;
     }
   else
     {
        event.keycode     = keycode;
        event.state       = modifiers;
     }
   if (press)
     event.type = KeyPress;
   else
     event.type = KeyRelease;
   event.send_event  = EINA_FALSE;
   event.serial = 0;

   return event;
}

static void
_ecore_imf_context_ibus_forward_key_event_cb(IBusInputContext  *ibuscontext EINA_UNUSED,
                                             guint              keyval,
                                             guint              state,
                                             IBusIMContext     *ibusimcontext EINA_UNUSED)
{
   EINA_LOG_DBG("keyval : %d, state : %d", keyval, state);

   _ecore_imf_ibus_key_event_put(keyval, -1, state);
}

static void
_ecore_imf_context_ibus_delete_surrounding_text_cb(IBusInputContext *ibuscontext EINA_UNUSED,
                                                   gint              offset_from_cursor,
                                                   guint             nchars,
                                                   IBusIMContext    *ibusimcontext)
{
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (_focus_im_context != ibusimcontext->ctx)
     return;

   Ecore_IMF_Event_Delete_Surrounding ev;
   ev.ctx = _focus_im_context;
   ev.n_chars = nchars;
   ev.offset = offset_from_cursor;
   ecore_imf_context_event_callback_call(_focus_im_context,
                                         ECORE_IMF_CALLBACK_DELETE_SURROUNDING,
                                         &ev);
}

static void
_ecore_imf_context_ibus_update_preedit_text_cb(IBusInputContext  *ibuscontext EINA_UNUSED,
                                               IBusText          *text,
                                               gint               cursor_pos,
                                               gboolean           visible,
                                               IBusIMContext     *ibusimcontext)
{
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);
   EINA_SAFETY_ON_NULL_RETURN(text);

   const char *str;
   gboolean flag;
   Ecore_IMF_Preedit_Attr *attr = NULL;

   if (ibusimcontext->preedit_string)
     free(ibusimcontext->preedit_string);

   if (ibusimcontext->preedit_attrs)
     {
        EINA_LIST_FREE(ibusimcontext->preedit_attrs, attr)
           free(attr);

        ibusimcontext->preedit_attrs = NULL;
     }

   str = text->text;

   if (str)
     ibusimcontext->preedit_string = strdup(str);
   else
     ibusimcontext->preedit_string = strdup("");

   if (text->attrs)
     {
        unsigned int i;
        unsigned int pos;
        unsigned int preedit_length;
        preedit_length = strlen(ibusimcontext->preedit_string);
        Eina_Bool *attrs_flag = calloc(1, sizeof(Eina_Bool)*preedit_length);

        for (i = 0; ; i++)
          {
             attr = NULL;
             IBusAttribute *ibus_attr = ibus_attr_list_get(text->attrs, i);
             if (ibus_attr == NULL)
               break;

             attr = (Ecore_IMF_Preedit_Attr *)calloc(1, sizeof(Ecore_IMF_Preedit_Attr));
             if (attr == NULL)
               continue;

             attr->start_index = utf8_offset_to_index(ibusimcontext->preedit_string,
                                                      ibus_attr->start_index);
             attr->end_index = utf8_offset_to_index(ibusimcontext->preedit_string,
                                                    ibus_attr->end_index);

             switch (ibus_attr->type)
               {
                case IBUS_ATTR_TYPE_FOREGROUND:
                   attr->preedit_type = ECORE_IMF_PREEDIT_TYPE_SUB2;
                   for (pos = attr->start_index; pos < attr->end_index; ++pos)
                     attrs_flag[pos] = 1;
                   break;
                default:
                   if (attr)
                     {
                        free(attr);
                        attr = NULL;
                     }
                   continue;
               }

             if (attr)
               ibusimcontext->preedit_attrs = eina_list_append(ibusimcontext->preedit_attrs,
                                                               (void *)attr);
          }

        // Add underline for all characters which don't have attribute.
        for (pos = 0; pos < preedit_length; ++pos)
          {
             if (!attrs_flag[pos])
               {
                  int begin_pos = pos;

                  while (pos < preedit_length && !attrs_flag[pos])
                    ++pos;

                  attr = (Ecore_IMF_Preedit_Attr *)calloc(1, sizeof(Ecore_IMF_Preedit_Attr));
                  if (attr == NULL)
                    continue;
                  attr->preedit_type = ECORE_IMF_PREEDIT_TYPE_SUB1;
                  attr->start_index = begin_pos;
                  attr->end_index = pos;
                  ibusimcontext->preedit_attrs = eina_list_append(ibusimcontext->preedit_attrs,
                                                                  (void *)attr);
               }
          }

        if (attrs_flag)
          free(attrs_flag);

        ibusimcontext->preedit_attrs = eina_list_sort(ibusimcontext->preedit_attrs,
                                                      eina_list_count(ibusimcontext->preedit_attrs),
                                                      sort_cb);
     }

   ibusimcontext->preedit_cursor_pos = cursor_pos;

   EINA_LOG_DBG("string : %s, cursor : %d",
                ibusimcontext->preedit_string,
                ibusimcontext->preedit_cursor_pos);

   flag = ibusimcontext->preedit_visible != visible;
   ibusimcontext->preedit_visible = visible;

   if (ibusimcontext->preedit_visible)
     {
        if (flag)
          {
             ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                                   ECORE_IMF_CALLBACK_PREEDIT_START,
                                                   NULL);
          }

        ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                              ECORE_IMF_CALLBACK_PREEDIT_CHANGED,
                                              NULL);
     }
   else
     {
        if (flag)
          {
             ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                                   ECORE_IMF_CALLBACK_PREEDIT_CHANGED,
                                                   NULL);
          }

        ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                              ECORE_IMF_CALLBACK_PREEDIT_END,
                                              NULL);
     }
}

static void
_ecore_imf_context_ibus_show_preedit_text_cb(IBusInputContext *ibuscontext EINA_UNUSED,
                                             IBusIMContext    *ibusimcontext)
{
   EINA_LOG_DBG("preedit visible : %d", ibusimcontext->preedit_visible);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (ibusimcontext->preedit_visible == EINA_TRUE)
     return;

   ibusimcontext->preedit_visible = EINA_TRUE;

   // call preedit start
   ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                         ECORE_IMF_CALLBACK_PREEDIT_START,
                                         NULL);

   // call preedit changed
   ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                         ECORE_IMF_CALLBACK_PREEDIT_CHANGED,
                                         NULL);

   _request_surrounding_text(ibusimcontext);
}

static void
_ecore_imf_context_ibus_hide_preedit_text_cb(IBusInputContext *ibuscontext EINA_UNUSED,
                                             IBusIMContext    *ibusimcontext)
{
   EINA_LOG_DBG("%s", __FUNCTION__);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   if (ibusimcontext->preedit_visible == EINA_FALSE)
     return;

   ibusimcontext->preedit_visible = EINA_FALSE;

   // call preedit changed
   ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                         ECORE_IMF_CALLBACK_PREEDIT_CHANGED,
                                         NULL);

   // call preedit end
   ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                         ECORE_IMF_CALLBACK_PREEDIT_END,
                                         NULL);
}

static void
_ecore_imf_context_ibus_destroy_cb(IBusInputContext *ibuscontext EINA_UNUSED,
                                   IBusIMContext    *ibusimcontext)
{
   EINA_LOG_DBG("%s", __FUNCTION__);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   g_object_unref(ibusimcontext->ibuscontext);
   ibusimcontext->ibuscontext = NULL;

   /* clear preedit */
   ibusimcontext->preedit_visible = EINA_FALSE;
   ibusimcontext->preedit_cursor_pos = 0;
   free(ibusimcontext->preedit_string);
   ibusimcontext->preedit_string = NULL;

   // call preedit changed
   ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                         ECORE_IMF_CALLBACK_PREEDIT_CHANGED,
                                         NULL);

   // call preedit end
   ecore_imf_context_event_callback_call(ibusimcontext->ctx,
                                         ECORE_IMF_CALLBACK_PREEDIT_END,
                                         NULL);
}

static void
_ecore_imf_context_ibus_create(IBusIMContext *ibusimcontext)
{
   EINA_LOG_DBG("%s", __FUNCTION__);
   EINA_SAFETY_ON_NULL_RETURN(ibusimcontext);

   ibusimcontext->ibuscontext = ibus_bus_create_input_context(_bus, "ecore");

   g_return_if_fail(ibusimcontext->ibuscontext != NULL);

   g_signal_connect(ibusimcontext->ibuscontext,
                    "commit-text",
                    G_CALLBACK (_ecore_imf_context_ibus_commit_text_cb),
                    ibusimcontext);
   g_signal_connect(ibusimcontext->ibuscontext,
                    "forward-key-event",
                    G_CALLBACK (_ecore_imf_context_ibus_forward_key_event_cb),
                    ibusimcontext);
   g_signal_connect(ibusimcontext->ibuscontext,
                    "delete-surrounding-text",
                    G_CALLBACK (_ecore_imf_context_ibus_delete_surrounding_text_cb),
                    ibusimcontext);
   g_signal_connect(ibusimcontext->ibuscontext,
                    "update-preedit-text",
                    G_CALLBACK (_ecore_imf_context_ibus_update_preedit_text_cb),
                    ibusimcontext);
   g_signal_connect(ibusimcontext->ibuscontext,
                    "show-preedit-text",
                    G_CALLBACK (_ecore_imf_context_ibus_show_preedit_text_cb),
                    ibusimcontext);
   g_signal_connect(ibusimcontext->ibuscontext,
                    "hide-preedit-text",
                    G_CALLBACK (_ecore_imf_context_ibus_hide_preedit_text_cb),
                    ibusimcontext);
   g_signal_connect(ibusimcontext->ibuscontext, "destroy",
                    G_CALLBACK (_ecore_imf_context_ibus_destroy_cb),
                    ibusimcontext);

   ibus_input_context_set_capabilities(ibusimcontext->ibuscontext,
                                       ibusimcontext->caps);

   if (ibusimcontext->has_focus)
     ibus_input_context_focus_in(ibusimcontext->ibuscontext);
}
