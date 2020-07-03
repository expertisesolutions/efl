#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_TSLIB
# include <tslib.h>
# include <errno.h>
#endif

#include "Ecore_Fb.h"
#include "ecore_fb_private.h"

typedef struct _Ecore_Fb_Ts_Event Ecore_Fb_Ts_Event;
typedef struct _Ecore_Fb_Ts_Calibrate Ecore_Fb_Ts_Calibrate;
typedef struct _Ecore_Fb_Ts_Backlight Ecore_Fb_Ts_Backlight;
typedef struct _Ecore_Fb_Ts_Contrast Ecore_Fb_Ts_Contrast;
typedef struct _Ecore_Fb_Ts_Led Ecore_Fb_Ts_Led;
typedef struct _Ecore_Fb_Ts_Flite Ecore_Fb_Ts_Flite;

struct _Ecore_Fb_Ts_Event
{
   unsigned short pressure;
   unsigned short x;
   unsigned short y;
   unsigned short _unused;
};

struct _Ecore_Fb_Ts_Calibrate
{
   int xscale;
   int xtrans;
   int yscale;
   int ytrans;
   int xyswap;
};

struct _Ecore_Fb_Ts_Backlight
{
   int           on;
   unsigned char brightness;
};

struct _Ecore_Fb_Ts_Contrast
{
   unsigned char contrast;
};

struct _Ecore_Fb_Ts_Led
{
   unsigned char on;
   unsigned char blink_time;
   unsigned char on_time;
   unsigned char off_time;
};

struct _Ecore_Fb_Ts_Flite
{
   unsigned char mode;
   unsigned char pwr;
   unsigned char brightness;
};

static Eina_Bool _ecore_fb_ts_fd_handler(void *data, Ecore_Fd_Handler *fd_handler);
static int _ecore_fb_ts_fd = -1;
static int _ecore_fb_ts_apply_cal = 0;
#ifndef HAVE_TSLIB
static int _ecore_fb_ts_event_byte_count = 0;
static Ecore_Fb_Ts_Event _ecore_fb_ts_event;
#endif
static Ecore_Fb_Ts_Calibrate _ecore_fb_ts_cal = {1,1,0,0,0};
static Ecore_Fd_Handler *_ecore_fb_ts_fd_handler_handle = NULL;

#ifdef HAVE_TSLIB
struct tsdev *_ecore_fb_tslib_tsdev = NULL;
struct ts_sample _ecore_fb_tslib_event;
#endif

static double _ecore_fb_double_click_time = 0.25;
static void *_ecore_fb_ts_event_window = NULL;

ECORE_FB_API int
ecore_fb_ts_init(void)
{
#ifdef HAVE_TSLIB
   char *tslib_tsdevice = NULL;
#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
   if (getuid() == geteuid())
#endif
     {
        if ((tslib_tsdevice = getenv("TSLIB_TSDEVICE")) == NULL)
          tslib_tsdevice = "/dev/input/event0";
        printf( "ECORE_FB: TSLIB_TSDEVICE = '%s'\n", tslib_tsdevice );
        _ecore_fb_tslib_tsdev = ts_open( tslib_tsdevice, 1 ); /* 1 = nonblocking, 0 = blocking */

        if ( !_ecore_fb_tslib_tsdev )
          {
             printf( "ECORE_FB: Can't ts_open (%s)\n", strerror( errno ) );
             return 0;
          }

        if ( ts_config( _ecore_fb_tslib_tsdev ) )
          {
             printf( "ECORE_FB: Can't ts_config (%s)\n", strerror( errno ) );
             return 0;
          }
        _ecore_fb_ts_fd = ts_fd( _ecore_fb_tslib_tsdev );
        if ( _ecore_fb_ts_fd < 0 )
          {
             printf( "ECORE_FB: Can't open touchscreen (%s)\n", strerror( errno ) );
             return 0;
          }
     }
#else
   _ecore_fb_ts_fd = open("/dev/touchscreen/0", O_RDONLY);
#endif
   if (_ecore_fb_ts_fd >= 0)
     {
        _ecore_fb_ts_fd_handler_handle = ecore_main_fd_handler_add(_ecore_fb_ts_fd,
                                                                   ECORE_FD_READ,
                                                                   _ecore_fb_ts_fd_handler, NULL,
                                                                   NULL, NULL);
        if (!_ecore_fb_ts_fd_handler_handle)
          {
             close(_ecore_fb_ts_fd);
             return 0;
          }
        // FIXME _ecore_fb_kbd_fd = open("/dev/touchscreen/key", O_RDONLY);
        return 1;
     }
   return 0;
}

