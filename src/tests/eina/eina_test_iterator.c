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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>

#include <Eina.h>

#include "eina_suite.h"

struct Number{
   int number;
};

static Eina_Bool
_filter(const void *it EINA_UNUSED, void *data, void *fdata)
{
   struct Number *numb = data;

   ck_assert_ptr_eq(fdata, (void*)1);
   return numb->number % 2;
}

EFL_START_TEST(eina_iterator_filter_simple)
{
   Eina_Iterator *it, *filtered;
   Eina_Array *ea;

   ea = eina_array_new(11);
   fail_if(!ea);

   for(int i = 0; i <= 10; i++)
     {
        struct Number *number;

        number = calloc(1, sizeof(struct Number));
        number->number = i;

        eina_array_push(ea, number);
     }

   it = eina_array_iterator_new(ea);
   filtered = eina_iterator_filter_new(it, _filter, NULL, (void*)1);
   for(int i = 1; i <= 10; i+=2)
     {
        struct Number *numb;
        Eina_Bool ret;

        ret = eina_iterator_next(filtered, (void**)&numb);

        fail_if(!ret);
        ck_assert_int_eq(numb->number, i);
     }

   {
      struct Number *numb;
      Eina_Bool ret;

      ret = eina_iterator_next(filtered, (void**)&numb);
      fail_if(ret);
   }


   eina_iterator_free(it);

   eina_array_free(ea);

}
EFL_END_TEST

struct Free_test {
   Eina_Bool got_freed;
};

static Eina_Bool
_filter_free(const void *it EINA_UNUSED, void *data EINA_UNUSED, void *fdata EINA_UNUSED)
{
   return EINA_TRUE;
}

static void
_free(void *data)
{
   struct Free_test *test = data;
   test->got_freed = EINA_TRUE;
}

EFL_START_TEST(eina_iterator_filter_free)
{
   Eina_Iterator *it, *filtered;
   Eina_Array *ea;
   struct Free_test *data;

   data = calloc(1, sizeof(struct Free_test));
   ea = eina_array_new(11);
   fail_if(!ea);

   it = eina_array_iterator_new(ea);
   filtered = eina_iterator_filter_new(it, _filter_free, _free, data);
   eina_iterator_free(filtered);

   eina_array_free(ea);

   ck_assert_int_ne(data->got_freed, 0);

   free(data);

}
EFL_END_TEST
static Eina_Bool
eina_iterator_array_check(EINA_UNUSED const Eina_Array *array,
                          int *data,  int *fdata)
{
   fail_if(*fdata > *data);
   *fdata = *data;

   return EINA_TRUE;
}

EFL_START_TEST(eina_iterator_array_simple)
{
   Eina_Iterator *it;
   Eina_Array *ea;
   int *tmp;
   int i;

   ea = eina_array_new(11);
        fail_if(!ea);

   for (i = 0; i < 200; ++i)
     {
        tmp = malloc(sizeof(int));
        fail_if(!tmp);
        *tmp = i;

        eina_array_push(ea, tmp);
     }

   it = eina_array_iterator_new(ea);
   fail_if(!it);

   i = -1;
   eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_array_check), &i);
   fail_if(i != 199);

   fail_if(eina_iterator_container_get(it) != ea);
   fail_if(eina_iterator_next(it, (void **)&tmp) != EINA_FALSE);

   eina_iterator_free(it);

   eina_array_free(ea);

}
EFL_END_TEST

static Eina_Bool
eina_iterator_hash_key_check(const Eina_Hash *hash,
                             const char *key,
                             EINA_UNUSED void *fdata)
{
   fail_if(eina_hash_find(hash, key) == NULL);

   return EINA_TRUE;
}
static Eina_Bool
eina_iterator_hash_data_check(const Eina_Hash *hash,
                              int *data,
                              EINA_UNUSED void *fdata)
{
   char tmp[10];

   snprintf(tmp, 10, "%i", *data);
   fail_if(eina_hash_find(hash, tmp) != data);

   return EINA_TRUE;
}
static Eina_Bool
eina_iterator_hash_tuple_check(EINA_UNUSED const Eina_Hash *hash,
                               Eina_Hash_Tuple *tuple,
                               EINA_UNUSED void *fdata)
{
   fail_if(atoi((char *)tuple->key) != *((int *)tuple->data));

   return EINA_TRUE;
}

