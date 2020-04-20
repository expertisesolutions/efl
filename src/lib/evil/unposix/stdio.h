#ifndef UNPOSIX_STDIO_H
#define UNPOSIX_STDIO_H

//#include <evil_stdio.h>
#include <../ucrt/stdio.h>
#include <stdarg.h>

int vasprintf(char **str, const char *fmt, va_list ap);

#endif
