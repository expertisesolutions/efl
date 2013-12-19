#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <winsock2.h>
# undef WIN32_LEAN_AND_MEAN
# ifndef USER_TIMER_MINIMUM
#  define USER_TIMER_MINIMUM 0x0a
# endif
#endif

#ifdef __SUNPRO_C
# include <ieeefp.h>
# include <string.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>

#ifdef HAVE_SYSTEMD
# include <systemd/sd-daemon.h>
#endif

#ifdef _MSC_VER
# include <float.h>
#endif

#ifdef HAVE_ISFINITE
# define ECORE_FINITE(t)  isfinite(t)
#else
# ifdef _MSC_VER
#  define ECORE_FINITE(t) _finite(t)
# else
#  define ECORE_FINITE(t) finite(t)
# endif
#endif

//#define FIX_HZ 1

#ifdef FIX_HZ
# ifndef _MSC_VER
#  include <sys/param.h>
# endif
# ifndef HZ
#  define HZ 100
# endif
#endif

#ifdef HAVE_EVIL
# include <Evil.h>
#endif

#include "Ecore.h"
#include "ecore_private.h"

#ifdef HAVE_SYS_EPOLL_H
# define HAVE_EPOLL   1
# include <sys/epoll.h>
#else

# define HAVE_EPOLL   0
# define EPOLLIN      1
# define EPOLLPRI     2
# define EPOLLOUT     4
# define EPOLLERR     8

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3

typedef union epoll_data {
   void    *ptr;
   int      fd;
   uint32_t u32;
   uint64_t u64;
} epoll_data_t;

struct epoll_event
{
   uint32_t     events;
   epoll_data_t data;
};

static inline int
epoll_create(int size EINA_UNUSED)
{
   return -1;
}

static inline int
epoll_wait(int                 epfd EINA_UNUSED,
           struct epoll_event *events EINA_UNUSED,
           int                 maxevents EINA_UNUSED,
           int                 timeout EINA_UNUSED)
{
   return -1;
}

static inline int
epoll_ctl(int                 epfd EINA_UNUSED,
          int                 op EINA_UNUSED,
          int                 fd EINA_UNUSED,
          struct epoll_event *event EINA_UNUSED)
{
   return -1;
}

#endif

#ifdef HAVE_SYS_TIMERFD_H
# include <sys/timerfd.h>
#else
/* fallback code if we don't have real timerfd - reduces number of ifdefs  */
# ifndef CLOCK_MONOTONIC
#  define CLOCK_MONOTONIC 0 /* bogus value */
# endif
# ifndef TFD_NONBLOCK
#  define TFD_NONBLOCK    0 /* bogus value */
# endif
#endif /* HAVE_SYS_TIMERFD_H */

#ifndef TFD_TIMER_ABSTIME
# define TFD_TIMER_ABSTIME (1 << 0)
#endif
#ifndef TFD_TIMER_CANCELON_SET
# define TFD_TIMER_CANCELON_SET (1 << 1)
#endif

#ifndef HAVE_TIMERFD_CREATE
static inline int
timerfd_create(int clockid EINA_UNUSED,
               int flags EINA_UNUSED)
{
   return -1;
}

static inline int
timerfd_settime(int                      fd EINA_UNUSED,
                int                      flags EINA_UNUSED,
                const struct itimerspec *new_value EINA_UNUSED,
                struct itimerspec       *old_value EINA_UNUSED)
{
   return -1;
}

#endif /* HAVE_TIMERFD_CREATE */

#ifdef USE_G_MAIN_LOOP
# include <glib.h>
#endif

#define NS_PER_SEC (1000.0 * 1000.0 * 1000.0)

struct _Ecore_Fd_Handler
{
   EINA_INLIST;
                          ECORE_MAGIC;
   Ecore_Fd_Handler      *next_ready;
   int                    fd;
   Ecore_Fd_Handler_Flags flags;
   Ecore_Fd_Cb            func;
   void                  *data;
   Ecore_Fd_Cb            buf_func;
   void                  *buf_data;
   Ecore_Fd_Prep_Cb       prep_func;
   void                  *prep_data;
   int                    references;
   Eina_Bool              read_active : 1;
   Eina_Bool              write_active : 1;
   Eina_Bool              error_active : 1;
   Eina_Bool              delete_me : 1;
   Eina_Bool              file : 1;
#if defined(USE_G_MAIN_LOOP)
   GPollFD                gfd;
#endif
};
GENERIC_ALLOC_SIZE_DECLARE(Ecore_Fd_Handler);

#ifdef _WIN32
struct _Ecore_Win32_Handler
{
   EINA_INLIST;
                         ECORE_MAGIC;
   HANDLE                h;
   Ecore_Win32_Handle_Cb func;
   void                 *data;
   int                   references;
   Eina_Bool             delete_me : 1;
};
GENERIC_ALLOC_SIZE_DECLARE(Ecore_Win32_Handler);
#endif

#ifndef USE_G_MAIN_LOOP
static int  _ecore_main_select(double timeout);
#endif
static void _ecore_main_prepare_handlers(void);
static void _ecore_main_fd_handlers_cleanup(void);
#ifndef _WIN32
# ifndef USE_G_MAIN_LOOP
static void _ecore_main_fd_handlers_bads_rem(void);
# endif
#endif
static void _ecore_main_fd_handlers_call(void);
static int  _ecore_main_fd_handlers_buf_call(void);
#ifndef USE_G_MAIN_LOOP
static void _ecore_main_loop_iterate_internal(int once_only);
#endif

#ifdef _WIN32
static int _ecore_main_win32_select(int             nfds,
                                    fd_set         *readfds,
                                    fd_set         *writefds,
                                    fd_set         *exceptfds,
                                    struct timeval *timeout);
static void _ecore_main_win32_handlers_cleanup(void);
#endif

static int in_main_loop = 0;
static int do_quit = 0;
static Ecore_Fd_Handler *fd_handlers = NULL;
static Ecore_Fd_Handler *fd_handler_current = NULL;
static Eina_List *fd_handlers_with_prep = NULL;
static Eina_List *file_fd_handlers = NULL;
static Eina_List *fd_handlers_with_buffer = NULL;
static Eina_List *fd_handlers_to_delete = NULL;

/* single linked list of ready fdhs, terminated by loop to self */
static Ecore_Fd_Handler *fd_handlers_to_call;
static Ecore_Fd_Handler *fd_handlers_to_call_current;

#ifdef _WIN32
static Ecore_Win32_Handler *win32_handlers = NULL;
static Ecore_Win32_Handler *win32_handler_current = NULL;
static Eina_Bool win32_handlers_delete_me = EINA_FALSE;
#endif

#ifdef _WIN32
Ecore_Select_Function main_loop_select = _ecore_main_win32_select;
#else
# if !defined EXOTIC_NO_SELECT
#   include <sys/select.h>
Ecore_Select_Function main_loop_select = select;
# else
Ecore_Select_Function main_loop_select = NULL;
# endif
#endif

#ifndef USE_G_MAIN_LOOP
static double t1 = 0.0;
static double t2 = 0.0;
#endif

static int timer_fd = -1;
static int epoll_fd = -1;
static pid_t epoll_pid;

#ifdef USE_G_MAIN_LOOP
static GPollFD ecore_epoll_fd;
static GPollFD ecore_timer_fd;
static GSource *ecore_glib_source;
static guint ecore_glib_source_id;
static GMainLoop *ecore_main_loop;
static gboolean ecore_idling;
static gboolean _ecore_glib_idle_enterer_called;
static gboolean ecore_fds_ready;
#endif

Eina_Bool
_ecore_fd_close_on_exec(int fd)
{
#ifdef HAVE_EXECVP
   int flags;

   flags = fcntl(fd, F_GETFD);
   if (flags == -1)
     return EINA_FALSE;

   flags |= FD_CLOEXEC;
   if (fcntl(fd, F_SETFD, flags) == -1)
     return EINA_FALSE;
   return EINA_TRUE;
#else
   (void) fd;
   return EINA_FALSE;
#endif
}

static inline void
_ecore_fd_valid(void)
{
   if (HAVE_EPOLL && epoll_fd >= 0)
     {
        if (fcntl(epoll_fd, F_GETFD) < 0)
          {
             ERR("arghhh you caught me! report a backtrace to edevel!");
             pause();
          }
     }
}

