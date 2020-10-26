/* EINA - EFL data type library
 * Copyright (C) 2009 Gustavo Sverzut Barbieri
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
 * @page tutorial_matrixsparse_page Sparse Matrix Tutorial
 *
 * to be written...
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "eina_config.h"
#include "eina_private.h"
#include "eina_log.h"
#include "eina_magic.h"
#include "eina_mempool.h"

/* undefs EINA_ARG_NONULL() so NULL checks are not compiled out! */
#include "eina_safety_checks.h"
#include "eina_matrixsparse.h"


/*============================================================================*
*                                  Local                                     *
*============================================================================*/

/**
 * @cond LOCAL
 */

static const char EINA_MAGIC_MATRIXSPARSE_STR[] = "Eina Matrixsparse";
static const char EINA_MAGIC_MATRIXSPARSE_ROW_STR[] = "Eina Matrixsparse Row";
static const char EINA_MAGIC_MATRIXSPARSE_CELL_STR[] = "Eina Matrixsparse Cell";
static const char EINA_MAGIC_MATRIXSPARSE_ITERATOR_STR[] =
   "Eina Matrixsparse Iterator";
static const char EINA_MAGIC_MATRIXSPARSE_ROW_ACCESSOR_STR[] =
   "Eina Matrixsparse Row Accessor";
static const char EINA_MAGIC_MATRIXSPARSE_ROW_ITERATOR_STR[] =
   "Eina Matrixsparse Row Iterator";
static const char EINA_MAGIC_MATRIXSPARSE_CELL_ACCESSOR_STR[] =
   "Eina Matrixsparse Cell Accessor";
static const char EINA_MAGIC_MATRIXSPARSE_CELL_ITERATOR_STR[] =
   "Eina Matrixsparse Cell Iterator";


#define EINA_MAGIC_CHECK_MATRIXSPARSE(d, ...)           \
   do {                                                  \
        if (!EINA_MAGIC_CHECK(d, EINA_MAGIC_MATRIXSPARSE)) \
          {                                                \
             EINA_MAGIC_FAIL(d, EINA_MAGIC_MATRIXSPARSE);  \
             return __VA_ARGS__;                           \
          }                                                \
     } while(0)

#define EINA_MAGIC_CHECK_MATRIXSPARSE_ROW(d, ...)               \
   do {                                                          \
        if (!EINA_MAGIC_CHECK(d, EINA_MAGIC_MATRIXSPARSE_ROW))     \
          {                                                        \
             EINA_MAGIC_FAIL(d, EINA_MAGIC_MATRIXSPARSE_ROW);      \
             return __VA_ARGS__;                                   \
          }                                                        \
     } while(0)

#define EINA_MAGIC_CHECK_MATRIXSPARSE_CELL(d, ...)              \
   do {                                                          \
        if (!EINA_MAGIC_CHECK(d, EINA_MAGIC_MATRIXSPARSE_CELL))    \
          {                                                        \
             EINA_MAGIC_FAIL(d, EINA_MAGIC_MATRIXSPARSE_CELL);     \
             return __VA_ARGS__;                                   \
          }                                                        \
     } while(0)

#define EINA_MAGIC_CHECK_MATRIXSPARSE_ITERATOR(d, ...)                  \
   do {                                                                  \
        if (!EINA_MAGIC_CHECK(d, EINA_MAGIC_MATRIXSPARSE_ITERATOR))        \
          {                                                                \
             EINA_MAGIC_FAIL(d, EINA_MAGIC_MATRIXSPARSE_ITERATOR);         \
             return __VA_ARGS__;                                           \
          }                                                                \
     } while(0)

struct _Eina_Matrixsparse_Cell
{
   Eina_Matrixsparse_Cell *next;
   Eina_Matrixsparse_Cell *prev;

   void *data;
   unsigned long col;

   Eina_Matrixsparse_Row *parent;

   EINA_MAGIC
};

struct _Eina_Matrixsparse_Row
{
   Eina_Matrixsparse_Row *next;
   Eina_Matrixsparse_Row *prev;

   Eina_Matrixsparse_Cell *cols;
   Eina_Matrixsparse_Cell *last_col;
   Eina_Matrixsparse_Cell *last_used; /* fast sequential access */
   unsigned long row;

   Eina_Matrixsparse *parent;

   EINA_MAGIC
};

struct _Eina_Matrixsparse
{
   Eina_Matrixsparse_Row *rows;
   Eina_Matrixsparse_Row *last_row;
   Eina_Matrixsparse_Row *last_used; /* fast sequential access */

   struct
   {
      unsigned long rows;
      unsigned long cols;
   } size;

   struct
   {
      void (*func)(void *user_data, void *cell_data);
      void *user_data;
   } free;

   EINA_MAGIC
};

typedef struct _Eina_Matrixsparse_Iterator Eina_Matrixsparse_Iterator;
typedef struct _Eina_Matrixsparse_Iterator_Complete
Eina_Matrixsparse_Iterator_Complete;

struct _Eina_Matrixsparse_Iterator
{
   Eina_Iterator iterator;

