#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(){

	char  mm[1024];
    unsigned long len = 0;
    std::string regjson = "{\"client_type\": 3,\"mac_address\":\"00:ff:69:13:a6:03\",\"message_type\":\"register_client\"}";

    len = htons(regjson.length()); // 转化网络字节序;  ---- 网络字节序 为大端;

    memset(mm, 0 , 1024);
	memcpy(mm, &len, 4);
	printf("%s\n", mm);
	memcpy(mm + 4, regjson.c_str(), regjson.length());
	printf("2  regapp : send_str: %s\n", mm);



	/*
		// 接收并解析;
		int msglen = 0;
		char *mm = (char *)malloc(BUFSIZE);
		
		memset(mm, 0, BUFSIZE);
		len = recv(client_sockfd, recv_buf, 4, 0);
		memcpy(&msglen, recv_buf, 4);
		while (msglen)
		{
			len = recv(client_sockfd, recv_buf, msglen, 0);
			msglen = msglen - strlen(recv_buf);
		}
			//UnpackJson();
		

	*/



    
    return 0;

}
