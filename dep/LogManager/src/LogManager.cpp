#include "LogManager.h"
#include "easylogging++.h"

LogManager &LogManager::GetInstance()
{
	static LogManager instance;
	return instance;
}

LogManager::LogManager()
{
	Init("LogManager");
	Log("LogManager", LogManager::LOGINFO, "初始化日志管理器");
}

LogManager::~LogManager()
{
	Log("LogManager", LogManager::LOGINFO, "关闭日志管理器");
}

void LogManager::Init(const char* sName)
{
	el::Loggers::getLogger(sName);

	char logFileName[MAX_PATH];
	sprintf_s(logFileName, "Logs\\Log_%s.log", sName);

	el::Configurations conf;
	conf.setToDefault();

	conf.set(el::Level::Global, el::ConfigurationType::Filename, logFileName);
	conf.set(el::Level::Global, el::ConfigurationType::Format, "[%level| %datetime] | %msg");
	conf.set(el::Level::Global, el::ConfigurationType::Enabled, "true");
	conf.set(el::Level::Global, el::ConfigurationType::ToFile, "true");
	conf.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, "true");
	conf.set(el::Level::Global, el::ConfigurationType::MillisecondsWidth, "3");
	conf.set(el::Level::Global, el::ConfigurationType::PerformanceTracking, "false");
	conf.set(el::Level::Global, el::ConfigurationType::MaxLogFileSize, "2097152");
	conf.set(el::Level::Global, el::ConfigurationType::LogFlushThreshold, "0");

	conf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
	conf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
	conf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "false");
	conf.set(el::Level::Verbose, el::ConfigurationType::Enabled, "false");

	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
	el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
	el::Loggers::setLoggingLevel(el::Level::Global);
	el::Loggers::reconfigureLogger(sName, conf);
}

void LogManager::Log(const char* sName, int level, const char* sFormat, ...)
{
	if (!el::Loggers::hasLogger(sName)) return;

	va_list args;
	va_start(args, sFormat);

	int len = _vscprintf(sFormat, args) + 1;
	auto str = (char*)malloc(len * sizeof(char));
	vsprintf_s(str, len, sFormat, args);

	va_end(args);

	switch (level)
	{
	case LOGDEBUG:
#ifdef _DEBUG
		DCLOG(DEBUG, sName) << str;
		OutputDebugStringA("LOG: ");
		OutputDebugStringA(str);
		OutputDebugStringA("\n");
#else
		CLOG(DEBUG, sName) << str;
#endif // DEBUG
		break;
	case LOGWARNING:
#ifdef _DEBUG
		DCLOG(WARNING, sName) << str;
		OutputDebugStringA("LOG: ");
		OutputDebugStringA(str);
		OutputDebugStringA("\n");
#else
		CLOG(WARNING, sName) << str;
#endif // DEBUG
		break;
	case LOGERROR:
#ifdef _DEBUG
		DCLOG(ERROR, sName) << str;
		OutputDebugStringA("LOG: ");
		OutputDebugStringA(str);
		OutputDebugStringA("\n");
#else
		CLOG(ERROR, sName) << str;
#endif // DEBUG
		break;
	default:
#ifdef _DEBUG
		DCLOG(INFO, sName) << str;
		OutputDebugStringA("LOG: ");
		OutputDebugStringA(str);
		OutputDebugStringA("\n");
#else
		CLOG(INFO, sName) << str;
#endif // DEBUG
		break;
	}

	free(str);
}