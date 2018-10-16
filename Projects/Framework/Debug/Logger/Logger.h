#pragma once
#include "Patterns/Singleton.h"

enum ELogLevel
{
	INFO,
	WARNING,
	ERR0R,
	FATAL
};

class Logger : public Singleton<Logger>
{
public:
	void Logf(const ELogLevel logLevel, const char* format, ...);
	void LogInfo(const std::string& msg) { Logf(ELogLevel::INFO, "[INFO] %s", msg.c_str()); };
	void LogWarning(const std::string& msg) { Logf(ELogLevel::WARNING, "[WARNING] %s", msg.c_str()); };
	void LogError(const std::string& msg) { Logf(ELogLevel::ERR0R, "[ERR0R] %s", msg.c_str()); };
	void LogFatal(const std::string& msg) { Logf(ELogLevel::FATAL, "[FATAL] %s", msg.c_str()); };
private:
	void ChangeConsoleColor(const ELogLevel logLevel);
	void ResetConsoleColor();
};