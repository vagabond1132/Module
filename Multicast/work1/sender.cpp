#include <sys/types.h>    
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>    
#include <time.h>    
#include <string.h>    
#include <stdio.h>    
#include <unistd.h>    
#include <stdlib.h>    
#define HELLO_PORT 12345    
#define HELLO_GROUP "225.0.0.37"    

int main(int argc, char *argv[])    
{    
    struct sockaddr_in addr;    
    int fd, cnt, i;    
    struct ip_mreq mreq;    
    char *message = (char *) malloc(sizeof(char) * 128);    

    /* create what looks like an ordinary UDP socket */    
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)     
    {    
        perror("socket");    
        exit(1);    
    }    
    /* set up destination address */    

    memset(&addr,0,sizeof(addr));    
    addr.sin_family=AF_INET;    
    addr.sin_addr.s_addr=inet_addr(HELLO_GROUP);    
    addr.sin_port=htons(HELLO_PORT);    
    /* now just sendto() our destination! */    
    i = 0;
    while (1)    
    {    
    	sprintf(message,"%d: %s",i++,"Hello,world");
        if (sendto(fd, message, strlen(message), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0)     
        {    
            perror("sendto");    
            exit(1);    
        }    
        sleep(1);    
    }    

	free(message);
	message = NULL;
    return 0;    
}  
