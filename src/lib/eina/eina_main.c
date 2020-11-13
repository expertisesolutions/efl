/* EINA - EFL data type library
 * Copyright (C) 2008 Cedric Bail
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <time.h>

#ifdef EFL_HAVE_THREADS
# ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
# endif
# ifndef _MSC_VER
#  include <unistd.h>
# endif
#endif

#ifdef HAVE_MCHECK_H
# ifdef HAVE_MTRACE
#  define MT 1
# endif
#endif

#ifdef MT
#include <mcheck.h>
#endif

#include "eina_lock.h"
#include "eina_thread.h"
#include "eina_config.h"
#include "eina_private.h"
#include "eina_types.h"
#include "eina_main.h"
#include "eina_error.h"
#include "eina_log.h"
#include "eina_hash.h"
#include "eina_binshare.h"
#include "eina_stringshare.h"
#include "eina_ustringshare.h"
#include "eina_list.h"
#include "eina_matrixsparse.h"
#include "eina_array.h"
#include "eina_benchmark.h"
#include "eina_magic.h"
#include "eina_rectangle.h"
#include "eina_safety_checks.h"
#include "eina_inlist.h"
#include "eina_inarray.h"
#include "eina_value.h"
#include "eina_evlog.h"
#include "eina_freeq.h"
#include "eina_slstr.h"
#include "eina_vpath.h"
#include "eina_thread.h"
#include "eina_fnmatch.h"

/*============================================================================*
*                                  Local                                     *
*============================================================================*/

/**
 * @cond LOCAL
 */

static Eina_Version _version = { VMAJ, VMIN, VMIC, VREV };

static int _eina_main_count = 0;
#ifdef EFL_HAVE_THREADS
static int _eina_main_thread_count = 0;
#endif
static int _eina_log_dom = -1;

#ifdef ERR
#undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_eina_log_dom, __VA_ARGS__)

#ifdef DBG
#undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_eina_log_dom, __VA_ARGS__)

EINA_API Eina_Bool _eina_threads_activated = EINA_FALSE;
EINA_API Eina_Error EINA_ERROR_NOT_MAIN_LOOP = 0;
EINA_API Eina_Error EINA_ERROR_NOT_IMPLEMENTED = 0;
EINA_API unsigned int eina_seed = 0;

#ifdef EFL_HAVE_THREADS
EINA_API Eina_Thread _eina_main_loop;
#endif

#ifdef MT
static int _mt_enabled = 0;
#endif

#ifdef EFL_HAVE_THREADS
EINA_API int _eina_threads_debug = 0;
EINA_API Eina_Lock _eina_tracking_lock;
EINA_API Eina_Inlist *_eina_tracking = NULL;
extern Eina_Lock       _sysmon_lock;
#endif

/* place module init/shutdown functions here to avoid other modules
 * calling them by mistake.
 */
#define S(x) extern Eina_Bool eina_ ## x ## _init(void); \
   extern Eina_Bool eina_ ## x ## _shutdown(void)
   S(debug);
   S(evlog);
   S(log);
   S(error);
   S(safety_checks);
   S(magic_string);
   S(iterator);
   S(accessor);
   S(inarray);
   S(array);
   S(module);
   S(mempool);
   S(list);
   S(binshare);
   S(stringshare);
   S(ustringshare);
   S(matrixsparse);
   S(convert);
   S(benchmark);
   S(rectangle);
   S(strbuf);
   S(ustrbuf);
   S(quadtree);
   S(simple_xml);
   S(prefix);
   S(value);
   S(tmpstr);
   S(thread);
   S(cow);
   S(cpu);
   S(thread_queue);
   S(rbtree);
   S(file);
   S(safepointer);
   S(slstr);
   S(promise);
   S(vpath);
   S(abstract_content);
#undef S

struct eina_desc_setup
{
   const char *name;
   Eina_Bool (*init)(void);
   Eina_Bool (*shutdown)(void);
};

