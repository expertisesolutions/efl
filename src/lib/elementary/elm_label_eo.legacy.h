#ifndef _ELM_LABEL_EO_LEGACY_H_
#define _ELM_LABEL_EO_LEGACY_H_

#ifndef _ELM_LABEL_EO_CLASS_TYPE
#define _ELM_LABEL_EO_CLASS_TYPE

typedef Eo Elm_Label;

#endif

#ifndef _ELM_LABEL_EO_TYPES
#define _ELM_LABEL_EO_TYPES

/** Slide mode of a label widget
 *
 * @ingroup Elm_Label
 */
typedef enum
{
  ELM_LABEL_SLIDE_MODE_NONE = 0, /**< No slide effect. */
  ELM_LABEL_SLIDE_MODE_AUTO, /**< Slide only if the label area is bigger than
                              * the text width length. */
  ELM_LABEL_SLIDE_MODE_ALWAYS /**< Slide always. */
} Elm_Label_Slide_Mode;


#endif

/**
 * @brief Control wrap width of the label
 *
 * This function sets the maximum width size hint of the label.
 *
 * @warning This is only relevant if the label is inside a container.
 *
 * @param[in] obj The object.
 * @param[in] w The wrap width in pixels at a minimum where words need to wrap
 *
 * @ingroup Elm_Label_Group
 */
ELM_API void elm_label_wrap_width_set(Elm_Label *obj, int w);

/**
 * @brief Control wrap width of the label
 *
 * This function sets the maximum width size hint of the label.
 *
 * @warning This is only relevant if the label is inside a container.
 *
 * @param[in] obj The object.
 *
 * @return The wrap width in pixels at a minimum where words need to wrap
 *
 * @ingroup Elm_Label_Group
 */
ELM_API int elm_label_wrap_width_get(const Elm_Label *obj);

/**
 * @brief Control the slide speed of the label
 *
 * @note If you set the duration of the slide using
 * @ref elm_label_slide_duration_set you cannot get the correct speed using
 * this function until the label is actually rendered and resized.
 *
 * See @ref elm_label_slide_duration_set
 *
 * @param[in] obj The object.
 * @param[in] speed The speed of the slide animation in px per seconds
 *
 * @ingroup Elm_Label_Group
 */
ELM_API void elm_label_slide_speed_set(Elm_Label *obj, double speed);

/**
 * @brief Control the slide speed of the label
 *
 * @note If you set the duration of the slide using
 * @ref elm_label_slide_duration_set you cannot get the correct speed using
 * this function until the label is actually rendered and resized.
 *
 * See @ref elm_label_slide_duration_set
 *
 * @param[in] obj The object.
 *
 * @return The speed of the slide animation in px per seconds
 *
 * @ingroup Elm_Label_Group
 */
ELM_API double elm_label_slide_speed_get(const Elm_Label *obj);

/**
 * @brief Control the slide mode of the label widget.
 *
 * By default, slide mode is none. Possible values for @c mode are:
 * @ref ELM_LABEL_SLIDE_MODE_NONE - no slide effect
 * @ref ELM_LABEL_SLIDE_MODE_AUTO - slide only if the label area is bigger than
 * the text width length @ref ELM_LABEL_SLIDE_MODE_ALWAYS - slide always
 *
 * @warning @ref ELM_LABEL_SLIDE_MODE_AUTO, @ref ELM_LABEL_SLIDE_MODE_ALWAYS
 * only work with the themes "slide_short", "slide_long" and "slide_bounce".
 * Warning: @ref ELM_LABEL_SLIDE_MODE_AUTO, @ref ELM_LABEL_SLIDE_MODE_ALWAYS
 * don't work if the line wrap(elm_label_line_wrap_set()) or
 * ellipsis(elm_label_ellipsis_set()) is set.
 *
 * @param[in] obj The object.
 * @param[in] mode The slide mode
 *
 * @since 1.8
 *
 * @ingroup Elm_Label_Group
 */
ELM_API void elm_label_slide_mode_set(Elm_Label *obj, Elm_Label_Slide_Mode mode);

