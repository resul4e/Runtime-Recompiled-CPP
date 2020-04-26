project(PIL VERSION 1.0.0)

add_library("PIL" 
    "FileSystem.h"
    "FunctionDefinition.h"
    "SizeT.h"
    "SharedLibrary.h"
    "SharedLibrary.cpp")

target_link_libraries("PIL" "EngineBase")