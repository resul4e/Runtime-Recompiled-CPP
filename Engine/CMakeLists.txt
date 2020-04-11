cmake_minimum_required(VERSION 3.16)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project("RRCPP")

#includes from within the project
include_directories(BEFORE "Core")
include_directories(BEFORE "PluginCore")
include_directories(BEFORE "Plugins")

#external includes
include_directories(BEFORE "Extern/spdLog/include")
include_directories(BEFORE "Extern/glm")
include_directories(BEFORE "Extern/Box2DProj/Box2D")
include_directories(BEFORE "Extern/SFML/include")

set(SFML_STATIC_LIBRARIES TRUE)

#indicate where external dependencies can be found
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Extern/SFML/lib/cmake/SFML" CACHE FILEPATH "Filepath for SFML")

#base library that all other project depend on
add_library("Base" INTERFACE)

#what features we need from the compiler
target_compile_features("Base" INTERFACE cxx_attribute_deprecated)
target_compile_features("Base" INTERFACE cxx_std_17)

#base library for all engine projects
add_library("EngineBase" INTERFACE)
target_link_libraries("EngineBase" INTERFACE "Base")
target_compile_definitions("EngineBase" INTERFACE ENGINE_PROJECT)

#the subdirectories of this project
add_subdirectory("Main")
add_subdirectory("Core")
add_subdirectory("PluginCore")
add_subdirectory("Plugins")
