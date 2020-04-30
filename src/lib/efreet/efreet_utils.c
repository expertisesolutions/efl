#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* TODO: add no_display check, as we might want only displayable items */

#include <fnmatch.h>
#include <ctype.h>

#include <Ecore_File.h>

/* define macros and variable for using the eina logging system  */
#define EFREET_MODULE_LOG_DOM _efreet_utils_log_dom
static int _efreet_utils_log_dom = -1;

#include "Efreet.h"
#include "efreet_private.h"

static char *efreet_util_path_in_default(const char *section, const char *path);

static int  efreet_util_glob_match(const char *str, const char *glob);

static Eina_List *efreet_util_menus_find_helper(Eina_List *menus, const char *config_dir);

static Efreet_Desktop *efreet_util_cache_find(const char *search, const char *what1, const char *what2);
static Eina_List *efreet_util_cache_list(const char *search, const char *what);
static Eina_List *efreet_util_cache_glob_list(const char *search, const char *what);

static Eina_Lock _lock;

static Eina_Hash *file_id_by_desktop_path = NULL;

static int init = 0;

int
efreet_util_init(void)
{
    if (init++) return init;
    _efreet_utils_log_dom = eina_log_domain_register
      ("efreet_util", EFREET_DEFAULT_LOG_COLOR);
    if (_efreet_utils_log_dom < 0)
    {
        EINA_LOG_ERR("Efreet: Could not create a log domain for efreet_util");
        return 0;
    }

    if (!eina_lock_new(&_lock))
    {
        ERR("Could not create lock");
        goto error;
    }


    file_id_by_desktop_path = eina_hash_string_superfast_new(EINA_FREE_CB(eina_stringshare_del));

    return init;
error:
    eina_log_domain_unregister(_efreet_utils_log_dom);
    _efreet_utils_log_dom = -1;
    return 0;
}

int
efreet_util_shutdown(void)
{
    if (--init) return init;

    eina_lock_free(&_lock);

    eina_log_domain_unregister(_efreet_utils_log_dom);
    _efreet_utils_log_dom = -1;
    IF_FREE_HASH(file_id_by_desktop_path);

    return init;
}

static char *
efreet_util_path_in_default(const char *section, const char *path)
{
    Eina_List *dirs;
    char *ret = NULL;
    char *dir;

    dirs = efreet_default_dirs_get(efreet_data_home_get(), efreet_data_dirs_get(),
                                   section);

    EINA_LIST_FREE(dirs, dir)
    {
        if (!strncmp(path, dir, strlen(dir)))
            ret = dir;
        else
            eina_stringshare_del(dir);
    }

    return ret;
}

const char *
efreet_util_path_to_file_id(const char *path)
{
    size_t len, len2;
    char *tmp, *p;
    char *base;
    const char *file_id;

    EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);

    file_id = eina_hash_find(file_id_by_desktop_path, path);
    if (file_id) return file_id;

    base = efreet_util_path_in_default("applications", path);
    if (!base) return NULL;

    len = strlen(base);
    if (strlen(path) <= len)
    {
        eina_stringshare_del(base);
        return NULL;
    }
    if (strncmp(path, base, len))
    {
        eina_stringshare_del(base);
        return NULL;
    }

    len2 = strlen(path + len + 1) + 1;
    tmp = alloca(len2);
    memcpy(tmp, path + len + 1, len2);
    p = tmp;
    while (*p)
    {
        if (*p == '/') *p = '-';
        p++;
    }
    eina_stringshare_del(base);
    file_id = eina_stringshare_add(tmp);
    eina_lock_take(&_lock);
    eina_hash_set(file_id_by_desktop_path, path, (void *)file_id);
    eina_lock_release(&_lock);
    return file_id;
}

Eina_List *
efreet_util_desktop_mime_list(const char *mime)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(mime, NULL);
    return efreet_util_cache_list("mime_types", mime);
}

Efreet_Desktop *
efreet_util_desktop_wm_class_find(const char *wmname, const char *wmclass)
{
    EINA_SAFETY_ON_TRUE_RETURN_VAL((!wmname) && (!wmclass), NULL);
    return efreet_util_cache_find("startup_wm_class", wmname, wmclass);
}

