#pragma once
#include <string>

class RunPython
{
public:
	RunPython() = delete;
	~RunPython() = delete;
	
	static bool Run(std::string aCommand);
};
