cmake_minimum_required(VERSION 3.16)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

project("RRCPP")

#Set toggle for unit tests to be added to the project
set("INCLUDE_UNIT_TESTS" "FALSE" CACHE BOOL "If the project should include the engine unit tests")

#set the output directory for all target to be bin
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(WIN32)
     #Do nothing
else()
     set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()
#includes from within the project
include_directories(BEFORE "Core")
include_directories(BEFORE "PluginCore")
include_directories(BEFORE "Plugins")

#external includes
include_directories(BEFORE "Extern/spdLog/include")
include_directories(BEFORE "Extern/glm")
include_directories(BEFORE "Extern/Box2DProj/Box2D")
include_directories(BEFORE "Extern/SFML/include")
include_directories(BEFORE "PIL")

if(WIN32)
     set(SFML_STATIC_LIBRARIES TRUE)
endif()

#indicate where external dependencies can be found
if(WIN32)
     set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Extern/SFML/lib/cmake/SFML" CACHE FILEPATH "Filepath for SFML")
else()
     set(SFML_DIR "usr/lib/x86_64-linux-gnu/cmake/SFML" CACHE FILEPATH "Filepath for SFML")
endif()
find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)

if(${INCLUDE_UNIT_TESTS})
     add_subdirectory("Tests")
endif()

#base library that all other project depend on
add_library("Base" INTERFACE)

#what features we need from the compiler
target_compile_features("Base" INTERFACE cxx_attribute_deprecated)
target_compile_features("Base" INTERFACE cxx_std_17)
if(WIN32)
     #Do nothing
else()
    target_link_libraries("Base" INTERFACE stdc++fs)
    target_link_libraries("Base" INTERFACE ${CMAKE_DL_LIBS})
endif()


#base library for all engine projects
add_library("EngineBase" INTERFACE)
target_link_libraries("EngineBase" INTERFACE "Base")
target_compile_definitions("EngineBase" INTERFACE ENGINE_PROJECT)

#enable more rigorous warnings than the default.
target_compile_options(EngineBase INTERFACE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)

#the subdirectories of this project
add_subdirectory("PIL")
add_subdirectory("Main")
add_subdirectory("Core")
add_subdirectory("PluginCore")
add_subdirectory("Plugins")
