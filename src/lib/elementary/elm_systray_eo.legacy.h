#ifndef _ELM_SYSTRAY_EO_LEGACY_H_
#define _ELM_SYSTRAY_EO_LEGACY_H_

#ifndef _ELM_SYSTRAY_EO_CLASS_TYPE
#define _ELM_SYSTRAY_EO_CLASS_TYPE

typedef Eo Elm_Systray;

#endif

#ifndef _ELM_SYSTRAY_EO_TYPES
#define _ELM_SYSTRAY_EO_TYPES

/** Category of the Status Notifier Item.
 *
 * @since 1.8
 *
 * @ingroup Elm_Systray
 */
typedef enum
{
  ELM_SYSTRAY_CATEGORY_APP_STATUS = 0, /**< Indicators of application status */
  ELM_SYSTRAY_CATEGORY_COMMUNICATIONS, /**< Communications apps */
  ELM_SYSTRAY_CATEGORY_SYS_SERVICES, /**< System Service apps */
  ELM_SYSTRAY_CATEGORY_HARDWARE, /**< Hardware indicators */
  ELM_SYSTRAY_CATEGORY_OTHER /**< Undefined category */
} Elm_Systray_Category;

/** Application status information.
 *
 * @since 1.8
 *
 * @ingroup Elm_Systray
 */
typedef enum
{
  ELM_SYSTRAY_STATUS_PASSIVE = 0, /**< Passive (normal) */
  ELM_SYSTRAY_STATUS_ACTIVE, /**< Active */
  ELM_SYSTRAY_STATUS_ATTENTION /**< Needs Attention */
} Elm_Systray_Status;


#endif

/**
 * @brief Set the id of the Status Notifier Item.
 *
 * @param[in] obj The object.
 * @param[in] id Status notifier item ID
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API void elm_systray_id_set(Elm_Systray *obj, const char *id);

/**
 * @brief Get the id of the Status Notifier Item.
 *
 * @param[in] obj The object.
 *
 * @return Status notifier item ID
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API const char *elm_systray_id_get(const Elm_Systray *obj);

/**
 * @brief Set the category of the Status Notifier Item.
 *
 * @param[in] obj The object.
 * @param[in] cat Category
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API void elm_systray_category_set(Elm_Systray *obj, Elm_Systray_Category cat);

/**
 * @brief Get the category of the Status Notifier Item.
 *
 * @param[in] obj The object.
 *
 * @return Category
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API Elm_Systray_Category elm_systray_category_get(const Elm_Systray *obj);

/**
 * @brief Set the path to the theme where the icons can be found. Set this
 * value to "" to use the default path.
 *
 * @param[in] obj The object.
 * @param[in] icon_theme_path Icon theme path
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API void elm_systray_icon_theme_path_set(Elm_Systray *obj, const char *icon_theme_path);

/**
 * @brief Get the path to the icon's theme currently in use.
 *
 * @param[in] obj The object.
 *
 * @return Icon theme path
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API const char *elm_systray_icon_theme_path_get(const Elm_Systray *obj);

/**
 * @brief Set the object path of the D-Bus Menu that is to be show when the
 * Status Notifier Item is activated by the user.
 *
 * @param[in] obj The object.
 * @param[in] menu Object path for DBus menu
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API void elm_systray_menu_set(Elm_Systray *obj, const Efl_Object *menu);

/**
 * @brief Get the object path of the D-Bus Menu currently in use.
 *
 * @param[in] obj The object.
 *
 * @return Object path for DBus menu
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API const Efl_Object *elm_systray_menu_get(const Elm_Systray *obj);

/**
 * @brief Set the name of the attention icon to be used by the Status Notifier
 * Item.
 *
 * @param[in] obj The object.
 * @param[in] att_icon_name Attention icon name
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API void elm_systray_att_icon_name_set(Elm_Systray *obj, const char *att_icon_name);

/**
 * @brief Get the name of the attention icon used by the Status Notifier Item.
 *
 * @param[in] obj The object.
 *
 * @return Attention icon name
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API const char *elm_systray_att_icon_name_get(const Elm_Systray *obj);

/**
 * @brief Set the status of the Status Notifier Item.
 *
 * @param[in] obj The object.
 * @param[in] st Status
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API void elm_systray_status_set(Elm_Systray *obj, Elm_Systray_Status st);

/**
 * @brief Get the status of the Status Notifier Item.
 *
 * @param[in] obj The object.
 *
 * @return Status
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API Elm_Systray_Status elm_systray_status_get(const Elm_Systray *obj);

/**
 * @brief Set the name of the icon to be used by the Status Notifier Item.
 *
 * @param[in] obj The object.
 * @param[in] icon_name Status icon name
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API void elm_systray_icon_name_set(Elm_Systray *obj, const char *icon_name);

/**
 * @brief Get the name of the icon used by the Status Notifier Item.
 *
 * @param[in] obj The object.
 *
 * @return Status icon name
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API const char *elm_systray_icon_name_get(const Elm_Systray *obj);

/**
 * @brief Set the title of the Status Notifier Item.
 *
 * @param[in] obj The object.
 * @param[in] title Title
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API void elm_systray_title_set(Elm_Systray *obj, const char *title);

/**
 * @brief Get the title of the Status Notifier Item.
 *
 * @param[in] obj The object.
 *
 * @return Title
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API const char *elm_systray_title_get(const Elm_Systray *obj);

/**
 * @brief Register this Status Notifier Item in the System Tray Watcher. This
 * function should only be called after the event #ELM_EVENT_SYSTRAY_READY is
 * emitted.
 *
 * @param[in] obj The object.
 *
 * @return @c true on success, @c false otherwise
 *
 * @ingroup Elm_Systray_Group
 */
ELM_API Eina_Bool elm_systray_register(Elm_Systray *obj);

#endif
