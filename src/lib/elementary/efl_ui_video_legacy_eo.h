#ifndef _EFL_UI_VIDEO_LEGACY_EO_H_
#define _EFL_UI_VIDEO_LEGACY_EO_H_

#ifndef _EFL_UI_VIDEO_LEGACY_EO_CLASS_TYPE
#define _EFL_UI_VIDEO_LEGACY_EO_CLASS_TYPE

typedef Eo Efl_Ui_Video_Legacy;

#endif

#ifndef _EFL_UI_VIDEO_LEGACY_EO_TYPES
#define _EFL_UI_VIDEO_LEGACY_EO_TYPES


#endif
#ifdef EFL_BETA_API_SUPPORT
/** Efl UI video class
 *
 * @ingroup Efl_Ui_Video_Legacy
 */
#define EFL_UI_VIDEO_LEGACY_CLASS efl_ui_video_legacy_class_get()

ELM_API ELM_API_WEAK const Efl_Class *efl_ui_video_legacy_class_get(void);
#endif /* EFL_BETA_API_SUPPORT */

#endif
