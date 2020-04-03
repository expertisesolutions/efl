#define XSTR(x) STR(x)
#define STR(x) #x
#ifdef EVIL_API
#pragma message "Value of EVIL_API = " XSTR(EVIL_API)
#else
#pragma message "Value of EVIL_API = <undefined>"
#endif
