#ifndef _EVAS_FONT_H
#define _EVAS_FONT_H

typedef unsigned char         DATA8;
typedef unsigned long long    DATA64;

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_SIZES_H
#include FT_MODULE_H

#ifndef FT_HAS_COLOR
# define FT_HAS_COLOR(face) 0
#endif

#ifndef FT_LOAD_COLOR
# define FT_LOAD_COLOR FT_LOAD_DEFAULT
#endif

#include <Eina.h>

#include "evas_text_utils.h"

#include <evas_api.h>

#define LK(x)  Eina_Lock x
#define LKI(x) eina_lock_new(&(x))
#define LKD(x) eina_lock_free(&(x))
#define LKL(x) eina_lock_take(&(x))
#define LKT(x) eina_lock_take_try(&(x))
#define LKU(x) eina_lock_release(&(x))
#define LKDBG(x) eina_lock_debug(&(x))

/**
 * See explanation of variation_sequences at:
 * https://unicode.org/Public/UCD/latest/ucd/StandardizedVariants.txt
 * https://unicode.org/reports/tr37/
 * https://unicode.org/ivd/
 * https://www.freetype.org/freetype2/docs/reference/ft2-glyph_variants.html
*/
#define VAR_SEQ(x) GENERIC_VARIATION_SEQUENCES(x) | IDEOGRAPHICS_VARIATION_SEQUENCES(x) | MANGOLIAN_VARIATION_SEQUENCES(x)
#define GENERIC_VARIATION_SEQUENCES(x) (x>=0xFE00 && x<=0xFE0F) ? x : 0
#define IDEOGRAPHICS_VARIATION_SEQUENCES(x) (x>=0xE0100 && x<=0xE01EF) ? x : 0
#define MANGOLIAN_VARIATION_SEQUENCES(x) (x>=0x180B && x<=0x180D) ? x : 0
/**
 * http://unicode.org/emoji/charts/emoji-variants.html
*/
#define  VARIATION_EMOJI_PRESENTATION  0xFE0F
#define  VARIATION_TEXT_PRESENTATION   0xFE0E

/**
 * These Options (Flags) are used with evas_common_font_glyph_search function
 */
#define EVAS_FONT_SEARCH_OPTION_NONE            0x0000
#define EVAS_FONT_SEARCH_OPTION_SKIP_COLOR      0x0001


#define FASH_INT_MAGIC              0x01012345
#define FASH_GLYPH_MAGIC            0x02012345

enum _Evas_Font_Style
{
   EVAS_FONT_STYLE_SLANT,
   EVAS_FONT_STYLE_WEIGHT,
   EVAS_FONT_STYLE_WIDTH
};

enum _Evas_Font_Slant
{
   EVAS_FONT_SLANT_NORMAL,
   EVAS_FONT_SLANT_OBLIQUE,
   EVAS_FONT_SLANT_ITALIC
};

enum _Evas_Font_Weight
{
   EVAS_FONT_WEIGHT_NORMAL,
   EVAS_FONT_WEIGHT_THIN,
   EVAS_FONT_WEIGHT_ULTRALIGHT,
   EVAS_FONT_WEIGHT_EXTRALIGHT,
   EVAS_FONT_WEIGHT_LIGHT,
   EVAS_FONT_WEIGHT_BOOK,
   EVAS_FONT_WEIGHT_MEDIUM,
   EVAS_FONT_WEIGHT_SEMIBOLD,
   EVAS_FONT_WEIGHT_BOLD,
   EVAS_FONT_WEIGHT_ULTRABOLD,
   EVAS_FONT_WEIGHT_EXTRABOLD,
   EVAS_FONT_WEIGHT_BLACK,
   EVAS_FONT_WEIGHT_EXTRABLACK
};

enum _Evas_Font_Width
{
   EVAS_FONT_WIDTH_NORMAL,
   EVAS_FONT_WIDTH_ULTRACONDENSED,
   EVAS_FONT_WIDTH_EXTRACONDENSED,
   EVAS_FONT_WIDTH_CONDENSED,
   EVAS_FONT_WIDTH_SEMICONDENSED,
   EVAS_FONT_WIDTH_SEMIEXPANDED,
   EVAS_FONT_WIDTH_EXPANDED,
   EVAS_FONT_WIDTH_EXTRAEXPANDED,
   EVAS_FONT_WIDTH_ULTRAEXPANDED
};

