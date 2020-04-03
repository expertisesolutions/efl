#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "fnmatch.h"

int test_normal_string(void);
int test_path_string(void);

int test_normal_string(void)
{
    const char * string = "/usr/include/bits/types/__fpos64_t.h";
    const char * pattern = "*";
    const int flags = 0;

    int ret = fnmatch(pattern, string, flags); 

    printf("test normal string: %cok!\n", !ret?:'n');

    return ret;
}

int test_path_string(void)
{
    const char * string = "/usr/include/bits/types/__fpos64_t.h";
    const char * pattern = "/*/*/bit?/types/__fpos[36][24]_t.[hc]";
    const int flags = FNM_PATHNAME | FNM_NOESCAPE | FNM_PERIOD;

    int ret = fnmatch(pattern, string, flags);

    printf("test normal string: %cok!\n", !ret?:'n');

    return ret;
}

int main(void)
{
    assert(test_normal_string() == 0);
    assert(test_path_string() == 0);

    return 0;
}
