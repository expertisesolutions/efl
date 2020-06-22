/* Unit test cases for the filters submodule.
 * For now, limited to Evas_Object_Text, but I prefer to separate these
 * functions from the core text object.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>

#ifdef _WIN32
# include <evil_private.h> /* setenv */
#endif

#define EFL_GFX_FILTER_BETA

#include "../../lib/evas/include/evas_common_private.h"
#include "../../lib/evas/include/evas_filter.h"
#include <Ecore_Evas.h>

#include "evas_suite.h"

#define TEST_FONT_NAME "DejaVuSans,UnDotum"
#define TEST_FONT_SOURCE TESTS_SRC_DIR "/fonts/TestFont.eet"
#define EVAS_DATA_DIR TESTS_SRC_DIR "/../../lib/evas"

#define START_FILTER_TEST() \
   Ecore_Evas *ee; Evas *evas; \
   Evas_Object *to; \
   setenv("EVAS_DATA_DIR", EVAS_DATA_DIR, 1); \
   ee = ecore_evas_buffer_new(1, 1); \
   ecore_evas_show(ee); \
   ecore_evas_manual_render_set(ee, EINA_TRUE); \
   evas = ecore_evas_get(ee); \
   evas_font_hinting_set(evas, EVAS_FONT_HINTING_AUTO); \
   to = evas_object_text_add(evas); \
   evas_object_text_font_set(to, TEST_FONT_NAME, 20); \
   evas_object_text_text_set(to, "Tests"); \
   evas_object_show(to); \
   evas_object_text_font_source_set(to, TEST_FONT_SOURCE); \
   do {} while (0)

#define END_FILTER_TEST() \
   evas_object_del(to); \
   ecore_evas_free(ee); \
   do {} while (0)

#ifdef LITTLE_ENDIAN
#define ALPHA 3
#define RGB0 0
#define RGB3 3
#define RED 2
#define GREEN 1
#define BLUE 0
#else
#define ALPHA 0
#define RGB0 1
#define RGB3 4
#define RED 0
#define GREEN 1
#define BLUE 2
#endif