enum _Evas_Font_Spacing
{
   EVAS_FONT_SPACING_PROPORTIONAL,
   EVAS_FONT_SPACING_DUAL,
   EVAS_FONT_SPACING_MONO,
   EVAS_FONT_SPACING_CHARCELL
};

typedef enum _Evas_Font_Style               Evas_Font_Style;
typedef enum _Evas_Font_Slant               Evas_Font_Slant;
typedef enum _Evas_Font_Weight              Evas_Font_Weight;
typedef enum _Evas_Font_Width               Evas_Font_Width;
typedef enum _Evas_Font_Spacing             Evas_Font_Spacing;

typedef struct _Evas_Font_Dir               Evas_Font_Dir;
typedef struct _Evas_Font                   Evas_Font;
typedef struct _Evas_Font_Alias             Evas_Font_Alias;
typedef struct _Evas_Font_Description       Evas_Font_Description;


typedef struct _RGBA_Font             RGBA_Font;
typedef struct _RGBA_Font_Int         RGBA_Font_Int;
typedef struct _RGBA_Font_Source      RGBA_Font_Source;
typedef struct _RGBA_Font_Glyph       RGBA_Font_Glyph;
typedef struct _RGBA_Font_Glyph_Out   RGBA_Font_Glyph_Out;

typedef struct _Fash_Item_variation_Index_Item  Fash_Item_variation_Index_Item;
typedef struct _Fash_Item_variation_List        Fash_Item_variation_List;
typedef struct _Fash_Item_Index_Map_Variations  Fash_Item_Index_Map_Variations;

typedef struct _Fash_Item_Index_Map Fash_Item_Index_Map;
typedef struct _Fash_Int_Map        Fash_Int_Map;
typedef struct _Fash_Int_Map2       Fash_Int_Map2;
typedef struct _Fash_Int            Fash_Int;


struct _Fash_Item_Index_Map
{
   RGBA_Font_Int *fint;
   int            index;
};
struct _Fash_Item_variation_Index_Item
{
   Fash_Item_Index_Map  item;
   Eina_Unicode         variation_sequence;
};
struct _Fash_Item_variation_List
{
   Fash_Item_variation_Index_Item  *list;
   size_t                           length;
   size_t                           capacity;
};
struct _Fash_Item_Index_Map_Variations
{
   Fash_Item_Index_Map         item;
   Fash_Item_variation_List   *variations;
};
struct _Fash_Int_Map
{
  Fash_Item_Index_Map_Variations items[256];
};
struct _Fash_Int_Map2
{
   Fash_Int_Map *bucket[256];
};
struct _Fash_Int
{
   unsigned int MAGIC;
   Fash_Int_Map2 *bucket[256];
   void (*freeme) (Fash_Int *fash);
};

typedef struct _Fash_Glyph_Map  Fash_Glyph_Map;
typedef struct _Fash_Glyph_Map2 Fash_Glyph_Map2;
typedef struct _Fash_Glyph      Fash_Glyph;
struct _Fash_Glyph_Map
{
   RGBA_Font_Glyph *item[256];
};
struct _Fash_Glyph_Map2
{
   Fash_Glyph_Map *bucket[256];
};
struct _Fash_Glyph
{
   unsigned int MAGIC;
   Fash_Glyph_Map2 *bucket[256];
   void (*freeme) (Fash_Glyph *fash);
};


typedef enum _Font_Hint_Flags
{
   FONT_NO_HINT,
   FONT_AUTO_HINT,
   FONT_BYTECODE_HINT
} Font_Hint_Flags;

typedef enum _Font_Rend_Flags
{
   FONT_REND_REGULAR   = 0,
   FONT_REND_SLANT     = (1 << 0),
   FONT_REND_WEIGHT    = (1 << 1),
} Font_Rend_Flags;

