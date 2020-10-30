#ifdef HAVE_ECTOR
/**
 * @defgroup Evas_Object_Vg_Group Vector Graphics Object
 * @ingroup Evas
 *
 * @ref Evas_Object_Vg_Group is the scene graph for managing vector graphics objects.
 * User can create shape objects as well as fill objects and give them to the
 * Evas_Object_Vg for drawing on the screen as well as managing the lifecycle
 * of the objects, enabling reuse of shape objects.
 *
 * Since Evas_Object_Vg is an Evas_Object all operations applicable to
 * an Evas_Object can be performed on it (clipping, mapping, etc).
 *
 * To create complex vector graphics you can create a hierarchy of shape
 * and fill objects and give the hierarchy to Evas_Object which will be
 * responsible for drawing and showing on the screen.
 *
 * As the shape object and fill object (linear and radial gradients) have
 * retain mode API, you only have to create it once and set the properties
 * and give it to Evas_Object_Vg.
 *
 * Any change in the properties of a shape or fill object are automatically
 * notified to Evas_Object_Vg triggering a redraw to reflect the changes.
 *
 * To create a vector path, you can give a list of path commands to the shape
 * object using efl_gfx_shape_path_set() API.
 *
 * Below are the list of features currently supported by Vector object.
 *
 * @li Drawing SVG Path.
 *     You can construct a path by using api in efl_gfx_utils.h
 *
 * @li Gradient filling and stroking.
 *     You can fill or stroke the path using linear or radial gradient.
 *     @see Evas_Vg_Gradient_Linear and Evas_Vg_Gradient_Radial
 *
 * @li Transformation support for path and gradient fill. You can apply
       affine transforms to path objects.
 *     @see Eina_Matrix.
 *
 * @note Below are the list of interfaces and classes that can be used
 *       to draw vector graphics using vector objects.
 *
 * @li Efl.Gfx.Shape
 * @li Evas.VG_Shape
 * @li Evas.VG_Node
 * @li Efl.Gfx.Gradient
 * @li Efl.Gfx.Gradient_Radial
 * @li Efl.Gfx.Gradient_Linear
 *
 * Example:
 * @code
 * vector = evas_object_vg_add(canvas);
 * root = evas_obj_vg_root_node_get(vector);
 * shape = efl_add(EVAS_VG_SHAPE_CLASS, root);
 * Efl_Gfx_Path_Command *path_cmd = NULL;
 * double *points = NULL;
 * efl_gfx_path_append_circle(&path_cmd, &points);
 * *       evas_vg_node_origin_set(shape, 10, 10);
 * *       efl_gfx_shape_stroke_width_set(shape, 1.0);
 * *       evas_vg_node_color_set(shape, 128, 128, 128, 80);
 * *       efl_gfx_shape_path_set(shape, path_cmd, points);
 * @endcode
 *
 * @since 1.14
 *
 * @{
 */

/**
 * @brief Creates a new vector object on the given Evas @p e canvas.
 *
 * The shape object hierarchy can be added to the evas_object_vg by accessing
 * the root node of the vg canvas and adding the hierarchy as child to the root
 * node.
 *
 * @param[in] e The given canvas.
 * @return The created vector object handle.
 *
 * @see evas_obj_vg_root_node_get()
 *
 * @since 1.14
 */
EAPI Evas_Object *evas_object_vg_add(Evas *e) EINA_WARN_UNUSED_RESULT EINA_ARG_NONNULL(1) EINA_MALLOC;

/**
 * Get the total number of frames of the vector, if it's animated.
 *
 * @return The number of frames. 0, if it's not animated.
 *
 * @since 1.23
 */
EAPI int evas_object_vg_animated_frame_count_get(const Evas_Object *obj) EINA_ARG_NONNULL(1);

/**
 * Get the duration of a sequence of frames.
 *
 * This returns total duration in seconds that the specified
 * sequence of frames should take.
 *
 * If @p start_frame is 1 and @p frame_num is 0, this returns the
 * duration of frame 1. If @p start_frame is 1 and @p frame_num is 1,
 * this returns the total duration of frame 1 + frame 2.
 *
 * @param[in] start_frame The first frame, ranges from 1 to maximum frame count.
 * @param[in] frame_num Number of frames in the sequence, starts from 0.
 *
 * @return Duration in seconds.
 *
 * @see evas_object_vg_animated_frame_count_get()
 * @since 1.23
 */
EAPI double evas_object_vg_animated_frame_duration_get(const Evas_Object *obj, int start_frame EINA_UNUSED, int frame_num EINA_UNUSED) EINA_ARG_NONNULL(1);

