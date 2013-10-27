#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "eo_simple.h"
#include "eo_inherit.h"
#include "eo_composite.h"
#include "eo2_simple.h"
#include "eo3_simple.h"
#include "eo2_inherit.h"
#include "eo3_inherit.h"
#include "eo2_composite.h"

static void report(struct timespec t0, struct timespec t1,
                   struct timespec t2, struct timespec t3,
                   struct timespec t4, struct timespec t5, int n, int c)
{
   uint64_t dt0, dt1, dt2;

   dt0 = ((t1.tv_sec * 1000000000ULL) + t1.tv_nsec) -
      ((t0.tv_sec * 1000000000ULL) + t0.tv_nsec);
   dt1 = ((t3.tv_sec * 1000000000ULL) + t3.tv_nsec) -
      ((t2.tv_sec * 1000000000ULL) + t2.tv_nsec);
   dt2 = ((t5.tv_sec * 1000000000ULL) + t5.tv_nsec) -
      ((t4.tv_sec * 1000000000ULL) + t4.tv_nsec);

   printf("   #%d              %3u %3u                %3u %3u                %3u %3u\n",
          n, (unsigned int)(dt0/1000000), (unsigned int)(dt0/c),
          (unsigned int)(dt1/1000000),(unsigned int)(dt1/c),
          (unsigned int)(dt2/1000000),(unsigned int)(dt2/c));
}

#define check(val, ex) do_check(val, ex, __LINE__)
static void do_check(int val, int expected, int line)
{
   static int count = 0;
   count++;
   if (val != expected)
     {
        fprintf(stderr, "check line %d #%d failed %d != %d\n", line, count, val, expected);
        exit(count);
     }
}

#define EO_RUN_START                      \
   eo_do(eo_obj, eo_set(0));              \
   clock_gettime(CLOCK_MONOTONIC, &t0);

#define EO_RUN_END                        \
   clock_gettime(CLOCK_MONOTONIC, &t1);   \
   eo_do(eo_obj, eo_get(&v));             \
   check(v, n * k);                       \

#define EO2_RUN_START                     \
   eo2_do(eo2_obj, eo2_set(0); );         \
   clock_gettime(CLOCK_MONOTONIC, &t2);   \

#define EO2_RUN_END                       \
   clock_gettime(CLOCK_MONOTONIC, &t3);   \
   eo2_do(eo2_obj, v = eo2_get(); );      \
   check(v, n * k);                       \

#define EO3_RUN_START                     \
   eo2_do(eo3_obj, simple_set(0); );         \
   clock_gettime(CLOCK_MONOTONIC, &t4);   \

#define EO3_RUN_END                       \
   clock_gettime(CLOCK_MONOTONIC, &t5);   \
   eo2_do(eo3_obj, v = simple_get(); );      \
   check(v, n * k);                       \

