#include <gtest/gtest.h>
#include "SharedLibrary.h"

TEST(SharedLibraryTests, Constructor)
{
    EXPECT_NO_FATAL_FAILURE(SharedLibrary lib;);
}

TEST(SharedLibraryTests, LoadSharedLibraryWhereFileDoesNotExist)
{
    SharedLibrary lib;
	std::string libraryPath = std::string("DOESNOTEXIST.") + SHARED_LIBRARY_EXTENSION;
	EXPECT_FALSE(lib.LoadSharedLibrary(libraryPath));
}