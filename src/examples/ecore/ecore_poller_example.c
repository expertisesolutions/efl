//Compile with:
// gcc -o ecore_poller_example ecore_poller_example.c `pkg-config --libs --cflags ecore eo`

#include <Ecore.h>
//#include <Ecore_Eo.h>
#ifndef _MSC_VER
# include <unistd.h>
#endif

static double _initial_time = 0;

static Eina_Bool
_poller_print_cb(void *data)
{
   char *str = data;
   printf("Ecore Poller '%s' callback called after %0.3f seconds.\n",
          str, ecore_time_get() - _initial_time);

   return ECORE_CALLBACK_RENEW;
}

static Eina_Bool
_poller_quit_cb(void *data EINA_UNUSED)
{

   ecore_main_loop_quit();
   return EINA_TRUE;
}

int
main(void)
{
   double interval = 0.3; // tick each 0.3 seconds
   Ecore_Poller *poller1, *poller2, *poller3;
   char *str1 = "poller1";
   char *str2 = "poller2";
   char *str3 = "poller3";

   if (!ecore_init())
     {
        printf("ERROR: Cannot init Ecore!\n");
        return -1;
     }

   _initial_time = ecore_time_get();

   ecore_poller_poll_interval_set(ECORE_POLLER_CORE, interval);

   poller1 = ecore_poller_add(ECORE_POLLER_CORE, 4, _poller_print_cb, str1);
   poller2 = ecore_poller_add(ECORE_POLLER_CORE, 8, _poller_print_cb, str2);
   poller3 = ecore_poller_add(ECORE_POLLER_CORE, 30, _poller_quit_cb, str3);

//   poller1 = efl_add_ref(ECORE_POLLER_CLASS, NULL, //         ecore_poller_constructor(efl_added, ECORE_POLLER_CORE, 4, _poller_print_cb, str1));
//   poller2 = efl_add_ref(ECORE_POLLER_CLASS, NULL, //         ecore_poller_constructor(efl_added, ECORE_POLLER_CORE, 8, _poller_print_cb, str2));
//   poller3 = efl_add_ref(ECORE_POLLER_CLASS, NULL, //         ecore_poller_constructor(efl_added, ECORE_POLLER_CORE, 20, _poller_quit_cb, str3));


   ecore_main_loop_begin();

   printf("changing poller2 interval to 16\n");

   ecore_poller_poller_interval_set(poller2, 16);
//   ecore_poller_interval_set(poller2, 16, NULL);
   ecore_main_loop_begin();

//   efl_unref(poller1);
//   efl_unref(poller2);
//   efl_unref(poller3);
   ecore_poller_del(poller1);
   ecore_poller_del(poller2);
   ecore_poller_del(poller3);

   ecore_shutdown();
}

