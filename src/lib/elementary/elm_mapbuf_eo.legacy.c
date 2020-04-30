
void
elm_mapbuf_auto_set(Elm_Mapbuf *obj, Eina_Bool on)
{
   elm_private_mapbuf_auto_set(obj, on);
}

Eina_Bool
elm_mapbuf_auto_get(const Elm_Mapbuf *obj)
{
   return elm_private_mapbuf_auto_get(obj);
}

void
elm_mapbuf_smooth_set(Elm_Mapbuf *obj, Eina_Bool smooth)
{
   elm_private_mapbuf_smooth_set(obj, smooth);
}

Eina_Bool
elm_mapbuf_smooth_get(const Elm_Mapbuf *obj)
{
   return elm_private_mapbuf_smooth_get(obj);
}

void
elm_mapbuf_alpha_set(Elm_Mapbuf *obj, Eina_Bool alpha)
{
   elm_private_mapbuf_alpha_set(obj, alpha);
}

Eina_Bool
elm_mapbuf_alpha_get(const Elm_Mapbuf *obj)
{
   return elm_private_mapbuf_alpha_get(obj);
}

void
elm_mapbuf_enabled_set(Elm_Mapbuf *obj, Eina_Bool enabled)
{
   elm_private_mapbuf_enabled_set(obj, enabled);
}

Eina_Bool
elm_mapbuf_enabled_get(const Elm_Mapbuf *obj)
{
   return elm_private_mapbuf_enabled_get(obj);
}

void
elm_mapbuf_point_color_set(Elm_Mapbuf *obj, int idx, int r, int g, int b, int a)
{
   elm_private_mapbuf_point_color_set(obj, idx, r, g, b, a);
}

void
elm_mapbuf_point_color_get(const Elm_Mapbuf *obj, int idx, int *r, int *g, int *b, int *a)
{
   elm_private_mapbuf_point_color_get(obj, idx, r, g, b, a);
}
