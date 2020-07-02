#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "Ecore_Fb.h"
#include "ecore_fb_private.h"

static int _ecore_fb_vt_do_switch = 0;

static int _ecore_fb_vt_tty0_fd = -1;
static int _ecore_fb_vt_tty_fd = -1;
static int _ecore_fb_vt_current_vt = 0;
static int _ecore_fb_vt_prev_vt = 0;

static struct termios _ecore_fb_tty_prev_tio_mode;
static struct vt_mode _ecore_fb_vt_prev_mode;

static Eina_Bool _ecore_fb_signal_usr_handler(void *data, int type, void *ev);
static Ecore_Event_Handler *_ecore_fb_user_handler = NULL;
static int _ecore_fb_tty_prev_mode = 0;
static int _ecore_fb_tty_prev_kd_mode = 0;

/* callbacks for an attach/release of a vt */
static void (*_ecore_fb_func_fb_lost) (void *data) = NULL;
static void *_ecore_fb_func_fb_lost_data = NULL;
static void (*_ecore_fb_func_fb_gain) (void *data) = NULL;
static void *_ecore_fb_func_fb_gain_data = NULL;

/* FIXME what is the filter for? */
static Ecore_Event_Filter *_ecore_fb_filter_handler = NULL;

/* prototypes */
/* XXX: unused
static void _ecore_fb_vt_switch(int vt);
static void *_ecore_fb_event_filter_start(void *data);
static Eina_Bool _ecore_fb_event_filter_filter(void *data, void *loop_data, int type, void *event);
static void _ecore_fb_event_filter_end(void *data, void *loop_data);
*/

static Eina_Bool
_ecore_fb_signal_usr_handler(void *data EINA_UNUSED, int type EINA_UNUSED, void *ev)
{
   Ecore_Event_Signal_User *e;

   e = (Ecore_Event_Signal_User *)ev;
   if (e->number == 1)
     {
        /* release vt */
        if (_ecore_fb_func_fb_lost) _ecore_fb_func_fb_lost(_ecore_fb_func_fb_lost_data);
        /* TODO stop listening from the devices? let the callback do it? */
        ioctl(_ecore_fb_vt_tty_fd, VT_RELDISP, 1);
     }
   else if (e->number == 2)
     {
        /* attach vt */
        if (_ecore_fb_func_fb_gain) _ecore_fb_func_fb_gain(_ecore_fb_func_fb_gain_data);
        /* TODO reattach all devices */
     }
   return 1;
}

/* XXX: unused
static void
_ecore_fb_vt_switch(int vt)
{
   vt++;
   if (_ecore_fb_vt_tty_fd != 0)
     {
        if (vt != _ecore_fb_vt_current_vt)
          {
             tcsetattr(_ecore_fb_vt_tty_fd, TCSAFLUSH, &_ecore_fb_tty_prev_tio_mode);
             ioctl(_ecore_fb_vt_tty_fd, KDSETMODE, _ecore_fb_tty_prev_kd_mode);
             ioctl(_ecore_fb_vt_tty_fd, KDSKBMODE, _ecore_fb_tty_prev_mode);
          }
     }
   ioctl(_ecore_fb_vt_tty_fd, VT_ACTIVATE, vt);
}
*/

