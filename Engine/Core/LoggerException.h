#pragma once
#include <stdexcept>

/**
 * \brief An exception thrown by Logger if a severe enough message has been logged.
 */
class LoggerException : public std::runtime_error
{
public:
	explicit LoggerException(const std::string& _Message) : runtime_error(_Message.c_str()) {}

	explicit LoggerException(const char* _Message) : runtime_error(_Message) {}
};
