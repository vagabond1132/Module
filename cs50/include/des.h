#ifndef DES_H
#define DES_H
#include<string.h>
#include<stdio.h>
//using namespace std;
#define TWO_3DES  2       /**< 双倍3des */
#define THREE_3DES  3     /**< 3倍3des */
#define ENCRYPT  1        /**< 加密 */
#define DECRYPT  0        /**< 解密 */

class Des
{
    public:
        Des();
        virtual ~Des();
        int THreeDes( const unsigned char* srcData, const unsigned char* key,unsigned char* dstData,short IsEn ,short type);/**< 3DES加解密函数 */
        void DesEncrypt(short IsEn,const unsigned char* srcDatat, unsigned char* keyt,unsigned char* dstDatat);/**< DES加解密函数 */
    private:

        void DoFirstChange(unsigned char *s);
        void DoKey(unsigned char *s,unsigned char *C,unsigned char *D);
        void DoLeft(unsigned char *s, short n);
        void DoRight(unsigned char *s, short n);
        void SetKey(short encrypt,unsigned char *C,unsigned char *D,short n,unsigned char *K);
        void E_expand(unsigned char * s, unsigned char * r);
        void P_chang(unsigned char * s);
        unsigned char Find_S(unsigned char * s, short ns);
        void f_fun(unsigned char *R,unsigned char *K,unsigned char *m);

};
#endif // THREEDES_H

