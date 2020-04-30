#include "evas_common_private.h"
#include "evas_convert_color.h"
#include "draw.h"

DATA32
evas_common_convert_ag_premul(DATA16 *data, unsigned int len)
{
   DATA16 *de = data + len;
   DATA32 nas = 0;

   while (data < de)
     {
        DATA16  a = 1 + ((*data >> 8) & 0xff);

        *data = (*data & 0xff00) |
          ((((*data & 0xff) * a) >> 8) & 0xff);
        data++;

        if ((a == 1) || (a == 256))
          nas++;
     }

   return nas;
}

void
evas_common_convert_ag_unpremul(DATA16 *data, unsigned int len)
{
   DATA16 *de = data + len;
   DATA16 p_val = 0x0000, p_res = 0x0000;

   while (data < de)
     {
        if (p_val == *data) *data = p_res;
        else
          {
             DATA16 a = (*data >> 8);

             p_val = *data;
             if ((a > 0) && (a < 255))
               {
                  *data = ((a << 8) | (((*data & 0xff) * 0xff) / a));
               }
             else if (a == 0)
               {
                  *data = 0x0000;
               }
             p_res = *data;
          }
        data++;
     }
}

DATA32
evas_common_convert_argb_premul(DATA32 *data, unsigned int len)
{
   return (DATA32) efl_draw_argb_premul(data, len);
}

void
evas_common_convert_argb_unpremul(DATA32 *data, unsigned int len)
{
   return efl_draw_argb_unpremul(data, len);
}

void
evas_common_convert_color_argb_premul(int a, int *r, int *g, int *b)
{
   a++;
   if (r) { *r = (a * *r) >> 8; }
   if (g) { *g = (a * *g) >> 8; }
   if (b) { *b = (a * *b) >> 8; }
}

void
evas_common_convert_color_argb_unpremul(int a, int *r, int *g, int *b)
{
   if (!a) return;
   if (r) { *r = (255 * *r) / a; }
   if (g) { *g = (255 * *g) / a; }
   if (b) { *b = (255 * *b) / a; }
}

void
evas_common_convert_color_hsv_to_rgb(float h, float s, float v, int *r, int *g, int *b)
{
   int i;
   float f;

   v *= 255;
   if (EINA_FLT_EQ(s, 0.0))
     {
       if (r) *r = v;
       if (g) *g = v;
       if (b) *b = v;
       return;
     }

   h /= 60;
   i = h;
   f = h - i;

   s *= v;
   f *= s;
   s = v - s;

   switch (i)
     {
       case 1:
         if (r) *r = v - f;
         if (g) *g = v;
         if (b) *b = s;
         return;
       case 2:
         if (r) *r = s;
         if (g) *g = v;
         if (b) *b = s + f;
         return;
       case 3:
         if (r) *r = s;
         if (g) *g = v - f;
         if (b) *b = v;
         return;
       case 4:
         if (r) *r = s + f;
         if (g) *g = s;
         if (b) *b = v;
         return;
       case 5:
         if (r) *r = v;
         if (g) *g = s;
         if (b) *b = v - f;
         return;
       default:
         if (r) *r = v;
         if (g) *g = s + f;
         if (b) *b = s;
         break;
     }
}

void
evas_common_convert_color_rgb_to_hsv(int r, int g, int b, float *h, float *s, float *v)
{
   int max, min, d = r - g;

   //set min to MIN(g,r)
   d = (d & (~(d >> 8)));
   min = r - d;
   //set max to MAX(g,r)
   max = g + d;

   //set min to MIN(b,min)
   d = min - b;
   min -= (d & (~(d >> 8)));

   //set max to MAX(max,b)
   d = b - max;
   max += (d & (~(d >> 8)));

   d = max - min;

   if (v) *v = (max / 255.0);
   if (!max || !d)
     {
	if (s) *s = 0;
        if (h) *h = 0;
	return;
     }

   if (s) *s = (d / (float)max);
   if (r == max)
     {
       if (h)
         {
           *h = 60 * ((g - b) / (float)d);
           if (*h < 0) *h += 360;
         }
       return;
     }
   if (g == max)
     {
       if (h)
         {
           *h = 120 + (60 * ((b - r) / (float)d));
           if (*h < 0) *h += 360;
         }
       return;
     }
   if (h)
     {
       *h = 240 + (60 * ((r - g) / (float)d));
       if (*h < 0) *h += 360;
     }
}

void
evas_common_convert_color_hsv_to_rgb_int(int h, int s, int v, int *r, int *g, int *b)
{
   int   i, f;

   if (!s)
     {
	*r = *g = *b = v;
	return;
     }

   i = h / 255;
   f = h - (i * 255);
   s = (v * s) / 255;
   f = (s * f) / 255;
   s = v - s;

   switch (i)
     {
	case 1:
	  *r = v - f; *g = v; *b = s;
	  return;
	case 2:
	  *r = s; *g = v; *b = s + f;
	  return;
	case 3:
	  *r = s; *g = v - f; *b = v;
	  return;
	case 4:
	  *r = s + f; *g = s; *b = v;
	  return;
	case 5:
	  *r = v; *g = s; *b = v - f;
	  return;
	default:
	  *r = v; *g = s + f; *b = s;
         break;
     }
}

void
evas_common_convert_color_rgb_to_hsv_int(int r, int g, int b, int *h, int *s, int *v)
{
   int  min, max, d = r - g;

   d = (d & (~(d >> 8)));
   min = r - d;
   max = g + d;

   d = min - b;
   min -= (d & (~(d >> 8)));

   d = b - max;
   max += (d & (~(d >> 8)));

   d = max - min;

   *v = max;
   if (!max)
     {
	*s = *h = 0;
	return;
     }

   *s = ((d * 255) / max);

   if (r == max)
     {
	*h = (((g - b) * 255) / d);
	if (*h < 0) *h += 1530;
	return;
     }
   if (g == max)
     {
	*h = 510 + (((b - r) * 255) / d);
	if (*h < 0) *h += 1530;
	return;
     }
   *h = 1020 + (((r - g) * 255) / d);
   if (*h < 0) *h += 1530;

}
