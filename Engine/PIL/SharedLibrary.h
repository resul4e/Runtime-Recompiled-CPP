#pragma once
#include <string>
#include "FileSystem.h"

#if defined(WIN32) || defined(__WIN32)
#include <Windows.h>
#endif

//define the function that imports the function.
#if defined(WIN32) || defined(__WIN32)
typedef HINSTANCE SharedLibHandle;
#define GET_EXPORTED_FUNCTION(aHandle, aName) GetProcAddress(aHandle, aName)
#elif __unix__
typedef void* SharedLibHandle;
#define GET_EXPORTED_FUNCTION(aHandle, aName) dlsym(aHandle, aName)
#endif


/**
 * \brief A class that abstracts away the platform dependant parts of loading shared libraries.
 */
class SharedLibrary
{
public:
	//define the extension of the shared library.
#if defined(WIN32) || defined(__WIN32)
	static constexpr char* sharedLibraryExtension = ".dll";
#elif __unix__
	static constexpr char* sharedLibraryExtension = ".so";
#endif
	
public:
	
	/**
	 * \brief Create a shared library object. Use LoadSharedLibrary() to load a shared library.
	 * \param aRootDirectory The root from which shared libraries are searched for.
	 */
	SharedLibrary(const std::string& aRootDirectory);
	
	/**
	 * \brief Loads the shared 
	 * \remark The requested shared library has to be compiled with the same settings (most notably the platform configuration (x86/x64)
	 * \param aSharedLibraryPath 
	 * \return 
	 */
	bool LoadSharedLibrary(const std::string& aSharedLibraryPath);
	
	/**
	 * \brief returns the SharedLibHandle which is platform specific (HINSTANCE or void*)
	 * \return The SharedLibHandle that is loaded by LoadSharedLibrary()
	 */
	SharedLibHandle GetSharedLibraryHandle();
	
	/**
	 * \brief Gets the function contained by the shared library.
	 * \tparam Func The function blueprint e.g. bool (*)(const string&)
	 * \param aFunctionName The name of the function to retrieve
	 * \return The function if it is found or a nullptr if not.
	 */
	template<class Func>
	Func GetExportedFunction(const std::string& aFunctionName);

private:
	SharedLibHandle handle;
	std::string rootDirectory;	
};

template <class Func>
Func SharedLibrary::GetExportedFunction(const std::string& aFunctionName)
{
	return (Func)GET_EXPORTED_FUNCTION(handle, aFunctionName.c_str());
}
