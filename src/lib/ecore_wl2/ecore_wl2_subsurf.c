#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ecore_wl2_private.h"

void
_ecore_wl2_subsurf_unmap(Ecore_Wl2_Subsurface *subsurf)
{
   if (subsurf->wl.subsurface) wl_subsurface_destroy(subsurf->wl.subsurface);
   if (subsurf->wl.surface) wl_surface_destroy(subsurf->wl.surface);
   subsurf->wl.subsurface = NULL;
   subsurf->wl.surface = NULL;
}

void
_ecore_wl2_subsurf_free(Ecore_Wl2_Subsurface *subsurf)
{
   Ecore_Wl2_Window *parent;

   _ecore_wl2_subsurf_unmap(subsurf);

   parent = subsurf->parent;
   if (parent)
     {
        parent->subsurfs =
          eina_inlist_remove(parent->subsurfs, EINA_INLIST_GET(subsurf));
     }

   free(subsurf);
}

Ecore_Wl2_Subsurface *
ecore_wl2_subsurface_new(Ecore_Wl2_Window *window)
{
   Ecore_Wl2_Display *display;
   Ecore_Wl2_Subsurface *subsurf;

   EINA_SAFETY_ON_NULL_RETURN_VAL(window, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(window->surface, NULL);

   display = window->display;

   EINA_SAFETY_ON_NULL_RETURN_VAL(display->wl.compositor, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(display->wl.subcompositor, NULL);

   subsurf = calloc(1, sizeof(Ecore_Wl2_Subsurface));
   if (!subsurf) return NULL;

   subsurf->parent = window;

   subsurf->wl.surface = wl_compositor_create_surface(display->wl.compositor);
   if (!subsurf->wl.surface)
     {
        ERR("Failed to create surface");
        goto surf_err;
     }

   subsurf->wl.subsurface =
     wl_subcompositor_get_subsurface(display->wl.subcompositor,
                                     subsurf->wl.surface, window->surface);
   if (!subsurf->wl.subsurface)
     {
        ERR("Could not create subsurface");
        goto sub_surf_err;
     }

   window->subsurfs =
     eina_inlist_append(window->subsurfs, EINA_INLIST_GET(subsurf));

   return subsurf;

sub_surf_err:
   wl_surface_destroy(subsurf->wl.surface);

surf_err:
   free(subsurf);
   return NULL;
}

void
ecore_wl2_subsurface_del(Ecore_Wl2_Subsurface *subsurface)
{
   EINA_SAFETY_ON_NULL_RETURN(subsurface);

   _ecore_wl2_subsurf_free(subsurface);
}

struct wl_surface *
ecore_wl2_subsurface_surface_get(Ecore_Wl2_Subsurface *subsurface)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(subsurface, NULL);

   return subsurface->wl.surface;
}

void
ecore_wl2_subsurface_position_set(Ecore_Wl2_Subsurface *subsurface, int x, int y)
{
   EINA_SAFETY_ON_NULL_RETURN(subsurface);
   EINA_SAFETY_ON_NULL_RETURN(subsurface->wl.subsurface);

   if ((subsurface->x == x) && (subsurface->y == y)) return;

   subsurface->x = x;
   subsurface->y = y;

   wl_subsurface_set_position(subsurface->wl.subsurface, x, y);
}

void
ecore_wl2_subsurface_position_get(Ecore_Wl2_Subsurface *subsurface, int *x, int *y)
{
   EINA_SAFETY_ON_NULL_RETURN(subsurface);

   if (x) *x = subsurface->x;
   if (y) *y = subsurface->y;
}

void
ecore_wl2_subsurface_place_above(Ecore_Wl2_Subsurface *subsurface, struct wl_surface *surface)
{
   EINA_SAFETY_ON_NULL_RETURN(subsurface);
   EINA_SAFETY_ON_NULL_RETURN(surface);

   wl_subsurface_place_above(subsurface->wl.subsurface, surface);
}

void
ecore_wl2_subsurface_place_below(Ecore_Wl2_Subsurface *subsurface, struct wl_surface *surface)
{
   EINA_SAFETY_ON_NULL_RETURN(subsurface);
   EINA_SAFETY_ON_NULL_RETURN(surface);

   wl_subsurface_place_below(subsurface->wl.subsurface, surface);
}

void
ecore_wl2_subsurface_sync_set(Ecore_Wl2_Subsurface *subsurface, Eina_Bool sync)
{
   EINA_SAFETY_ON_NULL_RETURN(subsurface);
   EINA_SAFETY_ON_NULL_RETURN(subsurface->wl.subsurface);

   sync = !!sync;
   if (subsurface->sync == sync) return;

   subsurface->sync = sync;

   if (subsurface->sync)
     wl_subsurface_set_sync(subsurface->wl.subsurface);
   else
     wl_subsurface_set_desync(subsurface->wl.subsurface);
}

void
ecore_wl2_subsurface_opaque_region_set(Ecore_Wl2_Subsurface *subsurface, int x, int y, int w, int h)
{
   EINA_SAFETY_ON_NULL_RETURN(subsurface);
   EINA_SAFETY_ON_NULL_RETURN(subsurface->wl.subsurface);

   if ((w > 0) && (h > 0))
     {
        Ecore_Wl2_Window *parent;

        parent = subsurface->parent;
        if (parent)
          {
             struct wl_region *region;

             region =
               wl_compositor_create_region(parent->display->wl.compositor);
             if (!region)
               {
                  ERR("Failed to create opaque region");
                  return;
               }

             wl_region_add(region, x, y, w, h);
             wl_surface_set_opaque_region(subsurface->wl.surface, region);
             wl_region_destroy(region);
          }
     }
   else
     wl_surface_set_opaque_region(subsurface->wl.surface, NULL);
}
