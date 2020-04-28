#ifndef EINA_STDIO_H
#define EINA_STDIO_H

#include <stdio.h>

#ifdef _WIN32

#include <stdarg.h>

int eina_vasprintf(char **str, const char *fmt, va_list ap);

#else

#define eina_vasprintf vasprintf

#endif /* _WIN32 */

#endif
