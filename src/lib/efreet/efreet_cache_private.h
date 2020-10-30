#ifndef EFREET_CACHE_PRIVATE_H
#define EFREET_CACHE_PRIVATE_H

#define EFREET_DESKTOP_CACHE_MAJOR 2
#define EFREET_DESKTOP_CACHE_MINOR 0
#define EFREET_DESKTOP_UTILS_CACHE_MAJOR 1
#define EFREET_DESKTOP_UTILS_CACHE_MINOR 0

#define EFREET_ICON_CACHE_MAJOR 1
#define EFREET_ICON_CACHE_MINOR 0

#define EFREET_CACHE_VERSION "__efreet//version"
#define EFREET_CACHE_ICON_FALLBACK "__efreet_fallback"

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

extern int _efreet_cache_log_dom;
extern Eina_Bool disable_cache;
extern Eina_Bool run_in_tree;

typedef struct _Efreet_Cache_Check Efreet_Cache_Check;

typedef struct _Efreet_Cache_Icon_Theme Efreet_Cache_Icon_Theme;
typedef struct _Efreet_Cache_Directory Efreet_Cache_Directory;
typedef struct _Efreet_Cache_Desktop Efreet_Cache_Desktop;

EAPI const char *efreet_desktop_util_cache_file(void);
EAPI const char *efreet_desktop_cache_file(void);
EAPI const char *efreet_icon_cache_file(const char *theme);
EAPI const char *efreet_icon_theme_cache_file(void);

EAPI Eina_Bool efreet_file_cache_fill(const char *file, Efreet_Cache_Check *check);
EAPI Eina_Bool efreet_file_cache_check(const Efreet_Cache_Check *check1, const Efreet_Cache_Check *check2);

EAPI Eet_Data_Descriptor *efreet_version_edd(void);
EAPI Eet_Data_Descriptor *efreet_desktop_edd(void);
EAPI Eet_Data_Descriptor *efreet_hash_array_string_edd(void);
EAPI Eet_Data_Descriptor *efreet_hash_string_edd(void);
EAPI Eet_Data_Descriptor *efreet_array_string_edd(void);
EAPI Eet_Data_Descriptor *efreet_icon_theme_edd(Eina_Bool cache);
EAPI Eet_Data_Descriptor *efreet_icon_edd(void);
EAPI Eet_Data_Descriptor *efreet_icon_fallback_edd(void);

struct _Efreet_Cache_Check
{
   unsigned long long uid;
   unsigned long long gid;
   unsigned long long size;
   unsigned long long blocks;
   unsigned long long mtime;
   unsigned long long chtime;
   unsigned int       mode;
   unsigned char      link_sha1[20];
};

struct _Efreet_Cache_Icon_Theme
{
    Efreet_Icon_Theme theme;

    Efreet_Cache_Check check; /**< relevant stat info from last check */

    Eina_Hash *dirs;            /**< All possible icon paths for this theme */

    const char *path;           /**< path to index.theme */

    Eina_Bool hidden:1;         /**< Should this theme be hidden from users */
    Eina_Bool valid:1;          /**< Have we seen an index for this theme */
    Eina_Bool changed:1;        /**< Changed since last seen */
};

struct _Efreet_Cache_Directory
{
    Efreet_Cache_Check check; /**< relevant stat info from last check */
};

struct _Efreet_Cache_Desktop
{
    Efreet_Desktop desktop;

    Efreet_Cache_Check check; /**< relevant stat info from last check */
    double check_time; /**< Last time we check for disk modification */
};

// private efreet_cache functions declarations
void _icon_desktop_cache_update_event_add(int event_type);

// private efreet_cache{local/ipc} functions declarations
Eina_Bool _efreet_cache_reset_language(const char *s, int len);
void _desktop_build_signal_send();
void _desktop_add_signal_send(char * path, int path_len);
void _icon_exts_add_signal_send(const unsigned char *buf, int length);
void _icon_dirs_add_signal_send(const unsigned char *buf, int length);
void _server_config_signal_send();

#ifdef HAVE_EFREETD
void _ipc_data_clean();
#endif /* HAVE_EFREETD */


#undef EAPI
#define EAPI

#endif
