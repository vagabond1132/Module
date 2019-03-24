#include <sys/time.h>
#include <sys/resource.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    
    struct rlimit currlimit;
    
    printf("\n ** limit core ** \n");
    if ( getrlimit( RLIMIT_CORE, &currlimit) < 0)
    {
        printf(" err:\n"); 
    }

    printf("rlimit.rlim_cur = %d\n", currlimit.rlim_cur);
    printf("rlimit.rlim_max = %d\n", currlimit.rlim_max);


    printf("\n ** limit fsize **\n");
    if ( getrlimit( RLIMIT_FSIZE, &currlimit) < 0)
    {
        printf(" err:\n"); 
    }

    printf("rlimit.rlim_cur = %d\n", currlimit.rlim_cur);
    printf("rlimit.rlim_max = %d\n", currlimit.rlim_max);

    
    printf("\n ** limit nofile **\n");
    if ( getrlimit( RLIMIT_NOFILE, &currlimit) < 0)
    {
        printf(" err:\n"); 
    }

    printf("rlimit.rlim_cur = %d\n", currlimit.rlim_cur);
    printf("rlimit.rlim_max = %d\n", currlimit.rlim_max);



    printf("\n ** limit stack ** \n");
    if ( getrlimit( RLIMIT_STACK, &currlimit) < 0)
    {
        printf(" err:\n"); 
    }

    printf("rlimit.rlim_cur = %d\n", currlimit.rlim_cur);
    printf("rlimit.rlim_max = %d\n", currlimit.rlim_max);


    return 0;
}
