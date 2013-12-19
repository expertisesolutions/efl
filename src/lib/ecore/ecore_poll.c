#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#include <Eo.h>

#include "Ecore.h"
#include "ecore_private.h"

#define MY_CLASS ECORE_POLLER_CLASS

#define MY_CLASS_NAME "Ecore_Poller"

EAPI Eo_Op ECORE_POLLER_BASE_ID = EO_NOOP;

#define ECORE_POLLER_CHECK(obj)                       \
  if (!eo_isa((obj), ECORE_POLLER_CLASS)) \
    return

struct _Ecore_Poller_Private_Data
{
   EINA_INLIST;
   ECORE_MAGIC;
   Ecore_Poller *obj;
   int           ibit;
   unsigned char delete_me : 1;
   Ecore_Task_Cb func;
   void         *data;
};

typedef struct _Ecore_Poller_Private_Data Ecore_Poller_Private_Data;

static Ecore_Timer *timer = NULL;
static int min_interval = -1;
static int interval_incr = 0;
static int at_tick = 0;
static int just_added_poller = 0;
static int poller_delete_count = 0;
static int poller_walking = 0;
static double poll_interval = 0.125;
static double poll_cur_interval = 0.0;
static double last_tick = 0.0;
static Ecore_Poller_Private_Data *pollers[16] =
{
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};
static unsigned short poller_counters[16] =
{
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0
};

static void      _ecore_poller_next_tick_eval(void);
static Eina_Bool _ecore_poller_cb_timer(void *data);

static void
_ecore_poller_next_tick_eval(void)
{
   int i;
   double interval;

   min_interval = -1;
   for (i = 0; i < 15; i++)
     {
        if (pollers[i])
          {
             min_interval = i;
             break;
          }
     }
   if (min_interval < 0)
     {
        /* no pollers */
         if (timer)
           {
              ecore_timer_del(timer);
              timer = NULL;
           }
         return;
     }
   interval_incr = (1 << min_interval);
   interval = interval_incr * poll_interval;
   /* we are at the tick callback - so no need to do inter-tick adjustments
    * so we can fasttrack this as t -= last_tick in theory is 0.0 (though
    * in practice it will be a very very very small value. also the tick
    * callback will adjust the timer interval at the end anyway */
   if (at_tick)
     {
        if (!timer)
          timer = ecore_timer_add(interval, _ecore_poller_cb_timer, NULL);
     }
   else
     {
        double t;

        if (!timer)
          timer = ecore_timer_add(interval, _ecore_poller_cb_timer, NULL);
        else
          {
             t = ecore_time_get();
             if (interval != poll_cur_interval)
               {
                  t -= last_tick; /* time since we last ticked */
     /* delete the timer and reset it to tick off in the new
      * time interval. at the tick this will be adjusted */
                  ecore_timer_del(timer);
                  timer = ecore_timer_add(interval - t,
                                          _ecore_poller_cb_timer, NULL);
               }
          }
     }
   poll_cur_interval = interval;
}