EFL_START_TEST(evas_filter_parser)
{
   Evas_Filter_Program *pgm;

   // It's practically impossible to test all combinations since the language
   // itself is full featured. Let's just ensure that our main functions exist
   // and that calling them (kinda) works.

   setenv("EVAS_DATA_DIR", EVAS_DATA_DIR, 1);

#define CHECK_FILTER(_a, _v) do { \
   pgm = evas_filter_program_new("evas_suite", EINA_TRUE); \
   fail_if(evas_filter_program_parse(pgm, _a) != _v, "Filter test failed (result should be %s):\n%s", # _v, _a); \
   evas_filter_program_del(pgm); \
   } while (0)
#define CHKGOOD(_a) CHECK_FILTER(_a, EINA_TRUE)
#define CHKBAAD(_a) CHECK_FILTER(_a, EINA_FALSE)

   static const char *good [] = {
      // Single command names
      "blend()",
      "blur()",
      "grow(10)",
      "fill()",
      "curve('0:0-255:255')",
      "padding_set(10)",
      "transform(output)",
      // Buffer modes
      "a = buffer() blend()",
      "a = buffer('alpha') blend()",
      "a = buffer('rgba') blend()",
      "a = buffer{src = 'partname'} blend()",
      // Lua comments
      "blend() -- comment",
      "--comment\n blend()",
      "--[[ long comment\n blend() continues --]] blend()",
      // Combinations
      "blend() blend()",
      "m = buffer() transform({ m, op = 'vflip', src = input, oy = 0 })",
      // All default commands
      "blend ({ src = input, dst = output, ox = 0, oy = 0, color = 'white', fillmode = 'none' })",
      "blur ({ rx = 3, ry = nil, type = 'default', ox = 0, oy = 0, color = 'white', src = input, dst = output })",
      "bump ({ map, azimuth = 135.0, elevation = 45.0, depth = 8.0, specular = 0.0,"
              "color = 'white', compensate = false, src = input, dst = output,"
              "black = 'black', white = 'white', fillmode = 'repeat' })",
      "curve ({ points, interpolation = 'linear', channel = 'rgb', src = input, dst = output })",
      "m = buffer ('rgba') displace ({ m, intensity = 10, flags = 'default', src = input, dst = output, fillmode = 'repeat' })",
      "fill ({ dst = output, color = 'transparent', l = 0, r = 0, t = 0, b = 0 })",
      "grow ({ radius, smooth = true, src = input, dst = output })",
      "m = buffer('alpha') mask ({ m, src = input, dst = output, color = 'white', fillmode = 'repeat' })",
      "transform ({ output, op = 'vflip', src = input, oy = 0 })",
      "padding_set ({ 1, 2, 3, 4 })",
      NULL
   };

   static const char *bad[] = {
      "", // Empty filters should fail
      "function run () blend () end", // run() won't be called, filter is empty
      // All default commands with one extra invalid argument
      "blend ({ src = input, dst = output, ox = 0, oy = 0, color = 'white', fillmode = 'none', invalid = 42 })",
      "blur ({ rx = 3, ry = nil, type = 'default', ox = 0, oy = 0, color = 'white', src = input, dst = output, invalid = 42 })",
      "bump ({ map, azimuth = 135.0, elevation = 45.0, depth = 8.0, specular = 0.0,"
              "color = 'white', compensate = false, src = input, dst = output,"
              "black = 'black', white = 'white', fillmode = 'repeat', invalid = 42 })",
      "curve ({ points, interpolation = 'linear', channel = 'rgb', src = input, dst = output, invalid = 42 })",
      "m = buffer ('rgba') displace ({ m, intensity = 10, flags = 'default', src = input, dst = output, fillmode = 'repeat', invalid = 42 })",
      "fill ({ dst = output, color = 'transparent', l = 0, r = 0, t = 0, b = 0, invalid = 42 })",
      "grow ({ radius, smooth = true, src = input, dst = output, invalid = 42 })",
      "m = buffer('alpha') mask ({ m, src = input, dst = output, color = 'white', fillmode = 'repeat', invalid = 42 })",
      "transform ({ output, op = 'vflip', src = input, oy = 0, invalid = 42 })",
      "padding_set ({ 1, 2, 3, 4, invalid = 42 })",
      // Wrong order of arguments
      "blur({ 1, ry=12, ox=24, 43 })",
      //"blur({ 1, ox=24, 43 })", // not sure if that's possible to fix with lua
      // Some early check failures
      //"m = buffer ('alpha') displace ({ m, intensity = 10, flags = 'default', src = input, dst = output, fillmode = 'repeat' })",
      "m = buffer ('rgba') displace ({ m, intensity = 10, flags = 'invalid', src = input, dst = output, fillmode = 'repeat', invalid = 42 })",
      NULL
   };

   fprintf(stderr, "Evas filters tests: start invalid cases. Ignore the following ERRs.\n");
   fail_if(evas_filter_program_parse(NULL, "blend()"));

   pgm = evas_filter_program_new("evas_suite", EINA_TRUE);
   fail_if(evas_filter_program_parse(pgm, NULL));
   fprintf(stderr, "Evas filters tests: end of invalid cases.\n");
   evas_filter_program_del(pgm);

   for (int k = 0; good[k]; k++)
     CHKGOOD(good[k]);

   fprintf(stderr, "Evas filters tests: start invalid cases. Ignore the following ERRs.\n");
   for (int k = 0; bad[k]; k++)
     CHKBAAD(bad[k]);
   fprintf(stderr, "Evas filters tests: end of invalid cases.\n");

   // All colors -- FIXME: need to check actual color value
   static const char *colors [] = {
      "white",
      "black",
      "red",
      "green",
      "blue",
      "darkblue",
      "yellow",
      "magenta",
      "cyan",
      "orange",
      "purple",
      "brown",
      "maroon",
      "lime",
      "gray",
      "grey",
      "silver",
      "olive",
      "invisible",
      "transparent",
      "#ABC",
      "#ABCF",
      "#AABBCC",
      "#AABBCCFF",
      "WHITE",
      "White"
   };

   for (size_t c = 0; c < sizeof(colors) / sizeof(colors[0]); c++)
     {
        char buf[64];
        sprintf(buf, "blend { color = '%s' }", colors[c]);
        CHKGOOD(buf);
     }

   // fillmodes are parsed when converting from instructions to commands
}
EFL_END_TEST

