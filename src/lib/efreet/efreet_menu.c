#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Ecore.h>
#include <Ecore_File.h>

/* TODO: Don't scan app dirs if they are cached, read values from cache */

/* define macros and variable for using the eina logging system  */
#define EFREET_MODULE_LOG_DOM _efreet_menu_log_dom
static int _efreet_menu_log_dom = -1;

#include "Efreet.h"
#include "efreet_private.h"
#include "efreet_xml.h"


typedef struct Efreet_Menu_Move Efreet_Menu_Move;

struct Efreet_Menu_Move
{
    const char *old_name;     /**< The menu path to move from */
    const char *new_name;     /**< The menu path to move too */
};

typedef struct Efreet_Menu_Internal Efreet_Menu_Internal;

struct Efreet_Menu_Internal
{
    struct
    {
        const char *path;         /**< The base file path */
        const char *name;         /**< The filename for this menu */
    } file;                 /**< The menu file information */

    struct
    {
        const char *internal;     /**< The menu name */
        const char *name;         /**< Name to use in the menus */
    } name;                       /**< The names for this menu */

    Eina_Hash *efreet_merged_menus; /**< Merged menus */
    Eina_Hash *efreet_merged_dirs; /**< Merged dirs */

    Efreet_Desktop *directory; /**< The directory */
    Eina_List *directories;  /**< All the directories set in the menu file */

    Efreet_Menu_Move *current_move; /**< The current move */

    Eina_List *app_dirs;           /**< .desktop application directories */

    Eina_List *app_pool;           /**< application pool */
    Eina_List *applications;       /**< applications in this menu */

    Eina_List *directory_dirs;    /**< .directory file directories */
    Eina_Hash *directory_cache;    /**< .directory dirs */

    Eina_List *moves;              /**< List of moves to be handled by the menu */
    Eina_List *filters;            /**< Include and Exclude filters */

    Efreet_Menu_Internal *parent;   /**< Our parent menu */
    Eina_List *sub_menus;          /**< Our sub menus */

    Eina_List *layout;             /**< This menus layout */
    Eina_List *default_layout;     /**< Default layout */
    signed char show_empty;    /**< Whether to show empty menus */
    signed char in_line;       /**< Whether this meny can be inlined */
    signed char inline_limit;  /**< Number of elements which triggers inline */
    signed char inline_header; /**< Whether we should use the header name when this menu is inlined */
    signed char inline_alias;  /**< Whether we should use the menu name when inlining */

    unsigned char seen_allocated:1;     /**< have we set the only_unallocated */
    unsigned char only_unallocated:1;   /**< Show only unallocated .desktops */

    unsigned char seen_deleted:1;       /**< Have we seen the deleted item yet */
    unsigned char deleted:1;            /**< The menu is deleted */
};

typedef struct Efreet_Menu_App_Dir Efreet_Menu_App_Dir;

struct Efreet_Menu_App_Dir
{
    const char *path;           /**< directory path */
    const char *prefix;         /**< If it's legacy it can have a prefix */
    unsigned int legacy:1;      /**< is this a legacy dir */
};

enum Efreet_Menu_Filter_Op_Type
{
    EFREET_MENU_FILTER_OP_OR,
    EFREET_MENU_FILTER_OP_AND,
    EFREET_MENU_FILTER_OP_NOT
};

typedef enum Efreet_Menu_Filter_Op_Type Efreet_Menu_Filter_Op_Type;

enum Efreet_Menu_Filter_Type
{
    EFREET_MENU_FILTER_INCLUDE,
    EFREET_MENU_FILTER_EXCLUDE
};

typedef enum Efreet_Menu_Filter_Type Efreet_Menu_Filter_Type;

typedef struct Efreet_Menu_Filter_Op Efreet_Menu_Filter_Op;

struct Efreet_Menu_Filter_Op
{
    Efreet_Menu_Filter_Op_Type type; /**< The type of operation */
    Eina_List *categories;          /**< The categories this op applies too */
    Eina_List *filenames;           /**< The filenames this op applies too */

    Eina_List *filters;             /**< Child filters */

    unsigned char all:1;             /**< Applies to all .desktop files */
};

typedef struct Efreet_Menu_Filter Efreet_Menu_Filter;

struct Efreet_Menu_Filter
{
    Efreet_Menu_Filter_Type type;   /**< The type of filter */
    Efreet_Menu_Filter_Op *op;      /**< The filter operations */
};

enum Efreet_Menu_Layout_Type
{
    EFREET_MENU_LAYOUT_MENUNAME,
    EFREET_MENU_LAYOUT_FILENAME,
    EFREET_MENU_LAYOUT_SEPARATOR,
    EFREET_MENU_LAYOUT_MERGE
};

typedef enum Efreet_Menu_Layout_Type Efreet_Menu_Layout_Type;

typedef struct Efreet_Menu_Layout Efreet_Menu_Layout;

struct Efreet_Menu_Layout
{
    Efreet_Menu_Layout_Type  type;   /**< The type of layout */
    const char *name;                /**< The name of the element */

    /* The items below are for Menuname Layout elements */
    signed char show_empty;    /**< Whether to show empty menus */
    signed char in_line;       /**< Whether this meny can be inlined */
    signed char inline_limit;  /**< Number of elements which triggers inline */
    signed char inline_header; /**< Whether we should use the header name when this menu is inlined */
    signed char inline_alias;  /**< Whether we should use the menu name when inlining */
};

typedef struct Efreet_Menu_Desktop Efreet_Menu_Desktop;

struct Efreet_Menu_Desktop
{
    Efreet_Desktop *desktop;   /**< The desktop we refer too */
    const char *id;            /**< The desktop file id */
    unsigned char allocated:1; /**< If this desktop has been allocated */
};

typedef struct Efreet_Menu_Async Efreet_Menu_Async;

struct Efreet_Menu_Async
{
    Efreet_Menu_Cb    func;
    void             *data;
    Eina_Stringshare *path;
    Efreet_Menu      *menu;
};

static const char *efreet_menu_prefix = NULL; /**< The $XDG_MENU_PREFIX env var */
Eina_List *efreet_menu_kde_legacy_dirs = NULL; /**< The directories to use for KDELegacy entries */
static const char *efreet_tag_menu = NULL;
static const char *efreet_menu_file = NULL; /**< A menu file set explicityl as default */

static Eina_Hash *efreet_menu_handle_cbs = NULL;
static Eina_Hash *efreet_menu_filter_cbs = NULL;
static Eina_Hash *efreet_menu_move_cbs = NULL;
static Eina_Hash *efreet_menu_layout_cbs = NULL;

static Efreet_Menu_Internal *efreet_menu_by_name_find(Efreet_Menu_Internal *internal,
                                                    const char *name,
                                                    Efreet_Menu_Internal **parent);
static int efreet_menu_cb_compare_names(Efreet_Menu_Internal *internal, const char *name);
static int efreet_menu_cb_md_compare_ids(Efreet_Menu_Desktop *md, const char *name);

static int efreet_menu_cb_entry_compare_menu(Efreet_Menu *entry, Efreet_Menu_Internal *internal);
static int efreet_menu_cb_entry_compare_desktop(Efreet_Menu *entry, Efreet_Desktop *desktop);

#ifndef STRICT_SPEC
static int efreet_menu_cb_move_compare(Efreet_Menu_Move *move, const char *old);
#endif

static int efreet_menu_process(Efreet_Menu_Internal *internal, unsigned int only_unallocated);
static int efreet_menu_process_dirs(Efreet_Menu_Internal *internal);
static int efreet_menu_app_dirs_process(Efreet_Menu_Internal *internal);
static int efreet_menu_app_dir_scan(Efreet_Menu_Internal *internal,
                                        const char *path,
                                        const char *id,
                                        int legacy);
static int efreet_menu_directory_dirs_process(Efreet_Menu_Internal *internal);
static int efreet_menu_directory_dir_scan(const char *path,
                                            const char *relative_path,
                                            Eina_Hash *cache);
static Efreet_Desktop *efreet_menu_directory_get(Efreet_Menu_Internal *internal,
                                                    const char *path);
static void efreet_menu_process_filters(Efreet_Menu_Internal *internal,
                                            unsigned int only_unallocated);
static Eina_List *efreet_menu_process_app_pool(Eina_List *pool,
                                               Eina_List *applications,
                                               Eina_Hash *matches,
                                               Efreet_Menu_Filter *filter,
                                               unsigned int only_unallocated);
static int efreet_menu_filter_matches(Efreet_Menu_Filter_Op *op,
                                        Efreet_Menu_Desktop *md);
static int efreet_menu_filter_or_matches(Efreet_Menu_Filter_Op *op,
                                            Efreet_Menu_Desktop *md);
static int efreet_menu_filter_and_matches(Efreet_Menu_Filter_Op *op,
                                            Efreet_Menu_Desktop *md);
static int efreet_menu_filter_not_matches(Efreet_Menu_Filter_Op *op,
                                            Efreet_Menu_Desktop *md);

static Efreet_Menu *efreet_menu_layout_menu(Efreet_Menu_Internal *internal);
static Efreet_Menu *efreet_menu_layout_desktop(Efreet_Menu_Desktop *md);
static void efreet_menu_layout_entries_get(Efreet_Menu *entry, Efreet_Menu_Internal *internal,
                                            Efreet_Menu_Layout *layout);
static int efreet_menu_layout_is_empty(Efreet_Menu *entry);

static Efreet_Menu_Internal *efreet_menu_internal_new(Efreet_Menu_Internal *parent);
static void efreet_menu_internal_free(Efreet_Menu_Internal *internal);
static void efreet_menu_create_sub_menu_list(Efreet_Menu_Internal *internal);
static void efreet_menu_create_app_dirs_list(Efreet_Menu_Internal *internal);
static void efreet_menu_create_directory_dirs_list(Efreet_Menu_Internal *internal);
static void efreet_menu_create_directories_list(Efreet_Menu_Internal *internal);
static void efreet_menu_create_move_list(Efreet_Menu_Internal *internal);
static void efreet_menu_create_filter_list(Efreet_Menu_Internal *internal);
static void efreet_menu_create_layout_list(Efreet_Menu_Internal *internal);
static void efreet_menu_create_default_layout_list(Efreet_Menu_Internal *internal);
static const char *efreet_menu_path_get(Efreet_Menu_Internal *internal, const char *suffix);

static Efreet_Menu_App_Dir *efreet_menu_app_dir_new(void);
static void efreet_menu_app_dir_free(Efreet_Menu_App_Dir *dir);

static Efreet_Menu_Move *efreet_menu_move_new(void);
static void efreet_menu_move_free(Efreet_Menu_Move *move);

static Efreet_Menu_Filter *efreet_menu_filter_new(void);
static void efreet_menu_filter_free(Efreet_Menu_Filter *filter);

static Efreet_Menu_Layout *efreet_menu_layout_new(void);
static void efreet_menu_layout_free(Efreet_Menu_Layout *layout);

static Efreet_Menu_Filter_Op *efreet_menu_filter_op_new(void);
static void efreet_menu_filter_op_free(Efreet_Menu_Filter_Op *op);

static Efreet_Menu_Desktop *efreet_menu_desktop_new(void);
static void efreet_menu_desktop_free(Efreet_Menu_Desktop *md);

static Efreet_Menu *efreet_menu_entry_new(void);

static int efreet_menu_handle_menu(Efreet_Menu_Internal *internal, Efreet_Xml *xml);
static int efreet_menu_handle_name(Efreet_Menu_Internal *parent, Efreet_Xml *xml);

