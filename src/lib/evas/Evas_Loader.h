/* EVAS - EFL Scene Graph
 * Copyright (C) 2013 Enlightenment Developers:
 *           Cedric Bail <cedric.bail@free.fr>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * What does it means for you ?
 *    Evas Loader is dynamically linked to Evas at run time. You can use anything
 * you want here as long as the license of the module you add to your system matches
 * the application you are using it in. Be aware that if you make a module that uses
 * a GPL library, it may turn the license of your binary to GPL. Evas doesn't ship
 * with any GPL modules.
 */

#ifndef _EVAS_LOADER_H
#define _EVAS_LOADER_H

#include "Eina.h"
#include "Emile.h"

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* the module api version */
#define EVAS_MODULE_API_VERSION 3

/* the module types */
typedef enum _Evas_Module_Type
{
   EVAS_MODULE_TYPE_ENGINE = 0,
   EVAS_MODULE_TYPE_IMAGE_LOADER = 1,
   EVAS_MODULE_TYPE_IMAGE_SAVER = 2,
   EVAS_MODULE_TYPE_OBJECT = 3,
   EVAS_MODULE_TYPE_VG_LOADER = 4,
   EVAS_MODULE_TYPE_VG_SAVER = 5
} Evas_Module_Type;

typedef struct _Evas_Module_Api    Evas_Module_Api;
typedef struct _Evas_Module        Evas_Module;
typedef struct _Evas_Module_Path   Evas_Module_Path;
typedef struct _Evas_Module_Engine Evas_Module_Engine;
typedef struct _Evas_Module_Public Evas_Module_Public;

/* the module api structure, all modules should define this struct */
struct _Evas_Module_Api
{
   int			version;
   const char		*name;
   const char		*author;

   struct
     {
	int (*open)(Evas_Module *);
	void (*close)(Evas_Module *);
     } func;
};

/* the module structure */
struct _Evas_Module
{
   const Evas_Module_Api *definition;

   void		*functions;	/* these are the functions exported by the module */
   int           id_engine;	/* some internal data for the module i.e the id for engines */

   int           ref; /* how many refs */
   int           last_used; /* the cycle count when it was last used */

   Eina_Lock     lock;

   unsigned char	loaded : 1;
};

/**
 * @addtogroup Evas_Image
 * @{
 */

typedef struct _Evas_Image_Load_Opts  Evas_Image_Load_Opts;

struct _Evas_Image_Load_Opts
{
   Emile_Image_Load_Opts emile;
   Eina_Bool             skip_head; // skip open of file and header load and
                                    // defer this until a preload is done
                                    // to allow for real async/threaded load
};

typedef Emile_Image_Animated  Evas_Image_Animated;
typedef struct _Evas_Image_Property Evas_Image_Property;

struct _Evas_Image_Property
{
  Emile_Image_Property info;
  // Stretch region are directly encoded the way Evas excpect them internally
  // 8bits is used for each step. The lower bits indicate how long the stretch region
  // span. Masking with 0x80 will be true if the region is stretchable. If false, it
  // will be fixed size.
  struct {
     struct {
        uint8_t *region;
     } horizontal, vertical;
  } stretch;
  // Where inside the image are we supposed to overlay data
  Eina_Rectangle content;
  // need_data is set to True when to get accurate property, data need to be loaded
  Eina_Bool need_data;
};

typedef struct _Evas_Image_Load_Func Evas_Image_Load_Func;

/**
 * @brief Possible outcomes of a load operation
 */
typedef enum
{
   EVAS_LOAD_ERROR_NONE = 0,  /**< No error on load */
   EVAS_LOAD_ERROR_GENERIC,  /**< A non-specific error occurred */
   EVAS_LOAD_ERROR_DOES_NOT_EXIST,  /**< File (or file path) does not exist */
   EVAS_LOAD_ERROR_PERMISSION_DENIED,	 /**< Permission denied to an existing file (or path) */
   EVAS_LOAD_ERROR_RESOURCE_ALLOCATION_FAILED,  /**< Allocation of resources failure prevented load */
   EVAS_LOAD_ERROR_CORRUPT_FILE,  /**< File corrupt (but was detected as a known format) */
   EVAS_LOAD_ERROR_UNKNOWN_FORMAT,  /**< File is not a known format */
   EVAS_LOAD_ERROR_CANCELLED, /**< File loading has been cancelled */
} Evas_Load_Error;

typedef Emile_Image_Animated_Loop_Hint Evas_Image_Animated_Loop_Hint;

#define EVAS_IMAGE_ANIMATED_HINT_NONE EMILE_IMAGE_ANIMATED_HINT_NONE
#define EVAS_IMAGE_ANIMATED_HINT_LOOP EMILE_IMAGE_ANIMATED_HINT_LOOP
#define EVAS_IMAGE_ANIMATED_HINT_PINGPONG EMILE_IMAGE_ANIMATED_HINT_PINGPONG

typedef Emile_Image_Scale_Hint Evas_Image_Scale_Hint; /**< How an image's data is to be treated by Evas, with regard to scaling cache */

#define EVAS_IMAGE_SCALE_HINT_NONE EMILE_IMAGE_SCALE_HINT_NONE
#define EVAS_IMAGE_SCALE_HINT_DYNAMIC EMILE_IMAGE_SCALE_HINT_DYNAMIC
#define EVAS_IMAGE_SCALE_HINT_STATIC EMILE_IMAGE_SCALE_HINT_STATIC

/**
 * Colorspaces for pixel data supported by Evas
 * @ingroup Evas_Object_Image
 */
typedef Emile_Colorspace Evas_Colorspace; /**< Colorspaces for pixel data supported by Evas */

