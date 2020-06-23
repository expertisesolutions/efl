#ifndef _EVAS_DRAW_H
#define _EVAS_DRAW_H


EVAS_API void               evas_common_draw_init                            (void);

EVAS_API RGBA_Draw_Context *evas_common_draw_context_new                     (void);
EVAS_API RGBA_Draw_Context *evas_common_draw_context_dup                     (RGBA_Draw_Context *dc);
EVAS_API void               evas_common_draw_context_free                    (RGBA_Draw_Context *dc);
EVAS_API void               evas_common_draw_context_font_ext_set            (RGBA_Draw_Context *dc,
                                                                          void *data,
                                                                          void *(*gl_new)  (void *data, RGBA_Font_Glyph *fg),
                                                                          void  (*gl_free) (void *ext_dat),
                                                                          void  (*gl_draw) (void *data, void *dest, void *context, RGBA_Font_Glyph *fg, int x, int y, int w, int h),
                                                                          void *(*gl_image_new) (void *gc, RGBA_Font_Glyph *fg, int alpha, Evas_Colorspace cspace),
                                                                          void  (*gl_image_free) (void *image),
                                                                          void  (*gl_image_draw) (void *gc, void *im, int dx, int dy, int dw, int dh, int smooth));
EVAS_API void               evas_common_draw_context_clip_clip               (RGBA_Draw_Context *dc, int x, int y, int w, int h);
EVAS_API void               evas_common_draw_context_set_clip                (RGBA_Draw_Context *dc, int x, int y, int w, int h);
EVAS_API void               evas_common_draw_context_unset_clip              (RGBA_Draw_Context *dc);
EVAS_API void               evas_common_draw_context_set_color               (RGBA_Draw_Context *dc, int r, int g, int b, int a);
EVAS_API void               evas_common_draw_context_set_multiplier          (RGBA_Draw_Context *dc, int r, int g, int b, int a);
EVAS_API void               evas_common_draw_context_unset_multiplier        (RGBA_Draw_Context *dc);
EVAS_API Cutout_Rects      *evas_common_draw_context_cutouts_new             (void);
EVAS_API void               evas_common_draw_context_cutouts_free            (Cutout_Rects* rects);
EVAS_API void               evas_common_draw_context_cutouts_real_free       (Cutout_Rects* rects);
EVAS_API void               evas_common_draw_context_cutouts_del             (Cutout_Rects* rects, int idx);
EVAS_API void               evas_common_draw_context_add_cutout              (RGBA_Draw_Context *dc, int x, int y, int w, int h);
EVAS_API void               evas_common_draw_context_clear_cutouts           (RGBA_Draw_Context *dc);
EVAS_API Cutout_Rects      *evas_common_draw_context_apply_cutouts           (RGBA_Draw_Context *dc, Cutout_Rects *recycle);
EVAS_API void               evas_common_draw_context_apply_clear_cutouts     (Cutout_Rects* rects);
EVAS_API void               evas_common_draw_context_apply_clean_cutouts     (Cutout_Rects* rects);
EVAS_API void               evas_common_draw_context_set_anti_alias          (RGBA_Draw_Context *dc, unsigned char aa);
EVAS_API void               evas_common_draw_context_set_color_interpolation (RGBA_Draw_Context *dc, int color_space);
EVAS_API void               evas_common_draw_context_set_render_op           (RGBA_Draw_Context *dc, int op);
EVAS_API void               evas_common_draw_context_target_set              (RGBA_Draw_Context *dc, int x, int y, int w, int h);

#endif /* _EVAS_DRAW_H */
