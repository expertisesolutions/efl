#ifndef _ECORE_WL2_H_
# define _ECORE_WL2_H_

# include <Eina.h>
# include <Ecore.h>
# include <wayland-client.h>
# include <wayland-cursor.h>
# include <xkbcommon/xkbcommon.h>

# define WL_HIDE_DEPRECATED
# include <wayland-server.h>

#include <ecore_wl2_api.h>

/* # ifdef __cplusplus */
/* extern "C" { */
/* # endif */

# ifdef EFL_BETA_API_SUPPORT

#define ECORE_WL2_SURFACE_INTERFACE_VERSION 1

typedef struct _Ecore_Wl2_Subsurface Ecore_Wl2_Subsurface;

#  ifndef _ECORE_WL2_WINDOW_PREDEF
typedef struct _Ecore_Wl2_Window Ecore_Wl2_Window;
#  endif

typedef struct _Ecore_Wl2_Display Ecore_Wl2_Display;
typedef struct _Ecore_Wl2_Output Ecore_Wl2_Output;
typedef struct _Ecore_Wl2_Input Ecore_Wl2_Input;
typedef struct _Ecore_Wl2_Seat Ecore_Wl2_Seat;
typedef struct _Ecore_Wl2_Pointer Ecore_Wl2_Pointer;
typedef struct _Ecore_Wl2_Keyboard Ecore_Wl2_Keyboard;
typedef struct _Ecore_Wl2_Touch Ecore_Wl2_Touch;
typedef struct _Ecore_Wl2_Offer Ecore_Wl2_Offer;
typedef struct _Ecore_Wl2_Surface Ecore_Wl2_Surface;

/* matches protocol values */
typedef enum
{
   ECORE_WL2_DRAG_ACTION_NONE = 0,
   ECORE_WL2_DRAG_ACTION_COPY = 1,
   ECORE_WL2_DRAG_ACTION_MOVE = 2,
   ECORE_WL2_DRAG_ACTION_ASK = 4,
   ECORE_WL2_DRAG_ACTION_LAST = 5,
} Ecore_Wl2_Drag_Action;

typedef enum
{
  ECORE_WL2_SEAT_CAPABILITIES_NONE = 0,
  ECORE_WL2_SEAT_CAPABILITIES_POINTER = 1,
  ECORE_WL2_SEAT_CAPABILITIES_KEYBOARD = 2,
  ECORE_WL2_SEAT_CAPABILITIES_TOUCH = 4
} Ecore_Wl2_Seat_Capabilities;

struct _Ecore_Wl2_Event_Connection
{
   Ecore_Wl2_Display *display;
};

typedef struct _Ecore_Wl2_Event_Connection Ecore_Wl2_Event_Connect;
typedef struct _Ecore_Wl2_Event_Connection Ecore_Wl2_Event_Disconnect;

