#include "eina_hash.h"
#include "eina_lock.h"

Eina_Hash *_eina_tls_map;
Eina_Lock _eina_tls_map_lock;

void
_eina_free_tls_value(Eina_TLS *key, void *val)
{
   if (val)
     {
        Eina_TLS_Delete_Cb delete_cb = eina_hash_find(_eina_tls_map, key);
        if (delete_cb) delete_cb(val);
        TlsSetValue(*key, NULL);
     }
}

static Eina_Bool
_eina_tls_hash_foreach_cb(const Eina_Hash *h, const void *key, void *cb, void *fdata)
{
    void *data = TlsGetValue(*(DWORD *) key);
    if (data)
      {
         Eina_TLS_Delete_Cb delete_cb = (Eina_TLS_Delete_Cb) cb;
         delete_cb(data);
         TlsSetValue(*(DWORD *) key, NULL);
      }
    return EINA_TRUE;
}

BOOL WINAPI
DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved)
{
    switch (reason)
      {
        case DLL_PROCESS_ATTACH:
            _eina_tls_map = eina_hash_int32_new(NULL);
            if (!eina_lock_new(&_eina_tls_map_lock))
              {
                  eina_hash_free(_eina_tls_map);
                  _eina_tls_map = NULL;
                  return FALSE;
              }
            break;
        case DLL_PROCESS_DETACH:
            eina_hash_foreach(_eina_tls_map, _eina_tls_hash_foreach_cb, NULL);
            eina_lock_free(&_eina_tls_map_lock);
            eina_hash_free(_eina_tls_map);
            break;
        case DLL_THREAD_DETACH:
            eina_lock_take(&_eina_tls_map_lock);
            eina_hash_foreach(_eina_tls_map, _eina_tls_hash_foreach_cb, NULL);
            eina_lock_release(&_eina_tls_map_lock);
            break;
      }

    return TRUE;
}
