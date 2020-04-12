#include "stdafx.h"
#include "Handle.h"
#include "Logger.h"

using namespace spdlog;

CORE_API std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::loggerHandleList;

extern "C" CORE_API LoggerHandle Logger::Add(const char* aConsoleName)
{
	loggerHandleList.insert(std::pair<const char*, std::shared_ptr<spdlog::logger>>(aConsoleName,{ stdout_color_mt(aConsoleName)}));
	spdlog::set_level(spdlog::level::trace);
	return { aConsoleName };
}

extern "C" CORE_API LoggerHandle Logger::Get(const char* aConsoleName)
{
	if(loggerHandleList.find(aConsoleName) == loggerHandleList.end())
	{
		LOG_ERROR(Logger::Get("core"), "No logger found with the name {}. Returning the default LoggerHandle", aConsoleName);
		return { "core" };
	}

	return { aConsoleName };
}
