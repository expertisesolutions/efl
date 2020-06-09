/* EINA - EFL data type library
 * Copyright (C) 2013 Cedric Bail
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

#ifndef EINA_COW_H_
#define EINA_COW_H_


/**
 * @addtogroup Eina_Tools_Group Tools
 *
 * @{
 */

/**
 * @defgroup Eina_Cow_Group Copy On Write
 *
 * @brief These functions provide some helper for a pseudo Copy-On-Write mechanism.
 *
 * Eina_Cow will return a const memory pointer to some default value that you will
 * be able to change only by requesting a writable pointer. Later on a garbage collector
 * can come online and try to merge back some of those pointers.
 *
 * @since 1.8.0
 *
 * @{
 */

/**
 * @typedef Eina_Cow
 * Type for Eina_Cow pool
 */
typedef struct _Eina_Cow Eina_Cow;
/**
 * @typedef Eina_Cow_Data
 * Type of the returned pointer to simplify some reading.
 */
typedef void Eina_Cow_Data;

/**
 * @brief Instantiates a new Eina_Cow pool.
 *
 * @param[in] name The name of this pool, used for debugging.
 * @param[in] struct_size The size of the object from this pool.
 * @param[in] step How many objects to allocate when the pool gets empty.
 * @param[in] default_value The default value returned by this pool.
 * @param[in] gc Is it possible to run garbage collection on this pool.
 * @return A valid new Eina_Cow, or @c NULL on error.
 */
EAPI Eina_Cow *eina_cow_add(const char *name, unsigned int struct_size, unsigned int step, const void *default_value, Eina_Bool gc) EINA_WARN_UNUSED_RESULT;

/**
 * @brief Destroys an Eina_Cow pool and all the allocated memory.
 *
 * @param[in] cow The pool to destroy
 */
EAPI void eina_cow_del(Eina_Cow *cow);

/**
 * @brief Returns an initialized pointer from the pool.
 *
 * @param[in] cow The pool to take things from.
 * @return A pointer to the new pool instance
 */
EAPI const Eina_Cow_Data *eina_cow_alloc(Eina_Cow *cow) EINA_WARN_UNUSED_RESULT;

/**
 * @brief Frees a pointer from the pool.
 *
 * @param[in,out] cow The pool to gave back memory to.
 * @param[in] data The data to give back.
 *
 * @note To simplify the caller code *data will point to the default
 * read only state after the call to this function.
 */
EAPI void eina_cow_free(Eina_Cow *cow, const Eina_Cow_Data **data);

/**
 * @brief Gets a writeable pointer from a const pointer.
 *
 * @param[in,out] cow The pool the pointer came from.
 * @param[in] src The pointer you want to write to.
 *
 * @note This function is not thread safe.
 */
EAPI void *eina_cow_write(Eina_Cow *cow,
			  const Eina_Cow_Data * const *src) EINA_WARN_UNUSED_RESULT;

/**
 * @brief Sets back a pointer into read only.
 *
 * @param[in,out] cow The pool the pointer came from.
 * @param[in] dst The read only version of the pointer.
 * @param[in] data The pointer to which data was written to.
 * @param[in] needed_gc Does this pool need to be garbage collected?
 *
 * @note This function is not thread safe.
 */
EAPI void eina_cow_done(Eina_Cow *cow,
			const Eina_Cow_Data * const *dst,
			const void *data,
			Eina_Bool needed_gc);
/**
 * @brief Makes the destination contain the same thing as the source pointer.
 *
 * @param[in,out] cow The pool the pointers came from.
 * @param[in] dst The destination to update.
 * @param[in] src The source of information to copy.
 */
EAPI void eina_cow_memcpy(Eina_Cow *cow,
			  const Eina_Cow_Data * const *dst,
			  const Eina_Cow_Data *src);

/**
 * @brief Tries to find entries that have the same content and update them.
 *
 * @param[in,out] cow The cow to try to compact.
 * @return EINA_TRUE if something was compacted, EINA_FALSE if nothing was.
 *
 * There is no guaranty in the time it will require, but should remain low.
 * It does run a hash function on all possible common structures trying to
 * find the one that matches and merge them into one pointer.
 */
EAPI Eina_Bool eina_cow_gc(Eina_Cow *cow);

/**
 * @def EINA_COW_WRITE_BEGIN
 * @brief Definition for the macro to setup a writeable pointer from a const one.
 *
 * @param[in,out] Cow The Eina_Cow where the const pointer came from.
 * @param[in] Read The const pointer to get a writable handler from.
 * @param[in] Write_Type The type of the pointer you want to write to.
 * @param[in] Write The name of the variable where to put the writeable pointer to.
 *
 * @since 1.8.0
 *
 * @note This macro opens a C scope that is expected to be closed by
 * EINA_COW_WRITE_END().
 */
#define EINA_COW_WRITE_BEGIN(Cow, Read, Write_Type, Write)		\
  do									\
    {									\
      Write_Type *Write;						\
      									\
      Write = eina_cow_write(Cow, ((const Eina_Cow_Data**)&(Read)));

/**
 * @def EINA_COW_WRITE_END
 * @brief Definition for the macro to close the writeable pointer.
 *
 * @param[in,out] Cow The Eina_Cow where the const pointer came from.
 * @param[in] Read The const pointer to get a writable handler from.
 * @param[in] Write The name of the variable where to put the writeable pointer to.
 *
 * @since 1.8.0
 *
 * @note This macro closes the scope opened by EINA_COW_WRITE_BEGIN().
 */
#define EINA_COW_WRITE_END(Cow, Read, Write)				\
      eina_cow_done(Cow, ((const Eina_Cow_Data**)&(Read)), Write,	\
		    EINA_TRUE);						\
    }									\
  while (0);

/**
 * @def EINA_COW_WRITE_END_NOGC
 * @brief Definition for the macro to close the writeable pointer without triggering the GC.
 *
 * @param[in,out] Cow The Eina_Cow where the const pointer came from.
 * @param[in] Read The const pointer to get a writable handler from.
 * @param[in] Write The name of the variable where to put the writeable pointer to.
 *
 * @since 1.8.0
 *
 * @note This macro closes the scope opened by EINA_COW_WRITE_BEGIN().
 */
#define EINA_COW_WRITE_END_NOGC(Cow, Read, Write)                       \
      eina_cow_done(Cow, ((const Eina_Cow_Data**)&(Read)), Write,	\
		    EINA_FALSE);                                        \
    }									\
  while (0);

/**
 * @}
 */

/**
 * @}
 */

#endif