static int
_ecore_fb_vt_setup(void)
{
   char buf[64];
// XXX: unused
//   struct termios tio;
   struct vt_mode new_vtmode;

   if (_ecore_fb_vt_current_vt != _ecore_fb_vt_prev_vt)
     {
        snprintf(buf, sizeof(buf), "/dev/tty%i", _ecore_fb_vt_current_vt);
        if ((_ecore_fb_vt_tty_fd = open(buf, O_RDWR)) < 0)
          {
             printf("[ecore_fb:vt_setup] can't open tty %d\n", _ecore_fb_vt_current_vt);
             return 0;
          }
        close(_ecore_fb_vt_tty0_fd);
        _ecore_fb_vt_tty0_fd = -1;
        /* FIXME detach the process from current tty ? */
     }
   else
      _ecore_fb_vt_tty_fd = _ecore_fb_vt_tty0_fd;
   /* for backup */
   tcgetattr(_ecore_fb_vt_tty_fd, &_ecore_fb_tty_prev_tio_mode);
   ioctl(_ecore_fb_vt_tty_fd, KDGETMODE, &_ecore_fb_tty_prev_kd_mode);
   ioctl(_ecore_fb_vt_tty_fd, VT_GETMODE, &_ecore_fb_vt_prev_mode);

   if (ioctl(_ecore_fb_vt_tty_fd, KDSETMODE, KD_GRAPHICS) < 0)
     {
        perror("[ecore_fb:vt_setup] can't set the mode to KD_GRAPHICS");
        close(_ecore_fb_vt_tty_fd);
        _ecore_fb_vt_tty_fd = -1;
        return 0;
     }
   ioctl(_ecore_fb_vt_tty_fd, KDGKBMODE, &_ecore_fb_tty_prev_mode);

   /* support of switching */
   memset(&new_vtmode, 0, sizeof(new_vtmode));
   new_vtmode.mode = VT_PROCESS;
   new_vtmode.waitv = 0;
   new_vtmode.relsig = SIGUSR1;
   new_vtmode.acqsig = SIGUSR2;
   if (ioctl(_ecore_fb_vt_tty_fd, VT_SETMODE, &new_vtmode) < 0)
     {
        perror("[ecore_fb:vt_setup] can't set the tty mode");
        close(_ecore_fb_vt_tty_fd);
        _ecore_fb_vt_tty_fd = -1;
        return 0;
     }
   /* register signal handlers when alloc/detach of vt */
   _ecore_fb_user_handler = ecore_event_handler_add(ECORE_EVENT_SIGNAL_USER,
                                                    _ecore_fb_signal_usr_handler,
                                                    NULL);
   /* What does this do? */
   /*
   _ecore_fb_filter_handler = ecore_event_filter_add(_ecore_fb_event_filter_start, _ecore_fb_event_filter_filter, _ecore_fb_event_filter_end, NULL);
   */

   usleep(40000);
   if (ioctl(_ecore_fb_vt_tty_fd, VT_ACTIVATE, _ecore_fb_vt_current_vt) < 0)
     {
        perror("[ecore_fb:vt_setup] error on VT_ACTIVATE");
        close(_ecore_fb_vt_tty_fd);
        _ecore_fb_vt_tty_fd = -1;
        return 0;
     }
   if(ioctl(_ecore_fb_vt_tty_fd, VT_WAITACTIVE, _ecore_fb_vt_current_vt) < 0)
     {
        perror("[ecore_fb:vt_setup] error on VT_WAITACTIVE");
        close(_ecore_fb_vt_tty_fd);
        _ecore_fb_vt_tty_fd = -1;
        return 0;
     }
   /* FIXME assign the fb to the tty in case isn't setup */
   return 1;
}

int
ecore_fb_vt_init(void)
{
   struct vt_stat vtstat;

   /* as root you can allocate another tty */
   if (!geteuid())
      _ecore_fb_vt_do_switch = 1;
   if ((_ecore_fb_vt_tty0_fd = open("/dev/tty0", O_RDONLY)) < 0)
     {
        printf("[ecore_fb:init] can't open /dev/tty0\n");
        return 0;
     }
   /* query current vt state */
   if ((ioctl(_ecore_fb_vt_tty0_fd, VT_GETSTATE, &vtstat)) < 0)
     {
        printf("[ecore_fb:init] can't get current tty state\n");
        return 0;
     }
   _ecore_fb_vt_prev_vt = vtstat.v_active;
   /* switch to another tty */
   if (_ecore_fb_vt_do_switch)
     {
        int vtno;

        if ((ioctl(_ecore_fb_vt_tty0_fd, VT_OPENQRY, &vtno) < 0))
          {
             printf("[ecore_fb:init] can't query for a vt\n");
             return 0;
          }
        _ecore_fb_vt_current_vt = vtno;
     }
   /* use current tty */
   else
      _ecore_fb_vt_current_vt = _ecore_fb_vt_prev_vt;
   if (!_ecore_fb_vt_setup())
     {
        printf("[ecore_fb:init] can't setup the vt, restoring previous mode...\n");
        /* TODO finish this */
        if (_ecore_fb_vt_do_switch)
          {
             printf("[ecore_fb:init] switching back to vt %d\n", _ecore_fb_vt_prev_vt);
          }
        return 0;
     }
   return 1;
}