/**
 *
 * Set the source file from where an vector object must fetch the real
 * vector data (it may be one of json, svg, eet files).
 *
 * If the file supports multiple data stored in it (as Eet files do),
 * you can specify the key to be used as the index of the vector in
 * this file.
 *
 * @param[in] file The vector file path.
 * @param[in] key The vector key in @p file (if its an Eet one), or @c
NULL, otherwise.
 *
 * @return @c EINA_TRUE if it's succeed to read file, @c EINA_FALSE otherwise.
 *
 * @since 1.23
 */
EAPI Eina_Bool evas_object_vg_file_set(Evas_Object *obj, const char *file, const char *key);

/**
 * Set current frame of animated vector object.
 *
 * @param[in] frame_index The index of current frame.
 *
 * @note the @p frame_index must be in range of animation frames. (0 ~ max frame count)
 *
 * @return @c EINA_TRUE, if the frame index is valid. @c EINA_FALSE, otherwise.
 *
 * @see evas_object_vg_animated_frame_count_get()
 *
 * @since 1.23
 */
EAPI Eina_Bool evas_object_vg_animated_frame_set(Evas_Object *obj, int frame_index) EINA_ARG_NONNULL(1, 2);

/**
 * Get the current frame number of animated vector object.
 *
 * @return The frame index.
 *
 * @see evas_object_vg_animated_frame_set()
 * @see evas_object_vg_animated_frame_count_get()
 *
 * @since 1.23
 */
EAPI int evas_object_vg_animated_frame_get(const Evas_Object *obj) EINA_ARG_NONNULL(1);


#include "canvas/efl_canvas_vg_node_eo.legacy.h"
#include "canvas/efl_canvas_vg_object_eo.legacy.h"
#include "canvas/efl_canvas_vg_container_eo.legacy.h"
#include "canvas/efl_canvas_vg_shape_eo.legacy.h"
#include "canvas/efl_canvas_vg_gradient_eo.legacy.h"

/**
 * @brief These values determine how the end of opened sub-paths are rendered in a
 * stroke. @ref evas_vg_shape_stroke_cap_set
 *
 * @since 1.24
 */
typedef enum Evas_Vg_Cap_Type
{
  EVAS_VG_CAP_BUTT = 0, /**< The end of lines is rendered as a full stop on the
                         * last point itself */
  EVAS_VG_CAP_ROUND,    /**< The end of lines is rendered as a half-circle
                         * around */
  EVAS_VG_CAP_SQUARE,   /**< The end of lines is rendered as a square around
                         * the last point */
  EVAS_VG_CAP_LAST      /**< Sentinel value to indicate last enum field during
                         * iteration */
} Evas_Vg_Cap;


/**
 * @brief These values determine how two joining lines are rendered in a stroker.
 * @ref evas_vg_shape_stroke_join_set
 *
 * @since 1.24
 */
typedef enum Evas_Vg_Join_Type
{
  EVAS_VG_JOIN_MITER = 0, /**< Used to render mitered line joins. The intersection
                           * of the strokes is clipped at a line perpendicular to
                           * the bisector of the angle between the strokes, at the
                           * distance from the intersection of the segments equal
                           * to the product of the miter limit value and the border
                           * radius.  This prevents long spikes being created */
  EVAS_VG_JOIN_ROUND,     /**< Used to render rounded line joins. Circular arcs
                           * are used to join two lines smoothly */
  EVAS_VG_JOIN_BEVEL,     /**< Used to render beveled line joins. The outer corner
                           * of the joined lines is filled by enclosing the
                           * triangular region of the corner with a straight line
                           * between the outer corners of each stroke */
  EVAS_VG_JOIN_LAST       /**< Sentinel value to indicate last enum field during
                           * iteration */
} Evas_Vg_Join;

/**
 * @brief These values determine how the points are interpreted in a stream of points.
 *
 * @since 1.24
 */
typedef enum Evas_Vg_Path_Command_Type
{
  EVAS_VG_PATH_COMMAND_END = 0,  /**< The end of stream , no more points to process */
  EVAS_VG_PATH_COMMAND_MOVE_TO,  /**< The next point is the start point of a sub path */
  EVAS_VG_PATH_COMMAND_LINE_TO,  /**< The next point is used to draw a line from
                                  * current point */
  EVAS_VG_PATH_COMMAND_CUBIC_TO, /**< The next three point is used to draw
                                  * a cubic bezier curve from current point */
  EVAS_VG_PATH_COMMAND_CLOSE,    /**< Close the current subpath by drawing a line
                                  * between current point and the first point of
                                  * current subpath */
  EVAS_VG_PATH_COMMAND_LAST      /**< Sentinel value to indicate last enum field
                                  * during iteration */
} Evas_Vg_Path_Command;

/**
 * @brief Type defining gradient stops. Describes the location and color of a
 * transition point in a gradient.
 *
 * @since 1.24
 */
