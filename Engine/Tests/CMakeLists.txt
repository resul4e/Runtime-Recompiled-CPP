include(LoadGTest.cmake)

add_executable(Tests 
"SharedLibraryTests.cpp"
"PluginLoaderTests.cpp"
"LevelTests.cpp"
"ScriptLoaderTests.cpp"
"LoggerTests.cpp"
"CustomMacros.h"
"TestData/Level/Game/Scripts/TestScript.h")

target_compile_features(Tests INTERFACE cxx_std_17)
if(WIN32)
     #Do nothing
else()
    target_link_libraries(Tests stdc++fs)
endif()

target_link_libraries(Tests gtest_main)
target_link_libraries(Tests PIL)
target_link_libraries(Tests PluginCore)
target_link_libraries(Tests Core)

target_include_directories(Tests PRIVATE "${CMAKE_SOURCE_DIR}/Engine/PIL")
target_include_directories(Tests PRIVATE "${CMAKE_SOURCE_DIR}/Engine")
add_test(NAME Tests COMMAND Tests)

#set the root of the TestData folder where all data is located for testing purposes
target_compile_definitions("Tests" PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/TestData")
target_compile_definitions("Tests" PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")
target_compile_definitions("Tests" PRIVATE BINARY_DIR="${CMAKE_BINARY_DIR}")

#add all testing related projects into one visual studio filter
set_target_properties("gtest_main" PROPERTIES FOLDER Tests)
set_target_properties("gtest" PROPERTIES FOLDER Tests)
set_target_properties("Tests" PROPERTIES FOLDER Tests)

#make filters in visual studio for test files
source_group("TestData" FILES "TestData/Level/Game/Scripts/TestScript.h") 