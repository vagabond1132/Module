#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <errno.h>


#define MAXLISTEN 2 
#define PORT  4677

#ifndef DEBUG_PRINT

#define DEBUG(...)  \
    do { \
        fprintf(stderr, "------DEBUG-INFO-----\n");  \
        fprintf(stderr, "%s, %s\n", __TIME__, __DATE__ );  \
        fprintf(stderr, "%s:%d:%s()\n", __FILE__, __LINE__, __func__);  \
        fprintf(stderr, __VA_ARGS__);  \
        fprintf(stderr, "\n------DEBUG-Over-----\n");  \
    } while(0);

#else 

#define DEBUG(...)  fprintf(stdout, __VA_ARGS__);  

#endif


static int ListenSock[MAXLISTEN];

//按照fd进行顺序排放; 插入; 查找; -- 存储对应的fd, addr, ip;
//typedef struct Node
//{
//    int fd;
//    char host[16];
//    int port; 
//    struct Node* prevNode; //设计双向节点;;;
//    struct Node* nextNode;
//}Node_t;
//
//Node_t *head = NULL;

/*
 * FD_ZERO  将指定的文件描述符清空;
 * FD_SET   在指定的文件描述符中新增一个文件描述符;
 * FD_CLR   在指定的文件描述符集合中删除一个文件描述符
 * FD_ISSET 用于测试指定文件描述符是否在 集合中;
 */


static int initMasks(fd_set *rmask)
{
    int maxsock =  -1;
    unsigned  i = 0; 

    FD_ZERO(rmask); //文件描述符集合; -- 清空操作;

    for( i = 0 ; i < MAXLISTEN ; i++)
    {
        int fd = ListenSock[i];    
        
        if(  -1 == fd )
            break;

        FD_SET(fd, rmask); //在rmask 中新增一个文件描述符;

        if( fd > maxsock )
            maxsock = fd; 
    }
    
    return maxsock + 1; //返回use socket num;
}


int main(int argc, char *argv[])
{
    
    // 尚未绑定本地端口;;
    int maxfd = 0;
    fd_set readmask;
    int socket_fd = 0, client_fd = 0;
    socklen_t sin_size = 0;
    int ret  = 0, yes =  1;
    int count = 0;
    struct sockaddr_in server_addr;
    struct  sockaddr_in client_addr;
    int connNum = 0;
    char buf[BUFSIZ];
    
    //Node_t *node = NULL;

    sin_size = sizeof(client_addr);

    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        DEBUG("create socket fail");    
        exit(1);
    }

    //设置socket 套接字:
    // 打开Mac地址复用;   yes = 1: on;  = 0, off
    if ( setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1 )
    {
        DEBUG("socket set reuseraddr fail"); 
        exit(1);
    }

    server_addr.sin_family = AF_INET; //ipv4;
    server_addr.sin_port = htons(PORT); //设置port;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));


    if(bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr) ) == -1) //绑定本地地址;
    {
        DEBUG("bind lcoal Addr fail"); 
        exit(1);
    }
    
    //listen;
    if(listen(socket_fd, 10) == -1)
    {
        DEBUG("listen...");
        exit(1); 
    }


    FD_ZERO(&readmask);
    maxfd = socket_fd; //文件描述符 从3开始不断自增;
    /*
     * tcp-selec 31082 ligang    0u   CHR    136,4      0t0        7 /dev/pts/4
     * tcp-selec 31082 ligang    1u   CHR    136,4      0t0        7 /dev/pts/4
     * tcp-selec 31082 ligang    2u   CHR    136,4      0t0        7 /dev/pts/4
     * tcp-selec 31082 ligang    3u  IPv4 22801762      0t0      TCP *:bcs (LISTEN)
     * tcp-selec 31082 ligang    4u  IPv4 22801776      0t0      TCP localhost:bcs->localhost:52094 (ESTABLISHED)
     * tcp-selec 31082 ligang    5u  IPv4 22801825      0t0      TCP localhost:bcs->localhost:52098 (ESTABLISHED)
     */

    /*
     *  　int select(int maxfd, fd_set *rdset, fd_set *wrset, fd_set *exset, struct timeval *timeout);
     *  maxfd  最大描述符 +1;
     *  reset  可读文件描述符集合;
     *  wrset  可写文件描述符集合;
     *  exset  异常文件描述符集合;
     *  timeout 设置为0 , 意味着是一个非堵塞函数;
     */

    while(1)
    {
    
        //maxfd = initMasks(&readmask); //设置select 的readmask;
        FD_SET(socket_fd, &readmask); //监听;; --- 不断监听;;;

        for( count = 0; count < connNum; count++) //一直在循环;;
        {
            if(ListenSock[count] != 0)
                FD_SET(ListenSock[count], &readmask);
        }

        ret = select(maxfd + 1,  &readmask, NULL, NULL , 0);  // 监控的fd 在当前的最大值 + 1;
    
        if( ret < 0 )
        {
            if (errno != EINTR && errno != EWOULDBLOCK)
                break;
        }
        else if( ret == 0 )
            continue; //说明当前超时;; 为获取当完整fd;

        for( count = 0; count < connNum; count++)
        {

            if( FD_ISSET(ListenSock[count], &readmask) ) //loop read;
            {
                ret = recv(ListenSock[count], buf, sizeof(buf), 0);

                send(ListenSock[count], buf, sizeof(buf), 0 ); //ligang;

                if(ret == 0) //end err
                {
                    printf("client close  \n");//, inet_ntoa(ListenSock[count].sin_addr), ntohs(ListenSock[count].sin_port); 
                    //进行文件描述符的清除工作;
                    close(ListenSock[count]);
                    FD_CLR(ListenSock[count], &readmask);
                    ListenSock[count] = 0; 
                
                }
                else
                {
                    printf("fd = %d :%s\n", ListenSock[count], buf);
                    memset(buf, 0, BUFSIZ);
                }
            }
        
        }


        
        if( FD_ISSET(socket_fd, &readmask)) //当前的socket_fd 在select的轮训中;
        {
            //将其关联关系进行存储;;;     
            client_fd =  accept(socket_fd, (struct sockaddr *)&client_addr, &sin_size); //接收到客户端描述符;;  


            if( client_fd <= 0 )
            {
                DEBUG("accept client fd fail") 
                continue;
            }
            
           // node = (Node_t *)malloc(sizeof(Node_t));
           
             //文件描述符的数量;;   
             //ListenSock[conn++] = client_fd; //新增加一个;  添加到 ListenSock数组中;;


            if( connNum < MAXLISTEN )
            {
                ListenSock[connNum++] = client_fd;

                printf("new connection client: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                if( client_fd > maxfd)
                    maxfd = client_fd; //监听的socket +1;
            
            }
            else
            {
                printf("max connections arrive , exit\n"); 
                send(client_fd, "bye", 4, 0);
                close(client_fd);
                break;
            } 

        }



    }
    

    return 0;
}