typedef struct _Evas_Vg_Gradient_Stop
{
  double offset; /**< The location of the gradient stop within the gradient
                  * vector */
  int r; /**< The component R color of the gradient stop */
  int g; /**< The component G color of the gradient stop */
  int b; /**< The component B color of the gradient stop */
  int a; /**< The component A color of the gradient stop */
} Evas_Vg_Gradient_Stop;

/**
 * @brief Specifies how the area outside the gradient area should be filled.
 * @ref evas_vg_gradient_spread_set
 *
 * @since 1.24
 */
typedef enum Evas_Vg_Gradient_Spread_Type
{
  EVAS_VG_GRADIENT_SPREAD_PAD = 0, /**< The area is filled with the closest stop
                                    * color. This is the default */
  EVAS_VG_GRADIENT_SPREAD_REFLECT, /**< The gradient is reflected outside the
                                    * gradient area */
  EVAS_VG_GRADIENT_SPREAD_REPEAT, /**< The gradient is repeated outside the
                                   * gradient area */
  EVAS_VG_GRADIENT_SPREAD_LAST /**< Sentinel value to indicate last enum field
                                * during iteration */
} Evas_Vg_Gradient_Spread;

/**
 * @brief Type describing dash.
 * @ref evas_vg_shape_stroke_dash_set
 *
 * @since 1.24
 */
typedef struct _Evas_Vg_Dash
{
  double length; /**< Dash drawing length */
  double gap;    /**< Distance between two dashes */
} Evas_Vg_Dash;

/**
 * @brief Creates a new vector shape object.
 *
 * @param[in] parent The given vector container object.
 * @return The created vector shape object handle.
 *
 * @since 1.14
 */
EAPI Evas_Vg_Shape* evas_vg_shape_add(Evas_Vg_Container *parent);

/**
 * @brief Creates a new vector container object.
 *
 * @param[in] parent The given vector container object.
 * @return The created vector container object handle.
 *
 * @since 1.14
 */

EAPI Evas_Vg_Container* evas_vg_container_add(Evas_Object *parent);

/**
 * @brief Retrieves whether or not the given Evas_Vg_Node object is visible.
 *
 * @param[in] obj The object.
 * @return @c EINA_TRUE if the object is visible, @c EINA_FALSE otherwise.
 *
 * @since 1.24
 */
EAPI Eina_Bool evas_vg_node_visible_get(Evas_Vg_Node *obj);

/**
 * @brief Makes the given Evas_Vg_Node object visible or invisible.
 *
 * @param[in] obj The object.
 * @param[in] v @c EINA_TRUE if to make the object visible, @c EINA_FALSE otherwise.
 *
 * @since 1.24
 */
EAPI void evas_vg_node_visible_set(Evas_Vg_Node *obj, Eina_Bool v);

/**
 * @brief Retrieves the general/main color of the given Evas_Vg_Node object.
 *
 * Retrieves the “main” color's RGB component (and alpha channel)
 * values, <b>which range from 0 to 255</b>. For the alpha channel,
 * which defines the object's transparency level, 0 means totally
 * transparent, while 255 means opaque. These color values are
 * premultiplied by the alpha value.
 *
 *
 * @note Use @c NULL pointers on the components you're not interested
 * in: they'll be ignored by the function.
 *
 * @param[in] obj The object.
 * @param[out] r The red component of the given color.
 * @param[out] g The green component of the given color.
 * @param[out] b The blue component of the given color.
 * @param[out] a The alpha component of the given color.
 *
 * @since 1.24
 */
EAPI void evas_vg_node_color_get(Evas_Vg_Node *obj, int *r, int *g, int *b, int *a);

/**
 * @brief Sets the general/main color of the given Evas_Vg_Node object to the given
 * one.
 *
 * @see evas_vg_node_color_get() (for an example)
 * @note These color values are expected to be premultiplied by @p a.
 *
 * @param[in] obj The object.
 * @param[in] r The red component of the given color.
 * @param[in] g The green component of the given color.
 * @param[in] b The blue component of the given color.
 * @param[in] a The alpha component of the given color.
 *
 * @since 1.24
 */
EAPI void evas_vg_node_color_set(Evas_Vg_Node *obj, int r, int g, int b, int a);

/**
 * @brief Retrieves the geometry of the given Evas_Vg_Node object.
 *
 * @param[in] obj The object.
 * @param[out] x The x geometry of the given object.
 * @param[out] y The y geometry of the given object.
 * @param[out] w The w geometry of the given object.
 * @param[out] h The h geometry of the given object.
 *
 * @since 1.24
 */
EAPI void evas_vg_node_geometry_get(Evas_Vg_Node *obj, int *x, int *y, int *w, int *h);

/**
 * @brief Changes the geometry of the given Evas_Vg_Node object.
 *
 * @param[in] obj The object.
 * @param[in] x The x geometry of the given object.
 * @param[in] y The y geometry of the given object.
 * @param[in] w The w geometry of the given object.
 * @param[in] h The h geometry of the given object.
 *
 * @since 1.24
 * @deprecated
 */
