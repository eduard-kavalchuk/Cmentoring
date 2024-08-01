#include <stdio.h>
#include <string.h>

const char *issubstring(const char *str1, const char *str2 );

int main()
{
    printf("%p\n", issubstring("abcdefg", "cde"));
    printf("%p\n", issubstring("abcdfg", "cde"));
    
    return 0;
}

const char *issubstring(const char *str1, const char *str2 )
{
    return strstr(str1, str2);
}
