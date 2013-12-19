#ifndef EVAS_CS2_H
#define EVAS_CS2_H 1

#include <Eina.h>

#ifdef EVAS_CSERVE2

#include <Evas_Loader.h>

typedef enum {
   CSERVE2_OPEN = 1,
   CSERVE2_OPENED,
   CSERVE2_LOAD,
   CSERVE2_LOADED,
   CSERVE2_PRELOAD,
   CSERVE2_UNLOAD,
   CSERVE2_CLOSE,
   CSERVE2_FONT_LOAD,
   CSERVE2_FONT_LOADED,
   CSERVE2_FONT_UNLOAD,
   CSERVE2_FONT_GLYPHS_LOAD,
   CSERVE2_FONT_GLYPHS_LOADED,
   CSERVE2_FONT_GLYPHS_USED,
   CSERVE2_STATS,
   CSERVE2_FONT_DEBUG,
   CSERVE2_INDEX_LIST,
   CSERVE2_ERROR
} Message_Type;

typedef enum {
   CSERVE2_NONE,
   CSERVE2_GENERIC,
   CSERVE2_DOES_NOT_EXIST,
   CSERVE2_PERMISSION_DENIED,
   CSERVE2_RESOURCE_ALLOCATION_FAILED,
   CSERVE2_CORRUPT_FILE,
   CSERVE2_UNKNOWN_FORMAT,
   CSERVE2_INVALID_COMMAND,
   CSERVE2_LOADER_DIED,
   CSERVE2_LOADER_EXEC_ERR,
   CSERVE2_INVALID_CACHE, // invalid cserve cache entry
   CSERVE2_FILE_CHANGED,
   CSERVE2_REQUEST_CANCEL
} Error_Type;

struct _Msg_Base {
   int type;
   unsigned int rid;
};

typedef struct _Msg_Base Msg_Base;

struct _Msg_Open {
   Msg_Base base;
   unsigned int file_id;
   int path_offset;
   int key_offset;
   unsigned int image_id;

   Eina_Bool has_load_opts : 1;

   // Followed by:
   // const char file[];
   // const char key[];
   // Evas_Image_Load_Opts opts; // if has_load_opts
};

struct _Msg_Opened {
   Msg_Base base;
   struct {
      int w, h;
      int frame_count;
      int loop_count;
      int loop_hint; /* include Evas.h? Copy the enum around? */
      Eina_Bool alpha : 1;
   } image;
};

struct _Msg_Load {
   Msg_Base base;
   unsigned int image_id;
};

struct _Msg_Loaded {
   Msg_Base base;
   struct {
      int mmap_offset;
      int use_offset;
      int mmap_size;
      int image_size;
   } shm;
   Eina_Bool alpha_sparse : 1;
};

struct _Msg_Preload {
   Msg_Base base;
   unsigned int image_id;
};

struct _Msg_Preloaded {
   Msg_Base base;
};

struct _Msg_Unload {
   Msg_Base base;
   unsigned int image_id;
};

struct _Msg_Close {
   Msg_Base base;
   unsigned int file_id;
};

/**
 * @struct _Msg_Font_Load
 *
 * Message from client to request load or unload of a font.
 *
 * The path strings follow the struct inside the message.
 */
struct _Msg_Font_Load {
   Msg_Base base;
   unsigned int sourcelen; // font id
   unsigned int pathlen; // font id
   unsigned int rend_flags; // font id
   unsigned int size; // font id
   unsigned int dpi; // font id
};

/**
 * @struct _Msg_Font_Loaded
 *
 * Message from server to inform that a font was loaded.
 */
struct _Msg_Font_Loaded {
   Msg_Base base;
};

/**
 * @struct _Msg_Font_Glyphs_Request
 *
 * Message from client to request load of glyphs, or inform usage of them.
 *
 * The path strings follow the struct inside the message, as well as
 * the list of glyphs to be loaded.
 */
struct _Msg_Font_Glyphs_Request {
   Msg_Base base;
   unsigned int sourcelen; // font id
   unsigned int pathlen; // font id
   unsigned int rend_flags; // font id
   unsigned int size; // font id
   unsigned int dpi; // font id
   unsigned int hint;
   unsigned int nglyphs;
};

/**
 * @struct _Msg_Font_Glyphs_Loaded
 *
 * Message from server to inform that some glyphs were loaded.
 *
 * This message is quite complex: it contains the font id, representing for
 * which font these glyphs should be loaded, and the number of caches on which
 * the loaded glyphs are stored. Each cache is a SHM segment. The information
 * about each SHM comes serialized just after this struct, and can be read in
 * order as follows:
 *
 * shm name:
 *  - unsigned int size;
 *  - char name[];
 * glyphs:
 *  - unsigned int nglyphs;
 *  - struct {
 *      unsigned int index;
 *      unsigned int offset;
 *      unsigned int size;
 *      unsigned int rows;
 *      unsigned int width;
 *      unsigned int pitch;
 *      unsigned int num_grays;
 *      unsigned int pixel_mode;
 *    } glarray[];
 */
struct _Msg_Font_Glyphs_Loaded {
   Msg_Base base;
   unsigned int ncaches;
};

