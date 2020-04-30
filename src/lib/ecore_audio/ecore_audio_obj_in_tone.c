#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_FEATURES_H
#include <features.h>
#endif

#include <Eo.h>
#include "ecore_audio_private.h"
#include <math.h>

#define MY_CLASS ECORE_AUDIO_IN_TONE_CLASS
#define MY_CLASS_NAME "Ecore_Audio_In_Tone"

struct _Ecore_Audio_In_Tone_Data
{
  int freq;
  int phase;
};

typedef struct _Ecore_Audio_In_Tone_Data Ecore_Audio_In_Tone_Data;

EOLIAN static ssize_t
_ecore_audio_in_tone_ecore_audio_in_read_internal(Eo *eo_obj, Ecore_Audio_In_Tone_Data *obj, void *data, size_t len)
{
  size_t i, remain;
  Ecore_Audio_Input *in_obj = efl_data_scope_get(eo_obj, ECORE_AUDIO_IN_CLASS);

  float *val = data;

  remain = in_obj->length * in_obj->samplerate * 4 - obj->phase * 4;
  if (remain > len)
    remain = len;

  for (i=0; i<remain/4; i++) {
      val[i] = sin(2* M_PI * obj->freq * (obj->phase + i) / in_obj->samplerate);
  }

  obj->phase += i;

  return remain;
}

EOLIAN static double
_ecore_audio_in_tone_ecore_audio_in_seek(Eo *eo_obj, Ecore_Audio_In_Tone_Data *obj, double offs, int mode)
{
  int tmp;
  Ecore_Audio_Input *in_obj = efl_data_scope_get(eo_obj, ECORE_AUDIO_IN_CLASS);

  switch (mode) {
    case SEEK_SET:
      tmp = offs * in_obj->samplerate;
      break;
    case SEEK_CUR:
      tmp = obj->phase + offs * in_obj->samplerate;
      break;
    case SEEK_END:
      tmp = (in_obj->length + offs) * in_obj->samplerate;
      break;
    default:
      goto err;
  }
  if ((tmp < 0) || (tmp > in_obj->length * in_obj->samplerate))
    goto err;

  obj->phase = tmp;

  return (double)obj->phase / in_obj->samplerate;
err:
  return -1.0;
}

EOLIAN static void
_ecore_audio_in_tone_ecore_audio_in_length_set(Eo *eo_obj, Ecore_Audio_In_Tone_Data *_pd EINA_UNUSED, double length)
{
  Ecore_Audio_Input *in_obj = efl_data_scope_get(eo_obj, ECORE_AUDIO_IN_CLASS);
  in_obj->length = length;
}

EOLIAN static void
_ecore_audio_in_tone_efl_object_key_data_set(Eo *eo_obj, Ecore_Audio_In_Tone_Data *obj, const char *key, const void *val)
{
  if (!key) return;

  if (!strcmp(key, ECORE_AUDIO_ATTR_TONE_FREQ)) {
      obj->freq = *(int *)val;
  } else {
      efl_key_data_set(efl_super(eo_obj, MY_CLASS), key, val);
  }

}

EOLIAN static void *
_ecore_audio_in_tone_efl_object_key_data_get(Eo *eo_obj, Ecore_Audio_In_Tone_Data *obj, const char *key)
{
  if (!strcmp(key, ECORE_AUDIO_ATTR_TONE_FREQ)) {
      return (void *) (intptr_t) obj->freq;
  } else {
      return efl_key_data_get(efl_super(eo_obj, MY_CLASS), key);
  }
}

EOLIAN static Eo *
_ecore_audio_in_tone_efl_object_constructor(Eo *eo_obj, Ecore_Audio_In_Tone_Data *obj)
{
  Ecore_Audio_Input *in_obj = efl_data_scope_get(eo_obj, ECORE_AUDIO_IN_CLASS);

  eo_obj = efl_constructor(efl_super(eo_obj, MY_CLASS));

  in_obj->channels = 1;
  in_obj->samplerate = 44100;
  in_obj->length = 1;
  in_obj->seekable = EINA_TRUE;

  obj->freq = 1000;

  return eo_obj;
}

#define ECORE_AUDIO_IN_TONE_EXTRA_OPS \
   EFL_OBJECT_OP_FUNC(efl_key_data_set, _ecore_audio_in_tone_efl_object_key_data_set), \
   EFL_OBJECT_OP_FUNC(efl_key_data_get, _ecore_audio_in_tone_efl_object_key_data_get)

#include "ecore_audio_in_tone.eo.c"
