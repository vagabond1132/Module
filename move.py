import time
import json
import struct
from socket import *
from threading import Thread


def receive2dic(tcp_sock):
    """接收 Bytes 流数据，返回 ‘字典’ :"""
    head_bytes = tcp_sock.recv(4)
    print("head_bytes:", head_bytes)

    #while 4 - head_bytes.__len__():
    #    add = tcp_sock.recv(4 - head_bytes.__len__())
    #    head_bytes += add
    #print("head_bytes += add :", head_bytes)

    head_int = struct.unpack('=L', head_bytes)

    buffer = []
    message_len = 0
    while head_int[0] - message_len:
        data = tcp_sock.recv(head_int[0] - message_len)
        buffer.append(data)
        message_len += len(data)

    str_json = (b''.join(buffer)).decode()
    return json.loads(str_json)


def pack2bytes(dic_ready):
    """将 JSON 命令打包成（可直接发送的） Bytes 流"""
    json_str = json.dumps(dic_ready)        # Python 数据格式 -> JSON（实际是str）
    json_bytes = json_str.encode()          # encode 字符串 为 bytes 格式
    # Head
    json_len = len(json_bytes)              # 计算 bytes 信息长度
    json_pack = struct.pack('=L', json_len) + json_bytes
    return json_pack
    # return json_bytes
    
f=open('log.txt','w') 

class RecvTread(Thread):

    def __init__(self, tcp_socket):
        Thread.__init__(self)
        self.s = tcp_socket

    def run(self):
        while True:
            msg = receive2dic(self.s)
            #print(msg['message_type'])
            print("msg:",msg,"len", len(msg))
            f.write(msg)
        

# Mac
local_mac = '00:8B:92:3E:56:33'
robot_mac = '00:07:32:38:E2:5A'        # 修改为实际机器人的 mac

# 命令
reg_client_dic = {
    "message_type": "register_client",
    "client_type": 3,
    "mac_address": local_mac
}

get_all_robot_info_dic = {
    "message_type": "get_all_robot_info",
}

move_dic = {
    "message_type": "move",
    "robot_mac_address": robot_mac,
    "vx": 0,
    "vy": 0,
    'vtheta': 0.2
}

# Socket TCP
s = socket(AF_INET, SOCK_STREAM)
s.connect(('192.168.1.107', 6789))


# 接收
RecvTread(s).start()


# Send
json_packed = pack2bytes(reg_client_dic)
s.send(json_packed)

json_packed = pack2bytes(get_all_robot_info_dic)
s.send(json_packed)



# 控制
json_packed = pack2bytes(move_dic)
print(json_packed)
while True:
    s.send(json_packed)
    time.sleep(0.2)

# Close
s.close()
f.close()
