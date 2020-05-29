#ifndef STUB_ECORE_TYPES_H
#define STUB_ECORE_TYPES_H

// We shoud try to separete ecore and ecore_input

/*          ECORE TYPES         */
typedef void* Ecore_Window;
typedef struct _root_loco
{
  void* z;
  void* x;
  void* y;
} root_loco;

typedef struct _Ecore_Event_Mouse_Wheel
{
  Ecore_Window window;
  void* event_window;
  void* direction;
  void* z;
  void* x;
  void* y;
  void* timestamp;
  root_loco root;
  void* modifiers;
}Ecore_Event_Mouse_Wheel;

typedef struct _Ecore_Event_Mouse_Button
{
  Ecore_Window window;
  void* event_window;
  void* direction;
  void* z;
  void* x;
  void* y;
  void* timestamp;
  root_loco root;
  void* modifiers;
  void* buttons;
  void* double_click;
  void* triple_click;
}Ecore_Event_Mouse_Button;


typedef struct _Ecore_Event_Mouse_Move
{
  Ecore_Window window;
  void* event_window;
  void* direction;
  void* z;
  void* x;
  void* y;
  void* timestamp;
  root_loco root;
  void* modifiers;
}Ecore_Event_Mouse_Move;

typedef struct _Ecore_Event_Key
{
  char* keyname;
  char* key;
  char* compose;
  void* root_window;
  void* string;
  void* event_window;
  void* timestamp;
  void* modifiers;
  int   same_screen;
  Ecore_Window window;
} Ecore_Event_Key;

#define ECORE_EVENT_MODIFIER_SHIFT 0
#define ECORE_EVENT_MODIFIER_CTRL 0
#define ECORE_EVENT_MODIFIER_ALT 0
#define ECORE_EVENT_MODIFIER_ALTGR 0
#define ECORE_EVENT_MODIFIER_WIN 0
#define ECORE_EVENT_LOCK_SCROLL 0
#define ECORE_EVENT_LOCK_NUM 0
#define ECORE_EVENT_LOCK_CAPS 0
#define ECORE_EVENT_KEY_DOWN 0
#define ECORE_EVENT_KEY_UP 0
#define ECORE_EVENT_MOUSE_WHEEL 0
#define ECORE_EVENT_MOUSE_MOVE 0
#define ECORE_EVENT_MOUSE_BUTTON_DOWN 0
#define ECORE_EVENT_MOUSE_BUTTON_UP 0

/*          ECORE_INPUT TYPES         */
#endif