   const Eina_Matrixsparse *m;
   struct
   {
      const Eina_Matrixsparse_Row *row;
      const Eina_Matrixsparse_Cell *col;
   } ref;

   EINA_MAGIC
};

struct _Eina_Matrixsparse_Iterator_Complete
{
   Eina_Iterator iterator;

   const Eina_Matrixsparse *m;
   struct
   {
      const Eina_Matrixsparse_Row *row;
      const Eina_Matrixsparse_Cell *col;
   } ref;

   struct
   {
      unsigned long row, col;
   } idx;

   struct
   {
      Eina_Matrixsparse_Row row;
      Eina_Matrixsparse_Cell col;
   } dummy;

   EINA_MAGIC
};

/**
 * @todo Eina_Matrixsparse_Row_Iterator: iterator over rows in matrix
 * @todo Eina_Matrixsparse_Row_Accessor: accessor over rows in matrix
 * @todo Eina_Matrixsparse_Cell_Iterator: iterator over cells in row
 * @todo Eina_Matrixsparse_Cell_Accessor: accessor over cells in row
 */

static int _eina_matrixsparse_log_dom = -1;

#ifdef ERR
#undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_eina_matrixsparse_log_dom, __VA_ARGS__)

#ifdef DBG
#undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_eina_matrixsparse_log_dom, __VA_ARGS__)

static Eina_Mempool *_eina_matrixsparse_cell_mp = NULL;
static Eina_Mempool *_eina_matrixsparse_row_mp = NULL;

static inline void
_eina_matrixsparse_cell_free(Eina_Matrixsparse_Cell *c, void (*free_func)(
                                void *,
                                void *), void *user_data)
{
   if (free_func)
      free_func(user_data, c->data);

   EINA_MAGIC_SET(c, EINA_MAGIC_NONE);
   eina_mempool_free(_eina_matrixsparse_cell_mp, c);
}

static inline void
_eina_matrixsparse_cell_unlink(Eina_Matrixsparse_Cell *c)
{
   Eina_Matrixsparse_Row *r = c->parent;

   if (r->last_used == c)
     {
        if (c->next)
           r->last_used = c->next;
        else
           r->last_used = c->prev;
     }

   if (r->last_col == c)
      r->last_col = c->prev;

   if (r->cols == c)
      r->cols = c->next;

   if (c->next && c->prev)
     {
        c->next->prev = c->prev;
        c->prev->next = c->next;
     }
   else if (c->next)
      c->next->prev = NULL;
   else if (c->prev)
      c->prev->next = NULL;
}

static inline void
_eina_matrixsparse_row_cells_free(Eina_Matrixsparse_Row *r, void (*free_func)(
                                     void *,
                                     void *), void *user_data)
{
   Eina_Matrixsparse_Cell *c = r->cols;
   while (c)
     {
        Eina_Matrixsparse_Cell *c_aux = c;
        c = c->next;
        _eina_matrixsparse_cell_free(c_aux, free_func, user_data);
     }
}

static inline void
_eina_matrixsparse_row_free(Eina_Matrixsparse_Row *r, void (*free_func)(void *,
                                                                        void *),
                            void *user_data)
{
   _eina_matrixsparse_row_cells_free(r, free_func, user_data);
   EINA_MAGIC_SET(r, EINA_MAGIC_NONE);
   eina_mempool_free(_eina_matrixsparse_row_mp, r);
}

static inline void
_eina_matrixsparse_row_unlink(Eina_Matrixsparse_Row *r)
{
   Eina_Matrixsparse *m = r->parent;

   if (m->last_used == r)
     {
        if (r->next)
           m->last_used = r->next;
        else
           m->last_used = r->prev;
     }

   if (m->last_row == r)
      m->last_row = r->prev;

   if (m->rows == r)
      m->rows = r->next;

   if (r->next && r->prev)
     {
        r->next->prev = r->prev;
        r->prev->next = r->next;
     }
   else if (r->next)
      r->next->prev = NULL;
   else if (r->prev)
      r->prev->next = NULL;
}

static inline void
_eina_matrixsparse_row_find_parms_get(const Eina_Matrixsparse *m,
                                      unsigned long row,
                                      Eina_Matrixsparse_Row **p_r,
                                      int *p_dir)
{
   Eina_Matrixsparse_Row *r;
   unsigned long dist;
   int dir;

   dist = row - m->rows->row;
   r = m->rows;
   dir = 1;
   if (dist > m->last_row->row - row)
     {
        dist = m->last_row->row - row;
        r = m->last_row;
        dir = -1;
     }

   if (m->last_used)
     {
        if (m->last_used->row < row)
          {
             if (dist > row - m->last_used->row)
               {
/*      dist = row = m->last_used->row; */
                  r = m->last_used;
                  dir = 1;
               }
          }
        else if (dist > m->last_used->row - row)
          {
/*      dist = m->last_used->row - row; */
             r = m->last_used;
             dir = -1;
          }
     }

   *p_r = r;
   *p_dir = dir;
}

