/* Getopt for Microsoft C
This code is a modification of the Free Software Foundation, Inc.
Getopt library for parsing command line argument the purpose was
to provide a Microsoft Visual C friendly derivative. This code
provides functionality for both Unicode and Multibyte builds.
Date: 02/03/2011 - Ludvik Jerabek - Initial Release
Version: 1.0
Comment: Supports getopt, getopt_long, and getopt_long_only
and POSIXLY_CORRECT environment flag
License: LGPL
Revisions:
02/03/2011 - Ludvik Jerabek - Initial Release
02/20/2011 - Ludvik Jerabek - Fixed compiler warnings at Level 4
07/05/2011 - Ludvik Jerabek - Added no_argument, required_argument, optional_argument defs
08/03/2011 - Ludvik Jerabek - Fixed non-argument runtime bug which caused runtime exception
08/09/2011 - Ludvik Jerabek - Added code to export functions for DLL and LIB
02/15/2012 - Ludvik Jerabek - Fixed _GETOPT_THROW definition missing in implementation file
08/01/2012 - Ludvik Jerabek - Created separate functions for char and wchar_t characters so single dll can do both unicode and ansi
10/15/2012 - Ludvik Jerabek - Modified to match latest GNU features
06/19/2015 - Ludvik Jerabek - Fixed maximum option limitation caused by option_a (255) and option_w (65535) structure val variable
**DISCLAIMER**
THIS MATERIAL IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING, BUT Not LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE, OR NON-INFRINGEMENT. SOME JURISDICTIONS DO NOT ALLOW THE
EXCLUSION OF IMPLIED WARRANTIES, SO THE ABOVE EXCLUSION MAY NOT
APPLY TO YOU. IN NO EVENT WILL I BE LIABLE TO ANY PARTY FOR ANY
DIRECT, INDIRECT, SPECIAL OR OTHER CONSEQUENTIAL DAMAGES FOR ANY
USE OF THIS MATERIAL INCLUDING, WITHOUT LIMITATION, ANY LOST
PROFITS, BUSINESS INTERRUPTION, LOSS OF PROGRAMS OR OTHER DATA ON
YOUR INFORMATION HANDLING SYSTEM OR OTHERWISE, EVEN If WE ARE
EXPRESSLY ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*/
#ifndef __GETOPT_H_
	#define __GETOPT_H_

	#ifdef _GETOPT_API
		#undef _GETOPT_API
	#endif

	#if defined(EXPORTS_GETOPT) && defined(STATIC_GETOPT)
		#error "The preprocessor definitions of EXPORTS_GETOPT and STATIC_GETOPT can only be used individually"
	#elif defined(STATIC_GETOPT)
		#define _GETOPT_API
	#elif defined(EXPORTS_GETOPT)
		#pragma message("Exporting getopt library")
		#define _GETOPT_API __declspec(dllexport)
	#else
		#pragma message("Importing getopt library")
		#define _GETOPT_API __declspec(dllimport)
	#endif

	// Change behavior for C\C++
	#ifdef __cplusplus
		#define _BEGIN_EXTERN_C extern "C" {
		#define _END_EXTERN_C }
		#define _GETOPT_THROW throw()
	#else
		#define _BEGIN_EXTERN_C
		#define _END_EXTERN_C
		#define _GETOPT_THROW
	#endif

	// Standard GNU options
	#define	null_argument		0	/*Argument Null*/
	#define	no_argument			0	/*Argument Switch Only*/
	#define required_argument	1	/*Argument Required*/
	#define optional_argument	2	/*Argument Optional*/

	// Shorter Options
	#define ARG_NULL	0	/*Argument Null*/
	#define ARG_NONE	0	/*Argument Switch Only*/
	#define ARG_REQ		1	/*Argument Required*/
	#define ARG_OPT		2	/*Argument Optional*/

	#include <../ucrt/string.h>

_BEGIN_EXTERN_C

	extern _GETOPT_API int optind;
	extern _GETOPT_API int opterr;
	extern _GETOPT_API int optopt;

	// Ansi
	struct option
	{
		const char* name;
		int has_arg;
		int *flag;
		int val;
	};
	extern _GETOPT_API char *optarg;
	extern _GETOPT_API int getopt(int argc, char *const *argv
                               , const char *optstring) _GETOPT_THROW;
	extern _GETOPT_API int getopt_long(int argc, char *const *argv
                                    , const char *options
                                    , const struct option *long_options
                                    , int *opt_index) _GETOPT_THROW;
	extern _GETOPT_API int getopt_long_only(int argc, char *const *argv
                                         , const char *options
                                         , const struct option *long_options
                                         , int *opt_index) _GETOPT_THROW;

_END_EXTERN_C

	#undef _BEGIN_EXTERN_C
	#undef _END_EXTERN_C
	#undef _GETOPT_THROW
	#undef _GETOPT_API

#endif  // __GETOPT_H_
