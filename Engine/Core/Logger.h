#pragma once
#include <string>

#include <unordered_map>

#include "ForwardDecl.h"

#include "spdlog/spdlog.h"
#include "Handle.h"

#define LOG_TRACE(console,message, ...) Logger::Trace(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_DEBUG(console,message, ...) Logger::Debug(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_INFO(console,message, ...) Logger::Info(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_WARN(console,message, ...) Logger::Warn(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(console,message, ...) Logger::Error(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_CRITICAL(console,message, ...) Logger::Critical(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);

/**
 * \brief Class that makes it possible to log messages easily.
 * Usage:
 * - Using a macro
 *		\code{.cpp}
 *		LOG_DEBUG(console, "Position on screen X:{} and Y:{}", XPos, YPos);
 *		\endcode
 *	The macro will append the file name and number of itself automatically.
 *	
 *	-Using Logger
 *		\code{.cpp}
 *		Logger::Warn(Logger::Get("Core"), "An error occurred in script {} at line {} with message: {}", scriptName, lineNbr, msg);
 *		\endcode
 */
class Logger
{
	Logger() {};
	~Logger(){};
public:
	/**
	 * \brief Creates a new logger with a specified name.
	 * \note All scripts have a logger called "script". This can be accessed by the Get method.
	 * \param aConsoleName the new logger's name.
	 * \return a handle to the logger.
	 * \see Get()
	 */
	CORE_API static LoggerHandle Add(const char* aConsoleName);
	/**
	 * \brief Retrieves the handle of the specified logger.
	 * \param aConsoleName The name of the logger you went to retrieve.
	 * \return the handle of the logger.
	 */
	CORE_API static LoggerHandle Get(const char* aConsoleName);

	/**
	 * \brief A Message in a teal color. With the category Trace.
	 * \param aHandle The LoggerHandle, the name of the LoggerHandle will show in the message, so it is easier to find where the message came from
	 * \param aMessage The message you want to send with "{}" substituted for the parameters.
	 * \param aVariables All of the parameters you want to supply. They will be substituted in the order they are specified.
	 */
	template<typename... Args> static void Trace(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables);
	/**
	* \brief A Message in a teal color. With the category Debug.
	* \param aHandle The LoggerHandle, the name of the LoggerHandle will show in the message, so it is easier to find where the message came from
	* \param aMessage The message you want to send with "{}" substituted for the parameters.
	* \param aVariables All of the parameters you want to supply. They will be substituted in the order they are specified.
	*/
	template<typename... Args> static void Debug(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables);
	/**
	* \brief A Message in a white color. With the category Info.
	* \param aHandle The LoggerHandle, the name of the LoggerHandle will show in the message, so it is easier to find where the message came from
	* \param aMessage The message you want to send with "{}" substituted for the parameters.
	* \param aVariables All of the parameters you want to supply. They will be substituted in the order they are specified.
	*/
	template<typename... Args> static void Info(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables);
	/**
	* \brief A Message in a Yellow color. With the category Warning.
	* \param aHandle The LoggerHandle, the name of the LoggerHandle will show in the message, so it is easier to find where the message came from
	* \param aMessage The message you want to send with "{}" substituted for the parameters.
	* \param aVariables All of the parameters you want to supply. They will be substituted in the order they are specified.
	*/
	template<typename... Args> static void Warn(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables);
	/**
	* \brief A Message in a red color. With the category Error.
	* \param aHandle The LoggerHandle, the name of the LoggerHandle will show in the message, so it is easier to find where the message came from
	* \param aMessage The message you want to send with "{}" substituted for the parameters.
	* \param aVariables All of the parameters you want to supply. They will be substituted in the order they are specified.
	*/
	template<typename... Args> static void Error(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables);
	/**
	* \brief A Message in a white color with a red background. With the category Ciritcal.
	* \param aHandle The LoggerHandle, the name of the LoggerHandle will show in the message, so it is easier to find where the message came from
	* \param aMessage The message you want to send with "{}" substituted for the parameters.
	* \param aVariables All of the parameters you want to supply. They will be substituted in the order they are specified.
	*/
	template<typename... Args> static void Critical(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables);
private:

//variables
public:

private:
	///the list of logger handles with the name as the key.
	CORE_API static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> loggerHandleList;
};

template <typename ... Args>
void Logger::Trace(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.index)->trace(aMessage, std::forward<Args>(aVariables)...);
}

template <typename ... Args>
void Logger::Debug(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.index)->debug(aMessage, std::forward<Args>(aVariables)...);
}

template <typename ... Args>
void Logger::Info(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.index)->info(aMessage, std::forward<Args>(aVariables)...);
}

template <typename ... Args>
void Logger::Warn(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.index)->warn(aMessage, std::forward<Args>(aVariables)...);
}

template <typename ... Args>
void Logger::Error(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.index)->error(aMessage, std::forward<Args>(aVariables)...);
}

template <typename ... Args>
void Logger::Critical(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.index)->critical(aMessage, std::forward<Args>(aVariables)...);
}

