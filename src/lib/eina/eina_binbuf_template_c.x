/*
 * vim:ts=8:sw=3:sts=8:noexpandtab:cino=>5n-3f0^-2{2
 */

/* This file should be included from files implementing binbuf.
 * The including file should define the following macros:
 * _STRBUF_DATA_TYPE
 * _STRBUF_CSIZE
 * _STRBUF_STRUCT_NAME
 * _STRBUF_MAGIC
 * _STRBUF_MAGIC_STR
 * _FUNC_EXPAND
 * See how it's done in eina_ustrbuf.c and eina_strbuf.c. This just makes things
 * a lot easier since those are essentially the same just with different sizes.
 */

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/**
 * @internal
 * @brief Initialize the strbuf module.
 *
 * @return #EINA_TRUE on success, #EINA_FALSE on failure.
 *
 * This function sets up the strbuf module of Eina. It is called by
 * eina_init().
 *
 * @see eina_init()
 */
Eina_Bool
_FUNC_EXPAND(init)(void)
{
   eina_magic_string_static_set(_STRBUF_MAGIC, _STRBUF_MAGIC_STR);
   return eina_strbuf_common_init();
}

/**
 * @internal
 * @brief Shut down the strbuf module.
 *
 * @return #EINA_TRUE on success, #EINA_FALSE on failure.
 *
 * This function shuts down the strbuf module set up by
 * eina_ustrbuf_init(). It is called by eina_shutdown().
 *
 * @see eina_shutdown()
 */
