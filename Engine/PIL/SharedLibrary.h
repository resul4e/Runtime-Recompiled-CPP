#pragma once
#include <string>

#if defined(WIN32) || defined(__WIN32)
#include <Windows.h>

typedef HINSTANCE SharedLibHandle;
#define SHARED_LIBRARY_EXTENSION ".dll";
#elif __unix__
typedef void* SharedLibHandle;
#define SHARED_LIBRARY_EXTENSION ".so";
#endif

class SharedLibrary
{
public:
	SharedLibrary() = default;

	bool LoadSharedLibrary(const std::string& aSharedLibraryPath);

	SharedLibHandle GetSharedLibraryHandle();
	template<class Func>
	Func GetExportedFunction(const std::string& aName);

	SharedLibHandle handle;
};

template <class Func>
Func SharedLibrary::GetExportedFunction(const std::string& aName)
{
	return nullptr;
}
