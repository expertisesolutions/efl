#ifndef _EFL_UI_ANIMATION_VIEW_EO_LEGACY_H_
#define _EFL_UI_ANIMATION_VIEW_EO_LEGACY_H_

#ifndef _ELM_ANIMATION_VIEW_EO_CLASS_TYPE
#define _ELM_ANIMATION_VIEW_EO_CLASS_TYPE

typedef Eo Elm_Animation_View;

#endif

#ifndef _ELM_ANIMATION_VIEW_EO_TYPES
#define _ELM_ANIMATION_VIEW_EO_TYPES

/**
 * @brief Defines the state of animation view
 *
 * @ingroup Elm_Animation_View
 */
typedef enum
{
  ELM_ANIMATION_VIEW_STATE_NOT_READY = 0, /**< Animation is not ready to
                                              * play. (Probably, it didn't file
                                              * set yet or failed to read file.
                                              *
                                              * @since 1.23 */
  ELM_ANIMATION_VIEW_STATE_PLAY, /**< Animation is on playing. see
                                     * @ref elm_animation_view_play
                                     *
                                     * @since 1.23 */
  ELM_ANIMATION_VIEW_STATE_PLAY_BACK, /**< Animation is on playing back
                                          * (rewinding). see
                                          * @ref elm_animation_view_play_back
                                          *
                                          * @since 1.23 */
  ELM_ANIMATION_VIEW_STATE_PAUSE, /**< Animation has been paused. To continue
                                      * animation, call
                                      * @ref elm_animation_view_resume. see
                                      * @ref elm_animation_view_pause
                                      *
                                      * @since 1.23 */
  ELM_ANIMATION_VIEW_STATE_STOP /**< Animation view successfully loaded a
                                    * file then readied for playing. Otherwise
                                    * after finished animation or stopped
                                    * forcely by request. see
                                    * @ref elm_animation_view_stop
                                    *
                                    * @since 1.23 */
} Elm_Animation_View_State;


#endif

