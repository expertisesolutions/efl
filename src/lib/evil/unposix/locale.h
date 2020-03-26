#ifndef UNPOSIX_LOCALE_H
#define UNPOSIX_LOCALE_H

#include_next <locale.h>

#define LC_CTYPE            0
#define LC_NUMERIC          1
#define LC_TIME             2
#define LC_COLLATE          3
#define LC_MONETARY         4
#define LC_MESSAGES         5
#define LC_ALL              6
#define LC_PAPER            7
#define LC_NAME             8
#define LC_ADDRESS	        9
#define LC_TELEPHONE	    10
#define LC_MEASUREMENT	    11
#define LC_IDENTIFICATION	12 

struct _locale_t
{
  struct __locale_data *__locales[13]; /* 13 = __LC_LAST. */
  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;
  const char *__names[13];
};
 
typedef _locale_t *locale_t;

#endif