static Eina_Bool
_ecore_poller_cb_timer(void *data EINA_UNUSED)
{
   int i;
   Ecore_Poller_Private_Data *poller, *l;
   int changes = 0;

   at_tick++;
   last_tick = ecore_time_get();
   /* we have 16 counters - each increments every time the poller counter
    * "ticks". it increments by the minimum interval (which can be 1, 2, 4,
    * 7, 16 etc. up to 32768) */
   for (i = 0; i < 15; i++)
     {
        poller_counters[i] += interval_incr;
        /* wrap back to 0 if we exceed out loop count for the counter */
        if (poller_counters[i] >= (1 << i)) poller_counters[i] = 0;
     }

   just_added_poller = 0;
   /* walk the pollers now */
   poller_walking++;
   for (i = 0; i < 15; i++)
     {
        /* if the counter is @ 0 - this means that counter "went off" this
         * tick interval, so run all pollers hooked to that counter */
          if (poller_counters[i] == 0)
            {
               EINA_INLIST_FOREACH(pollers[i], poller)
                 {
                    if (!poller->delete_me)
                      {
                         if (!poller->func(poller->data))
                           {
                              if (!poller->delete_me)
                                {
                                   poller->delete_me = 1;
                                   poller_delete_count++;
                                }
                           }
                      }
                 }
            }
     }
   poller_walking--;

   /* handle deletes afterwards */
   if (poller_delete_count > 0)
     {
        /* FIXME: walk all pollers and remove deleted ones */
         for (i = 0; i < 15; i++)
           {
              for (l = pollers[i]; l; )
                {
                   poller = l;
                   l = (Ecore_Poller_Private_Data *)EINA_INLIST_GET(l)->next;
                   if (poller->delete_me)
                     {
                        pollers[i] = (Ecore_Poller_Private_Data *)eina_inlist_remove(EINA_INLIST_GET(pollers[i]), EINA_INLIST_GET(poller));

                        eo_do(poller->obj, eo_parent_set(NULL));
                        if (eo_destructed_is(poller->obj))
                           eo_manual_free(poller->obj);
                        else
                           eo_manual_free_set(poller->obj, EINA_FALSE);

                        poller_delete_count--;
                        changes++;
                        if (poller_delete_count <= 0) break;
                     }
                }
              if (poller_delete_count <= 0) break;
           }
     }
   /* if we deleted or added any pollers, then we need to re-evaluate our
    * minimum poll interval */
   if ((changes > 0) || (just_added_poller > 0))
     _ecore_poller_next_tick_eval();

   just_added_poller = 0;
   poller_delete_count = 0;

   at_tick--;

   /* if the timer was deleted then there is no point returning 1 - ambiguous
    * if we do as it implies keep running me" but we have been deleted
    * anyway */
   if (!timer) return ECORE_CALLBACK_CANCEL;

   /* adjust interval */
   ecore_timer_interval_set(timer, poll_cur_interval);
   return ECORE_CALLBACK_RENEW;
}

EAPI void
ecore_poller_poll_interval_set(Ecore_Poller_Type type EINA_UNUSED,
                               double            poll_time)
{
   EINA_MAIN_LOOP_CHECK_RETURN;

   if (poll_time < 0.0)
     {
        ERR("Poll time %f less than zero, ignored", poll_time);
        return;
     }

   poll_interval = poll_time;
   _ecore_poller_next_tick_eval();
}

EAPI double
ecore_poller_poll_interval_get(Ecore_Poller_Type type EINA_UNUSED)
{
   EINA_MAIN_LOOP_CHECK_RETURN_VAL(0.0);
   return poll_interval;
}

static void
_constructor(Eo *obj, void *_pd EINA_UNUSED, va_list *list EINA_UNUSED)
{
   eo_error_set(obj);
   ERR("only custom constructor can be used with '%s' class", MY_CLASS_NAME);
}

EAPI Ecore_Poller *
ecore_poller_add(Ecore_Poller_Type type EINA_UNUSED,
                 int               interval,
                 Ecore_Task_Cb     func,
                 const void       *data)
{
   Ecore_Poller *poller;
   poller = eo_add_custom(MY_CLASS, _ecore_parent,
                          ecore_poller_constructor(type, interval, func, data));
   eo_unref(poller);
   return poller;
}

