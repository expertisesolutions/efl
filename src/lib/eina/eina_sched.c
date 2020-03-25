/* EINA - EFL data type library
 * Copyright (C) 2010 ProFUSION embedded systems
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
#include "config.h"
#endif
#include "eina_thread.h"

#ifdef __linux__
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#endif

#include "eina_sched.h"
#include "eina_log.h"

EAPI void
eina_sched_prio_drop(void)
{
   _eina_sched_prio_drop();
}