static const struct eina_desc_setup _eina_desc_setup[] = {
#define S(x) {# x, eina_ ## x ## _init, eina_ ## x ## _shutdown}
   /* log is a special case as it needs printf */
   S(module),
   S(mempool),
   S(list),
   S(stringshare),
   S(vpath),
   S(debug),
   S(evlog),
   S(error),
   S(safety_checks),
   S(magic_string),
   S(iterator),
   S(accessor),
   S(inarray),
   S(array),
   S(binshare),
   S(ustringshare),
   S(matrixsparse),
   S(convert),
   S(benchmark),
   S(rectangle),
   S(strbuf),
   S(ustrbuf),
   S(quadtree),
   S(simple_xml),
   S(prefix),
   S(value),
   S(tmpstr),
   S(thread),
   S(cow),
   S(cpu),
   S(thread_queue),
   S(rbtree),
   S(file),
   S(safepointer),
   S(slstr),
   S(promise),
   S(abstract_content)
#undef S
};
static const size_t _eina_desc_setup_len = sizeof(_eina_desc_setup) /
   sizeof(_eina_desc_setup[0]);

static void
_eina_shutdown_from_desc(const struct eina_desc_setup *itr)
{
   for (itr--; itr >= _eina_desc_setup; itr--)
     {
        if (!itr->shutdown())
           ERR("Problems shutting down eina module '%s', ignored.", itr->name);
     }

   eina_log_domain_unregister(_eina_log_dom);
   _eina_log_dom = -1;
   eina_log_shutdown();
}

static void
_eina_threads_do_shutdown(void)
{
#ifdef EINA_HAVE_DEBUG_THREADS
   const Eina_Lock *lk;

   eina_lock_take(&_eina_tracking_lock);
   if (_eina_tracking)
     {
       if (((Eina_Lock*)_eina_tracking != (&_sysmon_lock)) || (_eina_tracking->next))
         {
            fprintf(stderr, "*************************\n");
            fprintf(stderr, "* The IMPOSSIBLE HAPPEN *\n");
            fprintf(stderr, "* LOCK STILL TAKEN :    *\n");
            fprintf(stderr, "*************************\n");
            EINA_INLIST_FOREACH(_eina_tracking, lk)
              {
                 fprintf(stderr, "=======\n");
                 eina_lock_debug(lk);
              }
            fprintf(stderr, "*************************\n");
            abort();
         }
     }
   eina_lock_release(&_eina_tracking_lock);
#endif

   eina_share_common_threads_shutdown();
   eina_log_threads_shutdown();

   _eina_threads_activated = EINA_FALSE;
}

/**
 * @endcond
 */

/*============================================================================*
*                                 Global                                     *
*============================================================================*/


/*============================================================================*
*                                   API                                      *
*============================================================================*/

/**
 * @var eina_version
 * @brief Eina version (defined at configuration time)
 */
EINA_API Eina_Version *eina_version = &_version;

#ifdef EFL_EXACTNESS_WIN32
typedef int(*eina_init_t)(void);
EINA_API eina_init_t eina_init_redirect = NULL;
EINA_API int eina_init_original(void);

EINA_API int 
eina_init(void)
{
   exactness_preload();
   if (eina_init_redirect)
     return eina_init_redirect();
   else
     return eina_init_original();
}

EINA_API int 
eina_init_original(void)
#else
EINA_API int
eina_init(void)
#endif
{
   fprintf(stderr, "\n >>> %s:%s <<< \n", __FILE__, __func__);
   const struct eina_desc_setup *itr, *itr_end;

   if (EINA_LIKELY(_eina_main_count > 0))
      return ++_eina_main_count;

   srand(time(NULL));
   while (eina_seed == 0)
     eina_seed = rand();

#ifdef MT
   if ((getenv("EINA_MTRACE")) && (getenv("MALLOC_TRACE")))
     {
        _mt_enabled = 1;
        mtrace();
     }
#endif

#ifdef EFL_HAVE_THREADS
   _eina_main_loop = eina_thread_self();
#endif

   eina_freeq_main_set(eina_freeq_new(EINA_FREEQ_DEFAULT));

   if (!eina_log_init())
     {
        fprintf(stderr, "Could not initialize eina logging system.\n");
        return 0;
     }

   _eina_log_dom = eina_log_domain_register("eina", EINA_LOG_COLOR_DEFAULT);
   if (_eina_log_dom < 0)
     {
        EINA_LOG_ERR("Could not register log domain: eina");
        eina_log_shutdown();
        return 0;
     }

#ifdef EINA_HAVE_DEBUG_THREADS
   eina_lock_new(&_eina_tracking_lock);

   if (getenv("EINA_DEBUG_THREADS"))
     _eina_threads_debug = atoi(getenv("EINA_DEBUG_THREADS"));
#endif

   itr = _eina_desc_setup;
   itr_end = itr + _eina_desc_setup_len;
   for (; itr < itr_end; itr++)
     {
        if (!itr->init())
          {
             ERR("Could not initialize eina module '%s'.", itr->name);
             _eina_shutdown_from_desc(itr);
             return 0;
          }
     }

   eina_cpu_count_internal();

   EINA_ERROR_NOT_MAIN_LOOP = eina_error_msg_static_register("Not in main loop.");
   EINA_ERROR_NOT_IMPLEMENTED = eina_error_msg_static_register("Functionality not implemented.");

   eina_log_timing(_eina_log_dom, EINA_LOG_STATE_STOP, EINA_LOG_STATE_INIT);

   _eina_main_count = 1;
   eina_evlog("-eina_init", NULL, 0.0, NULL);
   return 1;
}