Eina_Bool
_FUNC_EXPAND(shutdown)(void)
{
   return eina_strbuf_common_shutdown();
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

EAPI _STRBUF_STRUCT_NAME *
_FUNC_EXPAND(new)(void)
{
   _STRBUF_STRUCT_NAME *buf = eina_strbuf_common_new(_STRBUF_CSIZE);
   EINA_MAGIC_SET(buf, _STRBUF_MAGIC);
   return buf;
}

EAPI _STRBUF_STRUCT_NAME *
_FUNC_EXPAND(manage_new_length)(_STRBUF_DATA_TYPE *str, size_t length)
{
   _STRBUF_STRUCT_NAME *buf =
      eina_strbuf_common_manage_new(_STRBUF_CSIZE, (void *) str, length);
   EINA_MAGIC_SET(buf, _STRBUF_MAGIC);
   return buf;
}

EAPI _STRBUF_STRUCT_NAME *
_FUNC_EXPAND(manage_read_only_new_length)(const _STRBUF_DATA_TYPE *str, size_t length)
{
   _STRBUF_STRUCT_NAME *buf =
     eina_strbuf_common_manage_ro_new(_STRBUF_CSIZE, (void *) str, length);
   EINA_MAGIC_SET(buf, _STRBUF_MAGIC);
   return buf;
}

EAPI void
_FUNC_EXPAND(free)(_STRBUF_STRUCT_NAME *buf)
{
   if (!buf) return ;

   EINA_MAGIC_CHECK_STRBUF(buf);
   EINA_MAGIC_SET(buf, EINA_MAGIC_NONE);
   eina_strbuf_common_free(buf);
}

EAPI void
_FUNC_EXPAND(reset)(_STRBUF_STRUCT_NAME *buf)
{
   EINA_MAGIC_CHECK_STRBUF(buf);
   eina_strbuf_common_reset(_STRBUF_CSIZE, buf);
}

EAPI Eina_Rw_Slice
_FUNC_EXPAND(expand)(_STRBUF_STRUCT_NAME *buf, size_t minimum_unused_space)
{
   Eina_Rw_Slice ret = {.len = 0, .mem = NULL};
   EINA_MAGIC_CHECK_STRBUF(buf, ret);
   return eina_strbuf_common_expand(_STRBUF_CSIZE, buf, minimum_unused_space);
}

EAPI Eina_Bool
_FUNC_EXPAND(use)(_STRBUF_STRUCT_NAME *buf, size_t extra_bytes)
{
   EINA_MAGIC_CHECK_STRBUF(buf, EINA_FALSE);
   return eina_strbuf_common_use(buf, extra_bytes);
}

EAPI Eina_Bool
_FUNC_EXPAND(append_length)(_STRBUF_STRUCT_NAME *buf, const _STRBUF_DATA_TYPE *str, size_t length)
{
   EINA_MAGIC_CHECK_STRBUF(buf, EINA_FALSE);
   return eina_strbuf_common_append_length(_STRBUF_CSIZE, buf, (const void *) str, length);
}

EAPI Eina_Bool
_FUNC_EXPAND(append_slice)(_STRBUF_STRUCT_NAME *buf, const Eina_Slice slice)
{
   EINA_MAGIC_CHECK_STRBUF(buf, EINA_FALSE);
   return eina_strbuf_common_append_length(_STRBUF_CSIZE, buf, slice.mem, slice.len);
}

EAPI Eina_Bool
_FUNC_EXPAND(append_buffer)(_STRBUF_STRUCT_NAME *buf, const _STRBUF_STRUCT_NAME *data)
{
   EINA_MAGIC_CHECK_STRBUF(buf, EINA_FALSE);
   EINA_MAGIC_CHECK_STRBUF(data, EINA_FALSE);

   return eina_strbuf_common_append_length(_STRBUF_CSIZE, buf, (_STRBUF_DATA_TYPE *) eina_strbuf_common_string_get(data), eina_strbuf_common_length_get(data));
}

EAPI Eina_Bool
_FUNC_EXPAND(insert_length)(_STRBUF_STRUCT_NAME *buf, const _STRBUF_DATA_TYPE *str, size_t length, size_t pos)
{
   EINA_MAGIC_CHECK_STRBUF(buf, EINA_FALSE);
   return eina_strbuf_common_insert_length(_STRBUF_CSIZE, buf, (const void *) str, length, pos);
}

EAPI Eina_Bool
_FUNC_EXPAND(insert_slice)(_STRBUF_STRUCT_NAME *buf, const Eina_Slice slice, size_t pos)
{
   EINA_MAGIC_CHECK_STRBUF(buf, EINA_FALSE);
   return eina_strbuf_common_insert_length(_STRBUF_CSIZE, buf, slice.mem, slice.len, pos);
}

EAPI Eina_Bool
_FUNC_EXPAND(append_char)(_STRBUF_STRUCT_NAME *buf, _STRBUF_DATA_TYPE c)
{
   EINA_MAGIC_CHECK_STRBUF(buf, EINA_FALSE);
   return eina_strbuf_common_append_char(_STRBUF_CSIZE, buf, (const void *) &c);
}

EAPI Eina_Bool
_FUNC_EXPAND(insert_char)(_STRBUF_STRUCT_NAME *buf, _STRBUF_DATA_TYPE c, size_t pos)
{
   EINA_MAGIC_CHECK_STRBUF(buf, EINA_FALSE);
   return eina_strbuf_common_insert_char(_STRBUF_CSIZE, buf, (const void *) &c, pos);
}

EAPI Eina_Bool
_FUNC_EXPAND(remove)(_STRBUF_STRUCT_NAME *buf, size_t start, size_t end)
{
   EINA_MAGIC_CHECK_STRBUF(buf, EINA_FALSE);
   return eina_strbuf_common_remove(_STRBUF_CSIZE, buf, start, end);
}

EAPI const _STRBUF_DATA_TYPE *
_FUNC_EXPAND(string_get)(const _STRBUF_STRUCT_NAME *buf)
{
   EINA_MAGIC_CHECK_STRBUF(buf, NULL);
   return (const _STRBUF_DATA_TYPE *) eina_strbuf_common_string_get(buf);
}

EAPI _STRBUF_DATA_TYPE *
_FUNC_EXPAND(string_steal)(_STRBUF_STRUCT_NAME *buf)
{
   EINA_MAGIC_CHECK_STRBUF(buf, NULL);
   return (_STRBUF_DATA_TYPE *) eina_strbuf_common_string_steal(_STRBUF_CSIZE, buf);
}

EAPI void
_FUNC_EXPAND(string_free)(_STRBUF_STRUCT_NAME *buf)
{
   EINA_MAGIC_CHECK_STRBUF(buf);
   eina_strbuf_common_string_free(_STRBUF_CSIZE, buf);
}

EAPI size_t
_FUNC_EXPAND(length_get)(const _STRBUF_STRUCT_NAME *buf)
{
   EINA_MAGIC_CHECK_STRBUF(buf, 0);
   return eina_strbuf_common_length_get(buf);
}

EAPI Eina_Slice
_FUNC_EXPAND(slice_get)(const _STRBUF_STRUCT_NAME *buf)
{
   Eina_Slice ret = {.len = 0, .mem = NULL};
   EINA_MAGIC_CHECK_STRBUF(buf, ret);
   return eina_strbuf_common_slice_get(buf);
}

EAPI Eina_Rw_Slice
_FUNC_EXPAND(rw_slice_get)(const _STRBUF_STRUCT_NAME *buf)
{
   Eina_Rw_Slice ret = {.len = 0, .mem = NULL};
   EINA_MAGIC_CHECK_STRBUF(buf, ret);
   return eina_strbuf_common_rw_slice_get(buf);
}

EAPI _STRBUF_DATA_TYPE*
_FUNC_EXPAND(release)(_STRBUF_STRUCT_NAME *buf)
{
   _STRBUF_DATA_TYPE *result;

   result = _FUNC_EXPAND(string_steal)(buf);
   _FUNC_EXPAND(free)(buf);

   return result;
}