static inline void
_ecore_try_add_to_call_list(Ecore_Fd_Handler *fdh)
{
   /* check if this fdh is already in the list */
   if (fdh->next_ready)
     return;
   if (fdh->read_active || fdh->write_active || fdh->error_active)
     {
        /*
         * make sure next_ready is non-null by pointing to ourselves
         * use that to indicate this fdh is in the ready list
         * insert at the head of the list to avoid trouble
         */
        fdh->next_ready = fd_handlers_to_call ? fd_handlers_to_call : fdh;
        fd_handlers_to_call = fdh;
     }
}

static inline int
_ecore_get_epoll_fd(void)
{
   if (epoll_pid && epoll_pid != getpid())
     {
        /* forked! */
        _ecore_main_loop_shutdown();
     }
   if (epoll_pid == 0 && epoll_fd < 0)
     {
        _ecore_main_loop_init();
     }
   return epoll_fd;
}

static inline int
_ecore_epoll_add(int   efd,
                 int   fd,
                 int   events,
                 void *ptr)
{
   struct epoll_event ev;

   memset(&ev, 0, sizeof (ev));
   ev.events = events;
   ev.data.ptr = ptr;
   DBG("adding poll on %d %08x", fd, events);
   return epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
}

static inline int
_ecore_poll_events_from_fdh(Ecore_Fd_Handler *fdh)
{
   int events = 0;
   if (fdh->flags & ECORE_FD_READ) events |= EPOLLIN;
   if (fdh->flags & ECORE_FD_WRITE) events |= EPOLLOUT;
   if (fdh->flags & ECORE_FD_ERROR) events |= EPOLLERR | EPOLLPRI;
   return events;
}

#ifdef USE_G_MAIN_LOOP
static inline int
_gfd_events_from_fdh(Ecore_Fd_Handler *fdh)
{
   int events = 0;
   if (fdh->flags & ECORE_FD_READ) events |= G_IO_IN;
   if (fdh->flags & ECORE_FD_WRITE) events |= G_IO_OUT;
   if (fdh->flags & ECORE_FD_ERROR) events |= G_IO_ERR;
   return events;
}

#endif

static inline int
_ecore_main_fdh_poll_add(Ecore_Fd_Handler *fdh)
{
   int r = 0;

   if ((!fdh->file) && HAVE_EPOLL && epoll_fd >= 0)
     {
        r = _ecore_epoll_add(_ecore_get_epoll_fd(), fdh->fd,
                             _ecore_poll_events_from_fdh(fdh), fdh);
     }
   else
     {
#ifdef USE_G_MAIN_LOOP
        fdh->gfd.fd = fdh->fd;
        fdh->gfd.events = _gfd_events_from_fdh(fdh);
        fdh->gfd.revents = 0;
        DBG("adding gpoll on %d %08x", fdh->fd, fdh->gfd.events);
        g_source_add_poll(ecore_glib_source, &fdh->gfd);
#endif
     }
   return r;
}

static inline void
_ecore_main_fdh_poll_del(Ecore_Fd_Handler *fdh)
{
   if ((!fdh->file) && HAVE_EPOLL && epoll_fd >= 0)
     {
        struct epoll_event ev;
        int efd = _ecore_get_epoll_fd();

        memset(&ev, 0, sizeof (ev));
        DBG("removing poll on %d", fdh->fd);
        /* could get an EBADF if somebody closed the FD before removing it */
        if ((epoll_ctl(efd, EPOLL_CTL_DEL, fdh->fd, &ev) < 0))
          {
             if (errno == EBADF)
               {
                  WRN("fd %d was closed, can't remove from epoll - reinit!",
                      fdh->fd);
                  _ecore_main_loop_shutdown();
                  _ecore_main_loop_init();
               }
             else
               {
                  ERR("Failed to delete epoll fd %d! (errno=%d)", fdh->fd, errno);
               }
          }
     }
   else
     {
#ifdef USE_G_MAIN_LOOP
        fdh->gfd.fd = fdh->fd;
        fdh->gfd.events = _gfd_events_from_fdh(fdh);
        fdh->gfd.revents = 0;
        DBG("removing gpoll on %d %08x", fdh->fd, fdh->gfd.events);
        g_source_remove_poll(ecore_glib_source, &fdh->gfd);
#endif
     }
}

static inline int
_ecore_main_fdh_poll_modify(Ecore_Fd_Handler *fdh)
{
   int r = 0;
   if ((!fdh->file) && HAVE_EPOLL && epoll_fd >= 0)
     {
        struct epoll_event ev;
        int efd = _ecore_get_epoll_fd();

        memset(&ev, 0, sizeof (ev));
        ev.events = _ecore_poll_events_from_fdh(fdh);
        ev.data.ptr = fdh;
        DBG("modifing epoll on %d to %08x", fdh->fd, ev.events);
        r = epoll_ctl(efd, EPOLL_CTL_MOD, fdh->fd, &ev);
     }
   else
     {
#ifdef USE_G_MAIN_LOOP
        fdh->gfd.fd = fdh->fd;
        fdh->gfd.events = _gfd_events_from_fdh(fdh);
        fdh->gfd.revents = 0;
        DBG("modifing gpoll on %d to %08x", fdh->fd, fdh->gfd.events);
#endif
     }
   return r;
}

static inline int
_ecore_main_fdh_epoll_mark_active(void)
{
   struct epoll_event ev[32];
   int i, ret;
   int efd = _ecore_get_epoll_fd();

   memset(&ev, 0, sizeof (ev));
   ret = epoll_wait(efd, ev, sizeof(ev) / sizeof(struct epoll_event), 0);
   if (ret < 0)
     {
        if (errno == EINTR) return -1;
        ERR("epoll_wait failed %d", errno);
        return -1;
     }

   for (i = 0; i < ret; i++)
     {
        Ecore_Fd_Handler *fdh;

        fdh = ev[i].data.ptr;
        if (!ECORE_MAGIC_CHECK(fdh, ECORE_MAGIC_FD_HANDLER))
          {
             ECORE_MAGIC_FAIL(fdh, ECORE_MAGIC_FD_HANDLER,
                              "_ecore_main_fdh_epoll_mark_active");
             continue;
          }
        if (fdh->delete_me)
          {
             ERR("deleted fd in epoll");
             continue;
          }

        if (ev[i].events & EPOLLIN)
          fdh->read_active = EINA_TRUE;
        if (ev[i].events & EPOLLOUT)
          fdh->write_active = EINA_TRUE;
        if (ev[i].events & EPOLLERR)
          fdh->error_active = EINA_TRUE;

        _ecore_try_add_to_call_list(fdh);
     }

   return ret;
}

#ifdef USE_G_MAIN_LOOP

static inline int
_ecore_main_fdh_glib_mark_active(void)
{
   Ecore_Fd_Handler *fdh;
   int ret = 0;

   /* call the prepare callback for all handlers */
   EINA_INLIST_FOREACH(fd_handlers, fdh)
     {
        if (fdh->delete_me)
          continue;

        if (fdh->gfd.revents & G_IO_IN)
          fdh->read_active = EINA_TRUE;
        if (fdh->gfd.revents & G_IO_OUT)
          fdh->write_active = EINA_TRUE;
        if (fdh->gfd.revents & G_IO_ERR)
          fdh->error_active = EINA_TRUE;

        _ecore_try_add_to_call_list(fdh);

        if (fdh->gfd.revents & (G_IO_IN | G_IO_OUT | G_IO_ERR)) ret++;
     }

   return ret;
}

