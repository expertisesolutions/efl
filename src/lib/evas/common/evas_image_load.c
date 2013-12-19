#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "evas_common_private.h"
#include "evas_private.h"
//#include "evas_cs.h"
#ifdef EVAS_CSERVE2
#include "evas_cs2_private.h"
#endif

struct ext_loader_s
{
   unsigned int length;
   const char *extension;
   const char *loader;
};

#define MATCHING(Ext, Module)                   \
  { sizeof (Ext), Ext, Module }

static const struct ext_loader_s loaders[] =
{ /* map extensions to loaders to use for good first-guess tries */
   MATCHING(".png", "png"),
   
   MATCHING(".jpg", "jpeg"),
   MATCHING(".jpeg", "jpeg"),
   MATCHING(".jfif", "jpeg"),
   
   MATCHING(".j2k", "jp2k"),
   MATCHING(".jp2", "jp2k"),
   MATCHING(".jpx", "jp2k"),
   MATCHING(".jpf", "jp2k"),
   
   MATCHING(".eet", "eet"),
   MATCHING(".edj", "eet"),
   MATCHING(".eap", "eet"),
   
   MATCHING(".xpm", "xpm"),
   
   MATCHING(".tiff", "tiff"),
   MATCHING(".tif", "tiff"),
   
   MATCHING(".gif", "gif"),
   
   MATCHING(".pbm", "pmaps"),
   MATCHING(".pgm", "pmaps"),
   MATCHING(".ppm", "pmaps"),
   MATCHING(".pnm", "pmaps"),
   
   MATCHING(".bmp", "bmp"),
   
   MATCHING(".tga", "tga"),
   
   MATCHING(".wbmp", "wbmp"),
   
   MATCHING(".webp", "webp"),
   
   MATCHING(".ico", "ico"),
   MATCHING(".cur", "ico"),
   
   MATCHING(".psd", "psd"),
   /* xcf - gefenric */
   MATCHING(".xcf", "generic"),
   MATCHING(".xcf.gz", "generic"),
   /* docs */
   MATCHING(".doc", "generic"),
   MATCHING(".docx", "generic"),
   MATCHING(".odp", "generic"),
   MATCHING(".ods", "generic"),
   MATCHING(".odt", "generic"),
   MATCHING(".pdf", "generic"),
   MATCHING(".ps", "generic"),
   MATCHING(".ppt", "generic"),
   MATCHING(".pptx", "generic"),
   MATCHING(".rtf", "generic"),
   MATCHING(".xls", "generic"),
   MATCHING(".xlsx", "generic"),
   /* svg - generic */
   MATCHING(".svg", "generic"),
   MATCHING(".svgz", "generic"),
   MATCHING(".svg.gz", "generic"),
   /* RAW */
   MATCHING(".arw", "generic"),
   MATCHING(".cr2", "generic"),
   MATCHING(".crw", "generic"),
   MATCHING(".dcr", "generic"),
   MATCHING(".dng", "generic"),
   MATCHING(".erf", "generic"),
   MATCHING(".k25", "generic"),
   MATCHING(".kdc", "generic"),
   MATCHING(".mrw", "generic"),
   MATCHING(".nef", "generic"),
   MATCHING(".nrf", "generic"),
   MATCHING(".nrw", "generic"),
   MATCHING(".orf", "generic"),
   MATCHING(".pef", "generic"),
   MATCHING(".raf", "generic"),
   MATCHING(".raw", "generic"),
   MATCHING(".rw2", "generic"),
   MATCHING(".sr2", "generic"),
   MATCHING(".srf", "generic"),
   MATCHING(".x3f", "generic"),
   /* video */
   MATCHING(".264", "generic"),
   MATCHING(".3g2", "generic"),
   MATCHING(".3gp", "generic"),
   MATCHING(".3gp2", "generic"),
   MATCHING(".3gpp", "generic"),
   MATCHING(".3gpp2", "generic"),
   MATCHING(".3p2", "generic"),
   MATCHING(".asf", "generic"),
   MATCHING(".avi", "generic"),
   MATCHING(".bdm", "generic"),
   MATCHING(".bdmv", "generic"),
   MATCHING(".clpi", "generic"),
   MATCHING(".cpi", "generic"),
   MATCHING(".dv", "generic"),
   MATCHING(".fla", "generic"),
   MATCHING(".flv", "generic"),
   MATCHING(".m1v", "generic"),
   MATCHING(".m2t", "generic"),
   MATCHING(".m2v", "generic"),
   MATCHING(".m4v", "generic"),
   MATCHING(".mkv", "generic"),
   MATCHING(".mov", "generic"),
   MATCHING(".mp2", "generic"),
   MATCHING(".mp2ts", "generic"),
   MATCHING(".mp4", "generic"),
   MATCHING(".mpe", "generic"),
   MATCHING(".mpeg", "generic"),
   MATCHING(".mpg", "generic"),
   MATCHING(".mpl", "generic"),
   MATCHING(".mpls", "generic"),
   MATCHING(".mts", "generic"),
   MATCHING(".mxf", "generic"),
   MATCHING(".nut", "generic"),
   MATCHING(".nuv", "generic"),
   MATCHING(".ogg", "generic"),
   MATCHING(".ogm", "generic"),
   MATCHING(".ogv", "generic"),
   MATCHING(".qt", "generic"),
   MATCHING(".rm", "generic"),
   MATCHING(".rmj", "generic"),
   MATCHING(".rmm", "generic"),
   MATCHING(".rms", "generic"),
   MATCHING(".rmvb", "generic"),
   MATCHING(".rmx", "generic"),
   MATCHING(".rv", "generic"),
   MATCHING(".swf", "generic"),
   MATCHING(".ts", "generic"),
   MATCHING(".weba", "generic"),
   MATCHING(".webm", "generic"),
   MATCHING(".wmv", "generic")
};

