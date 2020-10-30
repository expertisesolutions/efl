#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* TODO: Consider flushing local icons cache after idling.
 *       Icon requests will probably come in batches, f.ex. during menu
 *       browsing.
 */

#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Eet.h>
#include <Ecore.h>
#include <Ecore_File.h>
#include <Ecore_Con.h>
#ifdef HAVE_EFREETD
# include <Ecore_Ipc.h>
#endif /* HAVE_EFREETD */

/* define macros and variable for using the eina logging system  */
#define EFREET_MODULE_LOG_DOM _efreet_cache_log_dom

#include "Efreet.h"
#include "efreet_cache_private.h"
#include "efreet_private.h"
#include "../../static_libs/buildsystem/buildsystem.h"
#define NON_EXISTING (void *)-1

/* define macros and variable for using the eina logging system  */
int _efreet_cache_log_dom = -1;

typedef struct _Efreet_Old_Cache Efreet_Old_Cache;

struct _Efreet_Old_Cache
{
    Eina_Hash *hash;
    Eet_File *ef;
};


static Eina_Lock _lock;

/**
 * Data for cache files
 */
static Eet_Data_Descriptor *directory_edd = NULL;
static Eet_Data_Descriptor *icon_theme_edd = NULL;
static Eet_Data_Descriptor *icon_theme_directory_edd = NULL;

static Eet_Data_Descriptor *icon_fallback_edd = NULL;
static Eet_Data_Descriptor *icon_element_pointer_edd = NULL;
static Eet_Data_Descriptor *icon_element_edd = NULL;
static Eet_Data_Descriptor *icon_edd = NULL;

static Eet_File            *icon_cache = NULL;
static Eet_File            *fallback_cache = NULL;
static Eet_File            *icon_theme_cache = NULL;

static Eina_Hash           *themes = NULL;
static Eina_Hash           *icons = NULL;
static Eina_Hash           *fallbacks = NULL;

static const char          *icon_theme_cache_file = NULL;

static const char          *theme_name = NULL;

static Eet_Data_Descriptor *version_edd = NULL;
static Eet_Data_Descriptor *desktop_edd = NULL;
static Eet_Data_Descriptor *desktop_action_edd = NULL;
static Eet_Data_Descriptor *hash_array_string_edd = NULL;
static Eet_Data_Descriptor *array_string_edd = NULL;
static Eet_Data_Descriptor *hash_string_edd = NULL;

static Eina_Hash           *desktops = NULL;
static Eet_File            *desktop_cache = NULL;
static const char          *desktop_cache_file = NULL;

static Eina_List           *old_desktop_caches = NULL;

static const char                *util_cache_file = NULL;
static Eet_File                  *util_cache = NULL;
static Efreet_Cache_Hash         *util_cache_hash = NULL;
static const char                *util_cache_hash_key = NULL;
static Efreet_Cache_Array_String *util_cache_names = NULL;
static const char                *util_cache_names_key = NULL;

static void efreet_cache_edd_shutdown(void);
static void efreet_cache_icon_free(Efreet_Cache_Icon *icon);
static void efreet_cache_icon_fallback_free(Efreet_Cache_Fallback_Icon *icon);
static void efreet_cache_icon_theme_free(Efreet_Icon_Theme *theme);

static Eina_Bool efreet_cache_check(Eet_File **ef, const char *path, int major);
static void *efreet_cache_close(Eet_File *ef);

static void icon_cache_update_free(void *data, void *ev);

static void *hash_array_string_add(void *hash, const char *key, void *data);
Eina_Bool run_in_tree;
Eina_Bool disable_cache;

EAPI int EFREET_EVENT_ICON_CACHE_UPDATE = 0;
EAPI int EFREET_EVENT_DESKTOP_CACHE_UPDATE = 0;
EAPI int EFREET_EVENT_DESKTOP_CACHE_BUILD = 0;

static void
_efreet_cache_reset()
{
   const char *s = efreet_language_get();
   int len = 0;

   if (s) len = strlen(s);

   if(_efreet_cache_reset_language(s, len) == EINA_TRUE)
     efreet_icon_extensions_refresh();
}

void
_icon_desktop_cache_update_event_add(int event_type)
{
   Efreet_Event_Cache_Update *ev;
   Efreet_Old_Cache *d = NULL;
   Eina_List *l = NULL;

   efreet_cache_desktop_close();

   ev = NEW(Efreet_Event_Cache_Update, 1);
   if (!ev) return;

   IF_RELEASE(theme_name);

   // Save all old caches
   d = NEW(Efreet_Old_Cache, 1);
   if (d)
     {
        d->hash = themes;
        d->ef = icon_theme_cache;
        l = eina_list_append(l, d);
     }

   d = NEW(Efreet_Old_Cache, 1);
   if (d)
     {
        d->hash = icons;
        d->ef = icon_cache;
        l = eina_list_append(l, d);
     }

   d = NEW(Efreet_Old_Cache, 1);
   if (d)
     {
        d->hash = fallbacks;
        d->ef = fallback_cache;
        l = eina_list_append(l, d);
     }

   // Create new empty caches
   themes = eina_hash_string_superfast_new(EINA_FREE_CB(efreet_cache_icon_theme_free));
   icons = eina_hash_string_superfast_new(EINA_FREE_CB(efreet_cache_icon_free));
   fallbacks = eina_hash_string_superfast_new(EINA_FREE_CB(efreet_cache_icon_fallback_free));

   icon_theme_cache = NULL;
   icon_cache = NULL;
   fallback_cache = NULL;

   // Send event
   ecore_event_add(event_type, ev, icon_cache_update_free, l);
}

