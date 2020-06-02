/**
 * @defgroup Elm_List List
 * @ingroup Elementary
 *
 * @image html list_inheritance_tree.png
 * @image latex list_inheritance_tree.eps
 *
 * @image html img/widget/list/preview-00.png
 * @image latex img/widget/list/preview-00.eps width=\textwidth
 *
 * @image html img/list.png
 * @image latex img/list.eps width=\textwidth
 *
 * A list widget is a container whose children are displayed
 * vertically or horizontally, in order, and can be selected. The list
 * can accept only one or multiple item selections. Also has many
 * modes of items displaying.
 *
 * A list is a very simple type of list widget. For more robust lists,
 * @ref Genlist should probably be used.
 *
 * This widget inherits from the @ref Elm_Layout one, so that all the
 * functions acting on it also work for list objects.
 *
 * This widget emits the following signals, besides the ones sent from
 * @ref Elm_Layout :
 * - @c "activated" - The user has double-clicked or pressed
 *   (enter|return|spacebar) on an item. The @p event_info parameter
 *   is the item that was activated.
 * - @c "clicked,double" - The user has double-clicked an item.
 *   The @p event_info parameter is the item that was double-clicked.
 * - @c "clicked,right" - The user has right-clicked an item. The @p
 *   event_info parameter is the item that was right-clicked. (since 1.13)
 * - @c "selected" - when the user selected an item
 * - @c "unselected" - when the user unselected an item
 * - @c "longpressed" - an item in the list is long-pressed
 * - @c "edge,top" - the list is scrolled until the top edge
 * - @c "edge,bottom" - the list is scrolled until the bottom edge
 * - @c "edge,left" - the list is scrolled until the left edge
 * - @c "edge,right" - the list is scrolled until the right edge
 * - @c "highlighted" - an item in the list is highlighted. This is called when
 *   the user presses an item or keyboard selection is done so the item is
 *   physically highlighted. The @p event_info parameter is the item that was
 *   highlighted.
 * - @c "unhighlighted" - an item in the list is unhighlighted. This is called
 *   when the user releases an item or keyboard selection is moved so the item
 *   is physically unhighlighted. The @p event_info parameter is the item that
 *   was unhighlighted.
 * - @c "language,changed" - the program's language changed
 * - @c "focused" - When the list has received focus. (since 1.8)
 * - @c "unfocused" - When the list has lost focus. (since 1.8)
 * - @c "item,focused" - When the list item has received focus. (since 1.10)
 * - @c "item,unfocused" - When the list item has lost focus. (since 1.10)
 *
 * Available styles for it are:
 * - @c "default"
 *
 * Default content parts of the list items that you can use are:
 * @li @c "start" - A start position object in the list item
 * @li @c "end" - An end position object in the list item
 * Another parts for customized styles are not accepted.
 *
 * Default text parts of the list items that you can use are:
 * @li @c "default" - A label in the list item
 * Another parts for customized styles are not accepted.
 *
 * Supported @c elm_object_item common APIs.
 * @li @ref elm_object_item_disabled_set
 * @li @ref elm_object_item_disabled_get
 * @li @ref elm_object_item_part_text_set
 * @li @ref elm_object_item_part_text_get
 * @li @ref elm_object_item_part_content_set
 * @li @ref elm_object_item_part_content_get
 * @li @ref elm_object_item_part_content_unset
 * @li @ref elm_object_item_del
 * @li @ref elm_object_item_signal_emit
 *
 * This widget implements the @ref elm-scrollable-interface
 * interface, so that all (non-deprecated) functions for the base @ref
 * Scroller widget also work for lists.
 *
 * Some calls on the list's API are marked as @b deprecated, as they
 * just wrap the scrollable widgets counterpart functions. Use the
 * ones we point you to, for each case of deprecation here, instead --
 * eventually the deprecated ones will be discarded (next major
 * release).
 *
 * List of examples:
 * @li @ref list_example_01
 * @li @ref list_example_02
 * @li @ref list_example_03
 */

/**
 * @addtogroup Elm_List
 * @{
 */

#ifndef EFL_NOLEGACY_API_SUPPORT
#include <elm_list_legacy.h>
#endif

/**
 * @}
 */
