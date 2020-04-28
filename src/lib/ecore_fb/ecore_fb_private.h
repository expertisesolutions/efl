#ifndef _ECORE_FB_PRIVATE_H
#define _ECORE_FB_PRIVATE_H

#include "Ecore.h"
#include "ecore_private.h"
#include "Ecore_Input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <evil_unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/version.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/fb.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,15)) && (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18))
 #define kernel_ulong_t unsigned long 
 #define BITS_PER_LONG 32
 #include <linux/input.h>
 #undef kernel_ulong_t
 #undef BITS_PER_LONG
#else
 #include <linux/input.h>
#endif

#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#include "Ecore_Fb.h"

#ifdef EAPI
# undef EAPI
#endif

#ifdef __GNUC__
# if __GNUC__ >= 4
#  define EAPI __attribute__ ((visibility("default")))
# else
#  define EAPI
# endif
#else
# define EAPI
#endif

/* ecore_fb_li.c */
struct _Ecore_Fb_Input_Device
{
   int fd;
   Ecore_Fd_Handler *handler;
   int listen;
   struct 
     {
        Ecore_Fb_Input_Device_Cap cap;
        char *name;
        char *dev;
     } info;
   struct
     {
        /* common mouse */
        int x,y;
        int w,h;

        double last;
        double prev;
        double threshold;
        Eina_Bool did_double;
        Eina_Bool did_triple;
        /* absolute axis */
        int min_w, min_h;
        double rel_w, rel_h;
        int event;
        int prev_button;
        int last_button;
     } mouse;
   struct
     {
        int shift;
        int ctrl;
        int alt;
        int lock;
     } keyboard;
   void *window;
};

/* ecore_fb_ts.c */
EAPI int    ecore_fb_ts_init(void);
EAPI void   ecore_fb_ts_shutdown(void);
EAPI void   ecore_fb_ts_events_window_set(void *window);
EAPI void  *ecore_fb_ts_events_window_get(void);
EAPI void   ecore_fb_ts_event_window_set(void *window);

/* ecore_fb_vt.c */
int  ecore_fb_vt_init(void);
void ecore_fb_vt_shutdown(void);

/* hacks to stop people NEEDING #include <linux/h3600_ts.h> */
#ifndef TS_SET_CAL
#define TS_SET_CAL 0x4014660b
#endif
#ifndef TS_GET_CAL
#define TS_GET_CAL 0x8014660a
#endif
  
#undef EAPI
#define EAPI

#endif
