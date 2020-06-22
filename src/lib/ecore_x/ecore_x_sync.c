/*
 * XSync code
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* ifdef HAVE_CONFIG_H */

#include "Ecore.h"
#include "ecore_x_private.h"
#include "Ecore_X.h"
#include "Ecore_X_Atoms.h"

EAPI Ecore_X_Sync_Alarm
ecore_x_sync_alarm_new(Ecore_X_Sync_Counter counter)
{
   Ecore_X_Sync_Alarm alarm;
   XSyncAlarmAttributes values;
   XSyncValue init;

   LOGFN;
   XSyncIntToValue(&init, 0);
   XSyncSetCounter(_ecore_x_disp, counter, init);

   values.trigger.counter = counter;
   values.trigger.value_type = XSyncAbsolute;
   XSyncIntToValue(&values.trigger.wait_value, 1);
   values.trigger.test_type = XSyncPositiveComparison;

   XSyncIntToValue(&values.delta, 1);

   values.events = True;

   alarm = XSyncCreateAlarm(_ecore_x_disp,
                            XSyncCACounter |
                            XSyncCAValueType |
                            XSyncCAValue |
                            XSyncCATestType |
                            XSyncCADelta |
                            XSyncCAEvents,
                            &values);

   ecore_x_sync();
   return alarm;
}

EAPI Eina_Bool
ecore_x_sync_alarm_free(Ecore_X_Sync_Alarm alarm)
{
   LOGFN;
   return XSyncDestroyAlarm(_ecore_x_disp, alarm);
}

EAPI Eina_Bool
ecore_x_sync_counter_query(Ecore_X_Sync_Counter counter,
                           unsigned int *val)
{
   XSyncValue value;

   LOGFN;
   if (XSyncQueryCounter(_ecore_x_disp, counter, &value))
     {
        *val = (unsigned int)XSyncValueLow32(value);
        return EINA_TRUE;
     }

   return EINA_FALSE;
}

EAPI Ecore_X_Sync_Counter
ecore_x_sync_counter_new(int val)
{
   XSyncCounter counter;
   XSyncValue v;

   LOGFN;
   XSyncIntToValue(&v, val);
   counter = XSyncCreateCounter(_ecore_x_disp, v);
   return counter;
}

EAPI void
ecore_x_sync_counter_free(Ecore_X_Sync_Counter counter)
{
   LOGFN;
   XSyncDestroyCounter(_ecore_x_disp, counter);
}

EAPI void
ecore_x_sync_counter_inc(Ecore_X_Sync_Counter counter,
                         int by)
{
   XSyncValue v;

   LOGFN;
   XSyncIntToValue(&v, by);
   XSyncChangeCounter(_ecore_x_disp, counter, v);
}

EAPI void
ecore_x_sync_counter_val_wait(Ecore_X_Sync_Counter counter,
                              int val)
{
   XSyncWaitCondition cond;
   XSyncValue v, v2;

   LOGFN;
   XSyncQueryCounter(_ecore_x_disp, counter, &v);
   XSyncIntToValue(&v, val);
   XSyncIntToValue(&v2, val + 1);
   cond.trigger.counter = counter;
   cond.trigger.value_type = XSyncAbsolute;
   cond.trigger.wait_value = v;
   cond.trigger.test_type = XSyncPositiveComparison;
   cond.event_threshold = v2;
   XSyncAwait(_ecore_x_disp, &cond, 1);
//   XSync(_ecore_x_disp, False); // dont need this
}

EAPI void
ecore_x_sync_counter_set(Ecore_X_Sync_Counter counter,
                         int val)
{
   XSyncValue v;

   LOGFN;
   XSyncIntToValue(&v, val);
   XSyncSetCounter(_ecore_x_disp, counter, v);
}

EAPI void
ecore_x_sync_counter_2_set(Ecore_X_Sync_Counter counter,
                           int val_hi,
                           unsigned int val_lo)
{
   XSyncValue v;

   LOGFN;
   XSyncIntsToValue(&v, val_lo, val_hi);
   XSyncSetCounter(_ecore_x_disp, counter, v);
}

EAPI Eina_Bool
ecore_x_sync_counter_2_query(Ecore_X_Sync_Counter counter,
                             int *val_hi,
                             unsigned int *val_lo)
{
   XSyncValue value;

   LOGFN;
   if (XSyncQueryCounter(_ecore_x_disp, counter, &value))
     {
        *val_lo = (unsigned int)XSyncValueLow32(value);
        *val_hi = (int)XSyncValueHigh32(value);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

