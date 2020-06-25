#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "evil_private.h"
#include <evil_api.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "evil_stat.h"

#define	AT_SYMLINK_NOFOLLOW	0x01

EVIL_API_H int 
fstatat(int dirfd, const char *pathname, struct stat *statbuf, int flags)
{
   int r_fstatat;

   if (pathname[2] == '\\')
     {  
        if (flags == AT_SYMLINK_NOFOLLOW)
          r_fstatat = stat(pathname, statbuf);
        else
          r_fstatat = stat(pathname, statbuf);
        return r_fstatat;
     }
   else
     {
        char *pathbuf = NULL;
        size_t pathbuf_size = 0;
        DWORD copied = 0;
        do
        {
           pathbuf_size += MAX_PATH;
           pathbuf = realloc(pathbuf, pathbuf_size * sizeof(char));
           copied = GetModuleFileName(NULL, pathbuf, pathbuf_size);
        } while (copied >= pathbuf_size);
       
        pathbuf_size = copied;
        pathbuf = realloc(pathbuf, pathbuf_size * sizeof(char)); 
        copied = GetModuleFileName(NULL, pathbuf, pathbuf_size);

        int size_str;
        for (size_str = strlen(pathbuf); size_str >= 0; size_str --)
         {
            if (pathbuf[size_str] == '\\')
              {
                 pathbuf[size_str + 1] = 0;
                 break;
              }
         }

        size_str = strlen(pathbuf) + strlen(pathname);

        char *path_complete = malloc(sizeof(char) * size_str);

        path_complete =  strcat(pathbuf, pathname);
      

        if (flags == AT_SYMLINK_NOFOLLOW)
          {
             r_fstatat = stat(path_complete, statbuf);
          }
        else
          { 
             r_fstatat = stat(path_complete, statbuf);
          }
        return r_fstatat;
     }
}