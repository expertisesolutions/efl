#ifndef _MSC_VER
#include <unistd.h>
#else
#include <windows.h>
#endif

#if defined(__clang__)
# pragma clang diagnostic ignored "-Wunused-parameter"
#elif (__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


int
main(int arc, char *argv[])
{
#ifndef _MSC_VER
   sleep(60);
#else
   Sleep(60);
#endif
   return 0;
}
