/*
 * Various MWM related functions.
 *
 * This is ALL the code involving anything MWM related. for both WM and
 * client.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* ifdef HAVE_CONFIG_H */

#include <stdlib.h>

#include "Ecore.h"
#include "ecore_x_private.h"
#include "Ecore_X.h"
#include "Ecore_X_Atoms.h"

#define ECORE_X_MWM_HINTS_FUNCTIONS   (1 << 0)
#define ECORE_X_MWM_HINTS_DECORATIONS (1 << 1)
#define ECORE_X_MWM_HINTS_INPUT_MODE  (1 << 2)
#define ECORE_X_MWM_HINTS_STATUS      (1 << 3)

typedef struct _mwmhints
{
   CARD32 flags;
   CARD32 functions;
   CARD32 decorations;
   INT32  inputmode;
   CARD32 status;
}
MWMHints;

ECORE_X_API Eina_Bool
ecore_x_mwm_hints_get(Ecore_X_Window win,
                      Ecore_X_MWM_Hint_Func *fhint,
                      Ecore_X_MWM_Hint_Decor *dhint,
                      Ecore_X_MWM_Hint_Input *ihint)
{
   unsigned char *p = NULL;
   MWMHints *mwmhints = NULL;
   int num;
   Eina_Bool ret;

   LOGFN;
   ret = EINA_FALSE;
   if (!ecore_x_window_prop_property_get(win,
                                         ECORE_X_ATOM_MOTIF_WM_HINTS,
                                         ECORE_X_ATOM_MOTIF_WM_HINTS,
                                         32, &p, &num))
     {
        if (p) free(p);
        return EINA_FALSE;
     }

   mwmhints = (MWMHints *)p;
   if (mwmhints)
     {
        if (num >= 4)
          {
             if (dhint)
               {
                  if (mwmhints->flags & ECORE_X_MWM_HINTS_DECORATIONS)
                    *dhint = mwmhints->decorations;
                  else
                    *dhint = ECORE_X_MWM_HINT_DECOR_ALL;
               }

             if (fhint)
               {
                  if (mwmhints->flags & ECORE_X_MWM_HINTS_FUNCTIONS)
                    *fhint = mwmhints->functions;
                  else
                    *fhint = ECORE_X_MWM_HINT_FUNC_ALL;
               }

             if (ihint)
               {
                  if (mwmhints->flags & ECORE_X_MWM_HINTS_INPUT_MODE)
                    *ihint = mwmhints->inputmode;
                  else
                    *ihint = ECORE_X_MWM_HINT_INPUT_MODELESS;
               }

             ret = EINA_TRUE;
          }

        free(mwmhints);
     }

   return ret;
}

ECORE_X_API void
ecore_x_mwm_borderless_set(Ecore_X_Window win,
                           Eina_Bool borderless)
{
   unsigned int data[5] = {0, 0, 0, 0, 0};

   data[0] = 2; /* just set the decorations hint! */
   data[2] = !borderless;

   LOGFN;
   ecore_x_window_prop_property_set(win,
                                    ECORE_X_ATOM_MOTIF_WM_HINTS,
                                    ECORE_X_ATOM_MOTIF_WM_HINTS,
                                    32, (void *)data, 5);
}

