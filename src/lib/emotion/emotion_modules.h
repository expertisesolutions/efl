#ifndef _EMOTION_MODULE_H_
#define _EMOTION_MODULE_H_ 1

#include "Emotion.h"

#ifdef EMOTION_API
# undef EMOTION_API
#endif

#ifdef _WIN32
# ifdef EFL_BUILD
#  ifdef DLL_EXPORT
#   define EMOTION_API __declspec(dllexport)
#  else
#   define EMOTION_API
#  endif
# else
#  define EMOTION_API __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EMOTION_API __attribute__ ((visibility("default")))
#  else
#   define EMOTION_API
#  endif
# else
#  define EMOTION_API
# endif
#endif

#define META_TRACK_TITLE 1
#define META_TRACK_ARTIST 2
#define META_TRACK_GENRE 3
#define META_TRACK_COMMENT 4
#define META_TRACK_ALBUM 5
#define META_TRACK_YEAR 6
#define META_TRACK_DISCID 7
#define META_TRACK_COUNT 8

typedef enum _Emotion_Format Emotion_Format;
typedef struct _Emotion_Engine Emotion_Engine;
typedef struct _Emotion_Module_Options Emotion_Module_Options;
typedef struct _Eina_Emotion_Plugins Eina_Emotion_Plugins;

enum _Emotion_Format
{
   EMOTION_FORMAT_NONE,
   EMOTION_FORMAT_I420,
   EMOTION_FORMAT_YV12,
   EMOTION_FORMAT_YUY2,     /* unused for now since evas does not support yuy2 format */
   EMOTION_FORMAT_BGRA
};

struct _Emotion_Module_Options
{
   Eina_Bool no_video : 1;
   Eina_Bool no_audio : 1;
};

struct _Emotion_Engine
{
#define EMOTION_ENGINE_API_VERSION (1U)
   unsigned       version;

#define EMOTION_ENGINE_PRIORITY_DEFAULT (50)
   int            priority; /* default priority, may be overwritten by user. Try to keep from 0-100. */

   const char    *name;
   void          *(*add)(const Emotion_Engine *api, Evas_Object *obj, const Emotion_Module_Options *opts);
   void           (*del)(void *ef);

