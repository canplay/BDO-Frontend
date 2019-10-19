#pragma once

#include <map>

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

class __declspec(dllexport) JSONManager
{
public:
	static JSONManager &GetInstance();

public:
	JSONManager();
	virtual ~JSONManager();

	int Read(const char* szFileName);
	void Write(const char* szFileName);
	void Close(int nID = -1);

	int Create(const char* szJson = "");
	std::string Get(int nID = -1);

	void Set(int nID, const char* key, int value);
	int GetInteger(int nID, const char* key);
	void Set(int nID, const char* key, bool value);
	bool GetBoolean(int nID, const char* key);
	void Set(int nID, const char* key, double value);
	double GetDouble(int nID, const char* key);
	void Set(int nID, const char* key, const char* value);
	const char* GetString(int nID, const char* key);
	void Set(int nID, const char* key, rapidjson::Value value);

	int Count(int nID, const char* key);

	rapidjson::Value GetValue(int nID, const char* key);
	rapidjson::Document* GetDocment(int nID) { return (*m_pDocuments)[nID]; }

	bool has(int nID, const char* key);

private:
	int m_nID;
	std::map<int, rapidjson::Document*>* m_pDocuments;
};
