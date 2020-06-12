/* EINA - EFL data type library
 * Copyright (C) 2013 ProFUSION embedded systems
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

#include <Eina.h>

#include "eina_suite.h"

EFL_START_TEST(eina_thread_test_self_id)
{
    ck_assert(eina_thread_self_id() == eina_thread_id(eina_thread_self()));
}
EFL_END_TEST

void
eina_test_thread(TCase *tc)
{
   tcase_add_test(tc, eina_thread_test_self_id);
}
