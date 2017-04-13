#include <sys/types.h>    
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>    
#include <time.h>    
#include <string.h>    
#include <stdio.h>    
#include <unistd.h>    
#include <stdlib.h>    
#define HELLO_PORT  12345    
#define HELLO_GROUP "225.0.0.37"    
#define MSGBUFSIZE 256    

int main(int argc, char *argv[])    
{    
    struct sockaddr_in addr;    
    int fd, nbytes,addrlen;    
    struct ip_mreq mreq;    
    char msgbuf[MSGBUFSIZE];    
    u_int yes=1; /*** MODIFICATION TO ORIGINAL */    

    // 接收到退出信号的时候， 进行组播的退出;

    /* create what looks like an ordinary UDP socket */    
    if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)     
    {    
        perror("socket");    
        exit(1);    
    }    
    /**** MODIFICATION TO ORIGINAL */    

    /* allow multiple sockets to use the same PORT number */    
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)    // 允许重新绑定网卡， 不必等网卡释放;
    {    
        perror("Reusing ADDR failed");    
        exit(1);    
    }    
    /*** END OF MODIFICATION TO ORIGINAL */    

    /* set up destination address */    
    memset(&addr,0,sizeof(addr));    
    addr.sin_family=AF_INET;    
    addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */    
    addr.sin_port=htons(HELLO_PORT);    

    /* bind to receive address */    
    if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0)    
    {    
        perror("bind");    
        exit(1);    
    }    

    //加入组播组;
    /* use setsockopt() to request that the kernel join a multicast group */    
    mreq.imr_multiaddr.s_addr=inet_addr(HELLO_GROUP); // 加入 或者 退出广播组Ip地址   
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);   // 加入 或者 退出的网络接口ip地址;
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)     
    {    
        perror("setsockopt");    
        exit(1);    
    }    

    /* now just enter a read-print loop */    
    while (1)     
    {    
        //ssize_t recvfrom(int s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);    
        addrlen=sizeof(addr);    
        //  指定接收地址;
        if ((nbytes=recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *) &addr, (socklen_t *)&addrlen)) < 0)     
        {    
            perror("recvfrom");    
            exit(1);    
        }    
        puts(msgbuf);    
    }    
    

    if (setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)     
    {    
        perror("setsockopt");    
        exit(1);    
    }    


    return 0;    
}    