/* like we are about to enter main_loop_select in  _ecore_main_select */
static gboolean
_ecore_main_gsource_prepare(GSource *source EINA_UNUSED,
                            gint    *next_time)
{
   gboolean ready = FALSE;

   _ecore_lock();
   in_main_loop++;

   if (!ecore_idling && !_ecore_glib_idle_enterer_called)
     {
        _ecore_time_loop_time = ecore_time_get();
        _ecore_timer_expired_timers_call(_ecore_time_loop_time);
        _ecore_timer_cleanup();

        _ecore_idle_enterer_call();
        _ecore_throttle();
        _ecore_glib_idle_enterer_called = FALSE;

        if (fd_handlers_with_buffer)
          _ecore_main_fd_handlers_buf_call();
     }

   _ecore_signal_received_process();

   /* don't check fds if somebody quit */
   if (g_main_loop_is_running(ecore_main_loop))
     {
        /* only set idling state in dispatch */
         if (ecore_idling && !_ecore_idler_exist() && !_ecore_event_exist())
           {
              if (_ecore_timers_exists())
                {
                   int r = -1;
                   double t = _ecore_timer_next_get();
                   if (timer_fd >= 0 && t > 0.0)
                     {
                        struct itimerspec ts;

                        ts.it_interval.tv_sec = 0;
                        ts.it_interval.tv_nsec = 0;
                        ts.it_value.tv_sec = t;
                        ts.it_value.tv_nsec = fmod(t * NS_PER_SEC, NS_PER_SEC);

     /* timerfd cannot sleep for 0 time */
                        if (ts.it_value.tv_sec || ts.it_value.tv_nsec)
                          {
                             r = timerfd_settime(timer_fd, 0, &ts, NULL);
                             if (r < 0)
                               {
                                  ERR("timer set returned %d (errno=%d)", r, errno);
                                  close(timer_fd);
                                  timer_fd = -1;
                               }
                             else
                               INF("sleeping for %ld s %06ldus",
                                   ts.it_value.tv_sec,
                                   ts.it_value.tv_nsec / 1000);
                          }
                     }
                   if (r == -1)
                     {
                        *next_time = ceil(t * 1000.0);
                        if (t == 0.0)
                          ready = TRUE;
                     }
                }
              else
                *next_time = -1;
           }
         else
           {
              *next_time = 0;
              if (_ecore_event_exist())
                ready = TRUE;
           }

         if (fd_handlers_with_prep)
           _ecore_main_prepare_handlers();
     }
   else
     ready = TRUE;

   in_main_loop--;
   INF("leave, timeout = %d", *next_time);
   _ecore_unlock();

   /* ready if we're not running (about to quit) */
   return ready;
}

static gboolean
_ecore_main_gsource_check(GSource *source EINA_UNUSED)
{
   gboolean ret = FALSE;

   _ecore_lock();
   in_main_loop++;

   /* check if old timers expired */
   if (ecore_idling && !_ecore_idler_exist() && !_ecore_event_exist())
     {
        if (timer_fd >= 0)
          {
             uint64_t count = 0;
             int r = read(timer_fd, &count, sizeof count);
             if (r == -1 && errno == EAGAIN)
               ;
             else if (r == sizeof count)
               ret = TRUE;
             else
               {
     /* unexpected things happened... fail back to old way */
                   ERR("timer read returned %d (errno=%d)", r, errno);
                   close(timer_fd);
                   timer_fd = -1;
               }
          }
     }
   else
     ret = TRUE;

   /* check if fds are ready */
   if (HAVE_EPOLL && epoll_fd >= 0)
     ecore_fds_ready = (_ecore_main_fdh_epoll_mark_active() > 0);
   else
     ecore_fds_ready = (_ecore_main_fdh_glib_mark_active() > 0);
   _ecore_main_fd_handlers_cleanup();
   if (ecore_fds_ready)
     ret = TRUE;

   /* check timers after updating loop time */
   if (!ret && _ecore_timers_exists())
     ret = (0.0 == _ecore_timer_next_get());

   in_main_loop--;
   _ecore_unlock();

   return ret;
}

/* like we just came out of main_loop_select in  _ecore_main_select */
static gboolean
_ecore_main_gsource_dispatch(GSource    *source EINA_UNUSED,
                             GSourceFunc callback EINA_UNUSED,
                             gpointer    user_data EINA_UNUSED)
{
   gboolean events_ready, timers_ready, idlers_ready;
   double next_time;

   _ecore_lock();
   _ecore_time_loop_time = ecore_time_get();
   _ecore_timer_enable_new();
   next_time = _ecore_timer_next_get();

   events_ready = _ecore_event_exist();
   timers_ready = _ecore_timers_exists() && (0.0 == next_time);
   idlers_ready = _ecore_idler_exist();

   in_main_loop++;
   INF("enter idling=%d fds=%d events=%d timers=%d (next=%.2f) idlers=%d",
       ecore_idling, ecore_fds_ready, events_ready,
       timers_ready, next_time, idlers_ready);

   if (ecore_idling && events_ready)
     {
        _ecore_animator_run_reset();
        _ecore_idle_exiter_call();
        ecore_idling = 0;
     }
   else if (!ecore_idling && !events_ready)
     {
        ecore_idling = 1;
     }

   if (ecore_idling)
     {
        _ecore_idler_all_call();

        events_ready = _ecore_event_exist();

        if (ecore_fds_ready || events_ready || timers_ready)
          {
             _ecore_animator_run_reset();
             _ecore_idle_exiter_call();
             ecore_idling = 0;
          }
     }

   /* process events */
   if (!ecore_idling)
     {
        _ecore_main_fd_handlers_call();
        if (fd_handlers_with_buffer)
          _ecore_main_fd_handlers_buf_call();
        _ecore_signal_received_process();
        _ecore_event_call();
        _ecore_main_fd_handlers_cleanup();

        _ecore_timer_expired_timers_call(_ecore_time_loop_time);
        _ecore_timer_cleanup();

        _ecore_idle_enterer_call();
        _ecore_throttle();
        _ecore_glib_idle_enterer_called = TRUE;

        if (fd_handlers_with_buffer)
          _ecore_main_fd_handlers_buf_call();
     }

   in_main_loop--;
   _ecore_unlock();

   return TRUE; /* what should be returned here? */
}

static void
_ecore_main_gsource_finalize(GSource *source EINA_UNUSED)
{
}

static GSourceFuncs ecore_gsource_funcs =
{
   .prepare = _ecore_main_gsource_prepare,
   .check = _ecore_main_gsource_check,
   .dispatch = _ecore_main_gsource_dispatch,
   .finalize = _ecore_main_gsource_finalize,
};

#endif

#ifdef HAVE_SYS_TIMERFD_H
static int realtime_fd = -1;

static void detect_time_changes_start(void);
static Eina_Bool
_realtime_update(void *data EINA_UNUSED, Ecore_Fd_Handler *fdh EINA_UNUSED)
{
   char buf[64];

   if (read(realtime_fd, buf, sizeof(buf)) >= 0) return EINA_TRUE;

   DBG("system clock changed");
   ecore_event_add(ECORE_EVENT_SYSTEM_TIMEDATE_CHANGED, NULL, NULL, NULL);

   close(realtime_fd);
   realtime_fd = -1;
   detect_time_changes_start();
   return EINA_FALSE;
}
#endif

static void
detect_time_changes_start(void)
{
#ifdef HAVE_SYS_TIMERFD_H
   struct itimerspec its;

   if (realtime_fd >= 0) return;

   realtime_fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC);
   if (realtime_fd < 0) return;

   memset(&its, 0, sizeof(its));
   its.it_value.tv_sec += 0xfffffff0; // end of time - 0xf
   if (timerfd_settime(realtime_fd,
                       TFD_TIMER_ABSTIME | TFD_TIMER_CANCELON_SET,
                       &its, NULL) < 0)
     {
        WRN("Couldn't arm timerfd to detect clock changes: %s",
            strerror(errno));
        close(realtime_fd);
        realtime_fd = -1;
        return;
     }

   ecore_main_fd_handler_add(realtime_fd, ECORE_FD_READ,
                             _realtime_update, NULL, NULL, NULL);
#endif
}

static void
detect_time_changes_stop(void)
{
#ifdef HAVE_SYS_TIMERFD_H
   if (realtime_fd > 0)
     {
        close(realtime_fd);
        realtime_fd = -1;
     }
#endif
}

void
_ecore_main_loop_init(void)
{
   epoll_fd = epoll_create(1);
   if (epoll_fd < 0)
     WRN("Failed to create epoll fd!");
   epoll_pid = getpid();
   _ecore_fd_close_on_exec(epoll_fd);

   /* add polls on all our file descriptors */
   Ecore_Fd_Handler *fdh;
   EINA_INLIST_FOREACH(fd_handlers, fdh)
     {
        if (fdh->delete_me)
          continue;
        _ecore_epoll_add(epoll_fd, fdh->fd,
                         _ecore_poll_events_from_fdh(fdh), fdh);
        _ecore_main_fdh_poll_add(fdh);
     }

   /* setup for the g_main_loop only integration */
#ifdef USE_G_MAIN_LOOP
   ecore_glib_source = g_source_new(&ecore_gsource_funcs, sizeof (GSource));
   if (!ecore_glib_source)
     CRIT("Failed to create glib source for epoll!");
   else
     {
        g_source_set_priority(ecore_glib_source, G_PRIORITY_HIGH_IDLE + 20);
        if (HAVE_EPOLL && epoll_fd >= 0)
          {
             /* epoll multiplexes fds into the g_main_loop */
              ecore_epoll_fd.fd = epoll_fd;
              ecore_epoll_fd.events = G_IO_IN;
              ecore_epoll_fd.revents = 0;
              g_source_add_poll(ecore_glib_source, &ecore_epoll_fd);
          }

        /* timerfd gives us better than millisecond accuracy in g_main_loop */
        timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
        if (timer_fd < 0)
          WRN("failed to create timer fd!");
        else
          {
             _ecore_fd_close_on_exec(timer_fd);
             ecore_timer_fd.fd = timer_fd;
             ecore_timer_fd.events = G_IO_IN;
             ecore_timer_fd.revents = 0;
             g_source_add_poll(ecore_glib_source, &ecore_timer_fd);
          }

        ecore_glib_source_id = g_source_attach(ecore_glib_source, NULL);
        if (ecore_glib_source_id <= 0)
          CRIT("Failed to attach glib source to default context");
     }
#endif

   detect_time_changes_start();
}