EAPI void evas_vg_node_geometry_set(Evas_Vg_Node *obj, int x, int y, int w, int h) EINA_DEPRECATED;

/**
 * @brief Stack @p obj immediately below @p below.
 *
 * Objects, in a given canvas, are stacked in the order they get added
 * to it.  This means that, if they overlap, the highest ones will
 * cover the lowest ones, in that order. This function is a way to
 * change the stacking order for the objects.
 *
 * This function is intended to be used with <b>objects belonging to
 * the same layer</b> in a given canvas, otherwise it will fail (and
 * accomplish nothing).
 *
 * If you have smart objects on your canvas and @p obj is a member of
 * one of them, then @p below must also be a member of the same
 * smart object.
 *
 * Similarly, if @p obj is not a member of a smart object, @p below
 * must not be either.
 *
 * @see evas_object_layer_get()
 * @see evas_object_layer_set()
 * @see evas_object_stack_below()
 *
 * @param[in] obj The object.
 * @param[in] below The object below which to stack．
 *
 * @since 1.24
 */
EAPI void evas_vg_node_stack_below(Evas_Vg_Node *obj, Evas_Vg_Node *below);

/**
 * @brief Stack @p obj immediately above @p above.
 *
 * Objects, in a given canvas, are stacked in the order they get added
 * to it.  This means that, if they overlap, the highest ones will
 * cover the lowest ones, in that order. This function is a way to
 * change the stacking order for the objects.
 *
 * This function is intended to be used with <b>objects belonging to
 * the same layer</b> in a given canvas, otherwise it will fail (and
 * accomplish nothing).
 *
 * If you have smart objects on your canvas and @p obj is a member of
 * one of them, then @p above must also be a member of the same
 * smart object.
 *
 * Similarly, if @p obj is not a member of a smart object, @p above
 * must not be either.
 *
 * @see evas_object_layer_get()
 * @see evas_object_layer_set()
 * @see evas_object_stack_below()
 *
 * @param[in] obj The object.
 * @param[in] above The object above which to stack.
 *
 * @since 1.24
 */
EAPI void evas_vg_node_stack_above(Evas_Vg_Node *obj, Evas_Vg_Node *above);

/**
 * @brief Raise @p obj to the top of its layer.
 *
 * @p obj will, then, be the highest one in the layer it belongs
 * to. Object on other layers won't get touched.
 *
 * @see evas_object_stack_above()
 * @see evas_object_stack_below()
 * @see evas_object_lower()
 *
 * @param[in] obj The object.
 *
 * @since 1.24
 */
EAPI void evas_vg_node_raise(Evas_Vg_Node *obj);

/**
 * @brief Lower @p obj to the bottom of its layer.
 *
 * @p obj will, then, be the lowest one in the layer it belongs
 * to. Objects on other layers won't get touched.
 *
 * @see evas_object_stack_above()
 * @see evas_object_stack_below()
 * @see evas_object_raise()
 *
 * @param[in] obj The object.
 *
 * @since 1.24
 */
EAPI void evas_vg_node_lower(Evas_Vg_Node *obj);

#include "canvas/efl_canvas_vg_node_eo.legacy.h"

/**
 * @brief Get the stroke scaling factor used for stroking this path.
 *
 * @param[in] obj The object.
 * @return The stroke scale of the given object.
 *
 * @since 1.14
 */
EAPI double evas_vg_shape_stroke_scale_get(Evas_Vg_Shape *obj);

/**
 * @brief Sets the stroke scale to be used for stroking the path.
 * the scale property will be used along with stroke width property.
 *
 * @param[in] obj The object.
 * @param[in] s The stroke scale value.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_stroke_scale_set(Evas_Vg_Shape *obj, double s);

/**
 * @brief Gets the color used for stroking the path.
 *
 * @param[in] obj The object.
 * @param[out] r The red component of the given color.
 * @param[out] g The green component of the given color.
 * @param[out] b The blue component of the given color.
 * @param[out] a The alpha component of the given color.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_stroke_color_get(Evas_Vg_Shape *obj, int *r, int *g, int *b, int *a);

/**
 * @brief Sets the color to be used for stroking the path.
 *
 * @param[in] obj The object.
 * @param[in] r The red component of the given color.
 * @param[in] g The green component of the given color.
 * @param[in] b The blue component of the given color.
 * @param[in] a The alpha component of the given color.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_stroke_color_set(Evas_Vg_Shape *obj, int r, int g, int b, int a);

/**
 * @brief Gets the stroke width to be used for stroking the path.
 *
 * @param[in] obj The object.
 * @return The stroke width of the given object.
 *
 * @since 1.14
 */
EAPI double evas_vg_shape_stroke_width_get(Evas_Vg_Shape *obj);

