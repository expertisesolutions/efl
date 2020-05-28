#ifndef _MSC_VER
# include <unistd.h>
#else
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <windows.h>
# undef WIN32_LEAN_AND_MEAN
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