void
_ecore_main_loop_shutdown(void)
{
#ifdef USE_G_MAIN_LOOP
   if (ecore_glib_source)
     {
        g_source_destroy(ecore_glib_source);
        ecore_glib_source = NULL;
     }
#endif

   detect_time_changes_stop();

   if (epoll_fd >= 0)
     {
        close(epoll_fd);
        epoll_fd = -1;
     }
   epoll_pid = 0;

   if (timer_fd >= 0)
     {
        close(timer_fd);
        timer_fd = -1;
     }
}

void *
_ecore_main_fd_handler_del(Ecore_Fd_Handler *fd_handler)
{
   if (fd_handler->delete_me)
     {
        ERR("fdh %p deleted twice", fd_handler);
        return NULL;
     }

   _ecore_main_fdh_poll_del(fd_handler);
   fd_handler->delete_me = EINA_TRUE;
   fd_handlers_to_delete = eina_list_append(fd_handlers_to_delete, fd_handler);
   if (fd_handler->prep_func && fd_handlers_with_prep)
     fd_handlers_with_prep = eina_list_remove(fd_handlers_with_prep, fd_handler);
   if (fd_handler->buf_func && fd_handlers_with_buffer)
     fd_handlers_with_buffer = eina_list_remove(fd_handlers_with_buffer, fd_handler);
   return fd_handler->data;
}

/**
 * @addtogroup Ecore_Main_Loop_Group
 *
 * @{
 */

/**
 * Runs a single iteration of the main loop to process everything on the
 * queue.
 *
 * It does everything that is already done inside an @c Ecore main loop, like
 * checking for expired timers, idlers, etc. But it will do it only once and
 * return, instead of keep watching for new events.
 *
 * DO NOT use this function unless you are the person God comes to ask for
 * advice when He has trouble managing the Universe.
 *
 * @see ecore_main_loop_iterate_may_block()
 */
EAPI void
ecore_main_loop_iterate(void)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
#ifndef USE_G_MAIN_LOOP
   _ecore_lock();
   _ecore_time_loop_time = ecore_time_get();
   _ecore_main_loop_iterate_internal(1);
   _ecore_unlock();
#else
   g_main_context_iteration(NULL, 0);
#endif
}

/**
 * Runs a single iteration of the main loop to process everything on the
 * queue with block/non-blocking status.
 *
 * @param may_block A flag if the main loop has a possibility of blocking.
 * (@c EINA_TRUE = may block/@c EINA_FALSE = non block)
 *
 * This is an extension API for ecore_main_loop_iterate() with additional
 * parameter. It does everything that is already done inside an
 * @c Ecore main loop, like checking for expired timers, idlers, etc. But it
 * will do it only once and return, instead of keep watching for new events.
 *
 * DO NOT use this function unless you are the person God comes to ask for
 * advice when He has trouble managing the Universe.
 *
 * @see ecore_main_loop_iterate()
 */
EAPI int
ecore_main_loop_iterate_may_block(int may_block)
{
   EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
#ifndef USE_G_MAIN_LOOP
   _ecore_lock();
   _ecore_time_loop_time = ecore_time_get();
in_main_loop++;
   _ecore_main_loop_iterate_internal(!may_block);
in_main_loop--;
   _ecore_unlock();
   return _ecore_event_exist();
#else
   return g_main_context_iteration(NULL, may_block);
#endif
}

/**
 * Runs the application main loop.
 *
 * This function will not return until @ref ecore_main_loop_quit is called. It
 * will check for expired timers, idlers, file descriptors being watched by fd
 * handlers, etc. Once everything is done, before entering again on idle state,
 * any callback set as @c Idle_Enterer will be called.
 *
 * Each main loop iteration is done by calling ecore_main_loop_iterate()
 * internally.
 *
 * The polling (select) function used can be changed with
 * ecore_main_loop_select_func_set().
 *
 * The function used to check for file descriptors, events, and that has a
 * timeout for the timers can be changed using
 * ecore_main_loop_select_func_set().
 */
EAPI void
ecore_main_loop_begin(void)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
#ifdef HAVE_SYSTEMD
   sd_notify(0, "READY=1");
#endif
#ifndef USE_G_MAIN_LOOP
   _ecore_lock();
   in_main_loop++;
   _ecore_time_loop_time = ecore_time_get();
   while (do_quit == 0) _ecore_main_loop_iterate_internal(0);
   do_quit = 0;
   in_main_loop--;
   _ecore_unlock();
#else
   if (!do_quit)
     {
        if (!ecore_main_loop)
          ecore_main_loop = g_main_loop_new(NULL, FALSE);
        g_main_loop_run(ecore_main_loop);
     }
   do_quit = 0;
#endif
}

/**
 * Quits the main loop once all the events currently on the queue have
 * been processed.
 *
 * This function returns immediately, but will mark the ecore_main_loop_begin()
 * function to return at the end of the current main loop iteration.
 */
EAPI void
ecore_main_loop_quit(void)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
   do_quit = 1;
#ifdef USE_G_MAIN_LOOP
   if (ecore_main_loop)
     g_main_loop_quit(ecore_main_loop);
#endif
}

/**
 * Returns if an animator has ticked off during this loop iteration
 *
 * @return EINA_TRUE if an animator has been called, EINA_FALSE otherwise.
 *
 * There should be little need for anyone to use this - ever.
 *
 * @since 1.9
 */
EAPI Eina_Bool
ecore_main_loop_animator_ticked_get(void)
{
   return _ecore_animator_run_get();
}

/**
 * Sets the function to use when monitoring multiple file descriptors,
 * and waiting until one of more of the file descriptors before ready
 * for some class of I/O operation.
 *
 * This function will be used instead of the system call select and
 * could possible be used to integrate the Ecore event loop with an
 * external event loop.
 *
 * @warning you don't know how to use, don't even try to use it.
 *
 * @param func The function to be used.
 */
EAPI void
ecore_main_loop_select_func_set(Ecore_Select_Function func)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
   main_loop_select = func;
}

/**
 * Gets the select function set by ecore_select_func_set(),
 * or the native select function if none was set.
 *
 */
EAPI Ecore_Select_Function
ecore_main_loop_select_func_get(void)
{
   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   return main_loop_select;
}

Ecore_Fd_Handler *
_ecore_main_fd_handler_add(int                    fd,
                           Ecore_Fd_Handler_Flags flags,
                           Ecore_Fd_Cb            func,
                           const void            *data,
                           Ecore_Fd_Cb            buf_func,
                           const void            *buf_data)
{
   Ecore_Fd_Handler *fdh = NULL;

   if ((fd < 0) || (flags == 0) || (!func)) return NULL;

   fdh = ecore_fd_handler_calloc(1);
   if (!fdh) return NULL;
   ECORE_MAGIC_SET(fdh, ECORE_MAGIC_FD_HANDLER);
   fdh->next_ready = NULL;
   fdh->fd = fd;
   fdh->flags = flags;
   if (_ecore_main_fdh_poll_add(fdh) < 0)
     {
        int err = errno;
        ERR("Failed to add poll on fd %d (errno = %d: %s)!", fd, err, strerror(err));
        ecore_fd_handler_mp_free(fdh);
        return NULL;
     }
   fdh->read_active = EINA_FALSE;
   fdh->write_active = EINA_FALSE;
   fdh->error_active = EINA_FALSE;
   fdh->delete_me = EINA_FALSE;
   fdh->func = func;
   fdh->data = (void *)data;
   fdh->buf_func = buf_func;
   if (buf_func)
     fd_handlers_with_buffer = eina_list_append(fd_handlers_with_buffer, fdh);
   fdh->buf_data = (void *)buf_data;
   fd_handlers = (Ecore_Fd_Handler *)
     eina_inlist_append(EINA_INLIST_GET(fd_handlers),
                        EINA_INLIST_GET(fdh));

   return fdh;
}

