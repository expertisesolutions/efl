#include "evas_gl_private.h"

#ifdef EVAS_CSERVE2
#include "evas_cs2_private.h"
#endif

static Eina_Thread async_loader_thread;
static Eina_Condition async_loader_cond;
static Eina_Lock async_loader_lock;

static Evas_GL_Texture_Async_Preload *async_current = NULL;
static Eina_List *async_loader_tex = NULL;
static Eina_List *async_loader_todie = NULL;
static Eina_Bool async_loader_exit = EINA_FALSE;
static Eina_Bool async_loader_running = EINA_FALSE;
static Eina_Bool async_loader_standby = EINA_FALSE;
static Eina_Bool async_current_cancel = EINA_FALSE;
static int async_loader_init = 0;

static void *async_engine_data = NULL;
static evas_gl_make_current_cb async_gl_make_current = NULL;

Eina_Bool
evas_gl_preload_push(Evas_GL_Texture_Async_Preload *async)
{
   if (!async_loader_init) return EINA_FALSE;

   eina_lock_take(&async_loader_lock);
   async_loader_tex = eina_list_append(async_loader_tex, async);
   eina_lock_release(&async_loader_lock);

   return EINA_TRUE;
}

void
evas_gl_preload_pop(Evas_GL_Texture *tex)
{
   Evas_GL_Texture_Async_Preload *async;
   Eina_List *l;

   if (!async_loader_init) return ;

   eina_lock_take(&async_loader_lock);

   if (async_gl_make_current && async_current && async_current->tex == tex)
     {
        Eina_Bool running = async_loader_running;
        evas_gl_make_current_cb tmp_cb = async_gl_make_current;
        void *tmp_data = async_engine_data;

        async_current_cancel = EINA_TRUE;
        eina_lock_release(&async_loader_lock);

        if (running) evas_gl_preload_render_lock(tmp_cb, tmp_data);

        evas_gl_common_texture_free(async_current->tex, EINA_FALSE);
#ifdef EVAS_CSERVE2
        if (evas_cache2_image_cached(&async_current->im->cache_entry))
          evas_cache2_image_close(&async_current->im->cache_entry);
        else
#endif
        evas_cache_image_drop(&async_current->im->cache_entry);
        free(async_current);

        async_current = NULL;

        if (running) evas_gl_preload_render_unlock(tmp_cb, tmp_data);

        return ;
     }

   EINA_LIST_FOREACH(async_loader_tex, l, async)
     if (async->tex == tex)
       {
          async_loader_tex = eina_list_remove_list(async_loader_tex, l);
          
          evas_gl_common_texture_free(async->tex, EINA_FALSE);
#ifdef EVAS_CSERVE2
        if (evas_cache2_image_cached(&async->im->cache_entry))
          evas_cache2_image_close(&async->im->cache_entry);
        else
#endif
          evas_cache_image_drop(&async->im->cache_entry);
          free(async);

          break;
       }

   eina_lock_release(&async_loader_lock);
}

static void
_evas_gl_preload_main_loop_wakeup(void)
{
   Evas_GL_Texture_Async_Preload *async;
   evas_gl_make_current_cb cb = async_gl_make_current;
   void *data = async_engine_data;
   Eina_Bool running = async_loader_running;

   if (running) evas_gl_preload_render_lock(cb, data);
   EINA_LIST_FREE(async_loader_todie, async)
     {
        Eo *target;

        if (async->tex)
          {
             EINA_LIST_FREE(async->tex->targets, target)
               eo_do(target, evas_obj_image_pixels_dirty_set(EINA_TRUE));
          }
        async->im->cache_entry.flags.preload_done = 0;
        if (async->tex)
          {
             async->tex->was_preloaded = EINA_TRUE;

             async->tex->ptt->allocations = 
               eina_list_remove(async->tex->ptt->allocations,
                                async->tex->aptt);
             pt_unref(async->tex->ptt);
             async->tex->ptt = NULL;
             free(async->tex->aptt);
             async->tex->aptt = NULL;

             evas_gl_common_texture_free(async->tex, EINA_FALSE);
          }
#ifdef EVAS_CSERVE2
        if (evas_cache2_image_cached(&async->im->cache_entry))
          evas_cache2_image_close(&async->im->cache_entry);
        else
#endif
        evas_cache_image_drop(&async->im->cache_entry);
        free(async);
     }
   if (running) evas_gl_preload_render_unlock(cb, data);
}

