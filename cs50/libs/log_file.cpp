
#include "../include/log_file.h"
#include <stdarg.h>
#include <stdio.h>


extern void log_mem(char *fmt, ...)
{
    va_list ap;// ap 将依次引用各个参数
    char *p, *val;
    int ival;
    double dval;

    va_start(ap, fmt); /// 初始化形参列表 // 将 ap 初始化为指向第一个无名参数的指针
//    printf("fmt, %s\n", fmt); // 参数列表

//    printf("this is arg: \n");//参数；
    for(p = fmt; *p ; p++)
    {
        if( '%' != *p)
        {
            putchar(*p);
            continue;
        }
        switch(*++p)
        {
            case 'd':
                ival = va_arg(ap, int);// 返回一个参数 ， 并将ap指向下一个参数
                printf("%d ", ival);
                break;
            case 'f':
                dval = va_arg(ap, double);
                printf("%f", dval);
                break;
            case 's':
               // printf(" ");
                for(val = va_arg(ap, char*); *val; val++)
                    putchar(*val);
                break;
        
            default:
                putchar(*p);
                break;
        }
    }
    va_end(ap);
}



extern void  log_file(char *fmt, ...)
{


}

//extern void log(char *fmt, ...)
//{
//
//
//
//}




//int main() // 测试代码
//{
//    log_mem("I`m name is %s, age is %d, mem is %f", "ligang", 24, 12.1223);
//    
//
//}