EAPI Ecore_Fd_Handler *
ecore_main_fd_handler_add(int                    fd,
                          Ecore_Fd_Handler_Flags flags,
                          Ecore_Fd_Cb            func,
                          const void            *data,
                          Ecore_Fd_Cb            buf_func,
                          const void            *buf_data)
{
   Ecore_Fd_Handler *fdh = NULL;
   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   _ecore_lock();
   fdh = _ecore_main_fd_handler_add(fd, flags, func, data, buf_func, buf_data);
   _ecore_unlock();
   return fdh;
}

EAPI Ecore_Fd_Handler *
ecore_main_fd_handler_file_add(int                    fd,
                               Ecore_Fd_Handler_Flags flags,
                               Ecore_Fd_Cb            func,
                               const void            *data,
                               Ecore_Fd_Cb            buf_func,
                               const void            *buf_data)
{
   Ecore_Fd_Handler *fdh = NULL;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   _ecore_lock();

   if ((fd < 0) || (flags == 0) || (!func)) goto unlock;

   fdh = ecore_fd_handler_calloc(1);
   if (!fdh) goto unlock;
   ECORE_MAGIC_SET(fdh, ECORE_MAGIC_FD_HANDLER);
   fdh->next_ready = NULL;
   fdh->fd = fd;
   fdh->flags = flags;
   fdh->file = EINA_TRUE;
   if (_ecore_main_fdh_poll_add(fdh) < 0)
     {
        int err = errno;
        ERR("Failed to add poll on fd %d (errno = %d: %s)!", fd, err, strerror(err));
        ecore_fd_handler_mp_free(fdh);
        fdh = NULL;
        goto unlock;
     }
   fdh->read_active = EINA_FALSE;
   fdh->write_active = EINA_FALSE;
   fdh->error_active = EINA_FALSE;
   fdh->delete_me = EINA_FALSE;
   fdh->func = func;
   fdh->data = (void *)data;
   fdh->buf_func = buf_func;
   if (buf_func)
     fd_handlers_with_buffer = eina_list_append(fd_handlers_with_buffer, fdh);
   fdh->buf_data = (void *)buf_data;
   fd_handlers = (Ecore_Fd_Handler *)
     eina_inlist_append(EINA_INLIST_GET(fd_handlers),
                        EINA_INLIST_GET(fdh));
   file_fd_handlers = eina_list_append(file_fd_handlers, fdh);
unlock:
   _ecore_unlock();

   return fdh;
}

#ifdef _WIN32
EAPI Ecore_Win32_Handler *
ecore_main_win32_handler_add(void                 *h,
                             Ecore_Win32_Handle_Cb func,
                             const void           *data)
{
   Ecore_Win32_Handler *wh;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   if (!h || !func) return NULL;

   wh = ecore_win32_handler_calloc(1);
   if (!wh) return NULL;
   ECORE_MAGIC_SET(wh, ECORE_MAGIC_WIN32_HANDLER);
   wh->h = (HANDLE)h;
   wh->delete_me = EINA_FALSE;
   wh->func = func;
   wh->data = (void *)data;
   win32_handlers = (Ecore_Win32_Handler *)
     eina_inlist_append(EINA_INLIST_GET(win32_handlers),
                        EINA_INLIST_GET(wh));
   return wh;
}

#else
EAPI Ecore_Win32_Handler *
ecore_main_win32_handler_add(void                 *h EINA_UNUSED,
                             Ecore_Win32_Handle_Cb func EINA_UNUSED,
                             const void           *data EINA_UNUSED)
{
   return NULL;
}

#endif

EAPI void *
ecore_main_fd_handler_del(Ecore_Fd_Handler *fd_handler)
{
   void *ret = NULL;

   if (!fd_handler) return NULL;
   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(fd_handler, ECORE_MAGIC_FD_HANDLER))
     {
        ECORE_MAGIC_FAIL(fd_handler, ECORE_MAGIC_FD_HANDLER,
                         "ecore_main_fd_handler_del");
        goto unlock;
     }
   ret = _ecore_main_fd_handler_del(fd_handler);
unlock:
   _ecore_unlock();
   return ret;
}

#ifdef _WIN32
EAPI void *
ecore_main_win32_handler_del(Ecore_Win32_Handler *win32_handler)
{
   if (!win32_handler) return NULL;
   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   if (!ECORE_MAGIC_CHECK(win32_handler, ECORE_MAGIC_WIN32_HANDLER))
     {
        ECORE_MAGIC_FAIL(win32_handler, ECORE_MAGIC_WIN32_HANDLER,
                         "ecore_main_win32_handler_del");
        return NULL;
     }
   win32_handler->delete_me = EINA_TRUE;
   win32_handlers_delete_me = EINA_TRUE;
   return win32_handler->data;
}

#else
EAPI void *
ecore_main_win32_handler_del(Ecore_Win32_Handler *win32_handler EINA_UNUSED)
{
   return NULL;
}

#endif

EAPI void
ecore_main_fd_handler_prepare_callback_set(Ecore_Fd_Handler *fd_handler,
                                           Ecore_Fd_Prep_Cb  func,
                                           const void       *data)
{
   EINA_MAIN_LOOP_CHECK_RETURN;
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(fd_handler, ECORE_MAGIC_FD_HANDLER))
     {
        ECORE_MAGIC_FAIL(fd_handler, ECORE_MAGIC_FD_HANDLER,
                         "ecore_main_fd_handler_prepare_callback_set");
        goto unlock;
     }
   fd_handler->prep_func = func;
   fd_handler->prep_data = (void *)data;
   if ((!fd_handlers_with_prep) ||
       (fd_handlers_with_prep && (!eina_list_data_find(fd_handlers_with_prep, fd_handler))))
     /* FIXME: THIS WILL NOT SCALE WITH LOTS OF PREP FUNCTIONS!!! */
     fd_handlers_with_prep = eina_list_append(fd_handlers_with_prep, fd_handler);
unlock:
   _ecore_unlock();
}

EAPI int
ecore_main_fd_handler_fd_get(Ecore_Fd_Handler *fd_handler)
{
   int fd = -1;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(-1);
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(fd_handler, ECORE_MAGIC_FD_HANDLER))
     {
        ECORE_MAGIC_FAIL(fd_handler, ECORE_MAGIC_FD_HANDLER,
                         "ecore_main_fd_handler_fd_get");
        goto unlock;
     }
   fd = fd_handler->fd;
unlock:
   _ecore_unlock();
   return fd;
}

EAPI Eina_Bool
ecore_main_fd_handler_active_get(Ecore_Fd_Handler      *fd_handler,
                                 Ecore_Fd_Handler_Flags flags)
{
   int ret = EINA_FALSE;

   EINA_MAIN_LOOP_CHECK_RETURN_VAL(EINA_FALSE);
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(fd_handler, ECORE_MAGIC_FD_HANDLER))
     {
        ECORE_MAGIC_FAIL(fd_handler, ECORE_MAGIC_FD_HANDLER,
                         "ecore_main_fd_handler_active_get");
        goto unlock;
     }
   if ((flags & ECORE_FD_READ) && (fd_handler->read_active)) ret = EINA_TRUE;
   if ((flags & ECORE_FD_WRITE) && (fd_handler->write_active)) ret = EINA_TRUE;
   if ((flags & ECORE_FD_ERROR) && (fd_handler->error_active)) ret = EINA_TRUE;
unlock:
   _ecore_unlock();
   return ret;
}

EAPI void
ecore_main_fd_handler_active_set(Ecore_Fd_Handler      *fd_handler,
                                 Ecore_Fd_Handler_Flags flags)
{
   int ret;

   EINA_MAIN_LOOP_CHECK_RETURN;
   _ecore_lock();

   if (!ECORE_MAGIC_CHECK(fd_handler, ECORE_MAGIC_FD_HANDLER))
     {
        ECORE_MAGIC_FAIL(fd_handler, ECORE_MAGIC_FD_HANDLER,
                         "ecore_main_fd_handler_active_set");
        goto unlock;
     }
   fd_handler->flags = flags;
   ret = _ecore_main_fdh_poll_modify(fd_handler);
   if (ret < 0)
     {
        ERR("Failed to mod epoll fd %d: %s!", fd_handler->fd, strerror(ret));
     }
unlock:
   _ecore_unlock();
}