#define EVAS_COLORSPACE_ARGB8888 EMILE_COLORSPACE_ARGB8888
#define EVAS_COLORSPACE_YCBCR422P601_PL EMILE_COLORSPACE_YCBCR422P601_PL
#define EVAS_COLORSPACE_YCBCR422P709_PL EMILE_COLORSPACE_YCBCR422P709_PL
#define EVAS_COLORSPACE_RGB565_A5P EMILE_COLORSPACE_RGB565_A5P
#define EVAS_COLORSPACE_GRY8 EMILE_COLORSPACE_GRY8
#define EVAS_COLORSPACE_YCBCR422601_PL EMILE_COLORSPACE_YCBCR422601_PL
#define EVAS_COLORSPACE_YCBCR420NV12601_PL EMILE_COLORSPACE_YCBCR420NV12601_PL
#define EVAS_COLORSPACE_YCBCR420TM12601_PL EMILE_COLORSPACE_YCBCR420TM12601_PL
#define EVAS_COLORSPACE_AGRY88 EMILE_COLORSPACE_AGRY88
   // ETC1/2 support
#define EVAS_COLORSPACE_ETC1 EMILE_COLORSPACE_ETC1
#define EVAS_COLORSPACE_RGB8_ETC2 EMILE_COLORSPACE_RGB8_ETC2
#define EVAS_COLORSPACE_RGBA8_ETC2_EAC EMILE_COLORSPACE_RGBA8_ETC2_EAC
#define EVAS_COLORSPACE_ETC1_ALPHA EMILE_COLORSPACE_ETC1_ALPHA
   // S3TC support
#define EVAS_COLORSPACE_RGB_S3TC_DXT1 EMILE_COLORSPACE_RGB_S3TC_DXT1
#define EVAS_COLORSPACE_RGBA_S3TC_DXT1 EMILE_COLORSPACE_RGBA_S3TC_DXT1
#define EVAS_COLORSPACE_RGBA_S3TC_DXT2 EMILE_COLORSPACE_RGBA_S3TC_DXT2
#define EVAS_COLORSPACE_RGBA_S3TC_DXT3 EMILE_COLORSPACE_RGBA_S3TC_DXT3
#define EVAS_COLORSPACE_RGBA_S3TC_DXT4 EMILE_COLORSPACE_RGBA_S3TC_DXT4
#define EVAS_COLORSPACE_RGBA_S3TC_DXT5 EMILE_COLORSPACE_RGBA_S3TC_DXT5

#define EVAS_IMAGE_LOAD_VERSION 2

struct _Evas_Image_Load_Func
{
  int       version;
  void     *(*file_open) (Eina_File *f, Eina_Stringshare *key,
			  Evas_Image_Load_Opts *opts,
			  Evas_Image_Animated *animated,
			  int *error);
  void     (*file_close) (void *loader_data);

  Eina_Bool (*file_head) (void *loader_data,
                          Evas_Image_Property *prop,
                          int *error);
  Eina_Bool (*file_head_with_data) (void *loader_data,
                                    Evas_Image_Property *prop,
                                    void *pixels, int *error);
  Eina_Bool (*file_data) (void *loader_data,
                          Evas_Image_Property *prop,
                          void *pixels, int *error);
  double    (*frame_duration) (void *loader_data,
			       int start, int frame_num);

  Eina_Bool threadable;
  Eina_Bool do_region;
};

EAPI Eina_Bool    evas_module_register   (const Evas_Module_Api *module, Evas_Module_Type type);
EAPI Eina_Bool    evas_module_unregister (const Evas_Module_Api *module, Evas_Module_Type type);

EAPI Eina_Bool    evas_module_task_cancelled (void); /**< @since 1.19 */

#define EVAS_MODULE_TASK_CHECK(Count, Mask, Error, Error_Handler)       \
  do {                                                                  \
     Count++;                                                           \
     if ((Count & Mask) == Mask)                                        \
       {                                                                \
          Count = 0;                                                    \
          if (evas_module_task_cancelled())                             \
            {                                                           \
               *Error = EVAS_LOAD_ERROR_CANCELLED;                 \
               goto Error_Handler;                                      \
            }                                                           \
       }                                                                \
  } while (0)

#define EVAS_MODULE_DEFINE(Type, Tn, Name)		\
  Eina_Bool evas_##Tn##_##Name##_init(void)		\
  {							\
     return evas_module_register(&evas_modapi, Type);	\
  }							\
  void evas_##Tn##_##Name##_shutdown(void)		\
  {							\
     evas_module_unregister(&evas_modapi, Type);	\
  }

#define EVAS_EINA_MODULE_DEFINE(Tn, Name)	\
  EINA_MODULE_INIT(evas_##Tn##_##Name##_init);	\
  EINA_MODULE_SHUTDOWN(evas_##Tn##_##Name##_shutdown);

static inline Eina_Bool
evas_loader_helper_stretch_region_push(uint8_t **region,
                                       uint8_t *offset,
                                       Eina_Bool stretchable)
{
   uint32_t length = 0;
   void *tmp;

   if (*offset == 0) return EINA_TRUE;

   while (*region && (*region)[length] != 0)
     length++;

   // +1 for termination and +1 for the region being pushed
   tmp = realloc(*region, sizeof (uint8_t) * (length + 2));
   if (!tmp) return EINA_FALSE;

   *region = (uint8_t *) tmp;
   (*region)[length] = (*offset) | (stretchable ? 0x80 : 0);
   (*region)[length + 1] = 0;

   *offset = 0;

   return EINA_TRUE;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#undef EAPI
#define EAPI

#endif
