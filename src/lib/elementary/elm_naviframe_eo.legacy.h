#ifndef _ELM_NAVIFRAME_EO_LEGACY_H_
#define _ELM_NAVIFRAME_EO_LEGACY_H_

#ifndef _ELM_NAVIFRAME_EO_CLASS_TYPE
#define _ELM_NAVIFRAME_EO_CLASS_TYPE

typedef Eo Elm_Naviframe;

#endif

#ifndef _ELM_NAVIFRAME_EO_TYPES
#define _ELM_NAVIFRAME_EO_TYPES


#endif

/**
 * @brief Control the event enabled when pushing/popping items
 *
 * If @c enabled is @c true, the contents of the naviframe item will receives
 * events from mouse and keyboard during view changing such as item push/pop.
 *
 * @warning Events will be blocked by calling evas_object_freeze_events_set()
 * internally. So don't call the API whiling pushing/popping items.
 *
 * @param[in] obj The object.
 * @param[in] enabled Events are received when enabled is @c true, and ignored
 * otherwise.
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API void elm_naviframe_event_enabled_set(Elm_Naviframe *obj, Eina_Bool enabled);

/**
 * @brief Control the event enabled when pushing/popping items
 *
 * If @c enabled is @c true, the contents of the naviframe item will receives
 * events from mouse and keyboard during view changing such as item push/pop.
 *
 * @warning Events will be blocked by calling evas_object_freeze_events_set()
 * internally. So don't call the API whiling pushing/popping items.
 *
 * @param[in] obj The object.
 *
 * @return Events are received when enabled is @c true, and ignored otherwise.
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API Eina_Bool elm_naviframe_event_enabled_get(const Elm_Naviframe *obj);

/**
 * @brief Preserve the content objects when items are popped.
 *
 * @param[in] obj The object.
 * @param[in] preserve Enable the preserve mode if @c true, disable otherwise
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API void elm_naviframe_content_preserve_on_pop_set(Elm_Naviframe *obj, Eina_Bool preserve);

/**
 * @brief Preserve the content objects when items are popped.
 *
 * @param[in] obj The object.
 *
 * @return Enable the preserve mode if @c true, disable otherwise
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API Eina_Bool elm_naviframe_content_preserve_on_pop_get(const Elm_Naviframe *obj);

/**
 * @brief Control if creating prev button automatically or not
 *
 * @param[in] obj The object.
 * @param[in] auto_pushed If @c true, the previous button(back button) will be
 * created internally when you pass the @c NULL to the prev_btn parameter in
 * elm_naviframe_item_push
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API void elm_naviframe_prev_btn_auto_pushed_set(Elm_Naviframe *obj, Eina_Bool auto_pushed);

/**
 * @brief Control if creating prev button automatically or not
 *
 * @param[in] obj The object.
 *
 * @return If @c true, the previous button(back button) will be created
 * internally when you pass the @c NULL to the prev_btn parameter in
 * elm_naviframe_item_push
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API Eina_Bool elm_naviframe_prev_btn_auto_pushed_get(const Elm_Naviframe *obj);

/**
 * @brief Get a list of all the naviframe items.
 *
 * @param[in] obj The object.
 *
 * @return A list of naviframe items, @ref Elm_Widget_Item, or @c NULL on
 * failure. Note: The returned list MUST be freed.
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API Eina_List *elm_naviframe_items_get(const Elm_Naviframe *obj) EINA_WARN_UNUSED_RESULT;

/**
 * @brief Get a top item on the naviframe stack
 *
 * @param[in] obj The object.
 *
 * @return The top item on the naviframe stack or @c NULL, if the stack is
 * empty
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API Elm_Widget_Item *elm_naviframe_top_item_get(const Elm_Naviframe *obj);

/**
 * @brief Get a bottom item on the naviframe stack
 *
 * @param[in] obj The object.
 *
 * @return The bottom item on the naviframe stack or @c NULL, if the stack is
 * empty
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API Elm_Widget_Item *elm_naviframe_bottom_item_get(const Elm_Naviframe *obj);

/**
 * @brief Pop an item that is on top of the stack
 *
 * This pops an item that is on the top(visible) of the naviframe, makes it
 * disappear, then deletes the item. The item that was underneath it on the
 * stack will become visible.
 *
 * When pop transition animation is in progress, new pop operation is blocked
 * until current pop operation is complete.
 *
 * @param[in] obj The object.
 *
 * @return @c NULL or the content object(if the
 * elm_naviframe_content_preserve_on_pop_get is true).
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API Efl_Canvas_Object *elm_naviframe_item_pop(Elm_Naviframe *obj);

/**
 * @brief Insert a new item into the naviframe before item @c before.
 *
 * The item is inserted into the naviframe straight away without any transition
 * operations. This item will be deleted when it is popped.
 *
 * @param[in] obj The object.
 * @param[in] before The naviframe item to insert before.
 * @param[in] title_label The label in the title area. The name of the title
 * label part is "elm.text.title"
 * @param[in] prev_btn The button to go to the previous item. If it is NULL,
 * then naviframe will create a back button automatically. The name of the
 * prev_btn part is "elm.swallow.prev_btn"
 * @param[in] next_btn The button to go to the next item. Or It could be just
 * an extra function button. The name of the next_btn part is
 * "elm.swallow.next_btn"
 * @param[in] content The main content object. The name of content part is
 * "elm.swallow.content"
 * @param[in] item_style The current item style name. @c NULL would be default.
 *
 * @return The created item or @c NULL upon failure.
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API Elm_Widget_Item *elm_naviframe_item_insert_before(Elm_Naviframe *obj, Elm_Widget_Item *before, const char *title_label, Efl_Canvas_Object *prev_btn, Efl_Canvas_Object *next_btn, Efl_Canvas_Object *content, const char *item_style);

/**
 * @brief Simple version of item_promote.
 *
 * @param[in] obj The object.
 * @param[in] content Item to promote
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API void elm_naviframe_item_simple_promote(Elm_Naviframe *obj, Efl_Canvas_Object *content);

/**
 * @brief Insert a new item into the naviframe after item @c after.
 *
 * The item is inserted into the naviframe straight away without any transition
 * operations. This item will be deleted when it is popped.
 *
 * The following styles are available for this item: "default"
 *
 * @param[in] obj The object.
 * @param[in] after The naviframe item to insert after.
 * @param[in] title_label The label in the title area. The name of the title
 * label part is "elm.text.title"
 * @param[in] prev_btn The button to go to the previous item. If it is NULL,
 * then naviframe will create a back button automatically. The name of the
 * prev_btn part is "elm.swallow.prev_btn"
 * @param[in] next_btn The button to go to the next item. Or It could be just
 * an extra function button. The name of the next_btn part is
 * "elm.swallow.next_btn"
 * @param[in] content The main content object. The name of content part is
 * "elm.swallow.content"
 * @param[in] item_style The current item style name. @c NULL would be default.
 *
 * @return The created item or @c NULL upon failure.
 *
 * @ingroup Elm_Naviframe_Group
 */
ELEMENTARY_API Elm_Widget_Item *elm_naviframe_item_insert_after(Elm_Naviframe *obj, Elm_Widget_Item *after, const char *title_label, Efl_Canvas_Object *prev_btn, Efl_Canvas_Object *next_btn, Efl_Canvas_Object *content, const char *item_style);

#endif