/**
 * @}
 */

void
_ecore_main_shutdown(void)
{
   if (in_main_loop)
     {
        ERR("\n"
            "*** ECORE WARNING: Calling ecore_shutdown() while still in the main loop.\n"
            "***                 Program may crash or behave strangely now.");
        return;
     }
   while (fd_handlers)
     {
        Ecore_Fd_Handler *fdh;

        fdh = fd_handlers;
        fd_handlers = (Ecore_Fd_Handler *)eina_inlist_remove(EINA_INLIST_GET(fd_handlers),
                                                             EINA_INLIST_GET(fdh));
        ECORE_MAGIC_SET(fdh, ECORE_MAGIC_NONE);
        ecore_fd_handler_mp_free(fdh);
     }
   if (fd_handlers_with_buffer)
     fd_handlers_with_buffer = eina_list_free(fd_handlers_with_buffer);
   if (fd_handlers_with_prep)
     fd_handlers_with_prep = eina_list_free(fd_handlers_with_prep);
   if (fd_handlers_to_delete)
     fd_handlers_to_delete = eina_list_free(fd_handlers_to_delete);
   if (file_fd_handlers)
     file_fd_handlers = eina_list_free(file_fd_handlers);

   fd_handlers_to_call = NULL;
   fd_handlers_to_call_current = NULL;
   fd_handlers_to_delete = NULL;
   fd_handler_current = NULL;

#ifdef _WIN32
   while (win32_handlers)
     {
        Ecore_Win32_Handler *wh;

        wh = win32_handlers;
        win32_handlers = (Ecore_Win32_Handler *)eina_inlist_remove(EINA_INLIST_GET(win32_handlers),
                                                                   EINA_INLIST_GET(wh));
        ECORE_MAGIC_SET(wh, ECORE_MAGIC_NONE);
        ecore_win32_handler_mp_free(wh);
     }
   win32_handlers_delete_me = EINA_FALSE;
   win32_handler_current = NULL;
#endif
}

static void
_ecore_main_prepare_handlers(void)
{
   Ecore_Fd_Handler *fdh;
   Eina_List *l, *l2;

   /* call the prepare callback for all handlers with prep functions */
   EINA_LIST_FOREACH_SAFE(fd_handlers_with_prep, l, l2, fdh)
     {
        if (!fdh)
          {
             fd_handlers_with_prep = eina_list_remove_list(l, fd_handlers_with_prep);
             continue;
          }
        if (!fdh->delete_me && fdh->prep_func)
          {
             fdh->references++;
             _ecore_call_prep_cb(fdh->prep_func, fdh->prep_data, fdh);
             fdh->references--;
          }
        else
          fd_handlers_with_prep = eina_list_remove_list(fd_handlers_with_prep, l);
     }
}

#ifndef USE_G_MAIN_LOOP
static int
_ecore_main_select(double timeout)
{
   struct timeval tv, *t;
   fd_set rfds, wfds, exfds;
   Ecore_Fd_Handler *fdh;
   Eina_List *l;
   int max_fd;
   int ret;

   t = NULL;
   if ((!ECORE_FINITE(timeout)) || (timeout == 0.0)) /* finite() tests for NaN, too big, too small, and infinity.  */
     {
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        t = &tv;
     }
   else if (timeout > 0.0)
     {
        int sec, usec;

#ifdef FIX_HZ
        timeout += (0.5 / HZ);
        sec = (int)timeout;
        usec = (int)((timeout - (double)sec) * 1000000);
#else
        sec = (int)timeout;
        usec = (int)((timeout - (double)sec) * 1000000);
#endif
        tv.tv_sec = sec;
        tv.tv_usec = usec;
        t = &tv;
     }
   max_fd = 0;
   FD_ZERO(&rfds);
   FD_ZERO(&wfds);
   FD_ZERO(&exfds);

   /* call the prepare callback for all handlers */
   if (fd_handlers_with_prep)
     _ecore_main_prepare_handlers();

   if (!HAVE_EPOLL || epoll_fd < 0)
     {
        EINA_INLIST_FOREACH(fd_handlers, fdh)
          {
             if (!fdh->delete_me)
               {
                  if (fdh->flags & ECORE_FD_READ)
                    {
                       FD_SET(fdh->fd, &rfds);
                       if (fdh->fd > max_fd) max_fd = fdh->fd;
                    }
                  if (fdh->flags & ECORE_FD_WRITE)
                    {
                       FD_SET(fdh->fd, &wfds);
                       if (fdh->fd > max_fd) max_fd = fdh->fd;
                    }
                  if (fdh->flags & ECORE_FD_ERROR)
                    {
                       FD_SET(fdh->fd, &exfds);
                       if (fdh->fd > max_fd) max_fd = fdh->fd;
                    }
               }
          }
     }
   else
     {
        /* polling on the epoll fd will wake when an fd in the epoll set is active */
         max_fd = _ecore_get_epoll_fd();
         FD_SET(max_fd, &rfds);
     }
   EINA_LIST_FOREACH(file_fd_handlers, l, fdh)
     if (!fdh->delete_me)
       {
          if (fdh->flags & ECORE_FD_READ)
            {
               FD_SET(fdh->fd, &rfds);
               if (fdh->fd > max_fd) max_fd = fdh->fd;
            }
          if (fdh->flags & ECORE_FD_WRITE)
            {
               FD_SET(fdh->fd, &wfds);
               if (fdh->fd > max_fd) max_fd = fdh->fd;
            }
          if (fdh->flags & ECORE_FD_ERROR)
            {
               FD_SET(fdh->fd, &exfds);
               if (fdh->fd > max_fd) max_fd = fdh->fd;
            }
          if (fdh->fd > max_fd) max_fd = fdh->fd;
       }
   if (_ecore_signal_count_get()) return -1;

   _ecore_unlock();
   ret = main_loop_select(max_fd + 1, &rfds, &wfds, &exfds, t);
   _ecore_lock();

   _ecore_time_loop_time = ecore_time_get();
   if (ret < 0)
     {
#ifndef _WIN32
        if (errno == EINTR) return -1;
        else if (errno == EBADF)
          _ecore_main_fd_handlers_bads_rem();
#endif
     }
   if (ret > 0)
     {
        if (HAVE_EPOLL && epoll_fd >= 0)
          _ecore_main_fdh_epoll_mark_active();
        else
          {
             EINA_INLIST_FOREACH(fd_handlers, fdh)
               {
                  if (!fdh->delete_me)
                    {
                       if (FD_ISSET(fdh->fd, &rfds))
                         fdh->read_active = EINA_TRUE;
                       if (FD_ISSET(fdh->fd, &wfds))
                         fdh->write_active = EINA_TRUE;
                       if (FD_ISSET(fdh->fd, &exfds))
                         fdh->error_active = EINA_TRUE;
                       _ecore_try_add_to_call_list(fdh);
                    }
               }
          }
        EINA_LIST_FOREACH(file_fd_handlers, l, fdh)
          {
             if (!fdh->delete_me)
               {
                  if (FD_ISSET(fdh->fd, &rfds))
                    fdh->read_active = EINA_TRUE;
                  if (FD_ISSET(fdh->fd, &wfds))
                    fdh->write_active = EINA_TRUE;
                  if (FD_ISSET(fdh->fd, &exfds))
                    fdh->error_active = EINA_TRUE;
                  _ecore_try_add_to_call_list(fdh);
               }
          }
        _ecore_main_fd_handlers_cleanup();
#ifdef _WIN32
        _ecore_main_win32_handlers_cleanup();
#endif
        return 1;
     }
   return 0;
}

#endif

