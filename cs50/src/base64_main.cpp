#include "../include/log_file.h"
#include "../include/base64.h"
#include "../include/aes.h"
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    string appid = "000001";
    string data = "hello,world";
    string key="1234567890";
    unsigned char * out = NULL;
    int outlen = 0;
    // time =>  int to std::string;
    std::stringstream ss;
    std::string timestamp;
    int second = time(NULL);
    ss << second;
    ss >> timestamp; // 


    aes_encrypt((unsigned char *)data.c_str(), &out, (char*)key.c_str(), data.length(), &outlen, 0);//  0 加密  1解密;

    //char *base64 = base64Encode((char const *)out, outlen); //
    char *base64 = base64EncodeUrlSafe((char const *)out, outlen);//url safe base 64 

    std::cout << "base64: " << base64  << std::endl;
    
    return 0;

}

