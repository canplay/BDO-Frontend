#pragma once

#include <string>

namespace HL
{
	class String
	{
	public:
		static std::string WideToMulit(const wchar_t* strWide);
		static std::wstring MulitToWide(const char* strMulite);

		static std::string UTF8ToGBK(const std::string& strUTF8);
		static std::string GBKToUTF8(const std::string& strGBK);

		static std::string StringToHexString(const std::string & strSrc);
		static std::string HexStringToString(const std::string & strSrc);

		static void Replace(std::string &strBase, std::string strSrc, std::string strDes);
	};
}
