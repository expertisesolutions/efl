#ifndef _ECORE_X_ATOMS_H
#define _ECORE_X_ATOMS_H

/**
 * @file
 * @brief Ecore X atoms
 */

/* generic atoms */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_ATOM;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_CARDINAL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_INTEGER; /**< @since 1.24 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_FLOAT; /**< @since 1.24 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_COMPOUND_TEXT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_FILE_NAME;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_STRING;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_TEXT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_UTF8_STRING;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_X_MOZ_URL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WINDOW;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_PIXMAP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_VISUALID;

/* dnd atoms */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_XDND;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_XDND;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_AWARE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_ENTER;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_TYPE_LIST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_POSITION;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_COPY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_MOVE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_PRIVATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_ASK;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_LIST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_LINK;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_ACTION_DESCRIPTION;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_PROXY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_STATUS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_LEAVE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_DROP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_FINISHED;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_XDND_DIRECTSAVE0; /**< @since 1.8 */

/* dnd atoms that need to be exposed to the application interface */
ECORE_X_API extern Ecore_X_Atom ECORE_X_DND_ACTION_COPY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_DND_ACTION_MOVE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_DND_ACTION_LINK;
ECORE_X_API extern Ecore_X_Atom ECORE_X_DND_ACTION_ASK;
ECORE_X_API extern Ecore_X_Atom ECORE_X_DND_ACTION_PRIVATE;

/* old E atom */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_FRAME_SIZE;

/* old Gnome atom */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WIN_LAYER;

/* ICCCM: client properties */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_NAME;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_ICON_NAME;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_NORMAL_HINTS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_SIZE_HINTS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_HINTS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_CLASS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_TRANSIENT_FOR;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_PROTOCOLS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_COLORMAP_WINDOWS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_COMMAND;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_CLIENT_MACHINE;

/* ICCCM: window manager properties */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_STATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_ICON_SIZE;

/* ICCCM: WM_STATEproperty */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_CHANGE_STATE;

/* ICCCM: WM_PROTOCOLS properties */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_TAKE_FOCUS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_SAVE_YOURSELF;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_DELETE_WINDOW;

/* ICCCM: WM_COLORMAP properties */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_COLORMAP_NOTIFY;

/* ICCCM: session management properties */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SM_CLIENT_ID;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_CLIENT_LEADER;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_WM_WINDOW_ROLE;

/* Motif WM atom */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_MOTIF_WM_HINTS;

ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_SUPPORTED;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_CLIENT_LIST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_CLIENT_LIST_STACKING;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_NUMBER_OF_DESKTOPS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_GEOMETRY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_VIEWPORT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_CURRENT_DESKTOP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_NAMES;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_ACTIVE_WINDOW;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WORKAREA;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_SUPPORTING_WM_CHECK;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_VIRTUAL_ROOTS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_DESKTOP_LAYOUT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_SHOWING_DESKTOP;

/* pager */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_CLOSE_WINDOW;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_MOVERESIZE_WINDOW;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_MOVERESIZE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_RESTACK_WINDOW;

ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_REQUEST_FRAME_EXTENTS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_NAME;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_VISIBLE_NAME;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ICON_NAME;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_VISIBLE_ICON_NAME;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_DESKTOP;

/* window type */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DESKTOP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DOCK;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_TOOLBAR;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_MENU;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_UTILITY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_SPLASH;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DIALOG;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_NORMAL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DROPDOWN_MENU;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_POPUP_MENU;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_TOOLTIP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_NOTIFICATION;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_COMBO;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_TYPE_DND;

/* state */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_MODAL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_STICKY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_MAXIMIZED_VERT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_MAXIMIZED_HORZ;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_SHADED;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_SKIP_TASKBAR;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_SKIP_PAGER;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_HIDDEN;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_FULLSCREEN;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_ABOVE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_BELOW;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STATE_DEMANDS_ATTENTION;

/* allowed actions */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ALLOWED_ACTIONS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MOVE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_RESIZE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MINIMIZE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_SHADE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_STICK;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MAXIMIZE_HORZ;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_MAXIMIZE_VERT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_FULLSCREEN;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_CHANGE_DESKTOP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_CLOSE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_ABOVE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ACTION_BELOW;

ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STRUT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_STRUT_PARTIAL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ICON_GEOMETRY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_ICON;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_PID;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_HANDLED_ICONS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_USER_TIME;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_STARTUP_ID;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_FRAME_EXTENTS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_PING;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_SYNC_REQUEST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_SYNC_REQUEST_COUNTER;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_OPACITY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_SHADOW;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_WM_WINDOW_SHADE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_STARTUP_INFO_BEGIN;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_NET_STARTUP_INFO;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_TARGETS;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PRIMARY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_SECONDARY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_CLIPBOARD;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_PRIMARY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_SECONDARY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SELECTION_PROP_CLIPBOARD;

