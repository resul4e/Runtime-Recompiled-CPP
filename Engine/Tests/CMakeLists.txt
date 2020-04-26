#from https://github.com/google/googletest/tree/master/googletest#incorporating-into-an-existing-cmake-project

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()


add_executable(Tests 
"SharedLibraryTests.cpp"
"PluginLoaderTests.cpp")

target_link_libraries(Tests gtest_main)
target_link_libraries(Tests PIL)
target_link_libraries(Tests PluginCore)
target_link_libraries(Tests Core)

target_include_directories(Tests PRIVATE "${CMAKE_SOURCE_DIR}/Engine/PIL")
add_test(NAME Tests COMMAND Tests)

#set the root of the TestData folder where all data is located for testing purposes
target_compile_definitions("Tests" PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/TestData")
target_compile_definitions("Tests" PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")
target_compile_definitions("Tests" PRIVATE BINARY_DIR="${CMAKE_BINARY_DIR}")

#add all testing related projects into one visual studio filter
set_target_properties("gtest_main" PROPERTIES FOLDER Tests)
set_target_properties("gtest" PROPERTIES FOLDER Tests)
set_target_properties("Tests" PROPERTIES FOLDER Tests)