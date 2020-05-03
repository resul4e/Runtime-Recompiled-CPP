#pragma once
#include <stdexcept>

class LoggerException : public std::runtime_error
{
public:
	explicit LoggerException(const std::string& _Message) : runtime_error(_Message.c_str()) {}

	explicit LoggerException(const char* _Message) : runtime_error(_Message) {}
};
