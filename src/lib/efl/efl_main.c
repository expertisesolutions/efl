#ifdef _WIN32
#include <windows.h>

BOOL WINAPI
DllMain(HINSTANCE const instance, DWORD const reason, LPVOID const reserved)
{
   (void) instance;
   (void) reason;
   (void) reserved;
   return TRUE;
}

#endif