/**
 * @brief Sets the stroke width to be used for stroking the path.
 *
 * @param[in] obj The object.
 * @param[in] w The stroke width to be used.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_stroke_width_set(Evas_Vg_Shape *obj, double w);

/**
 * @brief Gets the stroke location to be used for stroking the path.
 *
 * @param[in] obj The object.
 * @return The stroke location.
 *
 * @since 1.24
 */
EAPI double evas_vg_shape_stroke_location_get(Evas_Vg_Shape *obj);

/**
 * @brief Sets the stroke location to be used for stroking the path.
 *
 * @param[in] obj The object.
 * @param[in] centered The stroke location.
 *
 * @since 1.24
 */
EAPI void evas_vg_shape_stroke_location_set(Evas_Vg_Shape *obj, double centered);

/**
 * @brief Gets the stroke dash type used for stroking path.
 *
 * @param[in] obj The object.
 * @param[out] dash The dash types.
 * @param[out] length The length of dash types.
 *
 * @since 1.24
 */
EAPI void evas_vg_shape_stroke_dash_get(Evas_Vg_Shape *obj, const Evas_Vg_Dash **dash, unsigned int *length);

/**
 * @brief Sets the stroke dash type to be used for stroking the path.
 *
 * @param[in] obj The object.
 * @param[in] dash The dash types.
 * @param[in] length The length of dash types.
 *
 * @since 1.24
 */
EAPI void evas_vg_shape_stroke_dash_set(Evas_Vg_Shape *obj, const Evas_Vg_Dash *dash, unsigned int length);

/**
 * @brief Gets the cap style used for stroking path.
 *
 * @param[in] obj The object.
 * @return The cap style of the given object.
 *
 * @since 1.14
 */
EAPI Evas_Vg_Cap evas_vg_shape_stroke_cap_get(Evas_Vg_Shape *obj);

/**
 * @brief Sets the cap style to be used for stroking the path.
 *
 * The cap will be used for capping the end point of a
 * open subpath.
 *
 * @see Evas_Vg_Cap
 *
 * @param[in] obj The object.
 * @param[in] c The cap style to use , default is EVAS_VG_CAP_BUTT
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_stroke_cap_set(Evas_Vg_Shape *obj, Evas_Vg_Cap c);

/**
 * @brief Gets the join style used for stroking path.
 *
 * @param[in] obj The object.
 * @return The join style of the given object.
 *
 * @since 1.14
 */
EAPI Evas_Vg_Join evas_vg_shape_stroke_join_get(Evas_Vg_Shape *obj);

/**
 * @brief Sets the join style to be used for stroking the path.
 *
 * The join style will be used for joining the two line segment
 * while stroking the path.
 *
 * @see Evas_Vg_Join
 *
 * @param[in] obj The object.
 * @param[in] j The join style to use, default is EVAS_VG_JOIN_MITER.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_stroke_join_set(Evas_Vg_Shape *obj, Evas_Vg_Join j);

/**
 * @brief Set the list of commands and points to be used to create the
 * content of shape.
 *
 * @see Evas_Vg_Path_Command
 *
 * @param[in] obj The object.
 * @param[in] op The command list.
 * @param[in] points The point list.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_path_set(Evas_Vg_Shape *obj, const Evas_Vg_Path_Command *op, const double *points);

/**
 * @brief Gets the command and points list.
 *
 * @param[in] obj The object.
 * @param[out] op The command list.
 * @param[out] points The point list.
 *
 * @since 1.14
 */

EAPI void evas_vg_shape_path_get(Evas_Vg_Shape *obj, const Evas_Vg_Path_Command **op, const double **points);

