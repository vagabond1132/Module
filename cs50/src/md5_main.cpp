#include "../include/md5.h"
#include "../include/des.h"
#include "../include/log_file.h"


int main()
{

    std::string key="asddgdasdasdaeqwrewdsfdguyuyt";
    std::string str = "ligang";
    

    std::cout<< "源码: " << key+str << std::endl;

    MD5 md5(key+str);
    std::string  sign_md_str = md5.md5();

    
    log_mem( "this is log_mem call is %s function:file=%s:line=%d\n", "main", __FILE__, __LINE__);


    for(std::string::iterator iter=sign_md_str.begin(); iter!=sign_md_str.end(); ++iter)
       printf("%c" ,std::toupper(*iter));

    std::cout << "sign_md_str: " << sign_md_str << std::endl;    
    
        


    if(sign_md_str.compare("ligang") == 0)
    {
        std::cout << "this is ok" <<std::endl;
    }
    
    return 0;
}

