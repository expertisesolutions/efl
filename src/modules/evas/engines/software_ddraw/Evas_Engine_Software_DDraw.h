#ifndef __EVAS_ENGINE_SOFTWARE_DDRAW_H__
#define __EVAS_ENGINE_SOFTWARE_DDRAW_H__

#ifdef _WIN32
# include <evil_private.h>
#endif

typedef struct _Evas_Engine_Info_Software_DDraw Evas_Engine_Info_Software_DDraw;

struct _Evas_Engine_Info_Software_DDraw
{
   /* PRIVATE - don't mess with this baby or evas will poke its tongue out */
   /* at you and make nasty noises */
   Evas_Engine_Info magic;

   struct {
      HWND         window;
      int          rotation;
      unsigned int fullscreen : 1;
   } info;

   /* non-blocking or blocking mode */
   Evas_Engine_Render_Mode render_mode;
};


#endif /* __EVAS_ENGINE_SOFTWARE_DDRAW_H__ */
