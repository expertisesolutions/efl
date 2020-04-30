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

#include <stdlib.h>
#include <eina_stdio.h>
#include <time.h>

#ifdef EINA_BENCH_HAVE_GLIB
# include <glib.h>
#endif

#include "Evas_Data.h"
#include "Ecore_Data.h"

#include "eina_bench.h"
#include "eina_array.h"
#include "eina_list.h"
#include "eina_inlist.h"
#include "eina_main.h"

typedef struct _Eina_Bench_Object Eina_Bench_Object;
struct _Eina_Bench_Object
{
   EINA_INLIST;

   void *somewhere;
   int data;
   Eina_Bool keep;
};

static Eina_Bool keep(void *data, EINA_UNUSED void *gdata)
{
   Eina_Bench_Object *bo = data;

   if (bo->keep == EINA_TRUE)
      return EINA_TRUE;

   free(bo);
   return EINA_FALSE;
}

static void
eina_bench_array_4evas_render_inline(int request)
{
   Eina_Array *array;
   Eina_Bench_Object *ebo;
   Eina_Array_Iterator it;
   unsigned int i;
   unsigned int j;

   srand(time(NULL));

   eina_init();

   array = eina_array_new(64);

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < (unsigned int)request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             eina_array_push(array, ebo);
          }

        if (i == 500)
          {
             EINA_ARRAY_ITER_NEXT(array, j, ebo, it) free(ebo);
             eina_array_clean(array);
          }
        else if ((i % 30) == 0)
          {
             eina_array_remove(array, keep, NULL);
          }

        EINA_ARRAY_ITER_NEXT(array, j, ebo, it)
          ebo->keep = rand() < (RAND_MAX / 2) ? ebo->keep : EINA_FALSE;
     }

        EINA_ARRAY_ITER_NEXT(array, j, ebo, it)
          free(ebo);

   eina_array_free(array);

   eina_shutdown();
}

static Eina_Bool
eina_iterator_ebo_free(EINA_UNUSED const Eina_Array *array,
                       Eina_Bench_Object *ebo,  EINA_UNUSED void *fdata)
{
   free(ebo);
   return EINA_TRUE;
}

static Eina_Bool
eina_iterator_ebo_rand(EINA_UNUSED const void *container,
                       Eina_Bench_Object *ebo,  EINA_UNUSED void *fdata)
{
   ebo->keep = rand() < (RAND_MAX / 2) ? ebo->keep : EINA_FALSE;
   return EINA_TRUE;
}

static void
eina_bench_array_4evas_render_iterator(int request)
{
   Eina_Array *array;
   Eina_Bench_Object *ebo;
   Eina_Iterator *it;
   unsigned int i;
   unsigned int j;

   srand(time(NULL));

   eina_init();

   array = eina_array_new(64);

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < (unsigned int)request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             eina_array_push(array, ebo);
          }

        if (i == 500)
          {
             it = eina_array_iterator_new(array);
             eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_ebo_free), NULL);
             eina_iterator_free(it);

             eina_array_clean(array);
          }
        else if ((i % 30) == 0)
          {
             eina_array_remove(array, keep, NULL);
          }

        it = eina_array_iterator_new(array);
        eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_ebo_rand), NULL);
        eina_iterator_free(it);
     }

   it = eina_array_iterator_new(array);
   eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_ebo_free), NULL);
   eina_iterator_free(it);

   eina_array_free(array);

   eina_shutdown();
}

static void
eina_bench_list_4evas_render(int request)
{
   Eina_List *list = NULL;
   Eina_List *tmp;
   Eina_Bench_Object *ebo;
   int i;
   int j;

   eina_init();

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             list = eina_list_prepend(list, ebo);
          }

        if (i == 500)
           while (list)
             {
                       free(eina_list_data_get(list));
                list = eina_list_remove_list(list, list);
             }
        else if ((i % 30) == 0)
          {
             tmp = list;
             while (tmp)
               {
                  Eina_List *reminder = tmp;

                  ebo = eina_list_data_get(reminder);
                  tmp = eina_list_next(tmp);

                  if (ebo->keep == EINA_FALSE)
                    {
                       list = eina_list_remove_list(list, reminder);
                       free(ebo);
                    }
               }
          }

        for (tmp = list; tmp; tmp = eina_list_next(tmp))
          {
             ebo = eina_list_data_get(tmp);

             ebo->keep = rand() < (RAND_MAX / 2) ? ebo->keep : EINA_FALSE;
          }
     }

   while (list)
     {
                       free(eina_list_data_get(list));
        list = eina_list_remove_list(list, list);
     }

   eina_shutdown();
}