#ifndef _WIN32
# ifndef USE_G_MAIN_LOOP
static void
_ecore_main_fd_handlers_bads_rem(void)
{
   Ecore_Fd_Handler *fdh;
   Eina_Inlist *l;
   int found = 0;

   ERR("Removing bad fds");
   for (l = EINA_INLIST_GET(fd_handlers); l; )
     {
        fdh = (Ecore_Fd_Handler *)l;
        l = l->next;
        errno = 0;

        if ((fcntl(fdh->fd, F_GETFD) < 0) && (errno == EBADF))
          {
             ERR("Found bad fd at index %d", fdh->fd);
             if (fdh->flags & ECORE_FD_ERROR)
               {
                  ERR("Fd set for error! calling user");
                  fdh->references++;
                  if (!_ecore_call_fd_cb(fdh->func, fdh->data, fdh))
                    {
                       ERR("Fd function err returned 0, remove it");
                       if (!fdh->delete_me)
                         {
                            fdh->delete_me = EINA_TRUE;
                            fd_handlers_to_delete = eina_list_append(fd_handlers_to_delete, fdh);
                         }
                       found++;
                    }
                  fdh->references--;
               }
             else
               {
                  ERR("Problematic fd found at %d! setting it for delete", fdh->fd);
                  if (!fdh->delete_me)
                    {
                       fdh->delete_me = EINA_TRUE;
                       fd_handlers_to_delete = eina_list_append(fd_handlers_to_delete, fdh);
                    }

                  found++;
               }
          }
     }
   if (found == 0)
     {
#  ifdef HAVE_GLIB
        ERR("No bad fd found. Maybe a foreign fd from glib?");
#  else
        ERR("No bad fd found. EEEK!");
#  endif
     }
   _ecore_main_fd_handlers_cleanup();
}

# endif
#endif

static void
_ecore_main_fd_handlers_cleanup(void)
{
   Ecore_Fd_Handler *fdh;
   Eina_List *l, *l2;

   if (!fd_handlers_to_delete) return;
   EINA_LIST_FOREACH_SAFE(fd_handlers_to_delete, l, l2, fdh)
     {
        if (!fdh)
          {
             fd_handlers_to_delete = eina_list_remove_list(l, fd_handlers_to_delete);
             continue;
          }
        /* fdh->delete_me should be set for all fdhs at the start of the list */
        if (fdh->references)
          continue;
        if (fdh->buf_func && fd_handlers_with_buffer)
          fd_handlers_with_buffer = eina_list_remove(fd_handlers_with_buffer, fdh);
        if (fdh->prep_func && fd_handlers_with_prep)
          fd_handlers_with_prep = eina_list_remove(fd_handlers_with_prep, fdh);
        fd_handlers = (Ecore_Fd_Handler *)
          eina_inlist_remove(EINA_INLIST_GET(fd_handlers), EINA_INLIST_GET(fdh));
        if (fdh->file)
          file_fd_handlers = eina_list_remove(file_fd_handlers, fdh);
        ECORE_MAGIC_SET(fdh, ECORE_MAGIC_NONE);
        ecore_fd_handler_mp_free(fdh);
        fd_handlers_to_delete = eina_list_remove_list(fd_handlers_to_delete, l);
     }
}

#ifdef _WIN32
static void
_ecore_main_win32_handlers_cleanup(void)
{
   Ecore_Win32_Handler *wh;
   Eina_Inlist *l;
   int deleted_in_use = 0;

   if (!win32_handlers_delete_me) return;
   for (l = EINA_INLIST_GET(win32_handlers); l; )
     {
        wh = (Ecore_Win32_Handler *)l;

        l = l->next;
        if (wh->delete_me)
          {
             if (wh->references)
               {
                  deleted_in_use++;
                  continue;
               }

             win32_handlers = (Ecore_Win32_Handler *)
               eina_inlist_remove(EINA_INLIST_GET(win32_handlers),
                                  EINA_INLIST_GET(wh));
             ECORE_MAGIC_SET(wh, ECORE_MAGIC_NONE);
             ecore_win32_handler_mp_free(wh);
          }
     }
   if (!deleted_in_use) win32_handlers_delete_me = EINA_FALSE;
}

#endif

static void
_ecore_main_fd_handlers_call(void)
{
   /* grab a new list */
    if (!fd_handlers_to_call_current)
      {
         fd_handlers_to_call_current = fd_handlers_to_call;
         fd_handlers_to_call = NULL;
      }

    while (fd_handlers_to_call_current)
      {
         Ecore_Fd_Handler *fdh = fd_handlers_to_call_current;

         if (!fdh->delete_me)
           {
              if ((fdh->read_active) ||
                  (fdh->write_active) ||
                  (fdh->error_active))
                {
                   fdh->references++;
                   if (!_ecore_call_fd_cb(fdh->func, fdh->data, fdh))
                     {
                        if (!fdh->delete_me)
                          {
                             fdh->delete_me = EINA_TRUE;
                             fd_handlers_to_delete = eina_list_append(fd_handlers_to_delete, fdh);
                          }
                     }
                   fdh->references--;
                   _ecore_fd_valid();

                   fdh->read_active = EINA_FALSE;
                   fdh->write_active = EINA_FALSE;
                   fdh->error_active = EINA_FALSE;
                }
           }

         /* stop when we point to ourselves */
         if (fdh->next_ready == fdh)
           {
              fdh->next_ready = NULL;
              fd_handlers_to_call_current = NULL;
              break;
           }

         fd_handlers_to_call_current = fdh->next_ready;
         fdh->next_ready = NULL;
      }
}

static int
_ecore_main_fd_handlers_buf_call(void)
{
   Ecore_Fd_Handler *fdh;
   Eina_List *l, *l2;
   int ret;

   ret = 0;
   EINA_LIST_FOREACH_SAFE(fd_handlers_with_buffer, l, l2, fdh)
     {
        if (!fdh)
          {
             fd_handlers_with_buffer = eina_list_remove_list(l, fd_handlers_with_buffer);
             continue;
          }
        if ((!fdh->delete_me) && fdh->buf_func)
          {
             fdh->references++;
             if (_ecore_call_fd_cb(fdh->buf_func, fdh->buf_data, fdh))
               {
                  ret |= _ecore_call_fd_cb(fdh->func, fdh->data, fdh);
                  fdh->read_active = EINA_TRUE;
                  _ecore_try_add_to_call_list(fdh);
               }
             fdh->references--;
          }
        else
          fd_handlers_with_buffer = eina_list_remove_list(fd_handlers_with_buffer, l);
     }
   return ret;
}

#ifndef USE_G_MAIN_LOOP

enum {
   SPIN_MORE,
   SPIN_RESTART,
   LOOP_CONTINUE
};

static int
_ecore_main_loop_spin_core(void)
{
   /* as we are spinning we need to update loop time per spin */
    _ecore_time_loop_time = ecore_time_get();
    /* call all idlers, which returns false if no more idelrs exist */
    if (!_ecore_idler_all_call()) return SPIN_RESTART;
    /* sneaky - drop through or if checks - the first one to succeed
     * drops through and returns "continue" so further ones dont run */
    if ((_ecore_main_select(0.0) > 0) || (_ecore_event_exist()) ||
        (_ecore_signal_count_get() > 0) || (do_quit))
      return LOOP_CONTINUE;
    /* default - spin more */
    return SPIN_MORE;
}

static int
_ecore_main_loop_spin_no_timers(void)
{
   /* if we have idlers we HAVE to spin and handle everything
    * in a polling way - spin in a tight polling loop */
     for (;; )
       {
          int action = _ecore_main_loop_spin_core();
          if (action != SPIN_MORE) return action;
          /* if an idler has added a timer then we need to go through
           * the start of the spin cycle again to handle cases properly */
          if (_ecore_timers_exists()) return SPIN_RESTART;
       }
     /* just contiune handling events etc. */
     return LOOP_CONTINUE;
}

static int
_ecore_main_loop_spin_timers(void)
{
   /* if we have idlers we HAVE to spin and handle everything
    * in a polling way - spin in a tight polling loop */
     for (;; )
       {
          int action = _ecore_main_loop_spin_core();
          if (action != SPIN_MORE) return action;
          /* if next timer expires now or in the past - stop spinning and
           * continue the mainloop walk as our "select" timeout has
           * expired now */
          if (_ecore_timer_next_get() <= 0.0) return LOOP_CONTINUE;
       }
     /* just contiune handling events etc. */
     return LOOP_CONTINUE;
}

static void
_ecore_fps_marker_1(void)
{
   if (!_ecore_fps_debug) return;
   t2 = ecore_time_get();
   if ((t1 > 0.0) && (t2 > 0.0)) _ecore_fps_debug_runtime_add(t2 - t1);
}

static void
_ecore_fps_marker_2(void)
{
   if (!_ecore_fps_debug) return;
   t1 = ecore_time_get();
}