static void
_poller_constructor(Eo *obj, void *_pd, va_list *list)
{
   Ecore_Poller_Type type = va_arg(*list, Ecore_Poller_Type);
   (void)type;
   int interval = va_arg(*list, int);
   Ecore_Task_Cb func = va_arg(*list, Ecore_Task_Cb);
   const void *data = va_arg(*list, const void *);


   Ecore_Poller_Private_Data *poller = _pd;
   poller->obj = obj;

   int ibit;

    if (EINA_UNLIKELY(!eina_main_loop_is()))
      {
         eo_error_set(obj);
         EINA_MAIN_LOOP_CHECK_RETURN;
      }

   eo_do_super(obj, MY_CLASS, eo_constructor());
   eo_manual_free_set(obj, EINA_TRUE);

   if (!func)
     {
        eo_error_set(obj);
        ERR("callback function must be set up for an object of class: '%s'", MY_CLASS_NAME);
        return;
     }

   /* interval MUST be a power of 2, so enforce it */
   if (interval < 1) interval = 1;
   ibit = -1;
   while (interval != 0)
     {
        ibit++;
        interval >>= 1;
     }
   /* only allow up to 32768 - i.e. ibit == 15, so limit it */
   if (ibit > 15) ibit = 15;

   poller->ibit = ibit;
   poller->func = func;
   poller->data = (void *)data;
   pollers[poller->ibit] = (Ecore_Poller_Private_Data *)eina_inlist_prepend(EINA_INLIST_GET(pollers[poller->ibit]), EINA_INLIST_GET(poller));
   if (poller_walking)
     just_added_poller++;
   else
     _ecore_poller_next_tick_eval();
}

EAPI Eina_Bool
ecore_poller_poller_interval_set(Ecore_Poller *obj,
                                 int           interval)
{
   Eina_Bool ret;
   ECORE_POLLER_CHECK(obj) EINA_FALSE;
   eo_do(obj, ecore_poller_interval_set(interval, &ret));
   return ret;
}

static void
_poller_interval_set(Eo *obj EINA_UNUSED, void *_pd, va_list *list)
{
   int interval = va_arg(*list, int);
   Eina_Bool *ret = va_arg(*list, Eina_Bool *);
   Eina_Bool int_ret;

   EINA_MAIN_LOOP_CHECK_RETURN;

   int ibit;

   Ecore_Poller_Private_Data *poller = _pd;

   /* interval MUST be a power of 2, so enforce it */
   if (interval < 1) interval = 1;
   ibit = -1;
   while (interval != 0)
     {
        ibit++;
        interval >>= 1;
     }
   /* only allow up to 32768 - i.e. ibit == 15, so limit it */
   if (ibit > 15) ibit = 15;
   /* if interval specified is the same as interval set, return true without wasting time */
   if (poller->ibit == ibit)
     {
        int_ret = EINA_TRUE;
        goto end;
     }

   pollers[poller->ibit] = (Ecore_Poller_Private_Data *)eina_inlist_remove(EINA_INLIST_GET(pollers[poller->ibit]), EINA_INLIST_GET(poller));
   poller->ibit = ibit;
   pollers[poller->ibit] = (Ecore_Poller_Private_Data *)eina_inlist_prepend(EINA_INLIST_GET(pollers[poller->ibit]), EINA_INLIST_GET(poller));
   if (poller_walking)
     just_added_poller++;
   else
     _ecore_poller_next_tick_eval();

   int_ret = EINA_TRUE;

end:
   if (ret) *ret = int_ret;
}

EAPI int
ecore_poller_poller_interval_get(Ecore_Poller *obj)
{
   int ret;
   ECORE_POLLER_CHECK(obj) EINA_FALSE;
   eo_do(obj, ecore_poller_interval_get(&ret));
   return ret;
}

static void
_poller_interval_get(Eo *obj EINA_UNUSED, void *_pd, va_list *list)
{
   int *ret = va_arg(*list, int *);
   *ret =0;

   int ibit, interval = 1;

   EINA_MAIN_LOOP_CHECK_RETURN;
   Ecore_Poller_Private_Data *poller = _pd;

   ibit = poller->ibit;
   while (ibit != 0)
     {
        ibit--;
        interval <<= 1;
     }
   *ret = interval;
}

