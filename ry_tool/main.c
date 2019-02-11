#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "init.h"

#define HIGHBIT                 (0x80)
#define IS_HIGHBIT_SET(ch)      ((unsigned char)(ch) & HIGHBIT)

int
Ry_strncasecmp(const char *s1, const char *s2, size_t n)
{
    while (n-- > 0)
    {
        unsigned char ch1 = (unsigned char) * s1++;
        unsigned char ch2 = (unsigned char) * s2++;

        if (ch1 != ch2)
        {
            if (ch1 >= 'A' && ch1 <= 'Z')
                ch1 += 'a' - 'A';
            else if (IS_HIGHBIT_SET(ch1) && isupper(ch1))
                ch1 = tolower(ch1);

            if (ch2 >= 'A' && ch2 <= 'Z')
                ch2 += 'a' - 'A';
            else if (IS_HIGHBIT_SET(ch2) && isupper(ch2))
                ch2 = tolower(ch2);

            if (ch1 != ch2)
                return (int) ch1 - (int) ch2;
        }
        if (ch1 == 0)
            break;
    }
    return 0;
}


int
Ry_strcasecmp(const char *s1, const char *s2)
{
    for (;;)
    {
        unsigned char ch1 = (unsigned char) * s1++;
        unsigned char ch2 = (unsigned char) * s2++;

        if (ch1 != ch2)
        {
            if (ch1 >= 'A' && ch1 <= 'Z')
                ch1 += 'a' - 'A';
            else if (IS_HIGHBIT_SET(ch1) && isupper(ch1))
                ch1 = tolower(ch1);

            if (ch2 >= 'A' && ch2 <= 'Z')
                ch2 += 'a' - 'A';
            else if (IS_HIGHBIT_SET(ch2) && isupper(ch2))
                ch2 = tolower(ch2);

            if (ch1 != ch2)
                return (int) ch1 - (int) ch2;
        }
        if (ch1 == 0)
            break;
    }
    return 0;
}


int main(int argc, char *argv[])
{
    char *cmd = NULL;
    int i = 0 ;
    char ch = 'H';

    for ( ; i < argc; i++ )
    {
        cmd  = argv[i];

        printf("H => %c\n", IS_HIGHBIT_SET(ch));

        if ( pg_strcasecmp(cmd, "init") == 0)
        {
            do_init();
        }
    }


    return 0;
}