int
efreet_cache_init(void)
{
    char buf[PATH_MAX];

    _efreet_cache_log_dom = eina_log_domain_register("efreet_cache", EFREET_DEFAULT_LOG_COLOR);
    if (_efreet_cache_log_dom < 0)
        return 0;

    if (!eina_lock_new(&_lock))
    {
        ERR("Could not create lock");
        goto error;
    }

    snprintf(buf, sizeof(buf), "%s/efreet", efreet_cache_home_get());
    if (!ecore_file_mkpath(buf))
    {
        ERR("Failed to create directory '%s'", buf);
    }

    run_in_tree = !!getenv("EFL_RUN_IN_TREE");

    EFREET_EVENT_ICON_CACHE_UPDATE = ecore_event_type_new();
    EFREET_EVENT_DESKTOP_CACHE_UPDATE = ecore_event_type_new();
    EFREET_EVENT_DESKTOP_CACHE_BUILD = ecore_event_type_new();

    themes = eina_hash_string_superfast_new(EINA_FREE_CB(efreet_cache_icon_theme_free));
    icons = eina_hash_string_superfast_new(EINA_FREE_CB(efreet_cache_icon_free));
    fallbacks = eina_hash_string_superfast_new(EINA_FREE_CB(efreet_cache_icon_fallback_free));
    desktops = eina_hash_string_superfast_new(NULL);

#ifdef HAVE_EFREETD
    ecore_ipc_init();
   // XXX: connect to efreetd
#endif /* HAVE_EFREETD */

    if (efreet_cache_update)
      {
         _server_config_signal_send();
      }
    ecore_fork_reset_callback_add(_efreet_cache_reset, NULL);

    return 1;
error:
    if (themes) eina_hash_free(themes);
    themes = NULL;
    if (icons) eina_hash_free(icons);
    icons = NULL;
    if (fallbacks) eina_hash_free(fallbacks);
    fallbacks = NULL;
    if (desktops) eina_hash_free(desktops);
    desktops = NULL;

    efreet_cache_edd_shutdown();
    return 0;
}