/**
 * @brief Gets the command and points length.
 *
 * @param[in] obj The object.
 * @param[out] commands The command length.
 * @param[out] points The points length.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_path_length_get(Evas_Vg_Shape *obj, unsigned int *commands, unsigned int *points);

/**
 * @brief Gets the current points.
 *
 * @param[in] obj The object.
 * @param[out] x The x co-ordinate of current point.
 * @param[out] y The y co-ordinate of current point.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_current_get(Evas_Vg_Shape *obj, double *x, double *y);

/**
 * @brief Gets the current control points.
 *
 * @param[in] obj The object.
 * @param[out] x The x co-ordinate of control point.
 * @param[out] y The y co-ordinate of control point.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_current_ctrl_get(Evas_Vg_Shape *obj, double *x, double *y);

/**
 * @brief Copy the shape data from the object specified.
 *
 * @param[in] obj The object.
 * @param[in] dup_from The Shape object from where data will be copied.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_dup(Evas_Vg_Shape *obj, Evas_Vg_Shape *dup_from);

/**
 * @brief Reset the shape data of the shape object.
 *
 * @param[in] obj The object.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_reset(Evas_Vg_Shape *obj);

/**
 * @brief Moves the current point to the given point,
 * implicitly starting a new subpath and closing the previous one.
 *
 * @see evas_vg_shape_append_close()
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of the current point.
 * @param[in] y The y co-ordinate of the current point.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_move_to(Evas_Vg_Shape *obj, double x, double y);

/**
 * @brief Adds a straight line from the current position to the given endPoint.
 * After the line is drawn, the current position is updated to be at the end
 * point of the line.
 *
 * @note if no current position present, it draws a line to itself, basically
 * a point.
 *
 * @see evas_vg_shape_append_move_to()
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of end point of the line.
 * @param[in] y The y co-ordinate of end point of the line.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_line_to(Evas_Vg_Shape *obj, double x, double y);

/**
 * @brief Adds a quadratic Bezier curve between the current position and the
 * given end point (x,y) using the control points specified by (ctrl_x, ctrl_y).
 * After the path is drawn, the current position is updated to be at the end
 * point of the path.
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of end point of the line.
 * @param[in] y The y co-ordinate of end point of the line.
 * @param[in] ctrl_x The x co-ordinate of control point.
 * @param[in] ctrl_y The y co-ordinate of control point.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_quadratic_to(Evas_Vg_Shape *obj, double x, double y, double ctrl_x, double ctrl_y);

/**
 * @brief Same as evas_vg_shape_append_quadratic_to() api only difference is that it
 * uses the current control point to draw the bezier.
 *
 * @see evas_vg_shape_append_quadratic_to()
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of end point of the line.
 * @param[in] y The y co-ordinate of end point of the line.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_squadratic_to(Evas_Vg_Shape *obj, double x, double y);

/**
 * @brief Adds a cubic Bezier curve between the current position and the
 * given end point (x,y) using the control points specified by
 * (ctrl_x0, ctrl_y0), and (ctrl_x1, ctrl_y1). After the path is drawn,
 * the current position is updated to be at the end point of the path.
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of end point of the line.
 * @param[in] y The y co-ordinate of end point of the line.
 * @param[in] ctrl_x0 The x co-ordinate of 1st control point.
 * @param[in] ctrl_y0 The y co-ordinate of 1st control point.
 * @param[in] ctrl_x1 The x co-ordinate of 2nd control point.
 * @param[in] ctrl_y1 The y co-ordinate of 2nd control point.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_cubic_to(Evas_Vg_Shape *obj, double x, double y, double ctrl_x0, double ctrl_y0, double ctrl_x1, double ctrl_y1);

/**
 * @brief Same as evas_vg_shape_append_cubic_to() api only difference is that it uses
 * the current control point to draw the bezier.
 *
 * @see evas_vg_shape_append_cubic_to()
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of end point of the line.
 * @param[in] y The x co-ordinate of end point of the line.
 * @param[in] ctrl_x The x co-ordinate of 2nd control point.
 * @param[in] ctrl_y The y co-ordinate of 2nd control point.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_scubic_to(Evas_Vg_Shape *obj, double x, double y, double ctrl_x, double ctrl_y);

/**
 * @brief Append an arc that connects from the current point int the point list
 * to the given point (x,y). The arc is defined by the given radius in
 * x-direction (rx) and radius in y direction (ry).
 *
 * @note Use this api if you know the end point's of the arc otherwise
 * use more convenient function evas_vg_shape_append_arc_to()
 *
 * @see evas_vg_shape_append_arc_to()
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of end point of the arc.
 * @param[in] y The y co-ordinate of end point of the arc.
 * @param[in] rx The radius of arc in x direction.
 * @param[in] ry The radius of arc in y direction.
 * @param[in] angle The x-axis rotation , normally 0.
 * @param[in] large_arc Defines whether to draw the larger arc or smaller arc joining two point.
 * @param[in] sweep Defines whether the arc will be drawn counter-clockwise or clockwise from current point to the end point taking into account the large_arc property.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_arc_to(Evas_Vg_Shape *obj, double x, double y, double rx, double ry, double angle, Eina_Bool large_arc, Eina_Bool sweep);

/**
 * @brief Append an arc that enclosed in the given rectangle (x, y, w, h). The
 * angle is defined in counter clock wise , use -ve angle for clockwise arc.
 *
 * @param[in] obj The object.
 * @param[in] y The x co-ordinate of the rect.
 * @param[in] y The y co-ordinate of the rect.
 * @param[in] w The width of the rect.
 * @param[in] h The height of the rect.
 * @param[in] start_angle The angle at which the arc will start.
 * @param[in] sweep_length The length of the arc.
 *
 * @since 1.18
 */
EAPI void evas_vg_shape_append_arc(Evas_Vg_Shape *obj, double x, double y, double w, double h, double start_angle, double sweep_length);

