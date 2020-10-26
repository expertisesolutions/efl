#ifndef _EVAS_CONVERT_COLORSPACE_H
#define _EVAS_CONVERT_COLORSPACE_H


EVAS_API void *evas_common_convert_argb8888_to    (void *data, int w, int h, int stride, Eina_Bool has_alpha, Evas_Colorspace cspace);
EVAS_API void *evas_common_convert_rgb565_a5p_to  (void *data, int w, int h, int stride, Eina_Bool has_alpha, Evas_Colorspace cspace);
EVAS_API void *evas_common_convert_yuv_422P_601_to(void *data, int w, int h, Evas_Colorspace cspace);
EVAS_API void *evas_common_convert_yuv_422_601_to (void *data, int w, int h, Evas_Colorspace cspace);
EVAS_API void *evas_common_convert_yuv_420_601_to (void *data, int w, int h, Evas_Colorspace cspace);
EVAS_API void *evas_common_convert_yuv_420T_601_to(void *data, int w, int h, Evas_Colorspace cspace);
void *evas_common_convert_agry88_to(const void *data, int w, int h, int stride, Eina_Bool has_alpha, Evas_Colorspace cspace);
void *evas_common_convert_gry8_to(const void *data, int w, int h, int stride, Eina_Bool has_alpha, Evas_Colorspace cspace);

#endif /* _EVAS_CONVERT_COLORSPACE_H */
