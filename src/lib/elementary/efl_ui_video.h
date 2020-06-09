/**
 * @defgroup Efl_Ui_Video Video
 * @ingroup Elementary
 *
 * @addtogroup Efl_Ui_Video
 * @{
 *
 * @image html video_inheritance_tree.png
 * @image latex video_inheritance_tree.eps
 *
 * @image html player_inheritance_tree.png
 * @image latex player_inheritance_tree.eps
 *
 * Elementary comes with two object that help design application that need
 * to display video.
 *
 * The first one, Efl_Ui_Video, display a video by using Emotion.
 * It embeds the video inside an Edje object, so you can do some
 * animation depending on the video state change. It also implements a
 * resource management policy to remove this burden from the application.
 *
 * The second one,
 * Elm_Player is a video player that need to be linked with an Efl_Ui_Video.
 * It take care of updating its content according to Emotion event and provide a
 * way to theme itself. It also automatically raises the priority of the
 * linked Efl_Ui_Video so it will use the video decoder, if available. It also
 * activates the "remember" function on the linked Efl_Ui_Video object.
 *
 * Both widgets inherit from the @ref Elm_Layout one, so that all the
 * functions acting on it also work for video objects.
 *
 * This widget emits the following signals, besides the ones sent from
 * @ref Elm_Layout :
 * @li @c "focused" : When the video has received focus. (since 1.8)
 * @li @c "unfocused" : When the video has lost focus. (since 1.8)
 *
 * The player widget emits the following signals, besides the ones
 * sent from @ref Elm_Layout :
 *  - @c "forward,clicked" - the user clicked the forward button.
 *  - @c "info,clicked" - the user clicked the info button.
 *  - @c "next,clicked" - the user clicked the next button.
 *  - @c "pause,clicked" - the user clicked the pause button.
 *  - @c "play,clicked" - the user clicked the play button.
 *  - @c "prev,clicked" - the user clicked the prev button.
 *  - @c "rewind,clicked" - the user clicked the rewind button.
 *  - @c "stop,clicked" - the user clicked the stop button.
 *
 * Default content parts of the player widget that you can use for are:
 * @li "video" - A video of the player
 *
 */

/**
 * ELM_PLAYER_CLASS
 */

#include "efl_ui_video_eo.h"
#ifndef EFL_NOLEGACY_API_SUPPORT
#include "efl_ui_video_legacy.h"
#endif
/**
 * @}
 */
