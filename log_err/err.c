
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>


#define ERR_EXIT(m) \
    do \
    {  \
        perror(m); \
        exit(EXIT_FAILURE); \
    }while(0)



int main(){
    errno = 21;
    ERR_EXIT("SIGNAL ERR");


    return 0;
}
