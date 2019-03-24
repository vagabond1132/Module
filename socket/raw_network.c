#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <sys/ioctl.h>


int main(int argc, char **argv)
{
	int sock, n;
	char buffer[2048];
	struct ethhdr *eth;
	struct iphdr *iph;
    struct ifreq ifr;

	int ipcount, arpcount, rarpcount, allcount;

	ipcount = arpcount = rarpcount = allcount =  0;


	setuid(getuid()); //设置有效用户为root;

	if (0 > (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))))
    {
		perror("socket");
		exit(1);
	}


    //设置eth0 为混杂模式;
    //strcpy(ifr.ifr_name, "eth0");
    //if( ioctl(sock, SIOCGIFFLAGS, &ifr) < 0 )
    //{
    //    close(sock);
    //    return -1;
    //}
    //
    //if( !( ifr.ifr_flags  & IFF_RUNNING ) )
    //    return -1; //网卡为启动;
    //    
    //ifr.ifr_flags |= IFF_PROMISC; //设置为混杂;
    //if( ioctl(sock, SIOCSIFFLAGS, &ifr) < 0 )
    //    return -1;



	while (1) {
		printf("=====================================\n");
		//注意：在这之前我没有调用bind函数，原因是什么呢？
		n = recvfrom(sock, buffer, 2048, 0, NULL, NULL);
		printf("%d bytes read\n", n);

        if( buffer[12] == 0x08 && buffer[13] == 0x00)
        {
        
            eth = (struct ethhdr*)buffer;
            printf("Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
            printf("Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);

            iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
            //我们只对IPV4且没有选项字段的IPv4报文感兴趣
            if(iph->version == 4 && iph->ihl == 5){
                unsigned char *sd, *dd;
                sd = (unsigned char*)&iph->saddr;
                dd = (unsigned char*)&iph->daddr;
                printf("Source Host: %d.%d.%d.%d Dest host: %d.%d.%d.%d\n", sd[0], sd[1], sd[2], sd[3], dd[0], dd[1], dd[2], dd[3]);
            }
        
            ipcount++;
        
        }

        if( buffer[12] == 0x08 && buffer[13] == 0x06)
            rarpcount++;

        if( buffer[12] == 0x08 && buffer[13] == 0x35)
            rarpcount++;

        /*
         *  接收发往本机的all数据帧;
         *  还可以接收从本机发出的所有数据帧;
         *  
         *  打开混杂协议: 还可接收发往目的mac为非本地硬件地址的数据帧;;
         *
         */
        if( buffer[12] == 0x00 && buffer[13] == 0x03)
            allcount++;



        printf("ip = %d, rap = %d, rarp = %d, all = %d\n", ipcount, rarpcount, rarpcount, allcount);


		//获取数据类型;;;
		
		

		//接收到的数据帧头6字节是目的MAC地址，紧接着6字节是源MAC地址。
	}
	return 0;
}
