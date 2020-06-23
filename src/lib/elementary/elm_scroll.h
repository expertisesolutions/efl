/**
 * @defgroup Elm_Scrollhints Scrollhints
 * @ingroup Elementary
 *
 * Objects when inside a scroller can scroll, but this may not always be
 * desirable in certain situations. This allows an object to hint to itself
 * and parents to "not scroll" in one of 2 ways. If any child object of a
 * scroller has pushed a scroll freeze or hold then it affects all parent
 * scrollers until all children have released them.
 *
 * 1. To hold on scrolling. This means just flicking and dragging may no
 * longer scroll, but pressing/dragging near an edge of the scroller will
 * still scroll. This is automatically used by the entry object when
 * selecting text.
 *
 * 2. To totally freeze scrolling. This means it stops. until
 * popped/released.
 *
 * @{
 */

/**
 * Push the scroll hold by 1
 *
 * This increments the scroll hold count by one. If it is more than 0 it will
 * take effect on the parents of the indicated object.
 *
 * @param obj The object
 * @ingroup Elm_Scrollhints
 */
ELM_API void      elm_object_scroll_hold_push(Evas_Object *obj);

/**
 * Pop the scroll hold by 1
 *
 * This decrements the scroll hold count by one. If it is more than 0 it will
 * take effect on the parents of the indicated object.
 *
 * @param obj The object
 * @ingroup Elm_Scrollhints
 */
ELM_API void      elm_object_scroll_hold_pop(Evas_Object *obj);

/**
 * Get the scroll hold by 1
 *
 * This gets the scroll hold count by one.
 *
 * @param obj The object
 * @return The scroll hold count
 * @since 1.7
 * @ingroup Elm_Scrollhints
 */
ELM_API int       elm_object_scroll_hold_get(const Evas_Object *obj);

/**
 * Push the scroll freeze by 1
 *
 * This increments the scroll freeze count by one. If it is more
 * than 0 it will take effect on the parents of the indicated
 * object.
 *
 * @param obj The object
 * @ingroup Elm_Scrollhints
 */
ELM_API void      elm_object_scroll_freeze_push(Evas_Object *obj);

/**
 * Pop the scroll freeze by 1
 *
 * This decrements the scroll freeze count by one. If it is more
 * than 0 it will take effect on the parents of the indicated
 * object.
 *
 * @param obj The object
 * @ingroup Elm_Scrollhints
 */
ELM_API void      elm_object_scroll_freeze_pop(Evas_Object *obj);

/**
 * Get the scroll freeze by 1
 *
 * This gets the scroll freeze count by one.
 *
 * @param obj The object
 * @return The scroll freeze count
 * @since 1.7
 * @ingroup Elm_Scrollhints
 */
ELM_API int       elm_object_scroll_freeze_get(const Evas_Object *obj);

/**
 * Lock the scrolling of the given widget (and thus all parents)
 *
 * This locks the given object from scrolling in the X axis (and implicitly
 * also locks all parent scrollers too from doing the same).
 *
 * @param obj The object
 * @param lock The lock state (1 == locked, 0 == unlocked)
 * @ingroup Elm_Scrollhints
 */
ELM_API void      elm_object_scroll_lock_x_set(Evas_Object *obj, Eina_Bool lock);

/**
 * Lock the scrolling of the given widget (and thus all parents)
 *
 * This locks the given object from scrolling in the Y axis (and implicitly
 * also locks all parent scrollers too from doing the same).
 *
 * @param obj The object
 * @param lock The lock state (1 == locked, 0 == unlocked)
 * @ingroup Elm_Scrollhints
 */
ELM_API void      elm_object_scroll_lock_y_set(Evas_Object *obj, Eina_Bool lock);

/**
 * Get the scrolling lock of the given widget
 *
 * This gets the lock for X axis scrolling.
 *
 * @param obj The object
 * @ingroup Elm_Scrollhints
 */
ELM_API Eina_Bool elm_object_scroll_lock_x_get(const Evas_Object *obj);

/**
 * Get the scrolling lock of the given widget
 *
 * This gets the lock for Y axis scrolling.
 *
 * @param obj The object
 * @ingroup Elm_Scrollhints
 */
ELM_API Eina_Bool elm_object_scroll_lock_y_get(const Evas_Object *obj);

/**
 * Enable item loop feature of the given widget
 *
 * If @p enable is @c EINA_TRUE, item selection/focus will loop internally.
 * This means if arrow keys are pressed at end of scroller's item,
 * screen is moved to opposite side.
 *
 * @param obj The object
 * @param enable item loop feature (@c EINA_TRUE == enable, @c EINA_FALSE == disable)
 *
 * @see elm_object_scroll_item_loop_enabled_get()
 * @since 1.10
 * @ingroup Scrollitem
 */
ELM_API void      elm_object_scroll_item_loop_enabled_set(Evas_Object *obj, Eina_Bool enable);

/**
 * Get the item loop enable status of the given widget
 *
 * This gets the item loop enabled status.
 *
 * @param obj The object
 *
 * @see elm_objecdt_scroll_item_enabled_set()
 * @since 1.10
 * @ingroup Scrollitem
 */
ELM_API Eina_Bool elm_object_scroll_item_loop_enabled_get(const Evas_Object *obj);

/**
 * @}
 */