Efreet_Desktop *
efreet_util_desktop_file_id_find(const char *file_id)
{
    Efreet_Cache_Hash *hash;
    Efreet_Desktop *ret = NULL;
    const char *str;

    EINA_SAFETY_ON_NULL_RETURN_VAL(file_id, NULL);

    hash = efreet_cache_util_hash_string("file_id");
    if (!hash) return NULL;
    str = eina_hash_find(hash->hash, file_id);
    if (str)
        ret = efreet_desktop_get(str);
    return ret;
}

static char *
efreet_util_cmd_args_get(const char *cmd)
{
   Eina_Strbuf *buf;
   char *args;
   const char *p;
   Eina_Bool in_qout_double = EINA_FALSE;
   Eina_Bool in_qout_single = EINA_FALSE;
   Eina_Bool atargs = EINA_FALSE;
   Eina_Bool ingap = EINA_FALSE;

   buf = eina_strbuf_new();
   if (!buf) return NULL;

   // get the arguments to the command as a string on its own
   for (p = cmd; *p; p++)
     {
        if (!atargs)
          {
             if (in_qout_double)
               {
                  if (*p == '\\')
                    {
                       if (p[1]) p++;
                    }
                  else if (*p == '"') in_qout_double = EINA_FALSE;
               }
             else if (in_qout_single)
               {
                  if (*p == '\\')
                    {
                       if (p[1]) p++;
                    }
                  else if (*p == '\'') in_qout_single = EINA_FALSE;
               }
             else
               {
                  if (*p == '\\')
                    {
                       if (p[1]) p++;
                    }
                  else if (*p == '"') in_qout_double = EINA_TRUE;
                  else if (*p == '\'') in_qout_single = EINA_TRUE;
                  else
                    {
                       if (isspace((unsigned char)(*p)))
                         {
                            atargs = EINA_TRUE;
                            ingap = EINA_TRUE;
                         }
                    }
               }
          }
        else
          {
             if (ingap)
               {
                  if (!isspace((unsigned char)(*p))) ingap = EINA_FALSE;
               }
             if (!ingap) eina_strbuf_append_char(buf, *p);
          }
     }

   args = eina_strbuf_string_steal(buf);
   eina_strbuf_free(buf);
   if (!args) return strdup("");
   return args;
}

Efreet_Desktop *
efreet_util_desktop_exec_find(const char *exec)
{
    Efreet_Cache_Hash *hash = NULL;
    Efreet_Desktop *ret = NULL, *bestret = NULL;
    Efreet_Cache_Array_String *names = NULL;
    unsigned int i;

    EINA_SAFETY_ON_NULL_RETURN_VAL(exec, NULL);

    names = efreet_cache_util_names("exec_list");
    if (!names) return NULL;
    for (i = 0; i < names->array_count; i++)
    {
        const char *file;
        char *exe;
        unsigned int j;
        Efreet_Cache_Array_String *array;

        exe = ecore_file_app_exe_get(names->array[i]);
        if (!exe) continue;
        file = ecore_file_file_get(exe);
        if ((!file) || (strcmp(exec, exe) && strcmp(exec, file)))
        {
            free(exe);
            continue;
        }
        free(exe);

        if (!hash)
            hash = efreet_cache_util_hash_array_string("exec_hash");
        if (!hash) return NULL;
        array = eina_hash_find(hash->hash, names->array[i]);
        if (!array) continue;
        for (j = 0; j < array->array_count; j++)
        {
            ret = efreet_desktop_get(array->array[j]);
            if (ret)
            {
               if (!bestret)
               {
                  bestret = ret;
                  if (bestret->exec && !strcmp(bestret->exec, exec))
                    goto done;
               }
               else
               {
                  if (ret->exec)
                  {
                     // perfect match - best
                     if (!strcmp(ret->exec, exec))
                     {
                        bestret = ret;
                        goto done;
                     }
                     else if (bestret->exec)
                     {
                        char *f1, *f2;

                        f1 = efreet_util_cmd_args_get(ret->exec);
                        f2 = efreet_util_cmd_args_get(bestret->exec);
                        if ((f1) && (f2))
                          {
                             // if this is shorter (less arguments) than best
                             // match so far, thewn this is the best match
                             if (strlen(f1) < strlen(f2))
                               {
                                  bestret = ret;
                               }
                          }
                        free(f1);
                        free(f2);
                     }
                  }
               }
            }
        }
    }
done:
    return bestret;
}

