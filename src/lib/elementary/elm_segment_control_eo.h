#ifndef _ELM_SEGMENT_CONTROL_EO_H_
#define _ELM_SEGMENT_CONTROL_EO_H_

#ifndef _ELM_SEGMENT_CONTROL_EO_CLASS_TYPE
#define _ELM_SEGMENT_CONTROL_EO_CLASS_TYPE

typedef Eo Elm_Segment_Control;

#endif

#ifndef _ELM_SEGMENT_CONTROL_EO_TYPES
#define _ELM_SEGMENT_CONTROL_EO_TYPES


#endif
/** Elementary segment control class
 *
 * @ingroup Elm_Segment_Control
 */
#define ELM_SEGMENT_CONTROL_CLASS elm_segment_control_class_get()

ELM_API ELM_API_WEAK const Efl_Class *elm_segment_control_class_get(void);

/**
 * @brief Get the Segment items count from segment control.
 *
 * It will just return the number of items added to segment control @c obj.
 *
 * @param[in] obj The object.
 *
 * @return Segment items count.
 *
 * @ingroup Elm_Segment_Control
 */
ELM_API ELM_API_WEAK int elm_obj_segment_control_item_count_get(const Eo *obj);

/**
 * @brief Get the selected item.
 *
 * The selected item can be unselected with function
 * elm_segment_control_item_selected_set().
 *
 * The selected item always will be highlighted on segment control.
 *
 * @param[in] obj The object.
 *
 * @return The selected item or @c NULL if none of segment items is selected.
 *
 * @ingroup Elm_Segment_Control
 */
ELM_API ELM_API_WEAK Elm_Widget_Item *elm_obj_segment_control_item_selected_get(const Eo *obj);

/**
 * @brief Get the label of item.
 *
 * The return value is a pointer to the label associated to the item when it
 * was created, with function elm_segment_control_item_add(), or later with
 * function elm_object_item_text_set. If no label was passed as argument, it
 * will return @c NULL.
 *
 * @param[in] obj The object.
 * @param[in] idx The index of the segment item.
 *
 * @return The label of the item at @c index.
 *
 * @ingroup Elm_Segment_Control
 */
ELM_API ELM_API_WEAK const char *elm_obj_segment_control_item_label_get(const Eo *obj, int idx);

/**
 * @brief Insert a new item to the segment control object at specified
 * position.
 *
 * Index values must be between $0, when item will be prepended to segment
 * control, and items count, that can be get with
 * elm_segment_control_item_count_get(), case when item will be appended to
 * segment control, just like elm_segment_control_item_add().
 *
 * Items created with this function can be deleted with function
 * elm_object_item_del() or elm_segment_control_item_del_at().
 *
 * @note @c label set to @c NULL is different from empty string "". If an item
 * only has icon, it will be displayed bigger and centered. If it has icon and
 * label, even that an empty string, icon will be smaller and positioned at
 * left.
 *
 * @param[in] obj The object.
 * @param[in] icon The icon object to use for the left side of the item. An
 * icon can be any Evas object, but usually it is an icon created with
 * elm_icon_add().
 * @param[in] label The label of the item.
 * @param[in] idx Item position. Value should be between 0 and items count.
 *
 * @return The created item or @c NULL upon failure.
 *
 * @ingroup Elm_Segment_Control
 */
ELM_API ELM_API_WEAK Elm_Widget_Item *elm_obj_segment_control_item_insert_at(Eo *obj, Efl_Canvas_Object *icon, const char *label, int idx);

/**
 * @brief Get the item placed at specified index.
 *
 * Index is the position of an item in segment control widget. Its range is
 * from $0 to <tt> count - 1 </tt>. Count is the number of items, that can be
 * get with elm_segment_control_item_count_get().
 *
 * @param[in] obj The object.
 * @param[in] idx The index of the segment item.
 *
 * @return The segment control item or @c NULL on failure.
 *
 * @ingroup Elm_Segment_Control
 */
ELM_API ELM_API_WEAK Elm_Widget_Item *elm_obj_segment_control_item_get(const Eo *obj, int idx);

/**
 * @brief Remove a segment control item at given index from its parent,
 * deleting it.
 *
 * Items can be added with elm_segment_control_item_add() or
 * elm_segment_control_item_insert_at().
 *
 * @param[in] obj The object.
 * @param[in] idx The position of the segment control item to be deleted.
 *
 * @ingroup Elm_Segment_Control
 */
ELM_API ELM_API_WEAK void elm_obj_segment_control_item_del_at(Eo *obj, int idx);

/**
 * @brief Append a new item to the segment control object.
 *
 * A new item will be created and appended to the segment control, i.e., will
 * be set as last item.
 *
 * If it should be inserted at another position,
 * elm_segment_control_item_insert_at() should be used instead.
 *
 * Items created with this function can be deleted with function
 * elm_object_item_del() or elm_object_item_del_at().
 *
 * @note @c label set to @c NULL is different from empty string "". If an item
 * only has icon, it will be displayed bigger and centered. If it has icon and
 * label, even that an empty string, icon will be smaller and positioned at
 * left.
 *
 * @param[in] obj The object.
 * @param[in] icon The icon object to use for the left side of the item. An
 * icon can be any Evas object, but usually it is an icon created with
 * elm_icon_add().
 * @param[in] label The label of the item.  Note that, NULL is different from
 * empty string "".
 *
 * @return The created item or @c NULL upon failure.
 *
 * @ingroup Elm_Segment_Control
 */
ELM_API ELM_API_WEAK Elm_Widget_Item *elm_obj_segment_control_item_add(Eo *obj, Efl_Canvas_Object *icon, const char *label);

/**
 * @brief Get the icon associated to the item.
 *
 * The return value is a pointer to the icon associated to the item when it was
 * created, with function elm_segment_control_item_add(), or later with
 * function elm_object_item_part_content_set(). If no icon was passed as
 * argument, it will return @c NULL.
 *
 * @param[in] obj The object.
 * @param[in] idx The index of the segment item.
 *
 * @return The left side icon associated to the item at @c index.
 *
 * @ingroup Elm_Segment_Control
 */
ELM_API ELM_API_WEAK Efl_Canvas_Object *elm_obj_segment_control_item_icon_get(const Eo *obj, int idx);

ELM_API ELM_API_WEAK extern const Efl_Event_Description _ELM_SEGMENT_CONTROL_EVENT_CHANGED;

/** Called when segment control changed
 * @return Efl_Object *
 *
 * @ingroup Elm_Segment_Control
 */
#define ELM_SEGMENT_CONTROL_EVENT_CHANGED (&(_ELM_SEGMENT_CONTROL_EVENT_CHANGED))

#endif