/**
 * @brief Closes the current subpath by drawing a line to the beginning of the subpath,
 * automatically starting a new path. The current point of the new path is
 * (0, 0).
 *
 * @note If the subpath does not contain any points, this function does nothing.
 *
 * @param[in] obj The object.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_close(Evas_Vg_Shape *obj);

/**
 * @brief Append a circle with given center and radius.
 *
 * @see evas_vg_shape_append_arc()
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of the center of the circle.
 * @param[in] y The y co-ordinate of the center of the circle.
 * @param[in] radius The radius of the circle.
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_circle(Evas_Vg_Shape *obj, double x, double y, double radius);

/**
 * @brief Append the given rectangle with rounded corner to the path.
 *
 * The xr and yr arguments specify the radii of the ellipses defining the
 * corners of the rounded rectangle.
 *
 * @note xr and yr are specified in terms of width and height respectively.
 * @note if xr and yr are 0, then it will draw a rectangle without rounded corner.
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of the rectangle.
 * @param[in] y The y co-ordinate of the rectangle.
 * @param[in] w The width of the rectangle.
 * @param[in] h The height of the rectangle.
 * @param[in] rx The x radius of the rounded corner and should be in range [0 to w/2].
 * @param[in] ry The y radius of the rounded corner and should be in range [0 to h/2].
 *
 * @since 1.14
 */
EAPI void evas_vg_shape_append_rect(Evas_Vg_Shape *obj, double x, double y, double w, double h, double rx, double ry);

/**
 * @brief Append the SVG path data.
 *
 * @param[in] obj The object.
 * @param[in] svg_path_data The svg path data to append.
 *
 * @since 1.24
 */
EAPI void evas_vg_shape_append_svg_path(Evas_Vg_Shape *obj, const char *svg_path_data);

/**
 * @brief Creates intermediary path part-way between two paths
 *
 * Sets the points of the @c obj as the linear interpolation of the points in
 * the @c from and @c to paths.  The path's x,y position and control point
 * coordinates are likewise interpolated.
 *
 * The @c from and @c to paths must not already have equivalent points, and
 * @c to must contain at least as many points as @c from, else the function
 * returns @c false with no interpolation performed.  If @c to has more points
 * than @c from, the excess points are ignored.
 *
 * @param[in] obj The object.
 * @param[in] from The source path.
 * @param[in] to The destination path.
 * @param[in] pos_map The position map in range 0.0 to 1.0.
 *
 * @since 1.24
 */
EAPI Eina_Bool evas_vg_shape_interpolate(Evas_Vg_Shape *obj, const Eo *from, const Eo *to, double pos_map);

/**
 * @brief Check if the given objects have equal commands.
 *
 * @param[in] obj The object.
 * @param[in] with The target object.
 *
 * @return True on equal, @c false otherwise.
 *
 * @since 1.24
 */
EAPI Eina_Bool evas_vg_shape_equal_commands(Evas_Vg_Shape *obj, const Eo *with);

/**
 * @brief Set a vg object as the fill property.
 *
 * @param[in] obj The object whose fill property gets modified.
 * @param[in] f The object content to be used for filling.
 *
 * @since 1.24
 */
EAPI void evas_vg_shape_fill_set(Evas_Vg_Shape *obj, Evas_Vg_Node *f);

/**
 * @brief Returns the object that is set for the fill property.
 *
 * @param[in] obj The object whose fill property is inspected.
 * @return The object that is set as fill property.
 *
 * @since 1.24
 */
EAPI Evas_Vg_Node* evas_vg_shape_fill_get(const Evas_Vg_Shape *obj);

/**
 * @brief Set a vg object as the stroke fill property.
 *
 * @param[in] obj The object whose stroke fill property gets modified.
 * @param[in] f The object content will be used for stroke filling.
 *
 * @since 1.24
 */
EAPI void evas_vg_shape_stroke_fill_set(Evas_Vg_Shape *obj, Evas_Vg_Node *f);

/**
 * @brief Returns the object that is set for the stroke fill property.
 *
 * @param[in] obj The object whose stroke fill property is inspected.
 * @return The object that is set as stroke fill property.
 *
 * @since 1.24
 */
EAPI Evas_Vg_Node* evas_vg_shape_stroke_fill_get(const Evas_Vg_Shape *obj);

#include "canvas/efl_canvas_vg_shape_eo.legacy.h"

/**
 * @brief Set the list of color stops for the gradient.
 *
 * @param[in] obj The object.
 * @param[in] colors The color stops list.
 * @param[in] length The length of the list.
 *
 * @since 1.14
 */
EAPI void evas_vg_gradient_stop_set(Evas_Vg_Gradient *obj, const Evas_Vg_Gradient_Stop *colors, unsigned int length);

/**
 * @brief Get the list of color stops.
 *
 * @param[in] obj The object.
 * @param[out] colors The color stops list.
 * @param[out] length The length of the list.
 *
 * @since 1.14
 */