Efreet_Desktop *
efreet_util_desktop_name_find(const char *name)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);
    return efreet_util_cache_find("name", name, NULL);
}

Efreet_Desktop *
efreet_util_desktop_generic_name_find(const char *generic_name)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(generic_name, NULL);
    return efreet_util_cache_find("generic_name", generic_name, NULL);
}

Eina_List *
efreet_util_desktop_name_glob_list(const char *glob)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(glob, NULL);
    return efreet_util_cache_glob_list("name", glob);
}

Eina_List *
efreet_util_desktop_exec_glob_list(const char *glob)
{
    Efreet_Cache_Hash *hash = NULL;
    Eina_List *ret = NULL;
    Efreet_Cache_Array_String *names = NULL;
    unsigned int i;

    EINA_SAFETY_ON_NULL_RETURN_VAL(glob, NULL);

    if (!strcmp(glob, "*"))
        glob = NULL;

    names = efreet_cache_util_names("exec_list");
    if (!names) return NULL;
    for (i = 0; i < names->array_count; i++)
    {
        Efreet_Cache_Array_String *array;
        unsigned int j;
        char *exe;
        Efreet_Desktop *desk;

        exe = ecore_file_app_exe_get(names->array[i]);
        if (!exe) continue;
        if (glob && !efreet_util_glob_match(exe, glob))
        {
            free(exe);
            continue;
        }
        free(exe);

        if (!hash)
            hash = efreet_cache_util_hash_array_string("exec_hash");
        if (!hash) return NULL;

        array = eina_hash_find(hash->hash, names->array[i]);
        if (!array) continue;
        for (j = 0; j < array->array_count; j++)
        {
            desk = efreet_desktop_get(array->array[j]);
            if (desk)
                ret = eina_list_append(ret, desk);
        }
    }
    return ret;
}

Eina_List *
efreet_util_desktop_generic_name_glob_list(const char *glob)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(glob, NULL);
    return efreet_util_cache_glob_list("generic_name", glob);
}

Eina_List *
efreet_util_desktop_comment_glob_list(const char *glob)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(glob, NULL);
    return efreet_util_cache_glob_list("comment", glob);
}

Eina_List *
efreet_util_desktop_categories_list(void)
{
    Efreet_Cache_Array_String *array;
    Eina_List *ret = NULL;
    unsigned int i;

    array = efreet_cache_util_names("categories_list");
    if (!array) return NULL;
    for (i = 0; i < array->array_count; i++)
        ret = eina_list_append(ret, array->array[i]);
    return ret;
}

Eina_List *
efreet_util_desktop_category_list(const char *category)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(category, NULL);
    return efreet_util_cache_list("categories", category);
}

Eina_List *
efreet_util_desktop_environments_list(void)
{
    Efreet_Cache_Array_String *array;
    Eina_List *ret = NULL;
    unsigned int i;

    array = efreet_cache_util_names("environments_list");
    if (!array) return NULL;
    for (i = 0; i < array->array_count; i++)
        ret = eina_list_append(ret, array->array[i]);
    return ret;
}

static int
efreet_util_glob_match(const char *str, const char *glob)
{
    if (!str || !glob)
        return 0;
    if (glob[0] == '\0')
    {
        if (str[0] == '\0') return 1;
        return 0;
    }
    if (!strcmp(glob, "*")) return 1;
    if (!fnmatch(glob, str, 0)) return 1;
    return 0;
}