EFL_START_TEST(eina_iterator_hash_simple)
{
   Eina_Iterator *it;
   Eina_Hash *hash;
   int array[] = { 1, 42, 7, 8, 6 };


   hash = eina_hash_string_superfast_new(NULL);
   fail_if(hash == NULL);

   fail_if(eina_hash_add(hash, "1", &array[0]) != EINA_TRUE);
   fail_if(eina_hash_add(hash, "42", &array[1]) != EINA_TRUE);
   fail_if(eina_hash_add(hash, "7", &array[2]) != EINA_TRUE);
   fail_if(eina_hash_add(hash, "8", &array[3]) != EINA_TRUE);
   fail_if(eina_hash_add(hash, "6", &array[4]) != EINA_TRUE);

   it = eina_hash_iterator_key_new(hash);
   eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_hash_key_check), NULL);
   eina_iterator_free(it);

   it = eina_hash_iterator_data_new(hash);
   eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_hash_data_check), NULL);
   eina_iterator_free(it);

   it = eina_hash_iterator_tuple_new(hash);
   eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_hash_tuple_check), NULL);
   eina_iterator_free(it);

   eina_hash_free(hash);

}
EFL_END_TEST

typedef struct _Eina_Test_Inlist Eina_Test_Inlist;
struct _Eina_Test_Inlist
{
   EINA_INLIST;
   int i;
};

static Eina_Test_Inlist *
_eina_test_inlist_build(int i)
{
   Eina_Test_Inlist *tmp;

   tmp = malloc(sizeof(Eina_Test_Inlist));
   fail_if(!tmp);
   tmp->i = i;

   return tmp;
}

static Eina_Bool
eina_iterator_inlist_data_check(EINA_UNUSED const Eina_Inlist *in_list,
                                Eina_Test_Inlist *data,
                                int *fdata)
{
   switch (*fdata)
     {
      case 0: fail_if(data->i != 27); break;

      case 1: fail_if(data->i != 42); break;

      case 2: fail_if(data->i != 3227); break;

      case 3: fail_if(data->i != 1664); break;

      case 4: fail_if(data->i != 81); break;
     }

   (*fdata)++;

   return EINA_TRUE;
}

EFL_START_TEST(eina_iterator_inlist_simple)
{
   Eina_Inlist *lst = NULL;
   Eina_Test_Inlist *tmp;
   Eina_Test_Inlist *prev;
   Eina_Iterator *it;
   int i = 0;

   tmp = _eina_test_inlist_build(42);
   lst = eina_inlist_append(lst, EINA_INLIST_GET(tmp));
   fail_if(!lst);

   tmp = _eina_test_inlist_build(1664);
   lst = eina_inlist_append_relative(lst, EINA_INLIST_GET(tmp), lst);
   fail_if(!lst);
   fail_if(((Eina_Test_Inlist *)lst)->i != 42);

   prev = tmp;
   tmp = _eina_test_inlist_build(3227);
   lst = eina_inlist_prepend_relative(lst, EINA_INLIST_GET(
                                         tmp), EINA_INLIST_GET(prev));
   fail_if(!lst);
   fail_if(((Eina_Test_Inlist *)lst)->i != 42);

   tmp = _eina_test_inlist_build(27);
   lst = eina_inlist_prepend_relative(lst, EINA_INLIST_GET(tmp), NULL);

   tmp = _eina_test_inlist_build(81);
   lst = eina_inlist_append_relative(lst, EINA_INLIST_GET(tmp), NULL);

   it = eina_inlist_iterator_new(lst);
   fail_if(!it);

   eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_inlist_data_check), &i);
   eina_iterator_free(it);

   fail_if(i != 5);
}
EFL_END_TEST

static Eina_Bool
eina_iterator_list_data_check(EINA_UNUSED const Eina_List *list,
                              int *data,
                              int *fdata)
{
   switch (*fdata)
     {
      case 0: fail_if(*data != 81); break;

      case 1: fail_if(*data != 7); break;

      case 2: fail_if(*data != 9); break;

      case 3: fail_if(*data != 6); break;

      case 4: fail_if(*data != 42); break;

      case 5: fail_if(*data != 1); break;

      case 6: fail_if(*data != 1337); break;
     }

   (*fdata)++;

   return EINA_TRUE;
}

EFL_START_TEST(eina_iterator_list_simple)
{
   Eina_List *list = NULL;
   Eina_Iterator *it;
   int data[] = { 6, 9, 42, 1, 7, 1337, 81, 1664 };
   int i = 0;


   list = eina_list_append(list, &data[0]);
   fail_if(list == NULL);

   list = eina_list_prepend(list, &data[1]);
   fail_if(list == NULL);

   list = eina_list_append(list, &data[2]);
   fail_if(list == NULL);

   list = eina_list_append(list, &data[3]);
   fail_if(list == NULL);

   list = eina_list_prepend(list, &data[4]);
   fail_if(list == NULL);

   list = eina_list_append(list, &data[5]);
   fail_if(list == NULL);

   list = eina_list_prepend(list, &data[6]);
   fail_if(list == NULL);

   it = eina_list_iterator_new(list);
   fail_if(!it);

   eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_list_data_check), &i);
   eina_iterator_free(it);
}
EFL_END_TEST

