#include "JsonManager.h"
#include <LogManager.h>
#include <HLWindows.h>
#include <functional>
#include <thread>
#include <stringbuffer.h>
#include <writer.h>

#define json_check_is_bool(value, strKey) (value.HasMember(strKey) && value[strKey].IsBool())  
#define json_check_is_string(value, strKey) (value.HasMember(strKey) && value[strKey].IsString())  
#define json_check_is_int32(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt())  
#define json_check_is_uint32(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint())  
#define json_check_is_int64(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt64())  
#define json_check_is_uint64(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint64())  
#define json_check_is_double(value, strKey) (value.HasMember(strKey) && value[strKey].IsDouble())  
#define json_check_is_number(value, strKey) (value.HasMember(strKey) && value[strKey].IsNumber())  
#define json_check_is_array(value, strKey) (value.HasMember(strKey) && value[strKey].IsArray())  
#define json_check_is_object(value, strKey) (value.HasMember(strKey) && value[strKey].IsObject())  

JSONManager &JSONManager::GetInstance()
{
	static JSONManager instance;
	return instance;
}

JSONManager::JSONManager()
{
	LogManager::GetInstance().Init("JSONManager");
	LogManager::GetInstance().Log("JSONManager", LogManager::LOGINFO, "初始化JSON管理器");
	m_pDocuments = new std::map<int, rapidjson::Document *>;
	m_nID = 0;
}

JSONManager::~JSONManager()
{
	Close();
	delete m_pDocuments;
	LogManager::GetInstance().Log("JSONManager", LogManager::LOGINFO, "关闭JSON管理器");
}

int JSONManager::Read(const char* szFileName)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, szFileName, "rb, ccs=UTF-8");
	if (!pFile)
	{
		LogManager::GetInstance().Log("JSONManager", LogManager::LOGINFO, "打开文件 %s 失败！Code = %d", szFileName, errno);
		return -1;
	}

	fseek(pFile, 0, SEEK_END);
	long size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
;
	auto buf = new char[size + 1];
	memset(buf, 0, size + 1);
	fread((void *)buf, size, 1, pFile);
	fclose(pFile);
	pFile = nullptr;

	rapidjson::Document * pDocument = new rapidjson::Document();
	pDocument->Parse<rapidjson::kParseDefaultFlags>(buf);
	if (pDocument->HasParseError())
	{
		LogManager::GetInstance().Log("JSONManager", LogManager::LOGINFO, "验证JSON失败！");
		return -1;
	}
	int nID = HL::Windows::Random(500);
	m_pDocuments->insert(std::make_pair(nID, pDocument));

	delete[] buf;
	buf = nullptr;

	return nID;
}

void JSONManager::Write(const char* szFileName)
{
	rapidjson::Document * pDocument = new rapidjson::Document();
	pDocument->SetObject();

	rapidjson::StringBuffer buffer;
	buffer.Clear();

	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	pDocument->Accept(writer);

	std::string s(buffer.GetString(), buffer.GetSize());
	printf(s.c_str());

	std::function<void(std::string, const char*)> saveHandler = [](std::string str, const char* szFile)
	{
		FILE *pFile = nullptr;
		fopen_s(&pFile, szFile, "w, ccs=UTF-8");
		if (!pFile)
		{
			LogManager::GetInstance().Log("JSONManager", LogManager::LOGINFO, "打开文件 %s 失败！Code = %d", szFile, errno);
			return;
		}
		fwrite(str.c_str(), str.size(), 1, pFile);
		fclose(pFile);
	};

	std::thread save(saveHandler, std::string(buffer.GetString(), buffer.GetSize()), szFileName);
	save.join();

	pDocument->Clear();
	rapidjson::MemoryPoolAllocator<>::Free(pDocument);
}

int JSONManager::Create(const char* szJson)
{
	rapidjson::Document* pDocument = new rapidjson::Document();
	if (strcmp(szJson, "") == 0) pDocument->SetObject();
	else
	{
		pDocument->Parse<rapidjson::kParseDefaultFlags>(szJson);
		if (pDocument->HasParseError())
		{
			LogManager::GetInstance().Log("JSONManager", LogManager::LOGINFO, "验证JSON失败！");
			return -1;
		}
	}

	m_nID += 1;
	if (pDocument)
	{
		m_pDocuments->insert(std::make_pair(m_nID, pDocument));
		return m_nID;
	}

	Close(m_nID);
	LogManager::GetInstance().Log("JSONManager", LogManager::LOGINFO, "读取JSON失败！");
	return -1;
}

std::string JSONManager::Get(int nID)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	(*m_pDocuments)[nID]->Accept(writer);

	std::string strOut(buffer.GetString(), buffer.GetLength());
	return strOut;
}