void
efreet_cache_shutdown(void)
{
   Efreet_Old_Cache *d;

   ecore_event_type_flush(EFREET_EVENT_ICON_CACHE_UPDATE,
                          EFREET_EVENT_DESKTOP_CACHE_UPDATE,
                          EFREET_EVENT_DESKTOP_CACHE_BUILD);
   ecore_fork_reset_callback_del(_efreet_cache_reset, NULL);
   IF_RELEASE(theme_name);

   icon_cache = efreet_cache_close(icon_cache);
   icon_theme_cache = efreet_cache_close(icon_theme_cache);
   fallback_cache = efreet_cache_close(fallback_cache);

   IF_FREE_HASH(themes);
   IF_FREE_HASH(icons);
   IF_FREE_HASH(fallbacks);

   IF_FREE_HASH_CB(desktops, EINA_FREE_CB(efreet_cache_desktop_free));
   desktop_cache = efreet_cache_close(desktop_cache);
   IF_RELEASE(desktop_cache_file);

   efreet_cache_edd_shutdown();
   IF_RELEASE(icon_theme_cache_file);

   if (old_desktop_caches)
       ERR("This application has not properly closed all its desktop references!");
   EINA_LIST_FREE(old_desktop_caches, d)
     {
        eina_hash_free(d->hash);
        eet_close(d->ef);
        free(d);
     }

   IF_RELEASE(util_cache_names_key);
   efreet_cache_array_string_free(util_cache_names);
   util_cache_names = NULL;

   IF_RELEASE(util_cache_hash_key);
   if (util_cache_hash)
     {
        eina_hash_free(util_cache_hash->hash);
        free(util_cache_hash);
        util_cache_hash = NULL;
     }

   util_cache = efreet_cache_close(util_cache);
   IF_RELEASE(util_cache_file);

#ifdef HAVE_EFREETD
   _ipc_data_clean();
   ecore_ipc_shutdown();
#endif /* HAVE_EFREETD */

   eina_lock_free(&_lock);

   eina_log_domain_unregister(_efreet_cache_log_dom);
   _efreet_cache_log_dom = -1;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI const char *
efreet_icon_cache_file(const char *theme)
{
    static char cache_file[PATH_MAX] = { '\0' };
    const char *cache;

    EINA_SAFETY_ON_NULL_RETURN_VAL(theme, NULL);

    cache = efreet_cache_home_get();

    snprintf(cache_file, sizeof(cache_file), "%s/efreet/icons_%s_%s.eet", cache, theme, efreet_hostname_get());

    return cache_file;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI const char *
efreet_icon_theme_cache_file(void)
{
    char tmp[PATH_MAX] = { '\0' };

    if (icon_theme_cache_file) return icon_theme_cache_file;

    snprintf(tmp, sizeof(tmp), "%s/efreet/icon_themes_%s.eet",
             efreet_cache_home_get(), efreet_hostname_get());
    icon_theme_cache_file = eina_stringshare_add(tmp);

    return icon_theme_cache_file;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI const char *
efreet_desktop_util_cache_file(void)
{
    char tmp[PATH_MAX] = { '\0' };
    const char *cache_dir, *lang, *country, *modifier;

    if (util_cache_file) return util_cache_file;

    cache_dir = efreet_cache_home_get();
    lang = efreet_lang_get();
    country = efreet_lang_country_get();
    modifier = efreet_lang_modifier_get();

    if (lang && country && modifier)
        snprintf(tmp, sizeof(tmp), "%s/efreet/desktop_util_%s_%s_%s@%s.eet", cache_dir, efreet_hostname_get(), lang, country, modifier);
    else if (lang && country)
        snprintf(tmp, sizeof(tmp), "%s/efreet/desktop_util_%s_%s_%s.eet", cache_dir, efreet_hostname_get(), lang, country);
    else if (lang)
        snprintf(tmp, sizeof(tmp), "%s/efreet/desktop_util_%s_%s.eet", cache_dir, efreet_hostname_get(), lang);
    else
        snprintf(tmp, sizeof(tmp), "%s/efreet/desktop_util_%s.eet", cache_dir, efreet_hostname_get());

    util_cache_file = eina_stringshare_add(tmp);
    return util_cache_file;
}

/*
 * Needs EAPI because of helper binaries
 */
#define SHSH(n, v) ((((v) << (n)) & 0xffffffff) | ((v) >> (32 - (n))))

static inline int
int_to_bigendian(int in)
{
   static const unsigned char test[4] = { 0x11, 0x22, 0x33, 0x44 };
   static const unsigned int *test_i = (const unsigned int *)test;
   if (test_i[0] == 0x44332211) return eina_swap32(in);
   return in;
}

static void
sha1(unsigned char *data, int size, unsigned char *dst)
{
   unsigned int digest[5], word[80], wa, wb, wc, wd, we, t;
   unsigned char buf[64], *d;
   int idx, left, i;
   const unsigned int magic[4] =
     { 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 };

   idx = 0;
   digest[0] = 0x67452301; digest[1] = 0xefcdab89; digest[2] = 0x98badcfe;
   digest[3] = 0x10325476; digest[4] = 0xc3d2e1f0;

   memset(buf, 0, sizeof(buf));
   for (left = size, d = data; left > 0; left--, d++)
     {
        if ((idx == 0) && (left < 64))
          {
             memset(buf, 0, 60);
             buf[60] = (size >> 24) & 0xff;
             buf[61] = (size >> 16) & 0xff;
             buf[62] = (size >> 8) & 0xff;
             buf[63] = (size) & 0xff;
          }
        buf[idx] = *d;
        idx++;
        if ((idx == 64) || (left == 1))
          {
             if ((left == 1) && (idx < 64)) buf[idx] = 0x80;
             for (i = 0; i < 16; i++)
               {
                  word[i]  = (unsigned int)buf[(i * 4)    ] << 24;
                  word[i] |= (unsigned int)buf[(i * 4) + 1] << 16;
                  word[i] |= (unsigned int)buf[(i * 4) + 2] << 8;
                  word[i] |= (unsigned int)buf[(i * 4) + 3];
               }
             for (i = 16; i < 80; i++)
               word[i] = SHSH(1,
                              word[i - 3 ] ^ word[i - 8 ] ^
                              word[i - 14] ^ word[i - 16]);
             wa = digest[0]; wb = digest[1]; wc = digest[2];
             wd = digest[3]; we = digest[4];
             for (i = 0; i < 80; i++)
               {
                  if (i < 20)
                    t = SHSH(5, wa) + ((wb & wc) | ((~wb) & wd)) +
                        we + word[i] + magic[0];
                  else if (i < 40)
                    t = SHSH(5, wa) + (wb ^ wc ^ wd) +
                        we + word[i] + magic[1];
                  else if (i < 60)
                    t = SHSH(5, wa) + ((wb & wc) | (wb & wd) | (wc & wd)) +
                        we + word[i] + magic[2];
                  else if (i < 80)
                    t = SHSH(5, wa) + (wb ^ wc ^ wd) +
                        we + word[i] + magic[3];
                  we = wd;
                  wd = wc;
                  wc = SHSH(30, wb);
                  wb = wa;
                  wa = t;
               }
             digest[0] += wa; digest[1] += wb; digest[2] += wc;
             digest[3] += wd; digest[4] += we;
             idx = 0;
          }
     }
   t = int_to_bigendian(digest[0]); digest[0] = t;
   t = int_to_bigendian(digest[1]); digest[1] = t;
   t = int_to_bigendian(digest[2]); digest[2] = t;
   t = int_to_bigendian(digest[3]); digest[3] = t;
   t = int_to_bigendian(digest[4]); digest[4] = t;
   memcpy(dst, digest, 5 * 4);
}

EAPI Eina_Bool
efreet_file_cache_fill(const char *file, Efreet_Cache_Check *check)
{
   struct stat st;

   memset(check, 0, sizeof(Efreet_Cache_Check));
#ifdef _WIN32
   if (stat(file, &st) != 0) return EINA_FALSE;
#else
   ssize_t size = 0;

   if (lstat(file, &st) != 0) return EINA_FALSE;
   if (S_ISLNK(st.st_mode))
     {
        char link[PATH_MAX];

        size = readlink(file, link, sizeof(link));
        if ((size > 0) && ((size_t)size >= sizeof(link))) return EINA_FALSE;
        if (stat(file, &st) != 0) return EINA_FALSE;
     }
   if (size > 0) sha1((unsigned char *)link, size, check->link_sha1);
#endif
   check->uid    = st.st_uid;
   check->gid    = st.st_gid;
   check->size   = st.st_size;
#ifndef _WIN32
   check->blocks = st.st_blocks;
#else
   check->blocks = 0;
#endif
   check->mtime  = st.st_mtime;
   check->chtime = st.st_ctime;
   check->mode   = st.st_mode;
   return EINA_TRUE;
}

EAPI Eina_Bool // true if matches
efreet_file_cache_check(const Efreet_Cache_Check *check1, const Efreet_Cache_Check *check2)
{
   if ((check1->mtime  != check2->mtime ) ||
       (check1->size   != check2->size  ) ||
       (check1->chtime != check2->chtime ) ||
       (check1->blocks != check2->blocks) ||
       (check1->mode   != check2->mode  ) ||
       (check1->uid    != check2->uid   ) ||
       (check1->gid    != check2->gid   ) ||
       (memcmp(check1->link_sha1, check2->link_sha1, 20) != 0))
     {
        return EINA_FALSE;
     }
   return EINA_TRUE; // matches
}

EAPI Eet_Data_Descriptor *
efreet_version_edd(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (version_edd) return version_edd;

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Version);
    version_edd = eet_data_descriptor_file_new(&eddc);
    if (!version_edd) return NULL;

    EET_DATA_DESCRIPTOR_ADD_BASIC(version_edd, Efreet_Cache_Version,
                                  "minor", minor, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(version_edd, Efreet_Cache_Version,
                                  "major", major, EET_T_UCHAR);

    return version_edd;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI Eet_Data_Descriptor *
efreet_hash_array_string_edd(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (hash_array_string_edd) return hash_array_string_edd;

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Hash);
    eddc.func.hash_add = hash_array_string_add;
    hash_array_string_edd = eet_data_descriptor_file_new(&eddc);
    if (!hash_array_string_edd) return NULL;

    EET_DATA_DESCRIPTOR_ADD_HASH(hash_array_string_edd, Efreet_Cache_Hash,
                                  "hash", hash, efreet_array_string_edd());

    return hash_array_string_edd;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI Eet_Data_Descriptor *
efreet_hash_string_edd(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (hash_string_edd) return hash_string_edd;

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Hash);
    hash_string_edd = eet_data_descriptor_file_new(&eddc);
    if (!hash_string_edd) return NULL;

    EET_DATA_DESCRIPTOR_ADD_HASH_STRING(hash_string_edd, Efreet_Cache_Hash,
                                  "hash", hash);

    return hash_string_edd;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI Eet_Data_Descriptor *
efreet_array_string_edd(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (array_string_edd) return array_string_edd;

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Array_String);
    array_string_edd = eet_data_descriptor_file_new(&eddc);
    if (!array_string_edd) return NULL;
    EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY_STRING(array_string_edd, Efreet_Cache_Array_String,
                                             "array", array);

    return array_string_edd;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI const char *
efreet_desktop_cache_file(void)
{
    char tmp[PATH_MAX] = { '\0' };
    const char *cache, *lang, *country, *modifier;

    if (desktop_cache_file) return desktop_cache_file;

    cache = efreet_cache_home_get();
    lang = efreet_lang_get();
    country = efreet_lang_country_get();
    modifier = efreet_lang_modifier_get();

    if (lang && country && modifier)
        snprintf(tmp, sizeof(tmp), "%s/efreet/desktop_%s_%s_%s@%s.eet", cache, efreet_hostname_get(), lang, country, modifier);
    else if (lang && country)
        snprintf(tmp, sizeof(tmp), "%s/efreet/desktop_%s_%s_%s.eet", cache, efreet_hostname_get(), lang, country);
    else if (lang)
        snprintf(tmp, sizeof(tmp), "%s/efreet/desktop_%s_%s.eet", cache, efreet_hostname_get(), lang);
    else
        snprintf(tmp, sizeof(tmp), "%s/efreet/desktop_%s.eet", cache, efreet_hostname_get());

    desktop_cache_file = eina_stringshare_add(tmp);
    return desktop_cache_file;
}

#define EDD_SHUTDOWN(Edd)                       \
    if (Edd) eet_data_descriptor_free(Edd);       \
Edd = NULL;

static void
efreet_cache_edd_shutdown(void)
{
    EDD_SHUTDOWN(version_edd);
    EDD_SHUTDOWN(desktop_edd);
    EDD_SHUTDOWN(desktop_action_edd);
    EDD_SHUTDOWN(hash_array_string_edd);
    EDD_SHUTDOWN(array_string_edd);
    EDD_SHUTDOWN(hash_string_edd);
    EDD_SHUTDOWN(icon_theme_edd);
    EDD_SHUTDOWN(icon_theme_directory_edd);
    EDD_SHUTDOWN(directory_edd);
    EDD_SHUTDOWN(icon_fallback_edd);
    EDD_SHUTDOWN(icon_element_pointer_edd);
    EDD_SHUTDOWN(icon_element_edd);
    EDD_SHUTDOWN(icon_edd);
}

#define EFREET_POINTER_TYPE(Edd_Dest, Edd_Source, Type)   \
{                                                                     \
    typedef struct _Efreet_##Type##_Pointer Efreet_##Type##_Pointer;   \
    struct _Efreet_##Type##_Pointer                                    \
    {                                                                  \
        Efreet_##Type *pointer;                                         \
    };                                                                 \
    \
    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_##Type##_Pointer); \
    Edd_Dest = eet_data_descriptor_file_new(&eddc);                    \
    EET_DATA_DESCRIPTOR_ADD_SUB(Edd_Dest, Efreet_##Type##_Pointer,     \
                                "pointer", pointer, Edd_Source);       \
}

static Eet_Data_Descriptor *
efreet_icon_directory_edd(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (directory_edd) return directory_edd;

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Directory);
    directory_edd = eet_data_descriptor_file_new(&eddc);
    if (!directory_edd) return NULL;

   EET_DATA_DESCRIPTOR_ADD_BASIC(directory_edd, Efreet_Cache_Directory,
                                 "check.uid", check.uid, EET_T_LONG_LONG);
   EET_DATA_DESCRIPTOR_ADD_BASIC(directory_edd, Efreet_Cache_Directory,
                                 "check.gid", check.gid, EET_T_LONG_LONG);
   EET_DATA_DESCRIPTOR_ADD_BASIC(directory_edd, Efreet_Cache_Directory,
                                 "check.size", check.size, EET_T_LONG_LONG);
   EET_DATA_DESCRIPTOR_ADD_BASIC(directory_edd, Efreet_Cache_Directory,
                                 "check.blocks", check.blocks, EET_T_LONG_LONG);
   EET_DATA_DESCRIPTOR_ADD_BASIC(directory_edd, Efreet_Cache_Directory,
                                 "check.mtime", check.mtime, EET_T_LONG_LONG);
   EET_DATA_DESCRIPTOR_ADD_BASIC(directory_edd, Efreet_Cache_Directory,
                                 "check.chtime", check.chtime, EET_T_LONG_LONG);
   EET_DATA_DESCRIPTOR_ADD_BASIC(directory_edd, Efreet_Cache_Directory,
                                 "check.mode", check.mode, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC_ARRAY(directory_edd, Efreet_Cache_Directory,
                                       "check.link_sha1", check.link_sha1, EET_T_CHAR);

    return directory_edd;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI Eet_Data_Descriptor *
efreet_icon_edd(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (icon_edd) return icon_edd;

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Icon_Element);
    icon_element_edd = eet_data_descriptor_file_new(&eddc);
    if (!icon_element_edd) return NULL;

    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_element_edd, Efreet_Cache_Icon_Element,
                                  "type", type, EET_T_USHORT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_element_edd, Efreet_Cache_Icon_Element,
                                  "normal", normal, EET_T_USHORT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_element_edd, Efreet_Cache_Icon_Element,
                                  "normal", normal, EET_T_USHORT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_element_edd, Efreet_Cache_Icon_Element,
                                  "min", min, EET_T_USHORT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_element_edd, Efreet_Cache_Icon_Element,
                                  "max", max, EET_T_USHORT);
    EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY_STRING(icon_element_edd, Efreet_Cache_Icon_Element,
                                             "paths", paths);

    EFREET_POINTER_TYPE(icon_element_pointer_edd, icon_element_edd, Cache_Icon_Element);

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Icon);
    icon_edd = eet_data_descriptor_file_new(&eddc);
    if (!icon_edd) return NULL;

    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_edd, Efreet_Cache_Icon,
                                  "theme", theme, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY(icon_edd, Efreet_Cache_Icon,
                                      "icons", icons, icon_element_pointer_edd);

    return icon_edd;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI Eet_Data_Descriptor *
efreet_icon_theme_edd(Eina_Bool cache)
{
    Eet_Data_Descriptor_Class eddc;

    if (icon_theme_edd) return icon_theme_edd;

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Icon_Theme_Directory);
    icon_theme_directory_edd = eet_data_descriptor_file_new(&eddc);
    if (!icon_theme_directory_edd) return NULL;

    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_directory_edd, Efreet_Icon_Theme_Directory,
                                  "name", name, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_directory_edd, Efreet_Icon_Theme_Directory,
                                  "context", context, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_directory_edd, Efreet_Icon_Theme_Directory,
                                  "type", type, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_directory_edd, Efreet_Icon_Theme_Directory,
                                  "size.normal", size.normal, EET_T_UINT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_directory_edd, Efreet_Icon_Theme_Directory,
                                  "size.min", size.min, EET_T_UINT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_directory_edd, Efreet_Icon_Theme_Directory,
                                  "size.max", size.max, EET_T_UINT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_directory_edd, Efreet_Icon_Theme_Directory,
                                  "size.threshold", size.threshold, EET_T_UINT);

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Icon_Theme);
    icon_theme_edd = eet_data_descriptor_file_new(&eddc);
    if (!icon_theme_edd) return NULL;

    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                  "name.internal", theme.name.internal, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                  "name.name", theme.name.name, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                  "comment", theme.comment, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                  "example_icon", theme.example_icon, EET_T_STRING);

    eet_data_descriptor_element_add(icon_theme_edd, "paths", EET_T_STRING, EET_G_LIST,
                                    offsetof(Efreet_Cache_Icon_Theme, theme.paths), 0, NULL, NULL);
    eet_data_descriptor_element_add(icon_theme_edd, "inherits", EET_T_STRING, EET_G_LIST,
                                    offsetof(Efreet_Cache_Icon_Theme, theme.inherits), 0, NULL, NULL);
    EET_DATA_DESCRIPTOR_ADD_LIST(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                  "directories", theme.directories, icon_theme_directory_edd);

    if (cache)
    {
        EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                      "check.uid", check.uid, EET_T_LONG_LONG);
        EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                      "check.gid", check.gid, EET_T_LONG_LONG);
        EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                      "check.size", check.size, EET_T_LONG_LONG);
        EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                      "check.blocks", check.blocks, EET_T_LONG_LONG);
        EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                      "check.mtime", check.mtime, EET_T_LONG_LONG);
        EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                      "check.chtime", check.chtime, EET_T_LONG_LONG);
        EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                      "check.mode", check.mode, EET_T_INT);
        EET_DATA_DESCRIPTOR_ADD_BASIC_ARRAY(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                            "check.link_sha1", check.link_sha1, EET_T_CHAR);

        EET_DATA_DESCRIPTOR_ADD_BASIC(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                      "path", path, EET_T_STRING);

        EET_DATA_DESCRIPTOR_ADD_HASH(icon_theme_edd, Efreet_Cache_Icon_Theme,
                                     "dirs", dirs, efreet_icon_directory_edd());
    }

    return icon_theme_edd;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI Eet_Data_Descriptor *
