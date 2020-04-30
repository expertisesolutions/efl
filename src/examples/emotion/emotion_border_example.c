//Compile with:
// gcc -o emotion_border_example emotion_border_example.c `pkg-config --libs --cflags emotion evas ecore ecore-evas eina eo`
#ifndef EFL_BETA_API_SUPPORT
# define EFL_BETA_API_SUPPORT
#endif

#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Evas.h>
#include <Emotion.h>
#include <stdio.h>
#include <string.h>

#define WIDTH  (320)
#define HEIGHT (240)

static Eina_List *filenames = NULL;
static Eina_List *curfile = NULL;

static void
_playback_started_cb(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
    printf("Emotion object started playback.\n");
}

static Evas_Object *
_create_emotion_object(Evas *e)
{
   Evas_Object *em = emotion_object_add(e);

   emotion_object_init(em, "gstreamer1");

   efl_event_callback_add(em, EFL_CANVAS_VIDEO_EVENT_PLAYBACK_START, _playback_started_cb, NULL);

   return em;
}

static void
_on_key_down(void *data, Evas *e EINA_UNUSED, Evas_Object *o EINA_UNUSED, void *event_info)
{
   Evas_Event_Key_Down *ev = event_info;
   Evas_Object *em = data;

   if (!strcmp(ev->key, "Return"))
     {
	emotion_object_play_set(em, EINA_TRUE);
     }
   else if (!strcmp(ev->key, "space"))
     {
	emotion_object_play_set(em, EINA_FALSE);
     }
   else if (!strcmp(ev->key, "Escape"))
     {
	ecore_main_loop_quit();
     }
   else if (!strcmp(ev->key, "n"))
     {
	const char *file;
	if (!curfile)
	  curfile = filenames;
	else
	  curfile = eina_list_next(curfile);
	file = eina_list_data_get(curfile);
	fprintf(stderr, "playing next file: %s\n", file);
	emotion_object_file_set(em, file);
     }
   else if (!strcmp(ev->key, "p"))
     {
	const char *file;
	if (!curfile)
	  curfile = eina_list_last(filenames);
	else
	  curfile = eina_list_prev(curfile);
	file = eina_list_data_get(curfile);
	fprintf(stderr, "playing next file: %s\n", file);
	emotion_object_file_set(em, file);
     }
   else if (!strcmp(ev->key, "b"))
     {
	emotion_object_border_set(em, 0, 0, 50, 50);
     }
   else if (!strcmp(ev->key, "0"))
     {
	emotion_object_keep_aspect_set(em, EMOTION_ASPECT_KEEP_NONE);
     }
   else if (!strcmp(ev->key, "w"))
     {
	emotion_object_keep_aspect_set(em, EMOTION_ASPECT_KEEP_WIDTH);
     }
   else if (!strcmp(ev->key, "h"))
     {
	emotion_object_keep_aspect_set(em, EMOTION_ASPECT_KEEP_HEIGHT);
     }
   else if (!strcmp(ev->key, "2"))
     {
	emotion_object_keep_aspect_set(em, EMOTION_ASPECT_KEEP_BOTH);
     }
   else if (!strcmp(ev->key, "c"))
     {
	emotion_object_keep_aspect_set(em, EMOTION_ASPECT_CROP);
     }
   else
     {
	fprintf(stderr, "unhandled key: %s\n", ev->key);
     }
}

static void
_frame_decode_cb(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   // fprintf(stderr, "smartcb: frame_decode\n");
}

static void
_length_change_cb(void *data EINA_UNUSED, const Efl_Event *ev)
{
   fprintf(stderr, "smartcb: length_change: %0.3f\n", emotion_object_play_length_get(ev->object));
}

static void
_position_update_cb(void *data EINA_UNUSED, const Efl_Event *ev)
{
   fprintf(stderr, "smartcb: position_update: %0.3f\n", emotion_object_position_get(ev->object));
}

static void
_progress_change_cb(void *data EINA_UNUSED, const Efl_Event *ev)
{
   fprintf(stderr, "smartcb: progress_change: %0.3f, %s\n",
	   emotion_object_progress_status_get(ev->object),
	   emotion_object_progress_info_get(ev->object));
}

