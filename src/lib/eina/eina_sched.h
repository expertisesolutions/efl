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

/**
 * @defgroup Schedule Schedule
 * @ingroup Eina_Tools_Group
 *
 * @{
 *
 * TODO: description
 *
 */

#ifndef EINA_SCHED_H_
# define EINA_SCHED_H_
# ifdef HAVE_CONFIG_H
#   include "config.h"
# endif
# include "eina_types.h"
# include "eina_thread.h"
# include "eina_log.h"
# define RTNICENESS 1
# define NICENESS 5

/**
 * @brief Lowers the priority of the current thread.
 *
 * @details It's used by worker threads so that they use up the background CPU and do not stall
 *          the main thread. If the current thread is running with real-time priority, we
 *          decrease our priority by @c RTNICENESS. This is done in a portable way.
 *
 *          Otherwise, (we are running with the SCHED_OTHER policy) there's no portable way to
 *          set the nice level on the current thread. In Linux, it does work and it's the
 *          only one that is implemented as of now. In this case, the nice level is
 *          incremented on this thread by @c NICENESS.
 */
EAPI void eina_sched_prio_drop(void);

EAPI void eina_sched_prio_init(Eina_Thread_Call*);

/**
 * @}
 */

#endif /* EINA_SCHED_H_ */