efreet_icon_fallback_edd(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (icon_fallback_edd) return icon_fallback_edd;

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Fallback_Icon);
    icon_fallback_edd = eet_data_descriptor_file_new(&eddc);
    if (!icon_fallback_edd) return NULL;

    EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY_STRING(icon_fallback_edd,
                                             Efreet_Cache_Fallback_Icon, "icons", icons);

    return icon_fallback_edd;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI Eet_Data_Descriptor *
efreet_desktop_edd(void)
{
    Eet_Data_Descriptor_Class eddc;

    if (desktop_edd) return desktop_edd;

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Desktop_Action);
    desktop_action_edd = eet_data_descriptor_file_new(&eddc);
    if (!desktop_action_edd) return NULL;

    /* Desktop Spec 1.1 */
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_action_edd, Efreet_Desktop_Action, "key", key, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_action_edd, Efreet_Desktop_Action, "name", name, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_action_edd, Efreet_Desktop_Action, "icon", icon, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_action_edd, Efreet_Desktop_Action, "exec", exec, EET_T_STRING);

    EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Efreet_Cache_Desktop);
    desktop_edd = eet_data_descriptor_file_new(&eddc);
    if (!desktop_edd) return NULL;

    /* Desktop Spec 1.0 */
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "type", desktop.type, EET_T_INT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "version", desktop.version, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "orig_path", desktop.orig_path, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "load_time", desktop.load_time, EET_T_LONG_LONG);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "name", desktop.name, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "generic_name", desktop.generic_name, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "comment", desktop.comment, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "icon", desktop.icon, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "try_exec", desktop.try_exec, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "exec", desktop.exec, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "path", desktop.path, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "startup_wm_class", desktop.startup_wm_class, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "url", desktop.url, EET_T_STRING);
    eet_data_descriptor_element_add(desktop_edd, "only_show_in", EET_T_STRING, EET_G_LIST, offsetof(Efreet_Cache_Desktop, desktop.only_show_in), 0, NULL, NULL);
    eet_data_descriptor_element_add(desktop_edd, "not_show_in", EET_T_STRING, EET_G_LIST, offsetof(Efreet_Cache_Desktop, desktop.not_show_in), 0, NULL, NULL);
    eet_data_descriptor_element_add(desktop_edd, "categories", EET_T_STRING, EET_G_LIST, offsetof(Efreet_Cache_Desktop, desktop.categories), 0, NULL, NULL);
    eet_data_descriptor_element_add(desktop_edd, "mime_types", EET_T_STRING, EET_G_LIST, offsetof(Efreet_Cache_Desktop, desktop.mime_types), 0, NULL, NULL);
    eet_data_descriptor_element_add(desktop_edd, "x", EET_T_STRING, EET_G_HASH, offsetof(Efreet_Cache_Desktop, desktop.x), 0, NULL, NULL);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "no_display", desktop.no_display, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "hidden", desktop.hidden, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "terminal", desktop.terminal, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "startup_notify", desktop.startup_notify, EET_T_UCHAR);

    /* Desktop Spec 1.1 */
    EET_DATA_DESCRIPTOR_ADD_BASIC(desktop_edd, Efreet_Cache_Desktop, "dbus_activatable", desktop.dbus_activatable, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_LIST(desktop_edd, Efreet_Cache_Desktop,
                                 "actions", desktop.actions, desktop_action_edd);
    eet_data_descriptor_element_add(desktop_edd, "implements", EET_T_STRING, EET_G_LIST, offsetof(Efreet_Cache_Desktop, desktop.implements), 0, NULL, NULL);
    eet_data_descriptor_element_add(desktop_edd, "keywords", EET_T_STRING, EET_G_LIST, offsetof(Efreet_Cache_Desktop, desktop.keywords), 0, NULL, NULL);

    return desktop_edd;
}

