/* EINA - EFL data type library
 * Copyright (C) 2020 Carlos Signor
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

#ifndef EINA_THREAD_WIN32_H
#define EINA_THREAD_WIN32_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "eina_thread.h"
#include "unimplemented.h"
#include <errno.h>

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#define EINA_THREAD_CANCEL_ENABLE 0
#define EINA_THREAD_CANCEL_DISABLE 1
#define EINA_THREAD_CANCEL_DEFERRED 0
#define EINA_THREAD_CANCEL_ASYNCHRONOUS 1
#define EINA_THREAD_CANCELED   ((void *) -1)

#define EINA_THREAD_CLEANUP_PUSH(cleanup, data)
#define EINA_THREAD_CLEANUP_POP(exec_cleanup)

struct _Eina_ThreadData
{
   /*
    * In calling contains the argument to the thread func
    * On exit, holds the return value of the thread
    */
   void *data;
   void *(*func)(void *data);
};

/**
 * @typedef Eina_Thread
 * Type for a generic thread.
 */
typedef struct _Eina_Thread
{
   HANDLE handle;
   DWORD id;
   struct _Eina_ThreadData *data;
} Eina_Thread;

typedef DWORD Eina_ThreadId;

#endif
