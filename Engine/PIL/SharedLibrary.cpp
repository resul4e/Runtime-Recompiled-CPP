#include "SharedLibrary.h"
#include "FileSystem.h"
#include <iostream>

#if defined(WIN32) || defined(__WIN32)
#elif __unix__

#endif
bool SharedLibrary::LoadSharedLibrary(const std::string& aSharedLibraryPath)
{
	for (auto& p : RCP::directory_iterator("."))
		std::cout << p.path() << '\n';
	
	return false;
}