static inline void
_eina_matrixsparse_row_cell_find_parms_get(const Eina_Matrixsparse_Row *r,
                                           unsigned long col,
                                           Eina_Matrixsparse_Cell **p_c,
                                           int *p_dir)
{
   Eina_Matrixsparse_Cell *c;
   unsigned long dist;
   int dir;

   dist = col - r->cols->col;
   c = r->cols;
   dir = 1;
   if (dist > r->last_col->col - col)
     {
        dist = r->last_col->col - col;
        c = r->last_col;
        dir = -1;
     }

   if (r->last_used)
     {
        if (r->last_used->col < col)
          {
             if (dist > col - r->last_used->col)
               {
/*      dist = col = r->last_used->col; */
                  c = r->last_used;
                  dir = 1;
               }
          }
        else if (dist > r->last_used->col - col)
          {
/*      dist = r->last_used->col - col; */
             c = r->last_used;
             dir = -1;
          }
     }

   *p_c = c;
   *p_dir = dir;
}

static inline Eina_Matrixsparse_Row *
_eina_matrixsparse_row_idx_get(const Eina_Matrixsparse *m, unsigned long row)
{
   Eina_Matrixsparse_Row *r;
   int dir;

   if (!m->rows)
      return NULL;

   if      (m->rows->row == row)
      return m->rows;
   else if (m->rows->row > row)
      return NULL;

   if      (m->last_row->row == row)
      return m->last_row;
   else if (m->last_row->row < row)
      return NULL;

   if ((m->last_used) && (m->last_used->row == row))
      return m->last_used;

   _eina_matrixsparse_row_find_parms_get(m, row, &r, &dir);
   assert(dir != 0);
   if (dir > 0)
     {
        for (; r; r = r->next)
           if (r->row == row)
             {
                ((Eina_Matrixsparse *)m)->last_used = r;
                return r;
             }
           else if (r->row > row)
              return NULL;

     }
   else if (dir < 0)
     {
        for (; r; r = r->prev)
          if (r->row == row)
            {
               ((Eina_Matrixsparse *)m)->last_used = r;
               return r;
            }
          else if (r->row < row)
            return NULL;
     }

   return NULL;
}

static inline Eina_Matrixsparse_Cell *
_eina_matrixsparse_row_cell_idx_get(const Eina_Matrixsparse_Row *r,
                                    unsigned long col)
{
   Eina_Matrixsparse_Cell *c;
   int dir;

   if (!r->cols)
      return NULL;

   if      (r->cols->col == col)
      return r->cols;
   else if (r->cols->col > col)
      return NULL;

   if      (r->last_col->col == col)
      return r->last_col;
   else if (r->last_col->col < col)
      return NULL;

   if ((r->last_used) && (r->last_used->col == col))
      return r->last_used;

   _eina_matrixsparse_row_cell_find_parms_get(r, col, &c, &dir);
   assert(dir != 0);
   if (dir > 0)
     {
        for (; c; c = c->next)
           if (c->col == col)
             {
                ((Eina_Matrixsparse_Row *)r)->last_used = c;
                return c;
             }
           else if (c->col > col)
              return NULL;

     }
   else if (dir < 0)
     {
        for (; c; c = c->prev)
          if (c->col == col)
            {
               ((Eina_Matrixsparse_Row *)r)->last_used = c;
               return c;
            }
          else if (c->col < col)
            return NULL;
     }

   return NULL;
}

static inline Eina_Matrixsparse_Cell *
_eina_matrixsparse_cell_idx_get(const Eina_Matrixsparse *m,
                                unsigned long row,
                                unsigned long col)
{
   Eina_Matrixsparse_Row *r = _eina_matrixsparse_row_idx_get(m, row);
   if (!r)
      return NULL;

   return _eina_matrixsparse_row_cell_idx_get(r, col);
}

static inline void
_eina_matrixsparse_row_idx_siblings_find(const Eina_Matrixsparse *m,
                                         unsigned long row,
                                         Eina_Matrixsparse_Row **p_prev,
                                         Eina_Matrixsparse_Row **p_next)
{
   Eina_Matrixsparse_Row *r;
   int dir;

   _eina_matrixsparse_row_find_parms_get(m, row, &r, &dir);
        assert(dir != 0);
   if (dir > 0)
     {
        for (; r; r = r->next)
           if (r->row > row)
              break;

        assert(r != NULL);
        *p_prev = r->prev;
        *p_next = r;
     }
   else if (dir < 0)
     {
        for (; r; r = r->prev)
           if (r->row < row)
              break;

        assert(r != NULL);
        *p_prev = r;
        *p_next = r->next;
     }
}

static inline void
_eina_matrixsparse_row_cell_idx_siblings_find(const Eina_Matrixsparse_Row *r,
                                              unsigned long col,
                                              Eina_Matrixsparse_Cell **p_prev,
                                              Eina_Matrixsparse_Cell **p_next)
{
   Eina_Matrixsparse_Cell *c;
   int dir;

   _eina_matrixsparse_row_cell_find_parms_get(r, col, &c, &dir);
        assert(dir != 0);
   if (dir > 0)
     {
        for (; c; c = c->next)
           if (c->col > col)
              break;

        assert(c != NULL);
        *p_prev = c->prev;
        *p_next = c;
     }
   else if (dir < 0)
     {
        for (; c; c = c->prev)
           if (c->col < col)
              break;

        assert(c != NULL);
        *p_prev = c;
        *p_next = c->next;
     }
}

