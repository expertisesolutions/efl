/* EINA - EFL data type library
 * Copyright (C) 2008 Cedric Bail
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

#ifndef EINA_SUITE_H_
#define EINA_SUITE_H_

#include <check.h>
#include "../efl_check.h"

void eina_test_ustringshare(TCase *tc);
void eina_test_binshare(TCase *tc);
void eina_test_inarray(TCase *tc);
void eina_test_array(TCase *tc);
void eina_test_log(TCase *tc);
void eina_test_error(TCase *tc);
void eina_test_magic(TCase *tc);
void eina_test_inlist(TCase *tc);
void eina_test_clist(TCase *tc);
void eina_test_lalloc(TCase *tc);
void eina_test_main(TCase *tc);
void eina_test_counter(TCase *tc);
void eina_test_hash(TCase *tc);
void eina_test_list(TCase *tc);
void eina_test_iterator(TCase *tc);
void eina_test_accessor(TCase *tc);
void eina_test_module(TCase *tc);
void eina_test_convert(TCase *tc);
void eina_test_rbtree(TCase *tc);
void eina_test_thread(TCase *tc);
void eina_test_file(TCase *tc);
void eina_test_benchmark(TCase *tc);
void eina_test_mempool(TCase *tc);
void eina_test_rectangle(TCase *tc);
void eina_test_range(TCase *tc);
void eina_test_matrixsparse(TCase *tc);
void eina_test_tiler(TCase *tc);
void eina_test_strbuf(TCase *tc);
void eina_test_binbuf(TCase *tc);
void eina_test_str(TCase *tc);
void eina_test_ustr(TCase *tc);
void eina_test_quadtree(TCase *tc);
void eina_test_fp(TCase *tc);
void eina_test_sched(TCase *tc);
void eina_test_simple_xml_parser(TCase *tc);
void eina_test_value(TCase *tc);
void eina_test_model(TCase *tc);
void eina_test_cow(TCase *tc);
void eina_test_barrier(TCase *tc);
void eina_test_tmpstr(TCase *tc);
void eina_test_locking(TCase *tc);
void eina_test_abi(TCase *tc);
void eina_test_trash(TCase *tc);
void eina_test_xattr(TCase *tc);
void eina_test_crc(TCase *tc);
void eina_test_quad(TCase *tc);
void eina_test_matrix(TCase *tc);
void eina_test_quaternion(TCase *tc);
void eina_test_vector(TCase *tc);
void eina_test_promise(TCase *tc);
void eina_test_bezier(TCase *tc);
void eina_test_safepointer(TCase *tc);
void eina_test_slice(TCase *tc);
void eina_test_freeq(TCase *tc);
void eina_test_slstr(TCase *tc);
void eina_test_vpath(TCase *tc);
void eina_test_debug(TCase *tc);
void eina_test_abstract_content(TCase *tc);

#endif /* EINA_SUITE_H_ */