ECORE_FB_API void
ecore_fb_ts_shutdown(void)
{
   if (_ecore_fb_ts_fd_handler_handle)
      ecore_main_fd_handler_del(_ecore_fb_ts_fd_handler_handle);
   if (_ecore_fb_ts_fd >= 0) close(_ecore_fb_ts_fd);
   _ecore_fb_ts_fd = -1;
   _ecore_fb_ts_fd_handler_handle = NULL;
   _ecore_fb_ts_event_window = NULL;
}

ECORE_FB_API void
ecore_fb_ts_event_window_set(void *window)
{
   _ecore_fb_ts_event_window = window;
}

ECORE_FB_API void *
ecore_fb_ts_event_window_get(void)
{
    return _ecore_fb_ts_event_window;
}

/**
 * @defgroup Ecore_FB_Calibrate_Group Framebuffer Calibration Functions
 * @ingroup Ecore_FB_Group
 *
 * Functions that calibrate the screen.
 */


/**
 * Calibrates the touschreen using the given parameters.
 * @param   xscale X scaling, where 256 = 1.0
 * @param   xtrans X translation.
 * @param   yscale Y scaling.
 * @param   ytrans Y translation.
 * @param   xyswap Swap X & Y flag.
 * @ingroup Ecore_FB_Calibrate_Group
 */
ECORE_FB_API void
ecore_fb_touch_screen_calibrate_set(int xscale, int xtrans, int yscale, int ytrans, int xyswap)
{
   Ecore_Fb_Ts_Calibrate cal;

   if (_ecore_fb_ts_fd < 0) return;
   cal.xscale = xscale;
   cal.xtrans = xtrans;
   cal.yscale = yscale;
   cal.ytrans = ytrans;
   cal.xyswap = xyswap;
   if (ioctl(_ecore_fb_ts_fd, TS_SET_CAL, (void *)&cal))
     {
        _ecore_fb_ts_cal = cal;
        _ecore_fb_ts_apply_cal = 1;
     }
}

/**
 * Retrieves the calibration parameters of the touchscreen.
 * @param   xscale Pointer to an integer in which to store the X scaling.
 *                 Note that 256 = 1.0.
 * @param   xtrans Pointer to an integer in which to store the X translation.
 * @param   yscale Pointer to an integer in which to store the Y scaling.
 * @param   ytrans Pointer to an integer in which to store the Y translation.
 * @param   xyswap Pointer to an integer in which to store the Swap X & Y flag.
 * @ingroup Ecore_FB_Calibrate_Group
 */
ECORE_FB_API void
ecore_fb_touch_screen_calibrate_get(int *xscale, int *xtrans, int *yscale, int *ytrans, int *xyswap)
{
   Ecore_Fb_Ts_Calibrate cal;

   if (_ecore_fb_ts_fd < 0) return;
   if (!_ecore_fb_ts_apply_cal)
     {
        if (ioctl(_ecore_fb_ts_fd, TS_GET_CAL, (void *)&cal))
           _ecore_fb_ts_cal = cal;
     }
   else
      cal = _ecore_fb_ts_cal;
   if (xscale) *xscale = cal.xscale;
   if (xtrans) *xtrans = cal.xtrans;
   if (yscale) *yscale = cal.yscale;
   if (ytrans) *ytrans = cal.ytrans;
   if (xyswap) *xyswap = cal.xyswap;
}

