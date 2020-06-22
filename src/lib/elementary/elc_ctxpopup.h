/**
 * @defgroup Elm_Ctxpopup Ctxpopup
 * @ingroup Elementary
 *
 * @image html ctxpopup_inheritance_tree.png
 * @image latex ctxpopup_inheritance_tree.eps
 *
 * @image html img/widget/ctxpopup/preview-00.png
 * @image latex img/widget/ctxpopup/preview-00.eps
 *
 * @brief Context popup widget.
 *
 * A ctxpopup is a widget that, when shown, pops up a list of items.
 * It automatically chooses an area inside its parent object's view
 * (set via elm_ctxpopup_add() and elm_ctxpopup_hover_parent_set()) to
 * optimally fit into it. In the default theme, it will also point an
 * arrow to it's top left position at the time one shows it. Ctxpopup
 * items have a label and/or an icon. It is intended for a small
 * number of items (hence the use of list, not genlist).
 *
 * This widget inherits from the Layout one, so that all the
 * functions acting on it also work for context popup objects (since 1.8).
 *
 * This widget emits the following signals, besides the ones sent from
 * @ref Elm_Layout :
 * - @c "dismissed" - This is called when 1. the outside of ctxpopup was clicked
 * or 2. its parent area is changed or 3. the language is changed and also when
 * 4. the parent object is resized due to the window rotation. Then ctxpopup is
 * dismissed.
 * - @c "language,changed" - This is called when the program's language is
 * changed.
 * - @c "focused" - When the ctxpopup has received focus. (since 1.8)
 * - @c "unfocused" - When the ctxpopup has lost focus. (since 1.8)
 * - @c "geometry,update" - When the ctxpopup geometry is updated. This smart event passes the ctxpopup geometry information with the given event_info argument which type is Evas_Coord_Rectangle. (since 1.18)

 *
 * Default content parts of the ctxpopup widget that you can use for are:
 * @li "default" - A content of the ctxpopup
 *
 * Default content parts of the ctxpopup items that you can use for are:
 * @li "icon" - An icon in the title area
 *
 * Default text parts of the ctxpopup items that you can use for are:
 * @li "default" - A title label in the title area
 *
 * Supported elm_object common APIs.
 * @li @ref elm_object_disabled_set
 * @li @ref elm_object_disabled_get
 *
 * Supported elm_object_item common APIs.
 * @li @ref elm_object_item_del
 * @li @ref elm_object_item_disabled_set
 * @li @ref elm_object_item_disabled_get
 * @li @ref elm_object_item_part_text_set
 * @li @ref elm_object_item_part_text_get
 * @li @ref elm_object_item_part_content_set
 * @li @ref elm_object_item_part_content_get
 * @li @ref elm_object_item_signal_emit
 * @li @ref elm_object_item_focus_set
 * @li @ref elm_object_item_focus_get
 *
 * @ref tutorial_ctxpopup shows the usage of a good deal of the API.
 * @{
 */

#ifndef EFL_NOLEGACY_API_SUPPORT
#include "elc_ctxpopup_legacy.h"
#endif
/**
 * @}
 */
