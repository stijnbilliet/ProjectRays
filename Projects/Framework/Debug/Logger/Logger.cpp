#include "FrameworkPCH.h"
#include "Logger.h"

void Logger::Logf(const ELogLevel logLevel, const char * format, ...)
{
	ChangeConsoleColor(logLevel);
	{
		va_list argptr;
		va_start(argptr, format);
		vprintf(format, argptr);
		va_end(argptr);
	}
	ResetConsoleColor();
}

void Logger::ChangeConsoleColor(const ELogLevel logLevel)
{
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (logLevel)
	{
	case ELogLevel::INFO:
		SetConsoleTextAttribute(hConsole, 15);
		break;
	case ELogLevel::WARNING:
		SetConsoleTextAttribute(hConsole, 14);
		break;
	case ELogLevel::ERR0R:
		SetConsoleTextAttribute(hConsole, 11);
		break;
	case ELogLevel::FATAL:
		SetConsoleTextAttribute(hConsole, 12);
		break;
	}
#else

#endif
}

void Logger::ResetConsoleColor()
{
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);
#else

#endif
}