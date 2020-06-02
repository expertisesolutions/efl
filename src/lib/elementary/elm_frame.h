/**
 * @defgroup Elm_Frame Frame
 * @ingroup Elementary
 *
 * @image html frame_inheritance_tree.png
 * @image latex frame_inheritance_tree.eps
 *
 * @image html img/widget/frame/preview-00.png
 * @image latex img/widget/frame/preview-00.eps
 *
 * @brief Frame is a widget that holds some content and has a title.
 *
 * The default look is a frame with a title, but Frame supports multiple
 * styles:
 * @li default
 * @li pad_small
 * @li pad_medium
 * @li pad_large
 * @li pad_huge
 * @li outdent_top
 * @li outdent_bottom
 *
 * Of all this styles only default shows the title.
 *
 * This widget inherits from the @ref Elm_Layout one, so that all the
 * functions acting on it also work for frame objects.
 *
 * This widget emits the following signals, besides the ones sent from
 * @ref Elm_Layout :
 * - @c "clicked" - The user has clicked the frame's label
 * - @c "language,changed" - the program's language changed (since 1.9)
 *
 * Default content parts of the frame widget that you can use for are:
 * @li "default" - A content of the frame
 *
 * Default text parts of the frame widget that you can use for are:
 * @li "default" - A label of the frame
 *
 * Supported elm_object common APIs.
 * @li @ref elm_object_part_text_set
 * @li @ref elm_object_part_text_get
 * @li @ref elm_object_part_content_set
 * @li @ref elm_object_part_content_get
 * @li @ref elm_object_part_content_unset
 *
 * For a detailed example see the @ref tutorial_frame.
 *
 * @{
 */

#ifndef EFL_NOLEGACY_API_SUPPORT
#include "efl_ui_frame_legacy.h"
#endif
/**
 * @}
 */
