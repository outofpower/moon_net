#pragma once
#include <common/singleton.hpp>
#include <spdlog/spdlog.h>
namespace moon
{
	class moon_net_log :public singleton<moon_net_log>
	{
	public:
		moon_net_log()
		{		
			m_console = spdlog::stdout_logger_mt("moon_net_console");
			m_console->set_pattern("[%Y/%m/%d %H:%M:%S] [%t] %v");
			m_file = spdlog::daily_logger_mt("moon_net_daily_logger", "moon_net", 2, 30);
		}

		std::shared_ptr<spdlog::logger>& console()
		{
			return m_console;
		}

		std::shared_ptr<spdlog::logger>& file()
		{
			return m_file;
		}

	private:
		std::shared_ptr<spdlog::logger> m_console;
		std::shared_ptr<spdlog::logger> m_file;
	};

	inline std::shared_ptr<spdlog::logger>& console()
	{
		return moon_net_log::Instance().console();
	}

	inline std::shared_ptr<spdlog::logger>& log()
	{
		return moon_net_log::Instance().file();
	}
}