static inline Eina_Matrixsparse_Row *
_eina_matrixsparse_row_idx_add(Eina_Matrixsparse *m, unsigned long row)
{
   Eina_Matrixsparse_Row *r = eina_mempool_malloc
         (_eina_matrixsparse_row_mp, sizeof(Eina_Matrixsparse_Row));
   if (!r)
      return NULL;

   if (!m->rows)
     {
        r->prev = NULL;
        r->next = NULL;
        m->rows = r;
        m->last_row = r;
     }
   else if (row < m->rows->row)
     {
        r->prev = NULL;
        r->next = m->rows;
        m->rows->prev = r;
        m->rows = r;
     }
   else if (row > m->last_row->row)
     {
        r->prev = m->last_row;
        m->last_row->next = r;
        r->next = NULL;
        m->last_row = r;
     }
   else
     {
        Eina_Matrixsparse_Row *prev = NULL, *next = NULL;
        _eina_matrixsparse_row_idx_siblings_find(m, row, &prev, &next);
        assert(prev != NULL);
        assert(next != NULL);
        r->prev = prev;
        r->next = next;
        prev->next = r;
        next->prev = r;
     }

   r->cols = NULL;
   r->last_col = NULL;
   r->last_used = NULL;
   r->row = row;
   r->parent = m;
   EINA_MAGIC_SET(r, EINA_MAGIC_MATRIXSPARSE_ROW);
   m->last_used = r;
   return r;
}

static inline Eina_Matrixsparse_Cell *
_eina_matrixsparse_row_cell_idx_add(Eina_Matrixsparse_Row *r,
                                    unsigned long col,
                                    const void *data)
{
   Eina_Matrixsparse_Cell *c = eina_mempool_malloc
         (_eina_matrixsparse_cell_mp, sizeof(Eina_Matrixsparse_Cell));
   if (!c)
      return NULL;

   if (!r->cols)
     {
        c->prev = NULL;
        c->next = NULL;
        r->cols = c;
        r->last_col = c;
     }
   else if (col < r->cols->col)
     {
        c->prev = NULL;
        c->next = r->cols;
        r->cols->prev = c;
        r->cols = c;
     }
   else if (col > r->last_col->col)
     {
        c->prev = r->last_col;
        r->last_col->next = c;
        c->next = NULL;
        r->last_col = c;
     }
   else
     {
        Eina_Matrixsparse_Cell *prev = NULL, *next = NULL;
        _eina_matrixsparse_row_cell_idx_siblings_find(r, col, &prev, &next);
        assert(prev != NULL);
        assert(next != NULL);
        c->prev = prev;
        c->next = next;
        prev->next = c;
        next->prev = c;
     }

   c->data = (void *)data;
   c->col = col;
   c->parent = r;
   EINA_MAGIC_SET(c, EINA_MAGIC_MATRIXSPARSE_CELL);
   r->last_used = c;
   return c;
}

static inline Eina_Bool
_eina_matrixsparse_cell_idx_add(Eina_Matrixsparse *m,
                                unsigned long row,
                                unsigned long col,
                                const void *data)
{
   Eina_Matrixsparse_Row *r = _eina_matrixsparse_row_idx_get(m, row);
   if (!r)
      r = _eina_matrixsparse_row_idx_add(m, row);

   if (!r)
      return 0;

   if (_eina_matrixsparse_row_cell_idx_add(r, col, data))
      return 1;

   if (r->cols)
      return 0;

   _eina_matrixsparse_row_unlink(r);
   _eina_matrixsparse_row_free(r, m->free.func, m->free.user_data);
   return 0;
}

/*============================================================================*
*                Iterators                                    *
*============================================================================*/
static Eina_Bool
_eina_matrixsparse_iterator_next(Eina_Matrixsparse_Iterator *it, void **data)
{
   EINA_MAGIC_CHECK_MATRIXSPARSE_ITERATOR(it, EINA_FALSE);

   /* do not touch it->idx */

   if (!it->ref.col)
      return 0;

   *data = (Eina_Matrixsparse_Cell *)it->ref.col;

   it->ref.col = it->ref.col->next;
   if (!it->ref.col)
     {
        it->ref.row = it->ref.row->next;
        if (it->ref.row)
           it->ref.col = it->ref.row->cols;
     }

   return 1;
}

static Eina_Matrixsparse *
_eina_matrixsparse_iterator_get_container(Eina_Matrixsparse_Iterator *it)
{
   EINA_MAGIC_CHECK_MATRIXSPARSE_ITERATOR(it, NULL);
   return (Eina_Matrixsparse *)it->m;
}

static void
_eina_matrixsparse_iterator_free(Eina_Matrixsparse_Iterator *it)
{
   EINA_MAGIC_CHECK_MATRIXSPARSE_ITERATOR(it);
   EINA_MAGIC_SET(it,            EINA_MAGIC_NONE);
   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_NONE);
   free(it);
}