static void
_evas_gl_preload_main_loop_wakeup_cb(void *target EINA_UNUSED,
                                     Evas_Callback_Type type EINA_UNUSED,
                                     void *event_info EINA_UNUSED)
{
   _evas_gl_preload_main_loop_wakeup();
}

static Eina_Bool
_evas_gl_preload_lock(void)
{
   eina_lock_take(&async_loader_lock);
   if (async_loader_standby)
     {
        async_gl_make_current(async_engine_data, NULL);

        async_loader_running = EINA_FALSE;

        eina_condition_signal(&async_loader_cond);

        eina_condition_wait(&async_loader_cond);
        if (async_loader_exit) 
          {
             eina_lock_release(&async_loader_lock);
             return EINA_FALSE;
          }

        async_gl_make_current(async_engine_data, async_engine_data);
     }
   async_loader_running = EINA_TRUE;
   eina_lock_release(&async_loader_lock);

   return EINA_TRUE;
}

static void *
_evas_gl_preload_tile_async(void *data EINA_UNUSED, Eina_Thread t EINA_UNUSED)
{
   eina_lock_take(&async_loader_lock);
   while (!async_loader_exit)
     {
        Evas_GL_Texture_Async_Preload *async;
        GLuint fmt;

        if (!async_loader_standby && async_loader_tex)
          goto get_next;

     retry:
        eina_condition_wait(&async_loader_cond);
        if (async_loader_exit) break;

     get_next:
        // Get a texture to upload
        async = eina_list_data_get(async_loader_tex);
        async_loader_tex = eina_list_remove_list(async_loader_tex, async_loader_tex);
        if (!async) continue;

        async_loader_running = EINA_TRUE;
        async_current = async;

        eina_lock_release(&async_loader_lock);

        // Switch context to this thread
        if (!async_gl_make_current(async_engine_data, async_engine_data))
          {
             eina_lock_take(&async_loader_lock);
             async_loader_tex = eina_list_append(async_loader_tex, async_current);
             async_loader_running = EINA_FALSE;
             async_current = NULL;

             if (async_loader_standby)
               eina_condition_signal(&async_loader_cond);

             goto retry;
          }

        // FIXME: loop until all subtile are uploaded or the image is about to be deleted

        // TEMPORARY CODE JUST TO SEE IF IT WORK
        fmt = async->tex->pt->format;
        glBindTexture(GL_TEXTURE_2D, async->tex->pt->texture);
        GLERR(__FUNCTION__, __FILE__, __LINE__, "");
        if (async->unpack_row_length)
          {
             glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
             GLERR(__FUNCTION__, __FILE__, __LINE__, "");
          }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        GLERR(__FUNCTION__, __FILE__, __LINE__, "");

        //  +-+
        //  +-+
        //
        _tex_sub_2d(async->tex->gc, async->tex->x, async->tex->y,
                    async->im->cache_entry.w, async->im->cache_entry.h,
                    fmt, async->tex->pt->dataformat,
                    async->im->image.data);
        //  xxx
        //  xxx
        //  ---
        _tex_sub_2d(async->tex->gc, async->tex->x, async->tex->y + async->im->cache_entry.h,
                    async->im->cache_entry.w, 1,
                    fmt, async->tex->pt->dataformat,
                    async->im->image.data + ((async->im->cache_entry.h - 1) * async->im->cache_entry.w));
        //  xxx
        //  xxx
        // o
        _tex_sub_2d(async->tex->gc, async->tex->x - 1, async->tex->y + async->im->cache_entry.h,
                    1, 1,
                    fmt, async->tex->pt->dataformat,
                    async->im->image.data + ((async->im->cache_entry.h - 1) * async->im->cache_entry.w));
        //  xxx
        //  xxx
        //     o
        _tex_sub_2d(async->tex->gc, async->tex->x + async->im->cache_entry.w, async->tex->y + async->im->cache_entry.h,
                    1, 1,
                    fmt, async->tex->pt->dataformat,
                    async->im->image.data + ((async->im->cache_entry.h - 1) * async->im->cache_entry.w) + (async->im->cache_entry.w - 1));
        if (async->unpack_row_length)
          {
             glPixelStorei(GL_UNPACK_ROW_LENGTH, async->im->cache_entry.w);
             GLERR(__FUNCTION__, __FILE__, __LINE__, "");
             // |xxx
             // |xxx
             //
             _tex_sub_2d(async->tex->gc, async->tex->x - 1, async->tex->y,
                         1, async->im->cache_entry.h,
                         fmt, async->tex->pt->dataformat,
                         async->im->image.data);
             //  xxx|
             //  xxx|
             //
             _tex_sub_2d(async->tex->gc, async->tex->x + async->im->cache_entry.w, async->tex->y,
                         1, async->im->cache_entry.h,
                         fmt, async->tex->pt->dataformat,
                         async->im->image.data + (async->im->cache_entry.w - 1));
          }
        else
          {
             DATA32 *tpix, *ps, *pd;
             int i;

             tpix = alloca(async->im->cache_entry.h * sizeof(DATA32));
             pd = tpix;
             ps = async->im->image.data;
             for (i = 0; i < (int)async->im->cache_entry.h; i++)
               {
                  *pd = *ps;
                  pd++;
                  ps += async->im->cache_entry.w;
               }
             // |xxx
             // |xxx
             //
             _tex_sub_2d(async->tex->gc, async->tex->x - 1, async->tex->y,
                         1, async->im->cache_entry.h,
                         fmt, async->tex->pt->dataformat,
                         tpix);
             pd = tpix;
             ps = async->im->image.data + (async->im->cache_entry.w - 1);
             for (i = 0; i < (int)async->im->cache_entry.h; i++)
               {
                  *pd = *ps;
                  pd++;
                  ps += async->im->cache_entry.w;
               }
             //  xxx|
             //  xxx|
             //
             _tex_sub_2d(async->tex->gc, async->tex->x + async->im->cache_entry.w, async->tex->y,
                         1, async->im->cache_entry.h,
                         fmt, async->tex->pt->dataformat,
                         tpix);
          }

        // Switch back to current texture
        if (async->tex->ptt->texture != async->tex->gc->pipe[0].shader.cur_tex)
          {
             glBindTexture(GL_TEXTURE_2D, async->tex->gc->pipe[0].shader.cur_tex);
             GLERR(__FUNCTION__, __FILE__, __LINE__, "");
          }

        // Shall we block now ?
        if (!_evas_gl_preload_lock())
          break;

        // Release context
        async_gl_make_current(async_engine_data, NULL);

        evas_async_events_put(NULL, 0, NULL, _evas_gl_preload_main_loop_wakeup_cb);

        eina_lock_take(&async_loader_lock);
        async_current = NULL;
        async_loader_todie = eina_list_append(async_loader_todie, async);
        async_loader_running = EINA_FALSE;

        if (async_loader_standby)
          eina_condition_signal(&async_loader_cond);
     }
   eina_lock_release(&async_loader_lock);

   return NULL;
}

