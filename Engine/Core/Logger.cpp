#include "Handle.h"
#include "Logger.h"

using namespace spdlog;

CORE_API std::unordered_map<std::string, LoggerData> Logger::loggerHandleList;

extern "C" CORE_API LoggerHandle Logger::Add(const char* aConsoleName)
{
	if(aConsoleName == nullptr || aConsoleName[0] == '\0')
	{
		LOG_ERROR(Logger::Get("core"), "Supplied console name is not valid! We have not added the logger to the list.");
		return { "core" };
	}

	//TODO(Resul): Change indexing from string to int, and store name in LoggerData.
	loggerHandleList.insert({aConsoleName, { stdout_color_mt(aConsoleName), Logger::ExceptionThreshold::NEVER}});
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
	
	if(!Find(aConsoleName))
	{
		//make sure we don't end in an endless loop if "core" is not defined. This is only really the case when unit testing.
		assert(strcmp(aConsoleName, "core") != 0);
		
		LOG_ERROR(Logger::Get("core"), "No logger found with the name {}. Returning the default LoggerHandle", aConsoleName);
		return { "core" };
	}

	return { aConsoleName };
}

bool Logger::Find(const char* aConsoleName)
{
	return loggerHandleList.find(aConsoleName) != loggerHandleList.end();
}

bool Logger::Find(const char* aConsoleName, LoggerHandle& oHandle)
{
	if (!Find(aConsoleName))
	{
		return false;
	}
	oHandle = Get(aConsoleName);
	return true;
}

LoggerHandle Logger::AddOrGet(const char* aConsoleName)
{
	if (aConsoleName == nullptr)
	{
		LOG_ERROR(Logger::Get("core"), "aConsoleName is a nullptr");
		return { "core" };
	}

	if (!Find(aConsoleName))
	{
		return Add(aConsoleName);
	}
	return Get(aConsoleName);
}

void Logger::Remove(LoggerHandle aHandle)
{
	spdlog::details::registry::instance().drop(aHandle.name);
	loggerHandleList.at(aHandle.name).logger.reset();
	loggerHandleList.erase(aHandle.name);
}

void Logger::SetExceptionThreshold(LoggerHandle aHandle, ExceptionThreshold aThreshold)
{
	loggerHandleList.at(aHandle.name).threshold = aThreshold;
}

void Logger::ThrowException(const std::string& aMessage)
{
	throw LoggerException(aMessage);
}
