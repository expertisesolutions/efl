#ifndef EINA_STDIO_H
#define EINA_STDIO_H

#ifdef _WIN32

#include <../ucrt/stdio.h>
#include <stdarg.h>

int vasprintf(char **str, const char *fmt, va_list ap);

#else  /* _WIN32 */
#include <stdio.h>
#endif /* _WIN32 */

#endif
