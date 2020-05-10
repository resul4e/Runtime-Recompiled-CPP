project(Core VERSION 1.0.0)

add_library("Core" SHARED
    "ExportDetails.h"
    "ForwardDecl.h"
    "Handle.h"
    "Logger.h"
    "Logger.cpp"
    "Object.h"
    "Object.cpp"
    "PlatformDetails.h"
    "dllmain.cpp"
    "Level.h" 
    "Level.cpp"
    "Script.h"
    "Script.cpp"
    "ScriptCompiler.h"
    "ScriptCompiler.cpp"
    "ScriptLoader.h"
    "ScriptLoader.cpp"
    "Storage.h"
    "Storage.cpp"
    "targetver.h"
    "ConfigDirectories.h"
    "LoggerException.h"
    "LoggerException.cpp")
target_link_libraries("Core" "EngineBase")
target_link_libraries("Core" "PIL")
target_compile_definitions("Core" PRIVATE CORE_DLL)

#move the loggerexceptions to a separate filter.
source_group("Exceptions" FILES "LoggerException.h" "LoggerException.cpp")