static void
_ecore_main_loop_iterate_internal(int once_only)
{
   double next_time = -1.0;

   in_main_loop++;
   /* expire any timers */
   _ecore_timer_expired_timers_call(_ecore_time_loop_time);
   _ecore_timer_cleanup();

   /* process signals into events .... */
   _ecore_signal_received_process();
   /* if as a result of timers/animators or signals we have accumulated
    * events, then instantly handle them */
   if (_ecore_event_exist())
     {
        /* but first conceptually enter an idle state */
        _ecore_idle_enterer_call();
        _ecore_throttle();
        /* now quickly poll to see which input fd's are active */
        _ecore_main_select(0.0);
        /* allow newly queued timers to expire from now on */
        _ecore_timer_enable_new();
        /* go straight to processing the events we had queued */
        goto process_all;
     }

   if (once_only)
     {
        /* in once_only mode we should quickly poll for inputs, signals
         * if we got any events or signals, allow new timers to process.
         * use bitwise or to force both conditions to be tested and
         * merged together */
        if (_ecore_main_select(0.0) | _ecore_signal_count_get())
          {
             _ecore_timer_enable_new();
             goto process_all;
          }
     }
   else
     {
        /* call idle enterers ... */
        _ecore_idle_enterer_call();
        _ecore_throttle();
     }

   /* if these calls caused any buffered events to appear - deal with them */
   if (fd_handlers_with_buffer)
     _ecore_main_fd_handlers_buf_call();

   /* if there are any (buffered fd handling may generate them)
    * then jump to processing them */
   if (_ecore_event_exist())
     {
        _ecore_main_select(0.0);
        _ecore_timer_enable_new();
        goto process_all;
     }

   if (once_only)
     {
        /* in once_only mode enter idle here instead and then return */
        _ecore_idle_enterer_call();
        _ecore_throttle();
        _ecore_timer_enable_new();
        goto done;
     }

   _ecore_fps_marker_1();

   /* start of the sleeping or looping section */
start_loop: /***************************************************************/
   /* any timers re-added as a result of these are allowed to go */
   _ecore_timer_enable_new();
   /* if we have been asked to quit the mainloop then exit at this point */
   if (do_quit)
     {
        _ecore_timer_enable_new();
        goto done;
     }
   if (!_ecore_event_exist())
     {
        /* init flags */
        next_time = _ecore_timer_next_get();
        /* no idlers */
        if (!_ecore_idler_exist())
          {
             /* sleep until timeout or forever (-1.0) waiting for on fds */
             _ecore_main_select(next_time);
          }
        else
          {
             int action = LOOP_CONTINUE;

             /* no timers - spin */
             if (next_time < 0) action = _ecore_main_loop_spin_no_timers();
             /* timers - spin */
             else action = _ecore_main_loop_spin_timers();
             if (action == SPIN_RESTART) goto start_loop;
          }
     }
   _ecore_fps_marker_2();

   /* actually wake up and deal with input, events etc. */
process_all: /***********************************************************/

   /* we came out of our "wait state" so idle has exited */
   if (!once_only)
     {
        _ecore_animator_run_reset();
        _ecore_idle_exiter_call();
     }
   /* call the fd handler per fd that became alive... */
   /* this should read or write any data to the monitored fd and then */
   /* post events onto the ecore event pipe if necessary */
   _ecore_main_fd_handlers_call();
   if (fd_handlers_with_buffer) _ecore_main_fd_handlers_buf_call();
   /* process signals into events .... */
   _ecore_signal_received_process();
   /* handle events ... */
   _ecore_event_call();
   _ecore_main_fd_handlers_cleanup();

   if (once_only)
     {
        /* if in once_only mode handle idle exiting */
        _ecore_idle_enterer_call();
        _ecore_throttle();
     }

done: /*******************************************************************/
   in_main_loop--;
}

#endif

#ifdef _WIN32
static int
_ecore_main_win32_select(int             nfds EINA_UNUSED,
                         fd_set         *readfds,
                         fd_set         *writefds,
                         fd_set         *exceptfds,
                         struct timeval *tv)
{
   HANDLE objects[MAXIMUM_WAIT_OBJECTS];
   int sockets[MAXIMUM_WAIT_OBJECTS];
   Ecore_Fd_Handler *fdh;
   Ecore_Win32_Handler *wh;
   unsigned int objects_nbr = 0;
   unsigned int handles_nbr = 0;
   unsigned int events_nbr = 0;
   DWORD result;
   DWORD timeout;
   MSG msg;
   unsigned int i;
   int res;

   /* Create an event object per socket */
   EINA_INLIST_FOREACH(fd_handlers, fdh)
     {
        WSAEVENT event;
        long network_event;

        network_event = 0;
        if (readfds)
          {
             if (FD_ISSET(fdh->fd, readfds))
               network_event |= FD_READ;
          }
        if (writefds)
          {
             if (FD_ISSET(fdh->fd, writefds))
               network_event |= FD_WRITE;
          }
        if (exceptfds)
          {
             if (FD_ISSET(fdh->fd, exceptfds))
               network_event |= FD_OOB;
          }

        if (network_event)
          {
             event = WSACreateEvent();
             WSAEventSelect(fdh->fd, event, network_event);
             objects[objects_nbr] = event;
             sockets[events_nbr] = fdh->fd;
             events_nbr++;
             objects_nbr++;
          }
     }

   /* store the HANDLEs in the objects to wait for */
   EINA_INLIST_FOREACH(win32_handlers, wh)
     {
        objects[objects_nbr] = wh->h;
        handles_nbr++;
        objects_nbr++;
     }

   /* Empty the queue before waiting */
   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
     {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
     }

   /* Wait for any message sent or posted to this queue */
   /* or for one of the passed handles be set to signaled. */
   if (!tv)
     timeout = INFINITE;
   else
     timeout = (DWORD)((tv->tv_sec * 1000.0) + (tv->tv_usec / 1000.0));

   if (timeout == 0) return 0;

   result = MsgWaitForMultipleObjects(objects_nbr, (const HANDLE *)objects, EINA_FALSE,
                                      timeout, QS_ALLINPUT);

   if (readfds)
     FD_ZERO(readfds);
   if (writefds)
     FD_ZERO(writefds);
   if (exceptfds)
     FD_ZERO(exceptfds);

   /* The result tells us the type of event we have. */
   if (result == WAIT_FAILED)
     {
        char *m;

        m = evil_last_error_get();
        ERR("%s", m);
        free(m);
        res = -1;
     }
   else if (result == WAIT_TIMEOUT)
     {
        /* ERR("time out\n"); */
         res = 0;
     }
   else if (result == (WAIT_OBJECT_0 + objects_nbr))
     {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
          {
             TranslateMessage(&msg);
             DispatchMessage(&msg);
          }

        res = 0;
     }
   else if ((result >= 0) && (result < WAIT_OBJECT_0 + events_nbr))
     {
        WSANETWORKEVENTS network_event;

        WSAEnumNetworkEvents(sockets[result], objects[result], &network_event);

        if ((network_event.lNetworkEvents & FD_READ) && readfds)
          FD_SET(sockets[result], readfds);
        if ((network_event.lNetworkEvents & FD_WRITE) && writefds)
          FD_SET(sockets[result], writefds);
        if ((network_event.lNetworkEvents & FD_OOB) && exceptfds)
          FD_SET(sockets[result], exceptfds);

        res = 1;
     }
   else if ((result >= (WAIT_OBJECT_0 + events_nbr)) &&
            (result < (WAIT_OBJECT_0 + objects_nbr)))
     {
        if (!win32_handler_current)
          {
             /* regular main loop, start from head */
              win32_handler_current = win32_handlers;
          }
        else
          {
             /* recursive main loop, continue from where we were */
              win32_handler_current = (Ecore_Win32_Handler *)EINA_INLIST_GET(win32_handler_current)->next;
          }

        while (win32_handler_current)
          {
             wh = win32_handler_current;

             if (objects[result - WAIT_OBJECT_0] == wh->h)
               {
                  if (!wh->delete_me)
                    {
                       wh->references++;
                       if (!wh->func(wh->data, wh))
                         {
                            wh->delete_me = EINA_TRUE;
                            win32_handlers_delete_me = EINA_TRUE;
                         }
                       wh->references--;
                    }
               }
             if (win32_handler_current) /* may have changed in recursive main loops */
               win32_handler_current = (Ecore_Win32_Handler *)EINA_INLIST_GET(win32_handler_current)->next;
          }
        res = 1;
     }
   else
     {
        ERR("unknown result...\n");
        res = -1;
     }

   /* Remove event objects again */
   for (i = 0; i < events_nbr; i++) WSACloseEvent(objects[i]);

   return res;
}

#endif