struct Filter_Test_Case {
   int l, r, t, b;
   const char *code;
   const char *source;
};

/*
 * The following test case are tailored to cover as much code as possible in
 * evas/filters. This is a bit artificial, but at least we'll verify that all
 * filters produce something and not garbage data.
 *
 * NOTE: If the filters fail to link (program_use()), then Evas_Object_Text
 * will fallback to normal rendering. As a consequence, the pixels will look
 * fine, and there won't be an error.
 */
static struct Filter_Test_Case _test_cases[] = {
   // Single filters. Blend can be used to ensure all filters are valid.
   { 0, 0, 0, 0, "blend ()", NULL },
   { 7, 0, 11, 0, "blend ({ ox = -7, oy = -11 })", NULL },
   { 0, 7, 0, 11, "blend ({ ox = 7, oy = 11 })", NULL },
   { 0, 0, 0, 0, "a = buffer ({ 'rgba' }) b = buffer ({ 'alpha' }) blend ({ dst = a }) blend ({ src = a, dst = b }) blend ({ b })", NULL },

   { 5, 5, 7, 7, "blur ({ rx = 5, ry = 7 })", NULL },
   { 0, 0, 5, 5, "blur ({ rx = 0, ry = 5, type = 'box' })", NULL },
   { 0, 0, 5, 5, "a = buffer ({ 'rgba' })  blend ({ dst = a })  blur ({ src = a, rx = 0, ry = 5, type = 'box' })", NULL },
   { 5, 5, 0, 0, "blur ({ rx = 5, ry = 0, type = 'gaussian' })", NULL },
   { 5, 15, 7, 0, "blur ({ rx = 5, ry = 0, ox = 10, oy = -7, type = 'default' })", NULL },

   { 5, 5, 5, 5, "grow ({ 5 })", NULL },

   { 0, 0, 0, 0, "a = buffer ({ 'alpha' }) blend ({ dst = a }) curve ({ '0:0-255:255', src = a,dst = a }) blend ({ a })", NULL },
   { 0, 0, 0, 0, "a = buffer ({ 'alpha' }) blend ({ dst = a }) curve ({ '0:0-255:255',dst = a }) blend ({ a })", NULL },
   { 0, 0, 0, 0, "a = buffer ({ 'rgba' }) blend ({ dst = a }) curve ({ '0:0-255:255', src = a, channel = 'r' })", NULL },
   { 0, 0, 0, 0, "a = buffer ({ 'rgba' }) blend ({ dst = a }) curve ({ '0:128-128:0', src = a, channel = 'rgb', interpolation = 'none' })", NULL },

   { 0, 0, 0, 0, "fill ({ color = 'red' })", NULL },

   { 0, 0, 0, 0, "a = buffer ({ 'alpha' }) blend ({ dst = a }) mask ({ a })", NULL },
   { 0, 0, 0, 0, "a = buffer ({ 'alpha' }) b = buffer ({ 'alpha' }) blend ({ dst = a }) mask ({ a,dst = b }) blend ({ b })", NULL },
   { 0, 0, 0, 0, "a = buffer ({ 'rgba' }) blend ({ dst = a }) mask ({ a })", NULL },
   { 0, 0, 0, 0, "a = buffer ({ 'rgba' }) blend ({ dst = a }) mask ({ mask=input, src = a })", NULL },
   { 0, 0, 0, 0, "a = buffer ({ 'rgba' }) b = buffer ({ 'rgba' }) blend ({ dst = b, color = 'red' }) blend ({ dst = a }) mask ({ a, src = b })", NULL },

