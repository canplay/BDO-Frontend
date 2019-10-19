#include "HLInternet.h"

#include <curl.h>

namespace HL
{
	Internet &Internet::GetInstance()
	{
		static Internet instance;
		return instance;
	}

	size_t ResponseCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
	{
		std::string* str = dynamic_cast<std::string*>((std::string *)pParam);
		if (str == 0 || pBuffer == 0) return -1;

		char* pData = (char*)pBuffer;
		str->append(pData, nSize * nMemByte);
		return nMemByte;
	}

	int Internet::Post(const char *strUrl, const char *strPost)
	{
		m_sResponse = "";

		CURLcode res;
		CURL* curl = curl_easy_init();
		if (NULL == curl) return CURLE_FAILED_INIT;

		struct curl_slist *head = NULL;
		head = curl_slist_append(head, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);
		curl_easy_setopt(curl, CURLOPT_URL, strUrl);
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&m_sResponse);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return res;
	}

	int Internet::Get(const char *strUrl)
	{
		m_sResponse = "";

		CURLcode res;
		CURL* curl = curl_easy_init();
		if (NULL == curl) return CURLE_FAILED_INIT;

		struct curl_slist *head = NULL;
		head = curl_slist_append(head, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);
		curl_easy_setopt(curl, CURLOPT_URL, strUrl);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&m_sResponse);
		/**
		* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
		* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
		*/
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return res;
	}

	int Internet::Posts(const char *strUrl, const char *strPost, const char * pCaPath)
	{
		m_sResponse = "";

		CURLcode res;
		CURL* curl = curl_easy_init();
		if (NULL == curl) return CURLE_FAILED_INIT;

		struct curl_slist *head = NULL;
		head = curl_slist_append(head, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);
		curl_easy_setopt(curl, CURLOPT_URL, strUrl);
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&m_sResponse);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		if (NULL == pCaPath)
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		}
		else
		{
			//缺省情况就是PEM，所以无需设置，另外支持DER  
			//curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
			curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
		}
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return res;
	}

	int Internet::Gets(const char *strUrl, const char *pCaPath)
	{
		m_sResponse = "";

		CURLcode res;
		CURL* curl = curl_easy_init();
		if (NULL == curl) return CURLE_FAILED_INIT;

		struct curl_slist *head = NULL;
		head = curl_slist_append(head, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, head);
		curl_easy_setopt(curl, CURLOPT_URL, strUrl);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&m_sResponse);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

		if (NULL == pCaPath)
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		}
		else
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
			curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
		}

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return res;
	}

	size_t DownloadCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
	{
		int written = fwrite(pBuffer, nSize, nMemByte, (FILE*)pParam);
		return written;
	}

	int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
	{
		if (dltotal > -0.1 && dltotal < 0.1) return 0;
		int nPos = (int)((dlnow / dltotal) * 100);
		HL::Internet::GetInstance().DownloadProgress(nPos);
		return 0;
	}

	int Internet::DownloadFile(const char *strUrl, const char *strFile)
	{
		CURL* pCurl = 0;
		FILE* pFile = 0;
		fopen_s(&pFile, strFile, "wb");
		if (!pFile) return 0;

		pCurl = curl_easy_init();
		curl_easy_setopt(pCurl, CURLOPT_URL, strUrl);
		curl_easy_setopt(pCurl, CURLOPT_MAXREDIRS, 5);
		curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, pFile);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, DownloadCallback);
		curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
		curl_easy_perform(pCurl);
		fclose(pFile);
		curl_easy_cleanup(pCurl);

		return 1;
	}

	std::string Internet::Format(const char *str)
	{
		CURL* pCurl = 0;
		pCurl = curl_easy_init();

		curl_easy_escape(pCurl, str, strlen(str));
		curl_easy_cleanup(pCurl);

		std::string sTemp(str);
		return sTemp;
	}
}
