/* EINA - EFL data type library
 * Copyright (C) 2008 Cedric BAIL, Vincent Torri
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

#ifndef EINA_CONVERT_H_
#define EINA_CONVERT_H_

#include "eina_types.h"
#include "eina_error.h"

#include "eina_fp.h"


/**
 * @addtogroup Eina_Convert_Group Convert
 *
 * These functions allow you to convert integer or real numbers to
 * string or conversely.
 *
 * To use these functions, you have to call eina_init()
 * first, and eina_shutdown() when eina is not used anymore.
 *
 * @section Eina_Convert_From_Integer_To_String Conversion from integer to string
 *
 * To convert an integer to a string in the decimal base,
 * eina_convert_itoa() should be used. If the hexadecimal base is
 * wanted, eina_convert_xtoa() should be used. They all need a buffer
 * sufficiently large to store all the cyphers.
 *
 * Here is an example of use:
 *
 * @code
 * #include <stdlib.h>
 * #include <eina_stdio.h>
 *
 * #include <Eina.h>
 *
 * int main(void)
 * {
 *    char tmp[128];
 *
 *    if (!eina_init())
 *    {
 *        printf ("Error during the initialization of eina.\n");
 *        return EXIT_FAILURE;
 *    }
 *
 *    eina_convert_itoa(45, tmp);
 *    printf("value: %s\n", tmp);
 *    eina_convert_xtoa(0xA1, tmp);
 *    printf("value: %s\n", tmp);
 *
 *    eina_shutdown();
 *
 *    return EXIT_SUCCESS;
 * }
 * @endcode
 *
 * Compile this code with the following command:
 *
 * @code
 * gcc -Wall -o test_eina_convert test_eina.c `pkg-config --cflags --libs eina`
 * @endcode
 *
 * @note
 * The alphabetical cyphers are in lower case.
 *
 * @section Eina_Convert_Double Conversion double / string
 *
 * To convert a double to a string, eina_convert_dtoa() should be
 * used. Like with the integer functions, a buffer must be used. The
 * resulting string has the following format (which is the result
 * obtained with snprintf() and the @%a modifier):
 *
 * @code
 * [-]0xh.hhhhhp[+-]e
 * @endcode
 *
 * To convert a string to a double, eina_convert_atod() should be
 * used. The format of the string must be as above. Then, the double
 * has the following mantissas and exponent:
 *
 * @code
 * mantissa  : [-]hhhhhh
 * exponent : 2^([+-]e - 4 * n)
 * @endcode
 *
 * with n being number of cyphers after the point in the string
 * format. To obtain the double number from the mantissas and exponent,
 * use ldexp().
 *
 * Here is an example of use:
 *
 * @code
 * #include <stdlib.h>
 * #include <eina_stdio.h>
 * #include <math.h>
 *
 * #include <Eina.h>
 *
 * int main(void)
 * {
 *    char      tmp[128];
 *    long long int m = 0;
 *    long int  e = 0;
 *    double    r;
 *
 *    if (!eina_init())
 *    {
 *        printf ("Error during the initialization of eina.\n");
 *        return EXIT_FAILURE;
 *    }
 *
 *    printf("initial value : 40.56\n");
 *    eina_convert_dtoa(40.56, tmp);
 *    printf("result dtoa   : %s\n", tmp);

 *    eina_convert_atod(tmp, 128, &m, &e);
 *    r = ldexp((double)m, e);
 *    printf("result atod   : %f\n", r);
 *
 *    eina_shutdown();
 *
 *    return EXIT_SUCCESS;
 * }
 * @endcode
 *
 * Compile this code with the following command:
 *
 * @code
 * gcc -Wall -o test_eina_convert test_eina.c `pkg-config --cflags --libs eina` -lm
 * @endcode
 */

/**
 * @addtogroup Eina_Tools_Group Tools
 *
 * @{
 */

/**
 * @defgroup Eina_Convert_Group Convert
 *
 * @{
 */

EAPI extern Eina_Error EINA_ERROR_CONVERT_P_NOT_FOUND; /**< Not used, perhaps a placeholder?  Defined as 0 in eina_convert.c*/
EAPI extern Eina_Error EINA_ERROR_CONVERT_0X_NOT_FOUND; /**< Not used, perhaps a placeholder?  Defined as 0 in eina_convert.c*/
EAPI extern Eina_Error EINA_ERROR_CONVERT_OUTRUN_STRING_LENGTH; /**< Not used, perhaps a placeholder?  Defined as 0 in eina_convert.c*/

/**
 * @brief Converts an integer number to a string in decimal base.
 *
 * @param[in] n The integer to convert.
 * @param[out] s The buffer to store the converted integer.
 * @return The length of the string, including the null terminated
 * character.
 *
 * This function converts @p n to a null terminated string. The
 * converted string is in decimal base. As no check is done, @p s must
 * be a buffer that is sufficiently large to store the integer.
 *
 * The returned value is the length of the string, including the null
 * terminated character.
 */
EAPI int       eina_convert_itoa(int n, char *s)  EINA_ARG_NONNULL(2);

/**
 * @brief Converts an integer number to a string in hexadecimal base.
 *
 * @param[in] n The integer to convert.
 * @param[out] s The buffer to store the converted integer.
 * @return The length of the string, including the null terminated
 * character.
 *
 * This function converts @p n to a null terminated string. The
 * converted string is in hexadecimal base and the alphabetical
 * cyphers are in lower case. As no check is done, @p s must be a
 * buffer that is sufficiently large to store the integer.
 *
 * The returned value is the length of the string, including the nul
 * terminated character.
 */