static int efreet_menu_handle_sub_menu(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_app_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_default_app_dirs(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_directory_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_default_directory_dirs(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_directory(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_only_unallocated(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_not_only_unallocated(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_deleted(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_not_deleted(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_include(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_exclude(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_filename(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml);
static int efreet_menu_handle_category(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml);
static int efreet_menu_handle_all(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml);
static int efreet_menu_handle_and(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml);
static int efreet_menu_handle_or(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml);
static int efreet_menu_handle_not(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml);
static int efreet_menu_handle_merge_file(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_merge_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_default_merge_dirs(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_legacy_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static Efreet_Menu_Internal *efreet_menu_handle_legacy_dir_helper(Efreet_Menu_Internal *root,
                                                Efreet_Menu_Internal *parent,
                                                const char *legacy_dir,
                                                const char *prefix);
static int efreet_menu_handle_kde_legacy_dirs(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_move(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_old(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_new(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_layout(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
static int efreet_menu_handle_default_layout(Efreet_Menu_Internal *parent, Efreet_Xml *xml);

static int efreet_menu_handle_filter(Efreet_Menu_Internal *parent, Efreet_Xml *xml,
                                                    Efreet_Menu_Filter_Type type);
static int efreet_menu_handle_filter_op(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml);
static int efreet_menu_handle_filter_child_op(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml,
                                                      Efreet_Menu_Filter_Op_Type type);

static int efreet_menu_handle_layout_menuname(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def);
static int efreet_menu_handle_layout_filename(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def);
static int efreet_menu_handle_layout_separator(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def);
static int efreet_menu_handle_layout_merge(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def);

static int efreet_menu_merge(Efreet_Menu_Internal *parent, Efreet_Xml *xml, const char *path);
static int efreet_menu_merge_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml, const char *path);

static int efreet_menu_cb_app_dirs_compare(Efreet_Menu_App_Dir *a, const char *b);

static void efreet_menu_resolve_moves(Efreet_Menu_Internal *internal);
static void efreet_menu_concatenate(Efreet_Menu_Internal *dest, Efreet_Menu_Internal *src);

static int efreet_menu_cb_menu_compare(Efreet_Menu_Internal *a, Efreet_Menu_Internal *b);
static int efreet_menu_cb_md_compare(const Efreet_Menu_Desktop *a, const Efreet_Menu_Desktop *b);

static void efreet_menu_path_set(Efreet_Menu_Internal *internal, const char *path);

static int efreet_menu_save_menu(Efreet_Menu *menu, FILE *f, int indent);
static int efreet_menu_save_indent(FILE *f, int indent);

int
efreet_menu_init(void)
{
    int i;

    struct
    {
        const char *key;
        int (*cb)(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
    } menu_cbs[] = {
        {"Menu", efreet_menu_handle_sub_menu},
        {"AppDir", efreet_menu_handle_app_dir},
        {"DefaultAppDirs", efreet_menu_handle_default_app_dirs},
        {"DirectoryDir", efreet_menu_handle_directory_dir},
        {"DefaultDirectoryDirs", efreet_menu_handle_default_directory_dirs},
        {"Name", efreet_menu_handle_name},
        {"Directory", efreet_menu_handle_directory},
        {"OnlyUnallocated", efreet_menu_handle_only_unallocated},
        {"NotOnlyUnallocated", efreet_menu_handle_not_only_unallocated},
        {"Deleted", efreet_menu_handle_deleted},
        {"NotDeleted", efreet_menu_handle_not_deleted},
        {"Include", efreet_menu_handle_include},
        {"Exclude", efreet_menu_handle_exclude},
        {"MergeFile", efreet_menu_handle_merge_file},
        {"MergeDir", efreet_menu_handle_merge_dir},
        {"DefaultMergeDirs", efreet_menu_handle_default_merge_dirs},
        {"LegacyDir", efreet_menu_handle_legacy_dir},
        {"KDELegacyDirs", efreet_menu_handle_kde_legacy_dirs},
        {"Move", efreet_menu_handle_move},
        {"Layout", efreet_menu_handle_layout},
        {"DefaultLayout", efreet_menu_handle_default_layout},
        {NULL, NULL}
    };

    struct
    {
        const char *key;
        int (*cb)(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml);
    } filter_cbs[] = {
        {"Filename", efreet_menu_handle_filename},
        {"Category", efreet_menu_handle_category},
        {"All", efreet_menu_handle_all},
        {"And", efreet_menu_handle_and},
        {"Or", efreet_menu_handle_or},
        {"Not", efreet_menu_handle_not},
        {NULL, NULL}
    };

    struct
    {
        const char *key;
        int (*cb)(Efreet_Menu_Internal *parent, Efreet_Xml *xml);
    } move_cbs[] = {
        {"Old", efreet_menu_handle_old},
        {"New", efreet_menu_handle_new},
        {NULL, NULL}
    };

    struct
    {
        const char *key;
        int (*cb)(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def);
    } layout_cbs[] = {
        {"Menuname", efreet_menu_handle_layout_menuname},
        {"Filename", efreet_menu_handle_layout_filename},
        {"Separator", efreet_menu_handle_layout_separator},
        {"Merge", efreet_menu_handle_layout_merge},
        {NULL, NULL}
    };

    _efreet_menu_log_dom = eina_log_domain_register
      ("efreet_menu", EFREET_DEFAULT_LOG_COLOR);
    if (_efreet_menu_log_dom < 0)
    {
        EINA_LOG_ERR("Efreet: Could not create a log domain for efreet_menu");
        return 0;
    }

#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
   if (getuid() == geteuid())
#endif
     efreet_menu_prefix = getenv("XDG_MENU_PREFIX");
   if (!efreet_menu_prefix) efreet_menu_prefix = "";

    efreet_menu_handle_cbs = eina_hash_string_superfast_new(NULL);
    efreet_menu_filter_cbs = eina_hash_string_superfast_new(NULL);
    efreet_menu_move_cbs = eina_hash_string_superfast_new(NULL);
    efreet_menu_layout_cbs = eina_hash_string_superfast_new(NULL);
    if (!efreet_menu_handle_cbs || !efreet_menu_filter_cbs
            || !efreet_menu_move_cbs || !efreet_menu_layout_cbs)
    {
        eina_log_domain_unregister(_efreet_menu_log_dom);
        _efreet_menu_log_dom = -1;
        return 0;
    }

    /* set Menu into it's own so we can check the XML is valid before trying
     * to handle it */
    efreet_tag_menu = eina_stringshare_add(menu_cbs[0].key);

    for (i = 0; menu_cbs[i].key; i++)
    {
        eina_hash_set(efreet_menu_handle_cbs,
                        menu_cbs[i].key,
                        menu_cbs[i].cb);
    }
    for (i = 0; filter_cbs[i].key; i++)
    {
        eina_hash_set(efreet_menu_filter_cbs,
                        filter_cbs[i].key,
                        filter_cbs[i].cb);
    }
    for (i = 0; move_cbs[i].key; i++)
    {
        eina_hash_set(efreet_menu_move_cbs,
                        move_cbs[i].key,
                        move_cbs[i].cb);
    }
    for (i = 0; layout_cbs[i].key; i++)
    {
        eina_hash_set(efreet_menu_layout_cbs,
                        layout_cbs[i].key,
                        layout_cbs[i].cb);
    }
    return 1;
}

EFREET_API int
efreet_menu_kde_legacy_init(void)
{
    FILE *f;
    char buf[PATH_MAX];
    char *p, *s;

    IF_FREE_LIST(efreet_menu_kde_legacy_dirs, eina_stringshare_del);

    f = popen("kde-config --path apps", "r");
    if (!f) return 0;

    /* XXX if the return from kde-config is a line longer than PATH_MAX,
     * this won't be correct (increase buffer and get the rest...) */
    if (!fgets(buf, sizeof(buf), f))
    {
        ERR("Error initializing KDE legacy information");
        pclose(f);
        return 0;
    }
    s = buf;

    p = strchr(s, ':');
    while (p)
    {
        *p = '\0';
        efreet_menu_kde_legacy_dirs = eina_list_append(efreet_menu_kde_legacy_dirs,
                            (void *)eina_stringshare_add(s));
        s = p + 1;
        p = strchr(s, ':');
    }

    if (*s)
        efreet_menu_kde_legacy_dirs = eina_list_append(efreet_menu_kde_legacy_dirs,
                            (void *)eina_stringshare_add(s));

    pclose(f);
    return 1;
}

void
efreet_menu_shutdown(void)
{
    IF_RELEASE(efreet_menu_file);

    IF_FREE_HASH(efreet_menu_handle_cbs);
    IF_FREE_HASH(efreet_menu_filter_cbs);
    IF_FREE_HASH(efreet_menu_move_cbs);
    IF_FREE_HASH(efreet_menu_layout_cbs);

    IF_FREE_LIST(efreet_menu_kde_legacy_dirs, eina_stringshare_del);

    IF_RELEASE(efreet_tag_menu);

    eina_log_domain_unregister(_efreet_menu_log_dom);
    _efreet_menu_log_dom = -1;
}

EFREET_API Efreet_Menu *
efreet_menu_new(const char *name)
{
    Efreet_Menu *menu;

    EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);

    menu = efreet_menu_entry_new();
    menu->type = EFREET_MENU_ENTRY_MENU;
    menu->name = eina_stringshare_add(name);
    return menu;
}

EFREET_API void
efreet_menu_file_set(const char *file)
{
    IF_RELEASE(efreet_menu_file);
    efreet_menu_file = NULL;
    if (file) efreet_menu_file = eina_stringshare_add(file);
}

/* deprecated */
EFREET_DEPRECATED_API EFREET_API void
efreet_menu_async_get(Efreet_Menu_Cb func EINA_UNUSED, const void *data EINA_UNUSED)
{
    ERR("%s is deprecated and shouldn't be called", __func__);

    return;
}

EFREET_API Efreet_Menu *
efreet_menu_get(void)
{
    char menu[PATH_MAX];
    const char *dir;
    Eina_List *config_dirs, *l;

#ifndef STRICT_SPEC
    /* prefer user set menu */
    if (efreet_menu_file)
    {
        if (ecore_file_exists(efreet_menu_file))
            return efreet_menu_parse(efreet_menu_file);
    }
#endif

    /* check the users config directory first */
    snprintf(menu, sizeof(menu), "%s/menus/%sapplications.menu",
                        efreet_config_home_get(), efreet_menu_prefix);
    if (ecore_file_exists(menu))
        return efreet_menu_parse(menu);

    /* fallback to the XDG_CONFIG_DIRS */
    config_dirs = efreet_config_dirs_get();
    EINA_LIST_FOREACH(config_dirs, l, dir)
    {
        snprintf(menu, sizeof(menu), "%s/menus/%sapplications.menu",
                                    dir, efreet_menu_prefix);
        if (ecore_file_exists(menu))
            return efreet_menu_parse(menu);
    }

    return NULL;
}

/* deprecated */
EFREET_DEPRECATED_API EFREET_API void
efreet_menu_async_parse(const char *path EINA_UNUSED, Efreet_Menu_Cb func EINA_UNUSED, const void *data EINA_UNUSED)
{
    ERR("%s is deprecated and shouldn't be called", __func__);

    return;
}

EFREET_API Efreet_Menu *
efreet_menu_parse(const char *path)
{
    Efreet_Xml *xml;
    Efreet_Menu_Internal *internal = NULL;
    Efreet_Menu *entry = NULL;

    EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);

    xml = efreet_xml_new(path);
    if (!xml) return NULL;

    /* make sure we've got a <Menu> to start with */
    if (xml->tag != efreet_tag_menu)
    {
        WRN("Menu file didn't start with <Menu> tag: '%s'", path);
        efreet_xml_del(xml);
        return NULL;
    }

    /* split apart the filename and the path */
    internal = efreet_menu_internal_new(NULL);
    if (!internal)
    {
        efreet_xml_del(xml);
        return NULL;
    }
    internal->efreet_merged_menus = eina_hash_string_superfast_new(NULL);
    internal->efreet_merged_dirs = eina_hash_string_superfast_new(NULL);

    /* Set default values */
    internal->show_empty = 0;
    internal->in_line = 0;
    internal->inline_limit = 4;
    internal->inline_header = 1;
    internal->inline_alias = 0;

    efreet_menu_path_set(internal, path);
    if (!efreet_menu_handle_menu(internal, xml))
    {
        efreet_xml_del(xml);
        goto error;
    }
    efreet_xml_del(xml);

    efreet_menu_resolve_moves(internal);

    if (!efreet_menu_process_dirs(internal))
        goto error;

    /* handle all .desktops */
    if (!efreet_menu_process(internal, 0))
        goto error;

    /* handle menus with only unallocated .desktops */
    if (!efreet_menu_process(internal, 1))
        goto error;

    /* layout menu */
    entry = efreet_menu_layout_menu(internal);

error:
    IF_FREE_HASH(internal->efreet_merged_menus);
    IF_FREE_HASH(internal->efreet_merged_dirs);
    efreet_menu_internal_free(internal);
    return entry;
}

EFREET_API int
efreet_menu_save(Efreet_Menu *menu, const char *path)
{
    FILE *f;
    int ret;

    EINA_SAFETY_ON_NULL_RETURN_VAL(menu, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(path, 0);

    f = fopen(path, "wb");
    if (!f) return 0;
    fprintf(f, "<?xml version=\"1.0\"?>\n");
    fprintf(f, "<!DOCTYPE Menu PUBLIC \"-//freedesktop//DTD Menu 1.0//EN\" "
                "\"http://standards.freedesktop.org/menu-spec/menu-1.0.dtd\">\n");
    ret = efreet_menu_save_menu(menu, f, 0);
    fclose(f);
    return ret;
}

EFREET_API int
efreet_menu_desktop_insert(Efreet_Menu *menu, Efreet_Desktop *desktop, int pos)
{
    Efreet_Menu *entry;
    const char *id;

    EINA_SAFETY_ON_NULL_RETURN_VAL(menu, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(desktop, 0);

    id = efreet_util_path_to_file_id(desktop->orig_path);
    if (!id) return 0;

    entry = efreet_menu_entry_new();
    entry->type = EFREET_MENU_ENTRY_DESKTOP;
    entry->id = eina_stringshare_add(id);
    entry->name = eina_stringshare_add(desktop->name);
    if (desktop->icon) entry->icon = eina_stringshare_add(desktop->icon);
    efreet_desktop_ref(desktop);
    entry->desktop = desktop;

    if (pos < 0 || (unsigned int)pos >= eina_list_count(menu->entries))
        menu->entries = eina_list_append(menu->entries, entry);
    else
    {
        menu->entries = eina_list_append_relative(menu->entries, entry,
                                                  eina_list_nth(menu->entries, pos));
    }
    return 1;
}

EFREET_API int
efreet_menu_desktop_remove(Efreet_Menu *menu, Efreet_Desktop *desktop)
{
    Efreet_Menu *entry;

    EINA_SAFETY_ON_NULL_RETURN_VAL(menu, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(desktop, 0);

    entry = eina_list_search_unsorted(menu->entries,
                                      EINA_COMPARE_CB(efreet_menu_cb_entry_compare_desktop),
                            desktop);
    if (entry)
    {
        menu->entries = eina_list_remove(menu->entries, entry);
        efreet_menu_free(entry);
        return 1;
    }
    return 0;
}

EFREET_API void
efreet_menu_dump(Efreet_Menu *menu, const char *indent)
{
    Eina_List *l;

    EINA_SAFETY_ON_NULL_RETURN(menu);
    EINA_SAFETY_ON_NULL_RETURN(indent);

    INF("%s%s: ", indent, menu->name);
    INF("%s", (menu->icon ? menu->icon : "No icon"));

    /* XXX dump the rest of the menu info */

    if (menu->entries)
    {
        Efreet_Menu *entry;
        char *new_indent;
        size_t len;

        len = strlen(indent) + 3;
        new_indent = alloca(len);
        snprintf(new_indent, len, "%s  ", indent);

        EINA_LIST_FOREACH(menu->entries, l, entry)
        {
            if (entry->type == EFREET_MENU_ENTRY_SEPARATOR)
                INF("%s|---", new_indent);
            else if (entry->type == EFREET_MENU_ENTRY_DESKTOP)
                INF("%s|-%s", new_indent, entry->name);
            else if (entry->type == EFREET_MENU_ENTRY_MENU)
                efreet_menu_dump(entry, new_indent);
            else if (entry->type == EFREET_MENU_ENTRY_HEADER)
                INF("%s|---%s", new_indent, entry->name);
        }
    }
}

/**
 * @internal
 * @return Returns a new Efreet_Menu_Internal struct
 * @brief Allocates and initializes a new Efreet_Menu_Internal structure
 */
static Efreet_Menu_Internal *
efreet_menu_internal_new(Efreet_Menu_Internal *parent)
{
    Efreet_Menu_Internal *internal;

    internal = NEW(Efreet_Menu_Internal, 1);
    if (!internal) return NULL;
    internal->show_empty = -1;
    internal->in_line = -1;
    internal->inline_limit = -1;
    internal->inline_header = -1;
    internal->inline_alias = -1;

    if (parent)
    {
        internal->efreet_merged_menus = parent->efreet_merged_menus;
        internal->efreet_merged_dirs = parent->efreet_merged_dirs;
    }

    return internal;
}

/**
 * @param menu The menu to free
 * @return Returns no value
 * @brief Frees up the given menu structure
 */
void
efreet_menu_internal_free(Efreet_Menu_Internal *internal)
{
    if (!internal) return;

    IF_RELEASE(internal->file.path);
    IF_RELEASE(internal->file.name);

    IF_RELEASE(internal->name.internal);
    internal->name.name = NULL;

    internal->applications = eina_list_free(internal->applications);

    IF_FREE_LIST(internal->directories, eina_stringshare_del);
    IF_FREE_LIST(internal->app_dirs, efreet_menu_app_dir_free);
    IF_FREE_LIST(internal->app_pool, efreet_menu_desktop_free);
    IF_FREE_LIST(internal->directory_dirs, eina_stringshare_del);
    IF_FREE_HASH(internal->directory_cache);

    IF_FREE_LIST(internal->moves, efreet_menu_move_free);
    IF_FREE_LIST(internal->filters, efreet_menu_filter_free);

    IF_FREE_LIST(internal->sub_menus, efreet_menu_internal_free);

    IF_FREE_LIST(internal->layout, efreet_menu_layout_free);
    IF_FREE_LIST(internal->default_layout, efreet_menu_layout_free);

    FREE(internal);
}

/**
 * @internal
 * @param menu The menu to populate
 * @param xml The xml dom tree to populate from
 * @return Returns 1 if this XML tree is valid, 0 otherwise
 * @brief Populates the given menu from the given xml structure
 *
 * We walk the Menu children backwards. The reason for this is so that we
 * can deal with all the things that make us select the 'last' element
 * (MergeFile, Directory, etc). We'll see the last one first and can deal
 * with it right away.
 */
static int
efreet_menu_handle_menu(Efreet_Menu_Internal *internal, Efreet_Xml *xml)
{
    Efreet_Xml *child;
    Eina_List *l;
    int (*cb)(Efreet_Menu_Internal *parent, Efreet_Xml *xml);

    EINA_LIST_REVERSE_FOREACH(xml->children, l, child)
    {
        cb = eina_hash_find(efreet_menu_handle_cbs, child->tag);
        if (cb)
        {
            if (!cb(internal, child))
                return 0;
        }
        else
        {
            WRN("Unknown XML tag '%s' in file '%s/%s'", child->tag, internal->file.path, internal->file.name);
            return 0;
        }
    }
    return 1;
}

/**
 * @internal
 * @param parent The parent Menu
 * @param xml The xml that defines the menu
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the sub-menu nodes of the XML file
 */
static int
efreet_menu_handle_sub_menu(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    Efreet_Menu_Internal *internal, *match;

    efreet_menu_create_sub_menu_list(parent);

    internal = efreet_menu_internal_new(parent);
    if (!internal) return 0;
    internal->file.path = eina_stringshare_add(parent->file.path);
    if (!efreet_menu_handle_menu(internal, xml))
    {
        efreet_menu_internal_free(internal);
        return 0;
    }

    /* if this menu already exists we just take this one and stick it on the
     * start of the existing one */
    if ((match = eina_list_search_unsorted(parent->sub_menus,
                                           EINA_COMPARE_CB(efreet_menu_cb_menu_compare),
                                           internal)))
    {

        efreet_menu_concatenate(match, internal);
        efreet_menu_internal_free(internal);
    }
    else
        parent->sub_menus = eina_list_prepend(parent->sub_menus, internal);

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the AppDir tag
 */
static int
efreet_menu_handle_app_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    const char *path;
    Efreet_Menu_App_Dir *app_dir;

    if (!parent || !xml) return 0;

    efreet_menu_create_app_dirs_list(parent);
    path = efreet_menu_path_get(parent, xml->text);
    if (!path) return 0;

    /* we've already got this guy in our list we can skip it */
    if (eina_list_search_unsorted(parent->app_dirs,
                                  EINA_COMPARE_CB(efreet_menu_cb_app_dirs_compare),
                                  path))
    {
        eina_stringshare_del(path);
        return 1;
    }

    app_dir = efreet_menu_app_dir_new();
    app_dir->path = path;

    parent->app_dirs = eina_list_prepend(parent->app_dirs, app_dir);

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml UNUSED
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the DefaultAppDirs
 */
static int
efreet_menu_handle_default_app_dirs(Efreet_Menu_Internal *parent, Efreet_Xml *xml EINA_UNUSED)
{
    Eina_List *prepend = NULL;
    Eina_List *dirs;
    char *dir;

    if (!parent) return 0;

    efreet_menu_create_app_dirs_list(parent);
    dirs = efreet_default_dirs_get(efreet_data_home_get(), efreet_data_dirs_get(),
                                                                    "applications");
    EINA_LIST_FREE(dirs, dir)
    {
        if (!eina_list_search_unsorted(parent->app_dirs,
                                       EINA_COMPARE_CB(efreet_menu_cb_app_dirs_compare),
                                       dir))
        {
            Efreet_Menu_App_Dir *app_dir;

            app_dir = efreet_menu_app_dir_new();
            app_dir->path = eina_stringshare_ref(dir);

            prepend = eina_list_append(prepend, app_dir);
        }

        eina_stringshare_del(dir);
    }
    parent->app_dirs = eina_list_merge(prepend, parent->app_dirs);

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the DirectoryDir tag
 */
static int
efreet_menu_handle_directory_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    const char *path;

    if (!parent || !xml) return 0;

    efreet_menu_create_directory_dirs_list(parent);
    path = efreet_menu_path_get(parent, xml->text);
    if (!path) return 0;

    /* we've already got this guy in our list we can skip it */
    if (eina_list_search_unsorted(parent->directory_dirs, EINA_COMPARE_CB(strcmp), path))
    {
        eina_stringshare_del(path);
        return 1;
    }

    parent->directory_dirs = eina_list_prepend(parent->directory_dirs, path);

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml UNUSED
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the DefaultDirectoryDirs tag
 */
static int
efreet_menu_handle_default_directory_dirs(Efreet_Menu_Internal *parent, Efreet_Xml *xml EINA_UNUSED)
{
    Eina_List *dirs;
    char *dir;

    if (!parent) return 0;

    efreet_menu_create_directory_dirs_list(parent);
    dirs = efreet_default_dirs_get(efreet_data_home_get(), efreet_data_dirs_get(),
                                                            "desktop-directories");
    EINA_LIST_FREE(dirs, dir)
    {
        if (!eina_list_search_unsorted(parent->directory_dirs, EINA_COMPARE_CB(strcmp), dir))
            parent->directory_dirs = eina_list_prepend(parent->directory_dirs, eina_stringshare_ref(dir));
        eina_stringshare_del(dir);
    }

    return 1;
}

/**
 * @internal
 * @param parent The parent Menu
 * @param xml The xml to work with
 * @return Returns 1 on success or 0 on failure
 * @brief Sets the menu name from the given XML fragment.
 */
static int
efreet_menu_handle_name(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    /* not allowed to have two Name settings in a menu */
    if (parent->name.internal)
    {
        INF("efreet_menu_handle_name() setting second name into menu: '%s/%s'", parent->file.path, parent->file.name);
        return 0;
    }
    /* ignore the name if it is empty */
    if (!xml->text) return 1;

    /* ignore the name if it contains a / */
    if (strchr(xml->text, '/')) return 1;

    parent->name.internal = eina_stringshare_add(xml->text);

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Directory tag
 *
 * This just adds the given directory path to a list which we'll walk once
 * we've traversed the entire menu into memory.
 */
static int
efreet_menu_handle_directory(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    if (!parent || !xml) return 0;

    efreet_menu_create_directories_list(parent);
    parent->directories = eina_list_prepend(parent->directories, eina_stringshare_add(xml->text));

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the OnlyUnallocated tag
 */
static int
efreet_menu_handle_only_unallocated(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    if (!parent || !xml) return 0;

    /* a later instance has been seen so we can ignore this one */
    if (parent->seen_allocated) return 1;

    parent->seen_allocated = 1;
    parent->only_unallocated = 1;

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the NotOnlyUnallocated tag
 */
static int
efreet_menu_handle_not_only_unallocated(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    if (!parent || !xml) return 0;

    /* a later instance has been seen so we can ignore this one */
    if (parent->seen_allocated) return 1;

    parent->seen_allocated = 1;
    parent->only_unallocated = 0;

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Deleted tag
 */
static int
efreet_menu_handle_deleted(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    if (!parent || !xml) return 0;

    /* a later instance has been seen so we can ignore this one */
    if (parent->seen_deleted) return 1;

    parent->seen_deleted = 1;
    parent->deleted = 1;

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the NotDeleted tag
 */
static int
efreet_menu_handle_not_deleted(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    if (!parent || !xml) return 0;

    /* a later instance has been seen so we can ignore this one */
    if (parent->seen_deleted) return 1;

    parent->seen_deleted = 1;
    parent->deleted = 0;

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The XML tree to work with
 * @return Returns 1 on success or 0 on failure
 * @brief Handles parsing the Include tag and all subtags
 */
static int
efreet_menu_handle_include(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    return efreet_menu_handle_filter(parent, xml,
                                EFREET_MENU_FILTER_INCLUDE);
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Exclude tag and all subtags
 */
static int
efreet_menu_handle_exclude(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    return efreet_menu_handle_filter(parent, xml,
                                EFREET_MENU_FILTER_EXCLUDE);
}

/**
 * @internal
 * @param op The filter operation
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Filename tag
 */
static int
efreet_menu_handle_filename(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml)
{
    if (!op || !xml) return 0;

    op->filenames = eina_list_append(op->filenames, eina_stringshare_add(xml->text));

    return 1;
}

/**
 * @internal
 * @param op The filter operation
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Category tag
 */
static int
efreet_menu_handle_category(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml)
{
    if (!op || !xml) return 0;


    op->categories = eina_list_append(op->categories, eina_stringshare_add(xml->text));

    return 1;
}

/**
 * @internal
 * @param op The filter operation
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the All tag and all subtags
 */
static int
efreet_menu_handle_all(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml)
{
    if (!op || !xml) return 0;

    op->all = 1;

    return 1;
}

/**
 * @internal
 * @param op The filter operation
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the And tag and all subtags
 */
static int
efreet_menu_handle_and(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml)
{
    if (!op || !xml) return 0;

    return efreet_menu_handle_filter_child_op(op, xml,
                            EFREET_MENU_FILTER_OP_AND);
}

/**
 * @internal
 * @param op The filter operation
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Or tag and all subtags
 */
static int
efreet_menu_handle_or(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml)
{
    if (!op || !xml) return 0;

    return efreet_menu_handle_filter_child_op(op, xml,
                            EFREET_MENU_FILTER_OP_OR);
}

/**
 * @internal
 * @param op The filter operation
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Not tag and all subtags
 */
static int
efreet_menu_handle_not(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml)
{
    if (!op || !xml) return 0;

    return efreet_menu_handle_filter_child_op(op, xml,
                            EFREET_MENU_FILTER_OP_NOT);
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the MergeFile tag
 */
static int
efreet_menu_handle_merge_file(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    Eina_List *l;
    const char *path = NULL;
    const char *attr = NULL;
    int is_path = 1;
    int ret = 1;

    if (!parent || !xml) return 0;

    /* check to see if this is a path or parent type */
    attr = efreet_xml_attribute_get(xml, "type");
    if (attr && !strcmp(attr, "parent"))
        is_path = 0;

    /* we're given a path */
    if (is_path)
        path = efreet_menu_path_get(parent, xml->text);

    /* need to find the next menu with the same name as ours in the config
     * dir after ours (if we're in a config dir) */
    else
    {
        Eina_List *search_dirs;
        const char *dir, *p;

        if (!parent->file.path)
        {
            INF("efreet_menu_handle_merge_file() missing menu path ... '%s'", parent->file.name);
            return 0;
        }

        search_dirs = efreet_config_dirs_get();

        /* we need to find the next menu with the same name in the directory
         * after the on the the menu was found in. to do that we first check
         * if it's in the config_home_directory() if so we need to search
         * all of the dirs. If it isn't in the config home directory then we
         * scan the search dirs and look for it. The search_dirs list will
         * be left at the next pointer so we can start looking for the menu
         * from that point */

        dir = efreet_config_home_get();
        if (strncmp(dir, parent->file.path, eina_stringshare_strlen(dir)))
        {
            EINA_LIST_FOREACH(search_dirs, l, dir)
            {
                if (!strncmp(dir, parent->file.path, eina_stringshare_strlen(dir)))
                    break;
            }
        }

        if (!dir)
        {
            INF("efreet_menu_handle_merge_file() failed to find "
                    "menu parent directory");
            return 0;
        }

        /* the parent file path may have more path then just the base
         * directory so we need to append that as well */
        p = parent->file.path + eina_stringshare_strlen(dir);

        /* whatever dirs are left in the search dir we need to look for the
         * menu with the same relative filename */
        EINA_LIST_FOREACH(search_dirs, l, dir)
        {
            char file[PATH_MAX];

            snprintf(file, sizeof(file), "%s/%s/%s", dir, p,
                                                        parent->file.name);
            if (ecore_file_exists(file))
            {
                path = eina_stringshare_add(file);
                break;
            }
        }
    }

    /* nothing to do if no file found */
    if (!path) return 1;

    if (!efreet_menu_merge(parent, xml, path))
        ret = 0;

    eina_stringshare_del(path);

    return ret;
}

/**
 * @internal
 * @param parent The parent menu to merge into
 * @param xml The XML to be merged
 * @param path The path to the .menu file to merge
 */
static int
efreet_menu_merge(Efreet_Menu_Internal *parent, Efreet_Xml *xml, const char *path)
{
    Efreet_Xml *merge_xml;
    Efreet_Menu_Internal *internal;

    if (!parent || !xml || !path) return 0;

    /* do nothing if the file doesn't exist */
    if (!ecore_file_exists(path)) return 1;

    /* don't merge the same path twice */
    if (eina_hash_find(parent->efreet_merged_menus, path))
    {
        return 1;
    }

    eina_hash_add(parent->efreet_merged_menus, path, (void *)1);

    merge_xml = efreet_xml_new(path);

    if (!merge_xml)
    {
        INF("efreet_menu_merge() failed to read in the "
                "merge file '%s'", path);
        return 0;
    }

    internal = efreet_menu_internal_new(parent);
    if (!internal)
    {
        efreet_xml_del(merge_xml);
        return 0;
    }
    efreet_menu_path_set(internal, path);
    efreet_menu_handle_menu(internal, merge_xml);
    efreet_menu_concatenate(parent, internal);
    efreet_menu_internal_free(internal);

    efreet_xml_del(merge_xml);

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the MergeDir tag
 */
static int
efreet_menu_handle_merge_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    const char *path;
    int ret;

    if (!parent || !xml || !xml->text) return 0;

    path = efreet_menu_path_get(parent, xml->text);
    if (!path) return 1;
    if (!ecore_file_exists(path))
    {
        eina_stringshare_del(path);
        return 1;
    }

    ret = efreet_menu_merge_dir(parent, xml, path);
    eina_stringshare_del(path);

    return ret;
}

/**
 * @internal
 * @param parent the parent menu of the merge
 * @param xml The xml tree
 * @param path The path to the merge directory
 * @return Returns 1 on success or 0 on failure
 * @brief Find all of the .menu files in the given directory and merge them
 * into the @a parent menu.
 */
static int
efreet_menu_merge_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml, const char *path)
{
    Eina_Iterator *it;
    Eina_File_Direct_Info *info;

    if (!parent || !xml || !path) return 0;

    /* check to see if we've merged this directory already */
    if (eina_hash_find(parent->efreet_merged_dirs, path)) return 1;
    eina_hash_add(parent->efreet_merged_dirs, path, (void *)1);

    it = eina_file_direct_ls(path);
    if (!it) return 1;

    EINA_ITERATOR_FOREACH(it, info)
    {
        char *p;

        p = strrchr(info->path + info->name_start, '.');
        if (!p) continue;
        if (strcmp(p, ".menu")) continue;

        if (!efreet_menu_merge(parent, xml, info->path))
        {
            eina_iterator_free(it);
            return 0;
        }
    }
    eina_iterator_free(it);

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the DefaultMergeDirs tag
 */
static int
efreet_menu_handle_default_merge_dirs(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    Eina_List *dirs;
    char path[PATH_MAX], *p = NULL;
    const char *pp;
#ifndef STRICT_SPEC
    char parent_path[PATH_MAX + PATH_MAX + 128];
#endif

    if (!parent || !xml) return 0;

    if ((!strcmp(parent->file.name, "gnome-applications.menu")) ||
        (!strcmp(parent->file.name, "kde-applications.menu")))
    {
        p = alloca(sizeof("applications"));
        memcpy(p, "applications", sizeof("applications"));
    }
    else
    {
        size_t len;

        len = strlen(efreet_menu_prefix);
        if (!strncmp(parent->file.name, efreet_menu_prefix, len))
        {
            pp = parent->file.name;
            pp += len;
            if (!strcmp(pp, "applications.menu"))
            {
                p = alloca(sizeof("applications"));
                memcpy(p, "applications", sizeof("applications"));
            }
        }

        if (!p)
        {
            char *s;
            size_t len2;

            len2 = strlen(parent->file.name) + 1;
            p = alloca(len2);
            memcpy(p, parent->file.name, len2);
            s = strrchr(p, '.');
            if (s) *s = '\0';
        }
    }
    snprintf(path, sizeof(path), "menus/%s-merged", p);

    dirs = efreet_default_dirs_get(efreet_config_home_get(),
                                    efreet_config_dirs_get(), path);

    EINA_LIST_FREE(dirs, pp)
    {
        efreet_menu_merge_dir(parent, xml, pp);
        eina_stringshare_del(pp);
    }
#ifndef STRICT_SPEC
    /* Also check the path of the parent file */
    snprintf(parent_path, sizeof(parent_path), "%s/%s", parent->file.path, path);
    efreet_menu_merge_dir(parent, xml, parent_path);
#endif

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the LegacyDir tag
 */
static int
efreet_menu_handle_legacy_dir(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    Efreet_Menu_Internal *legacy;

    if (!parent || !xml) return 0;

    legacy = efreet_menu_handle_legacy_dir_helper(NULL, parent, xml->text,
                                efreet_xml_attribute_get(xml, "prefix"));
    if (legacy)
    {
        efreet_menu_concatenate(parent, legacy);
        efreet_menu_internal_free(legacy);
    }

    return 1;

}

/**
 * @internal
 * @param parent The parent menu
 * @param legacy_dir The legacy directory path
 * @param prefix The legacy directory prefix if one set
 * @return Returns the Efreet_Menu_Internal representing the legacy hierarchy
 * @brief Handles the process of merging @a legacy_dir into @a parent menu
 */
static Efreet_Menu_Internal *
efreet_menu_handle_legacy_dir_helper(Efreet_Menu_Internal *root,
                                        Efreet_Menu_Internal *parent,
                                        const char *legacy_dir,
                                        const char *prefix)
{
    const char *path;
    Efreet_Menu_Internal *legacy_internal;
    Efreet_Menu_Filter *filter;
    Efreet_Menu_App_Dir *app_dir;
    int count = 0;
    Eina_Iterator *it;

    if (!parent || !legacy_dir) return 0;

    path = efreet_menu_path_get(parent, legacy_dir);

    /* nothing to do if the legacy path doesn't exist */
    if (!path || !ecore_file_exists(path))
    {
        eina_stringshare_del(path);
        return NULL;
    }

    legacy_internal = efreet_menu_internal_new(parent);
    if (!legacy_internal)
        return NULL;
    legacy_internal->name.internal = eina_stringshare_add(ecore_file_file_get(path));

    /* add the legacy dir as an app dir */
    app_dir = efreet_menu_app_dir_new();
    app_dir->path = eina_stringshare_add(path);
    app_dir->legacy = 1;
    if (prefix && !strchr(prefix, '/')) app_dir->prefix = eina_stringshare_add(prefix);

    efreet_menu_create_app_dirs_list(legacy_internal);
    legacy_internal->app_dirs = eina_list_append(legacy_internal->app_dirs, app_dir);
#ifndef STRICT_SPEC
    if (root)
    {
        /* XXX This seems wrong, but it makes efreet pass the fdo tests */
        app_dir = efreet_menu_app_dir_new();
        app_dir->path = eina_stringshare_add(path);
        app_dir->legacy = 1;
        if (prefix && !strchr(prefix, '/')) app_dir->prefix = eina_stringshare_add(prefix);
        root->app_dirs = eina_list_append(root->app_dirs, app_dir);
    }
#endif

    /* add the legacy dir as a directory dir */
    efreet_menu_create_directory_dirs_list(legacy_internal);
    legacy_internal->directory_dirs = eina_list_append(legacy_internal->directory_dirs, eina_stringshare_add(path));

    /* setup a filter for all the conforming .desktop files in the legacy
     * dir */
    filter = efreet_menu_filter_new();
    if (!filter)
    {
        efreet_menu_internal_free(legacy_internal);
        return NULL;
    }
    filter->type = EFREET_MENU_FILTER_INCLUDE;

    filter->op->type = EFREET_MENU_FILTER_OP_OR;

    efreet_menu_create_filter_list(legacy_internal);
    legacy_internal->filters = eina_list_append(legacy_internal->filters, filter);

    it = eina_file_stat_ls(path);
    if (it)
    {
        Eina_File_Direct_Info *info;

        EINA_ITERATOR_FOREACH(it, info)
        {
            Efreet_Desktop *desktop = NULL;
            char buf[PATH_MAX];
            char *exten;
            const char *fname;

            fname = info->path + info->name_start;
            /* recurse into sub directories */
            if (info->type == EINA_FILE_DIR)
            {
                Efreet_Menu_Internal *ret;

                ret = efreet_menu_handle_legacy_dir_helper(root ? root : legacy_internal,
                        legacy_internal, info->path, prefix);
                if (!ret)
                {
                    efreet_menu_internal_free(legacy_internal);
                    eina_stringshare_del(path);
                    eina_iterator_free(it);
                    return NULL;
                }

                efreet_menu_create_sub_menu_list(legacy_internal);
                legacy_internal->sub_menus = eina_list_prepend(legacy_internal->sub_menus, ret);

                continue;
            }

            if (!strcmp(fname, ".directory"))
            {
                legacy_internal->directory = efreet_desktop_get(info->path);
                if (legacy_internal->directory
                        && legacy_internal->directory->type != EFREET_DESKTOP_TYPE_DIRECTORY)
                {
                    efreet_desktop_free(legacy_internal->directory);
                    legacy_internal->directory = NULL;
                }
                continue;
            }

            exten = strrchr(fname, '.');

            if (exten && !strcmp(exten, ".desktop"))
                desktop = efreet_desktop_get(info->path);

            if (!desktop) continue;

            /* if the .desktop has categories it isn't legacy */
            if (efreet_desktop_category_count_get(desktop) != 0)
            {
                efreet_desktop_free(desktop);
                continue;
            }

            /* XXX: This will disappear when the .desktop is free'd */
            efreet_desktop_category_add(desktop, "Legacy");

            if (prefix)
            {
                snprintf(buf, sizeof(buf), "%s%s", prefix, fname);
                filter->op->filenames = eina_list_append(filter->op->filenames, eina_stringshare_add(buf));
            }
            else
                filter->op->filenames = eina_list_append(filter->op->filenames, eina_stringshare_add(fname));

            count++;
            efreet_desktop_free(desktop);
        }
        eina_iterator_free(it);
    }

    eina_stringshare_del(path);
    return legacy_internal;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml UNUSED
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the KDELegacyDirs tag
 */
static int
efreet_menu_handle_kde_legacy_dirs(Efreet_Menu_Internal *parent, Efreet_Xml *xml EINA_UNUSED)
{
    Eina_List *l;
    const char *dir;

    if (!parent) return 0;

    if (!efreet_menu_kde_legacy_dirs) return 1;

    /* XXX if one _helper() call succeeds, we return success. should this be flipped?
     * (return fail if on of them failed) */
    EINA_LIST_FOREACH(efreet_menu_kde_legacy_dirs, l, dir)
    {
        Efreet_Menu_Internal *kde;

        kde = efreet_menu_handle_legacy_dir_helper(NULL, parent, dir, "kde");
        if (kde)
        {
            efreet_menu_concatenate(parent, kde);
            efreet_menu_internal_free(kde);
            return 1;
        }
    }

    return 0;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Move tag and all subtags
 */
static int
efreet_menu_handle_move(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    Efreet_Xml *child;
    Eina_List *l;

    if (!parent || !xml) return 0;

    efreet_menu_create_move_list(parent);

    EINA_LIST_FOREACH(xml->children, l, child)
    {
        int (*cb)(Efreet_Menu_Internal *parent, Efreet_Xml *xml);

        cb = eina_hash_find(efreet_menu_move_cbs, child->tag);
        if (cb)
        {
            if (!cb(parent, child))
                return 0;
        }
        else
        {
            INF("efreet_menu_handle_move() unknown tag found "
                    "in Move '%s' in file '%s/%s'", child->tag,
                    parent->file.path, parent->file.name);
            return 0;
        }
    }

    parent->current_move = NULL;

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Old tag
 */
static int
efreet_menu_handle_old(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    Efreet_Menu_Move *move;

    if (!parent || !xml || !xml->text) return 0;

    if (parent->current_move)
    {
        INF("efreet_menu_handle_old() saw second <Old> "
                "before seeing <New> in file '%s/%s'",
                parent->file.path, parent->file.name);
        return 0;
    }

    /* If we already moved this menu, remove the old move */
    /* XXX This seems wrong, but it makes efreet pass the fdo tests */
#ifndef STRICT_SPEC
    move = eina_list_search_unsorted(parent->moves,
                                     EINA_COMPARE_CB(efreet_menu_cb_move_compare),
                                     xml->text);
    if (move)
    {
        parent->moves = eina_list_remove(parent->moves, move);
        efreet_menu_move_free(move);
    }
#endif

    move = efreet_menu_move_new();
    move->old_name = eina_stringshare_add(xml->text);

    parent->current_move = move;
    parent->moves = eina_list_append(parent->moves, move);

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the New tag
 */
static int
efreet_menu_handle_new(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    if (!parent || !xml || !xml->text) return 0;

    if (!parent->current_move)
    {
        INF("efreet_menu_handle_new() saw New before seeing Old in '%s/%s'",
            parent->file.path, parent->file.name);
        return 0;
    }

    parent->current_move->new_name = eina_stringshare_add(xml->text);
    parent->current_move = NULL;

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the Layout tag and all subtags
 */
static int
efreet_menu_handle_layout(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    Efreet_Xml *child;
    Eina_List *l;

    if (!parent || !xml) return 0;

    /* We use the last existing layout */
    if (parent->layout) return 1;

    efreet_menu_create_layout_list(parent);

    EINA_LIST_FOREACH(xml->children, l, child)
    {
        int (*cb)(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def);

        cb = eina_hash_find(efreet_menu_layout_cbs, child->tag);
        if (cb)
        {
            if (!cb(parent, child, 0))
                return 0;
        }
        else
        {
            INF("efreet_menu_handle_move() unknown tag found "
                    "in Layout '%s' in file '%s/%s'", child->tag,
                    parent->file.path, parent->file.name);
            return 0;
        }
    }

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The xml tree
 * @return Returns 1 on success or 0 on failure
 * @brief Handles the DefaultLayout tag
 */
static int
efreet_menu_handle_default_layout(Efreet_Menu_Internal *parent, Efreet_Xml *xml)
{
    const char *val;
    Efreet_Xml *child;
    Eina_List *l;

    if (!parent || !xml) return 0;

    /* We use the last existing layout */
    if (parent->default_layout) return 1;

    val = efreet_xml_attribute_get(xml, "show_empty");
    if (val) parent->show_empty = !strcmp(val, "true");

    val = efreet_xml_attribute_get(xml, "inline");
    if (val) parent->in_line = !strcmp(val, "true");

    val = efreet_xml_attribute_get(xml, "inline_limit");
    if (val) parent->inline_limit = atoi(val);

    val = efreet_xml_attribute_get(xml, "inline_header");
    if (val) parent->inline_header = !strcmp(val, "true");

    val = efreet_xml_attribute_get(xml, "inline_alias");
    if (val) parent->inline_alias = !strcmp(val, "true");

    efreet_menu_create_default_layout_list(parent);

    EINA_LIST_FOREACH(xml->children, l, child)
    {
        int (*cb)(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def);

        cb = eina_hash_find(efreet_menu_layout_cbs, child->tag);
        if (cb)
        {
            if (!cb(parent, child, 1))
                return 0;
        }
        else
        {
            INF("efreet_menu_handle_move() unknown tag found in "
                    "DefaultLayout '%s' in file '%s/%s'", child->tag,
                    parent->file.path, parent->file.name);
            return 0;
        }
    }

    return 1;
}

static int
efreet_menu_handle_layout_menuname(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def)
{
    Efreet_Menu_Layout *layout;
    const char *val;

    if (!parent || !xml) return 0;

    if (!xml->text)
    {
        INF("efreet_menu_handle_layout_menuname() The Menuname tag in "
                "layout needs a filename in file '%s/%s'",
                parent->file.path, parent->file.name);
        return 0;
    }

    layout = efreet_menu_layout_new();
    layout->type = EFREET_MENU_LAYOUT_MENUNAME;
    layout->name = eina_stringshare_add(xml->text);

    val = efreet_xml_attribute_get(xml, "show_empty");
    if (val) layout->show_empty = !strcmp(val, "true");

    val = efreet_xml_attribute_get(xml, "inline");
    if (val) layout->in_line = !strcmp(val, "true");

    val = efreet_xml_attribute_get(xml, "inline_limit");
    if (val) layout->inline_limit = atoi(val);

    val = efreet_xml_attribute_get(xml, "inline_header");
    if (val) layout->inline_header = !strcmp(val, "true");

    val = efreet_xml_attribute_get(xml, "inline_alias");
    if (val) layout->inline_alias = !strcmp(val, "true");

    if (def) parent->default_layout = eina_list_append(parent->default_layout, layout);
    else parent->layout = eina_list_append(parent->layout, layout);

    return 1;
}

static int
efreet_menu_handle_layout_filename(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def)
{
    Efreet_Menu_Layout *layout;

    if (!parent || !xml) return 0;

    if (!xml->text)
    {
        INF("efreet_menu_handle_layout_filename() The Filename tag in "
                "layout needs a filename in file '%s/%s'",
                parent->file.path, parent->file.name);
        return 0;
    }

    layout = efreet_menu_layout_new();
    layout->type = EFREET_MENU_LAYOUT_FILENAME;
    layout->name = eina_stringshare_add(xml->text);

    if (def) parent->default_layout = eina_list_append(parent->default_layout, layout);
    else parent->layout = eina_list_append(parent->layout, layout);

    return 1;
}

static int
efreet_menu_handle_layout_separator(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def)
{
    Efreet_Menu_Layout *layout;

    if (!parent || !xml) return 0;

    layout = efreet_menu_layout_new();
    layout->type = EFREET_MENU_LAYOUT_SEPARATOR;
    if (def)
        parent->default_layout = eina_list_append(parent->default_layout, layout);
    else
        parent->layout = eina_list_append(parent->layout, layout);
    return 1;
}

static int
efreet_menu_handle_layout_merge(Efreet_Menu_Internal *parent, Efreet_Xml *xml, int def)
{
    Efreet_Menu_Layout *layout;
    const char *attr;

    if (!parent || !xml) return 0;

    attr = efreet_xml_attribute_get(xml, "type");
    if (!attr)
    {
        INF("efreet_menu_handle_layout_merge() The Merge tag in layout "
                "needs a type attribute in file '%s/%s'",
                parent->file.path, parent->file.name);
        return 0;
    }

    if (strcmp(attr, "files") && strcmp(attr, "menus") && strcmp(attr, "all"))
    {
        INF("efreet_menu_handle_layout_merge() The type attribute for "
                "the Merge tag contains an unknown value '%s' in file '%s/%s'",
                attr, parent->file.path, parent->file.name);
        return 0;
    }

    layout = efreet_menu_layout_new();
    layout->type = EFREET_MENU_LAYOUT_MERGE;
    layout->name = eina_stringshare_add(attr);

    if (def) parent->default_layout = eina_list_append(parent->default_layout, layout);
    else parent->layout = eina_list_append(parent->layout, layout);

    return 1;
}

/**
 * @internal
 * @param parent The parent menu
 * @param xml The XML tree to parse
 * @param type The type of filter
 * @return Returns 1 on success or 0 on failure
 * @brief Parses the given XML tree and adds the filter to the parent menu
 */
static int
efreet_menu_handle_filter(Efreet_Menu_Internal *parent, Efreet_Xml *xml,
                                        Efreet_Menu_Filter_Type type)
{
    Efreet_Menu_Filter *filter;

    efreet_menu_create_filter_list(parent);

    /* filters have a default or relationship */
    filter = efreet_menu_filter_new();
    if (!filter) return 0;
    filter->type = type;
    filter->op->type = EFREET_MENU_FILTER_OP_OR;

    if (!efreet_menu_handle_filter_op(filter->op, xml))
    {
        efreet_menu_filter_free(filter);
        return 0;
    }

    parent->filters = eina_list_prepend(parent->filters, filter);

    return 1;
}

/**
 * @internal
 * @param op The operation to work with
 * @param xml The XML tree representing this operation
 * @return Returns 1 on success or 0 on failure
 * @brief Parses the given XML tree and populates the operation
 */
static int
efreet_menu_handle_filter_op(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml)
{
    Efreet_Xml *child;
    Eina_List *l;

    EINA_LIST_FOREACH(xml->children, l, child)
    {
        int (*cb)(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml);

        cb = eina_hash_find(efreet_menu_filter_cbs, child->tag);
        if (cb)
        {
            if (!cb(op, child))
                return 0;
        }
        else
        {
            INF("efreet_menu_handle_filter_op() unknown tag in filter '%s'",
                child->tag);
            return 0;
        }
    }
    return 1;
}

/**
 * @internal
 * @return Returns a new Efreet_Menu_Filter on success or NULL on failure
 * @brief Creates and initializes an Efreet_Menu_Filter object
 */
static Efreet_Menu_Filter *
efreet_menu_filter_new(void)
{
    Efreet_Menu_Filter *filter;

    filter = NEW(Efreet_Menu_Filter, 1);
    if (!filter) return NULL;
    filter->op = efreet_menu_filter_op_new();
    if (!filter->op)
    {
        FREE(filter);
        return NULL;
    }

    return filter;
}

/**
 * @internal
 * @param filter The filter to work with
 * @return Returns no data
 * @brief Frees the given filter and all data
 */
static void
efreet_menu_filter_free(Efreet_Menu_Filter *filter)
{
    if (!filter) return;

    if (filter->op) efreet_menu_filter_op_free(filter->op);
    filter->op = NULL;

    FREE(filter);
}

/**
 * @internal
 * @return Returns a new Efreet_Menu_Layout on success or NULL on failure
 * @brief Creates and initializes an Efreet_Menu_Layout object
 */
static Efreet_Menu_Layout *
efreet_menu_layout_new(void)
{
    Efreet_Menu_Layout *layout;

    layout = NEW(Efreet_Menu_Layout, 1);
    layout->show_empty = -1;
    layout->in_line = -1;
    layout->inline_limit = -1;
    layout->inline_header = -1;
    layout->inline_alias = -1;

    return layout;
}

/**
 * @internal
 * @param filter The filter to work with
 * @return Returns no data
 * @brief Frees the given filter and all data
 */
static void
efreet_menu_layout_free(Efreet_Menu_Layout *layout)
{
    if (!layout) return;

    IF_RELEASE(layout->name);
    FREE(layout);
}

/**
 * @internal
 * @return Returns a new Efreet_Menu_Filter_Op on success or NULL on failure
 * @brief Creates and initializes an Efreet_Menu_Filter_Op structure
 */
static Efreet_Menu_Filter_Op *
efreet_menu_filter_op_new(void)
{
    Efreet_Menu_Filter_Op *op;

    op = NEW(Efreet_Menu_Filter_Op, 1);

    return op;
}

/**
 * @internal
 * @param op The operation to work with
 * @return Returns no value.
 * @brief Frees the given operation and all sub data
 */
static void
efreet_menu_filter_op_free(Efreet_Menu_Filter_Op *op)
{
    if (!op) return;

    IF_FREE_LIST(op->categories, eina_stringshare_del);
    IF_FREE_LIST(op->filenames, eina_stringshare_del);
    IF_FREE_LIST(op->filters, efreet_menu_filter_op_free);

    FREE(op);
}

/**
 * @internal
 * @return Returns a new Efreet_Menu_Desktop on success or NULL on failure
 * @brief Creates and returns an Efreet_Menu_Desktop
 */
static Efreet_Menu_Desktop *
efreet_menu_desktop_new(void)
{
    Efreet_Menu_Desktop *md;

    md = NEW(Efreet_Menu_Desktop, 1);

    return md;
}

/**
 * @internal
 * @param md The Efreet_Menu_Desktop to free
 * @return Returns no value
 * @brief Frees the given structure
 */
static void
efreet_menu_desktop_free(Efreet_Menu_Desktop *md)
{
    IF_RELEASE(md->id);
    if (md->desktop) efreet_desktop_free(md->desktop);
    FREE(md);
}

/**
 * @internal
 * @return Returns a new Efreet_Menu on success or NULL on failure
 * @brief Creates and returns an Efreet_Menu
 */
static Efreet_Menu *
efreet_menu_entry_new(void)
{
    Efreet_Menu *entry;

    entry = NEW(Efreet_Menu, 1);
    entry->references = 1;
    return entry;
}

EFREET_API void
efreet_menu_free(Efreet_Menu *entry)
{
    Efreet_Menu *sub;

    if (!entry) return;

    entry->references--;
    if (entry->references > 0) return;
    IF_RELEASE(entry->name);
    IF_RELEASE(entry->icon);
    EINA_LIST_FREE(entry->entries, sub)
        efreet_menu_free(sub);
    IF_RELEASE(entry->id);
    if (entry->desktop) efreet_desktop_free(entry->desktop);
    FREE(entry);
}

EFREET_API void
efreet_menu_ref(Efreet_Menu *entry)
{
    Efreet_Menu *sub;
    Eina_List *l;

    if (!entry) return;

    EINA_LIST_FOREACH(entry->entries, l, sub)
        efreet_menu_ref(sub);
    entry->references++;
}

EFREET_API void
efreet_menu_unref(Efreet_Menu *entry)
{
    efreet_menu_free(entry);
}

/**
 * @internal
 * @param op The op to add a child too
 * @param xml The XML tree of the child
 * @param type The type of child to add
 * @return Returns 1 on success or 0 on failure
 * @brief Parses the given XML tree and populates a new child operation.
 */
static int
efreet_menu_handle_filter_child_op(Efreet_Menu_Filter_Op *op, Efreet_Xml *xml,
                                                Efreet_Menu_Filter_Op_Type type)
{
    Efreet_Menu_Filter_Op *child_op;

    child_op = efreet_menu_filter_op_new();
    child_op->type = type;

    if (!efreet_menu_handle_filter_op(child_op, xml))
    {
        efreet_menu_filter_op_free(child_op);
        return 0;
    }

    op->filters = eina_list_append(op->filters, child_op);

    return 1;
}

/**
 * @internal
 * @param menu The menu to work with
 * @param only_unallocated Do we only look for unallocated items?
 * @return Returns 1 if we've successfully processed the menu, 0 otherwise
 * @brief Handles the processing of the menu data to retrieve the .desktop
 * files for the menu
 */
static int
efreet_menu_process(Efreet_Menu_Internal *internal, unsigned int only_unallocated)
{
    Eina_List *l;

    /* a menu _MUST_ have a name */
    if (!internal->name.internal || (internal->name.internal[0] == '\0'))
        return 0;

    /* handle filtering out .desktop files as needed. This deals with all
     * .desktop files */
    efreet_menu_process_filters(internal, only_unallocated);

    if (internal->sub_menus)
    {
        Efreet_Menu_Internal *sub_internal;

        EINA_LIST_FOREACH(internal->sub_menus, l, sub_internal)
        {
            sub_internal->parent = internal;
            efreet_menu_process(sub_internal, only_unallocated);
        }
    }

    return 1;
}

/* This will walk through all of the app dirs and load all the .desktop
 * files into the cache for the menu. The .desktop files will have their
 * allocated flag set to 0 */
static int
efreet_menu_process_dirs(Efreet_Menu_Internal *internal)
{
    Eina_List *l;

    /* Scan application directories for .desktop files */
    if (!efreet_menu_app_dirs_process(internal))
        return 0;

    /* Scan directory directories for .directory file */
    if (!efreet_menu_directory_dirs_process(internal))
        return 0;

    if (internal->sub_menus)
    {
        Efreet_Menu_Internal *sub_internal;

        EINA_LIST_FOREACH(internal->sub_menus, l, sub_internal)
        {
            sub_internal->parent = internal;
            efreet_menu_process_dirs(sub_internal);
        }
    }

    return 1;
}

/**
 * @internal
 * @param menu the menu to process
 * @param only_unallocated Only handle menus that deal with unallocated items
 * @return Returns no value
 * @brief Handles the processing of the filters attached to the given menu.
 *
 * For each include filter we'll add the items to our applications array. Each
 * exclude filter will remove items from the applications array
 */
static void
efreet_menu_process_filters(Efreet_Menu_Internal *internal, unsigned int only_unallocated)
{
    Efreet_Menu_Filter *filter;
    Efreet_Menu_Desktop *md;
    Eina_List *l, *ll;

    int included = 0;

    /* nothing to do if we're checking the other option */
    if (only_unallocated != internal->only_unallocated) return;

    internal->applications = eina_list_free(internal->applications);

    if (!internal->filters) return;

    EINA_LIST_FOREACH(internal->filters, l, filter)
    {
        /* skip excludes until we get an include */
        if (!included && (filter->type == EFREET_MENU_FILTER_EXCLUDE))
            continue;
        included = 1;

        if (filter->type == EFREET_MENU_FILTER_INCLUDE)
        {
            Eina_Hash *matches;

            matches = eina_hash_string_superfast_new(NULL);
            internal->applications = efreet_menu_process_app_pool(internal->app_pool, internal->applications,
                                        matches, filter, internal->only_unallocated);
            if (internal->parent)
            {
                Efreet_Menu_Internal *parent;

                parent = internal->parent;
                do {
                    internal->applications = efreet_menu_process_app_pool(parent->app_pool,
                                                internal->applications, matches, filter,
                                                internal->only_unallocated);
                } while ((parent = parent->parent));
            }
            eina_hash_free(matches);
        }
        else
        {
            /* check each item in our menu so far and see if it's excluded */
            l = internal->applications;
            while ((md = eina_list_data_get(l)))
            {
                ll = eina_list_next(l);
                if (efreet_menu_filter_matches(filter->op, md))
                    internal->applications = eina_list_remove_list(internal->applications, l);
                l = ll;
            }
        }
    }

    /* sort the menu applications. we do this in process filters so it will only
     * be done once per menu.*/
    if (eina_list_count(internal->applications))
    {
        Eina_List           *l2;

        EINA_LIST_FOREACH_SAFE(internal->applications, l, l2, md)
        {
            if (md->desktop->no_display)
                internal->applications = eina_list_remove_list(internal->applications, l);
        }
        internal->applications = eina_list_sort(internal->applications,
                                                eina_list_count(internal->applications),
                                                EINA_COMPARE_CB(efreet_menu_cb_md_compare));
    }
}

/**
 * @internal
 * @param pool The app pool to iterate
 * @param applications The list of applications to append too
 * @param matches The hash of previously matched ids
 * @param filter The menu filter to run on the pool items
 * @param only_unallocated Do we check only unallocated pool items?
 * @return Returns no value.
 * @brief This will iterate the items in @a pool and append them to @a
 * applications if they match the @a filter given and aren't previoulsy entered
 * in @a matches. If @a only_unallocated is set we'll only only at the
 * .desktop files that haven't been previoulsy matched
 */
static Eina_List *
efreet_menu_process_app_pool(Eina_List *pool, Eina_List *applications,
                                Eina_Hash *matches, Efreet_Menu_Filter *filter,
                                unsigned int only_unallocated)
{
    Efreet_Menu_Desktop *md;
    Eina_List *l;

    EINA_LIST_FOREACH(pool, l, md)
    {
        if (eina_hash_find(matches, md->id)) continue;
        if (only_unallocated && md->allocated) continue;
        if (efreet_menu_filter_matches(filter->op, md))
        {
            applications = eina_list_append(applications, md);
            eina_hash_direct_add(matches, (void *)md->id, md);
            md->allocated = 1;
        }
    }
    return applications;
}

/**
 * @internal
 * @param op The filter operation to execute
 * @param md The desktop to run the filter on
 * @return Returns 1 if this desktop matches the given filter, 0 otherwise
 * @brief This will execute the given @a filter on the given desktop
 */
static int
efreet_menu_filter_matches(Efreet_Menu_Filter_Op *op, Efreet_Menu_Desktop *md)
{
    if (op->type == EFREET_MENU_FILTER_OP_OR)
        return efreet_menu_filter_or_matches(op, md);

    if (op->type == EFREET_MENU_FILTER_OP_AND)
        return efreet_menu_filter_and_matches(op, md);

    if (op->type == EFREET_MENU_FILTER_OP_NOT)
        return efreet_menu_filter_not_matches(op, md);

    return 0;
}

/**
 * @internal
 * @param op The filter operation to execute
 * @param md The desktop to execute on
 * @return Returns 1 if the desktop matches, 0 otherwise
 * @brief Executes the OR operation, @a op, on the desktop, @a md.
 */
static int
efreet_menu_filter_or_matches(Efreet_Menu_Filter_Op *op, Efreet_Menu_Desktop *md)
{
    Efreet_Menu_Filter_Op *child;
    Eina_List *l;
    char *t;

    if (op->all) return 1;

    if (op->categories && md->desktop->categories)
    {
        EINA_LIST_FOREACH(op->categories, l, t)
        {
            if (eina_list_search_unsorted(md->desktop->categories,
                                          EINA_COMPARE_CB(strcmp), t))
                return 1;
        }
    }

    if (op->filenames)
    {
        EINA_LIST_FOREACH(op->filenames, l, t)
            if (t == md->id) return 1;
    }

    if (op->filters)
    {
        EINA_LIST_FOREACH(op->filters, l, child)
        {
            if (efreet_menu_filter_matches(child, md))
                return 1;
        }
    }

    return 0;
}

/**
 * @internal
 * @param op The filter operation to execute
 * @param md The desktop to execute on
 * @return Returns 1 if the desktop matches, 0 otherwise
 * @brief Executes the AND operation, @a op, on the desktop, @a md.
 */
static int
efreet_menu_filter_and_matches(Efreet_Menu_Filter_Op *op, Efreet_Menu_Desktop *md)
{
    Efreet_Menu_Filter_Op *child;
    Eina_List *l;
    char *t;

    if (op->categories)
    {
        if ((eina_list_count(op->categories) > 0) && !md->desktop->categories)
            return 0;

        EINA_LIST_FOREACH(op->categories, l, t)
        {
            if (!eina_list_search_unsorted(md->desktop->categories,
                                           EINA_COMPARE_CB(strcmp), t))
                return 0;
        }
    }

    if (op->filenames)
    {
        EINA_LIST_FOREACH(op->filenames, l, t)
        {
            if (t != md->id) return 0;
        }
    }

    if (op->filters)
    {
        EINA_LIST_FOREACH(op->filters, l, child)
        {
            if (!efreet_menu_filter_matches(child, md))
                return 0;
        }
    }

    return 1;
}

/**
 * @internal
 * @param op The filter operation to execute
 * @param md The desktop to execute on
 * @return Returns 1 if the desktop matches, 0 otherwise
 * @brief Executes the NOT operation, @a op, on the desktop, @a md.
 */
static int
efreet_menu_filter_not_matches(Efreet_Menu_Filter_Op *op, Efreet_Menu_Desktop *md)
{
    Efreet_Menu_Filter_Op *child;
    Eina_List *l;
    char *t;

    /* !all means no desktops match */
    if (op->all) return 0;

    if (op->categories)
    {
        if ((eina_list_count(op->categories) > 0) && !md->desktop->categories)
            return 1;

        EINA_LIST_FOREACH(op->categories, l, t)
        {
            if (eina_list_search_unsorted(md->desktop->categories,
                                          EINA_COMPARE_CB(strcmp), t))
                return 0;
        }
    }

    if (op->filenames)
    {
        EINA_LIST_FOREACH(op->filenames, l, t)
        {
            if (t == md->id) return 0;
        }
    }

    if (op->filters)
    {
        EINA_LIST_FOREACH(op->filters, l, child)
        {
            if (efreet_menu_filter_matches(child, md))
                return 0;
        }
    }

    return 1;
}

/**
 * @internal
 * @param dest The destination menu
 * @param src The source menu
 * @return Returns no value
 * @brief Takes the child elements of the menu @a src and puts then on the
 * _start_ of the menu @a dest.
 */
static void
efreet_menu_concatenate(Efreet_Menu_Internal *dest, Efreet_Menu_Internal *src)
{
    Efreet_Menu_Internal *submenu;

    if (!dest || !src) return;

    if (!dest->directory && src->directory)
    {
        dest->directory = src->directory;
        src->directory = NULL;
    }

    if (!dest->seen_allocated && src->seen_allocated)
    {
        dest->only_unallocated = src->only_unallocated;
        dest->seen_allocated = 1;
    }

    if (!dest->seen_deleted && src->seen_deleted)
    {
        dest->deleted = src->deleted;
        dest->seen_deleted = 1;
    }

    if (src->directories)
    {
        efreet_menu_create_directories_list(dest);
        dest->directories = eina_list_merge(src->directories, dest->directories);
        src->directories = NULL;
    }

    if (src->app_dirs)
    {
        efreet_menu_create_app_dirs_list(dest);
        dest->app_dirs = eina_list_merge(src->app_dirs, dest->app_dirs);
        src->app_dirs = NULL;
    }

    if (src->directory_dirs)
    {
        efreet_menu_create_directory_dirs_list(dest);
        dest->directory_dirs = eina_list_merge(src->directory_dirs, dest->directory_dirs);
        src->directory_dirs = NULL;
    }

    if (src->moves)
    {
        efreet_menu_create_move_list(dest);
        dest->moves = eina_list_merge(src->moves, dest->moves);
        src->moves = NULL;
    }

    if (src->filters)
    {
        efreet_menu_create_filter_list(dest);
        dest->filters = eina_list_merge(src->filters, dest->filters);
        src->filters = NULL;
    }

    if (src->sub_menus)
    {
        efreet_menu_create_sub_menu_list(dest);

        while ((submenu = eina_list_data_get(eina_list_last(src->sub_menus))))
        {
            Efreet_Menu_Internal *match;

            src->sub_menus = eina_list_remove(src->sub_menus, submenu);
            /* if this menu is in the list already we just add to that */
            if ((match = eina_list_search_unsorted(dest->sub_menus,
                                                   EINA_COMPARE_CB(efreet_menu_cb_menu_compare),
                                                   submenu)))
            {
                efreet_menu_concatenate(match, submenu);
                efreet_menu_internal_free(submenu);
            }
            else
                dest->sub_menus = eina_list_prepend(dest->sub_menus, submenu);
        }
    }
}

/**
 * @internal
 * @param menu The menu to work with
 * @return Returns no value
 * @brief Handles any \<Move\> commands in the menus
 */
static void
efreet_menu_resolve_moves(Efreet_Menu_Internal *internal)
{
    Efreet_Menu_Internal *child;
    Efreet_Menu_Move *move;
    Eina_List *l;

    /* child moves are handled before parent moves */
    if (internal->sub_menus)
    {
        EINA_LIST_FOREACH(internal->sub_menus, l, child)
            efreet_menu_resolve_moves(child);
    }

    /* nothing to do if this menu has no moves */
    if (!internal->moves) return;

    EINA_LIST_FOREACH(internal->moves, l, move)
    {
        Efreet_Menu_Internal *origin, *dest, *parent;

        /* if the origin path doesn't exist we do nothing */
        origin = efreet_menu_by_name_find(internal, move->old_name, &parent);
        if (!origin) continue;

        /* remove the origin menu from the parent */
        parent->sub_menus = eina_list_remove(parent->sub_menus, origin);

        /* if the destination path doesn't exist we just rename the origin
         * menu and append to the parents list of children */
        dest = efreet_menu_by_name_find(internal, move->new_name, &parent);
        if (!dest)
        {
            char *path, *tmp, *t;
            size_t len;

            /* if the dest path has /'s in it then we need to add menus to
             * fill out the paths */
            len = strlen(move->new_name) + 1;
            t = alloca(len);
            memcpy(t, move->new_name, len);
            tmp = t;
            path = strchr(tmp, '/');
            while (path)
            {
                Efreet_Menu_Internal *ancestor;

                *path = '\0';

                ancestor = efreet_menu_internal_new(parent);
                if (!ancestor) goto error;
                ancestor->name.internal = eina_stringshare_add(tmp);

                efreet_menu_create_sub_menu_list(parent);
                parent->sub_menus = eina_list_append(parent->sub_menus, ancestor);

                parent = ancestor;
                tmp = ++path;
                path = strchr(tmp, '/');
            }

            IF_RELEASE(origin->name.internal);
            origin->name.internal = eina_stringshare_add(tmp);

            efreet_menu_create_sub_menu_list(parent);
            parent->sub_menus = eina_list_append(parent->sub_menus, origin);
        }
        else
        {
            efreet_menu_concatenate(dest, origin);
            efreet_menu_internal_free(origin);
        }
    }
error:
    IF_FREE_LIST(internal->moves, efreet_menu_move_free);
}

/**
 * @internal
 * @param menu The menu to start searching from
 * @param name The menu name to find
 * @param parent The parent of the found menu
 * @return Returns the menu with the given @a name or NULL if none found
 * @brief Searches the menu tree starting at @a menu looking for a menu with
 * @a name.
 */
static Efreet_Menu_Internal *
efreet_menu_by_name_find(Efreet_Menu_Internal *internal, const char *name, Efreet_Menu_Internal **parent)
{
    char *part, *tmp, *ptr;
    size_t len;

    if (parent) *parent = internal;

    /* find the correct parent menu */
    len = strlen(name) + 1;
    tmp = alloca(len);
    memcpy(tmp, name, len);
    ptr = tmp;
    part = strchr(ptr, '/');
    while (part)
    {
        *part = '\0';

        if (!(internal = eina_list_search_unsorted(internal->sub_menus,
                                                   EINA_COMPARE_CB(efreet_menu_cb_compare_names),
                                                   ptr)))
        {
            return NULL;
        }

        ptr = ++part;
        part = strchr(ptr, '/');
    }

    if (parent) *parent = internal;

    /* find the menu in the parent list */
    if (!(internal = eina_list_search_unsorted(internal->sub_menus,
                                               EINA_COMPARE_CB(efreet_menu_cb_compare_names),
                                               ptr)))
    {
        return NULL;
    }

    return internal;
}

/**
 * @internal
 * @return Returns a new Efreet_Menu_Move struct on success or NULL on failure
 * @brief Creates an returns a new Efreet_Menu_Move struct or NULL on failure
 */
static Efreet_Menu_Move *
efreet_menu_move_new(void)
{
    Efreet_Menu_Move *move;

    move = NEW(Efreet_Menu_Move, 1);

    return move;
}

/**
 * @internal
 * @param move The Efreet_Menu_Move to free
 * @return Returns no value.
 * @brief Frees the given move structure
 */
static void
efreet_menu_move_free(Efreet_Menu_Move *move)
{
    if (!move) return;

    IF_RELEASE(move->old_name);
    IF_RELEASE(move->new_name);

    FREE(move);
}

/**
 * @internal
 * @return Returns a new Efreet_Menu_App_Dir on success or NULL on failure
 * @brief Creates and initializes a new Efreet_Menu_App_Dir structure
 */
static Efreet_Menu_App_Dir *
efreet_menu_app_dir_new(void)
{
    Efreet_Menu_App_Dir *dir;

    dir = NEW(Efreet_Menu_App_Dir, 1);

    return dir;
}

/**
 * @internal
 * @param dir The Efreet_Menu_App_Dir to free
 * @return Returns no value.
 * @brief Frees the given dir structure
 */
static void
efreet_menu_app_dir_free(Efreet_Menu_App_Dir *dir)
{
    if (!dir) return;

    IF_RELEASE(dir->path);
    IF_RELEASE(dir->prefix);
    FREE(dir);
}

/**
 * @internal
 * @param a The app dir to compare too
 * @param b The path to compare too
 * @return Returns 0 if the strings are equals, != 0 otherwise
 * @brief Compares the too strings
 */
static int
efreet_menu_cb_app_dirs_compare(Efreet_Menu_App_Dir *a, const char *b)
{
    if (!a->path || !b) return 1;
    if (a->path == b) return 0;
    return strcmp(a->path, b);
}

static void
efreet_menu_create_sub_menu_list(Efreet_Menu_Internal *internal)
{
    if (!internal || internal->sub_menus) return;

    internal->sub_menus = NULL;
}

static void
efreet_menu_create_app_dirs_list(Efreet_Menu_Internal *internal)
{
    if (!internal || internal->app_dirs) return;

    internal->app_dirs = NULL;
}

static void
efreet_menu_create_directory_dirs_list(Efreet_Menu_Internal *internal)
{
    if (!internal || internal->directory_dirs) return;

    internal->directory_dirs = NULL;
}

static void
efreet_menu_create_move_list(Efreet_Menu_Internal *internal)
{
    if (!internal || internal->moves) return;

    internal->moves = NULL;
}

static void
efreet_menu_create_filter_list(Efreet_Menu_Internal *internal)
{
    if (!internal || internal->filters) return;

    internal->filters = NULL;
}

static void
efreet_menu_create_layout_list(Efreet_Menu_Internal *internal)
{
    if (!internal || internal->layout) return;

    internal->layout = NULL;
}

static void
efreet_menu_create_default_layout_list(Efreet_Menu_Internal *internal)
{
    if (!internal || internal->default_layout) return;

    internal->default_layout = NULL;
}

static void
efreet_menu_create_directories_list(Efreet_Menu_Internal *internal)
{
    if (!internal || internal->directories) return;

    internal->directories = NULL;
}

static const char *
efreet_menu_path_get(Efreet_Menu_Internal *internal, const char *suffix)
{
    char path[PATH_MAX];
    size_t len;

    /* see if we've got an absolute or relative path */
    if (suffix[0] == '/')
        snprintf(path, sizeof(path), "%s", suffix);

    else
    {
        if (!internal->file.path)
        {
            INF("efreet_menu_handle_app_dir() missing menu path ... '%s'", internal->file.name);
            return NULL;
        }
        snprintf(path, sizeof(path), "%s/%s", internal->file.path, suffix);
    }

    len = strlen(path);
    while (path[len] == '/') path[len--] = '\0';

    return eina_stringshare_add(path);
}

static int
efreet_menu_cb_menu_compare(Efreet_Menu_Internal *a, Efreet_Menu_Internal *b)
{
    if (!a->name.internal || !b->name.internal) return 1;
    if (a->name.internal == b->name.internal) return 0;
    return strcmp(a->name.internal, b->name.internal);
}

static int
efreet_menu_app_dirs_process(Efreet_Menu_Internal *internal)
{
    Efreet_Menu_App_Dir *app_dir;
    Efreet_Menu_Desktop *md;
    Eina_List *l;

    EINA_LIST_FREE(internal->app_pool, md)
        efreet_menu_desktop_free(md);

    EINA_LIST_FOREACH(internal->app_dirs, l, app_dir)
        efreet_menu_app_dir_scan(internal, app_dir->path, app_dir->prefix, app_dir->legacy);

    return 1;
}

static int
efreet_menu_app_dir_scan(Efreet_Menu_Internal *internal, const char *path, const char *id, int legacy)
{
    Efreet_Desktop *desktop;
    Efreet_Menu_Desktop *menu_desktop;
    char buf2[PATH_MAX];
    Eina_Iterator *it;
    Eina_File_Direct_Info *info;

    it = eina_file_stat_ls(path);
    if (!it) return 1;

    EINA_ITERATOR_FOREACH(it, info)
    {
        const char *fname;

        fname = info->path + info->name_start;
        if (id)
            snprintf(buf2, sizeof(buf2), "%s-%s", id, fname);
        else
        {
            strncpy(buf2, fname, PATH_MAX);
            buf2[PATH_MAX - 1] = '\0';
        }

        if (info->type == EINA_FILE_DIR)
        {
            if (!legacy)
                efreet_menu_app_dir_scan(internal, info->path, buf2, legacy);
        }
        else
        {
            const char *ext;

            ext = strrchr(fname, '.');

            if (!ext || strcmp(ext, ".desktop")) continue;
            desktop = efreet_desktop_get(info->path);

            if (!desktop || desktop->type != EFREET_DESKTOP_TYPE_APPLICATION)
            {
                if (desktop) efreet_desktop_free(desktop);
                continue;
            }
            /* Don't add two files with the same id in the app pool */
            if (eina_list_search_unsorted(internal->app_pool,
                                          EINA_COMPARE_CB(efreet_menu_cb_md_compare_ids),
                                          buf2))
            {
                if (desktop) efreet_desktop_free(desktop);
                continue;
            }

            menu_desktop = efreet_menu_desktop_new();
            menu_desktop->desktop = desktop;
            menu_desktop->id = eina_stringshare_add(buf2);
            internal->app_pool = eina_list_prepend(internal->app_pool, menu_desktop);
        }
    }
    eina_iterator_free(it);

    return 1;
}

/**
 * @internal
 * @param menu The menu to work with
 * @return Returns 1 on success or 0 on failure
 * @brief Process the directory dirs in @a menu
 */
static int
efreet_menu_directory_dirs_process(Efreet_Menu_Internal *internal)
{
    const char *path;
    Eina_List *l;

    if (internal->directory_dirs)
    {
        internal->directory_cache =
            eina_hash_string_superfast_new(EINA_FREE_CB(efreet_desktop_free));

        EINA_LIST_REVERSE_FOREACH(internal->directory_dirs, l, path)
            efreet_menu_directory_dir_scan(path, NULL, internal->directory_cache);
    }

    if (internal->directories)
    {
        EINA_LIST_REVERSE_FOREACH(internal->directories, l, path)
        {
            internal->directory = efreet_menu_directory_get(internal, path);
            if (internal->directory) break;
        }
    }
    if (!internal->directory)
        internal->name.name = internal->name.internal;
    else
        internal->name.name = internal->directory->name;

    return 1;
}

/**
 * @internal
 * @param path The path to scan
 * @param relative_path The relative portion of the path
 * @param cache The cache to populate
 * @return Returns 1 on success or 0 on failure
 * @brief Scans the given directory dir for .directory files and adds the
 * applications to the cache
 */
static int
efreet_menu_directory_dir_scan(const char *path, const char *relative_path,
                                Eina_Hash *cache)
{
    Efreet_Desktop *desktop;
    char buf2[PATH_MAX];
    Eina_Iterator *it;
    Eina_File_Direct_Info *info;
    char *ext;

    it = eina_file_stat_ls(path);
    if (!it) return 1;

    EINA_ITERATOR_FOREACH(it, info)
    {
        const char *fname;

        fname = info->path + info->name_start;
        if (relative_path)
            snprintf(buf2, sizeof(buf2), "%s/%s", relative_path, fname);
        else
        {
            strncpy(buf2, fname, PATH_MAX);
            buf2[PATH_MAX - 1] = '\0';
        }

        if (info->type == EINA_FILE_DIR)
            efreet_menu_directory_dir_scan(info->path, buf2, cache);

        else
        {
            ext = strrchr(fname, '.');
            if (!ext || strcmp(ext, ".directory")) continue;

            desktop = efreet_desktop_get(info->path);
            if (!desktop || desktop->type != EFREET_DESKTOP_TYPE_DIRECTORY)
            {
                efreet_desktop_free(desktop);
                continue;
            }

            eina_hash_del(cache, buf2, NULL);
            eina_hash_add(cache, buf2, desktop);
        }
    }
    eina_iterator_free(it);

    return 1;
}

/**
 * @internal
 * @param menu The menu to work with
 * @param path The path to work with
 * @return Returns the desktop file for this path or NULL if none exists
 * @brief Finds the desktop file for the given path.
 */
static Efreet_Desktop *
efreet_menu_directory_get(Efreet_Menu_Internal *internal, const char *path)
{
    Efreet_Desktop *dir;

    if (internal->directory_cache)
    {
        dir = eina_hash_find(internal->directory_cache, path);
        if (dir) return dir;
    }

    if (internal->parent)
        return efreet_menu_directory_get(internal->parent, path);

    return NULL;
}

/**
 * @internal
 * @param a The first desktop
 * @param b The second desktop
 * @return Returns the comparison of the desktop files
 * @brief Compares the desktop files.
 */
static int
efreet_menu_cb_md_compare(const Efreet_Menu_Desktop *a, const Efreet_Menu_Desktop *b)
{
#ifdef STRICT_SPEC
    return strcmp(ecore_file_file_get(a->desktop->orig_path), ecore_file_file_get(b->desktop->orig_path));
#else
    if (a->desktop->name == b->desktop->name) return 0;
    return strcasecmp(a->desktop->name, b->desktop->name);
#endif
}

static int
efreet_menu_cb_compare_names(Efreet_Menu_Internal *internal, const char *name)
{
    if (internal->name.internal == name) return 0;
    return strcmp(internal->name.internal, name);
}

static int
efreet_menu_cb_md_compare_ids(Efreet_Menu_Desktop *md, const char *name)
{
    if (md->id == name) return 0;
    return strcmp(md->id, name);
}

static Efreet_Menu *
efreet_menu_layout_menu(Efreet_Menu_Internal *internal)
{
    Efreet_Menu *entry;
    Eina_List *layout = NULL;
    Eina_List *l;

    if (internal->parent)
    {
        /* Copy default layout rules */
        if (internal->show_empty == -1)    internal->show_empty = internal->parent->show_empty;
        if (internal->in_line == -1)       internal->in_line = internal->parent->in_line;
        if (internal->inline_limit == -1)  internal->inline_limit = internal->parent->inline_limit;
        if (internal->inline_header == -1) internal->inline_header = internal->parent->inline_header;
        if (internal->inline_alias == -1)  internal->inline_alias = internal->parent->inline_alias;
    }

    if (internal->layout)
        layout = internal->layout;

    else if (internal->parent)
    {
        Efreet_Menu_Internal *parent;
        parent = internal->parent;
        do
        {
            layout = parent->default_layout;
            parent = parent->parent;
        } while (!layout && parent);
    }

    /* init entry */
    entry = efreet_menu_entry_new();
    entry->type = EFREET_MENU_ENTRY_MENU;
    entry->id = eina_stringshare_add(internal->name.internal);
    entry->name = eina_stringshare_add(internal->name.name);
    if (internal->directory)
    {
        entry->icon = eina_stringshare_add(internal->directory->icon);
        efreet_desktop_ref(internal->directory);
        entry->desktop = internal->directory;
    }
    entry->entries = NULL;

#if 1 //STRICT_SPEC
    if (internal->sub_menus)
    {
        internal->sub_menus = eina_list_sort(internal->sub_menus,
                                             0,
                                             EINA_COMPARE_CB(efreet_menu_cb_menu_compare));
    }
#endif

    if (layout)
    {
        Efreet_Menu_Layout *lay;

        EINA_LIST_FOREACH(layout, l, lay)
            efreet_menu_layout_entries_get(entry, internal, lay);
    }
    else
    {
        /* Default layout, first menus, then desktop */
        if (internal->sub_menus)
        {
            Efreet_Menu_Internal *sub;

            EINA_LIST_FOREACH(internal->sub_menus, l, sub)
            {
                Efreet_Menu *sub_entry;
                if ((sub->directory && sub->directory->no_display) || sub->deleted) continue;
                sub_entry = efreet_menu_layout_menu(sub);
                /* Don't show empty menus */
                if (!sub_entry->entries)
                {
                    efreet_menu_free(sub_entry);
                    continue;
                }
                entry->entries = eina_list_append(entry->entries, sub_entry);
            }
        }

        if (internal->applications)
        {
            Efreet_Menu_Desktop *md;

            EINA_LIST_FOREACH(internal->applications, l, md)
            {
                Efreet_Menu *sub_entry;
                sub_entry = efreet_menu_layout_desktop(md);
                entry->entries = eina_list_append(entry->entries, sub_entry);
            }
        }
    }

    /* Don't keep this list around if it is empty */

    return entry;
}

static Efreet_Menu *
efreet_menu_layout_desktop(Efreet_Menu_Desktop *md)
{
    Efreet_Menu *entry;

    /* init entry */
    entry = efreet_menu_entry_new();
    entry->type = EFREET_MENU_ENTRY_DESKTOP;
    entry->id = eina_stringshare_add(md->id);
    entry->name = eina_stringshare_add(md->desktop->name);
    if (md->desktop->icon) entry->icon = eina_stringshare_add(md->desktop->icon);
    efreet_desktop_ref(md->desktop);
    entry->desktop = md->desktop;

    return entry;
}

static void
efreet_menu_layout_entries_get(Efreet_Menu *entry, Efreet_Menu_Internal *internal,
        Efreet_Menu_Layout *layout)
{
    Efreet_Menu *sub_entry;

    if (internal->sub_menus && layout->type == EFREET_MENU_LAYOUT_MENUNAME)
    {
        Efreet_Menu_Internal *sub;

        /* Efreet_Menu_Layout might be from DefaultLayout, so we need a local copy */
        int show_empty, in_line, inline_limit, inline_header, inline_alias;

        if (layout->show_empty == -1) show_empty = internal->show_empty;
        else show_empty = layout->show_empty;

        if (layout->in_line == -1) in_line = internal->in_line;
        else in_line = layout->in_line;

        if (layout->inline_limit == -1) inline_limit = internal->inline_limit;
        else inline_limit = layout->inline_limit;

        if (layout->inline_header == -1) inline_header = internal->inline_header;
        else inline_header = layout->inline_header;

        if (layout->inline_alias == -1) inline_alias = internal->inline_alias;
        else inline_alias = layout->inline_alias;

        sub = eina_list_search_unsorted(internal->sub_menus,
                                        EINA_COMPARE_CB(efreet_menu_cb_compare_names), layout->name);
        if (sub)
        {
            if (!(sub->directory && sub->directory->no_display) && !sub->deleted)
            {
                sub_entry = efreet_menu_layout_menu(sub);
                if (!show_empty && efreet_menu_layout_is_empty(sub_entry))
                    efreet_menu_free(sub_entry);
                else if (in_line &&
                        ((inline_limit == 0) ||
                         (!sub_entry->entries ||
                          (inline_limit > 0 && eina_list_count(sub_entry->entries) <= (unsigned int)inline_limit))))
                {
                    /* Inline */
                    if (!sub_entry->entries)
                    {
                        /* Can't inline an empty submenu */
                        entry->entries = eina_list_append(entry->entries, sub_entry);
                    }
                    else if (inline_alias && (eina_list_count(sub_entry->entries) == 1))
                    {
                        Efreet_Menu *tmp;

                        tmp = eina_list_data_get(sub_entry->entries);
                        sub_entry->entries = eina_list_remove_list(sub_entry->entries, sub_entry->entries);
                        IF_RELEASE(tmp->name);
                        tmp->name = sub_entry->name;
                        sub_entry->name = NULL;
                        IF_RELEASE(tmp->icon);
                        tmp->icon = sub_entry->icon;
                        sub_entry->icon = NULL;
                        entry->entries = eina_list_append(entry->entries, tmp);
                        efreet_menu_free(sub_entry);
                    }
                    else
                    {
                        Efreet_Menu *tmp;

                        if (inline_header)
                        {
                            tmp = efreet_menu_entry_new();
                            tmp->type = EFREET_MENU_ENTRY_HEADER;
                            tmp->name = sub_entry->name;
                            sub_entry->name = NULL;
                            tmp->icon = sub_entry->icon;
                            sub_entry->icon = NULL;
                            entry->entries = eina_list_append(entry->entries, tmp);
                        }
                        while ((tmp = eina_list_data_get(sub_entry->entries)))
                        {
                            sub_entry->entries = eina_list_remove_list(sub_entry->entries, sub_entry->entries);
                            entry->entries = eina_list_append(entry->entries, tmp);
                        }
                        efreet_menu_free(sub_entry);
                    }
                }
                else
                    entry->entries = eina_list_append(entry->entries, sub_entry);
            }
            internal->sub_menus = eina_list_remove(internal->sub_menus, sub);
            efreet_menu_internal_free(sub);
        }
    }
    else if (internal->applications && layout->type == EFREET_MENU_LAYOUT_FILENAME)
    {
        Efreet_Menu_Desktop *md;
        md = eina_list_search_unsorted(internal->applications,
                                       EINA_COMPARE_CB(efreet_menu_cb_md_compare_ids), layout->name);
        if (md)
        {
            sub_entry = efreet_menu_layout_desktop(md);
            entry->entries = eina_list_append(entry->entries, sub_entry);
            internal->applications = eina_list_remove(internal->applications, md);
        }
    }
    else if (layout->type == EFREET_MENU_LAYOUT_MERGE)
    {
        if (internal->applications && !strcmp(layout->name, "files"))
        {
            Efreet_Menu_Desktop *md;

            while ((md = eina_list_data_get(internal->applications)))
            {
                internal->applications = eina_list_remove_list(internal->applications,
                                                               internal->applications);
                sub_entry = eina_list_search_unsorted(entry->entries,
                                                      EINA_COMPARE_CB(efreet_menu_cb_entry_compare_desktop),
                                                      md->desktop);
                if (!sub_entry)
                {
                    sub_entry = efreet_menu_layout_desktop(md);
                    entry->entries = eina_list_append(entry->entries, sub_entry);
                }
            }
            internal->applications = eina_list_free(internal->applications);
        }
        else if (internal->sub_menus && !strcmp(layout->name, "menus"))
        {
            Efreet_Menu_Internal *sub;

            while ((sub = eina_list_data_get(internal->sub_menus)))
            {
                internal->sub_menus = eina_list_remove_list(internal->sub_menus, internal->sub_menus);
                if ((sub->directory && sub->directory->no_display) || sub->deleted)
                {
                    efreet_menu_internal_free(sub);
                    continue;
                }
                sub_entry = eina_list_search_unsorted(entry->entries,
                                                      EINA_COMPARE_CB(efreet_menu_cb_entry_compare_menu),
                                                      sub);
                if (!sub_entry)
                {
                    sub_entry = efreet_menu_layout_menu(sub);
                    if (!internal->show_empty && efreet_menu_layout_is_empty(sub_entry))
                        efreet_menu_free(sub_entry);
                    else if (internal->in_line &&
                            ((internal->inline_limit == 0) ||
                             (!sub_entry->entries ||
                              (internal->inline_limit > 0 && eina_list_count(sub_entry->entries) <= (unsigned int)internal->inline_limit))))
                    {
                        /* Inline */
                        if (!sub_entry->entries)
                        {
                            /* Can't inline an empty submenu */
                            entry->entries = eina_list_append(entry->entries, sub_entry);
                        }
                        else if (internal->inline_alias && (eina_list_count(sub_entry->entries) == 1))
                        {
                            Efreet_Menu *tmp;

                            tmp = eina_list_data_get(sub_entry->entries);
                            sub_entry->entries = eina_list_remove_list(sub_entry->entries, sub_entry->entries);
                            eina_stringshare_del(tmp->name);
                            tmp->name = sub_entry->name;
                            sub_entry->name = NULL;
                            IF_RELEASE(tmp->icon);
                            if (sub_entry->icon)
                            {
                                tmp->icon = sub_entry->icon;
                                sub_entry->icon = NULL;
                            }
                            entry->entries = eina_list_append(entry->entries, tmp);
                            efreet_menu_free(sub_entry);
                        }
                        else
                        {
                            Efreet_Menu *tmp;

                            if (internal->inline_header)
                            {
                                tmp = efreet_menu_entry_new();
                                tmp->type = EFREET_MENU_ENTRY_HEADER;
                                tmp->name = sub_entry->name;
                                sub_entry->name = NULL;
                                if (sub_entry->icon) tmp->icon = sub_entry->icon;
                                sub_entry->icon = NULL;
                                entry->entries = eina_list_append(entry->entries, tmp);
                            }
                            while ((tmp = eina_list_data_get(sub_entry->entries)))
                            {
                                sub_entry->entries = eina_list_remove_list(sub_entry->entries,
                                                                           sub_entry->entries);
                                entry->entries = eina_list_append(entry->entries, tmp);
                            }
                            efreet_menu_free(sub_entry);
                        }
                    }
                    else
                        entry->entries = eina_list_append(entry->entries, sub_entry);
                }
                efreet_menu_internal_free(sub);
            }
            IF_FREE_LIST(internal->sub_menus, efreet_menu_internal_free);
        }
        else if (internal->sub_menus && !strcmp(layout->name, "all"))
        {
            const char *orig;

            orig = layout->name;
            layout->name = "menus";
            efreet_menu_layout_entries_get(entry, internal, layout);
            layout->name = "files";
            efreet_menu_layout_entries_get(entry, internal, layout);
            layout->name = orig;
        }
    }
    else if (layout->type == EFREET_MENU_LAYOUT_SEPARATOR)
    {
        sub_entry = efreet_menu_entry_new();
        sub_entry->type = EFREET_MENU_ENTRY_SEPARATOR;
        entry->entries = eina_list_append(entry->entries, sub_entry);
    }
}

static int
efreet_menu_cb_entry_compare_menu(Efreet_Menu *entry, Efreet_Menu_Internal *internal)
{
    if (entry->type != EFREET_MENU_ENTRY_MENU) return 1;
    if (!entry->name || !internal->name.name) return 1;
    if (entry->name == internal->name.name) return 0;
    return strcmp(entry->name, internal->name.name);
}

static int
efreet_menu_cb_entry_compare_desktop(Efreet_Menu *entry, Efreet_Desktop *desktop)
{
    if (entry->type != EFREET_MENU_ENTRY_DESKTOP) return -1;
    if (!entry->name || !desktop->name) return -1;
    if (entry->name == desktop->name) return 0;
    return strcmp(entry->name, desktop->name);
}

#ifndef STRICT_SPEC
static int
efreet_menu_cb_move_compare(Efreet_Menu_Move *move, const char *old)
{
    if (!move->old_name || !old) return 1;
    if (move->old_name == old) return 0;
    return 1;
}
#endif

static int
efreet_menu_layout_is_empty(Efreet_Menu *entry)
{
    Efreet_Menu *sub_entry;
    Eina_List *l;

    if (!entry->entries) return 1;

    EINA_LIST_FOREACH(entry->entries, l, sub_entry)
    {
        if (sub_entry->type == EFREET_MENU_ENTRY_MENU) return 0;
        if (sub_entry->type == EFREET_MENU_ENTRY_DESKTOP) return 0;
    }
    return 1;
}

static void
efreet_menu_path_set(Efreet_Menu_Internal *internal, const char *path)
{
    char *tmp, *p;
    size_t len;

    len = strlen(path) + 1;
    tmp = alloca(len);
    memcpy(tmp, path, len);
    p = strrchr(tmp, '/');
    if (p)
    {
        *p = '\0';
        p++;

        internal->file.path = eina_stringshare_add(tmp);
        internal->file.name = eina_stringshare_add(p);
    }
}

static int
efreet_menu_save_menu(Efreet_Menu *menu, FILE *f, int indent)
{
    Eina_List *l;

    efreet_menu_save_indent(f, indent);
    fprintf(f, "<Menu>\n");
    if (menu->name)
    {
        efreet_menu_save_indent(f, indent + 1);
        fprintf(f, "<Name>%s</Name>\n", menu->name);
    }

    if (indent == 0)
    {
        /* Only save these for the root element */
        efreet_menu_save_indent(f, indent + 1);
        fprintf(f, "<DefaultAppDirs/>\n");
        efreet_menu_save_indent(f, indent + 1);
        fprintf(f, "<DefaultDirectoryDirs/>\n");
    }

    if (menu->desktop)
    {
        efreet_menu_save_indent(f, indent + 1);
        fprintf(f, "<Directory>%s</Directory>\n", menu->desktop->orig_path);
    }

    if (menu->entries)
    {
        Efreet_Menu *entry;
        int has_desktop = 0, has_menu = 0;

        efreet_menu_save_indent(f, indent + 1);
        fprintf(f, "<Layout>\n");
        EINA_LIST_FOREACH(menu->entries, l, entry)
        {
            if (entry->type == EFREET_MENU_ENTRY_MENU)
            {
                efreet_menu_save_indent(f, indent + 2);
                fprintf(f, "<Menuname>%s</Menuname>\n", entry->id);
                has_menu = 1;
            }
            else if (entry->type == EFREET_MENU_ENTRY_DESKTOP)
            {
                efreet_menu_save_indent(f, indent + 2);
                fprintf(f, "<Filename>%s</Filename>\n", entry->id);
                has_desktop = 1;
            }
            else if (entry->type == EFREET_MENU_ENTRY_SEPARATOR)
            {
                efreet_menu_save_indent(f, indent + 2);
                fprintf(f, "<Separator/>\n");
            }
        }
        efreet_menu_save_indent(f, indent + 1);
        fprintf(f, "</Layout>\n");

        if (has_desktop)
        {
            efreet_menu_save_indent(f, indent + 1);
            fprintf(f, "<Include>\n");
            EINA_LIST_FOREACH(menu->entries, l, entry)
            {
                if (entry->type == EFREET_MENU_ENTRY_DESKTOP)
                {
                    efreet_menu_save_indent(f, indent + 2);
                    fprintf(f, "<Filename>%s</Filename>\n", entry->id);
                }
            }
            efreet_menu_save_indent(f, indent + 1);
            fprintf(f, "</Include>\n");
        }

        if (has_menu)
        {
            EINA_LIST_FOREACH(menu->entries, l, entry)
            {
                if (entry->type == EFREET_MENU_ENTRY_MENU)
                    efreet_menu_save_menu(entry, f, indent + 1);
            }
        }
    }
    efreet_menu_save_indent(f, indent);
    fprintf(f, "</Menu>\n");
    return 1;
}

static int
efreet_menu_save_indent(FILE *f, int indent)
{
    int i;

    for (i = 0; i < indent; i++)
        fprintf(f, "  ");
    return 1;
}