static Eina_Bool
eina_reverse_iterator_list_data_check(EINA_UNUSED const Eina_List *list,
                              int *data,
                              int *fdata)
{
   switch (*fdata)
     {
      case 0: fail_if(*data != 1337); break;

      case 1: fail_if(*data != 1); break;

      case 2: fail_if(*data != 42); break;

      case 3: fail_if(*data != 6); break;

      case 4: fail_if(*data != 9); break;

      case 5: fail_if(*data != 7); break;

      case 6: fail_if(*data != 81); break;
     }

   (*fdata)++;

   return EINA_TRUE;
}

EFL_START_TEST(eina_reverse_iterator_list_simple)
{
   Eina_List *list = NULL;
   Eina_Iterator *it;
   int data[] = { 6, 9, 42, 1, 7, 1337, 81, 1664 };
   int i = 0;


   list = eina_list_append(list, &data[0]);
   fail_if(list == NULL);

   list = eina_list_prepend(list, &data[1]);
   fail_if(list == NULL);

   list = eina_list_append(list, &data[2]);
   fail_if(list == NULL);

   list = eina_list_append(list, &data[3]);
   fail_if(list == NULL);

   list = eina_list_prepend(list, &data[4]);
   fail_if(list == NULL);

   list = eina_list_append(list, &data[5]);
   fail_if(list == NULL);

   list = eina_list_prepend(list, &data[6]);
   fail_if(list == NULL);

   it = eina_list_iterator_reversed_new(list);
   fail_if(!it);

   eina_iterator_foreach(it, EINA_EACH_CB(eina_reverse_iterator_list_data_check), &i);
   eina_iterator_free(it);
}
EFL_END_TEST

typedef struct _Eina_Rbtree_Int Eina_Rbtree_Int;
struct _Eina_Rbtree_Int
{
   Eina_Rbtree node;
   int value;
};

static Eina_Rbtree_Direction
eina_rbtree_int_cmp(const Eina_Rbtree_Int *left, const Eina_Rbtree_Int *right, void *data EINA_UNUSED)
{
   fail_if(!left);
   fail_if(!right);

   if (left->value < right->value)
      return EINA_RBTREE_LEFT;

   return EINA_RBTREE_RIGHT;
}

static Eina_Rbtree *
_eina_rbtree_int_new(int value)
{
   Eina_Rbtree_Int *it;

   it = malloc(sizeof (Eina_Rbtree_Int));
   fail_if(!it);

   it->value = value;

   return &it->node;
}

static Eina_Bool
eina_iterator_rbtree_data_check_sorted(EINA_UNUSED const Eina_List *list,
                                       Eina_Rbtree_Int *data,
                                       int *fdata)
{
   switch (*fdata)
     {
      case 0: fail_if(data->value != 10); break;

      case 1: fail_if(data->value != 27); break;

      case 2: fail_if(data->value != 42); break;

      case 3: fail_if(data->value != 69); break;

      case 4: fail_if(data->value != 1337); break;
     }

   (*fdata)++;

   return EINA_TRUE;
}

static Eina_Bool
eina_iterator_rbtree_data_check_prefix(EINA_UNUSED const Eina_List *list,
                                       Eina_Rbtree_Int *data,
                                       int *fdata)
{
   switch (*fdata)
     {
      case 0: fail_if(data->value != 27); break;

      case 1: fail_if(data->value != 10); break;

      case 2: fail_if(data->value != 69); break;

      case 3: fail_if(data->value != 42); break;

      case 4: fail_if(data->value != 1337); break;
     }

   (*fdata)++;

   return EINA_TRUE;
}

static Eina_Bool
eina_iterator_rbtree_data_check_postfix(EINA_UNUSED const Eina_List *list,
                                        Eina_Rbtree_Int *data,
                                        int *fdata)
{
   switch (*fdata)
     {
      case 0: fail_if(data->value != 10); break;

      case 1: fail_if(data->value != 42); break;

      case 2: fail_if(data->value != 1337); break;

      case 3: fail_if(data->value != 69); break;

      case 4: fail_if(data->value != 27); break;
     }

   (*fdata)++;

   return EINA_TRUE;
}

