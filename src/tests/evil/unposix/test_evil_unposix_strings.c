// #include "src\lib\evil\unposix\strings.h";
#include "strings.h"
#include <stdio.h>

int equals_strings(){
    char *str1,*str2;
    str1 = "fOo";
    str2 = "FoO";
    
    return strcasecmp(str1,str2);
}

int str1_lessthan_str2(){
    char *str1,*str2;
    str1 = "fOo";
    str2 = "fOoO";
    
    return strcasecmp(str1,str2);
}

int str1_greaterthan_str2(){
    char *str1,*str2;
    str1 = "fOoO";
    str2 = "fOo";
    
    return strcasecmp(str1,str2);
}

int str1_n_equals_str2(){
    char *str1,*str2;
    str1 = "fOoO";
    str2 = "fOo";
    
    return strncasecmp(str1,str2,3);
}
int main()
{
    
    printf("str1 equal str2 (should be 0): %d\n", equals_strings());
    printf("str1 less than str2 (should be < 0): %d\n", str1_lessthan_str2());    
    printf("str1 greater than str2 (should be > 0): %d\n", str1_greaterthan_str2());
    printf("str1 first 3 chars equals str2 (should be 0): %d\n", str1_n_equals_str2());

    return 0;
}

