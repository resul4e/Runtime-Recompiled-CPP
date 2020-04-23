#include "SharedLibrary.h"

#if defined(WIN32) || defined(__WIN32)
#elif __unix__

#endif
bool SharedLibrary::LoadSharedLibrary(const std::string& aSharedLibraryPath)
{
	return false;
}
