#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/tcp.h>

#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/sockios.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <netinet/in.h>

#define PACKET_SIZE     4096
#define MAX_WAIT_TIME   5
#define MAX_NO_PACKETS  3

int socketfd = 0;
struct sockaddr_in destaddr; //设置目标地址;


unsigned short cal_chksum(unsigned short *addr, int len)
{
     int nleft = len;
     int sum = 0;
     unsigned short *w = addr;
     unsigned short answer = 0;
             
    /*把ICMP报头二进制数据以2字节为单位累加起来*/
     while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;

    }
    /*若ICMP报头为奇数个字节，会剩下最后一字节。把最后一个字节视为一个2字节数据的高字节，这个2字节数据的低字节为0，继续累加*/
    if ( nleft == 1)
    {
               *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;

    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}









int main(int argc, char *argv[])
{

    struct hostent *host;
    unsigned long inaddr = 01;
    int waittime = MAX_WAIT_TIME;
    int size = 50 * 1024; //设置recvbuf 为50kb;

    
    if( (socketfd =  socket(AF_INET, SOCK_RAW, IPPROTO_ICMP) ) < 0)
    {
        perror("create socket"); 
        exit(1); 
    }

    setuid(getuid()); //需要使用超级用户;;;


    setsockopt(socketfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)); //设置接收缓冲区;
    
    bzero(&destaddr, sizeof(destaddr);
    destaddr.sin_family = AF_INET; ///IPv4;
    //destaddr.sin_addr.s_addr = INADDR_ANY; //需要设置成对方的ip;;
    
    
    



    return 0;
}