static void
run_batch(const char *title, Eo* eo_obj, Eo* eo2_obj, Eo* eo3_obj, int n)
{
   int i, k, v;
   Eina_Bool eo1;
   struct timespec t0, t1, t2, t3, t4, t5;

   if (getenv("EO2ONLY") != NULL)
     {
        eo1 = EINA_FALSE;
        t0.tv_sec = t1.tv_sec = 0;
        t0.tv_nsec = t1.tv_nsec = 0;
     }
   else
     eo1 = EINA_TRUE;

   printf("%s - %d calls\ncalls/eo_do()  EO [ms] [ns]/call  - EO2 [ms] [ns]/call\n", title, n);

   /* 1 call per batch */
   k = 1;
   if (eo1)
     {
        EO_RUN_START
           for (i = 0; i < n; i++)
             eo_do(eo_obj, eo_inc());
        EO_RUN_END
     }
   EO2_RUN_START
   for (i = 0; i < n; i++)
     eo2_do(eo2_obj, eo2_inc(); );
   EO2_RUN_END
   EO3_RUN_START
   for (i = 0; i < n; i++)
     eo2_do(eo3_obj, simple_inc(); );
   EO3_RUN_END
   report(t0, t1, t2, t3, t4, t5, k, n * k);

   /* 3 calls per batch */
   k = 3;
   if (eo1)
     {
        EO_RUN_START
           for (i = 0; i < n; i++)
             eo_do(eo_obj, eo_inc(), eo_inc(), eo_inc());
        EO_RUN_END
     }
   EO2_RUN_START
   for (i = 0; i < n; i++)
     eo2_do(eo2_obj, eo2_inc(); eo2_inc(); eo2_inc(); );
   EO2_RUN_END
   EO3_RUN_START
   for (i = 0; i < n; i++)
     eo2_do(eo3_obj, simple_inc(); simple_inc(); simple_inc(); );
   EO3_RUN_END
   report(t0, t1, t2, t3, t4, t5, k, n * k);

   /* 5 calls per batch */
   k = 5;
   if (eo1)
     {
        EO_RUN_START
           for (i = 0; i < n; i++)
             eo_do(eo_obj, eo_inc(), eo_inc(), eo_inc(), eo_inc(), eo_inc());
        EO_RUN_END
     }
   EO2_RUN_START
   for (i = 0; i < n; i++)
     eo2_do(eo2_obj, eo2_inc(); eo2_inc(); eo2_inc(); eo2_inc(); eo2_inc(); );
   EO2_RUN_END
   EO3_RUN_START
   for (i = 0; i < n; i++)
     eo2_do(eo3_obj, simple_inc();simple_inc();simple_inc();simple_inc();simple_inc(); );
   EO3_RUN_END
   report(t0, t1, t2, t3, t4, t5, k, n * k);

   /* 7 calls per batch */
   k = 7;
   if (eo1)
     {
        EO_RUN_START
           for (i = 0; i < n; i++)
             eo_do(eo_obj, eo_inc(), eo_inc(), eo_inc(), eo_inc(), eo_inc(), eo_inc(), eo_inc());
        EO_RUN_END
     }
   EO2_RUN_START
   for (i = 0; i < n; i++)
     eo2_do(eo2_obj, eo2_inc(); eo2_inc(); eo2_inc(); eo2_inc(); eo2_inc(); eo2_inc(); eo2_inc(); );
   EO2_RUN_END
   EO3_RUN_START
   for (i = 0; i < n; i++)
     eo2_do(eo3_obj, simple_inc();simple_inc();simple_inc();simple_inc();simple_inc();simple_inc();simple_inc(); );
   EO3_RUN_END
   report(t0, t1, t2, t3, t4, t5, k, n * k);
}

static void
do_batch_test()
{
   int a, b, c;
   Eo *eo_obj, *eo2_obj, *eo3_obj;

   printf("\n *** do_batch_test\n");

   eo_obj = eo_add(EO_SIMPLE_CLASS, NULL);
   eo2_obj = eo2_add_custom(EO2_SIMPLE_CLASS, NULL, eo2_simple_constructor(66));
   eo3_obj = eo2_add_custom(EO3_GET_CLASS(EO3_SIMPLE_CLASS), NULL, /*eo2_*/simple_constructor(66));

   /* EO check */
   a = b = c = 0;
   eo_do(eo_obj, eo_get(&a), eo_set(10), eo_inc(), eo_get(&b), eo_inc(), eo_inc(), eo_get(&c));
   check(a, 66);
   check(b, 11);
   check(c, 13);
   /* EO2 check */
   a = b = c = 0;
   eo2_do(eo2_obj,
          a = eo2_get();
          eo2_set(10);
          eo2_inc();
          b = eo2_get();
          eo2_inc();
          eo2_inc();
          c = eo2_get();
          );
   check(a, 66);
   check(b, 11);
   check(c, 13);
   /* EO3 check */
   a = b = c = 0;
   eo2_do(eo3_obj,
          a = simple_get();
          simple_set(10);
          simple_inc();
          b = simple_get();
          simple_inc();
          simple_inc();
          c = simple_get();
          );
   check(a, 66);
   check(b, 11);
   check(c, 13);

   run_batch("simple inc()", eo_obj, eo2_obj, eo3_obj, /*99999*/1);

   eo_del(eo_obj);
   eo_del(eo2_obj);
   eo_del(eo3_obj);
}

