#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <eina_stdio.h>
#include <png.h>
#include <setjmp.h>

#include "evas_common_private.h"
#include "evas_private.h"

static int evas_image_save_file_png(RGBA_Image *im, const char *file, const char *key, int quality, int compress, const char *encoding);

static Evas_Image_Save_Func evas_image_save_png_func =
{
   evas_image_save_file_png
};

static int
save_image_png(RGBA_Image *im, const char *file, int do_compress, int interlace)
{
   FILE               *f;
   png_structp         png_ptr;
   png_infop           info_ptr;
   uint8_t            *ptr, *data = NULL;
   unsigned int        x, y, j;
   png_bytep           row_ptr;
   volatile png_bytep  png_data = NULL;
   png_color_8         sig_bit = {};
   int                 num_passes = 1, pass;
   volatile Eina_Bool  gry8 = EINA_FALSE;
   volatile Eina_Bool  agry88 = EINA_FALSE;
   volatile Eina_Bool  free_data = EINA_FALSE;
   volatile int        pixel_size = 4;

   if (!im || !im->image.data || !file)
      return 0;

   if (((Efl_Gfx_Colorspace)im->cache_entry.space != EFL_GFX_COLORSPACE_ARGB8888) &&
       ((Efl_Gfx_Colorspace)im->cache_entry.space != EFL_GFX_COLORSPACE_AGRY88) &&
       ((Efl_Gfx_Colorspace)im->cache_entry.space != EFL_GFX_COLORSPACE_GRY8))
     return 0;

   f = fopen(file, "wb");
   if (!f) return 0;

   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (!png_ptr)
     goto close_file;

   info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr)
     {
        png_destroy_write_struct(&png_ptr, NULL);
        goto close_file;
     }
   if (setjmp(png_jmpbuf(png_ptr)))
     {
        png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
        png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);
        goto close_file;
     }

   if (interlace)
     {
#ifdef PNG_WRITE_INTERLACING_SUPPORTED
        interlace = PNG_INTERLACE_ADAM7;
#else
        interlace = PNG_INTERLACE_NONE;
#endif
     }
   else
     interlace = PNG_INTERLACE_NONE;

   if ((Efl_Gfx_Colorspace)im->cache_entry.space == EFL_GFX_COLORSPACE_GRY8)
     {
        gry8 = EINA_TRUE;
        pixel_size = 1;
        data = im->image.data8;
        png_init_io(png_ptr, f);
        png_set_IHDR(png_ptr, info_ptr, im->cache_entry.w, im->cache_entry.h, 8,
                     PNG_COLOR_TYPE_GRAY, interlace,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
     }
   else if ((Efl_Gfx_Colorspace)im->cache_entry.space == EFL_GFX_COLORSPACE_AGRY88)
     {
        agry88 = EINA_TRUE;
        pixel_size = 2;
        data = malloc(im->cache_entry.w * im->cache_entry.h * pixel_size);
        if (!data)
          {
             png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
             png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);
             goto close_file;
          }
        free_data = EINA_TRUE;
        memcpy(data, im->image.data, im->cache_entry.w * im->cache_entry.h * pixel_size);
        evas_common_convert_ag_unpremul((DATA16 *) data, im->cache_entry.w * im->cache_entry.h);
        png_init_io(png_ptr, f);
        png_set_IHDR(png_ptr, info_ptr, im->cache_entry.w, im->cache_entry.h, 8,
                     PNG_COLOR_TYPE_GRAY_ALPHA, interlace,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
     }
   else if (im->cache_entry.flags.alpha)
     {
        data = malloc(im->cache_entry.w * im->cache_entry.h * sizeof(DATA32));
        if (!data)
          {
             png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
             png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);
             goto close_file;
          }
        free_data = EINA_TRUE;
        memcpy(data, im->image.data, im->cache_entry.w * im->cache_entry.h * sizeof(DATA32));
        evas_common_convert_argb_unpremul((DATA32 *) data, im->cache_entry.w * im->cache_entry.h);
        png_init_io(png_ptr, f);
        png_set_IHDR(png_ptr, info_ptr, im->cache_entry.w, im->cache_entry.h, 8,
                     PNG_COLOR_TYPE_RGB_ALPHA, interlace,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
#ifdef WORDS_BIGENDIAN
        png_set_swap_alpha(png_ptr);
#else
        png_set_bgr(png_ptr);
#endif
     }
   else
     {
        data = im->image.data8;
        png_init_io(png_ptr, f);
        png_set_IHDR(png_ptr, info_ptr, im->cache_entry.w, im->cache_entry.h, 8,
                     PNG_COLOR_TYPE_RGB, interlace,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        png_data = alloca(im->cache_entry.w * 3 * sizeof(char));
     }
   sig_bit.red = 8;
   sig_bit.green = 8;
   sig_bit.blue = 8;
   sig_bit.alpha = 8;
   sig_bit.gray = 8;
   png_set_sBIT(png_ptr, info_ptr, &sig_bit);

   png_set_compression_level(png_ptr, do_compress);
   png_write_info(png_ptr, info_ptr);
   png_set_shift(png_ptr, &sig_bit);
   png_set_packing(png_ptr);

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   num_passes = png_set_interlace_handling(png_ptr);
#endif

   for (pass = 0; pass < num_passes; pass++)
     {
        ptr = data;

        for (y = 0; y < im->cache_entry.h; y++)
          {
             if (gry8 || agry88 || im->cache_entry.flags.alpha)
               row_ptr = (png_bytep) ptr;
             else
               {
                  DATA32 *ptr32 = (DATA32 *) ptr;
                  for (j = 0, x = 0; x < im->cache_entry.w; x++)
                    {
                       png_data[j++] = (ptr32[x] >> 16) & 0xff;
                       png_data[j++] = (ptr32[x] >> 8) & 0xff;
                       png_data[j++] = (ptr32[x]) & 0xff;
                    }
                  row_ptr = (png_bytep) png_data;
               }
             png_write_rows(png_ptr, &row_ptr, 1);
             ptr += im->cache_entry.w * pixel_size;
          }
     }
   png_write_end(png_ptr, info_ptr);
   png_destroy_write_struct(&png_ptr, (png_infopp) & info_ptr);
   png_destroy_info_struct(png_ptr, (png_infopp) & info_ptr);

   if (free_data) free(data);
   fclose(f);
   return 1;

 close_file:
   fclose(f);
   return 0;
}

static int 
evas_image_save_file_png(RGBA_Image *im, const char *file, const char *key EINA_UNUSED,
                         int quality EINA_UNUSED, int do_compress, const char *encoding EINA_UNUSED)
{
   return save_image_png(im, file, do_compress, 0);
}

static int
module_open(Evas_Module *em)
{
   if (!em) return 0;
   em->functions = (void *)(&evas_image_save_png_func);
   return 1;
}

static void
module_close(Evas_Module *em EINA_UNUSED)
{
}

static Evas_Module_Api evas_modapi =
{
   EVAS_MODULE_API_VERSION,
   "png",
   "none",
   {
     module_open,
     module_close
   }
};

EVAS_MODULE_DEFINE(EVAS_MODULE_TYPE_IMAGE_SAVER, image_saver, png);

#ifndef EVAS_STATIC_BUILD_PNG
EVAS_EINA_MODULE_DEFINE(image_saver, png);
#endif

