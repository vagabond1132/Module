
  
#ifndef _AES_HH  
#define _AES_HH  
  
  
#ifdef __cplusplus  
extern "C" {  
#endif  
  
int aes_encrypt(unsigned char* in, unsigned char** out, char *key ,int inlen, int *outlen, int type);
  
#ifdef __cplusplus  
}  
#endif  
  
#endif 
