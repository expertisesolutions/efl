#ifndef _EFL_LOOP_DATA_H
#define _EFL_LOOP_DATA_H

typedef struct _Efl_Loop_Data Efl_Loop_Data;
typedef struct _Efl_Loop_Timer_Data Efl_Loop_Timer_Data;

struct _Efl_Loop_Data
{
   double               loop_time;

   Efl_Loop_Message_Handler *future_message_handler;

   Efl_Loop_Timer      *poll_high;
   Efl_Loop_Timer      *poll_medium;
   Efl_Loop_Timer      *poll_low;

   Eina_List           *exes; // only used in main loop (for now?)

   Eina_List           *fd_handlers_obj;

   Ecore_Fd_Handler    *fd_handlers;
   Eina_List           *fd_handlers_with_prep;
   Eina_List           *file_fd_handlers;
   Eina_List           *always_fd_handlers;
   Eina_List           *fd_handlers_with_buffer;
   Eina_List           *fd_handlers_to_delete;
   Ecore_Fd_Handler    *fd_handlers_to_call;
   Ecore_Fd_Handler    *fd_handlers_to_call_current;

# ifdef _WIN32
   Ecore_Win32_Handler *win32_handlers;
   Ecore_Win32_Handler *win32_handler_current;
   Eina_List           *win32_handlers_to_delete;
# endif

   Eina_List           *thread_children;

   Eina_Inlist         *message_queue;
   Eina_Inlist         *message_pending_queue;
   unsigned int         message_walking;

   unsigned int         throttle;

   int                  epoll_fd;
   pid_t                epoll_pid;
   int                  timer_fd;

   double               last_check;
   Eina_Inlist         *timers;
   Eina_Inlist         *suspended;
   Efl_Loop_Timer_Data *timer_current;
   int                  timers_added;

   Eina_Value           exit_code;

   int                  idlers;
   int                  in_loop;
   unsigned int         loop_active;

   struct {
      int               high;
      int               medium;
      int               low;
   } pollers;

   struct {
      char      **environ_ptr;
      char      **environ_copy;
   } env;

   Eina_Bool            do_quit : 1;
   Eina_Bool            quit_on_last_thread_child_del : 1;
};


#endif