#ifdef EFL_EXACTNESS_WIN32
EINA_API eina_shutdown_t eina_shutdown_redirect = NULL;

EINA_API int 
eina_shutdown(void)
{
   exactness_preload();
   if (eina_shutdown_redirect)
     return eina_shutdown_redirect();
   else
     return eina_shutdown_original();
}

EINA_API int
eina_shutdown_original(void)
#else
EINA_API int
eina_shutdown(void)
#endif
{
   fprintf(stderr, "\n >>> %s:%s (original) <<< \n", __FILE__, __func__);
   if (_eina_main_count <= 0)
     {
        ERR("Init count not greater than 0 in shutdown.");
        return 0;
     }
   _eina_main_count--;
   if (EINA_UNLIKELY(_eina_main_count == 0))
     {
        eina_log_timing(_eina_log_dom,
                        EINA_LOG_STATE_START,
                        EINA_LOG_STATE_SHUTDOWN);

        _eina_shutdown_from_desc(_eina_desc_setup + _eina_desc_setup_len);

        if (_eina_threads_activated && (!_eina_main_thread_count))
          {
          _eina_threads_do_shutdown();
          }
#ifdef EINA_HAVE_DEBUG_THREADS
	eina_lock_free(&_eina_tracking_lock);
#endif
        eina_freeq_free(eina_freeq_main_get());
#ifdef MT
        if (_mt_enabled)
          {
             muntrace();
             _mt_enabled = 0;
          }
#endif
     }

   return _eina_main_count;
}


EINA_API int
eina_threads_init(void)
{
#ifdef EFL_HAVE_THREADS
   int ret;
# ifdef EINA_HAVE_DEBUG_THREADS
   assert(eina_thread_equal(_eina_main_loop, eina_thread_self()));
# endif

   ++_eina_main_thread_count;
   ret = _eina_main_thread_count;

   if(_eina_main_thread_count > 1)
     return ret;

   eina_share_common_threads_init();
   eina_log_threads_init();
   _eina_threads_activated = EINA_TRUE;

   return ret;
#else
   return 0;
#endif
}

EINA_API int
eina_threads_shutdown(void)
{
#ifdef EFL_HAVE_THREADS
   int ret;

# ifdef EINA_HAVE_DEBUG_THREADS
   assert(eina_thread_equal(_eina_main_loop, eina_thread_self()));
   assert(_eina_main_thread_count > 0);
# endif

   ret = --_eina_main_thread_count;
   if(_eina_main_thread_count > 0)
     return ret;

   if (!_eina_main_count)
     _eina_threads_do_shutdown();

   return ret;
#else
   return 0;
#endif
}

EINA_API Eina_Bool
eina_main_loop_is(void)
{
#ifdef EFL_HAVE_THREADS
# ifdef __GNUC__
   /* pthread_self() can't be optimized, it's a single asm "movl" */
   if (__builtin_types_compatible_p(Eina_Thread, unsigned long int))
     return (eina_thread_self() == _eina_main_loop);
   else
# endif
   if (eina_thread_equal(_eina_main_loop, eina_thread_self()))
     return EINA_TRUE;
#endif
   return EINA_FALSE;
}

/** The purpose of this API should not be documented, it is used only by the one who know what they are doing. */
EINA_API void
eina_main_loop_define(void)
{
#ifdef EFL_HAVE_THREADS
   _eina_main_loop = eina_thread_self();
#endif
}

