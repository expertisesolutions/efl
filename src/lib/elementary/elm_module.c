#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "../../static_libs/buildsystem/buildsystem.h"
#include "elm_priv.h"

/* what are moodules in elementary for? for modularising behavior and features
 * so they can be plugged in and out where you dont want the core source to
 * always behave like that or do it that way. plug it at runtime!
 *
 * they have module names (in config) and "slots" to plug that module into
 * to server a purpose. eg you plug plugin "xx" into the "entry-copy-paste"
 * slot so it would provide replacement copy & paste ui functionality and
 * specific symbols
 *
 * config is something like:
 *
 * export ELM_MODULES="xx>slot1:yy>slot2"
 *
 * where a module named xx is plugged into slot1 & yy is plugged into slot2
 *
 * real examples:
 *
 * export ELM_MODULES="my_module>entry/api"
 *
 * this loads the module called "my_module" into the slot "entry/api" which
 * is an api slot for entry modules to modify behavior and hook to
 * creation/deletion of the entry as well as replace the longpress behavior.
 */
static Eina_Hash *modules = NULL;
static Eina_Hash *modules_as = NULL;

void
_elm_module_init(void)
{
   modules = eina_hash_string_small_new(NULL);
   modules_as = eina_hash_string_small_new(NULL);
}

void
_elm_module_shutdown(void)
{
   Eina_Iterator *it;
   Elm_Module *m;

   if (modules)
     {
        Eina_List *tl = NULL;

        it = eina_hash_iterator_data_new(modules);

        EINA_ITERATOR_FOREACH(it, m) tl = eina_list_append(tl, m);
        eina_iterator_free(it);

        EINA_LIST_FREE(tl, m) _elm_module_del(m);

        ELM_SAFE_FREE(modules, eina_hash_free);
     }

   ELM_SAFE_FREE(modules_as, eina_hash_free);
}

void
_elm_module_parse(const char *s)
{
   const char *p, *pe;

   p = s;
   pe = p;
   for (;;)
     {
        if ((*pe == ':') || (!*pe))
          { // p -> pe == 'name:'
             if (pe > p)
               {
                  char *n = malloc(pe - p + 1);
                  if (n)
                    {
                       char *nn;

                       strncpy(n, p, pe - p);
                       n[pe - p] = 0;
                       nn = strchr(n, '>');
                       if (nn)
                         {
                            *nn = 0;
                            nn++;
                            _elm_module_add(n, nn);
                         }
                       free(n);
                    }
               }
             if (!*pe) break;
             p = pe + 1;
             pe = p;
          }
        else
          pe++;
     }
}

Elm_Module *
_elm_module_find_as(const char *as)
{
   Elm_Module *m;

   m = eina_hash_find(modules_as, as);
   if (!m) return NULL;

   if (!_elm_module_load(m))
     {
        ERR("Failed to load elementary module: '%s': %m", m->as);
        _elm_module_del(m);
        return NULL;
     }
   return m;
}

#if defined(_WIN32) || defined(__CYGWIN__)
# define EFL_SHARED_EXTENSION ".dll"
#else
# define EFL_SHARED_EXTENSION ".so"
#endif

Eina_Bool
_elm_module_load(Elm_Module *m)
{
   char buf[PATH_MAX];

   if (m->module) return EINA_TRUE;
   if (strchr(m->name, '/')) return EINA_FALSE;

#ifdef NEED_RUN_IN_TREE
   if (getenv("EFL_RUN_IN_TREE"))
     {
#ifndef _MSC_VER
        bs_mod_get(buf, sizeof(buf), "elementary", m->name);
#else
        snprintf(buf, sizeof(buf), PACKAGE_BUILD_DIR
			"src/modules/elementary/%s/module"EFL_SHARED_EXTENSION,
			m->name);
#endif
     }
   else
#endif
     {
        snprintf(buf, sizeof(buf), "%s/elementary/modules/%s/%s/module"EFL_SHARED_EXTENSION,
            _elm_lib_dir, m->name, MODULE_ARCH);
     }
   m->module = eina_module_new(buf);
   if ((m->module) && (eina_module_load(m->module) == EINA_TRUE))
     {
        m->init_func = eina_module_symbol_get(m->module, "elm_modapi_init");
        if (m->init_func)
          {
             m->shutdown_func =
               eina_module_symbol_get(m->module, "elm_modapi_shutdown");
             m->so_path = eina_stringshare_add(buf);
             snprintf(buf, sizeof(buf), "%s/elementary/modules/%s/%s",
                      _elm_lib_dir, m->name, MODULE_ARCH);
             m->bin_dir = eina_stringshare_add(buf);
             snprintf(buf, sizeof(buf), "%s/elementary/modules/%s",
                      _elm_lib_dir, m->name);
             m->data_dir = eina_stringshare_add(buf);
          }
        else
          {
             if (m->module)
               {
                  eina_module_unload(m->module);
                  eina_module_free(m->module);
                  m->module = NULL;
               }
             return EINA_FALSE;
          }
     }
   else if (m->module)
     {
        eina_module_free(m->module);
        m->module = NULL;
     }

   if (!m->module) return EINA_FALSE;
   return EINA_TRUE;
}

void
_elm_module_unload(Elm_Module *m)
{
   eina_stringshare_del(m->so_path);
   eina_stringshare_del(m->data_dir);
   eina_stringshare_del(m->bin_dir);
   ELM_SAFE_FREE(m->api, free);
   if (m->module)
     {
        if (m->shutdown_func) m->shutdown_func(m);
        eina_module_unload(m->module);
        eina_module_free(m->module);
        m->module = NULL;
     }
   m->shutdown_func = NULL;
   m->init_func = NULL;
}

Elm_Module *
_elm_module_add(const char *name, const char *as)
{
   Elm_Module *m;

   if (name[0] == '/') return NULL;

   m = eina_hash_find(modules, name);
   if (m)
     {
        m->references++;
        return m;
     }
   m = calloc(1, sizeof(Elm_Module));
   if (!m) return NULL;
   m->version = 1;
   m->name = eina_stringshare_add(name);
   m->references = 1;
   eina_hash_direct_add(modules, m->name, m);
   m->as = eina_stringshare_add(as);
   eina_hash_direct_add(modules_as, m->as, m);
   return m;
}

void
_elm_module_del(Elm_Module *m)
{
   m->references--;
   if (m->references > 0) return;
   _elm_module_unload(m);
   eina_hash_del(modules, m->name, m);
   eina_hash_del(modules_as, m->as, m);
   eina_stringshare_del(m->name);
   eina_stringshare_del(m->as);
   free(m);
}

const void *
_elm_module_symbol_get(Elm_Module *m, const char *name)
{
   return eina_module_symbol_get(m->module, name);
}