void
ecore_fb_vt_shutdown(void)
{
   /* restore the previous mode */
   if (_ecore_fb_vt_tty_fd != -1)
     {
        tcsetattr(_ecore_fb_vt_tty_fd, TCSAFLUSH, &_ecore_fb_tty_prev_tio_mode);
        ioctl(_ecore_fb_vt_tty_fd, KDSETMODE, _ecore_fb_tty_prev_kd_mode);
        ioctl(_ecore_fb_vt_tty_fd, KDSKBMODE, _ecore_fb_tty_prev_mode);
        ioctl(_ecore_fb_vt_tty_fd, VT_SETMODE, &_ecore_fb_vt_prev_mode);
        /* go back to previous vt */
        close(_ecore_fb_vt_tty_fd);
        _ecore_fb_vt_tty_fd = -1;
     }

   if (_ecore_fb_user_handler) ecore_event_handler_del(_ecore_fb_user_handler);
   _ecore_fb_user_handler = NULL;

   if (_ecore_fb_filter_handler) ecore_event_filter_del(_ecore_fb_filter_handler);
   _ecore_fb_filter_handler = NULL;
}

/**
 * @addtogroup Ecore_FB_Group Ecore_FB - Frame buffer convenience functions.
 *
 * @{
 */

/**
 * @brief Set a callback called when a virtual terminal is gained.
 *
 * @param func The callback called when vt is gained.
 * @param data The data to pass to the callback.
 *
 * This function sets the callback @p func which will be called when a
 * virtual terminal is gained (for example you press Ctrl-Alt-F1 to go
 * to vt1 and your app was using vt1). @p data will be pass to @p func if
 * the callback is called.
 */
ECORE_FB_API void
ecore_fb_callback_gain_set(void (*func) (void *data), void *data)
{
   _ecore_fb_func_fb_gain = func;
   _ecore_fb_func_fb_gain_data = data;
}

/**
 * @brief Set a callback called when a virtual terminal is lost.
 *
 * @param func The callback called when vt is lost.
 * @param data The data to pass to the callback.
 *
 * This function sets the callback @p func which will be called when a
 * virtual terminal is lost (someone wants the vt from you and you
 * want to give up that vt). @p data will be pass to @p func if the
 * callback is called.
 */
ECORE_FB_API void
ecore_fb_callback_lose_set(void (*func) (void *data), void *data)
{
   _ecore_fb_func_fb_lost = func;
   _ecore_fb_func_fb_lost_data = data;
}

/**
 * @}
 */

/*
 * This filter should take into account that the MOUSE_MOVE event can be
 * triggered by a mouse, not just a touchscreen device, so you can't discard
 * them (only those generated by a device that sends events with absolute
 * coordinates).

typedef struct _Ecore_Fb_Filter_Data Ecore_Fb_Filter_Data;

struct _Ecore_Fb_Filter_Data
{
   int last_event_type;
};

static void *
_ecore_fb_event_filter_start(void *data EINA_UNUSED)
{
   Ecore_Fb_Filter_Data *filter_data;

   filter_data = calloc(1, sizeof(Ecore_Fb_Filter_Data));
   return filter_data;
}

static Eina_Bool
_ecore_fb_event_filter_filter(void *data EINA_UNUSED, void *loop_data,int type, void *event EINA_UNUSED)
{
   Ecore_Fb_Filter_Data *filter_data;

   filter_data = loop_data;
   if (!filter_data) return EINA_TRUE;
   if (type == ECORE_EVENT_MOUSE_MOVE)
     {
        if ((filter_data->last_event_type) == ECORE_EVENT_MOUSE_MOVE)
          {
             filter_data->last_event_type = type;
             return EINA_FALSE;
          }
     }
   filter_data->last_event_type = type;
   return EINA_TRUE;
}

static void
_ecore_fb_event_filter_end(void *data EINA_UNUSED, void *loop_data)
{
   Ecore_Fb_Filter_Data *filter_data;

   filter_data = loop_data;
   if (filter_data) free(filter_data);
}
*/
