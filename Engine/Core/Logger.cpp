#include "Handle.h"
#include "Logger.h"

using namespace spdlog;

CORE_API std::unordered_map<std::string, std::shared_ptr<logger>> Logger::loggerHandleList;
Logger::ExceptionThreshold Logger::threshold = ExceptionThreshold::NEVER;

extern "C" CORE_API LoggerHandle Logger::Add(const char* aConsoleName)
{
	if(aConsoleName == nullptr || aConsoleName[0] == '\0')
	{
		LOG_ERROR(Logger::Get("core"), "Supplied console name is not valid! We have not added the logger to the list.");
		return { "core" };
	}
	
	loggerHandleList.insert(std::pair<const char*, std::shared_ptr<logger>>(aConsoleName,{ stdout_color_mt(aConsoleName)}));
	set_level(spdlog::level::trace);
	return { aConsoleName };
}

extern "C" CORE_API LoggerHandle Logger::Get(const char* aConsoleName)
{
	if(aConsoleName == nullptr)
	{
		LOG_ERROR(Logger::Get("core"),"aConsoleName is a nullptr");
		return { "core" };
	}
	
	if(loggerHandleList.find(aConsoleName) == loggerHandleList.end())
	{
		LOG_ERROR(Logger::Get("core"), "No logger found with the name {}. Returning the default LoggerHandle", aConsoleName);
		return { "core" };
	}

	return { aConsoleName };
}

LoggerHandle Logger::AddOrGet(const char* aConsoleName)
{
	if (aConsoleName == nullptr)
	{
		LOG_ERROR(Logger::Get("core"), "aConsoleName is a nullptr");
		return { "core" };
	}

	if (loggerHandleList.find(aConsoleName) == loggerHandleList.end())
	{
		return Add(aConsoleName);
	}
	return Get(aConsoleName);
}

void Logger::SetExceptionThreshold(ExceptionThreshold aThreshold)
{
	threshold = aThreshold;
}

void Logger::ThrowException(const std::string& aMessage)
{
	throw LoggerException(aMessage);
}