static Eina_Bool
_ecore_fb_ts_fd_handler(void *data EINA_UNUSED, Ecore_Fd_Handler *fd_handler EINA_UNUSED)
{
   static int prev_x = 0, prev_y = 0, prev_pressure = 0;
   static double last_time = 0;
   static double last_last_time = 0;
   int v = 0;

   do
     {
        int x, y, pressure;
        int num;
        double t = 0.0;
        static int did_double = 0;
        static int did_triple = 0;

#ifdef HAVE_TSLIB
        if (_ecore_fb_ts_apply_cal)
           num = ts_read_raw(_ecore_fb_tslib_tsdev, &_ecore_fb_tslib_event, 1);
        else
           num = ts_read(_ecore_fb_tslib_tsdev, &_ecore_fb_tslib_event, 1);
        if (num != 1) return 1; /* no more samples at this time */
        x = _ecore_fb_tslib_event.x;
        y = _ecore_fb_tslib_event.y;
        pressure = _ecore_fb_tslib_event.pressure;
        v = 1; /* loop, there might be more samples */
#else
        char *ptr;
        ptr = (char *)&(_ecore_fb_ts_event);
        ptr += _ecore_fb_ts_event_byte_count;
        num = sizeof(Ecore_Fb_Ts_Event) - _ecore_fb_ts_event_byte_count;
        v = read(_ecore_fb_ts_fd, ptr, num);
        if (v < 0) return 1;
        _ecore_fb_ts_event_byte_count += v;
        if (v < num) return 1;
        _ecore_fb_ts_event_byte_count = 0;
        if (_ecore_fb_ts_apply_cal)
          {
             x = ((_ecore_fb_ts_cal.xscale * _ecore_fb_ts_event.x) >> 8) + _ecore_fb_ts_cal.xtrans;
             y = ((_ecore_fb_ts_cal.yscale * _ecore_fb_ts_event.y) >> 8) + _ecore_fb_ts_cal.ytrans;
          }
        else
          {
             x = _ecore_fb_ts_event.x;
             y = _ecore_fb_ts_event.y;
          }
        pressure = _ecore_fb_ts_event.pressure;
#endif
        t = ecore_loop_time_get();
        /* add event to queue */
        /* always add a move event */
        if ((pressure) || (prev_pressure))
          {
             /* MOVE: mouse is down and was */
             Ecore_Event_Mouse_Move *e;

             e = calloc(1, sizeof(Ecore_Event_Mouse_Move));
             if (!e) goto retry;
             e->x = x;
             e->y = y;
             e->root.x = e->x;
             e->root.y = e->y;
             e->window = 1;
             e->event_window = e->window;
             e->root_window = e->window;
             e->same_screen = 1;
             e->timestamp = ecore_loop_time_get() * 1000.0;
             ecore_event_add(ECORE_EVENT_MOUSE_MOVE, e, NULL, NULL);
          }
        if ((pressure) && (!prev_pressure))
          {
             /* DOWN: mouse is down, but was not now */
             Ecore_Event_Mouse_Button *e;

             e = calloc(1, sizeof(Ecore_Event_Mouse_Button));
             if (!e) goto retry;
             e->x = x;
             e->y = y;
             e->root.x = e->x;
             e->root.y = e->y;
             e->buttons = 1;
             if ((t - last_time) <= _ecore_fb_double_click_time)
               {
                  e->double_click = 1;
                  did_double = 1;
               }
             else
               {
                  did_double = 0;
                  did_triple = 0;
               }
             if ((t - last_last_time) <= (2 * _ecore_fb_double_click_time))
               {
                  did_triple = 1;
                  e->triple_click = 1;
               }
             else
               {
                  did_triple = 0;
               }
             e->window = 1;
             e->event_window = e->window;
             e->root_window = e->window;
             e->same_screen = 1;
             e->timestamp = ecore_loop_time_get() * 1000.0;
             ecore_event_add(ECORE_EVENT_MOUSE_BUTTON_DOWN, e, NULL, NULL);
          }
        else if ((!pressure) && (prev_pressure))
          {
             /* UP: mouse was down, but is not now */
             Ecore_Event_Mouse_Button *e;

             e = calloc(1, sizeof(Ecore_Event_Mouse_Button));
             if (!e) goto retry;
             e->x = prev_x;
             e->y = prev_y;
             e->root.x = e->x;
             e->root.y = e->y;
             e->buttons = 1;
             if (did_double)
                e->double_click = 1;
             if (did_triple)
                e->triple_click = 1;
             e->window = 1;
             e->event_window = e->window;
             e->root_window = e->window;
             e->same_screen = 1;
             e->timestamp = ecore_loop_time_get() * 1000.0;
             ecore_event_add(ECORE_EVENT_MOUSE_BUTTON_UP, e, NULL, NULL);
          }
        if (did_triple)
          {
             last_time = 0;
             last_last_time = 0;
          }
        else
          {
             last_last_time = last_time;
             last_time = t;
          }
retry:
        prev_x = x;
        prev_y = y;
        prev_pressure = pressure;
     }
   while (v > 0);
   return 1;
}

