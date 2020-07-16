#pragma once
#include <string>

class RunPython
{
public:
	RunPython() = delete;
	~RunPython() = delete;
	
	static bool Run(const std::string& aCommand);
	static FILE* Start(const std::string& aCommand);
	static bool GetLine(FILE* aFile, std::string& oLine);
	static bool Stop(FILE* aFile);
};