static void
override_batch_test()
{
   int a, b;
   Eo *eo_obj, *eo2_obj, *eo3_obj;

   printf("\n *** override_batch_test\n");

   a = b = 0;
   eo_obj = eo_add(EO_INHERIT_CLASS, NULL);
   eo2_obj = eo2_add(EO2_INHERIT_CLASS, NULL);
   eo3_obj = eo2_add(EO3_GET_CLASS(EO3_INHERIT_CLASS), NULL);

   /* EO check */
   eo_do(eo_obj, eo_set(65), eo_get(&a), eo_inherit_get(&b));
   check(a, 65);
   check(b, 68);
   eo_do(eo_obj, eo_inc(), eo_get(&a), eo_inherit_get(&b));
   check(a, 66);
   check(b, 69);

   /* EO2 check */
   eo2_do(eo2_obj, eo2_set(65); a = eo2_get(); b = eo2_inherit_get(); );
   check(a, 65);
   check(b, 68);
   eo2_do(eo2_obj, eo2_inc(); a = eo2_get(); b = eo2_inherit_get(); );
   check(a, 66);
   check(b, 69);

   /* EO3 check */
   eo2_do(eo3_obj, simple_set(65); a = simple_get(); b = eo3_inherit_get(); );
   check(a, 65);
   check(b, 68);
   eo2_do(eo3_obj, simple_inc(); a = simple_get(); b = eo3_inherit_get(); );
   check(a, 66);
   check(b, 69);

   run_batch("overriden inc", eo_obj, eo2_obj, eo3_obj, 99999);

   eo_del(eo_obj);
   eo_del(eo2_obj);
}

static void
virtual_test()
{
   int a;
   Eo *eo2_obj;

   printf("\n *** virtual_test\n");

   a = 0;
   eo2_obj = eo2_add_custom(EO2_SIMPLE_CLASS, NULL, eo2_simple_constructor(66));
   eo2_do(eo2_obj, a = eo2_virtual(10); );
   check(a, -1);
   eo_del(eo2_obj);

   a = 0;
   eo2_obj = eo2_add(EO2_INHERIT_CLASS, NULL);
   eo2_do(eo2_obj, a = eo2_virtual(10); );
   check(a, 20);
   eo_del(eo2_obj);
}

static void inner_return(Eo *eo2_obj)
{
   eo2_do(eo2_obj,
          eo2_set(0);
          return;
          eo2_inc();
          );
}

static int
cleanup_test()
{
   int a;
   Eo *eo2_obj;

   printf("\n *** cleanup_test\n");

   eo2_obj = eo2_add_custom(EO2_SIMPLE_CLASS, NULL, eo2_simple_constructor(66));

   /* break */
   a = 0;
   check(eo2_call_stack_depth(), 0);
   eo2_do(eo2_obj,
          eo2_set(0);
          check(eo2_call_stack_depth(), 1);
          a = eo2_get();
          break;
          eo2_inc();
          a = eo2_get();
          );
   check(a, 0);
   check(eo2_call_stack_depth(), 0);

   /* return */
   a = 0;
   check(eo2_call_stack_depth(), 0);
   inner_return(eo2_obj);
   eo2_do(eo2_obj, a = eo2_get(); );
   check(a, 0);
   check(eo2_call_stack_depth(), 0);

   /* goto */
   a = 0;
   check(eo2_call_stack_depth(), 0);
   eo2_do(eo2_obj,
          eo2_set(0);
          a = eo2_get();
          check(eo2_call_stack_depth(), 1);
          goto check;
          eo2_inc();
          a = eo2_get();
          );

check:
   check(a, 0);
   check(eo2_call_stack_depth(), 0);

   /* wrong object */
   check(eo2_call_stack_depth(), 0);
#ifdef HAVE_EO_ID
   // segfault if eo2_do_end is called !!
   eo2_do((Eo *)((uintptr_t)eo2_obj + 666), eo2_set(0););
#endif

   eo_del(eo2_obj);

   return 0;
}

static int evt_count = 0;
static Eina_Bool
_changed_cb(void *data, Eo *obj, const Eo_Event_Description *desc, void *event_info)
{
   (void) data;
   (void) desc;
   (void) obj;
   int val = *((int *) event_info);
   check(val, 66);
   evt_count++;

   return EO_CALLBACK_CONTINUE;
}

