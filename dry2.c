#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* foo(char* str, int* len_ptr)   
{
    if(len_ptr==NULL)
    {
        return NULL;
    }
    char* str2;
    int i;
    *len_ptr = strlen(str);
    str2 = malloc(*len_ptr+1);
    for (i = 0; i < *len_ptr; i++){
        str2[i] = str[*len_ptr - i -1];
    }
    if (*len_ptr % 2 != 0) {
        printf("%s", str);
    }
    if (*len_ptr % 2 == 0)
    {
        printf("%s", str2);
    }
    return str2;
}