Efreet_Cache_Icon *
efreet_cache_icon_find(Efreet_Icon_Theme *theme, const char *icon)
{
    Efreet_Cache_Icon *cache = NULL;

    if (theme_name && strcmp(theme_name, theme->name.internal))
    {
        /* FIXME: this is bad if people have pointer to this cache, things will go wrong */
        INF("theme_name change from '%s' to '%s'", theme_name, theme->name.internal);
        IF_RELEASE(theme_name);
        icon_cache = efreet_cache_close(icon_cache);
        eina_hash_free(icons);
        icons = eina_hash_string_superfast_new(EINA_FREE_CB(efreet_cache_icon_free));
    }

    if (!efreet_cache_check(&icon_cache, efreet_icon_cache_file(theme->name.internal), EFREET_ICON_CACHE_MAJOR)) return NULL;
    if (!theme_name)
        theme_name = eina_stringshare_add(theme->name.internal);

    cache = eina_hash_find(icons, icon);
    if (cache == NON_EXISTING) return NULL;
    if (cache) return cache;

    cache = eet_data_read(icon_cache, efreet_icon_edd(), icon);
    if (cache)
        eina_hash_add(icons, icon, cache);
    else
        eina_hash_add(icons, icon, NON_EXISTING);
    return cache;
}

