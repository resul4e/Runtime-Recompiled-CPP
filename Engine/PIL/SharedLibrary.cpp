#include "SharedLibrary.h"

//define the platform dependant function that loads the library
#if defined(WIN32) || defined(__WIN32)
#define LoadLibraryFunc(libraryPath) LoadLibraryA(libraryPath);
#elif __unix__
#define LoadLibraryFunc(libraryPath) dlopen(libraryPath, RTLD_NOW);
#endif

SharedLibrary::SharedLibrary(const std::string& aRootDirectory):
handle(nullptr),
rootDirectory(aRootDirectory)
{
}

bool SharedLibrary::LoadSharedLibrary(const std::string& aSharedLibraryPath)
{
	//create full path to library.
	RCP::path libraryPath(rootDirectory);
	libraryPath /= aSharedLibraryPath;

	//replace the current extension with the correct one (regardless if the old one was correct)
	if(libraryPath.has_extension())
	{
		libraryPath.replace_extension(sharedLibraryExtension);
	}

	//Load the library and store the handle.
	handle = LoadLibraryFunc(libraryPath.string().c_str());
	if(handle == nullptr)
	{
		return false;
	}
	return true;
}

SharedLibHandle SharedLibrary::GetSharedLibraryHandle()
{
	return handle;
}