/**
 * @brief Animation will be started automatically when it's possible.
 *
 * If @ref elm_animation_view_auto_play_get is @c true, animation will be
 * started when it's readied. The condition of @c auto play is when animation
 * view opened file successfully, yet to play it plus when the object is
 * visible. If animation view is disabled, invisible, it turns to pause state
 * then resume animation when it's visible again.
 *
 * @warning This auto play will be only affected to the next animation source.
 * So must be called before setting animation file
 *
 * Set auto play mode of an animation view object. @c true Enable auto play
 * mode, disable otherwise
 *
 * @param[in] obj The object.
 * @param[in] auto_play Auto play mode, Default value is @c false
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API void elm_animation_view_auto_play_set(Elm_Animation_View *obj, Eina_Bool auto_play);

/**
 * @brief Animation will be started automatically when it's possible.
 *
 * If @ref elm_animation_view_auto_play_get is @c true, animation will be
 * started when it's readied. The condition of @c auto play is when animation
 * view opened file successfully, yet to play it plus when the object is
 * visible. If animation view is disabled, invisible, it turns to pause state
 * then resume animation when it's visible again.
 *
 * @warning This auto play will be only affected to the next animation source.
 * So must be called before setting animation file
 *
 * Returns auto play mode of an animation view object.
 *
 * @param[in] obj The object.
 *
 * @return Auto play mode, Default value is @c false
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Bool elm_animation_view_auto_play_get(const Elm_Animation_View *obj);

/**
 * @brief Turn on/off animation looping.
 *
 * If @ref elm_animation_view_auto_repeat_get is @c true, it repeats animation
 * when animation frame is reached to end. This auto repeat mode is valid to
 * both play and play_back cases.
 *
 * Enable loop mode if @c true, disable otherwise.
 *
 * @param[in] obj The object.
 * @param[in] auto_repeat Loop mode, Defalut is @c false.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API void elm_animation_view_auto_repeat_set(Elm_Animation_View *obj, Eina_Bool auto_repeat);

/**
 * @brief Turn on/off animation looping.
 *
 * If @ref elm_animation_view_auto_repeat_get is @c true, it repeats animation
 * when animation frame is reached to end. This auto repeat mode is valid to
 * both play and play_back cases.
 *
 * Returns auto repeat mode of an animation view object.
 *
 * @param[in] obj The object.
 *
 * @return Loop mode, Defalut is @c false.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Bool elm_animation_view_auto_repeat_get(const Elm_Animation_View *obj);

/**
 * @brief Control animation speed.
 *
 * Control animation speed by multiplying @c speed value. If you want to play
 * animation double-time faster, you can give @c speed 2. If you want to play
 * animation double-time slower, you can give @c speed 0.5.
 *
 * @warning speed must be greater than zero.
 *
 * @param[in] obj The object.
 * @param[in] speed Speed factor. Default value is 1.0
 *
 * @return @c true when it's successful. @c false otherwise.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Bool elm_animation_view_speed_set(Elm_Animation_View *obj, double speed);

/**
 * @brief Control animation speed.
 *
 * Control animation speed by multiplying @c speed value. If you want to play
 * animation double-time faster, you can give @c speed 2. If you want to play
 * animation double-time slower, you can give @c speed 0.5.
 *
 * @warning speed must be greater than zero.
 *
 * Returns current animation speed of an animation view object.
 *
 * @param[in] obj The object.
 *
 * @return Speed factor. Default value is 1.0
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API double elm_animation_view_speed_get(const Elm_Animation_View *obj);

/**
 * @brief Get the duration of animation in seconds.
 *
 * This API returns total duration time of current animation in the seconds. If
 * current animation source isn't animatable, it returns zero.
 *
 * Returns current duration of an animation in seconds.
 *
 * @param[in] obj The object.
 *
 * @return duration time in the seconds
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API double elm_animation_view_duration_time_get(const Elm_Animation_View *obj);

/**
 * @brief Set current progress position of animation view object.
 *
 * When you required to jump on a certain frame instantly, you can change
 * current position by using this API.
 *
 * @warning The range of progress is 0 ~ 1.
 *
 * Set the current progress position of an animation view object.
 *
 * @param[in] obj The object.
 * @param[in] progress Progress position. Value must be 0 ~ 1.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API void elm_animation_view_progress_set(Elm_Animation_View *obj, double progress);

/**
 * @brief Set current progress position of animation view object.
 *
 * When you required to jump on a certain frame instantly, you can change
 * current position by using this API.
 *
 * @warning The range of progress is 0 ~ 1.
 *
 * Returns current progress position of an animation view object.
 *
 * @param[in] obj The object.
 *
 * @return Progress position. Value must be 0 ~ 1.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API double elm_animation_view_progress_get(const Elm_Animation_View *obj);

/**
 * @brief Number of current frame.
 *
 * Ranges from 0 to @ref elm_animation_view_frame_count_get - 1.
 *
 * Set the current frame of an animation view object.
 *
 * @param[in] obj The object.
 * @param[in] frame_num Current frame number.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API void elm_animation_view_frame_set(Elm_Animation_View *obj, int frame_num);

/**
 * @brief Number of current frame.
 *
 * Ranges from 0 to @ref elm_animation_view_frame_count_get - 1.
 *
 * Returns the current frame of an animation view object.
 *
 * @param[in] obj The object.
 *
 * @return Current frame number.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API int elm_animation_view_frame_get(const Elm_Animation_View *obj);

/**
 * @brief Play animation one time instantly when it's available.
 *
 * If current keyframe is on a certain position by playing back, this will play
 * forward from there.
 *
 * @warning Play request will be ignored if animation source is not set yet or
 * animation is paused state or it's already on playing.
 *
 * @param[in] obj The object.
 *
 * @return @c true when it's successful. @c false otherwise.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Bool elm_animation_view_play(Elm_Animation_View *obj);

/**
 * @brief Play back animation one time instantly when it's available.
 *
 * If current keyframe is on a certain position by playing, this will play
 * backward from there.
 *
 * @warning Play back request will be ignored if animation source is not set
 * yet or animation is paused state or it's already on playing back.
 *
 * @param[in] obj The object.
 *
 * @return @c true when it's successful. @c false otherwise.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Bool elm_animation_view_play_back(Elm_Animation_View *obj);

/**
 * @brief Pause current animation instantly.
 *
 * Once animation is paused, animation view must get resume to play continue
 * again.
 *
 * @warning Animation must be on playing or playing back status.
 *
 * @param[in] obj The object.
 *
 * @return @c true when it's successful. @c false otherwise.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Bool elm_animation_view_pause(Elm_Animation_View *obj);

/**
 * @brief Resume paused animation to continue animation.
 *
 * @warning This resume must be called on animation paused status.
 *
 * @param[in] obj The object.
 *
 * @return @c true when it's successful. @c false otherwise.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Bool elm_animation_view_resume(Elm_Animation_View *obj);

/**
 * @brief Stop playing animation.
 *
 * Stop animation instatly regardless of it's status and reset to show first
 * frame of animation. Even though current animation is paused, the animation
 * status will be stopped.
 *
 * @param[in] obj The object.
 *
 * @return @c true when it's successful. @c false otherwise.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Bool elm_animation_view_stop(Elm_Animation_View *obj);

/** Get the default view size that specified from vector resource.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Size2D elm_animation_view_default_size_get(const Elm_Animation_View *obj);


/**
 * @brief Returns the status whether current animation is on playing forward or
 * backward. warning: If animation view is not on playing, it will return
 * @c false.
 *
 * @param[in] obj The object.
 *
 * @return @c true, if animation on playing back, @c false otherwise.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API Eina_Bool elm_animation_view_is_playing_back(Elm_Animation_View *obj);

/**
 * @brief Get the index of end frame of the animation view, if it's animated.
 *  note : frame number starts with 0.
 *
 * @param[in] obj The object.
 *
 * @return The number of frames. 0, if it's not animated.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API int elm_animation_view_frame_count_get(const Elm_Animation_View *obj);

/**
 * @brief The start progress of the play. Default value is 0.
 *
 * Set start progress of an animation object.
 *
 * @param[in] obj The object.
 * @param[in] min_progress The minimum progress. Value must be 0 ~ 1.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API void elm_animation_view_min_progress_set(Elm_Animation_View *obj, double min_progress);

/**
 * @brief The start progress of the play. Default value is 0.
 *
 * Returns start progress of an animation object.
 *
 * @param[in] obj The object.
 *
 * @return The minimum progress. Value must be 0 ~ 1.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API double elm_animation_view_min_progress_get(const Elm_Animation_View *obj);

/**
 * @brief The last progress of the play. Default value is 1.
 *
 * Set last progress of an animation object.
 *
 * @param[in] obj The object.
 * @param[in] max_progress The maximum progress. Value must be 0 ~ 1.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API void elm_animation_view_max_progress_set(Elm_Animation_View *obj, double max_progress);

/**
 * @brief The last progress of the play. Default value is 1.
 *
 * Returns last progress of an animation object.
 *
 * @param[in] obj The object.
 *
 * @return The maximum progress. Value must be 0 ~ 1.
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API double elm_animation_view_max_progress_get(const Elm_Animation_View *obj);

/**
 * @brief The start frame of the play. Default value is 0.
 *
 * Set minimum frame of an animation object.
 *
 * @param[in] obj The object.
 * @param[in] min_frame The minimum frame for play. Value must be 0 ~
 * @ref elm_animation_view_max_frame_get
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API void elm_animation_view_min_frame_set(Elm_Animation_View *obj, int min_frame);

/**
 * @brief The start frame of the play. Default value is 0.
 *
 * Returns minimum frame of an animation object.
 *
 * @param[in] obj The object.
 *
 * @return The minimum frame for play. Value must be 0 ~
 * @ref elm_animation_view_max_frame_get
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API int elm_animation_view_min_frame_get(const Elm_Animation_View *obj);

/**
 * @brief The last frame of the play. Default value is
 * @ref elm_animation_view_frame_count_get - 1
 *
 * Set maximum frame of an animation object.
 *
 * @param[in] obj The object.
 * @param[in] max_frame The maximum frame for play. Value must be
 * @ref elm_animation_view_min_frame_get ~ (@ref elm_animation_view_frame_count_get - 1)
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API void elm_animation_view_max_frame_set(Elm_Animation_View *obj, int max_frame);

/**
 * @brief The last frame of the play. Default value is
 * @ref elm_animation_view_frame_count_get - 1
 *
 * Returns maximum frame of an animation object.
 *
 * @param[in] obj The object.
 *
 * @return The maximum frame for play. Value must be
 * @ref elm_animation_view_min_frame_get ~ (@ref elm_animation_view_frame_count_get - 1)
 *
 * @since 1.23
 *
 * @ingroup Elm_Animation_View
 */
ELM_API int elm_animation_view_max_frame_get(const Elm_Animation_View *obj);

#ifdef EFL_BETA_API_SUPPORT
/** Elementary vg_animation class
 *
 * @ingroup Efl_Ui_VG_Animation_Legacy
 */
#define EFL_UI_VG_ANIMATION_LEGACY_CLASS efl_ui_vg_animation_legacy_class_get()

EWAPI const Efl_Class *efl_ui_vg_animation_legacy_class_get(void);
#endif /* EFL_BETA_API_SUPPORT */

#endif
