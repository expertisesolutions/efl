#include "ecore_evas_extn_engine.h"

#ifndef O_BINARY
# define O_BINARY 0
#endif

struct _Extnbuf
{
   const char *file, *lock;
   void *addr;
   int fd, lockfd;
   int w, h, stride, size;
   Eina_Bool have_lock : 1;
   Eina_Bool am_owner : 1;
};

// "owner" creates/frees the bufs, clients just open existing ones
Extnbuf *
_extnbuf_new(const char *base, int id, Eina_Bool sys, int num,
             int w, int h, Eina_Bool owner)
{
   Extnbuf *b;
   char file[PATH_MAX];
   mode_t mode = S_IRUSR;
   int prot = PROT_READ;
   int page_size;
   Eina_Tmpstr *tmp = NULL;

   page_size = eina_cpu_page_size();

   b = calloc(1, sizeof(Extnbuf));
   b->fd = -1;
   b->lockfd = -1;
   b->addr = MAP_FAILED;
   b->w = w;
   b->h = h;
   b->stride = w * 4;
   b->size = page_size * (((b->stride * b->h) + (page_size - 1)) / page_size);
   b->am_owner = owner;

   snprintf(file, sizeof(file), "/%s-%i.%i", base, id, num);
   b->file = eina_stringshare_add(file);
   if (!b->file) goto err;


   if (sys) mode |= S_IRGRP | S_IROTH;

   if (owner)
     {
        mode |= S_IWUSR;
        prot |= PROT_WRITE;
     }

   if (b->am_owner)
     {
        b->lockfd = eina_file_mkstemp("ee-lock-XXXXXX", &tmp);
        if (b->lockfd < 0) goto err;
        b->lock = eina_stringshare_add(tmp);
        if (!b->lock) goto err;
        b->fd = shm_open(b->file, O_RDWR | O_CREAT | O_EXCL, mode);
        if (b->fd < 0) goto err;
        if (ftruncate(b->fd, b->size) < 0) goto err;
     }
   else
     {
        b->fd = shm_open(b->file, O_RDONLY, mode);
        if (b->fd < 0) goto err;
     }
   b->addr = mmap(NULL, b->size, prot, MAP_SHARED, b->fd, 0);
   if (b->addr == MAP_FAILED) goto err;
   eina_tmpstr_del(tmp);
   return b;
err:
   eina_tmpstr_del(tmp);
   _extnbuf_free(b);
   return NULL;
}

void
_extnbuf_free(Extnbuf *b)
{
   if (b->have_lock) _extnbuf_unlock(b);

   if (b->am_owner)
     {
        if (b->file) shm_unlink(b->file);
        if (b->lock) eina_file_unlink(b->lock);
     }
   
   if (b->addr != MAP_FAILED) munmap(b->addr, b->size);
   if (b->fd >= 0) close(b->fd);
   if (b->lockfd >= 0) close(b->lockfd);
   eina_stringshare_del(b->file);
   eina_stringshare_del(b->lock);
   b->file = NULL;
   b->lock = NULL;
   b->addr = MAP_FAILED;
   b->fd = 1;
   b->lockfd = 1;
   b->am_owner = EINA_FALSE;
   b->have_lock = EINA_FALSE;
   b->w = 0;
   b->h = 0;
   b->stride = 0;
   b->size = 0;
   free(b);
}

// both ends can lock or unlock a buf
void *
_extnbuf_data_get(Extnbuf *b, int *w, int *h, int *stride)
{
   if (w) *w = b->w;
   if (h) *h = b->h;
   if (stride) *stride = b->stride;
   return b->addr;
}

void *
_extnbuf_lock(Extnbuf *b, int *w, int *h, int *stride)
{
   if (!b) return NULL;
   if (!b->have_lock)
     {
        if (b->lockfd >= 0)
          {
             struct flock filelock;

             filelock.l_type = b->am_owner ? F_WRLCK : F_RDLCK;
             filelock.l_whence = SEEK_SET;
             filelock.l_start = 0;
             filelock.l_len = 0;
             if (fcntl(b->lockfd, F_SETLK, &filelock) == -1)
               {
                  ERR("lock take fail");
                  return NULL;
               }
          }
        b->have_lock = EINA_TRUE;
     }
   return _extnbuf_data_get(b, w, h, stride);
}

void
_extnbuf_unlock(Extnbuf *b)
{
   if (!b || !b->have_lock) return;
   if (b->lockfd >= 0)
     {
        struct flock filelock;

        filelock.l_type = F_UNLCK;
        filelock.l_whence = SEEK_SET;
        filelock.l_start = 0;
        filelock.l_len = 0;
        if (fcntl(b->lockfd, F_SETLKW, &filelock) == -1)
          {
             ERR("lock release fail");
             return;
          }
     }
   b->have_lock = EINA_FALSE;
}

const char *
_extnbuf_lock_file_get(const Extnbuf *b)
{
   return b->lock;
}

Eina_Bool
_extnbuf_lock_file_set(Extnbuf *b, const char *file)
{
   if (b->am_owner) return EINA_FALSE;
   if (b->lock) eina_stringshare_del(b->lock);
   if (b->lockfd >= 0) close(b->lockfd);
   b->lockfd = -1;
   if (!file)
     {
        b->lock = NULL;
        b->lockfd = -1;
        return EINA_TRUE;
     }
   b->lock = eina_stringshare_add(file);
   if (!b->lock) goto err;
   b->lockfd = open(b->lock, O_RDWR | O_BINARY);
   if (b->lockfd >= 0) return EINA_TRUE;
err:
   if (b->lock) eina_stringshare_del(b->lock);
   if (b->lockfd >= 0) close(b->lockfd);
   b->lockfd = -1;
   b->lock = NULL;
   return EINA_FALSE;
}

Eina_Bool
_extnbuf_lock_get(const Extnbuf *b)
{
   return b->have_lock;
}
