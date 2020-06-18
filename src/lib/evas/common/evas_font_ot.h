#ifndef _EVAS_FONT_OT_H
# define _EVAS_FONT_OT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

# ifdef HAVE_HARFBUZZ
#  define OT_SUPPORT
#  define USE_HARFBUZZ
# endif

# ifdef OT_SUPPORT
#  include <stdlib.h>
typedef struct _Evas_Font_OT_Info Evas_Font_OT_Info;
# else
typedef void *Evas_Font_OT_Info;
# endif

# ifdef OT_SUPPORT
struct _Evas_Font_OT_Info
{
   size_t source_cluster;
   int x_offset;
   int y_offset;
};
# endif

# ifdef OT_SUPPORT
#  define EVAS_FONT_OT_X_OFF_GET(a) ((a).x_offset)
#  define EVAS_FONT_OT_Y_OFF_GET(a) ((a).y_offset)
#  define EVAS_FONT_OT_POS_GET(a)   ((a).source_cluster)
# endif

#include "evas_font.h"
#include "Evas.h"

EAPI int
evas_common_font_ot_cluster_size_get(const Evas_Text_Props *props, size_t char_index);

EAPI Eina_Bool
evas_common_font_ot_populate_text_props(const Eina_Unicode *text,
      Evas_Text_Props *props, int len, Evas_Text_Props_Mode mode, const char *lang);
#endif

