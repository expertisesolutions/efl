/* EINA - EFL data type library
 * Copyright (C) 2011 Cedric Bail
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>

#ifndef _MSC_VER
# include <unistd.h>
#endif

#ifdef HAVE_XATTR
# include <sys/xattr.h>
#endif

#include <fcntl.h>

#include "eina_config.h"
#include "eina_private.h"

#include "eina_safety_checks.h"
#include "eina_xattr.h"
#include "eina_convert.h"
#include "eina_stringshare.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

typedef struct _Eina_Xattr_Iterator Eina_Xattr_Iterator;

struct _Eina_Xattr_Iterator
{
   Eina_Iterator iterator;

   const char *file;
   Eina_Xattr *attr;

   ssize_t length;
   ssize_t offset;

   int fd;

   char xattr[1];
};

#ifdef HAVE_XATTR
static Eina_Bool
_eina_xattr_value_ls_fd_iterator_next(Eina_Xattr_Iterator *it, void **data)
{
   char *tmp;

   if (it->offset >= it->length)
     return EINA_FALSE;

   *data = it->attr;
   it->attr->name = it->xattr + it->offset;
   it->offset += strlen(it->attr->name) + 1;

   it->attr->length = fgetxattr(it->fd, it->attr->name, NULL, 0);
   if (it->attr->length > 0)
     {
        tmp = realloc((void*) it->attr->value, it->attr->length + 1);
        if (!tmp)
          {
             free((void*) it->attr->value);
             it->attr->value = NULL;
             it->attr->length = 0;
          }
        else
          {
             it->attr->value = tmp;
             it->attr->length = fgetxattr(it->fd, it->attr->name,
                                          (void *) it->attr->value,
                                          it->attr->length);
             tmp[it->attr->length] = '\0';
          }
     }

   return EINA_TRUE;
}

static Eina_Bool
_eina_xattr_value_ls_iterator_next(Eina_Xattr_Iterator *it, void **data)
{
   char *tmp;

   if (it->offset >= it->length)
     return EINA_FALSE;

   *data = it->attr;
   it->attr->name = it->xattr + it->offset;
   it->offset += strlen(it->attr->name) + 1;

   it->attr->length = getxattr(it->file, it->attr->name, NULL, 0);
   if (it->attr->length > 0)
     {
        tmp = realloc((void*) it->attr->value, it->attr->length + 1);
        if (!tmp)
          {
             free((void*) it->attr->value);
             it->attr->value = NULL;
             it->attr->length = 0;
          }
        else
          {
             it->attr->value = tmp;
             it->attr->length = getxattr(it->file, it->attr->name,
                                         (void*) it->attr->value,
                                         it->attr->length);
             tmp[it->attr->length] = '\0';
          }
     }

   return EINA_TRUE;
}

static Eina_Bool
_eina_xattr_ls_iterator_next(Eina_Xattr_Iterator *it, void **data)
{
   if (it->offset >= it->length)
     return EINA_FALSE;

   *data = it->xattr + it->offset;
   it->offset += strlen(it->xattr + it->offset) + 1;

   return EINA_TRUE;
}

static void *
_eina_xattr_ls_iterator_container(Eina_Xattr_Iterator *it EINA_UNUSED)
{
   return NULL;
}

static void
_eina_xattr_ls_iterator_free(Eina_Xattr_Iterator *it)
{
   EINA_MAGIC_SET(&it->iterator, 0);
   if (it->attr) free((void *) it->attr->value);
   eina_stringshare_del(it->file);
   free(it->attr);
   free(it);
}
#endif

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

Eina_Iterator *
eina_xattr_value_fd_ls(int fd)
{
#ifdef HAVE_XATTR
   Eina_Xattr_Iterator *it;
   ssize_t length;

   if (fd < 0) return NULL;

   length = flistxattr(fd, NULL, 0);
   if (length <= 0) return NULL;

   it = calloc(1, sizeof (Eina_Xattr_Iterator) + length - 1);
   if (!it) return NULL;

   it->attr = calloc(1, sizeof (Eina_Xattr));
   if (!it->attr)
     {
        free(it);
        return NULL;
     }

   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

   it->fd = fd;
   it->length = flistxattr(fd, it->xattr, length);
   if (it->length != length)
     {
        free(it);
	return NULL;
     }

   it->iterator.version = EINA_ITERATOR_VERSION;
   it->iterator.next = FUNC_ITERATOR_NEXT(_eina_xattr_value_ls_fd_iterator_next);
   it->iterator.get_container = FUNC_ITERATOR_GET_CONTAINER(_eina_xattr_ls_iterator_container);
   it->iterator.free = FUNC_ITERATOR_FREE(_eina_xattr_ls_iterator_free);

   return &it->iterator;
#else
   return NULL;
   (void)fd;
#endif
}

Eina_Iterator *
eina_xattr_fd_ls(int fd)
{
#ifdef HAVE_XATTR
   Eina_Xattr_Iterator *it;
   ssize_t length;

   if (fd < 0) return NULL;

   length = flistxattr(fd, NULL, 0);
   if (length <= 0) return NULL;

   it = calloc(1, sizeof (Eina_Xattr_Iterator) + length - 1);
   if (!it) return NULL;

   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

   it->length = flistxattr(fd, it->xattr, length);
   if (it->length != length)
     {
        free(it);
	return NULL;
     }

   it->iterator.version = EINA_ITERATOR_VERSION;
   it->iterator.next = FUNC_ITERATOR_NEXT(_eina_xattr_ls_iterator_next);
   it->iterator.get_container = FUNC_ITERATOR_GET_CONTAINER(_eina_xattr_ls_iterator_container);
   it->iterator.free = FUNC_ITERATOR_FREE(_eina_xattr_ls_iterator_free);

   return &it->iterator;
#else
   return NULL;
   (void)fd;
#endif
}

Eina_Iterator *
eina_xattr_ls(const char *file)
{
#ifdef HAVE_XATTR
   Eina_Xattr_Iterator *it;
   ssize_t length;

   EINA_SAFETY_ON_NULL_RETURN_VAL(file, NULL);

   length = listxattr(file, NULL, 0);
   if (length <= 0) return NULL;

   it = calloc(1, sizeof (Eina_Xattr_Iterator) + length - 1);
   if (!it) return NULL;

   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

   it->length = listxattr(file, it->xattr, length);
   if (it->length != length)
     {
        free(it);
	return NULL;
     }

   it->iterator.version = EINA_ITERATOR_VERSION;
   it->iterator.next = FUNC_ITERATOR_NEXT(_eina_xattr_ls_iterator_next);
   it->iterator.get_container = FUNC_ITERATOR_GET_CONTAINER(_eina_xattr_ls_iterator_container);
   it->iterator.free = FUNC_ITERATOR_FREE(_eina_xattr_ls_iterator_free);

   return &it->iterator;
#else
   return NULL;
   (void)file;
#endif
}

Eina_Iterator *
eina_xattr_value_ls(const char *file)
{
#ifdef HAVE_XATTR
   Eina_Xattr_Iterator *it;
   ssize_t length;

   EINA_SAFETY_ON_NULL_RETURN_VAL(file, NULL);

   length = listxattr(file, NULL, 0);
   if (length <= 0) return NULL;

   it = calloc(1, sizeof (Eina_Xattr_Iterator) + length - 1);
   if (!it) return NULL;

   it->attr = calloc(1, sizeof (Eina_Xattr));
   if (!it->attr)
     {
        free(it);
        return NULL;
     }

   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

   it->length = listxattr(file, it->xattr, length);
   if (it->length != length)
     {
        free(it->attr);
        free(it);
        return NULL;
     }

   it->file = eina_stringshare_add(file);

   it->iterator.version = EINA_ITERATOR_VERSION;
   it->iterator.next = FUNC_ITERATOR_NEXT(_eina_xattr_value_ls_iterator_next);
   it->iterator.get_container = FUNC_ITERATOR_GET_CONTAINER(_eina_xattr_ls_iterator_container);
   it->iterator.free = FUNC_ITERATOR_FREE(_eina_xattr_ls_iterator_free);

   return &it->iterator;
#else
   return NULL;
   (void)file;
#endif
}

void *
eina_xattr_get(const char *file, const char *attribute, ssize_t *size)
{
#ifdef HAVE_XATTR
   char *ret = NULL;
   ssize_t tmp;

   EINA_SAFETY_ON_NULL_RETURN_VAL(file, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(attribute, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(size, NULL);

   *size = getxattr(file, attribute, NULL, 0);
   /* Size should be less than 2MB (already huge in my opinion) */
   if (!(*size > 0 && *size < 2 * 1024 * 1024))
     goto on_error;

   ret = malloc(*size + 1);
   if (!ret) goto on_error;

   tmp = getxattr(file, attribute, ret, *size);
   if (tmp != *size)
     goto on_error;
   ret[tmp] = '\0';

   return ret;

 on_error:
   free(ret);
   *size = 0;
   return NULL;