EFL_START_TEST(eina_iterator_rbtree_simple)
{
   Eina_Rbtree *root = NULL;
   Eina_Iterator *it;
   int i;

   root = eina_rbtree_inline_insert(NULL,
				    _eina_rbtree_int_new(10),
                                    EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
				    NULL);
   fail_if(!root);

   root = eina_rbtree_inline_insert(root,
				    _eina_rbtree_int_new(1337),
                                    EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
				    NULL);
   fail_if(!root);

   root = eina_rbtree_inline_insert(root,
				    _eina_rbtree_int_new(27),
                                    EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
				    NULL);
   fail_if(!root);

   root = eina_rbtree_inline_insert(root,
				    _eina_rbtree_int_new(69),
                                    EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
				    NULL);
   fail_if(!root);

   root = eina_rbtree_inline_insert(root,
				    _eina_rbtree_int_new(42),
                                    EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
				    NULL);
   fail_if(!root);

   i = 0;
   it = eina_rbtree_iterator_prefix(root);
   fail_if(!it);

   eina_iterator_foreach(it,
			 EINA_EACH_CB(eina_iterator_rbtree_data_check_prefix),
			 &i);
   eina_iterator_free(it);

   /* This will return the item sorted. */
   i = 0;
   it = eina_rbtree_iterator_infix(root);
   fail_if(!it);

   eina_iterator_foreach(it,
			 EINA_EACH_CB(eina_iterator_rbtree_data_check_sorted),
			 &i);
   eina_iterator_free(it);

   i = 0;
   it = eina_rbtree_iterator_postfix(root);
   fail_if(!it);

   eina_iterator_foreach(it,
			 EINA_EACH_CB(eina_iterator_rbtree_data_check_postfix),
			 &i);
   eina_iterator_free(it);
}
EFL_END_TEST

EFL_START_TEST(eina_iterator_carray_length)
{
   unsigned int array[] = { 1, 4, 9, 16 };
   Eina_Iterator *it;
   unsigned int j = 1;
   unsigned int i = 0;

   it = EINA_C_ARRAY_ITERATOR_NEW(array);
   EINA_ITERATOR_FOREACH(it, i)
     {
        fail_if(i != j * j);
        j++;
     }
   fail_if(j < EINA_C_ARRAY_LENGTH(array));
   eina_iterator_free(it);
}
EFL_END_TEST

EFL_START_TEST(eina_iterator_multi)
{
   unsigned int array1[] = { 1, 4, 9, 16 };
   unsigned int array2[] = { 25, 36, 49, 64 };
   unsigned int array3[] = { 81, 100, 121, 144 };
   Eina_Iterator *it;
   unsigned int i;
   unsigned int j = 1;

   it = eina_multi_iterator_new(EINA_C_ARRAY_ITERATOR_NEW(array1),
                                EINA_C_ARRAY_ITERATOR_NEW(array2),
                                EINA_C_ARRAY_ITERATOR_NEW(array3));
   EINA_ITERATOR_FOREACH(it, i)
     {
        fail_if(i != j * j);
        j++;
     }
   fail_if(j < EINA_C_ARRAY_LENGTH(array1)
             + EINA_C_ARRAY_LENGTH(array2)
             + EINA_C_ARRAY_LENGTH(array3));
   eina_iterator_free(it);
}
EFL_END_TEST

static void*
_return_x(const void *container EINA_UNUSED, void *data, void *fdata)
{
   Eina_Rectangle *rect = data;
   ck_assert_int_eq(*((int*)fdata), 1337);

   return &rect->x;
}

static void
_free_cb(void *data)
{
   int *free_data = data;

   *free_data = 0;
}

EFL_START_TEST(eina_iterator_process)
{
   Eina_Inarray *rects = eina_inarray_new(sizeof(Eina_Rectangle), 5);
   Eina_Rectangle rect_arr[] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}, {16, 17, 18, 19}};
   Eina_Iterator *it;
   int free = 1337;
   int *a, i = 0;

   eina_inarray_push(rects, &rect_arr[0]);
   eina_inarray_push(rects, &rect_arr[1]);
   eina_inarray_push(rects, &rect_arr[2]);
   eina_inarray_push(rects, &rect_arr[3]);
   eina_inarray_push(rects, &rect_arr[4]);

   it = eina_iterator_processed_new(eina_inarray_iterator_new(rects), _return_x, _free_cb, &free);
   EINA_ITERATOR_FOREACH(it, a)
     {
         ck_assert_int_eq(*a, i*4);
         i++;
     }
   ck_assert_int_eq(i, 5);
   eina_iterator_free(it);
   ck_assert_int_eq(free, 0);
}
EFL_END_TEST

void
eina_test_iterator(TCase *tc)
{
   tcase_add_test(tc, eina_iterator_array_simple);
   tcase_add_test(tc, eina_iterator_hash_simple);
   tcase_add_test(tc, eina_iterator_inlist_simple);
   tcase_add_test(tc, eina_iterator_list_simple);
   tcase_add_test(tc, eina_reverse_iterator_list_simple);
   tcase_add_test(tc, eina_iterator_rbtree_simple);
   tcase_add_test(tc, eina_iterator_filter_simple);
   tcase_add_test(tc, eina_iterator_filter_free);
   tcase_add_test(tc, eina_iterator_carray_length);
   tcase_add_test(tc, eina_iterator_multi);
   tcase_add_test(tc, eina_iterator_process);
}