static const char *loaders_name[] =
{ /* in order of most likely needed */
  "png", "jpeg", "eet", "xpm", "tiff", "gif", "svg", "webp", "pmaps",
  "bmp", "tga", "wbmp", "ico", "psd", "jp2k", "generic"
};

struct evas_image_foreach_loader_data
{
   Image_Entry *ie;
   int *error;
   Evas_Module *em;
};

static Eina_Bool
_evas_image_file_header(Evas_Module *em, Image_Entry *ie, int *error)
{
   Evas_Image_Load_Func *evas_image_load_func = NULL;
   Eina_Bool r = EINA_TRUE;
   
   if (!evas_module_load(em)) goto load_error;
   evas_image_load_func = em->functions;
   evas_module_use(em);
   *error = EVAS_LOAD_ERROR_NONE;
   if (evas_image_load_func)
     {
        Evas_Image_Property property;
        const char *file;

        if (!ie->f)
          {
             ie->f = eina_file_open(ie->file, EINA_FALSE);
             file = ie->file;
          }
        else
          {
             file = eina_file_filename_get(ie->f);
          }
        if (!ie->f)
          {
             *error = EVAS_LOAD_ERROR_DOES_NOT_EXIST;
             goto load_error;
          }

	ie->loader_data = evas_image_load_func->file_open(ie->f, ie->key,
							  &ie->load_opts,
							  &ie->animated,
							  error);
	if (!ie->loader_data)
	  {
             goto load_error;
	  }

        memset(&property, 0, sizeof (Evas_Image_Property));
        if (evas_image_load_func->file_head(ie->loader_data, &property,
                                            error) &&
            (*error == EVAS_LOAD_ERROR_NONE))
          {
             DBG("loaded file head using module '%s' (%p): %s",
                 em->definition->name, em, file);

             ie->w = property.w;
             ie->h = property.h;
             ie->scale = property.scale;
             ie->flags.alpha = property.alpha;
	     if (ie->load_opts.orientation &&
		 ie->load_opts.degree != 0)
	       ie->flags.rotated = EINA_TRUE;
             r = EINA_FALSE;
          }
        else
          {
             evas_image_load_func->file_close(ie->loader_data);
             ie->loader_data = NULL;
             evas_module_unload(em);
             INF("failed to load file head using module '%s' (%p): "
                 "%s (%s)",
                 em->definition->name, em, file, evas_load_error_str(*error));
          }
     }
   else
     {
     load_error:
        evas_module_unload(em);
        WRN("failed to load module '%s'.", em->definition->name);
     }

   return r;
}

static Eina_Bool
_evas_image_foreach_loader(const Eina_Hash *hash EINA_UNUSED, const void *key EINA_UNUSED, void *data, void *fdata)
{
   struct evas_image_foreach_loader_data *d = fdata;
   Evas_Module *em = data;
   Image_Entry *ie = d->ie;
   Eina_Bool r;

   r = _evas_image_file_header(em, ie, d->error);

   if (!r)
     d->em = em;
   return r;
}

