/* EINA - EFL data type library
 * Copyright (C) 2007-2008 Jorge Luis Zapata Muga
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

#ifndef EINA_CPU_H_
#define EINA_CPU_H_

/**
 * @addtogroup Eina_Cpu_Group Cpu
 *
 * @brief Cpu and architecture related helpers
 */

/**
 * @addtogroup Eina_Tools_Group Tools
 *
 * @{
 */

/**
 * @defgroup Eina_Cpu_Group Cpu
 *
 * @{
 */

#include "eina_types.h"

/**
 * @typedef Eina_Cpu_Features
 * Enumerates different hardware architectures.
 */
typedef enum _Eina_Cpu_Features
{
   EINA_CPU_MMX     = 0x00000001, /**< Multimedia Extensions (Intel) */
   EINA_CPU_SSE     = 0x00000002, /**< Streaming SIMD Extensions (Intel) */
   EINA_CPU_SSE2    = 0x00000004, /**< Streaming SIMD Extensions 2 (Intel) */
   EINA_CPU_SSE3    = 0x00000008, /**< Streaming SIMD Extensions 3 (Intel) */
   /* TODO 3DNow! */
   EINA_CPU_ALTIVEC = 0x00000010, /**< Single-precision floating point and integer SIMD (PowerPC) */
   EINA_CPU_VIS     = 0x00000020, /**< Visual Instruction Set (Spark) */
   EINA_CPU_NEON    = 0x00000040, /**< Advanced SIMD Extension (ARM) */
   EINA_CPU_SSSE3   = 0x00000080, /**< Supplemental Streaming SIMD Extension 3 (Intel) */
   EINA_CPU_SSE41   = 0x00000100, /**< Streaming SIMD Extension 4.1 (Intel) */
   EINA_CPU_SSE42   = 0x00000200, /**< Streaming SIMD Extension 4.2 (Intel) */
   EINA_CPU_SVE     = 0x00000400  /**< Scalable Vector Extension (ARM) */
} Eina_Cpu_Features;

/**
 * @brief Global hardware architecture handler.
 *
 * @return the current cpu features
 */
EAPI EXTERN Eina_Cpu_Features eina_cpu_features;

/**
 * @brief Cpu features accessor.
 *
 * @return the current cpu features
 */
EAPI Eina_Cpu_Features eina_cpu_features_get(void);

/**
 * @brief Gets the current number of processors.
 *
 * @return The number of processors that are online, that
 * is available when the function is called.
 */
EAPI int               eina_cpu_count(void);

/**
 * @brief Gets the current virtual page size.
 *
 * @return The fixed length that represents the smallest unit of data for memory
 * allocation performed by the operating system on behalf of the program, and
 * for transfers between the main memory and any other auxiliary store.
 */
EAPI int               eina_cpu_page_size(void);

/**
 * @brief Reverses the byte order of a 16-bit (destination) register.
 *
 * @param[in] x The binary word to swap
 * @return A byte order swapped 16-bit integer.  
 *
 * On big endian systems, the number is converted to little endian byte order.
 * On little endian systems, the number is converted to big endian byte order.
 */
static inline unsigned short eina_swap16(unsigned short x);

/**
 * @brief Reverses the byte order of a 32-bit (destination) register.
 *
 * @param[in] x The binary word to swap
 * @return A byte order swapped 32-bit integer.  
 *
 * On big endian systems, the number is converted to little endian byte order.
 * On little endian systems, the number is converted to big endian byte order.
 */
static inline unsigned int eina_swap32(unsigned int x);

/**
 * @brief Reverses the byte order of a 64-bit (destination) register.
 *
 * @param[in] x The binary word to swap
 * @return A byte order swapped 64-bit integer.
 *
 * On big endian systems, the number is converted to little endian byte order.
 * On little endian systems, the number is converted to big endian byte order.
 */
static inline unsigned long long eina_swap64(unsigned long long x);

static inline unsigned short eina_htons(unsigned short host);

static inline unsigned int eina_htonl(unsigned int host);

static inline unsigned long long eina_htonll(unsigned long long host);

static inline unsigned short eina_ntohs(unsigned short net);

static inline unsigned int eina_ntohl(unsigned int net);

static inline unsigned long long eina_ntohll(unsigned long long net);

#ifndef MIN
/** Returns the minimum of the two parameters
 * @warning Beware of side effects, since the returned parameter is evaluated
 * twice.
 */
# define MIN(x, y)          (((x) > (y)) ? (y) : (x))
#endif

#ifndef MAX
/** Returns the maximum of the two parameters
 * @warning Beware of side effects, since the returned parameter is evaluated
 * twice.
 */
# define MAX(x, y)          (((x) > (y)) ? (x) : (y))
#endif

#include "eina_inline_cpu.x"

/**
 * @}
 */

/**
 * @}
 */

#endif /* EINA_CPU_H_ */