struct _RGBA_Font
{
   Eina_List       *fonts;
   Fash_Int        *fash;
   Font_Hint_Flags  hinting;
   int              references;
   LK(lock);
   unsigned char    sizeok : 1;
};


struct _Evas_Font_Dir
{
   Eina_Hash *lookup;
   Eina_List *fonts;
   Eina_List *aliases;
   DATA64     dir_mod_time;
   DATA64     fonts_dir_mod_time;
   DATA64     fonts_alias_mod_time;
};

struct _Evas_Font
{
   struct {
      const char *prop[14];
   } x;
   struct {
      const char *name;
   } simple;
   const char *path;
   char     type;
};

struct _Evas_Font_Alias
{
   const char *alias;
   Evas_Font  *fn;
};

struct _Evas_Font_Description
{
   int ref;
   Eina_Stringshare *name;
   Eina_Stringshare *fallbacks;
   Eina_Stringshare *lang;
   Eina_Stringshare *style;

   Evas_Font_Slant slant;
   Evas_Font_Weight weight;
   Evas_Font_Width width;
   Evas_Font_Spacing spacing;

   Eina_Bool is_new : 1;
};

struct _RGBA_Font_Int
{
   EINA_INLIST;
   RGBA_Font_Source *src;
   Eina_Hash        *kerning;
   Fash_Glyph       *fash;
   unsigned int      size;
   double            scale_factor;
   Eina_Bool         is_resized;
   int               real_size;
   int               max_h;
   int               references;
   int               usage;
   struct {
      FT_Size       size;
#ifdef USE_HARFBUZZ
      void         *hb_font;
#endif
   } ft;
   LK(ft_mutex);
   Font_Hint_Flags  hinting;
   Font_Rend_Flags  wanted_rend; /* The wanted rendering style */
   Font_Rend_Flags  runtime_rend; /* The rendering we need to do on runtime
                                     in order to comply with the wanted_rend. */

   Eina_List       *task;

   int              generation;

   Efl_Text_Font_Bitmap_Scalable bitmap_scalable;

   unsigned char    sizeok : 1;
   unsigned char    inuse : 1;
};

struct _RGBA_Font_Source
{
   const char       *name;
   const char       *file;
   void             *data;
   unsigned int      current_size;
   int               data_size;
   int               references;
   struct {
      int            orig_upem;
      FT_Face        face;
   } ft;
};

/*
 * laziness wins for now. The parts used from the freetpye struct are
 * kept intact to avoid changing the code using it until we know exactly
 * what needs to be changed
 */
struct _RGBA_Font_Glyph_Out
{
   unsigned char *rle;
   struct {
      unsigned char *buffer;
      unsigned short rows;
      unsigned short width;
      unsigned short pitch;
      unsigned short rle_alloc : 1;
      unsigned short no_free_glout : 1;
   } bitmap;
   int rle_size;
};

struct _RGBA_Font_Glyph
{
   FT_UInt         index;
   Evas_Coord      width;
   Evas_Coord      x_bear;
   Evas_Coord      y_bear;
   FT_Glyph        glyph;
   RGBA_Font_Glyph_Out *glyph_out;
   void           *col_dat;
   /* this is a problem - only 1 engine at a time can extend such a font... grrr */
   void           *ext_dat;
   void           (*ext_dat_free) (void *ext_dat);
   RGBA_Font_Int   *fi;
};


/* The tangent of the slant angle we do on runtime. */
#define _EVAS_FONT_SLANT_TAN 0.221694663

/* main */

EVAS_API void              evas_common_font_init                 (void);
EVAS_API void              evas_common_font_shutdown             (void);
EVAS_API void              evas_common_font_font_all_unload      (void);