Efreet_Cache_Fallback_Icon *
efreet_cache_icon_fallback_find(const char *icon)
{
    Efreet_Cache_Fallback_Icon *cache;

    if (!efreet_cache_check(&fallback_cache, efreet_icon_cache_file(EFREET_CACHE_ICON_FALLBACK), EFREET_ICON_CACHE_MAJOR)) return NULL;

    cache = eina_hash_find(fallbacks, icon);
    if (cache == NON_EXISTING) return NULL;
    if (cache) return cache;

    cache = eet_data_read(fallback_cache, efreet_icon_fallback_edd(), icon);
    if (cache)
        eina_hash_add(fallbacks, icon, cache);
    else
        eina_hash_add(fallbacks, icon, NON_EXISTING);
    return cache;
}

Efreet_Icon_Theme *
efreet_cache_icon_theme_find(const char *theme)
{
    Efreet_Cache_Icon_Theme *cache;

    if (!efreet_cache_check(&icon_theme_cache, efreet_icon_theme_cache_file(), EFREET_ICON_CACHE_MAJOR)) return NULL;

    cache = eina_hash_find(themes, theme);
    if (cache == NON_EXISTING) return NULL;
    if (cache) return &(cache->theme);

    cache = eet_data_read(icon_theme_cache, efreet_icon_theme_edd(EINA_FALSE), theme);
    if (cache)
    {
        eina_hash_add(themes, theme, cache);
        return &(cache->theme);
    }
    else
        eina_hash_add(themes, theme, NON_EXISTING);
    return NULL;
}

static void
efreet_cache_icon_free(Efreet_Cache_Icon *icon)
{
    unsigned int i;

    if (!icon) return;
    if (icon == NON_EXISTING) return;

    for (i = 0; i < icon->icons_count; ++i)
    {
        free(icon->icons[i]->paths);
        free(icon->icons[i]);
    }

    free(icon->icons);
    free(icon);
}

static void
efreet_cache_icon_fallback_free(Efreet_Cache_Fallback_Icon *icon)
{
    if (!icon) return;
    if (icon == NON_EXISTING) return;

    free(icon->icons);
    free(icon);
}

static void
efreet_cache_icon_theme_free(Efreet_Icon_Theme *theme)
{
    void *data;

    if (!theme) return;
    if (theme == NON_EXISTING) return;

    eina_list_free(theme->paths);
    eina_list_free(theme->inherits);
    EINA_LIST_FREE(theme->directories, data)
        free(data);

    free(theme);
}

