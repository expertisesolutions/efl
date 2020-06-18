/* EINA - EFL data type library
 * Copyright (C) 2010 ProFUSION embedded systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#include "eina_sched.h"
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <pthread.h>

void
eina_sched_prio_drop(void)
{
   struct sched_param param;
   int pol, ret;
   Eina_Thread pthread_id;

   pthread_id = eina_thread_self();
   ret = pthread_getschedparam(pthread_id, &pol, &param);
   if (ret)
     {
        EINA_LOG_ERR("Unable to query sched parameters");
        return;
     }

   if (EINA_UNLIKELY(pol == SCHED_RR || pol == SCHED_FIFO))
     {
        param.sched_priority -= RTNICENESS;

        /* We don't change the policy */
        if (param.sched_priority < 1)
          {
             EINA_LOG_INFO("RT prio < 1, setting to 1 instead");
             param.sched_priority = 1;
          }

        pthread_setschedparam(pthread_id, pol, &param);
     }
   else
     {
        int prio;
        errno = 0;
        prio = getpriority(PRIO_PROCESS, 0);
        if (errno == 0)
          { 
             prio += NICENESS;
             if (prio > 19)
               {
                  EINA_LOG_INFO("Max niceness reached; keeping max (19)");
                  prio = 19;
               }
             setpriority(PRIO_PROCESS, 0, prio);
          }
     }
}

void
eina_sched_prio_init(Eina_Thread_Call* c)
{
   Eina_Thread self = eina_thread_self();

   if (c->prio == EINA_THREAD_IDLE)
     {
        struct sched_param params;
        int min;
#ifdef SCHED_IDLE
        int pol = SCHED_IDLE;
#else
        int pol;
        pthread_getschedparam(self, &pol, &params);
#endif
        min = sched_get_priority_min(pol);
        params.sched_priority = min;
        pthread_setschedparam(self, pol, &params);
     }
   else if (c->prio == EINA_THREAD_BACKGROUND)
     {
        struct sched_param params;
        int min, max;
#ifdef SCHED_BATCH
        int pol = SCHED_BATCH;
#else
        int pol;
        pthread_getschedparam(self, &pol, &params);
#endif
        min = sched_get_priority_min(pol);
        max = sched_get_priority_max(pol);
        params.sched_priority = (max - min) / 2;
        pthread_setschedparam(self, pol, &params);
     }
// do nothing for normal
//   else if (c->prio == EINA_THREAD_NORMAL)
//     {
//     }
   else if (c->prio == EINA_THREAD_URGENT)
     {
        struct sched_param params;
        int max, pol;

        pthread_getschedparam(self, &pol, &params);
        max = sched_get_priority_max(pol);
        params.sched_priority += 5;
        if (params.sched_priority > max) params.sched_priority = max;
        pthread_setschedparam(self, pol, &params);
     }
}