static void
event_test()
{
   Eo *eo_obj, *eo2_obj;

   printf("\n *** event_test\n");

   eo_obj = eo_add(EO_SIMPLE_CLASS, NULL);
   eo2_obj = eo2_add(EO2_SIMPLE_CLASS, NULL);

   evt_count = 0;
   eo_do(eo_obj, eo_event_callback_add(EO_EV_X_CHANGED, _changed_cb, NULL));
   eo_do(eo_obj, eo_set_evt(66));
   check(evt_count, 1);

   evt_count = 0;
   eo2_do(eo2_obj, eo2_event_callback_add(EO2_EV_X_CHANGED, _changed_cb, NULL));
   eo2_do(eo2_obj, eo2_set_evt(66));
   check(evt_count, 1);

   eo_del(eo_obj);
   eo_del(eo2_obj);
}

static void
composite_test()
{
   Eo *eo_obj, *eo2_obj;
   Eo *simple, *simple2;
   int a;

   /* EO */
   eo_obj = eo_add(EO_COMPOSITE_CLASS, NULL);
   check(eo_composite_is(eo_obj), 0);

   eo_do(eo_obj, eo_base_data_get("simple-obj", (void **) &simple));
   eo_ref(simple);
   check(eo_composite_is(simple), 1);

   eo_do(eo_obj, eo_event_callback_add(EO_EV_X_CHANGED, _changed_cb, NULL));

   evt_count = 0;
   eo_do(eo_obj, eo_set_evt(66));
   check(evt_count, 1);

   eo_do(eo_obj, eo_get(&a));
   check(a, 66);

   /* disable the callback forwarder, and fail if it's still called. */
   eo_do(simple, eo_event_callback_forwarder_del(EO_EV_X_CHANGED, eo_obj));

   evt_count = 0;
   eo_do(eo_obj, eo_set_evt(66));
   check(evt_count, 0);

   check(eo_composite_is(simple), 1);
   eo_composite_detach(simple, eo_obj);
   check(eo_composite_is(simple), 0);
   eo_composite_attach(simple, eo_obj);
   check(eo_composite_is(simple), 1);

   eo_unref(simple);
   eo_unref(eo_obj);

   /* EO2 */
   simple2 = NULL;
   eo2_obj = eo2_add(EO2_COMPOSITE_CLASS, NULL);
   check(eo_composite_is(eo2_obj), 0);

   eo2_do(eo2_obj, simple2 = eo2_base_data_get("simple-obj"));
   eo_ref(simple2);
   check(eo_composite_is(simple2), 1);

   eo2_do(eo2_obj, eo2_event_callback_add(EO2_EV_X_CHANGED, _changed_cb, NULL));

   evt_count = 0;
   eo2_do(eo2_obj, eo2_set_evt(66));
   check(evt_count, 1);

   eo2_do(eo2_obj, eo2_get(&a));
   check(a, 66);

   /* disable the callback forwarder, and fail if it's still called. */
   eo2_do(simple2, eo2_event_callback_forwarder_del(EO2_EV_X_CHANGED, eo2_obj));

   evt_count = 0;
   eo2_do(eo2_obj, eo2_set_evt(66));
   check(evt_count, 0);

   check(eo_composite_is(simple2), 1);
   eo_composite_detach(simple2, eo2_obj);
   check(eo_composite_is(simple2), 0);
   eo_composite_attach(simple2, eo2_obj);
   check(eo_composite_is(simple2), 1);

   eo_unref(simple2);
   eo_unref(eo2_obj);
}

static void
class_do_test()
{
   printf("\n *** class_do_test\n");

   eo2_do(EO2_INHERIT_CLASS, eo2_class_hello(2); );
}

int
main(int argc EINA_UNUSED, char** argv EINA_UNUSED, char** env EINA_UNUSED)
{
   eo_init();

   if (argc > 1)
     eina_log_domain_level_set("eo", EINA_LOG_LEVEL_DBG);

   if(cleanup_test())
     fprintf(stderr, "something went wrong in cleanup_test()\n");

   class_do_test();

   do_batch_test();

   override_batch_test();

   virtual_test();

   event_test();

   composite_test();

   eo_shutdown();

   return 0;
}