struct _Msg_Stats {
   Msg_Base base;
   struct {
      unsigned int requested_size;
      unsigned int real_size;
      unsigned int unused_size;
      unsigned int fonts_loaded; /* number of loaded fonts */
      unsigned int fonts_unused; /* number of loaded fonts without reference
                                  * from any clients */
      int fonts_load_time; /* total time spent loading fonts */
      int fonts_used_load_time; /* total time spent loading fonts that are
                                 * really used, i.e. fonts that have glyphs
                                 * rendered */
      int fonts_used_saved_time;
      int glyphs_request_time; /* time spent waiting for glyph requests */
      int glyphs_load_time; /* total time spent loading glyphs */
      int glyphs_render_time; /* total time spent rendering glyphs */
      int glyphs_slave_time; /* total time spent on the slave processing glyph load requests */
      int glyphs_saved_time; /* total time spent loading glyphs */
   } fonts;
   struct {
      unsigned int files_loaded; /* number of file headers loaded */
      unsigned int images_loaded; /* number of image data loaded */
      unsigned int images_unused; /* number of image data loaded and unused */

      unsigned int requested_size; /* memory usage originally requested by
                                    * the client */
      unsigned int files_size; /* memory usage from image headers */
      unsigned int images_size; /* memory usage from image data */
      unsigned int unused_size; /* memory usage from image data */

      int files_load_time;
      int files_saved_time;
      int images_load_time;
      int images_saved_time;
   } images;
};

/**
 * @struct _Msg_Font_Debug
 *
 * Message from server containing all font cache info.
 *
 * Content of the message follows:
 *
 * * number of font entries;
 * * each font entry:
 *   - unsigned int filelen
 *   - const char file
 *   - unsigned int namelen
 *   - const char name
 *   - unsigned int rend_flags;
 *   - unsigned int size;
 *   - unsigned int dpi;
 *   - unsigned int unused;
 *   - ncaches:
 *   - each cache:
 *     * usigned int shmnamelen;
 *     * const char shmname;
 *     * unsigned int size;
 *     * unsigned int usage;
 *     * unsigned int nglyphs;
 *     * each glyph:
 *       - unsigned int index;
 *       - unsigned int offset;
 *       - unsigned int size;
 *       - unsigned int rows;
 *       - unsigned int width;
 *       - unsigned int pitch;
 *       - unsigned int num_grays;
 *       - unsigned int pixel_mode;
 */
struct _Msg_Font_Debug {
    Msg_Base base;
};

/**
 * @brief The Msg_Index_List struct
 *
 * Message sent from the server, without request from the client,
 * to inform all clients of the shared index files. Contains the paths
 * to the latest File, Image and Font index shm.
 *
 * The paths contain only the filename used in the call to shm_open.
 * All strings must be null-terminated.
 */
struct _Msg_Index_List {
   Msg_Base base;
   char files_index_path[64];
   char images_index_path[64];
   char fonts_index_path[64];
};

typedef struct _Shm_Object Shm_Object;
typedef struct _File_Data File_Data;
typedef struct _Image_Data Image_Data;
typedef struct _Shared_Array_Header Shared_Array_Header;
typedef int string_t;
#define SHMOBJECT unsigned int id; unsigned int refcount

struct _Shared_Array_Header
{
   int32_t tag;
   int32_t elemsize;
   int32_t count;
   int32_t generation_id;
   int32_t emptyidx;
   int32_t sortedidx;
   int32_t _reserved1;
   int32_t _reserved2;
};

struct _Shm_Object
{
   SHMOBJECT;
};

struct _File_Data {
   SHMOBJECT;
   string_t path;
   string_t key;
   string_t loader_data;
   int w, h;
   int frame_count;
   int loop_count;
   int loop_hint;
   Eina_Bool alpha : 1;
   Eina_Bool invalid : 1;
};

struct _Image_Data {
   SHMOBJECT;
   unsigned int file_id;
   string_t shm_id;
   Evas_Image_Load_Opts opts;
   Eina_Bool alpha_sparse : 1;
   Eina_Bool unused : 1;
   Eina_Bool doload : 1;
};

struct _Msg_Error {
   Msg_Base base;
   int error;
};

typedef struct _Msg_Open Msg_Open;
typedef struct _Msg_Opened Msg_Opened;
typedef struct _Msg_Load Msg_Load;
typedef struct _Msg_Loaded Msg_Loaded;
typedef struct _Msg_Preload Msg_Preload;
typedef struct _Msg_Preloaded Msg_Preloaded;
typedef struct _Msg_Unload Msg_Unload;
typedef struct _Msg_Close Msg_Close;
typedef struct _Msg_Font_Load Msg_Font_Load;
typedef struct _Msg_Font_Loaded Msg_Font_Loaded;
typedef struct _Msg_Font_Glyphs_Request Msg_Font_Glyphs_Request;
typedef struct _Msg_Font_Glyphs_Loaded Msg_Font_Glyphs_Loaded;
typedef struct _Msg_Stats Msg_Stats;
typedef struct _Msg_Font_Debug Msg_Font_Debug;
typedef struct _Msg_Error Msg_Error;
typedef struct _Msg_Index_List Msg_Index_List;

#endif
#endif
