

    组播;
        关键在于链接进来的客户端如何处理:
                 加入广播组;  



    linux 能发送与接收组播数据包的前提条件:
        1. 以下是某网卡部分信息 
            eth0      Link encap:Ethernet 
              UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
        
            可以发现其开启组播: MULTICAST ;
            即说明此网卡支持组播的转发 以及 接收 发送
        2. 查看防火墙是否对组播 支持; 防止防火墙的过滤

        3. 查看NetworkManager进程对linux网络服务的限制
        4. 查看sysctl中 rp_filter 限制是否允许组播;
前两者是决绝条件;
