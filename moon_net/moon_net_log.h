#pragma once
#include <spdlog/spdlog.h>

namespace moon
{
	class moon_net_log :noncopyable
	{
	public:
		static moon_net_log& Instance()
		{
			static moon_net_log tmp;
			return tmp;
		}

		template <typename... Args>
		void console(const char* fmt, const Args&... args)
		{
			if (!m_bLog)
				return;
			m_console->info(fmt, args...);
		}

		template <typename... Args>
		void log(const char* fmt, const Args&... args)
		{
			if (!m_bLog)
				return;
			m_file->info(fmt, args...);
		}

		void set_write_log(bool bWrite)
		{
			m_bLog = bWrite;
		}
	private:
		moon_net_log()
		{
			m_bLog = false;
			m_console = spdlog::stdout_logger_mt("moon_net_console");
			m_console->set_pattern("[%Y/%m/%d %H:%M:%S] [%t] %v");
			m_file = spdlog::daily_logger_mt("moon_net_daily_logger", "moon_net", 2, 30);
		}

		std::shared_ptr<spdlog::logger> m_console;
		std::shared_ptr<spdlog::logger> m_file;
		bool												m_bLog;
	};

#define NET_LOG moon_net_log::Instance()
}