typedef struct _Ecore_Wl2_Global
{
   Eina_Stringshare *interface;
   unsigned int id, version;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Global;

typedef struct _Ecore_Wl2_Event_Global
{
   Ecore_Wl2_Display *display;
   Eina_Stringshare *interface;
   unsigned int id, version;
} Ecore_Wl2_Event_Global;

typedef struct _Ecore_Wl2_Event_Focus_In
{
   Ecore_Wl2_Window *window;
   unsigned int timestamp;
   Eo *dev; //The seat device
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Focus_In;

typedef struct _Ecore_Wl2_Event_Focus_Out
{
   Ecore_Wl2_Window *window;
   unsigned int timestamp;
   Eo *dev; //The seat device
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Focus_Out;

typedef struct _Ecore_Wl2_Event_Dnd_Enter
{
   Ecore_Wl2_Window *win, *source;
   Ecore_Wl2_Offer *offer;
   int x, y;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_Enter;

typedef struct _Ecore_Wl2_Event_Dnd_Leave
{
   Ecore_Wl2_Window *win, *source;
   Ecore_Wl2_Offer *offer;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_Leave;

typedef struct _Ecore_Wl2_Event_Dnd_Motion
{
   Ecore_Wl2_Window *win, *source;
   Ecore_Wl2_Offer *offer;
   int x, y;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_Motion;

typedef struct _Ecore_Wl2_Event_Dnd_Drop
{
   Ecore_Wl2_Window *win, *source;
   int x, y;
   Ecore_Wl2_Offer *offer;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_Drop;

typedef struct _Ecore_Wl2_Event_Dnd_End
{
   Ecore_Wl2_Window *win, *source;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Dnd_End;

struct _Ecore_Wl2_Event_Data_Source_Event
{
   Ecore_Wl2_Window *win, *source;
   Ecore_Wl2_Drag_Action action;
   unsigned int seat;
   uint32_t serial;
   Ecore_Wl2_Display *display;
};

typedef struct Ecore_Wl2_Event_Data_Source_End
{
   Ecore_Wl2_Window *win, *source;
   Ecore_Wl2_Drag_Action action;
   unsigned int seat;
   uint32_t serial;
   Ecore_Wl2_Display *display;
   Eina_Bool cancelled;
} Ecore_Wl2_Event_Data_Source_End;
typedef struct _Ecore_Wl2_Event_Data_Source_Event Ecore_Wl2_Event_Data_Source_Drop;
typedef struct _Ecore_Wl2_Event_Data_Source_Event Ecore_Wl2_Event_Data_Source_Action;

typedef struct _Ecore_Wl2_Event_Data_Source_Target
{
   char *type;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Data_Source_Target;

typedef struct _Ecore_Wl2_Event_Data_Source_Send
{
   char *type;
   int fd;
   unsigned int seat;
   uint32_t serial;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Data_Source_Send;

typedef struct Ecore_Wl2_Event_Seat_Selection
{
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Selection;

typedef struct _Ecore_Wl2_Event_Seat_Name
{
   Eina_Stringshare *name;
   unsigned int id;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Name;

typedef struct _Ecore_Wl2_Event_Seat_Capabilities
{
   unsigned int id;
   Eina_Bool pointer_enabled : 1;
   Eina_Bool keyboard_enabled : 1;
   Eina_Bool touch_enabled : 1;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Capabilities;

typedef enum
{
   ECORE_WL2_DEVICE_TYPE_SEAT,
   ECORE_WL2_DEVICE_TYPE_POINTER,
   ECORE_WL2_DEVICE_TYPE_KEYBOARD,
   ECORE_WL2_DEVICE_TYPE_TOUCH
} Ecore_Wl2_Device_Type;

typedef struct _Ecore_Wl2_Event_Device
{
   Eo *dev;
   Ecore_Wl2_Window *window;
   unsigned int seat_id;
   Ecore_Wl2_Device_Type type;
} Ecore_Wl2_Event_Device;

typedef enum
{
   ECORE_WL2_SELECTION_CNP,
   ECORE_WL2_SELECTION_DND
} Ecore_Wl2_Selection_Type;

typedef enum
{
   ECORE_WL2_WINDOW_STATE_NONE = 0,
   ECORE_WL2_WINDOW_STATE_FULLSCREEN = (1 << 0),
   ECORE_WL2_WINDOW_STATE_MAXIMIZED = (1 << 1),
} Ecore_Wl2_Window_States;

typedef struct _Ecore_Wl2_Event_Window_Configure
{
   Ecore_Wl2_Window *win, *event_win;
   unsigned int edges;
   unsigned int w, h;
   unsigned int states;
} Ecore_Wl2_Event_Window_Configure;

typedef struct _Ecore_Wl2_Event_Window_Configure_Complete
{
   Ecore_Wl2_Window *win;
} Ecore_Wl2_Event_Window_Configure_Complete;

typedef struct _Ecore_Wl2_Event_Input_Keymap_Changed
{
   unsigned int id;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Keymap_Changed;

typedef struct Ecore_Wl2_Event_Seat_Keyboard_Repeat_Changed
{
   unsigned int id;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Seat_Keyboard_Repeat_Changed;

typedef struct _Ecore_Wl2_Event_Sync_Done
{
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Sync_Done;

typedef struct _Ecore_Wl2_Event_Offer_Data_Ready
{
   Ecore_Wl2_Offer *offer;
   char *data;
   int len;
   const char *mimetype;
   unsigned int seat;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Offer_Data_Ready;

typedef struct _Ecore_Wl2_Event_Output_Transform
{
   Ecore_Wl2_Output *output;
   int transform, old_transform;
} Ecore_Wl2_Event_Output_Transform;

typedef struct _Ecore_Wl2_Event_Window_Rotation
{
   Ecore_Wl2_Window *win;
   int rotation, w, h, angle;
   Eina_Bool resize : 1;
} Ecore_Wl2_Event_Window_Rotation;

typedef struct _Ecore_Wl2_Event_Window_Rotation Ecore_Wl2_Event_Window_Rotation_Change_Prepare;
typedef struct _Ecore_Wl2_Event_Window_Rotation Ecore_Wl2_Event_Window_Rotation_Change_Prepare_Done;
typedef struct _Ecore_Wl2_Event_Window_Rotation Ecore_Wl2_Event_Window_Rotation_Change_Request;
typedef struct _Ecore_Wl2_Event_Window_Rotation Ecore_Wl2_Event_Window_Rotation_Change_Done;

typedef struct _Ecore_Wl2_Event_Window_Show
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Window *parent_win;
   Ecore_Wl2_Window *event_win;
} Ecore_Wl2_Event_Window_Show;

typedef struct _Ecore_Wl2_Event_Window_Hide
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Window *parent_win;
   Ecore_Wl2_Window *event_win;
} Ecore_Wl2_Event_Window_Hide;

typedef struct _Ecore_Wl2_Event_Window_Activate
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Window *parent_win;
   Ecore_Wl2_Window *event_win;
} Ecore_Wl2_Event_Window_Activate;

typedef struct _Ecore_Wl2_Event_Window_Deactivate
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Window *parent_win;
   Ecore_Wl2_Window *event_win;
} Ecore_Wl2_Event_Window_Deactivate;

typedef struct _Ecore_Wl2_Event_Window_Common
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Window *parent_win;
   Ecore_Wl2_Window *event_win;
} Ecore_Wl2_Event_Window_Common;

typedef struct _Ecore_Wl2_Event_Window_Iconify_State_Change
{
   Ecore_Wl2_Window *win;
   unsigned int iconified;
   unsigned int force;
} Ecore_Wl2_Event_Window_Iconify_State_Change;

typedef enum _Ecore_Wl2_Window_Type
{
   ECORE_WL2_WINDOW_TYPE_NONE,
   ECORE_WL2_WINDOW_TYPE_TOPLEVEL,
   ECORE_WL2_WINDOW_TYPE_MENU,
   ECORE_WL2_WINDOW_TYPE_DND,
   ECORE_WL2_WINDOW_TYPE_CUSTOM,
   ECORE_WL2_WINDOW_TYPE_NOTIFICATION, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_UTILITY, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_DIALOG, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_DOCK, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_SPLASH, /** @since 1.20 */
   ECORE_WL2_WINDOW_TYPE_LAST
} Ecore_Wl2_Window_Type;

typedef struct _Ecore_Wl2_Event_Aux_Hint_Allowed
{
   Ecore_Wl2_Window *win;
   int id;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Aux_Hint_Allowed;

typedef struct _Ecore_Wl2_Event_Aux_Hint_Supported
{
   Ecore_Wl2_Window *win;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Aux_Hint_Supported;

typedef struct Ecore_Wl2_Event_Aux_Message
{
   Ecore_Wl2_Window *win;
   Eina_Stringshare *key;
   Eina_Stringshare *val;
   Eina_List *options;
   Ecore_Wl2_Display *display;
} Ecore_Wl2_Event_Aux_Message;

typedef struct Ecore_Wl2_Event_Window_Offscreen
{
   Ecore_Wl2_Window *win;
} Ecore_Wl2_Event_Window_Offscreen;

typedef struct _Ecore_Wl2_Buffer Ecore_Wl2_Buffer;

typedef enum _Ecore_Wl2_Buffer_Type Ecore_Wl2_Buffer_Type;
enum _Ecore_Wl2_Buffer_Type
{
   ECORE_WL2_BUFFER_NONE = 0,
   ECORE_WL2_BUFFER_SHM = 1,
   ECORE_WL2_BUFFER_DMABUF = 2
};

typedef void (*Ecore_Wl2_Bind_Cb)(struct wl_client *client, void *data, uint32_t version, uint32_t id);
typedef void (*Ecore_Wl2_Unbind_Cb)(struct wl_resource *resource);
typedef void (*Ecore_Wl2_Frame_Cb)(Ecore_Wl2_Window *win, uint32_t timestamp, void *data);
typedef struct _Ecore_Wl2_Frame_Cb_Handle Ecore_Wl2_Frame_Cb_Handle;

ECORE_WL2_API extern int ECORE_WL2_EVENT_DISCONNECT; /** @since 1.18 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_CONNECT; /** @since 1.18 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_GLOBAL_ADDED; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_GLOBAL_REMOVED; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_FOCUS_IN; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_FOCUS_OUT; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DND_ENTER; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DND_LEAVE; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DND_MOTION; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DND_DROP; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DND_END; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DATA_SOURCE_END; /** @since 1.18 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DATA_SOURCE_DROP; /** @since 1.18 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DATA_SOURCE_ACTION; /** @since 1.18 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DATA_SOURCE_TARGET; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DATA_SOURCE_SEND; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_CONFIGURE; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_SYNC_DONE; /** @since 1.17 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_OFFER_DATA_READY; /** @since 1.19 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_SEAT_NAME_CHANGED; /** @since 1.19 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_SEAT_CAPABILITIES_CHANGED; /** @since 1.19 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DEVICE_ADDED; /** @since 1.19 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_DEVICE_REMOVED; /** @since 1.19 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE; /** @since 1.19 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_SEAT_KEYMAP_CHANGED; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_SEAT_SELECTION; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_OUTPUT_TRANSFORM; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_ROTATE; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_AUX_HINT_ALLOWED; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_AUX_HINT_SUPPORTED; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_AUX_MESSAGE; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_SHOW; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_HIDE; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_ACTIVATE; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_DEACTIVATE; /** @since 1.20 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_ICONIFY_STATE_CHANGE; /** @since 1.21 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_OFFSCREEN; /** @since 1.21 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_CREATE; /** @since 1.25 */
ECORE_WL2_API extern int ECORE_WL2_EVENT_WINDOW_DESTROY; /** @since 1.25 */

typedef struct _Ecore_Wl2_Surface_Interface
{
   int id;
   int version;

   void *(*setup)(Ecore_Wl2_Window *win);
   void (*destroy)(Ecore_Wl2_Surface *surface, void *priv_data);
   void (*reconfigure)(Ecore_Wl2_Surface *surface, void *priv_data, int w, int h, uint32_t flags, Eina_Bool alpha);
   void *(*data_get)(Ecore_Wl2_Surface *surface, void *priv_data, int *w, int *h);
   int  (*assign)(Ecore_Wl2_Surface *surface, void *priv_data);
   void (*post)(Ecore_Wl2_Surface *surface, void *priv_data, Eina_Rectangle *rects, unsigned int count);
   void (*flush)(Ecore_Wl2_Surface *surface, void *priv_data, Eina_Bool purge);
} Ecore_Wl2_Surface_Interface;

/**
 * @file
 * @brief Ecore functions for dealing with the Wayland display protocol
 *
 * @defgroup Ecore_Wl2_Group Ecore_Wl2 - Wayland integration
 * @ingroup Ecore
 *
 * Ecore_Wl2 provides a wrapper and convenience functions for using the
 * Wayland protocol in implementing a window system. Function groups for
 * this part of the library include the following:
 *
 * @li @ref Ecore_Wl2_Init_Group
 * @li @ref Ecore_Wl2_Display_Group
 * @li @ref Ecore_Wl2_Window_Group
 * @li @ref Ecore_Wl2_Output_Group
 * @li @ref Ecore_Wl2_Input_Group
 */

/**
 * @defgroup Ecore_Wl2_Init_Group Wayland Library Init and Shutdown Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that start and shutdown the Ecore Wl2 Library.
 */

/**
 * Initialize the Ecore_Wl2 library
 *
 * @return  The number of times the library has been initialized without being
 *          shutdown. 0 is returned if an error occurs.
 *
 * @ingroup Ecore_Wl2_Init_Group
 * @since 1.17
 */
ECORE_WL2_API int ecore_wl2_init(void);

/**
 * Shutdown the Ecore_Wl2 Library
 *
 * @return  The number of times the library has been initialized without
 *          being shutdown.
 *
 * @ingroup Ecore_Wl2_Init_Group
 * @since 1.17
 */
ECORE_WL2_API int ecore_wl2_shutdown(void);

/**
 * @defgroup Ecore_Wl2_Display_Group Wayland Library Display Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that deal with creating, connecting, or interacting with
 * Wayland displays
 */

/**
 * Create a new Wayland display
 *
 * @brief This function is typically used to create a new display for
 * use with compositors, or to create a new display for use in nested
 * compositors.
 *
 * @param name The display target name to create. If @c NULL, a default
 *             display name will be assigned.
 * @return The newly created Ecore_Wl2_Display
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API Ecore_Wl2_Display *ecore_wl2_display_create(const char *name);

/**
 * Destroy an existing Wayland display
 *
 * @brief This function is typically used by servers to terminate an
 * existing Wayland display.
 *
 * @param display The display to terminate
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_display_destroy(Ecore_Wl2_Display *display);

/**
 * Connect to an existing Wayland display
 *
 * @brief This function is typically used by clients to connect to an
 * existing Wayland display.
 *
 * @param name The display target name to connect to. If @c NULL, the default
 *             display is assumed.
 *
 * @return The Ecore_Wl2_Display which was connected to
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API Ecore_Wl2_Display *ecore_wl2_display_connect(const char *name);

/**
 * Disconnect an existing Wayland display
 *
 * @brief This function is typically used by clients to disconnect from an
 * existing Wayland display.
 *
 * @param display The display to disconnect from
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_display_disconnect(Ecore_Wl2_Display *display);

/**
 * Retrieve the existing Wayland display
 *
 * @param display The Ecore_Wl2_Display for which to retrieve the existing
 *                Wayland display from
 *
 * @return The wl_display which this Ecore_Wl2_Display is using
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API struct wl_display *ecore_wl2_display_get(Ecore_Wl2_Display *display);

/**
 * Retrieve the wl_shm from a given Ecore_Wl2_Display
 *
 * @param display The Ecore_Wl2_Display for which to retrieve the existing
 *                Wayland shm interface from
 *
 * @return The wl_shm which this Ecore_Wl2_Display is using
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API struct wl_shm *ecore_wl2_display_shm_get(Ecore_Wl2_Display *display);

/**
 * Retrieve the wl_dmabuf from a given Ecore_Wl2_Display
 *
 *
 * @param display The Ecore_Wl2_Display for which to retrieve the existing
 *                Wayland dmabuf interface from
 *
 * @return The wl_dmabuf which this Ecore_Wl2_Display is using
 *
 * @ingroup Ecore_Wl2_Display_Group
 *
 * @note This is intended for client use only and should be used only
 *       after ecore_wl2_display_connect().  Also, the return type is
 *       void * instead of zpw_linux_dmabuf_v1 * since we don't want
 *       to change our public API every time the version changes in
 *       wayland-protocols.
 *
 * @since 1.18
 */
ECORE_WL2_API void *ecore_wl2_display_dmabuf_get(Ecore_Wl2_Display *display);

/**
 * Return an Eina_Iterator that can be used to iterate through globals
 *
 * @param display The Ecore_Wl2_Display for which to return a global iterator
 *
 * @note The caller of this function should free the returned Eina_Iterator
 * when finished with it.
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API Eina_Iterator *ecore_wl2_display_globals_get(Ecore_Wl2_Display *display);

/**
 * Retrieves the size of the current screen.
 *
 * @param display The display to get the screen size of
 * @param w where to return the width. May be NULL. Returns 0 on error.
 * @param h where to return the height. May be NULL. Returns 0 on error.
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_display_screen_size_get(Ecore_Wl2_Display *display, int *w, int *h);

/**
 * Get all the Ecore_Wl2_Input from the display.
 *
 * @param display The display
 *
 * @return A Eina_Iterator of Ecore_Wl2_Input or @c NULL on error
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.19
 */
ECORE_WL2_API Eina_Iterator *ecore_wl2_display_inputs_get(Ecore_Wl2_Display *display);

/**
 * Find a seat for a given display object using the seat id
 *
 * @param display The display
 * @param id The seat id
 *
 * @return The corresponding Ecore_Wl2_Input object or @c NULL if no match is found
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.20
 */
ECORE_WL2_API Ecore_Wl2_Input *ecore_wl2_display_input_find(const Ecore_Wl2_Display *display, unsigned int id);

/**
 * Find a seat for a given display object using the seat id
 *
 * @param display The display
 * @param name The seat name
 *
 * @return The corresponding Ecore_Wl2_Input object or @c NULL if no match is found
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.20
 */
ECORE_WL2_API Ecore_Wl2_Input *ecore_wl2_display_input_find_by_name(const Ecore_Wl2_Display *display, const char *name);

/**
 * Retrieves the Wayland Registry used for the current Wayland display.
 *
 * @param display The display to get the registry of
 *
 * @return The current wayland registry, or NULL on error
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API struct wl_registry *ecore_wl2_display_registry_get(Ecore_Wl2_Display *display);

/**
 * Check if the display has performed a sync
 *
 * @param display The display
 *
 * @return True if the display sync has occurred
 * @see ECORE_WL2_EVENT_SYNC_DONE
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Bool ecore_wl2_display_sync_is_done(const Ecore_Wl2_Display *display);

/**
 * Get the name of the display object
 *
 * @param display The display
 *
 * @return The name of the display object
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.20
 */
ECORE_WL2_API const char *ecore_wl2_display_name_get(const Ecore_Wl2_Display *display);

/**
 * Finds an Ecore_Wl2_Window based on wl_surface
 *
 * @param display The display to search for the window
 * @param surface The wl_surface of the window to find
 *
 * @return The Ecore_Wl2_Window if found, or NULL if no such window exists
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.24
 */
ECORE_WL2_API Ecore_Wl2_Window *ecore_wl2_display_window_find_by_surface(Ecore_Wl2_Display *display, struct wl_surface *surface);

/**
 * Gets the connected display object
 *
 * @brief This function is typically used by clients to get an
 *        existing Wayland display.
 *
 * @param name The display target name. If @c NULL, the default
 *        display is assumed.
 *
 * @return The Ecore_Wl2_Display which was connected to
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.24
 */
ECORE_WL2_API Ecore_Wl2_Display *ecore_wl2_connected_display_get(const char *name);

/**
 * Gets the wl_compositor which belongs to this display
 *
 * @param display The Ecore_Wl2_Display to get the compositor of
 *
 * @return The wl_compositor associated with this display
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.24
 */
ECORE_WL2_API struct wl_compositor *ecore_wl2_display_compositor_get(Ecore_Wl2_Display *display);

/**
 * @defgroup Ecore_Wl2_Window_Group Wayland Library Window Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland windows
 */

/**
 * Create a new Ecore_Wl2_Window
 *
 * @param display The Ecore_Wl2_Display on which to create this new window
 * @param parent The Ecore_Wl2_Window which is the parent of this window
 * @param x Initial x position of window
 * @param y Initial y position of window
 * @param w Initial width of window
 * @param h Initial height of window
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API Ecore_Wl2_Window *ecore_wl2_window_new(Ecore_Wl2_Display *display, Ecore_Wl2_Window *parent, int x, int y, int w, int h);

/**
 * Set a callback to be caleld just before the window is closed and freed
 *
 * @param window The window to listen to for a xdg toplevel close callback
 * @param cb The callback function to call being passed data and window
 * @param data The Data pointer to pass as data to the callback
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API void ecore_wl2_window_close_callback_set(Ecore_Wl2_Window *window, void (*cb) (void *data, Ecore_Wl2_Window *win), void *data);

/**
 * Get the wl_surface which belongs to this window
 *
 * @param window The Ecore_Wl2_Window to get the surface of
 *
 * @return The wl_surface associated with this window.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API struct wl_surface *ecore_wl2_window_surface_get(Ecore_Wl2_Window *window);

/**
 * Get the id of a given Ecore_Wl2_Window
 *
 * @param window The window to return the id of
 *
 * @return The id of the given window, or -1 on failure
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API int ecore_wl2_window_surface_id_get(Ecore_Wl2_Window *window);

/**
 * @see evas_object_size_hint_aspect_set
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.21
 */
ECORE_WL2_API void ecore_wl2_window_aspect_set(Ecore_Wl2_Window *window, int w, int h, unsigned int aspect);

/**
 * @see evas_object_size_hint_aspect_get
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API void ecore_wl2_window_aspect_get(Ecore_Wl2_Window *window, int *w, int *h, unsigned int *aspect);

/**
 * Show a given Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window to show
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_show(Ecore_Wl2_Window *window);

/**
 * Hide a given Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window to hide
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_hide(Ecore_Wl2_Window *window);

/**
 * Free a given Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window to free
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_free(Ecore_Wl2_Window *window);

/**
 * @brief Begin moving a given Ecore_Wl2_Window
 *
 * @param window The Ecore_Wl2_Window which to move
 * @param input The seat on which the move is active on
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_move(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input);

/**
 * @brief Begin resizing a given window
 *
 * @param window The Ecore_Wl2_Window which to resize
 * @param input The seat on which the resize is active
 * @param location The edge of the window from which the resize should start
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_resize(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input, int location);

/**
 * Get if a given window is alpha
 *
 * @param window The window to get if is alpha
 *
 * @return EINA_TRUE if window is alpha, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_alpha_get(Ecore_Wl2_Window *window);

/**
 * Set a given window's alpha property
 *
 * @param window The window on which to set the alpha property
 * @param alpha EINA_TRUE to set window as alpha, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_alpha_set(Ecore_Wl2_Window *window, Eina_Bool alpha);

/**
 * Set the opaque region of the Ecore_Wl2_Window
 *
 * @param window The window
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_opaque_region_set(Ecore_Wl2_Window *window, int x, int y, int w, int h);

/**
 * Get the opaque region of the Ecore_Wl2_Window
 *
 * @param window The window
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API void ecore_wl2_window_opaque_region_get(Ecore_Wl2_Window *window, int *x, int *y, int *w, int *h);

/**
 * Set the input region of the Ecore_Wl2_Window.
 *
 * To set an empty region, pass width and height as 0.
 *
 * An empty input region means the entire window surface will accept input.
 *
 * @param window The window to set the input region of
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_input_region_set(Ecore_Wl2_Window *window, int x, int y, int w, int h);

/**
 * Get the input region of the Ecore_Wl2_Window.
 *
 * @param window The window to set the input region of
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API void ecore_wl2_window_input_region_get(Ecore_Wl2_Window *window, int *x, int *y, int *w, int *h);

/**
 * Get if a given window is maximized
 *
 * @param window The window to get the maximized state of
 *
 * @return EINA_TRUE if window is maximized, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_maximized_get(Ecore_Wl2_Window *window);

/**
 * Set the maximized state of a given window
 *
 * @param window The window to set the maximized state of
 * @param maximized EINA_TRUE to set maximized, EINA_FALSE to unset
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_maximized_set(Ecore_Wl2_Window *window, Eina_Bool maximized);

/**
 * Get if a given window is fullscreen
 *
 * @param window The window to get the fullscreen state of
 *
 * @return EINA_TRUE if window is fullscreen, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_fullscreen_get(Ecore_Wl2_Window *window);

/**
 * Set the fullscreen state of a given window
 *
 * @param window The window to set the fullscreen state of
 * @param fullscreen EINA_TRUE to set fullscreen, EINA_FALSE to unset
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_fullscreen_set(Ecore_Wl2_Window *window, Eina_Bool fullscreen);

/**
 * Get if a given window is rotated
 *
 * @param window The window to get the rotation of
 *
 * @return The amount of rotation for this window, or -1 on failure
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API int ecore_wl2_window_rotation_get(Ecore_Wl2_Window *window);

/**
 * Set the rotation of a given window
 *
 * @param window The window to set the rotation of
 * @param rotation The amount of rotation
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_rotation_set(Ecore_Wl2_Window *window, int rotation);

/**
 * Set the title of a given window
 *
 * @param window The window to set the title of
 * @param title The title of the window
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_title_set(Ecore_Wl2_Window *window, const char *title);

/**
 * Get the title of a given window
 *
 * @param window The window to set the title of
 *
 * @return A string if found, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API const char *ecore_wl2_window_title_get(Ecore_Wl2_Window *window);

/**
 * Set the class of a given window
 *
 * @param window The window to set the class of
 * @param clas The class of the window
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_class_set(Ecore_Wl2_Window *window, const char *clas);


/**
 * Get the class of a given window
 *
 * @param window The window to set the class of
 *
 * @return A string if found, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API const char *ecore_wl2_window_class_get(Ecore_Wl2_Window *window);

/**
 * Get the geometry of a given window
 *
 * @brief The window geometry returned here is the window geometry as
 * recognized by xdg_surface_set_window_geometry. As such, it represents the
 * "visible bounds" of a window from the user's perspective.
 *
 * @param window The window to get the geometry of
 * @param x The left point of the window geometry
 * @param y The top point of the window geometry
 * @param w The width of the window geometry
 * @param h The height of the window geometry
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_geometry_get(Ecore_Wl2_Window *window, int *x, int *y, int *w, int *h);

/**
 * Set the geometry of a given window
 *
 * @brief The window geometry referenced here is the window geometry as
 * recognized by xdg_surface_set_window_geometry. As such, it represents the
 * "visible bounds" of a window from the user's perspective.
 *
 * @param window The window to set the geometry of
 * @param x The left point of the window geometry
 * @param y The top point of the window geometry
 * @param w The width of the window geometry
 * @param h The height of the window geometry
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_geometry_set(Ecore_Wl2_Window *window, int x, int y, int w, int h);

/**
 * Set the type of a given window
 *
 * @param window The window to set the type of
 * @param type The Ecore_Wl2_Window_Type to set on the window
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API void ecore_wl2_window_type_set(Ecore_Wl2_Window *window, Ecore_Wl2_Window_Type type);

/**
 * Get the type of a given window
 *
 * @see Ecore_Wl2_Window_Type
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API Ecore_Wl2_Window_Type ecore_wl2_window_type_get(Ecore_Wl2_Window *window);

/**
 * Find the output that a given window is on
 *
 * @param window The window to find the output for
 *
 * @return An Ecore_Wl2_Output if found, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API Ecore_Wl2_Output *ecore_wl2_window_output_find(Ecore_Wl2_Window *window);

/**
 * Set if window rotation is supported by the window manager
 *
 * @param window
 * @param enabled
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_wm_rotation_supported_set(Ecore_Wl2_Window *window, Eina_Bool enabled);

/**
 * Get if window rotation is supported by the window manager
 *
 * @param window
 *
 * @return EINA_TRUE if supported, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_wm_rotation_supported_get(Ecore_Wl2_Window *window);

/**
 * Set if an application has set window rotation
 *
 * @param window
 * @param set
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_rotation_app_set(Ecore_Wl2_Window *window, Eina_Bool set);

/**
 * Get if an application has set window rotation
 *
 * @param window
 *
 * @return EINA_TRUE if set, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_rotation_app_get(Ecore_Wl2_Window *window);

/**
 * Set preferred rotation on a given window
 *
 * @param window The window to set preferred rotation on
 * @param rot The value of the preferred rotation to set
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_preferred_rotation_set(Ecore_Wl2_Window *window, int rot);

/**
 * Get preferred rotation for a given window
 *
 * @param window The window to get preferred rotation from
 *
 * @return Given windows preferred rotation
 *
 * @ingroup Ecore_Wl2_Window
 * @since 1.20
 */
ECORE_WL2_API int ecore_wl2_window_preferred_rotation_get(Ecore_Wl2_Window *window);

/**
 * Set a windows available rotations
 *
 * @param window
 * @param rots
 * @param count
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_available_rotations_set(Ecore_Wl2_Window *window, const int *rots, unsigned int count);

/**
 * Get a windows available rotations
 *
 * @param window
 * @param rots
 * @param count
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_available_rotations_get(Ecore_Wl2_Window *window, int **rots, unsigned int *count);

/**
 * @brief Get the activated state of a window
 *
 * @param window The window to get activated state from
 *
 * @return @c EINA_TRUE if activated
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_activated_get(const Ecore_Wl2_Window *window);

/**
 * @brief Set the seat for a popup window to be used with grab
 *
 * @param window The window
 * @param input The seat
 *
 * Use this function for desktop shell requests involving popup grabs which require
 * a seat for the grab.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_popup_input_set(Ecore_Wl2_Window *window, Ecore_Wl2_Input *input);

/**
 * @brief Get the seat for a popup window to be used with grab
 *
 * @param window The window
 *
 * @return Returns Ecore_Wl2_Input if the window has an input.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API Ecore_Wl2_Input *ecore_wl2_window_popup_input_get(Ecore_Wl2_Window *window);

/**
 * Check if a window has a shell surface - without one it can't be visible.
 *
 * @param win The window to check
 *
 * @return Returns true if the window has an associated shell surface.
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.19
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_shell_surface_exists(Ecore_Wl2_Window *win);

/**
 * Get which display a given window is using
 *
 * @param window The window to get the display of
 *
 * @return The Ecore_Wl2_Display that this window is using, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.17
 */
ECORE_WL2_API Ecore_Wl2_Display *ecore_wl2_window_display_get(const Ecore_Wl2_Window *window);

/**
 * Set if this window should ignore focus requests
 *
 * @param window The window to set ignore focus requests on
 * @param focus_skip EINA_TRUE if this window should skip focus requests, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_focus_skip_set(Ecore_Wl2_Window *window, Eina_Bool focus_skip);

/**
 * Get if this window ignores focus requests
 *
 * @param window The window to get the focus skip value from
 *
 * @return EINA_TRUE if a window should skip focus requests, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_focus_skip_get(Ecore_Wl2_Window *window);

/**
 * Set the role of a given window
 *
 * @param window
 * @param role
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_role_set(Ecore_Wl2_Window *window, const char *role);

/**
 * Get the role of a given window
 *
 * @param window The window to set the class role
 *
 * @return A string if found, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API const char *ecore_wl2_window_role_get(Ecore_Wl2_Window *window);

/**
 * Set if a given window is in floating mode
 *
 * @param window The window to set floating mode on
 * @param floating EINA_TRUE if this window should be in floating mode, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_window_floating_mode_set(Ecore_Wl2_Window *window, Eina_Bool floating);

/**
 * Get if a given window is in floating mode
 *
 * @param window The window to get floating mode
 *
 * @return EINA_TRUE if floating, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_floating_mode_get(Ecore_Wl2_Window *window);

/**
 * Finds a window by surface
 *
 * @param surface The surface to find the window of
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.24
 */
ECORE_WL2_API Ecore_Wl2_Window *ecore_wl2_window_surface_find(struct wl_surface *surface);

/**
 * @defgroup Ecore_Wl2_Input_Group Wayland Library Input Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland Input
 */

/**
 * Get the wl_seat that an input is using
 *
 * @param input The Ecore_Wl2_Input to get the seat of
 *
 * @return The wl_seat of this input, or NULL otherwise
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.17
 */
ECORE_WL2_API struct wl_seat *ecore_wl2_input_seat_get(Ecore_Wl2_Input *input);

/**
 * Get the seat capabilities for a given input.
 *
 * @param input The input
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.19
 */
ECORE_WL2_API Ecore_Wl2_Seat_Capabilities ecore_wl2_input_seat_capabilities_get(Ecore_Wl2_Input *input);

/**
 * Get the wayland's seat id from an input.
 *
 * @param input The input
 *
 * @return The seat id
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.19
 */
ECORE_WL2_API unsigned int ecore_wl2_input_seat_id_get(Ecore_Wl2_Input *input) EINA_WARN_UNUSED_RESULT;

/**
 * Get the display object of an input
 *
 * @param input The input
 *
 * @return The display
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
ECORE_WL2_API Ecore_Wl2_Display *ecore_wl2_input_display_get(const Ecore_Wl2_Input *input);

/**
 * Get the xkb_keymap object of an input
 *
 * @param input The input
 *
 * @return The xkb_keymap object
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
ECORE_WL2_API struct xkb_keymap *ecore_wl2_input_keymap_get(const Ecore_Wl2_Input *input);

/**
 * Get the name of an input
 *
 * @param input The input
 *
 * @return The name
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Stringshare *ecore_wl2_input_name_get(Ecore_Wl2_Input *input);

/**
 * Get the keyboard repeat rate and delay of an input
 *
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 *
 * @return True if repeat is enabled
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Bool ecore_wl2_input_keyboard_repeat_get(const Ecore_Wl2_Input *input, double *rate, double *delay);

/**
 * Set the keyboard repeat rate and delay of an input
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 * @return True if repeat is enabled
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.24
 */
ECORE_WL2_API Eina_Bool ecore_wl2_input_keyboard_repeat_set(Ecore_Wl2_Input *input, double rate, double delay);

/**
 * Retrieves the mouse position of the seat
 *
 * @param input The seat
 * @param x where to return the horizontal position. May be NULL. Returns 0 on error.
 * @param y where to return the vertical position. May be NULL. Returns 0 on error.
 * @return @c EINA_TRUE if coords were successfully retrieved
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
ECORE_WL2_API Eina_Bool ecore_wl2_input_pointer_xy_get(const Ecore_Wl2_Input *input, int *x, int *y);

/**
 * Set a given wl_surface to use as the pointer on a seat
 *
 * @param input The seat to set this surface as the pointer on
 * @param surface The surface to use as the pointer
 * @param hot_x The x coordinate to use as the cursor hot spot
 * @param hot_y The y coordinate to use as the cursor hot spot
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_input_pointer_set(Ecore_Wl2_Input *input, struct wl_surface *surface, int hot_x, int hot_y);

/**
 * Set a specific cursor on a given seat
 *
 * @brief This function will try to find a matching cursor inside the existing
 * cursor theme and set the pointer for the specified seat to be
 * the specified cursor
 *
 * @param input The seat to set the cursor on
 * @param cursor The name of the cursor to try and set
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.20
 */
ECORE_WL2_API void ecore_wl2_input_cursor_from_name_set(Ecore_Wl2_Input *input, const char *cursor);

/**
 * Gets default input of a given display
 *
 * @param ewd The display
 *
 * @ingroup Ecore_Wl2_Input_Group
 * @since 1.24
 */
ECORE_WL2_API Ecore_Wl2_Input *ecore_wl2_input_default_input_get(const Ecore_Wl2_Display *ewd);

/**
 * @defgroup Ecore_Wl2_Output_Group Wayland Library Output Functions
 * @ingroup Ecore_Wl2_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland Outputs
 */

/**
 * Return the DPI of a given output
 *
 * This is a simplistic call to get DPI. It does not account for differing
 * DPI in the x and y axes nor does it account for multihead or xinerama and
 * xrandr where different parts of the screen may have different DPI etc.
 *
 * @param output The output to get the DPI of
 *
 * @return the general screen DPI (dots/pixels per inch).
 *
 * @ingroup Ecore_Wl2_Output_Group
 * @since 1.17
 */
ECORE_WL2_API int ecore_wl2_output_dpi_get(Ecore_Wl2_Output *output);

/**
 * Get the current transform of a given output
 *
 * @param output The output to get the transform of
 *
 * @return The output's current transform value
 *
 * @ingroup Ecore_Wl2_Output_Group
 * @since 1.20
 */
ECORE_WL2_API int ecore_wl2_output_transform_get(Ecore_Wl2_Output *output);

/**
 * Return the version of the display's compositor object
 *
 * @param disp the display to get the compositor object version from
 *
 * @return the version of the display's compositor object
 *
 * @ingroup Ecore_Wl2_Display_Group
 * @since 1.17
 */
ECORE_WL2_API int ecore_wl2_display_compositor_version_get(Ecore_Wl2_Display *disp);

/**
 * Disable session recovery for any further connections.  Must be called
 * before connecting.  This is irreversible and not intended for general
 * use.
 *
 * @since 1.19
 */
ECORE_WL2_API void ecore_wl2_session_recovery_disable(void);

/**
 * Commit the surface of a wayland window.
 *
 * If flush is set this generates a wl_surface_commit(), otherwise it is
 * expected that some other call in the very near future (such as
 * eglSwapBuffers) will cause an implicit flush.
 *
 * A surface that has been commit will be in the "pending" state until
 * the compositor tells us it's time to draw again via a frame callback.
 *
 * @param window The window whose surface we want to commit
 * @param flush EINA_TRUE if we need to flush immediately.
 *
 * @since 1.21
 */
ECORE_WL2_API void ecore_wl2_window_commit(Ecore_Wl2_Window *window, Eina_Bool flush);

/**
 * Add a callback that fires when the window's surface_frame callback fires
 *
 * @param window The window to add a callback on
 * @param cb The callback
 * @param data User data to provide to the callback handler
 *
 * @since 1.21
 */
ECORE_WL2_API Ecore_Wl2_Frame_Cb_Handle *ecore_wl2_window_frame_callback_add(Ecore_Wl2_Window *window, Ecore_Wl2_Frame_Cb cb, void *data);

/**
 * Delete a callback that fires when the window's surface_frame callback fires
 *
 * @param handle The callback handle obtained through ecore_wl2_window_frame_callback_add().
 *
 * @since 1.21
 */
ECORE_WL2_API void ecore_wl2_window_frame_callback_del(Ecore_Wl2_Frame_Cb_Handle *handle);

/**
 * Push buffered wayland protocol to compositor
 *
 * Wayland protocol is only actually sent when a flush occurs,
 * so the display should be flushed at appropriate times, such
 * as after a commit.
 *
 * @param display The display to flush
 *
 * @since 1.21
 */
ECORE_WL2_API void ecore_wl2_display_flush(Ecore_Wl2_Display *display);

/**
 * Get if a given window is resizing
 *
 * @param window The window to check for resizing
 *
 * @return EINA_TRUE if resizing, EINA_FALSE otherwise
 *
 * @ingroup Ecore_Wl2_Window_Group
 * @since 1.21
 */
ECORE_WL2_API Eina_Bool ecore_wl2_window_resizing_get(Ecore_Wl2_Window *window);

/**
 * Latch window state at the start of an update
 *
 * When async render takes place we continue to dispatch wayland
 * events from the main loop. We need to defer any changes to
 * window state from those events until the update is complete.
 *
 * Events deferred during an update will automatically fire
 * immediately after the caller calls ecore_wl2_window_commit.
 *
 * @param window The window whose state we want to latch
 *
 * @since 1.21
 */
ECORE_WL2_API void ecore_wl2_window_update_begin(Ecore_Wl2_Window *window);

# endif

/* # ifdef __cplusplus */
/* } */
/* # endif */

#endif
