    
    /*  声明错误宏定义   */
    #define     ERR_NO_ERROR    0  /* No error                 */
    #define     ERR_OPEN_FILE   1  /* Open file error          */
    #define     ERR_SEND_MESG   2  /* sending a message error  */
    #define     ERR_BAD_ARGS    3  /* Bad arguments            */
    #define     ERR_MEM_NONE    4  /* Memeroy is not enough    */
    #define     ERR_SERV_DOWN   5  /* Service down try later   */
    #define     ERR_UNKNOW_INFO 6  /* Unknow information       */
    #define     ERR_SOCKET_ERR  7  /* Socket operation failed  */
    #define     ERR_PERMISSION  8  /* Permission denied        */
    #define     ERR_BAD_FORMAT  9  /* Bad configuration file   */
    #define     ERR_TIME_OUT   10  /* Communication time out   */
    
    /* 声明出错信息 */
    char* errmsg[] = {
        /* 0 */       "No error",                
        /* 1 */       "Open file error",        
        ...
    };
                              
    /* 声明错误代码全局变量 */
    unsigned int errno = 0;
    
    /* 打印出错信息函数 */
    void 
    perror( char* info)
    {
        if ( info ){
            printf("%s: %s/n", info, errmsg[errno] );
            return;
        }
        
        printf("Error: %s/n", errmsg[errno] );
    }



    int 
    main(void)
    {
        /*  指定errno */
        errno = ERR_SOCKET_ERR;

        perror("main");

    }
