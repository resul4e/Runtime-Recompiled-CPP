#include "RunPython.h"
#include "ProcessFunctions.h"

#if defined(WIN32) || defined(__WIN32)
	constexpr char pythonRunCommand[] = "py ";
#else
	constexpr char pythonRunCommand[] = "python3 ";
#endif

bool RunPython::Run(std::string aCommand)
{
	std::string setupCommand = std::string(pythonRunCommand) + aCommand;

	FILE* in;
	if ((in = OPEN_SOME_PROCESS(setupCommand.c_str(), "r")) == nullptr)
	{
		//TODO(Resul): Investigate how we can actually check if a process has run successfully.
		return false;
	}

	CLOSE_SOME_PROCESS(in);
	return true;
}