EAPI void *
ecore_poller_del(Ecore_Poller *obj)
{
   void *data;

   if (!obj) return NULL;
   EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
   Ecore_Poller_Private_Data *poller = eo_data_scope_get(obj, MY_CLASS);
   /* we are walking the poller list - a bad idea to remove from it while
    * walking it, so just flag it as delete_me and come back to it after
    * the loop has finished */
   if (poller_walking > 0)
     {
        poller_delete_count++;
        poller->delete_me = 1;
        return poller->data;
     }
   /* not in loop so safe - delete immediately */
   data = poller->data;
   pollers[poller->ibit] = (Ecore_Poller_Private_Data *)eina_inlist_remove(EINA_INLIST_GET(pollers[poller->ibit]), EINA_INLIST_GET(poller));

   eo_do(poller->obj, eo_parent_set(NULL));
   if (eo_destructed_is(poller->obj))
      eo_manual_free(obj);
   else
      eo_manual_free_set(obj, EINA_FALSE);

   _ecore_poller_next_tick_eval();
   return data;
}

static void
_destructor(Eo *obj, void *_pd, va_list *list EINA_UNUSED)
{
   Ecore_Poller_Private_Data *pd = _pd;

   if (!pd->delete_me)
   {
     pd->delete_me = 1;
     poller_delete_count++;
   }

   eo_do_super(obj, MY_CLASS, eo_destructor());
}

/**
 * @}
 */

void
_ecore_poller_shutdown(void)
{
   int i;
   Ecore_Poller_Private_Data *poller;

   for (i = 0; i < 15; i++)
     {
        while ((poller = pollers[i]))
          {
             pollers[i] = (Ecore_Poller_Private_Data *)eina_inlist_remove(EINA_INLIST_GET(pollers[i]), EINA_INLIST_GET(pollers[i]));
             eo_do(poller->obj, eo_parent_set(NULL));
             if (eo_destructed_is(poller->obj))
                eo_manual_free(poller->obj);
             else
                eo_manual_free_set(poller->obj, EINA_FALSE);
          }
     }
}

static void
_class_constructor(Eo_Class *klass)
{
   const Eo_Op_Func_Description func_desc[] = {
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_CONSTRUCTOR), _constructor),
        EO_OP_FUNC(EO_BASE_ID(EO_BASE_SUB_ID_DESTRUCTOR), _destructor),
        EO_OP_FUNC(ECORE_POLLER_ID(ECORE_POLLER_SUB_ID_CONSTRUCTOR), _poller_constructor),
        EO_OP_FUNC(ECORE_POLLER_ID(ECORE_POLLER_SUB_ID_INTERVAL_SET), _poller_interval_set),
        EO_OP_FUNC(ECORE_POLLER_ID(ECORE_POLLER_SUB_ID_INTERVAL_GET), _poller_interval_get),
        EO_OP_FUNC_SENTINEL
   };

   eo_class_funcs_set(klass, func_desc);
}

static const Eo_Op_Description op_desc[] = {
     EO_OP_DESCRIPTION(ECORE_POLLER_SUB_ID_CONSTRUCTOR, "Add an idle enterer handler."),
     EO_OP_DESCRIPTION(ECORE_POLLER_SUB_ID_INTERVAL_SET, "Changes the polling interval rate of poller."),
     EO_OP_DESCRIPTION(ECORE_POLLER_SUB_ID_INTERVAL_GET, "Gets the polling interval rate of poller"),
     EO_OP_DESCRIPTION_SENTINEL
};
static const Eo_Class_Description class_desc = {
     EO_VERSION,
     MY_CLASS_NAME,
     EO_CLASS_TYPE_REGULAR,
     EO_CLASS_DESCRIPTION_OPS(&ECORE_POLLER_BASE_ID, op_desc, ECORE_POLLER_SUB_ID_LAST),
     NULL,
     sizeof(Ecore_Poller_Private_Data),
     _class_constructor,
     NULL
};

EO_DEFINE_CLASS(ecore_poller_class_get, &class_desc, EO_BASE_CLASS, NULL)