#else
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!size, NULL);
   *size = 0;
   return NULL;
   (void)file;
   (void)attribute;
#endif
}

void *
eina_xattr_fd_get(int fd, const char *attribute, ssize_t *size)
{
#ifdef HAVE_XATTR
   char *ret = NULL;
   ssize_t tmp;

   EINA_SAFETY_ON_NULL_RETURN_VAL(attribute, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(size, NULL);

   *size = fgetxattr(fd, attribute, NULL, 0);
   /* Size should be less than 2MB (already huge in my opinion) */
   if (!(*size > 0 && *size < 2 * 1024 * 1024))
     goto on_error;

   ret = malloc(*size + 1);
   if (!ret) goto on_error;

   tmp = fgetxattr(fd, attribute, ret, *size);
   if (tmp != *size)
     goto on_error;
   ret[tmp] = '\0';

   return ret;

 on_error:
   free(ret);
   *size = 0;
   return NULL;
#else
   EINA_SAFETY_ON_NULL_RETURN_VAL(size, NULL);
   *size = 0;
   return NULL;
   (void)fd;
   (void)attribute;
#endif
}

Eina_Bool
eina_xattr_set(const char *file, const char *attribute, const void *data, ssize_t length, Eina_Xattr_Flags flags)
{
#ifdef HAVE_XATTR
   int iflags;

   EINA_SAFETY_ON_NULL_RETURN_VAL(file, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(attribute, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(data, EINA_FALSE);
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!(length > 0 && length < 2 * 1024 * 1024), EINA_FALSE);

   switch (flags)
     {
     case EINA_XATTR_INSERT: iflags = 0; break;
     case EINA_XATTR_REPLACE: iflags = XATTR_REPLACE; break;
     case EINA_XATTR_CREATED: iflags = XATTR_CREATE; break;
     default:
       return EINA_FALSE;
     }

   if (setxattr(file, attribute, data, length, iflags))
     return EINA_FALSE;
   return EINA_TRUE;
#else
   return EINA_FALSE;
   (void)file;
   (void)attribute;
   (void)data;
   (void)length;
   (void)flags;
#endif
}

Eina_Bool
eina_xattr_fd_set(int fd, const char *attribute, const void *data, ssize_t length, Eina_Xattr_Flags flags)
{
#ifdef HAVE_XATTR
   int iflags;

   EINA_SAFETY_ON_NULL_RETURN_VAL(attribute, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(data, EINA_FALSE);
   EINA_SAFETY_ON_TRUE_RETURN_VAL(!(length > 0 && length < 2 * 1024 * 1024), EINA_FALSE);

   switch (flags)
     {
     case EINA_XATTR_INSERT: iflags = 0; break;
     case EINA_XATTR_REPLACE: iflags = XATTR_REPLACE; break;
     case EINA_XATTR_CREATED: iflags = XATTR_CREATE; break;
     default:
       return EINA_FALSE;
     }

   if (fsetxattr(fd, attribute, data, length, iflags))
     return EINA_FALSE;
   return EINA_TRUE;
#else
   return EINA_FALSE;
   (void)fd;
   (void)attribute;
   (void)data;
   (void)length;
   (void)flags;
#endif
}

Eina_Bool
eina_xattr_del(const char *file, const char *attribute)
{
#ifdef HAVE_XATTR
   EINA_SAFETY_ON_NULL_RETURN_VAL(file, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(attribute, EINA_FALSE);
   return removexattr(file, attribute) == 0;
#else
   return EINA_FALSE;
   (void)file;
   (void)attribute;
#endif
}

Eina_Bool
eina_xattr_fd_del(int fd, const char *attribute)
{
#ifdef HAVE_XATTR
   EINA_SAFETY_ON_NULL_RETURN_VAL(attribute, EINA_FALSE);
   return fremovexattr(fd, attribute) == 0;
#else
   return EINA_FALSE;
   (void)fd;
   (void)attribute;
#endif
}

Eina_Bool
eina_xattr_string_set(const char *file, const char *attribute, const char *data, Eina_Xattr_Flags flags)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(data, EINA_FALSE);

   return eina_xattr_set(file, attribute, data, strlen(data) + 1, flags);
}

char *
eina_xattr_string_get(const char *file, const char *attribute)
{
   char *tmp;
   ssize_t size;

   tmp = eina_xattr_get(file, attribute, &size);
   if (!tmp) return NULL;

   if (tmp[size - 1] != '\0')
     {
        free(tmp);
        return NULL;
     }

   return tmp;
}

Eina_Bool
eina_xattr_double_set(const char *file, const char *attribute, double value, Eina_Xattr_Flags flags)
{
   char buffer[128];

   eina_convert_dtoa(value, buffer);
   return eina_xattr_string_set(file, attribute, buffer, flags);
}

Eina_Bool
eina_xattr_double_get(const char *file, const char *attribute, double *value)
{
   char *tmp;
   long long int m = 0;
   long int e = 0;

   EINA_SAFETY_ON_NULL_RETURN_VAL(value, EINA_FALSE);

   tmp = eina_xattr_string_get(file, attribute);
   if (!tmp) return EINA_FALSE;

   if (!eina_convert_atod(tmp, strlen(tmp), &m, &e))
     {
       free(tmp);
       return EINA_FALSE;
     }

   *value = ldexp((double)m, e);
   free(tmp);

   return EINA_TRUE;
}

Eina_Bool
eina_xattr_int_set(const char *file, const char *attribute, int value, Eina_Xattr_Flags flags)
{
   char buffer[10];

   eina_convert_itoa(value, buffer);
   return eina_xattr_string_set(file, attribute, buffer, flags);
}

Eina_Bool
eina_xattr_int_get(const char *file, const char *attribute, int *value)
{
   char *tmp;
   char *eos;
   Eina_Bool result;

   EINA_SAFETY_ON_NULL_RETURN_VAL(value, EINA_FALSE);

   tmp = eina_xattr_string_get(file, attribute);
   if (!tmp) return EINA_FALSE;

   *value = (int) strtol(tmp, &eos, 10);
   result = (*eos == '\0');
   free(tmp);

   return result;
}

Eina_Bool
eina_xattr_copy(const char *src, const char *dst)
{
#ifdef HAVE_XATTR
   int sfd, dfd;
   Eina_Bool ret = EINA_FALSE;

   EINA_SAFETY_ON_NULL_RETURN_VAL(src, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(dst, EINA_FALSE);

   sfd = open(src, O_RDONLY);
   EINA_SAFETY_ON_TRUE_GOTO(sfd < 0, s_end);

   dfd = open(dst, O_RDWR);
   EINA_SAFETY_ON_TRUE_GOTO(dfd < 0, d_end);

   ret = eina_xattr_fd_copy(sfd, dfd);

   close(dfd);
 d_end:
   close(sfd);
 s_end:
   return ret;

#else
   return EINA_FALSE;
   (void)src;
   (void)dst;
#endif
}

Eina_Bool
eina_xattr_fd_copy(int src, int dst)
{
#ifdef HAVE_XATTR
   char *buf, *attr;
   ssize_t i, length;
   Eina_Bool ret;

   length = flistxattr(src, NULL, 0);
   if (length == 0) return EINA_TRUE;
   EINA_SAFETY_ON_TRUE_RETURN_VAL(length < 0, EINA_FALSE);

   buf = malloc(length);
   EINA_SAFETY_ON_NULL_RETURN_VAL(buf, EINA_FALSE);

   attr = buf;
   length = flistxattr(src, attr, length);
   ret = EINA_TRUE;
   for (i = 0; i < length; i += strlen(attr) + 1, attr += strlen(attr) + 1)
     {
        ssize_t attr_length;
        void *value;

        attr_length = fgetxattr(src, attr, NULL, 0);
        EINA_SAFETY_ON_TRUE_GOTO(attr_length < 0, attr_error);

        if (attr_length == 0)
          value = NULL;
        else
          {
             value = malloc(attr_length);
             EINA_SAFETY_ON_NULL_GOTO(value, attr_error);

             attr_length = fgetxattr(src, attr, value, attr_length);
          }

        fsetxattr(dst, attr, value, attr_length, 0);
        free(value);
        continue;

     attr_error:
        ret = EINA_FALSE;
     }

   free(buf);
   return ret;

#else
   return EINA_FALSE;
   (void)src;
   (void)dst;
#endif
}