static void
eina_bench_list_4evas_render_iterator(int request)
{
   Eina_List *list = NULL;
   Eina_List *tmp;
   Eina_Bench_Object *ebo;
   Eina_Iterator *it;
   int i;
   int j;

   eina_init();

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             list = eina_list_prepend(list, ebo);
          }

        if (i == 500)
           while (list)
             {
                       free(eina_list_data_get(list));
                list = eina_list_remove_list(list, list);
             }
        else if ((i % 30) == 0)
          {
             tmp = list;
             while (tmp)
               {
                  Eina_List *reminder = tmp;

                  ebo = eina_list_data_get(reminder);
                  tmp = eina_list_next(tmp);

                  if (ebo->keep == EINA_FALSE)
                    {
                       list = eina_list_remove_list(list, reminder);
                       free(ebo);
                    }
               }
          }

        it = eina_list_iterator_new(list);
        eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_ebo_rand), NULL);
        eina_iterator_free(it);
     }

   while (list)
     {
        free(eina_list_data_get(list));
        list = eina_list_remove_list(list, list);
     }

   eina_shutdown();
}

static void
eina_bench_inlist_4evas_render(int request)
{
   Eina_Inlist *head = NULL;
   Eina_Inlist *tmp;
   Eina_Bench_Object *ebo;
   int i;
   int j;

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             head = eina_inlist_prepend(head, EINA_INLIST_GET(ebo));
          }

        if (i == 500)
           while (head)
             {
                tmp = head;
                head = head->next;
                       free(tmp);
             }
        else if ((i % 30) == 0)
          {
             tmp = head;
             while(tmp)
               {
                  ebo = (Eina_Bench_Object *)tmp;

                  tmp = tmp->next;
                  if (ebo->keep == EINA_FALSE)
                    {
                       head = eina_inlist_remove(head, EINA_INLIST_GET(ebo));
                       free(ebo);
                    }
               }
          }

        EINA_INLIST_FOREACH(head, ebo)
        ebo->keep = rand() < (RAND_MAX / 2) ? ebo->keep : EINA_FALSE;
     }

   while (head)
     {
        tmp = head;
        head = head->next;
                       free(tmp);
     }
}

static void
eina_bench_inlist_4evas_render_iterator(int request)
{
   Eina_Inlist *head = NULL;
   Eina_Inlist *tmp;
   Eina_Bench_Object *ebo;
   Eina_Iterator *it;
   int i;
   int j;

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             head = eina_inlist_prepend(head, EINA_INLIST_GET(ebo));
          }

        if (i == 500)
           while (head)
             {
                tmp = head;
                head = head->next;
                       free(tmp);
             }
        else if ((i % 30) == 0)
          {
             tmp = head;
             while(tmp)
               {
                  ebo = (Eina_Bench_Object *)tmp;

                  tmp = tmp->next;
                  if (ebo->keep == EINA_FALSE)
                    {
                       head = eina_inlist_remove(head, EINA_INLIST_GET(ebo));
                       free(ebo);
                    }
               }
          }

        it = eina_inlist_iterator_new(head);
        eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_ebo_rand), NULL);
        eina_iterator_free(it);
     }

   while (head)
     {
        tmp = head;
        head = head->next;
                       free(tmp);
     }
}

#ifdef EINA_BENCH_HAVE_GLIB
static void
eina_bench_glist_4evas_render(int request)
{
   GList *list = NULL;
   GList *tmp;
   Eina_Bench_Object *ebo;
   int i;
   int j;

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             list = g_list_prepend(list, ebo);
          }

        if (i == 500)
           while (list)
             {
                       free(list->data);
                list = g_list_delete_link(list, list);
             }
        else if ((i % 30) == 0)
          {
             tmp = list;
             while (tmp)
               {
                  GList *reminder = tmp;

                  ebo = reminder->data;
                  tmp = g_list_next(tmp);

                  if (ebo->keep == EINA_FALSE)
                    {
                       list = g_list_delete_link(list, reminder);
                       free(ebo);
                    }
               }
          }

        for (tmp = list; tmp; tmp = g_list_next(tmp))
          {
             ebo = tmp->data;

             ebo->keep = rand() < (RAND_MAX / 2) ? ebo->keep : EINA_FALSE;
          }
     }

   while (list)
     {
                       free(list->data);
        list = g_list_delete_link(list, list);
     }
}

static void
eina_bench_gptrarray_4evas_render(int request)
{
   GPtrArray *array = NULL;
   Eina_Bench_Object *ebo;
   unsigned int j;
   int i;

   array = g_ptr_array_new();

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < (unsigned int)request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             g_ptr_array_add(array, ebo);
          }

        if (i == 500)
          {
             for (j = 0; j < array->len; ++j)
                free(g_ptr_array_index(array, j));
             g_ptr_array_remove_range(array, 0, array->len);
          }
        else if ((i % 30) == 0)
           for (j = 0; j < array->len; )
             {
                ebo = g_ptr_array_index(array, j);

                if (ebo->keep == EINA_FALSE)
                   free(g_ptr_array_remove_index_fast(array, j));
                else
                   j++;
             }

        for (j = 0; j < array->len; ++j)
          {
             ebo = g_ptr_array_index(array, j);

             ebo->keep = rand() < (RAND_MAX / 2) ? ebo->keep : EINA_FALSE;
          }
     }

   for (j = 0; j < array->len; ++j)
                   free(g_ptr_array_index(array, j));
                   g_ptr_array_free(array, TRUE);
}
#endif

