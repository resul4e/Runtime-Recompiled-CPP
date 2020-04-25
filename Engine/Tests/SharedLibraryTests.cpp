#include <gtest/gtest.h>
#include "SharedLibrary.h"
#include "FunctionDefinition.h"

std::string rootDirectory = TEST_DATA_DIR;
typedef int (FUNCTION_CDECL *TEST_FUNCTION)(int);

TEST(SharedLibraryTests, Constructor)
{
    EXPECT_NO_FATAL_FAILURE(SharedLibrary lib("SOME_DIRECTORY"););
}

TEST(SharedLibraryTests, LoadSharedLibraryWhereFileDoesNotExist)
{
    SharedLibrary lib(rootDirectory);
	std::string libraryPath = std::string("DOESNOTEXIST");
	EXPECT_FALSE(lib.LoadSharedLibrary(libraryPath));
}

TEST(SharedLibraryTests, LoadSharedLibraryWhereFileDoesExist)
{
	SharedLibrary lib(rootDirectory);
	std::string libraryPath = std::string("SharedLibrary/SharedLibraryTest64");
	EXPECT_TRUE(lib.LoadSharedLibrary(libraryPath));
}

TEST(SharedLibraryTests, LoadSharedLibraryWithLinuxExtension)
{
	SharedLibrary lib(rootDirectory);
	std::string libraryPath = std::string("SharedLibrary/SharedLibraryTest64.so");
	EXPECT_TRUE(lib.LoadSharedLibrary(libraryPath));
}

TEST(SharedLibraryTests, LoadSharedLibraryWithWindowsExtension)
{
	SharedLibrary lib(rootDirectory);
	std::string libraryPath = std::string("SharedLibrary/SharedLibraryTest64.dll");
	EXPECT_TRUE(lib.LoadSharedLibrary(libraryPath));
}

TEST(SharedLibraryTests, GetExportedFunctionWhereFunctionDoesNotExist)
{
	SharedLibrary lib(rootDirectory);
	std::string libraryPath = std::string("SharedLibrary/SharedLibraryTest64.dll");
	lib.LoadSharedLibrary(libraryPath);
	EXPECT_EQ(lib.GetExportedFunction<TEST_FUNCTION>("NON_EXISTING_FUNCTION_NAME"), nullptr);
}

TEST(SharedLibraryTests, GetExportedFunctionWhereLibraryNameIsEmpty)
{
	SharedLibrary lib(rootDirectory);
	std::string libraryPath = std::string("");
	lib.LoadSharedLibrary(libraryPath);
	EXPECT_EQ(lib.GetExportedFunction<TEST_FUNCTION>("NON_EXISTING_FUNCTION_NAME"), nullptr);
}

TEST(SharedLibraryTests, GetExportedFunctionWhereFunctionDoesExist)
{
	SharedLibrary lib(rootDirectory);
	std::string libraryPath = std::string("SharedLibrary/SharedLibraryTest64.dll");
	lib.LoadSharedLibrary(libraryPath);
	TEST_FUNCTION TestFunction = lib.GetExportedFunction<TEST_FUNCTION>("TestFunction");
	EXPECT_NE(TestFunction, nullptr);
	//Function that is retrieved adds 10 to the passed value, so it should be equal to 14
	EXPECT_EQ(TestFunction(4), 14);
}

TEST(SharedLibraryTests, GetSharedLibraryHandleWhereLibraryDoesExist)
{
	SharedLibrary lib(rootDirectory);
	std::string libraryPath = std::string("SharedLibrary/SharedLibraryTest64.dll");
	lib.LoadSharedLibrary(libraryPath);
	SharedLibHandle handle = lib.GetSharedLibraryHandle();
	EXPECT_NE(handle, nullptr);
}

TEST(SharedLibraryTests, GetSharedLibraryHandleWhereLibraryDoesNotExist)
{
	SharedLibrary lib(rootDirectory);
	std::string libraryPath = std::string("DOESNOTEXIST");
	lib.LoadSharedLibrary(libraryPath);
	SharedLibHandle handle = lib.GetSharedLibraryHandle();
	EXPECT_EQ(handle, nullptr);
}