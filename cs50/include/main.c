#include <stdio.h>

typedef struct String{
    
    unsigned encode : 4 ;

    unsigned len : 4 ;

    char content[];

}String;

int main()
{
    printf("%d \n", sizeof(String));

    return 0;
}
