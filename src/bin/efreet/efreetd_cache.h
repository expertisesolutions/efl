#ifndef __EFREETD_CACHE_H
#define __EFREETD_CACHE_H

extern Eina_Mempool *efreetd_mp_stat;

void cache_desktop_dir_add(const char *dir);
void cache_icon_dir_add(const char *dir);
void cache_icon_ext_add(const char *ext);
void cache_desktop_update(void);
Eina_Bool cache_desktop_exists(void);

void _desktop_cache_build_signal_send(void);
void _icon_cache_update_signal_send(Eina_Bool update);
void _desktop_cache_update_signal_send(Eina_Bool update);
void _mime_cache_build_signal_send(void);

Eina_Bool cache_init(void);
Eina_Bool cache_shutdown(void);

#endif
