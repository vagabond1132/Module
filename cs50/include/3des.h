#ifndef _3DES__H_  
#define _3DES__H_  
  
#ifdef __cplusplus  
extern "C"  
{  
#endif  
  
#define ENCRYPT 0  
#define DECRYPT 1  
  
#define ECB 0  
#define CBC 1  
  
#define KEY_LEN_8 8  
#define KEY_LEN_16 16  
#define KEY_LEN_24 24  
  
#define PAD_ISO_1   0  
#define PAD_ISO_2   1  
#define PAD_PKCS_7  2  
  
    typedef char (*PSubKey)[16][48];  
  
    /*���ܺ�,�������ַ�ȥ��*/  
    void RunRsm(char* Text);  
  
    /*��48λ��������Կת����24���ַ�����Կ*/  
    int CovertKey(char* iKey, char *oKey);  
  
    /******************************************************************* 
      �� �� �� ��:  RunPad 
      �� �� �� ����  ����Э��Լ���ǰ�����ݽ������ 
      �� �� ˵ ����  bType   :���ͣ�PAD���� 
                    In      :���ݴ�ָ�� 
                    Out     :��������ָ�� 
                    in_len  :���ݵĳ��� 
                    padlen  :(in,out)���buffer�ĳ��ȣ�����ĳ��� 
 
      ����ֵ ˵����   char    :�Ƿ����ɹ� 
    *******************************************************************/  
    int RunPad(int nType,const char* In,unsigned in_len,char* Out,int* padlen);  
  
    /******************************************************************* 
    �� �� �� ��:    Run1Des 
    �� �� �� ����    ִ�е�DES�㷨���ı��ӽ��� 
    �� �� ˵ ����    bType   :���ͣ�����ENCRYPT������DECRYPT 
                    bMode   :ģʽ��ECB,CBC 
                    In      :�����ܴ�ָ�� 
                    in_len  :�����ܴ��ĳ��ȣ�ͬʱOut�Ļ�������СӦ���ڻ��ߵ���in_len 
                    Key     :��Կ(��Ϊ8λ,16λ,24λ)֧��3��Կ 
                    key_len :��Կ���ȣ����24λ���ֽ����Զ��ü� 
                    Out     :�������ָ�� 
                    out_len :��������С 
                    cvecstr :8�ֽ��漴�ַ��� 
 
    ����ֵ ˵���� int     :�Ƿ���ܳɹ���1���ɹ���0��ʧ�� 
    *******************************************************************/  
    int Run1Des(int bType, int bMode, const char *In, unsigned int in_len, const char *Key, unsigned int key_len, char* Out, unsigned int out_len, const char cvecstr[8]);  
  
    /******************************************************************* 
      �� �� �� ��:  Run3Des 
      �� �� �� ����  ִ��3DES�㷨���ı��ӽ��� 
      �� �� ˵ ����  bType   :���ͣ�����ENCRYPT������DECRYPT 
                    bMode   :ģʽ��ECB,CBC 
                    In      :�����ܴ�ָ�� 
                    in_len  :�����ܴ��ĳ��ȣ�ͬʱOut�Ļ�������СӦ���ڻ��ߵ���in_len 
                    Key     :��Կ(��Ϊ8λ,16λ,24λ)֧��3��Կ 
                    key_len :��Կ���ȣ����24λ���ֽ����Զ��ü� 
                    Out     :�������ָ�� 
                    out_len :��������С 
                    cvecstr :8�ֽ��漴�ַ��� 
 
      ����ֵ ˵����   int     :�Ƿ���ܳɹ���1���ɹ���0��ʧ�� 
 
      ��       ��:    huangjf 
      �� �� �� �ڣ�  2009.6.17 
 
      3DES(����) = DES(key1, ����) DES(key2, ����)  DES(key3, ����) 
      3DES(����) = DES(key3, ����) DES(key2, ����)  DES(key1, ����) 
      ÿ��KEYΪ64λ���ܹ�������192λ��KEY, ��һ�㶼ֻʹ��128λ��key 
      ���ֻ��128λ��Կ����key3 = key1 
 
    *******************************************************************/  
    int Run3Des(int bType, int bMode, const char *In, unsigned int in_len, const char *Key, unsigned int key_len, char* Out, unsigned int* out_len, const char cvecstr[8]);  
  
    /******************************************************************* 
    �� �� �� ��:    Crypt3Des 
    �� �� �� ����    ʵ��3DES�ļӽ��� 
    �� �� ˵ ����    type    :���ͣ�����ENCRYPT������DECRYPT 
                    in      :�����ܴ�ָ����ߴ����ܵ������ַ���ָ�� 
                    Out     :���ܺ��������߽��ܺ������ 
                    Key     :��Կ(48λ��ASCII�ַ���) 
 
    ����ֵ ˵���� 
                    0:�ɹ� 
                    -1:�Ƿ�����Կ���� 
                    -2:��Կ���з�16�����ַ� 
                    -3:�������ʧ�� 
                    -4:�ӽ���ʧ�� 
                    -5:�Ƿ��Ĳ������� 
    *******************************************************************/  
    int Crypt3Des(int type,char* in,char* key,char* out);  
  
    char *Base64Encode(char *src, int srclen);  
    char *Base64Decode(char *src);  
    unsigned char GetByte(char *s);  
  
#ifdef __cplusplus  
}  
#endif  
  
#endif  
