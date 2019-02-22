#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define MAX_SIZE 1024
 
int main()
{
    FILE *fstream = NULL;
    int error=0;
    char buff[MAX_SIZE]={0};
 
    if(NULL == (fstream=popen("nc -l  5888","r")))//这个应该是写方式的管道
    {
        fprintf(stderr,"execute command failed:%s",strerror(error));
        return -1;
    }
 
    while(NULL != fgets(buff,sizeof(buff),fstream))
    {
        printf("%s",buff);
    }
    pclose(fstream);
    printf("Hello world!\n");
    return 0;
}