Eina_List *
efreet_util_menus_find(void)
{
    Eina_List *menus = NULL;
    Eina_List *dirs, *l;
    const char *dir;

    menus = efreet_util_menus_find_helper(menus, efreet_config_home_get());

    dirs = efreet_config_dirs_get();
    EINA_LIST_FOREACH(dirs, l, dir)
        menus = efreet_util_menus_find_helper(menus, dir);

    return menus;
}

static Eina_List *
efreet_util_menus_find_helper(Eina_List *menus, const char *config_dir)
{
    Eina_Iterator *it;
    Eina_File_Direct_Info *info;
    char dbuf[PATH_MAX];

    snprintf(dbuf, sizeof(dbuf), "%s/menus", config_dir);
    it = eina_file_stat_ls(dbuf);
    if (!it) return menus;
    EINA_ITERATOR_FOREACH(it, info)
    {
        const char *exten;

        if (info->type == EINA_FILE_DIR) continue;

        exten = strrchr(info->path + info->name_start, '.');
        if (!exten) continue;

        if (strcmp(".menu", exten)) continue;

        menus = eina_list_append(menus, strdup(info->path));
    }
    eina_iterator_free(it);
    return menus;
}

static Efreet_Desktop *
efreet_util_cache_find(const char *search, const char *what1, const char *what2)
{
    Efreet_Cache_Hash *hash;
    Efreet_Desktop *ret = NULL;
    Efreet_Cache_Array_String *array = NULL;
    char key[256];

    if ((!what1) && (!what2)) return NULL;

    snprintf(key, sizeof(key), "%s_hash", search);
    hash = efreet_cache_util_hash_array_string(key);
    if (!hash) return NULL;
    if (what1)
        array = eina_hash_find(hash->hash, what1);
    if (!array && what2) array = eina_hash_find(hash->hash, what2);
    if (array)
    {
        unsigned int i;

        for (i = 0; i < array->array_count; i++)
        {
            ret = efreet_desktop_get(array->array[i]);
            if (ret) break;
        }
    }
    return ret;
}

static Eina_List *
efreet_util_cache_list(const char *search, const char *what)
{
    Efreet_Cache_Hash *hash;
    Efreet_Cache_Array_String *array;
    Eina_List *ret = NULL;
    char key[256];

    if (!what) return NULL;

    snprintf(key, sizeof(key), "%s_hash", search);
    hash = efreet_cache_util_hash_array_string(key);
    if (!hash) return NULL;
    array = eina_hash_find(hash->hash, what);
    if (array)
    {
        unsigned int i;
        Efreet_Desktop *desk;

        for (i = 0; i < array->array_count; i++)
        {
            desk = efreet_desktop_get(array->array[i]);
            if (desk)
                ret = eina_list_append(ret, desk);
        }
    }
    return ret;
}

static Eina_List *
efreet_util_cache_glob_list(const char *search, const char *what)
{
    Efreet_Cache_Hash *hash = NULL;
    Eina_List *ret = NULL;
    Efreet_Cache_Array_String *names = NULL;
    char key[256];
    unsigned int i;

    if (!what) return NULL;
    if (!strcmp(what, "*"))
        what = NULL;

    snprintf(key, sizeof(key), "%s_list", search);
    names = efreet_cache_util_names(key);
    if (!names) return NULL;
    for (i = 0; i < names->array_count; i++)
    {
        Efreet_Cache_Array_String *array;
        unsigned int j;
        Efreet_Desktop *desk;

        if (what && !efreet_util_glob_match(names->array[i], what)) continue;

        if (!hash)
        {
            snprintf(key, sizeof(key), "%s_hash", search);
            hash = efreet_cache_util_hash_array_string(key);
        }
        if (!hash) return NULL;

        array = eina_hash_find(hash->hash, names->array[i]);
        if (!array) continue;
        for (j = 0; j < array->array_count; j++)
        {
            desk = efreet_desktop_get(array->array[j]);
            if (desk)
                ret = eina_list_append(ret, desk);
        }
    }
    return ret;
}

/*
 * Needs because of helper binaries
 */
void
efreet_hash_free(Eina_Hash *hash, Eina_Free_Cb free_cb)
{
    eina_hash_free_cb_set(hash, free_cb);
    eina_hash_free(hash);
}

