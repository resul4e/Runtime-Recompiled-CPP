#include "RunPython.h"
#include "ProcessFunctions.h"
#include "Logger.h"

constexpr uint32_t bufferSize = 2048;

#if defined(WIN32) || defined(__WIN32)
	constexpr char pythonRunCommand[] = "py ";
#else
	constexpr char pythonRunCommand[] = "python3 ";
#endif

bool RunPython::Run(const std::string& aCommand)
{
	//try to run the python command.
	FILE* in = Start(aCommand);
	if (in == nullptr)
	{
		return false;
	}

	//Log all of the messages we get from the python script.
	char buff[bufferSize];
	while (fgets(buff, sizeof(buff), in) != nullptr)
	{
		LOG_INFO(Logger::Get("core"), buff);
	}

	//If we have noticed an error while trying to run the command return false otherwise true.
	return Stop(in);
}

FILE* RunPython::Start(const std::string& aCommand)
{
	//add the correct python run command at the start of the command.
	std::string setupCommand = std::string(pythonRunCommand) + aCommand;

	//Try to open the process and return the process pointer we get back.
	FILE* in;
	in = OPEN_SOME_PROCESS(setupCommand.c_str(), "r");
	return in;
}

bool RunPython::GetLine(FILE* aFile, std::string& oLine)
{
	//Read the next line in the buffer and save it in oLine.
	char buff[bufferSize];
	bool output = fgets(buff, sizeof(buff), aFile) != nullptr;
	if(output)
	{
		oLine = buff;
	}
	//Return wether we successfully read the next line.
	return output;
}

bool RunPython::Stop(FILE* aFile)
{
	//Close the process and check if there were any problems.
	if (CLOSE_SOME_PROCESS(aFile) != 0)
	{
		return false;
	}
	return true;
}
