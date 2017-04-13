#include <sys/types.h>    
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>    
#include <time.h>    
#include <string.h>    
#include <stdio.h>    
#include <unistd.h>    
#include <stdlib.h>    

/*
*   broadcast_server.c - 多播服务程序
*/

#define MCAST_PORT 8888
#define MCAST_ADDR "224.0.0.100"    /*一个局部连接多播地址，路由器不进行转发*/
#define MCAST_DATA "BROADCAST TEST DATA"            /*多播发送的数据*/

int main(int argc, char**argv)
{
    int s, i;
    struct sockaddr_in mcast_addr;     
    char *message = (char*)malloc(sizeof(char) * 128);
    s = socket(AF_INET, SOCK_DGRAM, 0);         /*建立套接字*/
    if (s == -1)
    {
        perror("socket()");
        return -1;
    }
   
    memset(&mcast_addr, 0, sizeof(mcast_addr));/*初始化IP多播地址为0*/
    mcast_addr.sin_family = AF_INET;                /*设置协议族类行为AF*/
    mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);/*设置多播IP地址*/
    mcast_addr.sin_port = htons(MCAST_PORT);        /*设置多播端口*/
   
                                                    /*向多播地址发送数据*/
    i = 0 ;
    while(1) {

        sprintf(message, "%d:  %s", i++, MCAST_DATA);
        int n = sendto(s,                           /*套接字描述符*/
                                    message,     /*数据*/
                                    sizeof(message),    /*长度*/
                                    0,
                                    (struct sockaddr*)&mcast_addr,
                                    sizeof(mcast_addr)) ;
        if( n < 0)
        {
            perror("sendto()");
            return -2;
        }      
       
        sleep(1);                          /*等待一段时间*/
    }
    free(message);
   
    return 0;
}