EAPI void evas_vg_gradient_stop_get(Evas_Vg_Gradient *obj, const Evas_Vg_Gradient_Stop **colors, unsigned int *length);

/**
 * @brief Specifies the spread method that should be used for this gradient.
 *
 * @param[in] obj The object.
 * @param[in] s The spread type to be used.
 *
 * @since 1.14
 */
EAPI void evas_vg_gradient_spread_set(Evas_Vg_Gradient *obj, Evas_Vg_Gradient_Spread s);

/**
 * @brief Returns the spread method use by this gradient. The default is
 * EVAS_VG_GRADIENT_SPREAD_PAD.
 *
 * @param[in] obj The object.
 * @return The spread type of the given object.
 *
 * @since 1.14
 */
EAPI Evas_Vg_Gradient_Spread evas_vg_gradient_spread_get(Evas_Vg_Gradient *obj);

/**
 * @brief Creates a new linear gradient object.
 *
 * @param[in] parent The given vector container object.
 * @return The created linear gradient object handle.
 *
 * @since 1.24
 */
EAPI Evas_Vg_Gradient_Linear* evas_vg_gradient_linear_add(Evas_Vg_Container *parent);

/**
 * @brief Sets the start point of this linear gradient.
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of start point.
 * @param[in] y The y co-ordinate of start point
 *
 * @since 1.24
 */
EAPI void evas_vg_gradient_linear_start_set(Evas_Vg_Gradient_Linear *obj, double x, double y);

/**
 * @brief Gets the start point of this linear gradient.
 *
 * @param[in] obj The object.
 * @param[out] x The x co-ordinate of start point.
 * @param[out] y The y co-ordinate of start point.
 *
 * @since 1.24
 */
EAPI void evas_vg_gradient_linear_start_get(Evas_Vg_Gradient_Linear *obj, double *x, double *y);

/**
 * @brief Sets the end point of this linear gradient.
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of end point.
 * @param[in] y The y co-ordinate of end point.
 *
 * @since 1.24
 */
EAPI void evas_vg_gradient_linear_end_set(Evas_Vg_Gradient_Linear *obj, double x, double y);

/**
 * @brief Gets the end point of this linear gradient.
 *
 * @param[in] obj The object.
 * @param[out] x The x co-ordinate of end point.
 * @param[out] y The y co-ordinate of end point.
 *
 * @since 1.24
 */
EAPI void evas_vg_gradient_linear_end_get(Evas_Vg_Gradient_Linear *obj, double *x, double *y);

/**
 * @brief Creates a new radial gradient object.
 *
 * @param[in] parent The given vector container object.
 * @return The created radial gradient object handle.
 *
 * @since 1.24
 */
EAPI Evas_Vg_Gradient_Radial* evas_vg_gradient_radial_add(Evas_Vg_Container *parent);

/**
 * @brief Sets the center of this radial gradient.
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of center point.
 * @param[in] y The y co-ordinate of center point.
 *
 * @since 1.24
 */
EAPI void evas_vg_gradient_radial_center_set(Evas_Vg_Gradient_Radial *obj, double x, double y);

/**
 * @brief Gets the center of this radial gradient.
 *
 * @param[in] obj The object.
 * @param[out] x The x co-ordinate of center point.
 * @param[out] y The y co-ordinate of center point.
 *
 * @since 1.24
 */
EAPI void evas_vg_gradient_radial_center_get(Evas_Vg_Gradient_Radial *obj, double *x, double *y);

/**
 * @brief Sets the center radius of this radial gradient.
 *
 * @param[in] obj The object.
 * @param[in] r The center radius.
 *
 * @since 1.24
 */
EAPI void evas_vg_gradient_radial_radius_set(Evas_Vg_Gradient_Radial *obj, double r);

/**
 * @brief Gets the center radius of this radial gradient.
 *
 * @param[in] obj The object.
 * @return The center radius of the given object.
 *
 * @since 1.24
 */
EAPI double evas_vg_gradient_radial_radius_get(Evas_Vg_Gradient_Radial *obj);

/**
 * @brief Sets the focal point of this radial gradient.
 *
 * @param[in] obj The object.
 * @param[in] x The x co-ordinate of focal point.
 * @param[in] y The y co-ordinate of focal point.
 *
 * @since 1.24
 */
EAPI void evas_vg_gradient_radial_focal_set(Evas_Vg_Gradient_Radial *obj, double x, double y);

/**
 * @brief Gets the focal point of this radial gradient.
 *
 * @param[in] obj The object.
 * @param[out] x The x co-ordinate of focal point.
 * @param[out] y The y co-ordinate of focal point.
 *
 * @since 1.24
 */
EAPI void evas_vg_gradient_radial_focal_get(Evas_Vg_Gradient_Radial *obj, double *x, double *y);

/**
 * @}
 */
#endif
