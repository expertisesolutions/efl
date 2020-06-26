#ifndef _EFL_UI_FRAME_LEGACY_EO_H_
#define _EFL_UI_FRAME_LEGACY_EO_H_

#ifndef _EFL_UI_FRAME_LEGACY_EO_CLASS_TYPE
#define _EFL_UI_FRAME_LEGACY_EO_CLASS_TYPE

typedef Eo Efl_Ui_Frame_Legacy;

#endif

#ifndef _EFL_UI_FRAME_LEGACY_EO_TYPES
#define _EFL_UI_FRAME_LEGACY_EO_TYPES


#endif
#ifdef EFL_BETA_API_SUPPORT
/**
 * @brief Frame widget
 *
 * The Frame widget allows for collapsing and expanding the content widget by
 * clicking on the frame label. the label and content can be set using text_set
 * and content_set api.
 *
 * @ingroup Efl_Ui_Frame_Legacy
 */
#define EFL_UI_FRAME_LEGACY_CLASS efl_ui_frame_legacy_class_get()

ELM_API ELM_API_WEAK const Efl_Class *efl_ui_frame_legacy_class_get(void);
#endif /* EFL_BETA_API_SUPPORT */

#endif
