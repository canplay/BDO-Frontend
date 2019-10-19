#pragma once

#include <string>

namespace HL
{
	class Internet
	{
	public:
		static Internet &GetInstance();

		/**
		* @brief HTTP POST请求
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
		* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
		* @param strResponse 输出参数,返回的内容
		* @return 返回是否Post成功
		*/
		int Post(const char *strUrl, const char *strPost);

		/**
		* @brief HTTP GET请求
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
		* @param strResponse 输出参数,返回的内容
		* @return 返回是否Get成功
		*/
		int Get(const char *strUrl);

		/**
		* @brief HTTPS POST请求,无证书版本
		* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
		* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
		* @param strResponse 输出参数,返回的内容
		* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
		* @return 返回是否Posts成功
		*/
		int Posts(const char *strUrl, const char *strPost, const char *pCaPath = NULL);

		/**
		* @brief HTTPS GET请求,无证书版本
		* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
		* @param strResponse 输出参数,返回的内容
		* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
		* @return 返回是否Gets成功
		*/
		int Gets(const char *strUrl, const char *pCaPath = NULL);

		/**
		* @brief HTTP 下载文件
		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
		* @return 返回是否下载成功
		*/
		int DownloadFile(const char *strUrl, const char *strFile);

		void (*DownloadProgress)(int);

		std::string GetResponse() { return m_sResponse; }

		std::string Format(const char *str);

	private:
		std::string m_sResponse;
	};
}
