project(PIL VERSION 1.0.0)

add_library("PIL" 
    "FileSystem.h"
    "FunctionDefinition.h"
    "ProcessFunctions.h"
    "SizeT.h"
    "SharedLibrary.h"
    "SharedLibrary.cpp"
    "RunPython.h"
    "RunPython.cpp")

target_link_libraries("PIL" "EngineBase")