/* currently E specific virtual keyboard extension, aim to submit to netwm spec
 * later */

ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_STATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ON;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_OFF;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_ALPHA;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_NUMERIC;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PIN;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PHONE_NUMBER;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_HEX;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_TERMINAL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_PASSWORD;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_IP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_HOST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_FILE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_URL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_KEYPAD;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIRTUAL_KEYBOARD_J2ME;

/* Illume specific atoms */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ZONE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ZONE_LIST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CONFORMANT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE_SINGLE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE_DUAL_TOP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_MODE_DUAL_LEFT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_FOCUS_BACK;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_FOCUS_FORWARD;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_FOCUS_HOME;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_HOME_NEW;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_HOME_DEL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLOSE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG_LOCKED;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG_START;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_DRAG_END;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_GEOMETRY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_SOFTKEY_GEOMETRY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_KEYBOARD_GEOMETRY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_STATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_STATE_TOGGLE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_ON;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_OFF;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_PRIORITY_MAJOR;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_PRIORITY_MINOR;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_ZONE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_QUICKPANEL_POSITION_UPDATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_STATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_ON;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_OFF;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_OPACITY_MODE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_OPAQUE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TRANSLUCENT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TRANSPARENT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TYPE_MODE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TYPE_1;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TYPE_2;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_INDICATOR_TYPE_3;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ROTATE_WINDOW_AVAILABLE_ANGLE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ROTATE_WINDOW_ANGLE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ROTATE_ROOT_ANGLE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_STATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_ON;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_OFF;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_CLIPBOARD_GEOMETRY;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_WINDOW_STATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_WINDOW_STATE_NORMAL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_WINDOW_STATE_FLOATING;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_CONTROL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_NEXT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_PREV;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_ACTIVATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_OVER;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ_NEXT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_READ_PREV;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_UP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_DOWN;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_BACK;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_SCROLL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_MOUSE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_ENABLE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ACTION_DISABLE;

/* Abi compat fix */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_ENABLE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_ACCESS_DISABLE;
/* End of Abi compat fix */

ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_COUNTER;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_DRAW_DONE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_SUPPORTED;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_BEGIN;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_END;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_SYNC_CANCEL;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_FLUSH;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_DUMP;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_COMP_PIXMAP;

ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIDEO_PARENT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_VIDEO_POSITION;

/* currently elementary and E specific extension */
/* @deprecated use ECORE_X_ATOM_E_WINDOW_PROFILE */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_PROFILE;
/* @deprecated use ECORE_X_ATOM_E_WINDOW_PROFILE_AVAILABLE_LIST */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_PROFILE_LIST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_SUPPORTED;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_CHANGE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_AVAILABLE_LIST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_CHANGE_REQUEST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_PROFILE_CHANGE_DONE;

/* for sliding window */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_SLIDING_WIN_STATE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_ILLUME_SLIDING_WIN_GEOMETRY;

/* for window stacks */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_STACK_TYPE; /**< @since 1.19 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_STACK_POSITION; /**< @since 1.19 */

/* for SDB(Samsung Debug Bridge) */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SDB_SERVER_CONNECT;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_SDB_SERVER_DISCONNECT;

/* for deiconify approve protcol */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_DEICONIFY_APPROVE;

/* E window rotation extension */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_SUPPORTED;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_APP_SUPPORTED;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_AVAILABLE_LIST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_PREFERRED_ROTATION;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_CHANGE_PREPARE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_CHANGE_PREPARE_DONE;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_CHANGE_REQUEST;
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_ROTATION_CHANGE_DONE;

/* E window auxiliary hint */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_AUX_HINT_SUPPORTED_LIST; /**< @since 1.10 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_AUX_HINT_SUPPORT; /**< @since 1.10 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_AUX_HINT; /**< @since 1.10 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_WINDOW_AUX_HINT_ALLOWED; /**< @since 1.10 */

/* E keyrouter protocol */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_KEYROUTER_SUPPORTED; /**< @since 1.15 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_E_KEYROUTER_WINDOW_KEYTABLE; /**< @since 1.15 */

/* Teamwork protocol */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_TEAMWORK_PROPERTY; /**< @since 1.18 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_TEAMWORK_PRELOAD; /**< @since 1.18 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_TEAMWORK_ACTIVATE; /**< @since 1.18 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_TEAMWORK_DEACTIVATE; /**< @since 1.18 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_TEAMWORK_OPEN; /**< @since 1.18 */

ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_TEAMWORK_COMPLETED; /**< @since 1.18 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_TEAMWORK_PROGRESS; /**< @since 1.18 */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_TEAMWORK_STARTED; /**< @since 1.18 */

/* Client-Side Decorations */
ECORE_X_API extern Ecore_X_Atom ECORE_X_ATOM_GTK_FRAME_EXTENTS; /**< @since 1.19 */
#endif /* _ECORE_X_ATOMS_H */