// In the main loop
// Push stuff on the todo queue
// Upload the mini texture
// Use the mini texture
// Once download of the big texture, destroy mini texture and image data


// Put the async preloader on standby
void
evas_gl_preload_render_lock(evas_gl_make_current_cb make_current, void *engine_data)
{
   if (!async_loader_init) return ;
   eina_lock_take(&async_loader_lock);
   if (async_loader_running)
     {
        async_loader_standby = EINA_TRUE;
        eina_condition_wait(&async_loader_cond);

        make_current(engine_data, engine_data);

        async_engine_data = NULL;
        async_gl_make_current = NULL;
     }

   eina_lock_release(&async_loader_lock);
}

// Let the async preloader run !
void
evas_gl_preload_render_unlock(evas_gl_make_current_cb make_current, void *engine_data)
{
   if (!async_loader_init) return ;
   if (!make_current) return ;

   eina_lock_take(&async_loader_lock);
   if (!async_loader_running && (async_loader_tex || async_current))
     {
        make_current(engine_data, NULL);

        async_gl_make_current = make_current;
        async_engine_data = engine_data;

        async_loader_standby = EINA_FALSE;
        eina_condition_signal(&async_loader_cond);
     }
   eina_lock_release(&async_loader_lock);
}

// add a way to destroy surface and temporarily stop the rendering of the image
void
evas_gl_preload_render_relax(evas_gl_make_current_cb make_current, void *engine_data)
{
   if (engine_data != async_engine_data) return ;

   evas_gl_preload_render_lock(make_current, engine_data);
}

