cmake_minimum_required(VERSION 3.16)

project("SharedLibraryTest")

add_library("${PROJECT_NAME}" SHARED "SharedLibraryFile.h" "SharedLibraryFile.cpp")
target_compile_definitions("${PROJECT_NAME}" PRIVATE -DSHAREDLIBRARYTESTDLL_EXPORTS)