static Eina_Bool
_eina_matrixsparse_iterator_complete_next(
   Eina_Matrixsparse_Iterator_Complete *it,
   void **data)
{
   EINA_MAGIC_CHECK_MATRIXSPARSE_ITERATOR(it, EINA_FALSE);

   if (it->idx.row >= it->m->size.rows)
      return 0;

   if (it->dummy.col.data)
      ERR("Last iterator call changed dummy cell!");

   if ((it->ref.col) &&
       (it->ref.col->col == it->idx.col) &&
       (it->ref.row->row == it->idx.row))
     {
        *data = (Eina_Matrixsparse_Cell *)it->ref.col;
        it->ref.col = it->ref.col->next;
        if (!it->ref.col)
          {
             it->ref.row = it->ref.row->next;
             if (it->ref.row)
                it->ref.col = it->ref.row->cols;
          }
     }
   else
     {
        it->dummy.col.data = NULL;
        it->dummy.col.col = it->idx.col;
        it->dummy.row.row = it->idx.row;
        *data = &it->dummy.col;
     }

   it->idx.col++;
   if (it->idx.col == it->m->size.cols)
     {
        it->idx.col = 0;
        it->idx.row++;
     }

   return 1;
}

static Eina_Matrixsparse *
_eina_matrixsparse_iterator_complete_get_container(
   Eina_Matrixsparse_Iterator_Complete *it)
{
      EINA_MAGIC_CHECK_MATRIXSPARSE_ITERATOR(it, NULL);
   return (Eina_Matrixsparse *)it->m;
}

static void
_eina_matrixsparse_iterator_complete_free(
   Eina_Matrixsparse_Iterator_Complete *it)
{
      EINA_MAGIC_CHECK_MATRIXSPARSE_ITERATOR(it);

   if (it->dummy.col.data)
      ERR("Last iterator call changed dummy cell!");

   EINA_MAGIC_SET(it,            EINA_MAGIC_NONE);
   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_NONE);
   free(it);
}


/**
 * @endcond
 */

/*============================================================================*
*                                 Global                                     *
*============================================================================*/

/**
 * @internal
 * @brief Initialize the matrixsparse module.
 *
 * @return #EINA_TRUE on success, #EINA_FALSE on failure.
 *
 * This function sets up the matrixsparse module of Eina. It is called by
 * eina_init().
 *
 * This function creates mempool to speed up matrix rows and cells
 * management, using EINA_MEMPOOL environment variable if it is set to
 * choose the memory pool type to use.
 *
 * @see eina_init()
 */
Eina_Bool
eina_matrixsparse_init(void)
{
   const char *choice, *tmp;

   _eina_matrixsparse_log_dom = eina_log_domain_register("eina_matrixsparse",
                                                         EINA_LOG_COLOR_DEFAULT);
   if (_eina_matrixsparse_log_dom < 0)
     {
        EINA_LOG_ERR("Could not register log domain: eina_matrixsparse");
        return EINA_FALSE;
     }

#ifdef EINA_DEFAULT_MEMPOOL
   choice = "pass_through";
#else
   choice = "chained_mempool";
#endif
   tmp = getenv("EINA_MEMPOOL");
   if (tmp && tmp[0])
      choice = tmp;

   _eina_matrixsparse_cell_mp = eina_mempool_add
         (choice,
         "matrixsparse_cell",
         NULL,
         sizeof (Eina_Matrixsparse_Cell),
         32);
   if (!_eina_matrixsparse_cell_mp)
     {
        ERR(
           "Mempool for matrixsparse_cell cannot be allocated in matrixsparse init.");
        goto on_init_fail;
     }

   _eina_matrixsparse_row_mp = eina_mempool_add
         (choice, "matrixsparse_row", NULL, sizeof (Eina_Matrixsparse_Row), 32);
   if (!_eina_matrixsparse_row_mp)
     {
        ERR(
           "Mempool for matrixsparse_row cannot be allocated in matrixsparse init.");
        goto on_init_fail;
     }

#define EMS(n) eina_magic_string_static_set(n, n ## _STR)
   EMS(EINA_MAGIC_MATRIXSPARSE);
   EMS(EINA_MAGIC_MATRIXSPARSE_ROW);
   EMS(EINA_MAGIC_MATRIXSPARSE_CELL);
   EMS(EINA_MAGIC_MATRIXSPARSE_ITERATOR);
   EMS(EINA_MAGIC_MATRIXSPARSE_ROW_ACCESSOR);
   EMS(EINA_MAGIC_MATRIXSPARSE_ROW_ITERATOR);
   EMS(EINA_MAGIC_MATRIXSPARSE_CELL_ACCESSOR);
   EMS(EINA_MAGIC_MATRIXSPARSE_CELL_ITERATOR);
#undef EMS

   return EINA_TRUE;

on_init_fail:
   eina_log_domain_unregister(_eina_matrixsparse_log_dom);
   _eina_matrixsparse_log_dom = -1;
   return EINA_FALSE;
}