/**
 * @brief Control the slide mode of the label widget.
 *
 * By default, slide mode is none. Possible values for @c mode are:
 * @ref ELM_LABEL_SLIDE_MODE_NONE - no slide effect
 * @ref ELM_LABEL_SLIDE_MODE_AUTO - slide only if the label area is bigger than
 * the text width length @ref ELM_LABEL_SLIDE_MODE_ALWAYS - slide always
 *
 * @warning @ref ELM_LABEL_SLIDE_MODE_AUTO, @ref ELM_LABEL_SLIDE_MODE_ALWAYS
 * only work with the themes "slide_short", "slide_long" and "slide_bounce".
 * Warning: @ref ELM_LABEL_SLIDE_MODE_AUTO, @ref ELM_LABEL_SLIDE_MODE_ALWAYS
 * don't work if the line wrap(elm_label_line_wrap_set()) or
 * ellipsis(elm_label_ellipsis_set()) is set.
 *
 * @param[in] obj The object.
 *
 * @return The slide mode
 *
 * @since 1.8
 *
 * @ingroup Elm_Label_Group
 */
ELM_API Elm_Label_Slide_Mode elm_label_slide_mode_get(const Elm_Label *obj);

/**
 * @brief Control the slide duration of the label
 *
 * @note If you set the speed of the slide using @ref elm_label_slide_speed_set
 * you cannot get the correct duration using this function until the label is
 * actually rendered and resized.
 *
 * @param[in] obj The object.
 * @param[in] duration The duration in seconds in moving text from slide begin
 * position to slide end position
 *
 * @ingroup Elm_Label_Group
 */
ELM_API void elm_label_slide_duration_set(Elm_Label *obj, double duration);

/**
 * @brief Control the slide duration of the label
 *
 * @note If you set the speed of the slide using @ref elm_label_slide_speed_set
 * you cannot get the correct duration using this function until the label is
 * actually rendered and resized.
 *
 * @param[in] obj The object.
 *
 * @return The duration in seconds in moving text from slide begin position to
 * slide end position
 *
 * @ingroup Elm_Label_Group
 */
ELM_API double elm_label_slide_duration_get(const Elm_Label *obj);

/**
 * @brief Control the wrapping behavior of the label
 *
 * By default no wrapping is done. Possible values for @c wrap are:
 * @c ELM_WRAP_NONE - No wrapping @c ELM_WRAP_CHAR - wrap between characters
 * @c ELM_WRAP_WORD - wrap between words @c ELM_WRAP_MIXED - Word wrap, and if
 * that fails, char wrap
 *
 * @param[in] obj The object.
 * @param[in] wrap To wrap text or not
 *
 * @ingroup Elm_Label_Group
 */
ELM_API void elm_label_line_wrap_set(Elm_Label *obj, Elm_Wrap_Type wrap);

/**
 * @brief Control the wrapping behavior of the label
 *
 * By default no wrapping is done. Possible values for @c wrap are:
 * @c ELM_WRAP_NONE - No wrapping @c ELM_WRAP_CHAR - wrap between characters
 * @c ELM_WRAP_WORD - wrap between words @c ELM_WRAP_MIXED - Word wrap, and if
 * that fails, char wrap
 *
 * @param[in] obj The object.
 *
 * @return To wrap text or not
 *
 * @ingroup Elm_Label_Group
 */
ELM_API Elm_Wrap_Type elm_label_line_wrap_get(const Elm_Label *obj);

/**
 * @brief Control the ellipsis behavior of the label
 *
 * If set to true and the text doesn't fit in the label an ellipsis("...") will
 * be shown at the end of the widget.
 *
 * Warning This doesn't work with slide(elm_label_slide_set()) or if the chosen
 * wrap method was @c ELM_WRAP_WORD.
 *
 * @param[in] obj The object.
 * @param[in] ellipsis To ellipsis text or not
 *
 * @ingroup Elm_Label_Group
 */
ELM_API void elm_label_ellipsis_set(Elm_Label *obj, Eina_Bool ellipsis);

/**
 * @brief Control the ellipsis behavior of the label
 *
 * If set to true and the text doesn't fit in the label an ellipsis("...") will
 * be shown at the end of the widget.
 *
 * Warning This doesn't work with slide(elm_label_slide_set()) or if the chosen
 * wrap method was @c ELM_WRAP_WORD.
 *
 * @param[in] obj The object.
 *
 * @return To ellipsis text or not
 *
 * @ingroup Elm_Label_Group
 */
ELM_API Eina_Bool elm_label_ellipsis_get(const Elm_Label *obj);

/**
 * @brief Start slide effect.
 *
 * See @ref elm_label_slide_mode_set.
 * @param[in] obj The object.
 *
 * @ingroup Elm_Label_Group
 */
ELM_API void elm_label_slide_go(Elm_Label *obj);

#endif