void JSONManager::Close(int nID)
{
	if (nID != -1)
	{
		auto it = m_pDocuments->find(nID);
		if (it != m_pDocuments->end())
		{
			rapidjson::MemoryPoolAllocator<>::Free((*it).second);
			m_pDocuments->erase(it);
		}
	}
	else
	{
		for (auto it = m_pDocuments->begin(); it != m_pDocuments->end();)
		{
			rapidjson::MemoryPoolAllocator<>::Free((*it).second);
			m_pDocuments->erase(it++);
		}
	}
}

void JSONManager::Set(int nID, const char* key, int value)
{
	rapidjson::Document::AllocatorType& allocator = (*m_pDocuments)[nID]->GetAllocator();
	if (json_check_is_int32((*(*m_pDocuments)[nID]), key)) (*(*m_pDocuments)[nID])[key].SetInt(value);
	else (*m_pDocuments)[nID]->AddMember(rapidjson::Value().SetString(key, allocator), value, allocator);
}

int JSONManager::GetInteger(int nID, const char* key)
{
	if (json_check_is_int32((*(*m_pDocuments)[nID]), key)) return (*(*m_pDocuments)[nID])[key].GetInt();
	return 0;
}

void JSONManager::Set(int nID, const char* key, bool value)
{
	rapidjson::Document::AllocatorType& allocator = (*m_pDocuments)[nID]->GetAllocator();
	if (json_check_is_bool((*(*m_pDocuments)[nID]), key)) (*(*m_pDocuments)[nID])[key].SetBool(value);
	else (*m_pDocuments)[nID]->AddMember(rapidjson::Value().SetString(key, allocator), value, allocator);
}

bool JSONManager::GetBoolean(int nID, const char* key)
{
	if (json_check_is_bool((*(*m_pDocuments)[nID]), key)) return (*(*m_pDocuments)[nID])[key].GetBool();
	return false;
}

void JSONManager::Set(int nID, const char* key, double value)
{
	rapidjson::Document::AllocatorType& allocator = (*m_pDocuments)[nID]->GetAllocator();
	if (json_check_is_double((*(*m_pDocuments)[nID]), key)) (*(*m_pDocuments)[nID])[key].SetDouble(value);
	else (*m_pDocuments)[nID]->AddMember(rapidjson::Value().SetString(key, allocator), value, allocator);
}

double JSONManager::GetDouble(int nID, const char* key)
{
	if (json_check_is_double((*(*m_pDocuments)[nID]), key)) return (*(*m_pDocuments)[nID])[key].GetDouble();
	return 0;
}

void JSONManager::Set(int nID, const char* key, const char* value)
{
	rapidjson::Document::AllocatorType& allocator = (*m_pDocuments)[nID]->GetAllocator();
	if (json_check_is_string((*(*m_pDocuments)[nID]), key)) (*(*m_pDocuments)[nID])[key].SetString(value, allocator);
	else (*m_pDocuments)[nID]->AddMember(rapidjson::Value().SetString(key, allocator), rapidjson::Value().SetString(value, allocator), allocator);
}

const char* JSONManager::GetString(int nID, const char* key)
{
	if (json_check_is_string((*(*m_pDocuments)[nID]), key)) return (*(*m_pDocuments)[nID])[key].GetString();
	return "";
}

void JSONManager::Set(int nID, const char* key, rapidjson::Value value)
{
	rapidjson::Document::AllocatorType& allocator = (*m_pDocuments)[nID]->GetAllocator();
	rapidjson::Value& var = *(*m_pDocuments)[nID];
	if ((*m_pDocuments)[nID]->HasMember(key)) var[key] = value;
	else (*m_pDocuments)[nID]->AddMember(rapidjson::Value().SetString(key, allocator), value, allocator);
}

int JSONManager::Count(int nID, const char* key)
{
	int n = 0;
	if (json_check_is_object((*(*m_pDocuments)[nID]), key))
	{
		for (auto& value : (*(*m_pDocuments)[nID])[key].GetObject())
		{
			n += 1;
		}
	}
	else if (json_check_is_array((*(*m_pDocuments)[nID]), key))
	{
		for (auto& value : (*(*m_pDocuments)[nID])[key].GetArray())
		{
			n += 1;
		}
	}
	return n;
}

rapidjson::Value JSONManager::GetValue(int nID, const char* key)
{
	rapidjson::Value value;

	if ((*m_pDocuments)[nID]->HasMember(key))
	{
		value = (*(*m_pDocuments)[nID])[key];
		return value;
	}

	value.SetNull();
	return value;
}

bool JSONManager::has(int nID, const char* key)
{
	if ((*m_pDocuments)[nID]->HasMember(key)) return true;
	return false;
}
