#ifndef _ECORE_SDL_H
#define _ECORE_SDL_H

#include <ecore_sdl_api.h>

/**
 * @file
 * @brief Ecore SDL system functions.
 */

#ifdef __cplusplus
extern "C" {
#endif

ECORE_SDL_API extern int ECORE_SDL_EVENT_GOT_FOCUS;
ECORE_SDL_API extern int ECORE_SDL_EVENT_LOST_FOCUS;
ECORE_SDL_API extern int ECORE_SDL_EVENT_RESIZE;
ECORE_SDL_API extern int ECORE_SDL_EVENT_EXPOSE;

typedef struct _Ecore_Sdl_Event_Video_Resize Ecore_Sdl_Event_Video_Resize;
struct _Ecore_Sdl_Event_Video_Resize
{
   unsigned int    windowID;
   int             w;
   int             h;
};

typedef struct _Ecore_Sdl_Event_Window Ecore_Sdl_Event_Window;
struct _Ecore_Sdl_Event_Window
{
   unsigned int    windowID;
};

ECORE_SDL_API int        ecore_sdl_init(const char *name);
ECORE_SDL_API int        ecore_sdl_shutdown(void);
ECORE_SDL_API void       ecore_sdl_feed_events(void);

  /* The following data structure have been deprecated since a long time */

typedef struct _Ecore_Sdl_Event_Key_Down Ecore_Sdl_Event_Key_Down;
struct _Ecore_Sdl_Event_Key_Down /** SDL Key Down event */
{
   const char      *keyname; /**< The name of the key that was pressed */
   const char      *keycompose; /**< The UTF-8 string conversion if any */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Key_Up Ecore_Sdl_Event_Key_Up;
struct _Ecore_Sdl_Event_Key_Up /** SDL Key Up event */
{
   const char      *keyname; /**< The name of the key that was released */
   const char      *keycompose; /**< The UTF-8 string conversion if any */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Mouse_Button_Down Ecore_Sdl_Event_Mouse_Button_Down;
struct _Ecore_Sdl_Event_Mouse_Button_Down /** SDL Mouse Down event */
{
   int             button; /**< Mouse button that was pressed (1 - 32) */
   int             x; /**< Mouse co-ordinates when mouse button was pressed */
   int             y; /**< Mouse co-ordinates when mouse button was pressed */
   int             double_click : 1; /**< Set if click was a double click */
   int             triple_click : 1; /**< Set if click was a triple click  */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Mouse_Button_Up Ecore_Sdl_Event_Mouse_Button_Up;
struct _Ecore_Sdl_Event_Mouse_Button_Up /** SDL Mouse Up event */
{
   int             button; /**< Mouse button that was released (1 - 32) */
   int             x; /**< Mouse co-ordinates when mouse button was raised */
   int             y; /**< Mouse co-ordinates when mouse button was raised */
   int             double_click : 1; /**< Set if click was a double click */
   int             triple_click : 1; /**< Set if click was a triple click  */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Mouse_Move Ecore_Sdl_Event_Mouse_Move;
struct _Ecore_Sdl_Event_Mouse_Move /** SDL Mouse Move event */
{
   int             x; /**< Mouse co-ordinates where the mouse cursor moved to */
   int             y; /**< Mouse co-ordinates where the mouse cursor moved to */
   unsigned int    time;
};

typedef struct _Ecore_Sdl_Event_Mouse_Wheel Ecore_Sdl_Event_Mouse_Wheel;
struct _Ecore_Sdl_Event_Mouse_Wheel /** SDL Mouse Wheel event */
{
   int             x,y;
   int             direction; /* 0 = vertical, 1 = horizontal */
   int             wheel; /* value 1 (left/up), -1 (right/down) */
   unsigned int    time;
};

#ifdef __cplusplus
}
#endif

#endif
