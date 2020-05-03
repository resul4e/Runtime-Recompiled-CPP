#pragma once
#include <string>

#include <unordered_map>

#include "ForwardDecl.h"

#include "spdlog/spdlog.h"
#include "Handle.h"
#include "LoggerException.h"

#if defined(WIN32) || defined(__WIN32)
#define LOG_TRACE(console,message, ...) Logger::Trace(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_DEBUG(console,message, ...) Logger::Debug(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_INFO(console,message, ...) Logger::Info(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_WARN(console,message, ...) Logger::Warn(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_ERROR(console,message, ...) Logger::Error(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#define LOG_CRITICAL(console,message, ...) Logger::Critical(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, __VA_ARGS__);
#else
#define LOG_TRACE(console,message, args...) Logger::Trace(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, ##args);
#define LOG_DEBUG(console,message, args...) Logger::Debug(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, ##args);
#define LOG_INFO(console,message, args...) Logger::Info(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, ##args);
#define LOG_WARN(console,message, args...) Logger::Warn(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, ##args);
#define LOG_ERROR(console,message, args...) Logger::Error(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, ##args);
#define LOG_CRITICAL(console,message, args...) Logger::Critical(console, (std::string("{}({}) ") + std::string(message)).c_str(), __FILE__, __LINE__, ##args);
#endif

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
	 * \brief When the logger will throw an exception when logging a message with a particular severity.
	 */
	enum class ExceptionThreshold
	{
		NEVER,					///< Never throw an exception
		WARN_AND_ABOVE,			///< Throw an exception when logging a warning, error, or critical message.
		ERROR_AND_ABOVE,		///< Throw an exception when logging an error or critical message.
		CRITICAL_ONLY			///< Throw an exception only when logging a critical message.
	};
	
	/**
	 * \brief Creates a new logger with a specified name.
	 * \note All scripts have a logger called "script". This can be accessed by the Get() method.
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
	 * \brief Returns a handle to the requested logger either by getting it from the list of existing ones or creating a new one.
	 * \param aConsoleName The name to look for or the name the new logger will have.
	 * \return The handle to the logger.
	 */
	CORE_API static LoggerHandle AddOrGet(const char* aConsoleName);
	
	/**
	 * \brief Sets the threshold after which an exception will be thrown.
	 * \param aThreshold The new threshold.
	 */
	CORE_API static void SetExceptionThreshold(ExceptionThreshold aThreshold);

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

	/**
	 * \brief Throws a LoggerException
	 * \param aMessage A custom message that is returned as the "what()" of the exception.
	 */
	CORE_API static void ThrowException(const std::string& aMessage = "Logger threw an exception. See message for more information!");
	
//variables
public:

private:
	
	///the list of logger handles with the name as the key.
	CORE_API static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> loggerHandleList;

	///The threshold after which an exception will be thrown.
	CORE_API static ExceptionThreshold threshold;
};

template <typename ... Args>
void Logger::Trace(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.name)->trace(aMessage, std::forward<Args>(aVariables)...);
}

template <typename ... Args>
void Logger::Debug(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.name)->debug(aMessage, std::forward<Args>(aVariables)...);
}

template <typename ... Args>
void Logger::Info(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.name)->info(aMessage, std::forward<Args>(aVariables)...);
}

template <typename ... Args>
void Logger::Warn(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.name)->warn(aMessage, std::forward<Args>(aVariables)...);
	
	if(threshold >= ExceptionThreshold::WARN_AND_ABOVE)
	{
		ThrowException();
	}
}

template <typename ... Args>
void Logger::Error(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.name)->error(aMessage, std::forward<Args>(aVariables)...);

	if (threshold >= ExceptionThreshold::ERROR_AND_ABOVE)
	{
		ThrowException();
	}
}

template <typename ... Args>
void Logger::Critical(LoggerHandle aHandle, const char* aMessage, Args&&... aVariables)
{
	loggerHandleList.at(aHandle.name)->critical(aMessage, std::forward<Args>(aVariables)...);

	if (threshold == ExceptionThreshold::CRITICAL_ONLY)
	{
		ThrowException();
	}
}