EVAS_API int               evas_common_font_ascent_get           (RGBA_Font *fn);
EVAS_API int               evas_common_font_descent_get          (RGBA_Font *fn);
EVAS_API int               evas_common_font_max_ascent_get       (RGBA_Font *fn);
EVAS_API int               evas_common_font_max_descent_get      (RGBA_Font *fn);
EVAS_API int               evas_common_font_instance_ascent_get           (RGBA_Font_Int *fi);
EVAS_API int               evas_common_font_instance_descent_get          (RGBA_Font_Int *fi);
EVAS_API int               evas_common_font_instance_max_ascent_get       (RGBA_Font_Int *fi);
EVAS_API int               evas_common_font_instance_max_descent_get      (RGBA_Font_Int *fi);
EVAS_API int               evas_common_font_instance_underline_position_get       (RGBA_Font_Int *fi);
EVAS_API int               evas_common_font_instance_underline_thickness_get      (RGBA_Font_Int *fi);
EVAS_API int               evas_common_font_get_line_advance     (RGBA_Font *fn);
void *evas_common_font_freetype_face_get(RGBA_Font *font); /* XXX: Not EVAS_API on purpose. Not ment to be used in modules. */

EVAS_API RGBA_Font_Glyph  *evas_common_font_int_cache_glyph_get  (RGBA_Font_Int *fi, FT_UInt index);
EVAS_API Eina_Bool         evas_common_font_int_cache_glyph_render(RGBA_Font_Glyph *fg);
EVAS_API FT_UInt           evas_common_get_char_index            (RGBA_Font_Int* fi, Eina_Unicode gl, Eina_Unicode variation_sequence);

