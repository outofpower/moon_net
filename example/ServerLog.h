#pragma once
#include <spdlog/spdlog.h>
#include <common/singleton.hpp>
class ServerLog:public moon::singleton<ServerLog>
{
public:
	ServerLog()
		:m_ok(false)
	{
	}

	void Init(const std::string& logPath)
	{
		try
		{
			size_t q_size = 8192; //queue size must be power of 2
			spdlog::set_async_mode(q_size);

			m_console = spdlog::stdout_logger_mt("console");
			m_console->set_pattern("[%Y/%m/%d %H:%M:%S] [%t] %v");
			m_trace = spdlog::daily_logger_mt("daily_trace", logPath + "trace", 2, 30);
			m_trace->set_level(spdlog::level::trace);
			m_debug = spdlog::daily_logger_mt("daily_debug", logPath + "debug", 2, 30);
			m_debug->set_level(spdlog::level::debug);
			m_warning = spdlog::daily_logger_mt("daily_warning", logPath + "warn", 2, 30);
			m_warning->set_level(spdlog::level::warn);
			m_error = spdlog::daily_logger_mt("daily_error", logPath + "error", 2, 30);
			m_error->set_level(spdlog::level::err);
			m_ok = true;
		}
		catch (const spdlog::spdlog_ex& ex)
		{
			std::cout << "Log failed: " << ex.what() << std::endl;
		}	
	}

	template <typename... Args>
	void console(const char* fmt, const Args&... args)
	{
		if (!m_ok)
			return;

		m_console->info(fmt, args...);
		m_trace->info(fmt, args...);
	}

	template <typename... Args>
	void trace(const char* fmt, const Args&... args)
	{
		if (!m_ok)
			return;
		m_trace->info(fmt, args...);
	}

	template <typename... Args>
	void debug(const char* fmt, const Args&... args)
	{
		if (!m_ok)
			return;
		m_trace->debug(fmt, args...);
		m_debug->debug(fmt, args...);
	}

	template <typename... Args>
	void warning(const char* fmt, const Args&... args)
	{
		if (!m_ok)
			return;
		m_trace->warn(fmt, args...);
		m_warning->warn(fmt, args...);
	}

	template <typename... Args>
	void error(const char* fmt, const Args&... args)
	{
		if (!m_ok)
			return;
		m_trace->error(fmt, args...);
		m_error->error(fmt, args...);
	}
private:
	bool												 m_ok;
	std::shared_ptr<spdlog::logger> m_console;
	std::shared_ptr<spdlog::logger> m_trace;
	std::shared_ptr<spdlog::logger> m_debug;
	std::shared_ptr<spdlog::logger> m_warning;
	std::shared_ptr<spdlog::logger> m_error;
};

#define Log ServerLog::Instance()