   Eina_Bool      (*file_open) (void *ef, const char *file);
   void           (*file_close) (void *ef);
   void           (*play) (void *ef, double pos);
   void           (*stop) (void *ef);
   void           (*size_get) (void *ef, int *w, int *h);
   void           (*pos_set) (void *ef, double pos);
   double         (*len_get) (void *ef);
   double         (*buffer_size_get) (void *ef);
   int            (*fps_num_get) (void *ef);
   int            (*fps_den_get) (void *ef);
   double         (*fps_get) (void *ef);
   double         (*pos_get) (void *ef);
   void           (*vis_set) (void *ef, Emotion_Vis vis);
   Emotion_Vis    (*vis_get) (void *ef);
   Eina_Bool      (*vis_supported) (void *ef, Emotion_Vis vis);
   double         (*ratio_get) (void *ef);
   int            (*video_handled) (void *ef);
   int            (*audio_handled) (void *ef);
   int            (*seekable) (void *ef);
   void           (*frame_done) (void *ef);
   Emotion_Format (*format_get) (void *ef);
   void           (*video_data_size_get) (void *ef, int *w, int *h);
   int            (*yuv_rows_get) (void *ef, int w, int h, unsigned char **yrows, unsigned char **urows, unsigned char **vrows);
   int            (*bgra_data_get) (void *ef, unsigned char **bgra_data);
   void           (*event_feed) (void *ef, int event);
   void           (*event_mouse_button_feed) (void *ef, int button, int x, int y);
   void           (*event_mouse_move_feed) (void *ef, int x, int y);
   int            (*video_channel_count) (void *ef);
   void           (*video_channel_set) (void *ef, int channel);
   int            (*video_channel_get) (void *ef);
   void           (*video_subtitle_file_set) (void *ef, const char *filepath);
   const char *   (*video_subtitle_file_get) (void *ef);
   const char *   (*video_channel_name_get) (void *ef, int channel);
   void           (*video_channel_mute_set) (void *ef, int mute);
   int            (*video_channel_mute_get) (void *ef);
   int            (*audio_channel_count) (void *ef);
   void           (*audio_channel_set) (void *ef, int channel);
   int            (*audio_channel_get) (void *ef);
   const char *   (*audio_channel_name_get) (void *ef, int channel);
   void           (*audio_channel_mute_set) (void *ef, int mute);
   int            (*audio_channel_mute_get) (void *ef);
   void           (*audio_channel_volume_set) (void *ef, double vol);
   double         (*audio_channel_volume_get) (void *ef);
   int            (*spu_channel_count) (void *ef);
   void           (*spu_channel_set) (void *ef, int channel);
   int            (*spu_channel_get) (void *ef);
   const char *   (*spu_channel_name_get) (void *ef, int channel);
   void           (*spu_channel_mute_set) (void *ef, int mute);
   int            (*spu_channel_mute_get) (void *ef);
   int            (*chapter_count) (void *ef);
   void           (*chapter_set) (void *ef, int chapter);
   int            (*chapter_get) (void *ef);
   const char *   (*chapter_name_get) (void *ef, int chapter);
   void           (*speed_set) (void *ef, double speed);
   double         (*speed_get) (void *ef);
   int            (*eject) (void *ef);
   const char *   (*meta_get) (void *ef, int meta);
   void           (*priority_set) (void *ef, Eina_Bool priority);
   Eina_Bool      (*priority_get) (void *ef);
   void       *   (*meta_artwork_get)(void *ef, Evas_Object *img, const char *path, Emotion_Artwork_Info type);
};

EMOTION_API void *_emotion_video_get(const Evas_Object *obj);
EMOTION_API void  _emotion_frame_new(Evas_Object *obj);
EMOTION_API void  _emotion_video_pos_update(Evas_Object *obj, double pos, double len);
EMOTION_API void  _emotion_frame_resize(Evas_Object *obj, int w, int h, double ratio);
EMOTION_API void  _emotion_frame_refill(Evas_Object *obj, double w, double h);
EMOTION_API void  _emotion_decode_stop(Evas_Object *obj);
EMOTION_API void  _emotion_open_done(Evas_Object *obj);
EMOTION_API void  _emotion_playback_started(Evas_Object *obj);
EMOTION_API void  _emotion_playback_finished(Evas_Object *obj);
EMOTION_API void  _emotion_audio_level_change(Evas_Object *obj);
EMOTION_API void  _emotion_channels_change(Evas_Object *obj);
EMOTION_API void  _emotion_title_set(Evas_Object *obj, char *title);
EMOTION_API void  _emotion_progress_set(Evas_Object *obj, char *info, double stat);
EMOTION_API void  _emotion_file_ref_set(Evas_Object *obj, const char *file, int num);
EMOTION_API void  _emotion_spu_button_num_set(Evas_Object *obj, int num);
EMOTION_API void  _emotion_spu_button_set(Evas_Object *obj, int button);
EMOTION_API void  _emotion_seek_done(Evas_Object *obj);
EMOTION_API void  _emotion_image_reset(Evas_Object *obj);

EMOTION_API void _emotion_pending_object_ref(void);
EMOTION_API void _emotion_pending_object_unref(void);

EMOTION_API void _emotion_pending_ecore_begin(void);
EMOTION_API void _emotion_pending_ecore_end(void);


EMOTION_API const char *emotion_webcam_custom_get(const char *device);

EMOTION_API Eina_Bool _emotion_module_register(const Emotion_Engine *api);
EMOTION_API Eina_Bool _emotion_module_unregister(const Emotion_Engine *api);

#undef EMOTION_API
#define EMOTION_API

#endif
