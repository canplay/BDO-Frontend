#pragma once

class __declspec(dllexport) LogManager
{
public:
	static LogManager &GetInstance();

public:
	LogManager();
	virtual ~LogManager();

	void Init(const char* sName);

	void Log(const char* sName, int level, const char* sFormat, ...);

public:
	enum { LOGINFO, LOGDEBUG, LOGWARNING, LOGERROR };
};