static void
_frame_resize_cb(void *data EINA_UNUSED, const Efl_Event *ev)
{
   int w, h;
   emotion_object_size_get(ev->object, &w, &h);
   fprintf(stderr, "smartcb: frame_resize: %dx%d\n", w, h);
}

static void /* adjust canvas' contents on resizes */
_canvas_resize_cb(Ecore_Evas *ee)
{
   int w, h;
   Evas_Object *bg, *em;

   ecore_evas_geometry_get(ee, NULL, NULL, &w, &h);

   bg = ecore_evas_data_get(ee, "bg");
   em = ecore_evas_data_get(ee, "emotion");

   evas_object_resize(bg, w, h);
   evas_object_move(em, 10, 10);
   evas_object_resize(em, w - 20, h - 20);
}

EFL_CALLBACKS_ARRAY_DEFINE(emotion_object_example_callbacks,
       { EFL_CANVAS_VIDEO_EVENT_FRAME_DECODE, _frame_decode_cb },
       { EFL_CANVAS_VIDEO_EVENT_LENGTH_CHANGE, _length_change_cb },
       { EFL_CANVAS_VIDEO_EVENT_POSITION_CHANGE, _position_update_cb },
       { EFL_CANVAS_VIDEO_EVENT_PROGRESS_CHANGE, _progress_change_cb },
       { EFL_CANVAS_VIDEO_EVENT_FRAME_RESIZE, _frame_resize_cb });

int
main(int argc, const char *argv[])
{
   Ecore_Evas *ee;
   Evas *e;
   Evas_Object *bg, *em;
   int i;

   if (argc < 2)
     {
	printf("One argument is necessary. Usage:\n");
	printf("\t%s <filename>\n", argv[0]);
     }

   eina_init();
   for (i = 1; i < argc; i++)
     filenames = eina_list_append(filenames, eina_stringshare_add(argv[i]));

   curfile = filenames;

   if (!ecore_evas_init())
     return EXIT_FAILURE;

   /* this will give you a window with an Evas canvas under the first
    * engine available */
   ee = ecore_evas_new(NULL, 10, 10, WIDTH, HEIGHT, NULL);
   if (!ee)
     goto error;

   ecore_evas_callback_resize_set(ee, _canvas_resize_cb);

   ecore_evas_show(ee);

   /* the canvas pointer, de facto */
   e = ecore_evas_get(ee);

   /* adding a background to this example */
   bg = evas_object_rectangle_add(e);
   evas_object_name_set(bg, "our dear rectangle");
   evas_object_color_set(bg, 255, 0, 0, 255); /* white bg */
   evas_object_move(bg, 0, 0); /* at canvas' origin */
   evas_object_resize(bg, WIDTH, HEIGHT); /* covers full canvas */
   evas_object_show(bg);

   ecore_evas_data_set(ee, "bg", bg);

   /* Creating the emotion object */
   em = _create_emotion_object(e);
   emotion_object_file_set(em, eina_list_data_get(curfile));
   evas_object_move(em, 10, 10);
   evas_object_resize(em, WIDTH, HEIGHT);
   evas_object_resize(em, WIDTH - 20, HEIGHT - 20);
   emotion_object_keep_aspect_set(em, EMOTION_ASPECT_KEEP_BOTH);
   emotion_object_bg_color_set(em, 0, 128, 0, 255);
   evas_object_show(em);

   ecore_evas_data_set(ee, "emotion", em);

   efl_event_callback_array_add(em, emotion_object_example_callbacks(), NULL);

   evas_object_event_callback_add(bg, EVAS_CALLBACK_KEY_DOWN, _on_key_down, em);
   evas_object_focus_set(bg, EINA_TRUE);

   emotion_object_play_set(em, EINA_TRUE);

   ecore_main_loop_begin();

   ecore_evas_free(ee);
   ecore_evas_shutdown();
   return 0;

error:
   fprintf(stderr, "error: Requires at least one Evas engine built and linked"
                   " to ecore-evas for this example to run properly.\n");

   EINA_LIST_FREE(filenames, curfile)
      eina_stringshare_del(eina_list_data_get(curfile));

   ecore_evas_shutdown();
   eina_shutdown();
   return -1;
}