Eina_List *
efreet_cache_icon_theme_list(void)
{
    Eina_List *ret = NULL;
    char **keys;
    int i, num;

    if (!efreet_cache_check(&icon_theme_cache, efreet_icon_theme_cache_file(), EFREET_ICON_CACHE_MAJOR)) return NULL;
    keys = eet_list(icon_theme_cache, "*", &num);
    if (!keys) return NULL;
    for (i = 0; i < num; i++)
    {
        Efreet_Icon_Theme *theme;
        if (!strncmp(keys[i], "__efreet", 8)) continue;

        theme = eina_hash_find(themes, keys[i]);
        if (!theme)
            theme = efreet_cache_icon_theme_find(keys[i]);
        if (theme && theme != NON_EXISTING)
            ret = eina_list_append(ret, theme);
    }
    free(keys);
    return ret;
}

/*
 * Needs EAPI because of helper binaries
 */
EAPI void
efreet_cache_array_string_free(Efreet_Cache_Array_String *array)
{
    if (!array) return;
    free(array->array);
    free(array);
}

Efreet_Desktop *
efreet_cache_desktop_find(const char *file)
{
    Efreet_Cache_Desktop *cache = NULL;

    eina_lock_take(&_lock);
    if (!efreet_cache_check(&desktop_cache, efreet_desktop_cache_file(), EFREET_DESKTOP_CACHE_MAJOR)) goto error;

    cache = eina_hash_find(desktops, file);
    if (cache == NON_EXISTING) goto error;
    if (cache)
    {
        /* If less than one second since last stat, return desktop */
        if ((ecore_time_get() - cache->check_time) < 1)
        {
            INF("Return without stat %f %f for file '%s'", ecore_time_get(), cache->check_time, file);
            eina_lock_release(&_lock);
            return &cache->desktop;
        }
        if (cache->desktop.load_time == ecore_file_mod_time(cache->desktop.orig_path))
        {
            INF("Return with stat %f %f for file '%s'", ecore_time_get(), cache->check_time, file);
            cache->check_time = ecore_time_get();
            eina_lock_release(&_lock);
            return &cache->desktop;
        }

        /* We got stale data. The desktop will be free'd eventually as
         * users will call efreet_desktop_free */
        eina_hash_set(desktops, file, NON_EXISTING);
        cache = NULL;
    }

    cache = eet_data_read(desktop_cache, efreet_desktop_edd(), file);
    if (cache)
    {
        if (cache->desktop.load_time != ecore_file_mod_time(cache->desktop.orig_path))
        {
            /* Don't return stale data */
            INF("We got stale data in the desktop cache for file '%s'", cache->desktop.orig_path);
            efreet_cache_desktop_free(&cache->desktop);
            eina_hash_set(desktops, file, NON_EXISTING);
        }
        else
        {
            cache->desktop.eet = 1;
            cache->check_time = ecore_time_get();
            eina_hash_set(desktops, cache->desktop.orig_path, cache);
            eina_lock_release(&_lock);
            return &cache->desktop;
        }
    }
    else
        eina_hash_set(desktops, file, NON_EXISTING);
error:
    eina_lock_release(&_lock);
    return NULL;
}

void
efreet_cache_desktop_free(Efreet_Desktop *desktop)
{
    Efreet_Old_Cache *d;
    Efreet_Desktop *curr;
    Eina_List *l;

    if (!desktop ||
        desktop == NON_EXISTING ||
        !desktop->eet) return;

    eina_lock_take(&_lock);
    curr = eina_hash_find(desktops, desktop->orig_path);
    if (curr == desktop)
    {
        INF("Found '%s' in current cache, purge", desktop->orig_path);
        eina_hash_del_by_key(desktops, desktop->orig_path);
    }

    EINA_LIST_FOREACH(old_desktop_caches, l, d)
    {
        curr = eina_hash_find(d->hash, desktop->orig_path);
        if (curr == desktop)
        {
            INF("Found '%s' in old cache, purge", desktop->orig_path);
            eina_hash_del_by_key(d->hash, desktop->orig_path);
            if (eina_hash_population(d->hash) == 0)
            {
                INF("Cache empty, close file");
                eina_hash_free(d->hash);
                eet_close(d->ef);
                free(d);
                old_desktop_caches = eina_list_remove_list(old_desktop_caches, l);
            }
            break;
        }
    }

    /* Desktop Spec 1.0 */
    eina_list_free(desktop->only_show_in);
    eina_list_free(desktop->not_show_in);
    eina_list_free(desktop->categories);
    eina_list_free(desktop->mime_types);
    IF_FREE_HASH(desktop->x);
    /* Desktop Spec 1.1 */
    eina_list_free(desktop->actions);
    eina_list_free(desktop->implements);
    eina_list_free(desktop->keywords);

    free(desktop);
    eina_lock_release(&_lock);
}

void
efreet_cache_desktop_add(Efreet_Desktop *desktop)
{
   char *path;

   if (!efreet_cache_update) return;
   if (!eina_main_loop_is()) return;
    /*
     * TODO: Call in thread with:
     *   ecore_thread_main_loop_begin();
     *  ecore_thread_main_loop_end();
    */
   path = ecore_file_dir_get(desktop->orig_path);
   if (!path) return;
   _desktop_add_signal_send(path, strlen(path));
   free(path);
}

void
efreet_cache_icon_exts_add(Eina_List *exts)
{
   Eina_List *l;
   const char *s;
   Eina_Binbuf *buf;
   int num = 0;
   unsigned char nil[1] = { 0 };

   if (!efreet_cache_update) return;
   buf = eina_binbuf_new();
   if (!buf) return;
   EINA_LIST_FOREACH(exts, l, s)
     {
        if (num > 0) eina_binbuf_append_length(buf, nil, 1);
        eina_binbuf_append_length(buf, (unsigned char *)s, strlen(s));
        num++;
     }
   _icon_exts_add_signal_send(eina_binbuf_string_get(buf)
                            , eina_binbuf_length_get(buf));
   eina_binbuf_free(buf);
}