/**
 * @internal
 * @brief Shut down the matrixsparse module.
 *
 * @return #EINA_TRUE on success, #EINA_FALSE on failure.
 *
 * This function shuts down the matrixsparse module set up by
 * eina_matrixsparse_init(). It is called by eina_shutdown().
 *
 * @see eina_shutdown()
 */
Eina_Bool
eina_matrixsparse_shutdown(void)
{
   eina_mempool_del(_eina_matrixsparse_row_mp);
   eina_mempool_del(_eina_matrixsparse_cell_mp);

   eina_log_domain_unregister(_eina_matrixsparse_log_dom);
   _eina_matrixsparse_log_dom = -1;
   return EINA_TRUE;
}

/*============================================================================*
*                                   API                                      *
*============================================================================*/

EINA_API Eina_Matrixsparse *
eina_matrixsparse_new(unsigned long rows, unsigned long cols, void (*free_func)(
                         void *user_data,
                         void *cell_data), const void *user_data)
{
   Eina_Matrixsparse *m;

   EINA_SAFETY_ON_FALSE_RETURN_VAL(rows > 0, NULL);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(cols > 0, NULL);

   m = malloc(sizeof(Eina_Matrixsparse));
   if (!m) return NULL;

   EINA_MAGIC_SET(m, EINA_MAGIC_MATRIXSPARSE);

   m->rows = NULL;
   m->last_row = NULL;
   m->last_used = NULL;

   m->size.rows = rows;
   m->size.cols = cols;
   m->free.func = free_func;
   m->free.user_data = (void *)user_data;

   return m;
}

EINA_API void
eina_matrixsparse_free(Eina_Matrixsparse *m)
{
   void (*free_func)(void *, void *);
   void *user_data;

   Eina_Matrixsparse_Row *r;

   if (!m)
     return;

   EINA_MAGIC_CHECK_MATRIXSPARSE(m);

   free_func = m->free.func;
   user_data = m->free.user_data;

   r = m->rows;
   while (r)
     {
        Eina_Matrixsparse_Row *r_aux = r;
        r = r->next;
        _eina_matrixsparse_row_free(r_aux, free_func, user_data);
     }

   EINA_MAGIC_SET(m, EINA_MAGIC_NONE);
   free(m);
}

EINA_API void
eina_matrixsparse_size_get(const Eina_Matrixsparse *m,
                           unsigned long *rows,
                           unsigned long *cols)
{
   if (rows)
      *rows = 0;

   if (cols)
      *cols = 0;

   EINA_MAGIC_CHECK_MATRIXSPARSE(m);
   if (rows)
      *rows = m->size.rows;

   if (cols)
      *cols = m->size.cols;
}

