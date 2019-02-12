#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

//#include <stdarg.h>


enum LOGWARN{
    SYSWARNING = 1,
    PRAWARNING = 2,
    WARNING  = 3,
};

#ifdef __cplusplus
extern "C" {
#endif
    // --- memory; // 内存显示 -- ；
    extern void log_mem(char *fmt, ...); //


    // --- file
    extern void log_file(char *fmt, ...);

    // --- ;
    //extern void log(char *fmt, ...);
#ifdef __cplusplus
}; // c code
#endif

#endif //log_file.h