void
efreet_cache_icon_dirs_add(Eina_List *dirs)
{
   Eina_List *l;
   const char *s;
   Eina_Binbuf *buf;
   int num = 0;
   unsigned char nil[1] = { 0 };

   if (!efreet_cache_update) return;
   buf = eina_binbuf_new();
   if (!buf) return;
   EINA_LIST_FOREACH(dirs, l, s)
     {
        if (num > 0) eina_binbuf_append_length(buf, nil, 1);
        eina_binbuf_append_length(buf, (unsigned char *)s, strlen(s));
        num++;
     }
   _icon_dirs_add_signal_send(eina_binbuf_string_get(buf)
                            , eina_binbuf_length_get(buf));
   eina_binbuf_free(buf);
}

void
efreet_cache_desktop_close(void)
{
    IF_RELEASE(util_cache_names_key);
    IF_RELEASE(util_cache_hash_key);

    eina_lock_take(&_lock);
    if ((desktop_cache) && (desktop_cache != NON_EXISTING))
    {
        Efreet_Old_Cache *d = NEW(Efreet_Old_Cache, 1);
        if (d)
        {
            d->hash = desktops;
            d->ef = desktop_cache;
            old_desktop_caches = eina_list_append(old_desktop_caches, d);
        }

        desktops = eina_hash_string_superfast_new(NULL);
    }
    desktop_cache = NULL;
    IF_RELEASE(desktop_cache_file);
    eina_lock_release(&_lock);

    efreet_cache_array_string_free(util_cache_names);
    util_cache_names = NULL;

    if (util_cache_hash)
    {
        eina_hash_free(util_cache_hash->hash);
        free(util_cache_hash);
        util_cache_hash = NULL;
    }

    util_cache = efreet_cache_close(util_cache);
    IF_RELEASE(util_cache_file);
}

void
efreet_cache_desktop_build(void)
{
   const char *s;
   int len = 0;
   if (!efreet_cache_update)  return;

   s = efreet_language_get();
   if (s) len = strlen(s);
   _desktop_build_signal_send(s, len);
}

static Eina_Bool
efreet_cache_check(Eet_File **ef, const char *path, int major)
{
    Efreet_Cache_Version *version;

    if (*ef == NON_EXISTING) return EINA_FALSE;
    if (*ef) return EINA_TRUE;
    if (!*ef)
        *ef = eet_open(path, EET_FILE_MODE_READ);
    if (!*ef)
    {
        *ef = NON_EXISTING;
        return EINA_FALSE;
    }

    version = eet_data_read(*ef, efreet_version_edd(), EFREET_CACHE_VERSION);
    if ((!version) || (version->major != major))
    {
        IF_FREE(version);
        eet_close(*ef);
        *ef = NON_EXISTING;
        return EINA_FALSE;
    }
    free(version);
    return EINA_TRUE;
}

static void *
efreet_cache_close(Eet_File *ef)
{
    if (ef && ef != NON_EXISTING)
        eet_close(ef);
    return NULL;
}

Efreet_Cache_Hash *
efreet_cache_util_hash_string(const char *key)
{
    if (util_cache_hash_key && !strcmp(key, util_cache_hash_key))
        return util_cache_hash;
    if (!efreet_cache_check(&util_cache, efreet_desktop_util_cache_file(), EFREET_DESKTOP_UTILS_CACHE_MAJOR)) return NULL;

    if (util_cache_hash)
    {
        /* free previous util_cache */
        IF_RELEASE(util_cache_hash_key);
        eina_hash_free(util_cache_hash->hash);
        free(util_cache_hash);
    }
    util_cache_hash_key = eina_stringshare_add(key);
    util_cache_hash = eet_data_read(util_cache, efreet_hash_string_edd(), key);
    return util_cache_hash;
}

Efreet_Cache_Hash *
efreet_cache_util_hash_array_string(const char *key)
{
    if (util_cache_hash_key && !strcmp(key, util_cache_hash_key))
        return util_cache_hash;
    if (!efreet_cache_check(&util_cache, efreet_desktop_util_cache_file(), EFREET_DESKTOP_UTILS_CACHE_MAJOR)) return NULL;

    IF_RELEASE(util_cache_hash_key);
    if (util_cache_hash)
    {
        /* free previous cache */
        eina_hash_free(util_cache_hash->hash);
        free(util_cache_hash);
    }
    util_cache_hash_key = eina_stringshare_add(key);
    util_cache_hash = eet_data_read(util_cache, efreet_hash_array_string_edd(), key);
    return util_cache_hash;
}

Efreet_Cache_Array_String *
efreet_cache_util_names(const char *key)
{
    if (util_cache_names_key && !strcmp(key, util_cache_names_key))
        return util_cache_names;
    if (!efreet_cache_check(&util_cache, efreet_desktop_util_cache_file(), EFREET_DESKTOP_UTILS_CACHE_MAJOR)) return NULL;

    if (util_cache_names)
    {
        /* free previous util_cache */
        IF_RELEASE(util_cache_names_key);
        efreet_cache_array_string_free(util_cache_names);
    }
    util_cache_names_key = eina_stringshare_add(key);
    util_cache_names = eet_data_read(util_cache, efreet_array_string_edd(), key);
    return util_cache_names;
}

static void
icon_cache_update_free(void *data, void *ev)
{
    Efreet_Old_Cache *d;
    Eina_List *l;

    l = data;
    EINA_LIST_FREE(l, d)
    {
        if (d->hash)
            eina_hash_free(d->hash);
        efreet_cache_close(d->ef);
        free(d);
    }
    free(ev);
}

static void *
hash_array_string_add(void *hash, const char *key, void *data)
{
    if (!hash)
        hash = eina_hash_string_superfast_new(EINA_FREE_CB(efreet_cache_array_string_free));
    if (!hash)
        return NULL;
    eina_hash_add(hash, key, data);
    return hash;
}