EAPI int       eina_convert_xtoa(unsigned int n, char *s) EINA_ARG_NONNULL(2);


/**
 * @brief Converts a double to a string.
 *
 * @param[in] d The double to convert.
 * @param[out] des The destination buffer to store the converted double.
 * @return #EINA_TRUE on success, #EINA_FALSE otherwise.
 *
 * This function converts the double @p d to a string. The string is
 * stored in the buffer pointed by @p des and must be sufficiently
 * large to contain the converted double. The returned string is null
 * terminated and has the following format:
 *
 * @code
 * [-]0xh.hhhhhp[+-]e
 * @endcode
 *
 * where the h are the hexadecimal cyphers of the mantissas and e the
 * exponent (a decimal number).
 *
 * The returned value is the length of the string, including the null
 * character.
 */
EAPI int       eina_convert_dtoa(double d, char *des) EINA_ARG_NONNULL(2);

/**
 * @brief Converts a string to a double.
 *
 * @param[in] src The string to convert.
 * @param[in] length The length of the string.
 * @param[out] m The mantissa.
 * @param[out] e The exponent.
 * @return #EINA_TRUE on success, #EINA_FALSE otherwise.
 *
 * This function converts the string @p s of length @p length that
 * represent a double in hexadecimal base to a double. It is used to
 * replace the use of snprintf() with the \%a modifier, which is
 * missing on some platform (like Windows (tm) or OpenBSD).
 *
 * The string must have the following format:
 *
 * @code
 * [-]0xh.hhhhhp[+-]e
 * @endcode
 *
 * where the h are the hexadecimal cyphers of the mantissas and e the
 * exponent (a decimal number). If n is the number of cyphers after the
 * point, the returned mantissas and exponents are:
 *
 * @code
 * mantissa  : [-]hhhhhh
 * exponent : 2^([+-]e - 4 * n)
 * @endcode
 *
 * The mantissas and exponent are stored in the buffers pointed
 * respectively by @p m and @p e.
 *
 * If the string is invalid #EINA_FALSE is returned, otherwise #EINA_TRUE is
 * returned.
 */
EAPI Eina_Bool eina_convert_atod(const char *src,
                                 int         length,
                                 long long  *m,
                                 long       *e) EINA_ARG_NONNULL(1, 3, 4);

/**
 * @brief Converts a 32.32 fixed point number to a string.
 *
 * @param[in] fp The fixed point number to convert.
 * @param[out] des The destination buffer to store the converted fixed point number.
 * @return #EINA_TRUE on success, #EINA_FALSE otherwise.
 *
 * This function converts the 32.32 fixed point number @p fp to a
 * string. The string is stored in the buffer pointed by @p des and
 * must be sufficiently large to contain the converted fixed point
 * number. The returned string is terminated and has the following
 * format:
 *
 * @code
 * [-]0xh.hhhhhp[+-]e
 * @endcode
 *
 * where the h are the hexadecimal cyphers of the mantissas and e the
 * exponent (a decimal number).
 *
 * The returned value is the length of the string, including the null
 * character.
 *
 * @note The code is the same than eina_convert_dtoa() except that it
 * implements the frexp() function for fixed point numbers and does
 * some optimization.
 */
EAPI int       eina_convert_fptoa(Eina_F32p32 fp,
                                  char       *des) EINA_ARG_NONNULL(2);

/**
 * @brief Converts a string to a 32.32 fixed point number.
 *
 * @param[in] src The string to convert.
 * @param[in] length The length of the string.
 * @param[out] fp The fixed point number.
 * @return #EINA_TRUE on success, #EINA_FALSE otherwise.
 *
 * This function converts the string @p src of length @p length that
 * represent a double in hexadecimal base to a 32.32 fixed point
 * number stored in @p fp. The function always tries to convert the
 * string with eina_convert_atod().
 *
 * The string must have the following format:
 *
 * @code
 * [-]0xh.hhhhhp[+-]e
 * @endcode
 *
 * where the h's are the hexadecimal cyphers of the mantissas and e the
 * exponent (a decimal number). If n is the number of cyphers after the
 * point, the returned mantissas and exponents are:
 *
 * @code
 * mantissa  : [-]hhhhhh
 * exponent : 2^([+-]e - 4 * n)
 * @endcode
 *
 * The mantissas and exponent are stored in the buffers pointed
 * respectively by @p m and @p e.
 *
 * If the string is invalid, #EINA_FALSE is returned,
 * otherwise @p fp is computed and #EINA_TRUE is returned.
 *
 * @note The code uses eina_convert_atod() and do the correct bit
 * shift to compute the fixed point number.
 */
EAPI Eina_Bool eina_convert_atofp(const char  *src,
                                  int          length,
                                  Eina_F32p32 *fp) EINA_ARG_NONNULL(1, 3);

/**
 * @brief Converts a string to a floating point number.
 *
 * @param[in] nptr a string to convert. It shouldn't be NULL.
 * @param[out] endptr If @p endptr is not NULL, a pointer to the character after the last
 *                    character used in the conversion is stored in the location referenced
 *                    by endptr. If @p nptr is NULL, this will also be set to NULL.
 * @return a double type floating point number.
 *
 * This function returns converted floating point number with locale-independency.
 * Actually, it use "C" locale for strtod_l function internally. If you need strtod
 * without locale-dependency, this function can replace strtod.
 * For more information, please refer documents of strtod, strtod_l.
 */
EAPI double eina_convert_strtod_c(const char *nptr, char **endptr);

/**
 * @}
 */

/**
 * @}
 */

#endif /* EINA_CONVERT_H_ */