EINA_API Eina_Bool
eina_matrixsparse_size_set(Eina_Matrixsparse *m,
                           unsigned long rows,
                           unsigned long cols)
{
   Eina_Bool update_last_used_row;
   Eina_Matrixsparse_Row *r;
   void (*free_func)(void *, void *);
   void *user_data;

   EINA_MAGIC_CHECK_MATRIXSPARSE(m, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(rows > 0, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(cols > 0, 0);

   if ((rows == m->size.rows) && (cols == m->size.cols))
      return 1;

   update_last_used_row = ((m->last_used) && (m->last_used->row >= rows));
   free_func = m->free.func;
   user_data = m->free.user_data;

   r = m->last_row;
   while (r && r->row >= rows)
     {
        Eina_Matrixsparse_Row *r_aux = r;
        r = r->prev;
        _eina_matrixsparse_row_free(r_aux, free_func, user_data);
     }
   if (!r)
     {
        m->last_row = NULL;
        m->rows = NULL;
     }
   else if (r != m->last_row)
     {
        r->next = NULL;
        m->last_row = r;
     }

   if (update_last_used_row)
      m->last_used = m->last_row;

   r = m->rows;
   while (r)
     {
        Eina_Matrixsparse_Cell *c = r->last_col;
        Eina_Bool update_last_used_col;
        update_last_used_col = ((r->last_used) && (r->last_used->col >= cols));
        while (c && c->col >= cols)
          {
             Eina_Matrixsparse_Cell *c_aux = c;
             c = c->prev;
             _eina_matrixsparse_cell_free(c_aux, free_func, user_data);
          }
        if (!c)
          {
             Eina_Matrixsparse_Row *r_aux = r;
             r->cols = NULL;
             r->last_col = NULL;
             if (r->next)
                r->next->prev = r->prev;
             else
                m->last_row = r->prev;

             if (r->prev)
                r->prev->next = r->next;
             else
                m->rows = r->next;

             r = r->next;
             _eina_matrixsparse_row_free(r_aux, free_func, user_data);
             if ((update_last_used_row) && (m->last_used == r_aux))
               m->last_used = r;
          }
        else
          {
             if (c != r->last_col)
               {
                  c->next = NULL;
                  r->last_col = c;
               }

             if (update_last_used_col)
                r->last_used = r->last_col;

             r = r->next;
          }
     }

   update_last_used_row = 0;
   if (m->last_used)
     {
        if (m->last_row)
           update_last_used_row = m->last_used->row > m->last_row->row;
        else
           update_last_used_row = 1;
     }

   if (update_last_used_row)
      m->last_used = m->last_row;

   m->size.rows = rows;
   m->size.cols = cols;
   return 1;
}

EINA_API Eina_Bool
eina_matrixsparse_cell_idx_get(const Eina_Matrixsparse *m,
                               unsigned long row,
                               unsigned long col,
                               Eina_Matrixsparse_Cell **cell)
{
   EINA_MAGIC_CHECK_MATRIXSPARSE(m, 0);
   EINA_SAFETY_ON_NULL_RETURN_VAL(cell, 0);
   *cell = NULL;
   EINA_SAFETY_ON_FALSE_RETURN_VAL(row < m->size.rows, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(col < m->size.cols, 0);
   *cell = _eina_matrixsparse_cell_idx_get(m, row, col);
   return 1;
}

EINA_API void *
eina_matrixsparse_cell_data_get(const Eina_Matrixsparse_Cell *cell)
{
   EINA_MAGIC_CHECK_MATRIXSPARSE_CELL(cell, NULL);
   return cell->data;
}

EINA_API void *
eina_matrixsparse_data_idx_get(const Eina_Matrixsparse *m,
                               unsigned long row,
                               unsigned long col)
{
   Eina_Matrixsparse_Cell *c;
   EINA_MAGIC_CHECK_MATRIXSPARSE(m, NULL);
   c = _eina_matrixsparse_cell_idx_get(m, row, col);
   if (c)
      return c->data;
   else
      return NULL;
}

EINA_API Eina_Bool
eina_matrixsparse_cell_position_get(const Eina_Matrixsparse_Cell *cell,
                                    unsigned long *row,
                                    unsigned long *col)
{
   if (row)
      *row = 0;

   if (col)
      *col = 0;

   EINA_MAGIC_CHECK_MATRIXSPARSE_CELL(cell, 0);
   EINA_MAGIC_CHECK_MATRIXSPARSE_ROW(cell->parent, 0);
   if (row)
      *row = cell->parent->row;

   if (col)
      *col = cell->col;

   return 1;
}

EINA_API Eina_Bool
eina_matrixsparse_cell_data_replace(Eina_Matrixsparse_Cell *cell,
                                    const void *data,
                                    void **p_old)
{
   if (p_old)
      *p_old = NULL;

   EINA_MAGIC_CHECK_MATRIXSPARSE_CELL(cell, 0);

   if (p_old)
      *p_old = cell->data;

   cell->data = (void *)data;
   return 1;
}

EINA_API Eina_Bool
eina_matrixsparse_cell_data_set(Eina_Matrixsparse_Cell *cell, const void *data)
{
   Eina_Matrixsparse *m;

   EINA_MAGIC_CHECK_MATRIXSPARSE_CELL(cell, 0);
   EINA_MAGIC_CHECK_MATRIXSPARSE_ROW(cell->parent, 0);
   EINA_MAGIC_CHECK_MATRIXSPARSE(cell->parent->parent, 0);

   m = cell->parent->parent;

   if (m->free.func)
      m->free.func(m->free.user_data, cell->data);

   cell->data = (void *)data;
   return 1;
}

EINA_API Eina_Bool
eina_matrixsparse_data_idx_replace(Eina_Matrixsparse *m,
                                   unsigned long row,
                                   unsigned long col,
                                   const void *data,
                                   void **p_old)
{
   Eina_Matrixsparse_Cell *cell;

   if (p_old)
      *p_old = NULL;

   EINA_MAGIC_CHECK_MATRIXSPARSE(m, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(row < m->size.rows, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(col < m->size.cols, 0);

   cell = _eina_matrixsparse_cell_idx_get(m, row, col);
   if (cell)
     {
        if (p_old)
           *p_old = cell->data;

        cell->data = (void *)data;
        return 1;
     }

   return _eina_matrixsparse_cell_idx_add(m, row, col, data);
}

EINA_API Eina_Bool
eina_matrixsparse_data_idx_set(Eina_Matrixsparse *m,
                               unsigned long row,
                               unsigned long col,
                               const void *data)
{
   Eina_Matrixsparse_Cell *cell;

   EINA_MAGIC_CHECK_MATRIXSPARSE(m, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(row < m->size.rows, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(col < m->size.cols, 0);

   cell = _eina_matrixsparse_cell_idx_get(m, row, col);
   if (cell)
     {
        if (m->free.func)
           m->free.func(m->free.user_data, cell->data);

        cell->data = (void *)data;
        return 1;
     }

   return _eina_matrixsparse_cell_idx_add(m, row, col, data);
}

EINA_API Eina_Bool
eina_matrixsparse_row_idx_clear(Eina_Matrixsparse *m, unsigned long row)
{
   Eina_Matrixsparse_Row *r;

   EINA_MAGIC_CHECK_MATRIXSPARSE(m, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(row < m->size.rows, 0);

   r = _eina_matrixsparse_row_idx_get(m, row);
   if (!r)
      return 1;

   _eina_matrixsparse_row_unlink(r);
   _eina_matrixsparse_row_free(r, m->free.func, m->free.user_data);

   return 1;
}

EINA_API Eina_Bool
eina_matrixsparse_column_idx_clear(Eina_Matrixsparse *m, unsigned long col)
{
   Eina_Matrixsparse_Row *r;
   void (*free_func)(void *, void *);
   void *user_data;

   EINA_MAGIC_CHECK_MATRIXSPARSE(m, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(col < m->size.cols, 0);

   free_func = m->free.func;
   user_data = m->free.user_data;

   for (r = m->rows; r; )
     {
        Eina_Matrixsparse_Row *r_aux = r;
        Eina_Matrixsparse_Cell *c;

        c = _eina_matrixsparse_row_cell_idx_get(r, col);
        r = r->next;

        if (!c)
           continue;

        if ((r_aux->cols != c) || (r_aux->last_col != c))
          {
             _eina_matrixsparse_cell_unlink(c);
             _eina_matrixsparse_cell_free(c, free_func, user_data);
          }
        else
          {
             _eina_matrixsparse_row_unlink(r_aux);
             _eina_matrixsparse_row_free(r_aux, free_func, user_data);
          }
     }

   return 1;
}

EINA_API Eina_Bool
eina_matrixsparse_cell_idx_clear(Eina_Matrixsparse *m,
                                 unsigned long row,
                                 unsigned long col)
{
   Eina_Matrixsparse_Cell *c;

   EINA_MAGIC_CHECK_MATRIXSPARSE(m, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(row < m->size.rows, 0);
   EINA_SAFETY_ON_FALSE_RETURN_VAL(col < m->size.cols, 0);

   c = _eina_matrixsparse_cell_idx_get(m, row, col);
   if (!c)
      return 1;

   _eina_matrixsparse_cell_unlink(c);
   _eina_matrixsparse_cell_free(c, m->free.func, m->free.user_data);

   return 1;
}

EINA_API Eina_Bool
eina_matrixsparse_cell_clear(Eina_Matrixsparse_Cell *cell)
{
   Eina_Matrixsparse *m;

   EINA_MAGIC_CHECK_MATRIXSPARSE_CELL(cell, 0);
   EINA_MAGIC_CHECK_MATRIXSPARSE_ROW(cell->parent, 0);
   EINA_MAGIC_CHECK_MATRIXSPARSE(cell->parent->parent, 0);

   m = cell->parent->parent;

   _eina_matrixsparse_cell_unlink(cell);
   _eina_matrixsparse_cell_free(cell, m->free.func, m->free.user_data);
   return 1;
}

EINA_API Eina_Iterator *
eina_matrixsparse_iterator_new(const Eina_Matrixsparse *m)
{
   Eina_Matrixsparse_Iterator *it;

   it = calloc(1, sizeof(*it));
   if (!it) return NULL;

   EINA_MAGIC_SET(it,            EINA_MAGIC_MATRIXSPARSE_ITERATOR);
   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

   it->m = m;
   it->ref.row = m->rows;
   it->ref.col = m->rows ? m->rows->cols : NULL;

   it->iterator.version = EINA_ITERATOR_VERSION;
   it->iterator.next = FUNC_ITERATOR_NEXT(_eina_matrixsparse_iterator_next);
   it->iterator.get_container = FUNC_ITERATOR_GET_CONTAINER(
         _eina_matrixsparse_iterator_get_container);
   it->iterator.free = FUNC_ITERATOR_FREE(_eina_matrixsparse_iterator_free);
   return &it->iterator;
}

EINA_API Eina_Iterator *
eina_matrixsparse_iterator_complete_new(const Eina_Matrixsparse *m)
{
   Eina_Matrixsparse_Iterator_Complete *it;

   it = calloc(1, sizeof(*it));
   if (!it) return NULL;

   EINA_MAGIC_SET(it,            EINA_MAGIC_MATRIXSPARSE_ITERATOR);
   EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

   it->m = m;
   it->idx.row = 0;
   it->idx.col = 0;
   it->ref.row = m->rows;
   it->ref.col = m->rows ? m->rows->cols : NULL;

   it->dummy.row.next = it->dummy.row.prev = NULL;
   it->dummy.row.cols = it->dummy.row.last_col = it->dummy.row.last_used = NULL;
   it->dummy.row.parent = (Eina_Matrixsparse *)m;
   EINA_MAGIC_SET(&it->dummy.row, EINA_MAGIC_MATRIXSPARSE_ROW);

   it->dummy.col.next = it->dummy.col.prev = NULL;
   it->dummy.col.data = NULL;
   it->dummy.col.parent = &it->dummy.row;
   EINA_MAGIC_SET(&it->dummy.col, EINA_MAGIC_MATRIXSPARSE_CELL);

   it->iterator.version = EINA_ITERATOR_VERSION;
   it->iterator.next = FUNC_ITERATOR_NEXT(
         _eina_matrixsparse_iterator_complete_next);
   it->iterator.get_container = FUNC_ITERATOR_GET_CONTAINER(
         _eina_matrixsparse_iterator_complete_get_container);
   it->iterator.free = FUNC_ITERATOR_FREE(
         _eina_matrixsparse_iterator_complete_free);
   return &it->iterator;
}
