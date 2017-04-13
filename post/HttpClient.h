//HttpClient.h源代码清单
#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_

#include <string>

/*
标题：从HTTP/HTTPS服务器中获取页面
测试环境：Windows 8.1
		Visual Studio 2013 SP1
		libcurl 7.36.0
说明：转自csdn某个页面，对源代码做了小修改，添加了Cookie的支持
参考资料：
《curl_eay_setopt手册》
http://www.helplib.net/s/linux.die/65_2740/man-3-curl-easy-setopt.shtml
《C++的cout高阶格式化操作》
http://www.cnblogs.com/devymex/archive/2010/09/06/1818754.html
*/

class CHttpClient
{
	public:
		CHttpClient(void);
		~CHttpClient(void);

	public:
		/**
		* @brief HTTP POST请求
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
		* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
		* @param strCookie 输入参数,Cookie文件名，例如 d:\temp\cookie.txt
		*                          如果为空，不启用Cookie.
		* @param strResponse 输出参数,返回的内容
		* @return 返回是否Post成功
		*/
		int Post(const std::string & strUrl, const std::string & strPost, 
			const std::string& strCookie, std::string & strResponse);

		/**
		* @brief HTTP GET请求
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
		* @param strResponse 输出参数,返回的内容
		* @return 返回是否Post成功
		*/
		int Get(const std::string & strUrl, std::string & strResponse);

		/**
		* @brief HTTPS POST请求,无证书版本
		* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
		* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
		* @param strCookie 输入参数,Cookie文件名，例如 d:\temp\cookie.txt
		*                          如果为空，不启用Cookie.
		* @param strResponse 输出参数,返回的内容
		* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
		* @return 返回是否Post成功
		*/
		int PostS(const std::string & strUrl, const std::string & strPost, const std::string& strCookie, 
			std::string & strResponse, const char * pCaPath = NULL);

		/**
		* @brief HTTPS GET请求,无证书版本
		* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
		* @param strResponse 输出参数,返回的内容
		* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
		* @return 返回是否Post成功
		*/
		int GetS(const std::string & strUrl, std::string & strResponse, const char * pCaPath = NULL);

	public:
		void SetDebug(bool bDebug);

	private:
		bool m_bDebug;

		bool PrintCookies(void* curl,std::string& strOut);
};

#endif