/* load */
EVAS_API void              evas_common_font_dpi_set              (int dpi_h, int dpi_v);
EVAS_API RGBA_Font_Source *evas_common_font_source_memory_load   (const char *name, const void *data, int data_size);
EVAS_API RGBA_Font_Source *evas_common_font_source_load          (const char *name);
EVAS_API int               evas_common_font_source_load_complete (RGBA_Font_Source *fs);
EVAS_API RGBA_Font_Source *evas_common_font_source_find          (const char *name);
EVAS_API void              evas_common_font_source_free          (RGBA_Font_Source *fs);
EVAS_API void              evas_common_font_size_use             (RGBA_Font *fn);
EVAS_API RGBA_Font_Int    *evas_common_font_int_load             (const char *name, int size, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API RGBA_Font_Int    *evas_common_font_int_load_init        (RGBA_Font_Int *fn);
EVAS_API RGBA_Font_Int    *evas_common_font_int_load_complete    (RGBA_Font_Int *fi);
EVAS_API RGBA_Font        *evas_common_font_memory_load          (const char *source, const char *name, int size, const void *data, int data_size, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API RGBA_Font        *evas_common_font_load                 (const char *name, int size, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API RGBA_Font        *evas_common_font_add                  (RGBA_Font *fn, const char *name, int size, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API RGBA_Font        *evas_common_font_memory_add           (RGBA_Font *fn, const char *source, const char *name, int size, const void *data, int data_size, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API void              evas_common_font_free                 (RGBA_Font *fn);
EVAS_API void              evas_common_font_int_unref            (RGBA_Font_Int *fi);
EVAS_API void              evas_common_font_hinting_set          (RGBA_Font *fn, Font_Hint_Flags hinting);
EVAS_API Eina_Bool         evas_common_hinting_available         (Font_Hint_Flags hinting);
EVAS_API RGBA_Font        *evas_common_font_memory_hinting_load  (const char *source, const char *name, int size, const void *data, int data_size, Font_Hint_Flags hinting, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API RGBA_Font        *evas_common_font_hinting_load         (const char *name, int size, Font_Hint_Flags hinting, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API RGBA_Font        *evas_common_font_hinting_add          (RGBA_Font *fn, const char *name, int size, Font_Hint_Flags hinting, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API RGBA_Font        *evas_common_font_memory_hinting_add   (RGBA_Font *fn, const char *source, const char *name, int size, const void *data, int data_size, Font_Hint_Flags hinting, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API void              evas_common_font_int_modify_cache_by  (RGBA_Font_Int *fi, int dir);
EVAS_API int               evas_common_font_cache_get            (void);
EVAS_API void              evas_common_font_cache_set            (int size);
EVAS_API void              evas_common_font_flush                (void);
EVAS_API void              evas_common_font_flush_last           (void);
EVAS_API RGBA_Font_Int    *evas_common_font_int_find             (const char *name, int size, Font_Rend_Flags wanted_rend, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
EVAS_API void              evas_common_font_all_clear            (void);
EVAS_API void              evas_common_font_ext_clear            (void);

/* query */

EVAS_API int               evas_common_font_query_kerning        (RGBA_Font_Int* fi, FT_UInt left, FT_UInt right, int* kerning);
EVAS_API void              evas_common_font_query_size           (RGBA_Font *fn, const Evas_Text_Props *intl_props, int *w, int *h);
EVAS_API int               evas_common_font_query_inset          (RGBA_Font *fn, const Evas_Text_Props *text_props);
EVAS_API int               evas_common_font_query_right_inset    (RGBA_Font *fn, const Evas_Text_Props *text_props);
EVAS_API void              evas_common_font_query_advance        (RGBA_Font *fn, const Evas_Text_Props *intl_props, int *h_adv, int *v_adv);
EVAS_API int               evas_common_font_query_char_coords    (RGBA_Font *fn, const Evas_Text_Props *intl_props, int pos, int *cx, int *cy, int *cw, int *ch);
EVAS_API int               evas_common_font_query_pen_coords     (RGBA_Font *fn, const Evas_Text_Props *intl_props, int pos, int *cpen_x, int *cy, int *cadv, int *ch);
EVAS_API int               evas_common_font_query_char_at_coords (RGBA_Font *fn, const Evas_Text_Props *intl_props, int x, int y, int *cx, int *cy, int *cw, int *ch);
EVAS_API int               evas_common_font_query_last_up_to_pos (RGBA_Font *fn, const Evas_Text_Props *intl_props, int x, int y, int width_offset);
EVAS_API int               evas_common_font_query_run_font_end_get(RGBA_Font *fn, RGBA_Font_Int **script_fi, RGBA_Font_Int **cur_fi, Evas_Script_Type script, const Eina_Unicode *text, int run_len);
EVAS_API void              evas_common_font_ascent_descent_get(RGBA_Font *fn, const Evas_Text_Props *text_props, int *ascent, int *descent);

EVAS_API void             *evas_common_font_glyph_compress(void *data, int num_grays, int pixel_mode, int pitch_data, int w, int h, int *size_ret);
EVAS_API DATA8            *evas_common_font_glyph_uncompress(RGBA_Font_Glyph *fg, int *wret, int *hret);
EVAS_API int               evas_common_font_glyph_search         (RGBA_Font *fn, RGBA_Font_Int **fi_ret, Eina_Unicode gl, Eina_Unicode variation_sequence, uint32_t evas_font_search_options);

void evas_common_font_load_init(void);
void evas_common_font_load_shutdown(void);

void evas_font_dir_cache_free(void);
const char *evas_font_dir_cache_find(char *dir, char *font);
Eina_List *evas_font_dir_available_list(const Eina_List *font_paths);
void evas_font_dir_available_list_free(Eina_List *available);
void evas_font_free(void *font);
void evas_fonts_zero_free();
void evas_fonts_zero_pressure();
void evas_font_name_parse(Evas_Font_Description *fdesc, const char *name);
unsigned int evas_font_style_find(const char *start, const char *end, Evas_Font_Style style);
const char *evas_font_style_find_str(int type, Evas_Font_Style style);
Evas_Font_Description *evas_font_desc_new(void);
Evas_Font_Description *evas_font_desc_dup(const Evas_Font_Description *fdesc);
void evas_font_desc_unref(Evas_Font_Description *fdesc);
int evas_font_desc_cmp(const Evas_Font_Description *a, const Evas_Font_Description *b);
Evas_Font_Description *evas_font_desc_ref(Evas_Font_Description *fdesc);
const char *evas_font_lang_normalize(const char *lang);
void * evas_font_load(const Eina_List *font_paths, int hinting, Evas_Font_Description *fdesc, const char *source, Evas_Font_Size size, Efl_Text_Font_Bitmap_Scalable bitmap_scalable);
void evas_font_load_hinting_set(void *font, int hinting);

#endif /* _EVAS_FONT_H */
