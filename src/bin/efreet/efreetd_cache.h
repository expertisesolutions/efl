#ifndef __EFREETD_CACHE_H
#define __EFREETD_CACHE_H

extern Eina_Mempool *efreetd_mp_stat;
extern FILE *efreetd_log_file;

void cache_desktop_dir_add(const char *dir);
void cache_icon_dir_add(const char *dir);
void cache_icon_ext_add(const char *ext);
void cache_desktop_update(void);
Eina_Bool cache_desktop_exists(void);

Eina_List * _parse_strs(void *data, int size);
char * _parse_str(void *data, int size);

// Architecture independent auxiliar functions
void _desktop_add(char *data, int size);
void _desktop_build(const char *data, int size);
void _icon_dirs_add(const unsigned char *data, int size);
void _icon_exts_add(const unsigned char *data, int size);

// Architecture dependent communicatiton
void _desktop_cache_build_signal_send(void);
void _icon_cache_update_signal_send(Eina_Bool update);
void _desktop_cache_update_signal_send(Eina_Bool update);
void _mime_cache_build_signal_send(void);

Eina_Bool cache_init(void);
Eina_Bool cache_shutdown(void);

#endif
