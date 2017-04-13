#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/socket.h>  
#include <netinet/in.h>           /* socket类定义需要*/  
#include <sys/epoll.h>            /* epoll头文件 */  
#include <fcntl.h>                    /* nonblocking需要 */  
#include <sys/resource.h>     /* 设置最大的连接数需要setrlimit */  
  
// ulimit -a   open file   // 查看proc/pid/   最大文件数
//#define MAXEPOLL    10000   /* 对于服务器来说，这个值可以很大的！ */  
#define MAXEPOLL    1000   /* 对于服务器来说，这个值可以很大的！ */  
#define MAXLINE     1024  
#define     PORT            6000  
#define MAXBACK  128
/*
 *  socket 
 *
 *  bind 
 *
 *  listen
 *
 *  accept
 *
 *  recv/send
 *
 *  close
 *
 * */

int setnonblocking( int fd )  
{  
    if( fcntl( fd, F_SETFL, fcntl( fd, F_GETFD, 0 )|O_NONBLOCK ) == -1 )  
    {  
        printf("Set blocking error : %d\n", errno);  
        return -1;  
    }  
    return 0;  
}  
  
int main( int argc, char ** argv )  
{  
    int         listen_fd;  
    int         conn_fd;  
    int         epoll_fd;  
    int         nread;  
    int         cur_fds;                //!> 当前已经存在的数量  
    int         wait_fds;               //!> epoll_wait 的返回值  
    int     i;  
    struct sockaddr_in servaddr;  
    struct sockaddr_in cliaddr;  
    struct  epoll_event ev;  
    struct  epoll_event evs[MAXEPOLL];  
    struct  rlimit  rlt;        //!> 设置连接数所需  
    char    buf[MAXLINE];  
    socklen_t   len = sizeof( struct sockaddr_in );  
  
    //!> 设置每个进程允许打开的最大文件数  
    //!> 每个主机是不一样的哦，一般服务器应该很大吧！  
    //!>   
    rlt.rlim_max = rlt.rlim_cur = MAXEPOLL;  
    if( setrlimit( RLIMIT_NOFILE, &rlt ) == -1 )      
    {  
        printf("Setrlimit Error : %d\n", errno);  
        exit( EXIT_FAILURE );  
    }  
      
    //!> server 套接口  
    //!>   
    bzero( &servaddr, sizeof( servaddr ) );  //设置 AF_INET  ipv4  s_addr ip,(localhost)   并且设置端口
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl( INADDR_ANY ); ///本机所有ip 
                           // inet_addr("192.168.0.1")
    servaddr.sin_port = htons( PORT );  
      
    //!> 建立套接字  
    if( ( listen_fd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) // 作用于为 ipv4，默认tcp流 
    {  
        printf("Socket Error...\n" , errno );  
        exit( EXIT_FAILURE );  
    }  
      
    //!> 设置非阻塞模式  
    //!>   
    if( setnonblocking( listen_fd ) == -1 ) //将网络描述符设置为非堵塞模式; 
    {  
        printf("Setnonblocking Error : %d\n", errno);  
        exit( EXIT_FAILURE );  
    }  
      
    //!> 绑定  
    //!>  
    if( bind( listen_fd, ( struct sockaddr *)&servaddr, sizeof( struct sockaddr ) ) == -1 ) // 将描述符 将本地地址联系到一起;  
    {  
        printf("Bind Error : %d\n", errno);  
        exit( EXIT_FAILURE );  
    }  
  
    //!> 监听  
    //!>   
    if( listen( listen_fd, MAXBACK ) == -1 )   // 不断监听词描述符; 监听消息队列长度  默认128  大于128时，强制归为128; -- 正在连接的监听
                                              //分为两个队列， 一个是未连接队列  tcp_max_syn_blocklog 即开始三次握手，但是未结束 ； 名字含义即可看出， tcp_最大 syn 积压数， syn存在于 三次握手； 
                                              //另一个已经成功连接 等待accept 提取  这个采用 MAXBACK
                                              //三次握手   client  syn j    server  ack j+1  syn k  client ack k+1  进行相互验证， 采用/proc/sys/net/ipv4/syn -- 数值.未连接
    {  
        printf("Listen Error : %d\n", errno);  
        exit( EXIT_FAILURE );  
    }  
      
    //!> 创建epoll  
    //!>   
    epoll_fd = epoll_create( MAXEPOLL );    //!> create  最大数目;
    ev.events = EPOLLIN | EPOLLET;      //!> accept Read!   设置为读 并 为边缘触发 
    //LT 水平触发 fd中还有数据可读，epoll_wait都会返回它的事件，提醒用户去操作；
    // ET  边缘触发只支持非堵塞状态， 只会提醒一次，直到下一次再有数据流入之前都不会进行提示。因此 recv 接收数组长度要大于发送长度，或者遇到 EAGAIN错误， 不然存在于描述符中的数据将会丢失 
    ev.data.fd = listen_fd;                 //!> 将listen_fd 加入 // 将创建的socket 描述符 关联进来; 
    if( epoll_ctl( epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev ) < 0 )   //添加
    {  
        printf("Epoll Error : %d\n", errno);  
        exit( EXIT_FAILURE );  
    }  
    cur_fds = 1;  
      
    while( 1 )   //不断循环， 等待描述符的相应;
    {  
        if( ( wait_fds = epoll_wait( epoll_fd, evs, cur_fds, -1 ) ) == -1 )//
        {  
            printf( "Epoll Wait Error : %d\n", errno );  
            exit( EXIT_FAILURE );  
        }  
  
        for( i = 0; i < wait_fds; i++ )  
        {  
            if( evs[i].data.fd == listen_fd && cur_fds < MAXEPOLL )    
                                                    //!> if是监听端口有事  
            {  
                if( ( conn_fd = accept( listen_fd, (struct sockaddr *)&cliaddr, &len ) ) == -1 )   // 接受连接
                {  
                    printf("Accept Error : %d\n", errno);  
                    exit( EXIT_FAILURE );  
                }  
                  
                printf( "Server get from client !\n"/*,  inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port */);  
                  
                ev.events = EPOLLIN | EPOLLET;      //!> accept Read!  
                ev.data.fd = conn_fd;                   //!> 将conn_fd 加入  
                if( epoll_ctl( epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev ) < 0 )  //
                {  
                    printf("Epoll Error : %d\n", errno);  
                    exit( EXIT_FAILURE );  
                }  
                ++cur_fds;   
                continue;         
            }  
              
            //!> 下面处理数据  
            //!>   
            nread = read( evs[i].data.fd, buf, sizeof( buf ) ); //
            if( nread <= 0 )                     //!> 结束后者出错  
            {  
                close( evs[i].data.fd );  
                epoll_ctl( epoll_fd, EPOLL_CTL_DEL, evs[i].data.fd, &ev );  //!> 删除计入的fd  
                --cur_fds;                  //!> 减少一个呗！  
                continue;  
            }  
              
            write( evs[i].data.fd, buf, nread );            //!> 回写  
        }  
    }  
      
    close( listen_fd );  
    return 0;  
} 
