#pragma once

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <common/singleton.hpp>

#if _DEBUG
#include <log4cplus/helpers/loglog.h>
//#pragma comment(lib,"log4cplusD.lib")
#else
//#pragma comment(lib,"log4cplus.lib")
#endif

#define LOG_BUF_SIZE 10240


class Log :public moon::singleton<Log>
{
public:
	Log(void)
	{
		log4cplus::initialize();
#if _DEBUG
		log4cplus::helpers::LogLog::getLogLog()->setInternalDebugging(true);
#endif
		log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("urconfig.properties"));
		trace = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TRACE_MSGS"));
		debug = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("DEBUG_INFO_MSGS"));
		fatal = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("FATAL_MSGS"));
		console = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("CONSOLE_MSGS"));
	}

	~Log(void)
	{
		log4cplus::Logger::shutdown();
	}

	void Log_Console(const char* fmt, ...)
	{
		char buf[LOG_BUF_SIZE] = { 0 };
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
		va_end(ap);
		LOG4CPLUS_INFO(console, buf);
	}

	void Log_Trace(const char* fmt, ...)
	{
		char buf[LOG_BUF_SIZE] = { 0 };
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
		va_end(ap);
		LOG4CPLUS_TRACE(console, buf);
	}

	void Log_Fatal(const char* fmt, ...)
	{
		char buf[LOG_BUF_SIZE] = { 0 };
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
		va_end(ap);
		LOG4CPLUS_FATAL(console, buf);
	}

	void Log_Debug(const char* fmt, ...)
	{
#if _DEBUG
		char buf[LOG_BUF_SIZE] = { 0 };
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
		va_end(ap);
		LOG4CPLUS_DEBUG(console, buf);
#endif
	}

	log4cplus::Logger trace;
	log4cplus::Logger debug;
	log4cplus::Logger fatal;
	log4cplus::Logger console;
};

#define LOG_CONSOLE(str,...)	 Log::Instance().Log_Console(str,##__VA_ARGS__);
#define LOG_DEBUG(str,...)		 Log::Instance().Log_Debug(str,##__VA_ARGS__);
#define LOG_TRACE(str,...)		 Log::Instance().Log_Trace(str,##__VA_ARGS__);
#define LOG_FATAL(str,...)		 Log::Instance().Log_Fatal(str,##__VA_ARGS__);