static void
eina_bench_evas_list_4evas_render(int request)
{
   Evas_List *list = NULL;
   Evas_List *tmp;
   Eina_Bench_Object *ebo;
   int i;
   int j;

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             list = evas_list_prepend(list, ebo);
          }

        if (i == 500)
           while (list)
             {
                       free(evas_list_data(list));
                list = evas_list_remove_list(list, list);
             }
        else if ((i % 30) == 0)
          {
             tmp = list;
             while (tmp)
               {
                  Evas_List *reminder = tmp;

                  ebo = evas_list_data(reminder);
                  tmp = evas_list_next(tmp);

                  if (ebo->keep == EINA_FALSE)
                    {
                       list = evas_list_remove_list(list, reminder);
                       free(ebo);
                    }
               }
          }

        for (tmp = list; tmp; tmp = evas_list_next(tmp))
          {
             ebo = evas_list_data(tmp);

             ebo->keep = rand() < (RAND_MAX / 2) ? ebo->keep : EINA_FALSE;
          }
     }

   while (list)
     {
                       free(evas_list_data(list));
        list = evas_list_remove_list(list, list);
     }
}

static void
_eina_ecore_for_each_remove(void *value, void *user_data)
{
   Eina_Bench_Object *ebo = value;
   Ecore_List *list = user_data;

   if (ebo->keep == EINA_FALSE)
      ecore_list_remove_destroy(list);
}

static void
_eina_ecore_for_each_rand(void *value, EINA_UNUSED void *user_data)
{
   Eina_Bench_Object *ebo = value;

   ebo->keep = rand() < (RAND_MAX / 2) ? ebo->keep : EINA_FALSE;
}

static void
eina_bench_ecore_list_4evas_render(int request)
{
   Ecore_List *list = NULL;
   Eina_Bench_Object *ebo;
   int i;
   int j;

   list = ecore_list_new();
   ecore_list_free_cb_set(list, free);

   for (i = 0; i < 1000; ++i)
     {
        for (j = 0; j < request; ++j)
          {
             ebo = malloc(sizeof (Eina_Bench_Object));
             if (!ebo)
                continue;

             ebo->keep = rand() < (RAND_MAX / 2) ? EINA_TRUE : EINA_FALSE;

             ecore_list_prepend(list, ebo);
          }

        if (i == 500)
           ecore_list_clear(list);
        else if ((i % 30) == 0)
          {
             ecore_list_for_each(list, _eina_ecore_for_each_remove, list);
          }

        ecore_list_for_each(list, _eina_ecore_for_each_rand,   list);
     }

           ecore_list_destroy(list);
}

void eina_bench_array(Eina_Benchmark *bench)
{
   eina_benchmark_register(bench, "array-inline",
                           EINA_BENCHMARK(
                              eina_bench_array_4evas_render_inline),    200,
                           4000, 100);
   eina_benchmark_register(bench, "array-iterator",
                           EINA_BENCHMARK(
                              eina_bench_array_4evas_render_iterator),  200,
                           4000, 100);
   eina_benchmark_register(bench, "list",
                           EINA_BENCHMARK(
                              eina_bench_list_4evas_render),            200,
                           4000, 100);
   eina_benchmark_register(bench, "list-iterator",
                           EINA_BENCHMARK(
                              eina_bench_list_4evas_render_iterator),   200,
                           4000, 100);
   eina_benchmark_register(bench, "inlist",
                           EINA_BENCHMARK(
                              eina_bench_inlist_4evas_render),          200,
                           4000, 100);
   eina_benchmark_register(bench, "inlist-iterator",
                           EINA_BENCHMARK(
                              eina_bench_inlist_4evas_render_iterator), 200,
                           4000, 100);
#ifdef EINA_BENCH_HAVE_GLIB
   eina_benchmark_register(bench, "glist",
                           EINA_BENCHMARK(
                              eina_bench_glist_4evas_render),           200,
                           4000, 100);
   eina_benchmark_register(bench, "gptrarray",
                           EINA_BENCHMARK(
                              eina_bench_gptrarray_4evas_render),       200,
                           4000, 100);
#endif
   eina_benchmark_register(bench, "evas",
                           EINA_BENCHMARK(
                              eina_bench_evas_list_4evas_render),       200,
                           4000, 100);
   eina_benchmark_register(bench, "ecore",
                           EINA_BENCHMARK(
                              eina_bench_ecore_list_4evas_render),      200,
                           500, 100);
}