   // Note: 'alpha' bump is not tested. Its render quality must be improved first.
   { 0, 0, 0, 0, "a = buffer ({ 'alpha' }) blend ({ dst = a }) bump ({ a })", NULL },
   { 0, 0, 0, 0, "a = buffer ({ 'alpha' }) blend ({ dst = a }) bump ({ a,compensate=yes,specular=10.0 })", NULL },

   { 7, 7, 7, 7, "a = buffer ({ 'alpha' }) b = buffer ({ 'rgba' }) blend ({ dst = b, color = '#330' }) displace ({ map = b, src = input, dst = a, intensity = 7 }) blend ({ a })", NULL },
   { 7, 7, 7, 7, "a = buffer ({ 'rgba' }) b = buffer ({ 'rgba' }) blend ({ dst = b, color = '#330' }) blend ({ dst = a }) displace ({ map = b, src = a, intensity = 7 })", NULL },
   { 7, 7, 7, 7, "a = buffer ({ 'rgba' }) b = buffer ({ 'rgba' }) blend ({ dst = b, color = '#330' }) blend ({ dst = a }) displace ({ map = b, src = a, intensity = 7 })", NULL },
   { 7, 7, 7, 7, "a = buffer ({ 'rgba' }) b = buffer ({ 'rgba' }) blend ({ dst = b, color = '#330' }) blend ({ dst = a }) displace ({ map = b, src = a, intensity = 7, flags = 'default' })", NULL },
   { 7, 7, 7, 7, "a = buffer ({ 'rgba' }) b = buffer ({ 'rgba' }) blend ({ dst = b, color = '#330' }) blend ({ dst = a }) displace ({ map = b, src = a, intensity = 7, flags = 'nearest' })", NULL },
   { 7, 7, 7, 7, "a = buffer ({ 'rgba' }) b = buffer ({ 'rgba' }) blend ({ dst = b, color = '#330' }) blend ({ dst = a }) displace ({ map = b, src = a, intensity = 7, flags = 'smooth' })", NULL },
   { 7, 7, 7, 7, "a = buffer ({ 'rgba' }) b = buffer ({ 'rgba' }) blend ({ dst = b, color = '#330' }) blend ({ dst = a }) displace ({ map = b, src = a, intensity = 7, flags = 'nearest_stretch' })", NULL },
   { 7, 7, 7, 7, "a = buffer ({ 'rgba' }) b = buffer ({ 'rgba' }) blend ({ dst = b, color = '#330' }) blend ({ dst = a }) displace ({ map = b, src = a, intensity = 7, flags = 'smooth_stretch' })", NULL },

   /* Transform filter. TODO: Improve API and test it more */
   { 0, 0, 0, 40, "a = buffer ({ 'alpha' }) transform ({ a, 'vflip', oy = 20 }) blend ({ src = a })", NULL },
   { 0, 0, 40, 0, "a = buffer ({ 'alpha' }) transform ({ a, 'vflip', oy = -20 }) blend ({ src = a })", NULL },
   { 0, 0, 0, 40, "a = buffer ({ 'alpha' }) blend ({ dst = a }) transform ({ a, 'vflip', oy = 20, src = a }) blend ({ src = a })", NULL },

   // Filter combos. TODO: Add some more tricky cases : })
   { 3, 5, 7, 11, "blend ({ ox = -3, oy = 11 })  blend ({ ox = 5, oy = -7 })", NULL },
   { 15, 15, 15, 15, "a = buffer ({ 'rgba' }) grow ({ 10,dst = a }) blur ({ 5, src = a })", NULL },
   { 10, 15, 10, 17, "a = buffer ({ 'alpha' }) blend ({ dst = a, ox = 5, oy = 7 }) blur ({ 10, src = a }) blend ({ ox = -6, oy = -9 })", NULL },
   { 5, 5, 5, 5, "a = buffer ({ 'alpha' }) blur ({ 5,dst = a }) bump ({ a, azimuth = 45.0, color = 'yellow' })", NULL },

