#include "SharedLibrary.h"

//define the platform dependant function that loads the library
#if defined(WIN32) || defined(__WIN32)
#define LOAD_LIBRARY_FUNC(libraryPath) LoadLibraryA(libraryPath)
#define FREE_SHARED_LIBRARY(aHandle) FreeLibrary(aHandle)
#define GET_LAST_ERROR_FUNCTION() std::to_string(GetLastError())
#elif __unix__
#define LOAD_LIBRARY_FUNC(libraryPath) dlopen(libraryPath, RTLD_NOW);
#define FREE_SHARED_LIBRARY(aHandle) dlclose(aHandle)
#define GET_LAST_ERROR_FUNCTION() std::string(dlerror())
#endif

SharedLibrary::SharedLibrary(const RCP::fs::path& aRootDirectory):
handle(nullptr),
rootDirectory(aRootDirectory)
{
}

SharedLibrary::~SharedLibrary()
{
	UnloadSharedLibrary();
}

bool SharedLibrary::LoadSharedLibrary(const std::string& aSharedLibraryPath)
{
	//reset the error
	lastError = std::string();
	
	//create full path to library.
	RCP::fs::path libraryPath = rootDirectory;
	libraryPath /= aSharedLibraryPath;

	//replace the current extension with the correct one (regardless if the old one was correct)
	if(libraryPath.has_extension())
	{
		libraryPath.replace_extension(sharedLibraryExtension);
	}
	else
	{
		libraryPath += sharedLibraryExtension;
	}

	//add the shared library prefix to the filename.
	libraryPath.replace_filename(std::string(SharedLibrary::sharedLibraryPrefix) + libraryPath.filename().string());

	//Load the library and store the handle.
	handle = LOAD_LIBRARY_FUNC(libraryPath.string().c_str());
	if(handle == nullptr)
	{
		lastError = GET_LAST_ERROR_FUNCTION();
		return false;
	}
	return true;
}

void SharedLibrary::UnloadSharedLibrary()
{
	if (handle != nullptr)
	{
		FREE_SHARED_LIBRARY(handle);
		handle = nullptr;
	}
}

SharedLibHandle SharedLibrary::GetSharedLibraryHandle()
{
	return handle;
}

std::string SharedLibrary::GetLoadingError()
{
	return lastError;
}