EAPI int
evas_common_load_rgba_image_module_from_file(Image_Entry *ie)
{
   const char           *loader = NULL, *end;
   const char           *file;
   Evas_Module          *em;
   unsigned int          i;
   int                   len, ret = EVAS_LOAD_ERROR_NONE;
   struct evas_image_foreach_loader_data fdata;

#ifdef EVAS_CSERVE2
   if (evas_cserve2_use_get() && evas_cache2_image_cached(ie))
     CRIT("This function shouldn't be called anymore!");
#endif

   if (ie->f)
     {
        len = strlen(eina_file_filename_get(ie->f));
        end = eina_file_filename_get(ie->f) + len;
        file = eina_file_filename_get(ie->f);
     }
   else
     {
        struct stat st;

        if (stat(ie->file, &st) != 0 || S_ISDIR(st.st_mode))
          {
             DBG("trying to open directory '%s' !", ie->file);
             return EVAS_LOAD_ERROR_DOES_NOT_EXIST;
          }

        len = strlen(ie->file);
        end = ie->file + len;
        file = ie->file;
     }

   for (i = 0; i < (sizeof (loaders) / sizeof(struct ext_loader_s)); i++)
     {
        int len2 = strlen(loaders[i].extension);
        if (len2 > len) continue;
        if (!strcasecmp(end - len2, loaders[i].extension))
          {
             loader = loaders[i].loader;
             DBG("known loader '%s' handles extension '%s' of file '%s'",
                 loader, end - len2, file);
             break;
          }
     }

   if (loader)
     {
	em = evas_module_find_type(EVAS_MODULE_TYPE_IMAGE_LOADER, loader);
	if (em)
	  {
	     DBG("found image loader '%s' (%p)", loader, em);
             if (!_evas_image_file_header(em, ie, &ret))
               goto end;
	  }
	else
	  INF("image loader '%s' is not enabled or missing!", loader);
     }

   fdata.ie = ie;
   fdata.error = &ret;
   fdata.em = NULL;
   ret = EVAS_LOAD_ERROR_NONE;
   evas_module_foreach_image_loader(_evas_image_foreach_loader, &fdata);
   em = fdata.em;
   if (em) goto end;

   /* This is our last chance, try all known image loader. */
   /* FIXME: We could use eina recursive module search ability. */
   for (i = 0; i < sizeof (loaders_name) / sizeof (char *); i++)
     {
	em = evas_module_find_type(EVAS_MODULE_TYPE_IMAGE_LOADER, loaders_name[i]);
	if (em)
	  {
             if (!_evas_image_file_header(em, ie, &ret))
               goto end;
	  }
	else
	  DBG("could not find module '%s'", loaders_name[i]);
     }

   INF("exhausted all means to load image '%s'", file);
   return EVAS_LOAD_ERROR_UNKNOWN_FORMAT;

 end:

   if (ret != EVAS_LOAD_ERROR_NONE)
     {
	const char *modname = NULL;
	int modversion = -1;
	if (em && em->definition)
	  {
	     modname = em->definition->name;
	     modversion = em->definition->version;
	  }
	WRN("loader '%s' (version %d) "
	    "handled file '%s', key '%s' with errors: %s",
	    modname ? modname : "<UNKNOWN>", modversion,
	    file, ie->key ? ie->key : "",
	    evas_load_error_str(ret));
	goto end;
     }

   DBG("loader '%s' used for file %s",
       (em && em->definition && em->definition->name) ?
       em->definition->name : "<UNKNOWN>",
       file);

   ie->info.module = em;
   ie->info.loader = (void*) em ? em->functions : NULL;
   evas_module_ref(ie->info.module);
   return ret;
}

EAPI int
evas_common_load_rgba_image_data_from_file(Image_Entry *ie)
{
   void *pixels;
   Evas_Image_Load_Func *evas_image_load_func = NULL;
   Evas_Image_Property property;
   int ret = EVAS_LOAD_ERROR_NONE;

   if ((ie->flags.loaded) && (!ie->animated.animated)) return EVAS_LOAD_ERROR_GENERIC;

#ifdef EVAS_CSERVE2
   if (evas_cserve2_use_get() && evas_cache2_image_cached(ie))
     CRIT("This function shouldn't be called anymore!");
#endif

   if (!ie->info.module) return EVAS_LOAD_ERROR_GENERIC;

//   printf("load data [%p] %s %s\n", ie, ie->file, ie->key);
           
   evas_image_load_func = ie->info.loader;
   evas_module_use(ie->info.module);

   if (!ie->f) return EVAS_LOAD_ERROR_DOES_NOT_EXIST;

   memset(&property, 0, sizeof (Evas_Image_Property));
   property.w = ie->w;
   property.h = ie->h;
   property.scale = ie->scale;
   property.rotated = ie->flags.rotated;
   property.premul = EINA_FALSE;
   property.alpha_sparse = EINA_FALSE;

   evas_cache_image_surface_alloc(ie, ie->w, ie->h);

   pixels = evas_cache_image_pixels(ie);
   if (!pixels)
     return EVAS_LOAD_ERROR_RESOURCE_ALLOCATION_FAILED;

   evas_image_load_func->file_data(ie->loader_data, &property, pixels, &ret);

   ie->flags.alpha_sparse = property.alpha_sparse;

   if (property.premul)
     evas_common_image_premul(ie);
//   evas_module_unref(ie->info.module);
//   ie->info.module = NULL;

   return ret;
}

EAPI double
evas_common_load_rgba_image_frame_duration_from_file(Image_Entry *ie, const int start, const int frame_num)
{
   Evas_Image_Load_Func *evas_image_load_func = NULL;

   if (!ie->info.module) return -1;

   evas_image_load_func = ie->info.loader;
   evas_module_use(ie->info.module);
   if (evas_image_load_func->frame_duration)
     {
        if (!ie->f) return -1;

        return evas_image_load_func->frame_duration(ie->loader_data, start, frame_num);
     }
   return -1;
}

EAPI Eina_Bool
evas_common_extension_can_load_get(const char *file)
{
   unsigned int length;
   unsigned int i;

   length = eina_stringshare_strlen(file) + 1;
   if (length < 5) return EINA_FALSE;

   for (i = 0; i < sizeof (loaders) / sizeof (struct ext_loader_s); ++i)
     {
        if (loaders[i].length > length) continue;

        if (!strcasecmp(loaders[i].extension, file + length - loaders[i].length))
          return EINA_TRUE;
     }

   return EINA_FALSE;
}