   // Proxy tests  ({ RECT as a proxy object })
   { 0, 0, 0, 0, "m = buffer ({ src = 'rect' }) mask ({ m, fillmode = 'none' })", "rect" },
   { 0, 0, 0, 0, "m = buffer ({ src = 'rect' }) mask ({ m, fillmode = 'repeat_x_stretch_y' })", "rect" },
   { 0, 0, 0, 0, "m = buffer ({ src = 'rect' }) mask ({ m, fillmode = 'repeat' })", "rect" },
   { 0, 0, 0, 0, "m = buffer ({ src = 'rect' }) mask ({ m, fillmode = 'stretch' })", "rect" },
   { 0, 0, 0, 0, "m = buffer ({ src = 'rect' }) b = buffer ({ 'rgba' }) blend ({ m,dst = b, fillmode = 'repeat_x_stretch_y' }) blend ()", "rect" },

   // Padding_set
   { 11, 22, 33, 44, "padding_set ({ 11,22,33,44 }) blend ()", NULL }
};

static const int _test_cases_count = sizeof(_test_cases) / sizeof(_test_cases[0]);

EFL_START_TEST(evas_filter_text_padding_test)
{
   START_FILTER_TEST();
   Evas_Coord x, y, w, h, W, H;
   int l, r, t, b;

   evas_object_geometry_get(to, &x, &y, &w, &h);
   //fprintf(stderr, "Geometry: %dx%d+%d,%d\n", w, h, x, y);

   for (int k = 0; k < _test_cases_count; k++)
     {
        struct Filter_Test_Case *tc = &(_test_cases[k]);
        l = r = t = b = 0;

        // Don't test proxy cases here.
        if (tc->source) continue;

        efl_gfx_filter_program_set(to, tc->code, "evas_test_filter");
        evas_object_text_style_pad_get(to, &l, &r, &t, &b);
        evas_object_geometry_get(to, NULL, NULL, &W, &H);
        //fprintf(stderr, "Case %d: %dx%d for padding %d,%d,%d,%d\n", k, W, H, l, r, t, b);

        fail_if((l != tc->l) || (r != tc->r) || (t != tc->t) || (b != tc->b),
                "Failed on invalid padding with '%s'\n", tc->code);
        fail_if((W != (tc->l + tc->r + w)) || (H != (tc->t + tc->b + h)),
                "Failed on invalid geometry with '%s'\n", tc->code);
     }

   END_FILTER_TEST();
}
EFL_END_TEST

/* This will only check that all pixels are valid premultiplied values
 * and that they are not all zero.
 */
static Eina_Bool
_ecore_evas_pixels_check(Ecore_Evas *ee)
{
   const DATA32 *pixels;
   Eina_Bool nonzero = EINA_FALSE;
   int w = 0, h = 0;

   pixels = ecore_evas_buffer_pixels_get(ee);
   if (!pixels) return EINA_FALSE;

   ecore_evas_geometry_get(ee, NULL, NULL, &w, &h);
   if (!w || !h) return EINA_FALSE;

   for (int k = w * h; k; k--, pixels++)
     {
        DATA8 *rgba = (DATA8 *) pixels;

        if (*pixels && (*pixels != 0xFF000000)) nonzero = EINA_TRUE;
        if ((rgba[ALPHA] < rgba[RED])
            || (rgba[ALPHA] < rgba[GREEN])
            || (rgba[ALPHA] < rgba[BLUE]))
          {
             fprintf(stderr, "Invalid RGBA values!\n");
             return EINA_FALSE;
          }
     }

   if (!nonzero) fprintf(stderr, "All pixels are empty!\n");
   return nonzero;
}

