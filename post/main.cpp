#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "HttpClient.h"

using namespace std;

#define DETECT_URL              "https://api-cn.faceplusplus.com/facepp/v3/detect"
#define COMPARE_URL             "https://api-cn.faceplusplus.com/facepp/v3/compare"
#define FACESET_CREATE_URL      "https://api-cn.faceplusplus.com/facepp/v3/faceset/create"
#define FACESET_ADDFACE_URL     "https://api-cn.faceplusplus.com/facepp/v3/faceset/addface"
#define FACESET_REMOVEFACE_URL  "https://api-cn.faceplusplus.com/facepp/v3/faceset/removeface"
#define FACESET_UPDATE_URL      "https://api-cn.faceplusplus.com/facepp/v3/faceset/update"
#define FACESET_DELETE_URL      "https://api-cn.faceplusplus.com/facepp/v3/faceset/delete"


char *message[] = 
    {
    /*   0   */  "No Error",
   
    
    
    };



int main(void)
{

    string poststr = "api_key=Dec6OnZZQ3nKa7ZU0UDwcNe_xu1aeotd&api_secret=nyhYWQ6potydeDJn9rL3gtFWoijU6wV4&image_file1=@maomao.jpeg&face_token2=c0531232e5ad4a47785deff3a41b81a4";

    CHttpClient chttpclient;

    string strResponse;

    int err = chttpclient.Post(FACESET_CREATE_URL , poststr, "", strResponse); 

    if( 0 != err)
        fprintf(stderr, "httpclient post error, errno: %d , err message: %s", err, message[err]);    

    printf("strResponse = %s \n", strResponse.c_str());

    return 0;
}