static Eina_Bool
_evas_gl_preload_target_die(void *data, Eo *obj,
                            const Eo_Event_Description *desc EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Evas_GL_Texture *tex = data;

   evas_gl_preload_target_unregister(tex, obj);

   return EO_CALLBACK_CONTINUE;
}

void
evas_gl_preload_target_register(Evas_GL_Texture *tex, Eo *target)
{
   EINA_SAFETY_ON_NULL_RETURN(tex);

   eo_do(target,
         eo_event_callback_add(EO_EV_DEL, _evas_gl_preload_target_die, tex));
   tex->targets = eina_list_append(tex->targets, target);
   tex->references++;
}

void
evas_gl_preload_target_unregister(Evas_GL_Texture *tex, Eo *target)
{
   Eina_List *l;
   const Eo *o;

   EINA_SAFETY_ON_NULL_RETURN(tex);

   eo_do(target,
         eo_event_callback_del(EO_EV_DEL, _evas_gl_preload_target_die, tex));

   EINA_LIST_FOREACH(tex->targets, l, o)
     if (o == target)
       {
          void *data = async_engine_data;
          evas_gl_make_current_cb cb = async_gl_make_current;
          Eina_Bool running = async_loader_running;

          if (running) evas_gl_preload_render_lock(cb, data);
          tex->targets = eina_list_remove_list(tex->targets, l);
          evas_gl_common_texture_free(tex, EINA_FALSE);
          if (running) evas_gl_preload_render_unlock(cb, data);

          break;
       }
}

int
evas_gl_preload_init(void)
{
   if (getenv("EVAS_GL_NOPRELOAD")) return 0;
   if (async_loader_init++) return async_loader_init;

   eina_lock_new(&async_loader_lock);
   eina_condition_new(&async_loader_cond, &async_loader_lock);

   if (!eina_thread_create(&async_loader_thread, EINA_THREAD_BACKGROUND, 0, _evas_gl_preload_tile_async, NULL))
     {
        // FIXME: handle error case
     }

   return async_loader_init;
}

int
evas_gl_preload_shutdown(void)
{
   if (getenv("EVAS_GL_NOPRELOAD")) return 0;
   if (--async_loader_init) return async_loader_init;

   async_loader_exit = EINA_TRUE;
   eina_condition_signal(&async_loader_cond);

   eina_thread_join(async_loader_thread);

   eina_condition_free(&async_loader_cond);
   eina_lock_free(&async_loader_lock);

   return async_loader_init;
}