EFL_START_TEST(evas_filter_text_render_test)
{
   Evas_Filter_Program *pgm;

   /* FIXME:
    * START_FILTER_TEST should be here instead of in the for loop
    * But there seems to be a problem with ecore_evas_buffer as the second
    * call to pixels_get will return some garbage. Always.
    */

   for (int k = 0; k < _test_cases_count; k++)
     {
        START_FILTER_TEST();

        Evas_Object *rect, *o = NULL, *o2 = NULL;
        Evas_Coord w, h;

        ecore_evas_alpha_set(ee, EINA_TRUE);
        ecore_evas_transparent_set(ee, EINA_TRUE);

        rect = evas_object_rectangle_add(evas);
        evas_object_color_set(rect, 0, 0, 0, 0);
        evas_object_move(rect, 0, 0);
        evas_object_stack_below(rect, to);
        evas_object_show(rect);

        struct Filter_Test_Case *tc = &(_test_cases[k]);
        w = h = 0;

        CHKGOOD(tc->code);
        if (tc->source)
          {
             o = evas_object_rectangle_add(evas);
             evas_object_color_set(o, 0, 255, 0, 255);
             evas_object_move(o, -999, -9999);
             evas_object_resize(o, 10, 10);
             evas_object_show(o);
             efl_gfx_color_set(to, 255, 255, 255, 255);
             efl_gfx_filter_source_set(to, tc->source, o);
             efl_gfx_filter_program_set(to, tc->code, "evas_test_filter");
             o2 = efl_gfx_filter_source_get(to, tc->source);
             fail_if(o != o2);
          }
        else
          {
             efl_gfx_color_set(to, 255, 255, 255, 255);
             efl_gfx_filter_program_set(to, tc->code, "evas_test_filter");
          }

        evas_object_geometry_get(to, NULL, NULL, &w, &h);
        ecore_evas_resize(ee, w, h);
        evas_object_resize(to, w, h);
        evas_object_resize(rect, w, h);

        ecore_evas_manual_render(ee);
        fail_if(!_ecore_evas_pixels_check(ee),
                "Render test failed with: [%dx%d] '%s'", w, h, tc->code);

        evas_object_del(o);
        evas_object_del(rect);
        END_FILTER_TEST();
     }

}
EFL_END_TEST

static inline Eina_Bool
strequal(const char *a, const char *b)
{
   if (a == b) return 1;
   if (!a || !b) return 0;
   return !strcmp(a, b);
}

EFL_START_TEST(evas_filter_state_test)
{
   /* dumb code testing state values */
   static const char *code =
         "c = data and color{data.r, data.g, data.b} or color(state.color)\n"
         "blur { state.scale * 10 * state.pos }\n"
         "blend { ox = state.next.value * 10 }\n"
         "fill { color = c }";

   const unsigned int *pixels;
   const char *s1, *s2;
   double v1, v2, p;

   START_FILTER_TEST();
   ecore_evas_alpha_set(ee, EINA_TRUE);
   ecore_evas_transparent_set(ee, EINA_TRUE);

   evas_object_color_set(to, 255, 0, 0, 255);
   efl_gfx_filter_program_set(to, code, "merf");
   efl_gfx_filter_state_set(to, "state1", 0.0, "state2", 1.0, 0.5);

   /* check pixels */
   ecore_evas_manual_render(ee);
   pixels = ecore_evas_buffer_pixels_get(ee);
   fail_if(!pixels || (*pixels != 0xFFFF0000),
           "state render test failed: %p (%#x)", pixels, pixels ? *pixels : 0);

   efl_gfx_filter_state_get(to, &s1, &v1, &s2, &v2, &p);
   fail_unless(strequal(s1, "state1") && strequal(s2, "state2") && EINA_DBL_EQ(v1, 0.0) && EINA_DBL_EQ(v2, 1.0) && EINA_DBL_EQ(p, 0.5),
               "got: %s %f %s %f %f", s1, v1, s2, v2, p);

   /* data test */
   efl_gfx_filter_data_set(to, "data", "{r=0, g=255, b=0, a=255}", 1);
   ecore_evas_manual_render(ee);
   pixels = ecore_evas_buffer_pixels_get(ee);
   fail_if(!pixels || (*pixels != 0xFF00FF00),
           "state render test failed: %p (%#x)", pixels, pixels ? *pixels : 0);

   END_FILTER_TEST();
}
EFL_END_TEST

void evas_test_filters(TCase *tc)
{
   tcase_add_test(tc, evas_filter_parser);
   tcase_add_test(tc, evas_filter_text_padding_test);
   tcase_add_test(tc, evas_filter_text_render_test);
   tcase_add_test(tc, evas_filter_state_test);
}
