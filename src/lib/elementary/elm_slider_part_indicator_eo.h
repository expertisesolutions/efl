#ifndef _ELM_SLIDER_PART_INDICATOR_EO_H_
#define _ELM_SLIDER_PART_INDICATOR_EO_H_

#ifndef _ELM_SLIDER_PART_INDICATOR_EO_CLASS_TYPE
#define _ELM_SLIDER_PART_INDICATOR_EO_CLASS_TYPE

typedef Eo Elm_Slider_Part_Indicator;

#endif

#ifndef _ELM_SLIDER_PART_INDICATOR_EO_TYPES
#define _ELM_SLIDER_PART_INDICATOR_EO_TYPES


#endif
/** Elementary slider internal part class
 *
 * @ingroup Elm_Slider_Part_Indicator
 */
#define ELM_SLIDER_PART_INDICATOR_CLASS elm_slider_part_indicator_class_get()

ELM_API_WEAK const Efl_Class *elm_slider_part_indicator_class_get(void);

/**
 * @brief Set/Get the visible mode of indicator.
 *
 * @param[in] obj The object.
 * @param[in] mode The indicator visible mode.
 *
 * @ingroup Elm_Slider_Part_Indicator
 */
ELM_API void elm_slider_part_indicator_visible_mode_set(Eo *obj, Elm_Slider_Indicator_Visible_Mode mode);

/**
 * @brief Set/Get the visible mode of indicator.
 *
 * @param[in] obj The object.
 *
 * @return The indicator visible mode.
 *
 * @ingroup Elm_Slider_Part_Indicator
 */
ELM_API Elm_Slider_Indicator_Visible_Mode elm_slider_part_indicator_visible_mode_get(const Eo *obj);

#endif
