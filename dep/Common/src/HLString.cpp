#include "HLString.h"

#define _WINSOCKAPI_
#include <Windows.h>
#include <sstream>
#include <iomanip>

namespace HL
{
	std::string String::WideToMulit(const wchar_t* strWide)
	{
		int nLen = WideCharToMultiByte(CP_ACP, 0, strWide, -1, nullptr, 0, nullptr, nullptr);

		if (nLen <= 0) return std::string("");

		auto pszDst = new char[nLen];
		if (pszDst == nullptr) return std::string("");

		WideCharToMultiByte(CP_ACP, 0, strWide, -1, pszDst, nLen, nullptr, nullptr);
		pszDst[nLen - 1] = 0;

		std::string strTemp(pszDst);
		delete[] pszDst;
		pszDst = nullptr;
		return strTemp;
	}

	std::wstring String::MulitToWide(const char* strMulite)
	{
		int nSize = MultiByteToWideChar(CP_ACP, 0, strMulite, strlen(strMulite), 0, 0);
		if (nSize <= 0) return L"";

		auto pwszDst = new wchar_t[nSize + 1];
		if (pwszDst == nullptr) return L"";

		MultiByteToWideChar(CP_ACP, 0, strMulite, strlen(strMulite), pwszDst, nSize);
		pwszDst[nSize] = 0;

		if (pwszDst[0] == 0xFEFF) // skip Oxfeff  
			for (int i = 0; i < nSize; i++)
				pwszDst[i] = pwszDst[i + 1];

		std::wstring wcharString(pwszDst);
		delete[] pwszDst;
		pwszDst = nullptr;
		return wcharString;
	}

	std::string String::UTF8ToGBK(const std::string& strUTF8)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, nullptr, 0);
		auto str1 = new wchar_t[len + 1];
		memset(str1, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, str1, len);

		len = WideCharToMultiByte(CP_ACP, 0, str1, -1, nullptr, 0, nullptr, nullptr);
		auto str2 = new char[len + 1];
		memset(str2, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, str1, -1, str2, len, nullptr, nullptr);

		std::string strTemp(str2);
		delete[] str2;
		str2 = nullptr;
		delete[] str1;
		str1 = nullptr;
		return strTemp;
	}

	std::string String::GBKToUTF8(const std::string& strGBK)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, nullptr, 0);
		auto str1 = new wchar_t[len + 1];
		memset(str1, 0, len * 2 + 2);
		MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, len);

		len = WideCharToMultiByte(CP_UTF8, 0, str1, -1, nullptr, 0, nullptr, nullptr);
		auto str2 = new char[len + 1];
		memset(str2, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, len, nullptr, nullptr);

		std::string strTemp(str2);
		delete[] str1;
		str1 = nullptr;
		delete[] str2;
		str2 = nullptr;
		return strTemp;
	}

	std::string String::StringToHexString(const std::string & strSrc)
	{
		std::string ret;
		static const char *hex = "0123456789ABCDEF";
		for (int i = 0; i != strSrc.size(); ++i)
		{
			ret.push_back(hex[(strSrc[i] >> 4) & 0xf]);
			ret.push_back(hex[strSrc[i] & 0xf]);
		}
		return ret;
	}

	unsigned char ch2hex(char ch)
	{
		static const char *hex = "0123456789ABCDEF";
		for (unsigned char i = 0; i != 16; ++i)
			if (ch == hex[i])
				return i;
		return 0;
	}

	std::string String::HexStringToString(const std::string & strSrc)
	{
		int i = 0;
		int cnt = 0;
		const char * tmp = strSrc.c_str();
		unsigned char * d = new unsigned char[strSrc.size()];
		memset(d, 0, strSrc.length());

		while (*tmp)
		{
			if (i & 1)
			{
				d[cnt++] |= ch2hex(*tmp);
			}
			else
			{
				d[cnt] = ch2hex(*tmp) << 4;
			}
			tmp++;
			i++;
		}

		std::string ret = (char*)d;
		delete d;
		d = 0;
		return ret;
	}

	void String::Replace(std::string & strBase, std::string strSrc, std::string strDes)
	{
		std::string::size_type pos = 0;
		std::string::size_type srcLen = strSrc.size();
		std::string::size_type desLen = strDes.size();
		pos = strBase.find(strSrc, pos);
		while ((pos != std::string::npos))
		{
			strBase.replace(pos, srcLen, strDes);
			pos = strBase.find(strSrc, (pos + desLen));
		}
	}
}
