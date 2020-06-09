#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* ifdef HAVE_CONFIG_H */

#include <stdlib.h>

#ifdef ECORE_XTEST
# include <X11/extensions/XTest.h>
#endif /* ifdef ECORE_XTEST */

#include "ecore_x_private.h"
#include "Ecore_X.h"
#include <string.h>

EAPI Eina_Bool
#ifdef ECORE_XTEST
ecore_x_test_fake_key_down(const char *key)
#else
ecore_x_test_fake_key_down(const char *key EINA_UNUSED)
#endif
{
#ifdef ECORE_XTEST
   KeyCode keycode = 0;
   KeySym keysym;

   LOGFN;
   if (!strncmp(key, "Keycode-", 8))
     keycode = atoi(key + 8);
   else
     {
        keysym = XStringToKeysym(key);
        if (keysym == NoSymbol)
          return EINA_FALSE;

        keycode = XKeysymToKeycode(_ecore_x_disp, keysym);
     }

   if (keycode == 0)
     return EINA_FALSE;

   return XTestFakeKeyEvent(_ecore_x_disp, keycode, 1, 0) ? EINA_TRUE : EINA_FALSE;
#else /* ifdef ECORE_XTEST */
   return EINA_FALSE;
#endif /* ifdef ECORE_XTEST */
}

EAPI Eina_Bool
#ifdef ECORE_XTEST
ecore_x_test_fake_key_up(const char *key)
#else
ecore_x_test_fake_key_up(const char *key EINA_UNUSED)
#endif
{
#ifdef ECORE_XTEST
   KeyCode keycode = 0;
   KeySym keysym;

   LOGFN;
   if (!strncmp(key, "Keycode-", 8))
     keycode = atoi(key + 8);
   else
     {
        keysym = XStringToKeysym(key);
        if (keysym == NoSymbol)
          return EINA_FALSE;

        keycode = XKeysymToKeycode(_ecore_x_disp, keysym);
     }

   if (keycode == 0)
     return EINA_FALSE;

   return XTestFakeKeyEvent(_ecore_x_disp, keycode, 0, 0) ? EINA_TRUE : EINA_FALSE;
#else /* ifdef ECORE_XTEST */
   return EINA_FALSE;
#endif /* ifdef ECORE_XTEST */
}

EAPI Eina_Bool
#ifdef ECORE_XTEST
ecore_x_test_fake_key_press(const char *key)
#else
ecore_x_test_fake_key_press(const char *key EINA_UNUSED)
#endif
{
#ifdef ECORE_XTEST
   KeyCode keycode = 0;
   KeySym keysym = 0;
   int shift = 0;

   LOGFN;
   if (!strncmp(key, "Keycode-", 8))
     keycode = atoi(key + 8);
   else
     {
        keysym = XStringToKeysym(key);
        if (keysym == NoSymbol)
          return EINA_FALSE;

        keycode = XKeysymToKeycode(_ecore_x_disp, keysym);
        if (_ecore_x_XKeycodeToKeysym(_ecore_x_disp, keycode, 0) != keysym)
          {
             if (_ecore_x_XKeycodeToKeysym(_ecore_x_disp, keycode, 1) == keysym)
               shift = 1;
             else
               keycode = 0;
          }
        else
          shift = 0;
     }

   if (keycode == 0)
     {
        static int mod = 0;
        KeySym *keysyms;
        int keycode_min, keycode_max, keycode_num;
        int i;

        XDisplayKeycodes(_ecore_x_disp, &keycode_min, &keycode_max);
        keysyms = XGetKeyboardMapping(_ecore_x_disp, keycode_min,
                                      keycode_max - keycode_min + 1,
                                      &keycode_num);
        mod = (mod + 1) & 0x7;
        i = (keycode_max - keycode_min - mod - 1) * keycode_num;

        keysyms[i] = keysym;
        XChangeKeyboardMapping(_ecore_x_disp, keycode_min, keycode_num,
                               keysyms, (keycode_max - keycode_min));
        XFree(keysyms);
        XSync(_ecore_x_disp, False);
        keycode = keycode_max - mod - 1;
     }

   if (shift)
     XTestFakeKeyEvent(_ecore_x_disp,
                       XKeysymToKeycode(_ecore_x_disp, XK_Shift_L), 1, 0);

   XTestFakeKeyEvent(_ecore_x_disp, keycode, 1, 0);
   XTestFakeKeyEvent(_ecore_x_disp, keycode, 0, 0);
   if (shift)
     XTestFakeKeyEvent(_ecore_x_disp,
                       XKeysymToKeycode(_ecore_x_disp, XK_Shift_L), 0, 0);

   return EINA_TRUE;
#else /* ifdef ECORE_XTEST */
   return EINA_FALSE;
#endif /* ifdef ECORE_XTEST */
}

EAPI const char *
ecore_x_keysym_string_get(int keysym)
{
   return XKeysymToString(keysym);
}

EAPI int 
ecore_x_keysym_keycode_get(const char *keyname)
{
   int keycode = 0;

   if (!strncmp(keyname, "Keycode-", 8))
     keycode = atoi(keyname + 8);
   else
     keycode = XKeysymToKeycode(_ecore_x_disp, XStringToKeysym(keyname));

   return keycode;
}

EAPI unsigned int
ecore_x_keysym_get(const char *string)
{
   return XStringToKeysym(string);
}
